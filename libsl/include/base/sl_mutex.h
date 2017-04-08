/*
 * @file sl_mutex.h
 * @brief SL Thread Mutex Routines
 *
 *  Created on: Nov 26, 2015
 *      Author: root
 */

#ifndef SL_MUTEX_H_
#define SL_MUTEX_H_

#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

/** Opaque thread-local mutex structure */
typedef struct sl_mutex sl_mutex_t;

#define SL_MUTEX_DEFAULT  0x0   /**< platform-optimal lock behavior */
#define SL_MUTEX_NESTED   0x1   /**< enable nested (recursive) locks */
#define SL_MUTEX_UNNESTED 0x2   /**< disable nested locks */

/**
 * Create and initialize a mutex that can be used to synchronize threads.
 * @param mutex the memory address where the newly created mutex will be
 *        stored.
 * @param flags Or'ed value of:
 * <PRE>
 *           SL_MUTEX_DEFAULT   platform-optimal lock behavior.
 *           SL_MUTEX_NESTED    enable nested (recursive) locks.
 *           SL_MUTEX_UNNESTED  disable nested locks (non-recursive).
 * </PRE>
 * @warning Be cautious in using SL_THREAD_MUTEX_DEFAULT.  While this is the
 * most optimal mutex based on a given platform's performance characteristics,
 * it will behave as either a nested or an unnested lock.
 */
sl_status_t sl_mutex_create(sl_mutex_t **mutex, unsigned flags);
/**
 * Acquire the lock for the given mutex. If the mutex is already locked,
 * the current thread will be put to sleep until the lock becomes available.
 * @param mutex the mutex on which to acquire the lock.
 */
sl_status_t sl_mutex_lock(sl_mutex_t *mutex);

/**
 * Attempt to acquire the lock for the given mutex. If the mutex has already
 * been acquired, the call returns immediately with SL_EBUSY. Note: it
 * is important that the SL_STATUS_IS_EBUSY(s) macro be used to determine
 * if the return value was SL_EBUSY, for portability reasons.
 * @param mutex the mutex on which to attempt the lock acquiring.
 */
sl_status_t sl_mutex_trylock(sl_mutex_t *mutex);

/**
 * Release the lock for the given mutex.
 * @param mutex the mutex from which to release the lock.
 */
sl_status_t sl_mutex_unlock(sl_mutex_t *mutex);

/**
 * Destroy the mutex and free the memory associated with the lock.
 * @param mutex the mutex to destroy.
 */
sl_status_t sl_mutex_destroy(sl_mutex_t *mutex);

SL_END_DECLS

#endif /* SL_MUTEX_H_ */
