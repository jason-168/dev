/*
 * sl_storage_datasource.h
 *
 *  Created on: Aug 5, 2016
 *      Author: root
 */

#ifndef SL_STORAGE_DATASOURCE_H_
#define SL_STORAGE_DATASOURCE_H_

#include <stdint.h>

#include <base/sl.h>
#include <sl/sl_protocol.h>

SL_BEGIN_DECLS

typedef struct sl_storage_datasource_listener{
	void (*on_stream_info)(void* userdata, int32_t result, SLAVFormat* avfmt);

	void (*on_time_info)(void* userdata, int32_t start_ts, int32_t duration);//sec

	void (*incoming_audio)(void* userdata, const void* data, int length,
			uint32_t seq, int64_t timestamp);
	void (*incoming_video)(void* userdata, const void* data, int length,
			int frametype, uint32_t seq, int64_t timestamp);

	void (*on_pull_size)(void* userdata, int32_t seek_sec, int32_t datasize, int32_t isEOF);
}sl_storage_datasource_listener;

typedef struct sl_storage_datasource sl_storage_datasource_t;

sl_storage_datasource_t* sl_storage_datasource_create(
		sl_storage_datasource_listener* l, void* userdata, const char* file, int32_t storage_type);
void sl_storage_datasource_autorelease(sl_storage_datasource_t **datasource);

sl_status_t sl_storage_datasource_start(sl_storage_datasource_t* datasource);
void sl_storage_datasource_stop(sl_storage_datasource_t* datasource);

sl_status_t sl_storage_datasource_pull(sl_storage_datasource_t* datasource,
		int32_t seek_sec, int32_t size);

SL_END_DECLS

#endif /* SL_STORAGE_DATASOURCE_H_ */
