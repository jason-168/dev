/*
 * sl_connection.h
 *
 *  Created on: Dec 1, 2015
 *      Author: root
 */

#ifndef SL_CONNECTION_H_
#define SL_CONNECTION_H_

#include <base/sl.h>
#include <base/sl_errors.h>

#include <base/sl_socket_helper.h>

SL_BEGIN_DECLS

typedef enum ConnType{
	SL_CONN_STREAM = 1,
	SL_CONN_DGRAM,
	SL_CONN_UX_STREAM,
	SL_CONN_UX_DGRAM,

	SL_CONN_V6_UX_STREAM,
	SL_CONN_V6_UX_DGRAM,
}ConnType;

typedef enum sl_af_t{
	SL_STREAM			= SOCK_STREAM,
	SL_DGRAM			= SOCK_DGRAM,
	SL_UX_STREAM		,
	SL_UX_DGRAM			,

	SL_STREAM_V6		= SOCK_STREAM,
	SL_DGRAM_V6			= SOCK_DGRAM,
	SL_UX_STREAM_V6,
	SL_UX_DGRAM_V6,
}sl_af_t;

typedef struct sl_connection sl_connection_t;

/*
 * Create a connection.
 * @return 0: success,
 * 		 < 0: failed.
 */
//sl_connection_t* sl_connection_create(int type, uint32_t id);
//sl_connection_t* sl_connection_create2(int type, int fd, uint32_t id);


//sl_connection_t* sl_connection_create4(int type, sl_connection_t *bindconn,
//		/*const char *destIp, uint16_t destPort,*/ uint32_t id);
//sl_connection_t* sl_connection_create5(int type, int ufd, sl_connection_t *udpconn,
//		/*const char *destIp, uint16_t destPort,*/ uint32_t id);

sl_connection_t* sl_connection_stream(int af, int fd,
		struct sockaddr* addr, socklen_t addrlen, uint32_t id);
sl_connection_t* sl_connection_stream2(int af, uint32_t id);

sl_connection_t* sl_connection_dgram(int af, int fd, uint32_t id);
sl_connection_t* sl_connection_dgram2(int af, uint32_t id);


sl_connection_t* sl_connection_ux(int type, int ufd,
		struct sockaddr* addr, socklen_t addrlen, sl_connection_t *bindconn, uint32_t id);
sl_connection_t* sl_connection_ux2(int type, sl_connection_t *bindconn, uint32_t id);


sl_connection_t* sl_connection_addref(sl_connection_t *conn);

/**
 * Destroy the sl_connection_t variable and free the associated memory.
 * @param conn the sl_connection_t variable to destroy.
 */
void sl_connection_autorelease(sl_connection_t **conn);

/**
 * Open connection
 * @return 0: The connection is pending
 * 		  1: local connection connected
 * 		  > 1: Connection had opened
 * 		  < 0: error
 */
sl_status_t sl_connection_open(sl_connection_t *conn, const char *host, uint16_t port, uint16_t bindPort);

sl_status_t sl_connection_open2(sl_connection_t *conn, struct sockaddr *addr, socklen_t addrlen, uint16_t bindPort);

/*
 * Close connection
 */
sl_status_t sl_connection_close(sl_connection_t *conn);

/**
 * Read bytes from the connection.
 *
 * @param buffer    Pointer to destination buffer.
 * @param size       Number of bytes to read.
 * @return Number of bytes read.
 * 			< 0 if error.
 */
int sl_connection_read(sl_connection_t *conn,
		void *buffer, size_t size);

int sl_connection_read2(sl_connection_t *conn,
		void *buffer, size_t size, struct sockaddr *fromAddress);

/**
 * Write bytes to the connection.
 *
 * @param buffer    Pointer to source buffer.
 * @param size       Number of bytes to read.
 * @return Number of bytes written.
 * 			< 0 if error.
 */
int sl_connection_write(sl_connection_t *conn,
		const void *buffer, size_t size);

int sl_connection_write2(sl_connection_t *conn,
		const char *host, uint16_t port, const void *buffer, size_t size);

void sl_connection_setid(sl_connection_t *conn, uint32_t id);

int sl_connection_fd(sl_connection_t *conn);
int sl_connection_af(sl_connection_t *conn);

//ConnType sl_connection_type(sl_connection_t *conn);

bool sl_connection_isUX(sl_connection_t *conn);

struct sockaddr* sl_connection_addr(sl_connection_t *conn);
socklen_t sl_connection_addrlen(sl_connection_t *conn);

sl_status_t sl_connection_localHost(sl_connection_t *conn, char *buf, int buflen);
uint16_t sl_connection_localPort(sl_connection_t *conn);

sl_status_t sl_connection_remoteHost(sl_connection_t *conn, char* host, int len);
uint16_t sl_connection_remotePort(sl_connection_t *conn);

SL_END_DECLS

#endif /* SL_CONNECTION_H_ */
