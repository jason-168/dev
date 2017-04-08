/*
 * sl_socketpair.h
 *
 *  Created on: Jun 10, 2016
 *      Author: root
 */

#ifndef SL_SOCKETPAIR_H_
#define SL_SOCKETPAIR_H_

#include <base/sl.h>

SL_BEGIN_DECLS

typedef struct sl_socketpeer{
	int* _sockfd;
}sl_socketpeer;

typedef struct sl_socketpair{
	int _fd[2];

	sl_socketpeer _sockpair[2];
}sl_socketpair;

sl_status_t sl_socketpair_init(sl_socketpair* sockpair, int type);

void sl_socketpair_deinit(sl_socketpair* sockpair);

sl_socketpeer* sl_socketpair_peer0(sl_socketpair* sockpair);
sl_socketpeer* sl_socketpair_peer1(sl_socketpair* sockpair);

int sl_socketpeer_fd(sl_socketpeer* sockpeer);
int sl_socketpeer_read(int fd, void* data, size_t size);
int sl_socketpeer_write(sl_socketpeer* sockpeer, const void* data, size_t size);

SL_END_DECLS

#endif /* SL_SOCKETPAIR_H_ */
