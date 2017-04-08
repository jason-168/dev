/*
 * sl_thread.h
 *
 *  Created on: Nov 27, 2015
 *      Author: root
 */

#ifndef SL_THREAD_H_
#define SL_THREAD_H_

#include <base/sl.h>
#include <base/sl_errors.h>

#include <base/sl_portable.h>

SL_BEGIN_DECLS

enum {
    /*
     * Use the levels below when appropriate. Intermediate values are
     * acceptable, preferably use the {MORE|LESS}_FAVORABLE constants below.
     */
    SL_PRIORITY_LOWEST         =  19,

    /* use for background tasks */
    SL_PRIORITY_BACKGROUND     =  10,

    /* most threads run at normal priority */
    SL_PRIORITY_NORMAL         =   0,

    /* threads currently running a UI that the user is interacting with */
    SL_PRIORITY_FOREGROUND     =  -2,

    /* the main UI thread has a slightly more favorable priority */
    SL_PRIORITY_DISPLAY        =  -4,

    /* ui service treads might want to run at a urgent display (uncommon) */
    SL_PRIORITY_URGENT_DISPLAY =  -8, /*HAL_PRIORITY_URGENT_DISPLAY,*/

    /* all normal audio threads */
    SL_PRIORITY_AUDIO          = -16,

    /* service audio threads (uncommon) */
    SL_PRIORITY_URGENT_AUDIO   = -19,

    /* should never be used in practice. regular process might not
     * be allowed to use this level */
    SL_PRIORITY_HIGHEST        = -20,

    SL_PRIORITY_DEFAULT        = SL_PRIORITY_NORMAL,
    SL_PRIORITY_MORE_FAVORABLE = -1,
    SL_PRIORITY_LESS_FAVORABLE = +1,
};

typedef struct sl_thread sl_thread_t;
typedef int (*sl_thread_process)(sl_thread_t *gthread, void *userdata);

/**
 * Create a new thread of execution
 * @param new_thread The newly created thread handle.
 * @param func The function to start the new thread in
 * @param userData Any data to be passed to the starting function
 */
sl_status_t sl_thread_create(sl_thread_t **new_thread, sl_thread_process process,
								void *userdata);

/*
 * Start the thread in sl_thread_process.
 */
sl_status_t sl_thread_start(sl_thread_t *thread, const char *thread_name, int32_t priority,
        					size_t stack);

/**
 * Get the thread ID
 */
os_thread_t sl_thread_id(sl_thread_t *thread);

/**
 * sl_thread_exit_pending() returns true if sl_thread_request_exit() has been called.
 */
int sl_thread_exit_pending(sl_thread_t *thread);

/**
 * Indicates whether this thread is running or not.
 */
int sl_thread_running(sl_thread_t *thread);

/**
 * Ask this object's thread to exit. This function is asynchronous, when the
 * function returns the thread might still be running. Of course, this
 * function can be called from a different thread.
 */
sl_status_t sl_thread_request_exit(sl_thread_t *thread);

sl_status_t sl_thread_destroy(sl_thread_t *thread);

//------------------------

/**
 * Get the thread ID
 */
os_thread_t sl_os_thread_id(void);

SL_END_DECLS

#endif /* SL_THREAD_H_ */
