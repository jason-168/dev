/*
 * sl_base.h
 *
 *  Created on: Apr 18, 2016
 *      Author: root
 */

#ifndef SL_BASE_H_
#define SL_BASE_H_

#include <stdint.h>

#define SL_REF_BASE \
	volatile int32_t _refs

#define SL_REFBASE_INIT(obj) (obj)->_refs = 1

typedef struct sl_base{
	SL_REF_BASE;
}sl_base;

#endif /* SL_BASE_H_ */
