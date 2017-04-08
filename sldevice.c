/*
 * sldevice.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>

#define LOG_TAG "sldevice"
#include "base/sl_log.h"

#include <base/sl_mutex.h>
#include <base/sl_cond.h>
#include <base/sl_thread.h>
#include <base/sl_time.h>
#include <base/sl_utils.h>

#include <sl/sl_service.h>

#include "mydev.h"

//static void my_sighandler(int s){
//	ALOGD("my_sighandler:%d", s);
//}

static void chan_on_mine_changed(void *userdata, int state){
	ALOGD("chan_on_mine_changed:%d", state);
}

sl_state_listener state_listener = {
	.on_mine_changed = chan_on_mine_changed,
	.on_preconnect_changed = NULL,
	.on_message = NULL,
};

extern sl_session_listener sessionlistener;


int main(int argc, char *argv[]){
	if(argc != 4){
		printf("usage:%s, devx 123456 configfile.json\n", argv[0]);
		return -1;
	}

	char sid[32] ={0};
	char passed[32] = {0};

	char configfile[64] = {0};

	strncpy(sid, argv[1], sizeof(sid));
	strncpy(passed, argv[2], sizeof(passed));

	strncpy(configfile, argv[3], sizeof(configfile));

	//--------把log写入文件，不需要则注释这几行--------
	char fname[32] = {0};
	snprintf(fname, sizeof(fname), "./%s.log", sid);
	FILE* logf = fopen(fname, "wb");
	sl_log_init(logf);
	//--------把log写入文件，不需要则注释这几行--------


//	signal(SIGINT, &my_sighandler);
//	signal(SIGPIPE, &my_sighandler);
//	signal(SIGKILL, &my_sighandler);

	ALOGD("========login:%s=========", sid);

	MyDevice mydev;
	mydevice_init(&mydev, configfile);

	sl_device_info* myinfo = &mydev.myinfo;

	myinfo->_nettype = SL_NETTYPE_ETH;

	myinfo->_appOrdev = SL_DEV;

	strncpy(myinfo->_sid, sid, sizeof(myinfo->_sid));
	strncpy(myinfo->_passwd, passed, sizeof(myinfo->_passwd));

	sl_status_t rs = sl_service_init(myinfo, &state_listener, NULL);
	ALOGD("sl_service_init:%d", rs);

	if(rs == SL_OK){
		sl_service_setSessionListener(&sessionlistener, &mydev);

		rs = sl_service_start(0);
		ALOGD("sl_service_start:%d", rs);
	}

	sl_service_stop();
	ALOGD("sl_service_stop");
	sl_service_deinit();

	mydevice_uninit(&mydev);

	sl_log_deinit();
	return 0;
}
