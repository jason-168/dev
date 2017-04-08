/*
 * sl_socket_helper.h
 *
 *  Created on: Dec 2, 2015
 *      Author: root
 */

#ifndef SL_SOCKET_HELPER_H_
#define SL_SOCKET_HELPER_H_

#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
/* Windows */
#if defined(WINNT) || defined(_WINNT) || defined(__BORLANDC__) || defined(__MINGW32__)
	|| defined(_WIN32_WCE) || defined (_MSC_VER)
#define _MSWSOCK_
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <windows.h>
#include <errno.h>
#include <string.h>

#define socket_close closesocket

#ifdef EWOULDBLOCK
#undef EWOULDBLOCK
#endif
#ifdef EINPROGRESS
#undef EINPROGRESS
#endif
#ifdef EAGAIN
#undef EAGAIN
#endif
#ifdef EINTR
#undef EINTR
#endif

#define EWOULDBLOCK	WSAEWOULDBLOCK
#define EINPROGRESS	WSAEWOULDBLOCK
#define EAGAIN		WSAEWOULDBLOCK
#define EINTR		WSAEINTR

#if defined(_WIN32_WCE)
#define NO_STRSTREAM 1
#endif

//// For "uintptr_t" and "intptr_t", we assume that if they're not already defined, then this must be
//// an old, 32-bit version of Windows:
//#if !defined(_MSC_STDINT_H_) && !defined(_UINTPTR_T_DEFINED) && !defined(_UINTPTR_T_DECLARED) && !defined(_UINTPTR_T)
//typedef unsigned uintptr_t;
//#endif
//#if !defined(_MSC_STDINT_H_) && !defined(_INTPTR_T_DEFINED) && !defined(_INTPTR_T_DECLARED) && !defined(_INTPTR_T)
//typedef int intptr_t;
//#endif

#elif defined(VXWORKS)
/* VxWorks */
#include <time.h>
#include <timers.h>
#include <sys/times.h>
#include <sockLib.h>
#include <hostLib.h>
#include <resolvLib.h>
#include <ioLib.h>

#else
/* Unix */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdbool.h>
#if defined(_QNX4)
#include <sys/select.h>
#include <unix.h>
#endif

#define socket_close close

#endif

//#ifndef SOCKLEN_T
//#define SOCKLEN_T int
//#endif

#include <base/sl.h>
#include <base/sl_errors.h>

#ifdef D_NO_SOCKADDR_LEN
#define SET_SOCKADDR_SIN_LEN(var)
#else
#define SET_SOCKADDR_SIN_LEN(var) var.sin_len = sizeof(var)
#endif

SL_BEGIN_DECLS

sl_status_t sl_socket_setnodelay(int socketNum);

void sl_socket_ignore_sigpipe(int socketNum);

int sl_socket_make_nonblocking(int socketNum);
int sl_socket_make_blocking(int socketNum, unsigned write_timeout_ms);

int sl_socket_create(int domain, int type);

int sl_socket_stream_setup(int domain, uint16_t port, bool make_nonBlocking);
int sl_socket_datagram_setup(int domain, uint16_t port, bool make_nonBlocking);

int sl_socket_read(int socketNum, void* buffer, size_t size);
int sl_socket_write(int socketNum, const void* buffer, size_t size);

int sl_socket_sendto(int socketNum, const void* buffer, size_t bufferSize,
		const struct sockaddr* addr, socklen_t addrlen);
int sl_socket_recvfrom(int socketNum, void* buffer, size_t bufferSize,
		struct sockaddr *fromAddress, socklen_t *addrlen);

unsigned sl_socket_getSendBufferSize(int socketNum);
unsigned sl_socket_getReceiveBufferSize(int socketNum);

unsigned sl_socket_setSendBufferTo(int socketNum, unsigned requestedSize);
unsigned sl_socket_setReceiveBufferTo(int socketNum, unsigned requestedSize);


sl_status_t sl_socket_localHost4(int socketNum, char *buf, int buflen);
sl_status_t sl_socket_localHost6(int socketNum, char *buf, int buflen);
#define sl_socket_localHost(af, s, b, l) \
	((af) == AF_INET) ? sl_socket_localHost4(s, b, l) : sl_socket_localHost6(s, b, l)

sl_status_t sl_socket_localPort4(int socketNum, uint16_t *port);
sl_status_t sl_socket_localPort6(int socketNum, uint16_t *port);
#define sl_socket_localPort(af, s, p) \
	((af) == AF_INET) ? sl_socket_localPort4(s, p) : sl_socket_localPort6(s, p)


sl_status_t sl_socket_inet_ntop4(struct sockaddr* in, char* buf, int buflen, uint16_t* port);
sl_status_t sl_socket_inet_ntop6(struct sockaddr* in, char* buf, int buflen, uint16_t* port);
#define sl_socket_inet_ntop(af, in, b, l, p) \
	((af) == AF_INET) ? sl_socket_inet_ntop4(in, b, l, p) : sl_socket_inet_ntop6(in, b, l, p)


int sl_socket_connected(int socketNum);

sl_status_t sl_socket_connecttimeout(int socketNum, int32_t timeoutMs);

int sl_socket_readable(int socketNum, int32_t timeoutMs);
int sl_socket_writable(int socketNum, int32_t timeoutMs);

//sl_status_t sl_inet_ntoa(char *buf, int buflen, struct in_addr in);

SL_END_DECLS

#endif /* SL_SOCKET_HELPER_H_ */

