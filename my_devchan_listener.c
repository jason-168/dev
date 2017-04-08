/*
 * my_devchan_listener.c
 *
 *  Created on: Aug 18, 2016
 *      Author: root
 */

#define LOG_TAG "MyDevchanLImpl"
#include "base/sl_log.h"
#include <base/cJSON.h>

#include <sl/sl_service.h>

#include "mydev.h"
#include "my_devchan_listener.h"

#define MAX_FRAME_SIZE 1024 * 500

static void playback_module_close(PlaybackModule* m){
	if(m->playback_file != NULL){
		fclose(m->playback_file);
		m->playback_file = NULL;
	}
	memset(m, 0, sizeof(PlaybackModule));

	ALOGD("playback_module_close");
}

static int playback_module_open(PlaybackModule* m, const char* file, int32_t starttime){
	playback_module_close(m);

	FILE* f = fopen(file, "rb");
	if(f == NULL){
		return -1;
	}
	m->playback_file = f;
	m->starttime = starttime;

	return 0;
}

static void mydevchan_playback_read_proc(void* userdata, long arg){
	MyDevchanLImpl* l = (MyDevchanLImpl*)userdata;
	PlaybackModule* m = &l->playback;

	if(m->ctrl == SL_PLAYBACK_PAUSE || m->ctrl == SL_PLAYBACK_STOP){
		return;
	}

	MediaPacket mp;
	int read_rs = 0;
	do{
		if(l->databuf_len == 0){
			int headlen = fread(&mp, 1, sizeof(MediaPacket), m->playback_file);
			if(headlen <= 0){
				read_rs = -1;
				break;
			}
			if(mp.length > MAX_FRAME_SIZE){
				ALOGE("============mp.length:%d", mp.length);
				read_rs = -1;
				break;
			}

			int datalen = fread(l->databuf, 1, mp.length, m->playback_file);
			if(datalen <= 0){
				read_rs = -1;
				break;
			}
			l->databuf_len = datalen;
		}

		int32_t send_rs = sl_channel_sendPlaybackData(l->devchan, l->databuf, l->databuf_len, mp.type, mp.timestamp);
//		ALOGD("l->databuf_len:%d, send_rs:%d", l->databuf_len, send_rs);

		if(send_rs == SL_OK){
			l->databuf_len = 0;
		}else if(send_rs == SL_EAGAIN){
			break;
		}else{
			read_rs = -1;
			break;
		}
	}while(0);

	if(read_rs != 0){
		sl_channel_playbackEOF(l->devchan);
	}else{
		sl_looper_post(l->mydev->looper, 10, mydevchan_playback_read_proc, l, 0);
	}
}

static void mydevchan_playback_ctrl_proc(void* userdata, long arg){
	MyDevchanLImpl* l = (MyDevchanLImpl*)userdata;
	l->playback.ctrl = (SLPlaybackCtrl)arg;

	if(l->playback.ctrl == SL_PLAYBACK_START || l->playback.ctrl == SL_PLAYBACK_RESUME){
		sl_looper_post(l->mydev->looper, 10, mydevchan_playback_read_proc, l, 0);
	}
}

static int mydevchan_on_playback_ctrl_req(void* userdata, SLPlaybackCtrl cmd, int32_t value){
	MyDevchanLImpl* l = (MyDevchanLImpl*)userdata;
	ALOGD("mydevchan_on_playback_ctrl:%d, value:%d", cmd, value);
	switch(cmd){
	case SL_PLAYBACK_START:{
		if(l->databuf == NULL){
			l->databuf = (char*)malloc(MAX_FRAME_SIZE);
		}
		l->databuf_len = 0;
		break;
	}
	case SL_PLAYBACK_PAUSE:{
		break;
	}
	case SL_PLAYBACK_RESUME:{
		break;
	}
	case SL_PLAYBACK_SEEK:{
		break;
	}
	case SL_PLAYBACK_SPEEDUP:{
		break;
	}
	case SL_PLAYBACK_SPEEDDOWN:{
		break;
	}
	case SL_PLAYBACK_STOP:{
		playback_module_close(&l->playback);
		break;
	}
	}

	sl_looper_post(l->mydev->looper, 0, mydevchan_playback_ctrl_proc, l, cmd);
	return 0;
}

static void mydevchan_on_disconnected(void* userdata, int errcode){

}

static int r_seq = 0;

static void mydevchan_incoming_data(void* userdata, const void* data, int length){
	TestHead* head = (TestHead*)data;
	ALOGD("read, seq:%d, data:%s, rlen:%d", head->seq, head->text, length);

	if(r_seq != 0){
		SL_CHECK_EQ(++r_seq, head->seq);
	}else{
		r_seq = head->seq;
	}

	char* buf = (char*)data;
	TestHead* tail = (TestHead*)(buf + (length - (length - sizeof(TestHead))));
	if(head->seq != tail->seq || strncmp(head->text, tail->text, sizeof(head->text) != 0)){
		ALOGE("tail, seq:%d, data:%s", tail->seq, tail->text);
	}
}

static void mydevchan_incoming_audio(void* userdata, const void* data, int length,
		uint32_t seq, int64_t timestamp){
}
static void mydevchan_incoming_video(void* userdata, const void* data, int length,
		int frametype, uint32_t seq, int64_t timestamp){
}

static int mydevchan_on_videocall_req(void *userdata, SLStreamReq *req, SLAVFormat* avfmt_res){
	ALOGD("mydevchan_on_videocall_req");
	return 0;
}

static int mydevchan_on_search_req(void* userdata, const char* data_req_json, int json_length,
		char* buffer, int bufferLen){
	MyDevchanLImpl* l = (MyDevchanLImpl*)userdata;
	cJSON* pb_list_file = cJSON_GetObjectItem(l->mydev->config, "playback_list");
	FILE* f = fopen(pb_list_file->valuestring, "rb");
	if(f != NULL){
		char buf[4096] = {0};
		int rlen = fread(buf, 1, sizeof(buf), f);

		strncpy(buffer, buf, rlen);
		fclose(f);
		return 0;
	}
	return -1;
}

static void parseFromJson(SLPlaybackRes *pb_res, cJSON* json){
	cJSON* afmtobj = cJSON_GetObjectItem(json, "afmt");
	cJSON* acIDobj = cJSON_GetObjectItem(afmtobj, "acID");
	cJSON* srteobj = cJSON_GetObjectItem(afmtobj, "srte");
	cJSON* chnobj = cJSON_GetObjectItem(afmtobj, "channels");

	pb_res->avfmt.audiofmt.acodecID = (SLACodecID)acIDobj->valueint;
	pb_res->avfmt.audiofmt.samplerate = srteobj->valueint;
	pb_res->avfmt.audiofmt.channels = chnobj->valueint;

	cJSON* vfmtobj = cJSON_GetObjectItem(json, "vfmt");
	cJSON* vcIDobj = cJSON_GetObjectItem(vfmtobj, "vcID");
	cJSON* widthobj = cJSON_GetObjectItem(vfmtobj, "width");
	cJSON* heightobj = cJSON_GetObjectItem(vfmtobj, "height");
	cJSON* frmRobj = cJSON_GetObjectItem(vfmtobj, "frmR");
	cJSON* ifivobj = cJSON_GetObjectItem(vfmtobj, "ifiv");

	pb_res->avfmt.videofmt.vcodecID = (SLVCodecID)vcIDobj->valueint;
	pb_res->avfmt.videofmt.width = widthobj->valueint;
	pb_res->avfmt.videofmt.height = heightobj->valueint;
	pb_res->avfmt.videofmt.framerate = frmRobj->valueint;
	pb_res->avfmt.videofmt.ifiv = ifivobj->valueint;

	cJSON* durobj = cJSON_GetObjectItem(json, "duration");
	pb_res->duration = durobj->valueint;
}

static int mydevchan_on_playback_req(void* userdata, SLPlaybackReq* pb_req, SLPlaybackRes *pb_res){
	MyDevchanLImpl* l = (MyDevchanLImpl*)userdata;

	if(playback_module_open(&l->playback, pb_req->file, pb_req->start_ts) != 0){
		return -1;
	}

	int rs = -1;

	cJSON* pb_info_file = cJSON_GetObjectItem(l->mydev->config, "playback_info");
	FILE* f = fopen(pb_info_file->valuestring, "rb");
	if(f != NULL){
		char buf[4096] = {0};
		/*int rlen =*/ fread(buf, 1, sizeof(buf), f);
		cJSON* json = cJSON_Parse(buf);

		fclose(f);

		cJSON* listobj = cJSON_GetObjectItem(json, "list");
		int listSize = cJSON_GetArraySize(listobj);
		int i = 0;
		for(i=0; i<listSize; i++){
			cJSON* item = cJSON_GetArrayItem(listobj, i);
			cJSON* fobj = cJSON_GetObjectItem(item, "file");
			if(strcmp(pb_req->file, fobj->valuestring) == 0){
				parseFromJson(pb_res, item);
				rs = 0;
				break;
			}
		}

		cJSON_Delete(json);
	}

	if(rs != 0){
		playback_module_close(&l->playback);
	}

	return rs;
}

sl_channel_listener mydevchan_listener_impl = {
	.on_reconnecting = NULL,
	.on_connected = NULL,
	.on_auth = NULL,
	.on_disconnected = mydevchan_on_disconnected,

	.on_datarate = NULL,
	.incoming_data = mydevchan_incoming_data,

	.on_stream_res = NULL,

	.incoming_audio = mydevchan_incoming_audio,
	.incoming_video = mydevchan_incoming_video,

	.on_videocall_req = mydevchan_on_videocall_req,
	.on_videocall_res = NULL,

	.on_search_req = mydevchan_on_search_req,
	.on_search_res = NULL,

	.on_playback_req = mydevchan_on_playback_req,
	.on_playback_ctrl_req = mydevchan_on_playback_ctrl_req,
//	.on_playback_read = mydevchan_on_playback_read,

	.on_playback_res = NULL,
	.on_playback_ctrl_res = NULL,
	.on_playback_eof = NULL,
};

MyDevchanLImpl* MyDevchanLImpl_Create(MyDevice* dev, sl_channel_t* chan){
	MyDevchanLImpl* l = malloc(sizeof(MyDevchanLImpl));
	memset(l, 0, sizeof(MyDevchanLImpl));
	ALOGD("MyDevchanLImpl_Create(%p)", l);

	l->mydev = dev;
	l->devchan = chan;
	l->databuf = NULL;
	l->databuf_len = 0;
	return l;
}

void MyDevchanLImpl_Destroy(MyDevchanLImpl* l){
	ALOGD("MyDevchanLImpl_Destroy(%p)", l);
	if(l->databuf != NULL){
		free(l->databuf);
		l->databuf = NULL;
	}
	free(l);
}

