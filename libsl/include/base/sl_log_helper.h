/*
 * sl_log_helper.h
 *
 *  Created on: Nov 27, 2015
 *      Author: root
 */

#ifndef SL_LOG_HELPER_H_
#define SL_LOG_HELPER_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void sl_log_init(FILE* f);
void sl_log_deinit();

int __sl_log_write(int prio, const char *tag, const char *msg);

int __sl_log_print(int prio, const char *tag, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* SL_LOG_HELPER_H_ */
