/*
 * mydev.c
 *
 *  Created on: Jul 7, 2016
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG_TAG "mydev"
#include "base/sl_log.h"

#include <sl/sl_service.h>
#include <sl/sl_channel_buffer.h>
#include <sl/sl_push.h>

#include "mydev.h"
#include "my_devchan_listener.h"


#define MIN_FRAME_SIZE 1024
#define MAX_FRAME_SIZE 1024 * 500



static int videostream_init(VideoStream* stream){
	memset(stream, 0, sizeof(VideoStream));
	return 0;
}

static void videostream_close_proc(void *userdata, long arg){
	VideoStream* stream = (VideoStream*)userdata;
	if(stream->vfile != NULL){
		fclose(stream->vfile);
		stream->vfile = NULL;
	}

	if(stream->videobuf != NULL){
		free(stream->videobuf);
		stream->videobuf = NULL;
	}
}

static void videostream_uninit(VideoStream* stream){
	videostream_close_proc(stream, 0);
}

static int mystream_video_read(FILE* file, MediaPacket* mp, char* buffer, int* readlen){
	int headlen = fread(mp, 1, sizeof(MediaPacket), file);
	if(headlen <= 0){
		ALOGD("1,eof");
		fseek(file, 0, SEEK_SET);
		return -1;
	}
	int datalen = fread(buffer, 1, mp->length, file);
	if(datalen <= 0){
		ALOGD("2,eof");
		fseek(file, 0, SEEK_SET);
		return -1;
	}

	*readlen = datalen;
	return 0;
}

static void mystream_video_proc(void *userdata, long arg){
	VideoStream* stream = (VideoStream*)userdata;
	if(stream->videobuf == NULL){
		return;
	}
	int32_t delay = 0;

	if(stream->readMP.length > 0){
		if(stream->readMP.type > 0){
			sl_avbuffer_putVideo(stream->vhandle, stream->readMP.type, stream->videobuf, stream->readMP.length);
//			if(stream->ahandle != NULL){
//				sl_avbuffer_putAudio(stream->ahandle, stream->videobuf, stream->readMP.length);
//			}
		}
	}

	for(;;){
		MediaPacket mp0;
		int datalen;
		int rs = mystream_video_read(stream->vfile, &mp0, stream->videobuf, &datalen);
		if(rs == 0){
			if(mp0.type == 0){
//				sl_avbuffer_putVideo(stream->vhandle, mp0.type, stream->videobuf, mp0.length);

//				if(stream->ahandle != NULL){
//					sl_avbuffer_putAudio(stream->ahandle, stream->videobuf, datalen);
//				}
			}else{
				if(stream->readMP.length > 0){
					int64_t de = mp0.timestamp - stream->readMP.timestamp;
					delay = (int32_t)(de - 2);
				}
				memcpy(&stream->readMP, &mp0, sizeof(MediaPacket));
				break;
			}
		}else{
			memset(&stream->readMP, 0, sizeof(MediaPacket));
			stream->rereads += 1;
		}
	}

	if(stream->cam->astream.ahandle != NULL && stream->cam->astream.rereads == stream->rereads){
		MediaPacket* audioMp = &stream->cam->astream.audioMP;

		int32_t latenessMs = (int32_t)(stream->readMP.timestamp - audioMp->timestamp);

		int32_t earlyms = latenessMs - delay;
		if(earlyms > 0){
			delay += earlyms;
		}else{
			 if(latenessMs < (delay * -1)){
				delay = 0;
			}else{
				delay = delay + latenessMs;
			}
		}

//		ALOGD("video timestamp:%lld, audio:%lld", stream->readMP.timestamp, audioMp->timestamp);
	}
	if(delay <= 0){
		ALOGD("video delay:%d, mp0.length:%d, mp0.type:%d, mp0.timestamp:%d", delay, stream->readMP.length, stream->readMP.type, stream->readMP.timestamp);
	}
	sl_looper_post(stream->looper, delay > 0 ? delay : 0, mystream_video_proc, stream, 0);
}

static int videostream_open(MyCam* cam, VideoStream* stream, sl_looper_t *looper,
		/*sl_audiobuffer_t abuffer,*/ sl_videobuffer_t vbuffer, int framerate, const char* file){
	stream->cam = cam;
	stream->looper = looper;
//	stream->ahandle = abuffer;
	stream->vhandle = vbuffer;

	stream->framerate = framerate;

	stream->videobuf = (char*)malloc(MAX_FRAME_SIZE);

	memset(&stream->readMP, 0, sizeof(MediaPacket));

	stream->vfile = fopen(file, "rb");

	stream->rereads = 0;

	if(stream->vfile != NULL){
		sl_looper_post(stream->looper, 100, mystream_video_proc, stream, 0);
		return 0;
	}else{
		return -1;
	}
}
static void videostream_close(VideoStream* stream){
	sl_looper_post(stream->looper, 0, videostream_close_proc, stream, 0);
}

//-----------------------------------------------

static int audiostream_init(AudioStream* stream){
	memset(stream, 0, sizeof(AudioStream));
	return 0;
}

static void audiostream_close_proc(void *userdata, long arg){
	AudioStream* stream = (AudioStream*)userdata;

	if(stream->afile != NULL){
		fclose(stream->afile);
		stream->afile = NULL;
	}
	if(stream->audiobuf != NULL){
		free(stream->audiobuf);
		stream->audiobuf = NULL;
	}
}

static void audiostream_uninit(AudioStream* stream){
	audiostream_close_proc(stream, 0);
}

static void audiostream_proc(void *userdata, long arg){
	AudioStream* stream = (AudioStream*)userdata;
	if(stream->audiobuf == NULL){
		return;
	}


	int32_t delay = 0;

	if(stream->audioMP.length > 0){
		if(stream->audioMP.type == 0){
			sl_avbuffer_putAudio(stream->ahandle, stream->audiobuf, stream->audioMP.length);
		}
	}

	for(;;){
		MediaPacket mp0;
		int datalen;
		int rs = mystream_video_read(stream->afile, &mp0, stream->audiobuf, &datalen);
		if(rs == 0){
			if(mp0.type > 0){
//				sl_avbuffer_putVideo(stream->vhandle, mp0.type, stream->videobuf, mp0.length);

//				if(stream->ahandle != NULL){
//					sl_avbuffer_putAudio(stream->ahandle, stream->videobuf, datalen);
//				}
			}else{
				if(stream->audioMP.length > 0){
					int64_t de = mp0.timestamp - stream->audioMP.timestamp;
					delay = (int32_t)(de - 2);
				}
				memcpy(&stream->audioMP, &mp0, sizeof(MediaPacket));
				break;
			}
		}else{
			memset(&stream->audioMP, 0, sizeof(MediaPacket));
			stream->rereads += 1;
		}
	}
	if(delay <= 0){
		ALOGD("audio delay:%d, mp0.length:%d, mp0.type:%d, mp0.timestamp:%d", delay, stream->audioMP.length, stream->audioMP.type, stream->audioMP.timestamp);
	}
	sl_looper_post(stream->looper, delay, audiostream_proc, stream, 0);
}

static int audiostream_open(AudioStream* stream, sl_looper_t *looper,
		sl_audiobuffer_t abuffer, int samplerate, int channels, const char* file){
	stream->looper = looper;
	stream->ahandle = abuffer;

	stream->audiobuf = (char*)malloc(MAX_FRAME_SIZE);

	memset(&stream->audioMP, 0, sizeof(MediaPacket));

	stream->afile = fopen(file, "rb");
	stream->rereads = 0;
	if(stream->afile != NULL){
		sl_looper_post(stream->looper, 100, audiostream_proc, stream, 0);
		return 0;
	}else{
		return -1;
	}
}
static void audiostream_close(AudioStream* stream){
	sl_looper_post(stream->looper, 0, audiostream_close_proc, stream, 0);
}

//-----------------------------------------------

static int mycam_init(MyCam* cam){
	int i;
	for(i=0; i<VSTREAM_COUNT; i++){
		videostream_init(&cam->vstreams[i]);
	}

	audiostream_init(&cam->astream);
	return 0;
}

static void mycam_uninit(MyCam* cam){
	int i;
	for(i=0; i<VSTREAM_COUNT; i++){
		videostream_uninit(&cam->vstreams[i]);
	}

	audiostream_uninit(&cam->astream);
}

static VideoStream* mycam_get(MyCam* cam, int streamtype){
	if(streamtype >= VSTREAM_COUNT){
		return NULL;
	}
	return &cam->vstreams[streamtype];
}



int mydevice_init(MyDevice* dev, const char* configfile){
	memset(dev, 0, sizeof(MyDevice));

	sl_cond_create(&dev->cond);
	sl_mutex_create(&dev->mutex, SL_MUTEX_DEFAULT);

	sl_status_t rs = sl_looper_create(&dev->looper);
	ALOGD("sl_looper_create, rs:%d", rs);
	rs = sl_looper_start(dev->looper);
	ALOGD("sl_looper_start, rs:%d", rs);


	FILE* f = fopen(configfile, "rb");
	if(f != NULL){
		char buf[4096] = {0};
		/*int rlen = */fread(buf, 1, sizeof(buf), f);
//		ALOGD("rlen:%d, %s", rlen, buf);

		dev->config = cJSON_Parse(buf);

		fclose(f);

		cJSON* streamtypes = cJSON_GetObjectItem(dev->config, "streamtypes");
		cJSON* channelArr = cJSON_GetObjectItem(dev->config, "channels");

		dev->myinfo._channelCount = cJSON_GetArraySize(channelArr);
		dev->myinfo._audioCount = dev->myinfo._channelCount;
		dev->myinfo._videoStreamTypes = streamtypes->valueint;
		dev->myinfo._maxConnections = 10;
		dev->myinfo._enableMiniFS = 1;//1

		dev->camArr = sl_ringarray_create(sizeof(MyCam), dev->myinfo._channelCount);
		int i = 0;
		for(i=0; i<sl_ringarray_capacity(dev->camArr); i++){
			MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, i);
			mycam_init(cam);
		}
	}

	dev->_alarm_id = NULL;

	return 0;
}

void mydevice_uninit(MyDevice* dev){
	if(dev->looper != NULL){
		sl_looper_destroy(dev->looper);
		dev->looper = NULL;
	}

	int i = 0;
	for(i=0; i<sl_ringarray_capacity(dev->camArr); i++){
		MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, i);
		mycam_uninit(cam);
	}

	sl_ringarray_destroy(dev->camArr);

	cJSON_Delete(dev->config);

	if(dev->cond != NULL){
		sl_cond_destroy(dev->cond);
		dev->cond = NULL;
	}
	if(dev->mutex != NULL){
		sl_mutex_destroy(dev->mutex);
		dev->mutex = NULL;
	}
}

static void mydevice_alarm_push_proc(void *userdata, long arg){
	MyDevice* dev = (MyDevice*)userdata;

	int rs = sl_push_alarm(0, 0, 0, 1);
	ALOGD("sl_push_alarm:%d", rs);

	dev->_alarm_id = sl_looper_post(dev->looper, 15000, mydevice_alarm_push_proc, dev, 0);
}

void mydevice_alarm_on(MyDevice* dev){
	ALOGD("mydevice_alarm_on");
	if(dev->_alarm_id == NULL){
		dev->_alarm_id = sl_looper_post(dev->looper, 15000, mydevice_alarm_push_proc, dev, 0);
	}
}

void mydevice_alarm_off(MyDevice* dev){
	ALOGD("mydevice_alarm_on");
	if(dev->_alarm_id != NULL){
		sl_looper_cancel(dev->looper, dev->_alarm_id);
		dev->_alarm_id = NULL;
	}
}

//--------------------------------------------------------------------------------------
extern sl_channel_listener mydevchan_listener_impl;

static sl_channel_t* chan_ = NULL;

//sl_channel_t* getIncomingChannel(){
//	return chan_;
//}

static int mydev_incoming_session(void *userdata, sl_channel_t *chan,
		const char *sid, const char *username, const char *passwd, SLReqType reqtype){
	ALOGD("mydev_incoming_client, clientsid:%s, username:%s, passwd:%s", sid, username, passwd);

	MyDevice* dev = (MyDevice*)userdata;
	MyDevchanLImpl* l = MyDevchanLImpl_Create(dev, chan);

	sl_channel_setListener(chan, &mydevchan_listener_impl, l);

	chan_ = chan;
	if(reqtype == SL_REQ_VOICECALL || reqtype == SL_REQ_VIDEOCALL){
		return 1;
	}
	return 0;
}

static void mydev_outgoing_session(void *userdata, sl_channel_t *chan){
	ALOGD("mydev_outgoing_client");

	MyDevchanLImpl* l = (MyDevchanLImpl*)sl_channel_userdata(chan);
	MyDevchanLImpl_Destroy(l);
	chan_ = NULL;
}

static bool mydev_has_audio_permission(void *userdata, const char *username,
		int channelNum_req){
	ALOGD("mydev_has_audio_permission");
	return true;
}

#define USING_AUDIO 1

static int mydev_on_audio_open(void *userdata, sl_audiobuffer_t abuffer,
		int channelNum_req, SLAudioFormat *afmt_res){
	ALOGD("mydev_on_audio_open, channelNum_req:%d", channelNum_req);
#if USING_AUDIO
	MyDevice* dev = (MyDevice*)userdata;
	MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, channelNum_req);

	cJSON* channelArr = cJSON_GetObjectItem(dev->config, "channels");
	cJSON* chanobj = cJSON_GetArrayItem(channelArr, channelNum_req);

	cJSON* audiobj = cJSON_GetObjectItem(chanobj, "audio");

	cJSON* codecobj = cJSON_GetObjectItem(audiobj, "acID");
	cJSON* srteobj = cJSON_GetObjectItem(audiobj, "srte");
	cJSON* chsobj = cJSON_GetObjectItem(audiobj, "channels");
	cJSON* fobj = cJSON_GetObjectItem(audiobj, "file");

	afmt_res->acodecID = codecobj->valueint;
	afmt_res->samplerate = srteobj->valueint;
	afmt_res->channels = chsobj->valueint;

	return audiostream_open(&cam->astream, dev->looper, abuffer,
			srteobj->valueint, chsobj->valueint, fobj->valuestring);
#else
	return -1;
#endif
}

static void mydev_on_audio_close(void *userdata, int channelNum_req){
	ALOGD("mydev_on_audio_close, channelNum_req:%d", channelNum_req);
#if USING_AUDIO
	MyDevice* dev = (MyDevice*)userdata;
	MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, channelNum_req);
	audiostream_close(&cam->astream);
#endif
}

static bool mydev_has_video_permission(void *userdata, const char *username,
		int channelNum_req, int videoStreamType_req){
	ALOGD("mydev_has_video_permission");
	return true;
}

static int mydev_on_video_open(void *userdata, sl_videobuffer_t vbuffer,
		int channelNum_req, int videoStreamType_req, SLVideoFormat *vfmt_res){
	ALOGD("mydev_on_video_open, channelNum_req:%d", channelNum_req);
	MyDevice* dev = (MyDevice*)userdata;
	MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, channelNum_req);

	VideoStream* vstream = mycam_get(cam, videoStreamType_req);

	if(vstream != NULL){
		cJSON* channelArr = cJSON_GetObjectItem(dev->config, "channels");
		cJSON* chanobj = cJSON_GetArrayItem(channelArr, channelNum_req);
		cJSON* streamArr = cJSON_GetObjectItem(chanobj, "streams");
		cJSON* streamobj = cJSON_GetArrayItem(streamArr, videoStreamType_req);

		cJSON* codecobj = cJSON_GetObjectItem(streamobj, "vcID");
		cJSON* wobj = cJSON_GetObjectItem(streamobj, "width");
		cJSON* hobj = cJSON_GetObjectItem(streamobj, "height");
		cJSON* rateobj = cJSON_GetObjectItem(streamobj, "frmR");
		cJSON* fobj = cJSON_GetObjectItem(streamobj, "file");

		vfmt_res->vcodecID = codecobj->valueint;
		vfmt_res->width = wobj->valueint;
		vfmt_res->height = hobj->valueint;
		vfmt_res->framerate = rateobj->valueint;

		return videostream_open(cam, vstream, dev->looper, /*cam->astream.ahandle,*/ vbuffer, rateobj->valueint, fobj->valuestring);
	}else{
		return -1;
	}
}

static void mydev_on_video_close(void *userdata, int channelNum_req, int videoStreamType_req){
	ALOGD("mydev_on_video_close, channelNum_req:%d", channelNum_req);
	MyDevice* dev = (MyDevice*)userdata;
	MyCam* cam = (MyCam*)sl_ringarray_get(dev->camArr, channelNum_req);
	VideoStream* vstream = mycam_get(cam, videoStreamType_req);
	if(vstream != NULL){
		videostream_close(vstream);
	}
}

sl_session_listener sessionlistener = {
	.incoming_session = mydev_incoming_session,
	.outgoing_session = mydev_outgoing_session,

	.has_audio_permission = mydev_has_audio_permission,
	.on_audio_open = mydev_on_audio_open,
	.on_audio_close = mydev_on_audio_close,

	.has_video_permission = mydev_has_video_permission,
	.on_video_open = mydev_on_video_open,
	.on_video_close = mydev_on_video_close,
};
