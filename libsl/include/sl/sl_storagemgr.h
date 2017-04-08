/*
 * sl_storagemgr.h
 *
 *  Created on: May 18, 2016
 *      Author: root
 */

#ifndef SL_STORAGEMGR_H_
#define SL_STORAGEMGR_H_

#include <stdint.h>
#include <base/sl.h>

SL_BEGIN_DECLS

typedef struct sl_storagemgr_listener{
	void (*on_response)(void* userdata, int32_t reqtype, int32_t result,
			const char* jsondata, int datalen);
}sl_storagemgr_listener;

typedef enum sl_storagemgr_reqtype{
	SL_STORAGEMGR_REQ_VAS4USER		= 1,
	SL_STORAGEMGR_REQ_FILES			= 3,
}sl_storagemgr_reqtype;


sl_status_t sl_storagemgr_registListener(sl_storagemgr_listener *l, void *userdata);
sl_status_t sl_storagemgr_unregistListener(sl_storagemgr_listener *l);

sl_status_t sl_storagemgr_req(sl_storagemgr_reqtype reqtype, const char* jsondata, int datalen);

SL_END_DECLS

#endif /* SL_STORAGEMGR_H_ */
