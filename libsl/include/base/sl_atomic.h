/*
 * sl_atomic.h
 *
 *  Created on: May 6, 2016
 *      Author: root
 */

#ifndef SL_ATOMIC_H_
#define SL_ATOMIC_H_

#include <stdint.h>
#include <base/sl.h>

SL_BEGIN_DECLS

/**
 * atomically add 'val' to an int32_t
 * @param mem pointer to the object
 * @param val amount to add
 * @return old value pointed to by mem
 */
int32_t sl_atomic_add(volatile int32_t *mem, int32_t val);

/**
 * atomically subtract 'val' from an int32_t
 * @param mem pointer to the object
 * @param val amount to subtract
 * @return new value pointed to by mem
 */
int32_t sl_atomic_sub(volatile int32_t *mem, int32_t val);

/**
 * atomically increment an int32_t by 1
 * @param mem pointer to the object
 * @return old value pointed to by mem
 */
int32_t sl_atomic_inc(volatile int32_t *mem);

/**
 * atomically decrement an int32_t by 1
 * @param mem pointer to the atomic value
 * @return zero if the value becomes zero on decrement, otherwise non-zero
 */
int32_t sl_atomic_dec(volatile int32_t *mem);

/**
 * exchange int32_t's value with 'val'.
 * @param mem pointer to the value
 * @param val what to swap it with
 * @return the old value of *mem
 */
int32_t sl_atomic_xchg(volatile int32_t *mem, int32_t val);

SL_END_DECLS

#endif /* SL_ATOMIC_H_ */
