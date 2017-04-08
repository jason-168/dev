/*
 * sl_push.h
 *
 *  Created on: Aug 11, 2016
 *      Author: root
 */

#ifndef SL_PUSH_H_
#define SL_PUSH_H_

#include <stdint.h>
#include <base/sl.h>

#include <sl/sl_protocol.h>
#include <sl/sl_define.h>

SL_BEGIN_DECLS

//---------------------------客户端API 开始-------------------------------------------
typedef struct sl_push_listener{
	void (*on_response)(void* userdata, int32_t reqtype, int32_t result,
			const char* jsondata, int datalen);
}sl_push_listener;

sl_status_t sl_push_registListener(sl_push_listener *l, void *userdata);
sl_status_t sl_push_unregistListener(sl_push_listener *l);

sl_status_t sl_push_req(sl_push_reqtype reqtype, const char* jsondata, int datalen);

//---------------------------客户端API 结束-------------------------------------------

//---------------------------设备端API 开始-------------------------------------------
/**
 * 推送一条报警消息
 * @param[in] qos
 * @param[in] expires
 * @param[in] ch		通道号
 * @param[in] alarmtype	报警类型
 *
 * @return 0 成功
 */
sl_status_t sl_push_alarm(int qos, int expires, int ch, int alarmtype);

/**
 * 推送一条自定义消息
 * @param[in] qos
 * @param[in] expires
 * @param[in] alert		消息标题(最大长度255)
 * @param[in] msg		消息体(最大长度255)
 *
 * @return 0 成功
 */
sl_status_t sl_push_message(int qos, int expires, const char* alert, const char *msg);

//---------------------------设备端API 结束-------------------------------------------

SL_END_DECLS

#endif /* SL_PUSH_H_ */
