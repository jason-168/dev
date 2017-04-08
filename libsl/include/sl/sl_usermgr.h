/*
 * sl_usermgr.h
 *
 *  Created on: May 18, 2016
 *      Author: root
 */

#ifndef SL_USERMGR_H_
#define SL_USERMGR_H_

#include <stdint.h>
#include <base/sl.h>

SL_BEGIN_DECLS

typedef struct sl_usermgr_listener{
	//0:login OK
	void (*on_login)(void* userdata, int result);

	void (*on_response)(void* userdata, int32_t reqtype, int32_t result,
			const char* jsondata, int datalen);
}sl_usermgr_listener;

typedef enum sl_usermgr_reqtype{
	SL_USERMGR_REQ_REG			= 1,
	SL_USERMGR_REQ_USERSEARCH	= 2,
	SL_USERMGR_REQ_DEVSEARCH	= 3,
	SL_USERMGR_REQ_ADD			= 4,
	SL_USERMGR_REQ_DEL			= 5,
	SL_USERMGR_REQ_UPD			= 6,
	SL_USERMGR_REQ_BUDDYLIST	= 7,
	SL_USERMGR_REQ_USERDETAIL	= 8,
	SL_USERMGR_REQ_DEVDETAIL	= 9,
}sl_usermgr_reqtype;

typedef enum sl_usermgr_restype{
	SL_USERMGR_RES_ADD			= 34,
}sl_usermgr_restype;

sl_status_t sl_usermgr_registListener(sl_usermgr_listener *l, void *userdata);
sl_status_t sl_usermgr_unregistListener(sl_usermgr_listener *l);

sl_status_t sl_usermgr_login(const char *uname, const char *passwd);
sl_status_t sl_usermgr_logout();

sl_status_t sl_usermgr_req(sl_usermgr_reqtype reqtype, const char* jsondata, int datalen);

sl_status_t sl_usermgr_res(sl_usermgr_restype restype, const char* jsondata, int datalen);

SL_END_DECLS

#endif /* SL_USERMGR_H_ */
