/*
 * sl_portable.h
 *
 *  Created on: May 7, 2016
 *      Author: root
 */

#ifndef SL_PORTABLE_H_
#define SL_PORTABLE_H_

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
/* Windows */
typedef HANDLE		os_thread_t;

#else
#include <pthread.h>

/* Unix */
typedef pthread_t	os_thread_t;      /**< native thread */

#endif

#endif /* SL_PORTABLE_H_ */
