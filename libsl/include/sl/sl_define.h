/*
 * sl_define.h
 *
 *  Created on: Apr 18, 2016
 *      Author: root
 */

#ifndef SL_DEFINE_H_
#define SL_DEFINE_H_

typedef enum sl_login_type{
	SL_APP = 0,
	SL_DEV = 1,
}sl_login_type;

typedef enum sl_nettype{
	SL_NETTYPE_NONE	= -1,
	SL_NETTYPE_MOBILE	= 0,
	SL_NETTYPE_WIFI		= 1,
	SL_NETTYPE_ETH		= 9,
}sl_nettype;

//0: UNKNOWN, 1: 2G, 2: 3G, 3: 4G
typedef enum sl_network_class{
	SL_NETWORK_CLASS_UNKNOWN = 0,
	SL_NETWORK_CLASS_2G = 1,
	SL_NETWORK_CLASS_3G = 2,
	SL_NETWORK_CLASS_4G = 3,
	SL_NETWORK_CLASS_5G = 4,
}sl_network_class;

typedef enum sl_ptype{
	SL_PUSH_TYPE_ZZ		= 0,
	SL_PUSH_TYPE_APNS	= 1,
	SL_PHONE_TYPE_APNS2	= 2,
	SL_PHONE_TYPE_MQTT	= 3,
	SL_PHONE_TYPE_GCM	= 4,
}sl_ptype;

typedef enum sl_push_reqtype{
	SL_PUSH_REQ_USERREG			= 1,
	SL_PUSH_REQ_USERREG_INFO	= 2,
	SL_PUSH_REQ_ONOFF			= 3,
	SL_PUSH_REQ_ONOFF_STATUS	= 4,
}sl_push_reqtype;


//device parameter
typedef struct sl_device_info{
	sl_nettype _nettype;
	sl_network_class _netclass;

	char _localIP[SL_IP_LENGTH];//Local IP
	char _ssid[SL_IP_LENGTH];

	//-----------------------------------------------
	//0:app client, 1:device
	sl_login_type _appOrdev;

	//客户端/设备端 帐号
	char _sid[SL_ID_LEN];
	//客户端/设备端 密码
	char _passwd[SL_ID_LEN];

	//视频通道数[0 - 1024]
	int _channelCount;
	//音频源数[0 - 1024]
	int _audioCount;
	//视频码流种数[0 - 5]
	int _videoStreamTypes;

	//最大支持连接数[0 - 1000]
	int _maxConnections;

	//是否启用Mini转发, 1:启用, 0:禁用
	int _enableMiniFS;

}sl_device_info;


#endif /* SL_DEFINE_H_ */
