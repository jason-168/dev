/*
 * sl_ringbuffer.h
 *
 *  Created on: Jul 24, 2015
 *      Author: root
 */

#ifndef SL_RINGBUFFER_H_
#define SL_RINGBUFFER_H_

#include <stdint.h>
#include <base/sl.h>

SL_BEGIN_DECLS

typedef struct sl_ringbuffer{
	char*		_buffer;
	uint32_t	_capacity;

	uint32_t	_in;
	uint32_t	_out;
}sl_ringbuffer;

sl_ringbuffer* sl_ringbuffer_create(uint32_t capacity);
void sl_ringbuffer_destroy(sl_ringbuffer* ringbuffer);

sl_status_t sl_ringbuffer_moveReadPosition(sl_ringbuffer* ringbuffer, int32_t size);

uint32_t sl_ringbuffer_get(sl_ringbuffer* ringbuffer, char* buffer, uint32_t size);
uint32_t sl_ringbuffer_put(sl_ringbuffer* ringbuffer, const char* buffer, uint32_t size);

uint32_t sl_ringbuffer_size(sl_ringbuffer* ringbuffer);
uint32_t sl_ringbuffer_capacity(sl_ringbuffer* ringbuffer);

SL_END_DECLS

#endif /* SL_RINGBUFFER_H_ */
