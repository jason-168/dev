/*
 * sl_channel.h
 *
 *  Created on: May 4, 2016
 *      Author: root
 */

#ifndef SL_CHANNEL_H_
#define SL_CHANNEL_H_

#include <stdint.h>

#include <base/sl.h>
#include <sl/sl_protocol.h>

SL_BEGIN_DECLS

typedef struct sl_channel_listener{
	void (*on_reconnecting)(void* userdata);
	void (*on_connected)(void* userdata, int mode, const char* ip, uint16_t port);
	void (*on_auth)(void* userdata, int result);
	void (*on_disconnected)(void* userdata, int errcode);

	//bitsPersecond
	void (*on_datarate)(void* userdata, int upstreamRate, int downstreamRate);

	void (*incoming_data)(void* userdata, const void* data, int length);

	void (*on_stream_res)(void* userdata, int result, SLAVFormat* avfmt);

	void (*incoming_audio)(void* userdata, const void* data, int length,
			uint32_t seq, int64_t timestamp);
	void (*incoming_video)(void* userdata, const void* data, int length,
			int frametype, uint32_t seq, int64_t timestamp);

	int (*on_videocall_req)(void *userdata, SLVideocallReq *req, SLAVFormat* avfmt_res);
	void (*on_videocall_res)(void* userdata, int result, SLAVFormat* avfmt);

	int (*on_search_req)(void* userdata, const char* data_json, int length,
			char* buffer_res, int buffer_len);
	void (*on_search_res)(void* userdata, const char* data_json, int length);

	int (*on_playback_req)(void* userdata, SLPlaybackReq* pb_req, SLPlaybackRes* pb_res);
	int (*on_playback_ctrl_req)(void* userdata, SLPlaybackCtrl cmd, int32_t value);
//	int (*on_playback_read)(void* userdata, void* buffer, int bufferLen, int* frametype, int64_t* ts);

	void (*on_playback_res)(void* userdata, SLPlaybackRes* pb_res);
	void (*on_playback_ctrl_res)(void* userdata, SLPlaybackCtrl cmd, int result);
	void (*on_playback_eof)(void* userdata);

	int (*on_upload_req)(void* userdata, const char* data_json, int length,
			char* buffer_res, int buffer_len);
	int (*on_upload_data)(void* userdata, const void* data, int length);
	int (*on_upload_end_req)(void* userdata);

	void (*on_upload_res)(void* userdata, const char* data_json, int length);
	void (*on_upload_failed)(void* userdata, int errcode);
	void (*on_upload_end_res)(void* userdata);

	int (*on_download_req)(void* userdata, const char* data_json, int length,
			char* buffer_res, int buffer_len);
	int (*on_download_read)(void* userdata, void* buffer, int buffer_len);

	void (*on_download_res)(void* userdata, const char* data_json, int length);
	void (*on_download_failed)(void* userdata, int errcode);
	void (*on_download_end)(void* userdata);

}sl_channel_listener;

typedef struct sl_channel sl_channel_t;

sl_channel_t* sl_channel_create();

sl_channel_t* sl_channel_addref(sl_channel_t* chan);

void sl_channel_autorelease(sl_channel_t **chan);

void sl_channel_setListener(sl_channel_t* chan, sl_channel_listener* l,
		void* userdata);

void* sl_channel_userdata(sl_channel_t* chan);

/**
 * private method.
 */
void sl_channel_setPFlow(sl_channel_t* chan, int32_t pflow);

sl_status_t sl_channel_setParams(sl_channel_t* chan, const char* sid, int64_t uid,
		const char* uname, const char* passwd);

/**
 * @param chan pointer to the object
 * @param ip peer ip
 * @param port
 * @param uname
 * @param passwd
 * @return
 */
sl_status_t sl_channel_setParams2(sl_channel_t* chan,
		const char* ip, uint16_t port, const char* uname, const char* passwd);

/**
 * @param type
 * @param channelnum channel number, starting from zero.
 * @param streamMask 0: data stream, 1: audio stream, 2: video stream, or 1|2
 * @param videoStreamType 0: main stream, 1: sub stream
 */
sl_status_t sl_channel_setRequest(sl_channel_t* chan, SLReqType type,
		int channelNum, int streamMask, int videoStreamType);

/**
 * @return -2: not set Parameters, -3:service not running
 */
sl_status_t sl_channel_connect(sl_channel_t* chan);

void sl_channel_disconnect(sl_channel_t* chan);

sl_status_t sl_channel_sendData(sl_channel_t* chan, const void* data, int len);

sl_status_t sl_channel_sendAudio(sl_channel_t* chan, const void *data, int len);
sl_status_t sl_channel_sendVideo(sl_channel_t* chan, int frametype, const void *data, int len);

sl_status_t sl_channel_search(sl_channel_t* chan, const char* arg_json);

sl_status_t sl_channel_playback(sl_channel_t* chan, SLPlaybackReq* req);
sl_status_t sl_channel_playback_ctrl(sl_channel_t* chan, SLPlaybackCtrl cmd, int32_t value);

//sl_status_t sl_channel_upload_req(sl_channel_t* chan, const char* arg_json);
//sl_status_t sl_channel_upload_data(sl_channel_t* chan, const void* data, int len);
//sl_status_t sl_channel_upload_end(sl_channel_t* chan);
//
//sl_status_t sl_channel_download_req(sl_channel_t* chan, const char* arg_json);


sl_status_t sl_channel_authRes(sl_channel_t* chan, int result, int flag);

sl_status_t sl_channel_sendPlaybackData(sl_channel_t* chan, void* data, int len, int frametype, int64_t ts);
sl_status_t sl_channel_playbackEOF(sl_channel_t* chan);

SL_END_DECLS

#endif /* SL_CHANNEL_H_ */
