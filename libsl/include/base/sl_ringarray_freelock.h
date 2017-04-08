/*
 * sl_ringarray_freelock.h
 *
 *  Created on: May 5, 2016
 *      Author: root
 */

#ifndef SL_RINGARRAY_FREELOCK_H_
#define SL_RINGARRAY_FREELOCK_H_

#include <stdlib.h>
#include <string.h>
#include <base/sl.h>

#include <base/sl_utils.h>

SL_BEGIN_DECLS

/**
 * ring array free lock
 */
#define SL_RINGARRAY_DECLARE(name, type) \
typedef struct name{	\
	type *_array;				\
	uint32_t _capacity;			\
	uint32_t _rindex;			\
	uint32_t _windex;			\
	uint32_t _size;				\
	int _abort;				\
}name;					\
static inline sl_status_t name##_init(name *ring, uint32_t capacity){ \
	memset(ring, 0, sizeof(name)); \
	if(capacity > 0){ \
		void *tmp = sl_mallocz(capacity * sizeof(type));\
		ring->_array = (type *)tmp; \
	}else{ \
		ring->_array = NULL; \
	} \
	ring->_capacity = capacity; \
	ring->_abort = 0; \
	return SL_OK; \
} \
static inline type* name##_get(name *ring, int index){ \
	return &(ring->_array[index]); \
} \
static inline type* name##_writeReady(name *ring, int *wresult){ \
	if(ring->_abort){ \
		*wresult = -1; \
		return NULL; \
	} \
	if(ring->_size >= ring->_capacity){ \
		*wresult = -2; \
		return NULL; \
	} \
	*wresult = ring->_windex; \
	return &(ring->_array[ring->_windex]); \
} \
static inline void name##_writeFinish(name *ring, int wresult){ \
	if(wresult < 0){ \
		return; \
	} \
	uint32_t capacity = ring->_capacity; \
	uint32_t index = ring->_windex; \
	ring->_windex = ++index % capacity; \
	++ring->_size; \
} \
static inline type* name##_readReady(name *ring, int *rresult){ \
	if(ring->_abort){ \
		*rresult = -1; \
		return NULL; \
	} \
	if(ring->_size <= 0){ \
		*rresult = -2; \
		return NULL; \
	} \
	*rresult = ring->_rindex; \
	return &(ring->_array[ring->_rindex]); \
} \
static inline void name##_readFinish(name *ring, int rresult){ \
	if(rresult < 0){ \
		return; \
	} \
	uint32_t capacity = ring->_capacity; \
	uint32_t index = ring->_rindex; \
	ring->_rindex = ++index % capacity; \
	--ring->_size; \
} \
static inline uint32_t name##_capacity(name *ring){ \
	return ring->_capacity; \
} \
static inline uint32_t name##_size(name *ring){ \
	return ring->_size; \
} \
static inline void name##_abort(name *ring){ \
	ring->_abort = 1; \
} \
static inline void name##_release(name *ring){ \
	name##_abort(ring); \
	sl_free(ring->_array); \
}

//-----------------------------------------------------------
/**
 * ring array free lock
 */
typedef struct sl_ringarray sl_ringarray_t;

sl_ringarray_t* sl_ringarray_create(int32_t block_size, int32_t capacity);

void sl_ringarray_destroy(sl_ringarray_t *ring);

void* sl_ringarray_get(sl_ringarray_t *ring, int index);
void* sl_ringarray_writeReady(sl_ringarray_t *ring, int *wresult);
void sl_ringarray_writeFinish(sl_ringarray_t *ring, int wresult);
void* sl_ringarray_readReady(sl_ringarray_t *ring, int *rresult);
void sl_ringarray_readFinish(sl_ringarray_t *ring, int rresult);
int32_t sl_ringarray_capacity(sl_ringarray_t *ring);
int32_t sl_ringarray_size(sl_ringarray_t *ring);

SL_END_DECLS

#endif /* SL_RINGARRAY_FREELOCK_H_ */
