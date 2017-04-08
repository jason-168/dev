/*
 * sl_cond.h
 *
 *  Created on: Nov 29, 2015
 *      Author: root
 */

#ifndef SL_COND_H_
#define SL_COND_H_

#include <stdint.h>
#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

/** Opaque structure for thread condition variables */
typedef struct sl_cond sl_cond_t;

/**
 * Create and initialize a condition variable that can be used to signal
 * and schedule threads in a single process.
 * @param cond the memory address where the newly created condition variable
 *        will be stored.
 */
sl_status_t sl_cond_create(sl_cond_t **cond);

/**
 * Put the active calling thread to sleep until signaled to wake up. Each
 * condition variable must be associated with a mutex, and that mutex must
 * be locked before  calling this function, or the behavior will be
 * undefined. As the calling thread is put to sleep, the given mutex
 * will be simultaneously released; and as this thread wakes up the lock
 * is again simultaneously acquired.
 * @param cond the condition variable on which to block.
 * @param mutex the mutex that must be locked upon entering this function,
 *        is released while the thread is asleep, and is again acquired before
 *        returning from this function.
 * @remark Spurious wakeups may occur. Before and after every call to wait on
 * a condition variable, the caller should test whether the condition is already
 * met.
 */
sl_status_t sl_cond_wait(sl_cond_t *cond, sl_mutex_t *mutex);


#define CONDWAIT_MS(t) ((t) * 1000000LL)
#define CONDWAIT_US(t) ((t) * 1000LL)

/**
 * Put the active calling thread to sleep until signaled to wake up or
 * the timeout is reached. Each condition variable must be associated
 * with a mutex, and that mutex must be locked before calling this
 * function, or the behavior will be undefined. As the calling thread
 * is put to sleep, the given mutex will be simultaneously released;
 * and as this thread wakes up the lock is again simultaneously acquired.
 * @param cond the condition variable on which to block.
 * @param mutex the mutex that must be locked upon entering this function,
 *        is released while the thread is asleep, and is again acquired before
 *        returning from this function.
 * @param timeout The amount of time in nanoseconds to wait. This is
 *        a maximum, not a minimum. If the condition is signaled, we
 *        will wake up before this time, otherwise the error SL_TIMEUP
 *        is returned.
 */
sl_status_t sl_cond_timedwait(sl_cond_t *cond, sl_mutex_t *mutex,
                                                    int64_t timeout);

/**
 * Signals a single thread, if one exists, that is blocking on the given
 * condition variable. That thread is then scheduled to wake up and acquire
 * the associated mutex. Although it is not required, if predictable scheduling
 * is desired, that mutex must be locked while calling this function.
 * @param cond the condition variable on which to produce the signal.
 * @remark If no threads are waiting on the condition variable, nothing happens.
 */
void sl_cond_signal(sl_cond_t *cond);

/**
 * Signals all threads blocking on the given condition variable.
 * Each thread that was signaled is then scheduled to wake up and acquire
 * the associated mutex. This will happen in a serialized manner.
 * @param cond the condition variable on which to produce the broadcast.
 * @remark If no threads are waiting on the condition variable, nothing happens.
 */
void sl_cond_broadcast(sl_cond_t *cond);

/**
 * Destroy the condition variable and free the associated memory.
 * @param cond the condition variable to destroy.
 */
sl_status_t sl_cond_destroy(sl_cond_t *cond);

SL_END_DECLS

#endif /* SL_THREAD_COND_H_ */
