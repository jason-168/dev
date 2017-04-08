/*
 * sl_looper.h
 *
 *  Created on: Nov 29, 2015
 *      Author: root
 */

#ifndef SL_LOOPER_H_
#define SL_LOOPER_H_

#include <stdint.h>

#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS

typedef struct sl_looper sl_looper_t;

typedef void* sl_looper_task_id;
typedef void sl_looper_task_proc(void *userdata, long arg);

sl_status_t sl_looper_create(sl_looper_t **looper);
void sl_looper_destroy(sl_looper_t *looper);

sl_status_t sl_looper_start(sl_looper_t *looper);
void sl_looper_stop(sl_looper_t *looper);

sl_looper_task_id sl_looper_post(sl_looper_t *looper,
		int32_t delay_ms, sl_looper_task_proc* proc, void *userdata, long arg);

sl_status_t sl_looper_cancel(sl_looper_t *looper, sl_looper_task_id id);

void sl_looper_clear(sl_looper_t *looper);

SL_END_DECLS

#endif /* SL_LOOPER_H_ */
