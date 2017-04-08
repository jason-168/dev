/*
 * sl_packet.h
 *
 *  Created on: May 5, 2016
 *      Author: root
 */

#ifndef SL_PACKET_H_
#define SL_PACKET_H_

#include <stdint.h>
#include <stddef.h>

#include <base/sl_base.h>
#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

struct sl_packet{
	SL_REF_BASE;

    int32_t _capacity;

    void *_data;

    int32_t _rangeOffset;
    int32_t _rangeLength;

    int64_t _dts;

    char _ownsData;
};

typedef struct sl_packet sl_packet;

sl_packet* sl_packet_create(int32_t capacity);
//sl_packet* sl_packet_create2(void *data, int32_t capacity);

sl_packet* sl_packet_addref(sl_packet *packet);
void sl_packet_autorelease(sl_packet **packet);

void sl_packet_autorelease_proc(sl_base **e);

static inline uint8_t *sl_packet_base(sl_packet *packet) {
	return (uint8_t *)packet->_data;
}
static inline uint8_t *sl_packet_data(sl_packet *packet) {
	return (uint8_t *)packet->_data + packet->_rangeOffset;
}
static inline int32_t sl_packet_capacity(sl_packet *packet) {
	return packet->_capacity;
}
static inline int32_t sl_packet_size(sl_packet *packet) {
	return packet->_rangeLength;
}
static inline int32_t sl_packet_offset(sl_packet *packet) {
	return packet->_rangeOffset;
}
static inline int64_t sl_packet_getdts(sl_packet *packet){
	return packet->_dts;
}
static inline void sl_packet_setdts(sl_packet *packet, int64_t dts) {
	packet->_dts = dts;
}
//static inline int64_t sl_packet_getpts(sl_packet *packet){
//	return packet->_pts;
//}
//static inline void sl_packet_setpts(sl_packet *packet, int64_t pts){
//	packet->_pts = pts;
//}

void sl_packet_setrange(sl_packet *packet, int32_t offset, int32_t size);
void sl_packet_setrange2(sl_packet *packet, int32_t offset, int32_t size);
void sl_packet_setrange3(sl_packet *packet, int32_t offset, int32_t size);

SL_END_DECLS

#endif /* SL_PACKET_H_ */
