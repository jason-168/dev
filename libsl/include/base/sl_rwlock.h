/*
 * sl_rwlock.h
 *
 *  Created on: May 5, 2016
 *      Author: root
 */

#ifndef SL_RWLOCK_H_
#define SL_RWLOCK_H_

#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

/** Opaque read-write thread-safe lock. */
typedef struct sl_rwlock sl_rwlock_t;

/**
 * Create and initialize a read-write lock that can be used to synchronize
 * threads.
 * @param rwlock the memory address where the newly created readwrite lock
 *        will be stored.
 */
sl_status_t sl_rwlock_create(sl_rwlock_t **rwlock);

/**
 * Acquire a shared-read lock on the given read-write lock. This will allow
 * multiple threads to enter the same critical section while they have acquired
 * the read lock.
 * @param rwlock the read-write lock on which to acquire the shared read.
 */
sl_status_t sl_rwlock_rdlock(sl_rwlock_t *rwlock);

/**
 * Attempt to acquire the shared-read lock on the given read-write lock. This
 * is the same as sl_rwlock_rdlock(), only that the function fails
 * if there is another thread holding the write lock, or if there are any
 * write threads blocking on the lock. If the function fails for this case,
 * SL_EBUSY will be returned. Note: it is important that the
 * SL_STATUS_IS_EBUSY(s) macro be used to determine if the return value was
 * SL_EBUSY, for portability reasons.
 * @param rwlock the rwlock on which to attempt the shared read.
 */
sl_status_t sl_rwlock_tryrdlock(sl_rwlock_t *rwlock);

/**
 * Acquire an exclusive-write lock on the given read-write lock. This will
 * allow only one single thread to enter the critical sections. If there
 * are any threads currently holding the read-lock, this thread is put to
 * sleep until it can have exclusive access to the lock.
 * @param rwlock the read-write lock on which to acquire the exclusive write.
 */
sl_status_t sl_rwlock_wrlock(sl_rwlock_t *rwlock);

/**
 * Attempt to acquire the exclusive-write lock on the given read-write lock.
 * This is the same as sl_rwlock_wrlock(), only that the function fails
 * if there is any other thread holding the lock (for reading or writing),
 * in which case the function will return SL_EBUSY. Note: it is important
 * that the SL_STATUS_IS_EBUSY(s) macro be used to determine if the return
 * value was SL_EBUSY, for portability reasons.
 * @param rwlock the rwlock on which to attempt the exclusive write.
 */
sl_status_t sl_rwlock_trywrlock(sl_rwlock_t *rwlock);

/**
 * Release either the read or write lock currently held by the calling thread
 * associated with the given read-write lock.
 * @param rwlock the read-write lock to be released (unlocked).
 */
sl_status_t sl_rwlock_unlock(sl_rwlock_t *rwlock);

/**
 * Destroy the read-write lock and free the associated memory.
 * @param rwlock the rwlock to destroy.
 */
sl_status_t sl_rwlock_destroy(sl_rwlock_t *rwlock);

SL_END_DECLS

#endif /* SL_RWLOCK_H_ */
