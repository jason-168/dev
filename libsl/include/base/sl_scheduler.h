/*
 * sl_scheduler.h
 *
 *  Created on: Apr 20, 2016
 *      Author: root
 */

#ifndef SL_SCHEDULER_H_
#define SL_SCHEDULER_H_

#include <stdint.h>
#include <stdbool.h>

#include <base/sl_portable.h>

#include <base/sl.h>
#include <base/sl_errors.h>

SL_BEGIN_DECLS


typedef struct sl_scheduler sl_scheduler_t;

sl_scheduler_t* sl_scheduler_create(uint32_t id);
void sl_scheduler_destroy(sl_scheduler_t *sched);

sl_status_t sl_scheduler_start(sl_scheduler_t *sched, const char* thread_name, int thread);
void sl_scheduler_stop(sl_scheduler_t *sched);

os_thread_t sl_scheduler_thread_id(sl_scheduler_t *sched);

bool sl_scheduler_running(sl_scheduler_t *sched);

void sl_scheduler_run(sl_scheduler_t *sched);

//void sl_scheduler_break(sl_scheduler_t *sched);

#define SL_READ		0x01 /* io detected read will not block */
#define SL_WRITE	0x02 /* io detected write will not block */

typedef void* sl_socktoken;

// For handling socket operations in the background (from the event loop):
typedef void sockethandler_proc(void* userdata, int fd, int mask);

sl_socktoken sl_scheduler_sockethandling(sl_scheduler_t *sched, int socketNum,
		int events, sockethandler_proc* handlerProc, void* userdata);

void sl_scheduler_sockethandling_disable(sl_scheduler_t *sched, sl_socktoken token, int events);
void sl_scheduler_sockethandling_enable(sl_scheduler_t *sched, sl_socktoken token, int events);

void sl_scheduler_socktoken_destroy(sl_scheduler_t *sched, sl_socktoken *token);

typedef void* sl_tasktoken;
typedef void taskproc(void* userdata, long arg);

/* ms: milliseconds */
//sl_tasktoken sl_scheduler_delayedtask(sl_scheduler_t *sched, uint32_t ms,
//		taskproc* proc, void* userdata);
//sl_status_t sl_scheduler_again_delayedtask(sl_scheduler_t *sched, sl_tasktoken token,
//		uint32_t ms);

sl_tasktoken sl_scheduler_delayedtask(sl_scheduler_t *sched, int32_t ms,
		taskproc* proc, void* userdata, long arg);
sl_tasktoken sl_scheduler_periodictask(sl_scheduler_t *sched, int32_t ms,
		taskproc* proc, void* userdata, long arg);
sl_status_t sl_scheduler_task_again(sl_scheduler_t *sched, sl_tasktoken token,
		int32_t ms, long arg);

void sl_scheduler_tasktoken_destroy(sl_scheduler_t *sched, sl_tasktoken *token);

sl_status_t sl_scheduler_triggerevent(sl_scheduler_t *sched, taskproc* proc, void* userdata, long arg);

SL_END_DECLS

#endif /* SL_SCHEDULER_H_ */
