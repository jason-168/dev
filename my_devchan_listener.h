/*
 * my_devchan_listener.h
 *
 *  Created on: Aug 18, 2016
 *      Author: root
 */

#ifndef MY_DEVCHAN_L_IMPL_H_
#define MY_DEVCHAN_L_IMPL_H_

#include <stdio.h>

#include <sl/sl_service.h>
#include <sl/sl_channel_buffer.h>

typedef struct PlaybackModule{
	FILE* playback_file;

	int64_t starttime;

	SLPlaybackCtrl ctrl;
}PlaybackModule;

typedef struct MyDevchanLImpl{
	MyDevice* mydev;

	PlaybackModule playback;

	sl_channel_t* devchan;//sl device channel

	char* databuf;
	int databuf_len;
}MyDevchanLImpl;

MyDevchanLImpl* MyDevchanLImpl_Create(MyDevice* dev,
		sl_channel_t* chan);

void MyDevchanLImpl_Destroy(MyDevchanLImpl* l);

#endif /* MY_DEVCHAN_L_IMPL_H_ */
