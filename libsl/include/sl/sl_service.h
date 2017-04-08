/*
 * sl_service.h
 *
 *  Created on: Apr 22, 2016
 *      Author: root
 */

#ifndef SL_SERVICE_H_
#define SL_SERVICE_H_

#include <stdbool.h>

#include <base/sl.h>
#include <base/sl_errors.h>

#include <sl/sl_protocol.h>
#include <sl/sl_define.h>

#include <sl/sl_channel_buffer.h>
#include <sl/sl_channel.h>


SL_BEGIN_DECLS

typedef struct sl_session_listener{
	/**
	 * 客户端接入验证接口。
	 * @param[in] userdata	通过sl_service_setSessionListener传入的userdata参数
	 * @param[in] chan		连接接口
	 * @param[in] sid		客户端账号
	 * @param[in] username	客户端登陆传进来的用户名
	 * @param[in] passwd	客户端登陆传进来的密码
	 * @param[in] reqtype	客户端请求类型
	 *
	 * @return 0 通过验证, 1:通过sl_channel_authRes接口验证, 负数则表示验证失败.
	 */
	int (*incoming_session)(void *userdata, sl_channel_t *chan,
			const char *sid, const char *username, const char *passwd,
			SLReqType reqtype);
	/**
	 * 客户端退出会话连接
	 * @param[in] userdata
	 * @param[in] chan		连接接口
	 */
	void (*outgoing_session)(void *userdata, sl_channel_t *chan);

	/**
	 * 验证客户端用户名为username的用户是否有打开音频的权限
	 * @param[in] userdata
	 * @param[in] username			客户端登陆传进来的用户名
	 * @param[in] channelNum_req	通道号
	 *
	 * @return true or false
	 */
	bool (*has_audio_permission)(void *userdata, const char *username,
			int channelNum_req);

	/**
	 * 客户端请求打开音频
	 * @param[in] userdata
	 * @param[in] abuffer			音频缓冲区
	 * @param[in] channelNum_req	请求打开音频的通道号
	 * @param[out] afmt_res			设置音频格式
	 *
	 * @return 0 成功, 负数则失败.
	 */
	int (*on_audio_open)(void *userdata, sl_audiobuffer_t abuffer,
			int channelNum_req, SLAudioFormat *afmt_res);
	/**
	 * 客户端请求关闭音频
	 * @param[in] userdata
	 * @param[in] channelNum_req	请求打开关闭的通道号
	 */
	void (*on_audio_close)(void *userdata, int channelNum_req);

	/**
	 * 验证客户端用户名为username的用户是否有打开视频的权限
	 * @param[in] userdata
	 * @param[in] username				客户端登陆传进来的用户名
	 * @param[in] channelNum_req		通道号
	 * @param[in] videoStreamType_req	码流类型
	 *
	 * @return true or false
	 */
	bool (*has_video_permission)(void *userdata, const char *username,
			int channelNum_req, int videoStreamType_req);
	/**
	 * 客户端请求打开视频
	 * @param[in] userdata
	 * @param[in] vbuffer				视频缓冲区
	 * @param[in] channelNum_req		请求打开视频的通道号
	 * @param[in] videoStreamType_req	码流类型
	 * @param[out] vfmt_res				设置视频格式
	 *
	 * @return 0 成功, 负数则失败.
	 */
	int (*on_video_open)(void *userdata,sl_videobuffer_t vbuffer,
			int channelNum_req, int videoStreamType_req, SLVideoFormat *vfmt_res);
	/**
	 * 客户端请求关闭视频
	 * @param[in] userdata
	 * @param[in] channelNum_req		请求关闭视频的通道号
	 * @param[in] videoStreamType_req	码流类型
	 */
	void (*on_video_close)(void *userdata, int channelNum_req, int videoStreamType_req);

	void *userdata;//private
}sl_session_listener;

typedef struct sl_state_listener{
	/**
	 * 上下线通知
	 * @param[in] userdata
	 * @param[in] state		1:在线, 0:离线
	 */
	void (*on_mine_changed)(void *userdata, int state);

	/**
	 * 客户端预连接结果回调
	 * @param[in] userdata
	 * @param[in] sid		设备账号
	 * @param[in] state		设备在线状态, 1:在线, 0:离线
	 */
	void (*on_preconnect_changed)(void *userdata, const char *sid, int state);

	/**
	 * 客户端添加好友/设备消息回调
	 * @param[in] userdata
	 * @param[in] reqtype
	 * @param[in] jsondata
	 * @param[in] datalen
	 */
	void (*on_message)(void* userdata, int32_t reqtype, const char* jsondata, int datalen);

	void *userdata;//private
}sl_state_listener;

sl_status_t sl_service_init(sl_device_info *info, sl_state_listener *l, void *userdata);
void sl_service_deinit();

void sl_service_setSessionListener(sl_session_listener *l, void *userdata);

/**
 * Service start.
 * @param[in] thread	0:用当前线程运行SDK Service, 会一直阻塞, 直到Service停止.
 * 						1:建立另一前线程运行SDK Service, 马上返回.
 * @return 0 成功, 负数则失败.
 */
sl_status_t sl_service_start(int thread);

/**
 * 停止SDK Service.
 */
void sl_service_stop();

int32_t sl_service_commID();
int32_t sl_service_myState();
int64_t sl_service_myUID();


void sl_service_network_changed(sl_nettype nettype, sl_network_class networkClass,
		const char* ip, const char* ssid);

sl_status_t sl_service_enable_preconnect(const char *sid, int64_t uid);
void sl_service_disable_preconnect(const char *sid);

sl_status_t sl_service_preconnectState();

sl_status_t sl_service_getPreconnInfo(const char* sid, char* buffer, int length);


SL_END_DECLS

#endif /* SL_SERVICE_H_ */
