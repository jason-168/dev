/*
 * sl_channel_buffer.h
 *
 *  Created on: May 5, 2016
 *      Author: root
 */

#ifndef SL_CHANNEL_BUFFER_H_
#define SL_CHANNEL_BUFFER_H_

#include <stdint.h>
#include <base/sl.h>

SL_BEGIN_DECLS

typedef void* sl_audiobuffer_t;
typedef void* sl_videobuffer_t;

/**
 * 把音频送入缓冲区
 * @param[in] abuffer 音频缓冲区句柄
 * @param[in] data 音频数据
 * @param[in] len 音频数据长度
 *
 * @return 0 成功, 负数则失败.
 */
sl_status_t sl_avbuffer_putAudio(sl_audiobuffer_t abuffer,
		const void *data, int len);

/**
 * 把视频送入缓冲区
 * @param[in] vbuffer 视频缓冲区句柄
 * @param[in] data 视频数据
 * @param[in] len 视频数据长度
 *
 * @return 0 成功, 负数则失败.
 */
sl_status_t sl_avbuffer_putVideo(sl_videobuffer_t vbuffer,
		int frametype, const void *data, int len);

SL_END_DECLS

#endif /* SL_CHANNEL_BUFFER_H_ */
