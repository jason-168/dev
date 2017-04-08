/*
 * sl_time.h
 *
 *  Created on: Nov 28, 2015
 *      Author: root
 */

#ifndef SL_TIME_H_
#define SL_TIME_H_

#include <stdint.h>

#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

/**
 * Get the current time in milliseconds.
 * @return milliseconds.
 */
int64_t sl_time_ms(void);

/**
 * Get the current time in microseconds.
 * @return microseconds.
 */
int64_t sl_time_us(void);

/**
 * Sleep for a given time.  Although the duration is expressed in
 * microseconds, the actual delay may be rounded to the precision of the
 * system timer.
 *
 * @param  ms Number of microseconds to sleep.
 * @return zero on success or (negative) error code.
 */
int sl_sleep(uint32_t ms);

SL_END_DECLS

#endif /* SL_TIME_H_ */
