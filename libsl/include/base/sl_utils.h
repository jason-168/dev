/*
 * sl_utils.h
 *
 *  Created on: Apr 25, 2016
 *      Author: root
 */

#ifndef SL_UTILS_H_
#define SL_UTILS_H_

#include <stdlib.h>
#include <stdint.h>

#include <base/sl.h>

#include <base/cJSON.h>


/**
 * Alignment macros
 */

/* SL_ALIGN() is only to be used to align on a power of 2 boundary */
#define SL_ALIGN(size, boundary) \
    (((size) + ((boundary) - 1)) & ~((boundary) - 1))

/** Default alignment */
#define SL_ALIGN_DEFAULT(size) SL_ALIGN(size, 8)


SL_BEGIN_DECLS


void* sl_malloc(size_t size);
void  sl_free(void* mem);

void* sl_mallocz(size_t size);

/* Use the system-supplied "random()" and "srandom()" functions */
long int sl_random();
void sl_srandom(unsigned int x);

uint32_t SL_U32_AT(const uint8_t *ptr);

uint32_t SL_U32LE_AT(const uint8_t *ptr);

int cJSON_getIntValue(cJSON* obj, int defaultValue);
double cJSON_getDoubleValue(cJSON* obj, double defaultValue);
char* cJSON_getStringValue(cJSON* obj, char* defaultValue);

//------------------------------------------------------------------

typedef struct sl_buffer{
	char* _data;
	int32_t _capacity;
}sl_buffer_t;

sl_status_t sl_buffer_init(sl_buffer_t* buffer, int32_t capacity);
void sl_buffer_deinit(sl_buffer_t* buffer);

int32_t sl_buffer_capacity(sl_buffer_t* buffer);

char* sl_buffer_base2(sl_buffer_t* buffer, int32_t expandIfNecessary);

SL_END_DECLS

#endif /* SL_UTILS_H_ */
