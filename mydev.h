/*
 * mydev.h
 *
 *  Created on: Jul 7, 2016
 *      Author: root
 */

#ifndef MYDEV_H_
#define MYDEV_H_

#include <base/sl_mutex.h>
#include <base/sl_cond.h>
#include <base/sl_looper.h>
#include <base/sl_time.h>
#include <base/sl_utils.h>
#include <base/sl_ringarray_freelock.h>
#include <base/cJSON.h>


SL_BEGIN_DECLS

typedef struct MediaPacket{
	uint32_t length;
	uint32_t type;	//0:audio, 1: I frame, 2: P frame
	int64_t timestamp;
}MediaPacket;

struct MyCam;

typedef struct VideoStream{
	struct MyCam* cam;

	sl_looper_t *looper;

	sl_videobuffer_t vhandle;//sl video buffer

	int framerate;

	char* videobuf;

	MediaPacket readMP;

	FILE* vfile;

	int rereads;
}VideoStream;

typedef struct AudioStream{
	sl_looper_t *looper;

	sl_audiobuffer_t ahandle;//sl audio buffer

	char* audiobuf;

	MediaPacket audioMP;

	FILE* afile;

	int rereads;
}AudioStream;


#define VSTREAM_COUNT 2

typedef struct MyCam{
	VideoStream vstreams[VSTREAM_COUNT];

	AudioStream astream;
}MyCam;


typedef struct MyDevice{
	sl_cond_t *cond;
	sl_mutex_t *mutex;

	sl_looper_t *looper;


	cJSON* config;
	sl_device_info myinfo;

	sl_ringarray_t* camArr;

	sl_looper_task_id _alarm_id;
}MyDevice;

int mydevice_init(MyDevice* dev, const char* configfile);
void mydevice_uninit(MyDevice* dev);

void mydevice_alarm_on(MyDevice* dev);
void mydevice_alarm_off(MyDevice* dev);

typedef struct TestHead{
	int seq;
	char text[64];
}TestHead;

SL_END_DECLS

#endif /* MYDEV_H_ */
