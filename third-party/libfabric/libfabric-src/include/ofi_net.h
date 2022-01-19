/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _OFI_NET_H_
#define _OFI_NET_H_

#include "config.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <ofi_osd.h>
#include <ofi_list.h>

#include <rdma/fabric.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/providers/fi_log.h>


#ifdef __cplusplus
extern "C" {
#endif


extern struct fi_provider core_prov;


/*
 * OS X doesn't have __BYTE_ORDER, Linux usually has BYTE_ORDER but not under
 * all features.h flags
 */
#if !defined(BYTE_ORDER)
#  if defined(__BYTE_ORDER) && \
      defined(__LITTLE_ENDIAN) && \
      defined(__BIG_ENDIAN)
#    define BYTE_ORDER __BYTE_ORDER
#    define LITTLE_ENDIAN __LITTLE_ENDIAN
#    define BIG_ENDIAN __BIG_ENDIAN
#  else
#    error "cannot determine endianness!"
#  endif
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#ifndef htonll
static inline uint64_t htonll(uint64_t x) { return bswap_64(x); }
#endif
#ifndef ntohll
static inline uint64_t ntohll(uint64_t x) { return bswap_64(x); }
#endif
#else
#ifndef htonll
static inline uint64_t htonll(uint64_t x) { return x; }
#endif
#ifndef ntohll
static inline uint64_t ntohll(uint64_t x) { return x; }
#endif
#endif

#ifdef MSG_ZEROCOPY
#define OFI_ZEROCOPY MSG_ZEROCOPY
#define OFI_ZEROCOPY_SIZE 9000 /* arbitrary based on documentation */
#else
#define OFI_ZEROCOPY 0
#define OFI_ZEROCOPY_SIZE SIZE_MAX
#endif


static inline int ofi_recvall_socket(SOCKET sock, void *buf, size_t len)
{
	ssize_t ret;

	ret = ofi_recv_socket(sock, buf, len, MSG_WAITALL);
	return (size_t) ret != len;
}

static inline int ofi_sendall_socket(SOCKET sock, const void *buf, size_t len)
{
	size_t sent;
	ssize_t ret;

	for (sent = 0, ret = 0; (sent < len) && (ret >= 0); ) {
		ret = ofi_send_socket(sock, ((char *) buf) + sent, len - sent, 0);
		if (ret > 0)
			sent += ret;
	}

	return (size_t) sent != len;
}

int ofi_discard_socket(SOCKET sock, size_t len);

/*
 * Byte queue - streaming socket staging buffer
 */
enum {
	OFI_BYTEQ_SIZE = 9000, /* Hard-coded max, good for 6 1500B buffers */
};

struct ofi_byteq {
	size_t size;
	unsigned int head;
	unsigned int tail;
	uint8_t data[OFI_BYTEQ_SIZE];
};

static inline void ofi_byteq_init(struct ofi_byteq *byteq, ssize_t size)
{
	memset(byteq, 0, sizeof *byteq);
	if (size > OFI_BYTEQ_SIZE)
		byteq->size = OFI_BYTEQ_SIZE;
	else if (size >= 0)
		byteq->size = size;
	else
		byteq->size = 0;
}

static inline void ofi_byteq_discard(struct ofi_byteq *byteq)
{
	byteq->head = 0;
	byteq->tail = 0;
}

static inline size_t ofi_byteq_readable(struct ofi_byteq *byteq)
{
	return byteq->tail - byteq->head;
}

static inline size_t ofi_byteq_writeable(struct ofi_byteq *byteq)
{
	return byteq->size - byteq->tail;
}

static inline size_t
ofi_byteq_read(struct ofi_byteq *byteq, void *buf, size_t len)
{
	size_t avail;

	avail = ofi_byteq_readable(byteq);
	if (!avail)
		return 0;

	if (len < avail) {
		memcpy(buf, &byteq->data[byteq->head], len);
		byteq->head += len;
		return len;
	}

	memcpy(buf, &byteq->data[byteq->head], avail);
	byteq->head = 0;
	byteq->tail = 0;
	return avail;
}

static inline void
ofi_byteq_write(struct ofi_byteq *byteq, const void *buf, size_t len)
{
	assert(len <= ofi_byteq_writeable(byteq));
	memcpy(&byteq->data[byteq->tail], buf, len);
	byteq->tail += len;
}

void ofi_byteq_writev(struct ofi_byteq *byteq, const struct iovec *iov,
		      size_t cnt);

static inline ssize_t ofi_byteq_recv(struct ofi_byteq *byteq, SOCKET sock)
{
	size_t avail;
	ssize_t ret;

	avail = ofi_byteq_writeable(byteq);
	assert(avail);
	ret = ofi_recv_socket(sock, &byteq->data[byteq->tail], avail,
			      MSG_NOSIGNAL);
	if (ret > 0)
		byteq->tail += ret;
	return ret;
}

size_t ofi_byteq_readv(struct ofi_byteq *byteq, struct iovec *iov,
		       size_t cnt, size_t offset);

static inline ssize_t ofi_byteq_send(struct ofi_byteq *byteq, SOCKET sock)
{
	size_t avail;
	ssize_t ret;

	avail = ofi_byteq_readable(byteq);
	assert(avail);
	ret = ofi_send_socket(sock, &byteq->data[byteq->head], avail,
			      MSG_NOSIGNAL);
	if (ret == avail) {
		byteq->head = 0;
		byteq->tail = 0;
	} else if (ret > 0) {
		byteq->head += ret;
	}
	return ret;
}


/*
 * Buffered socket - socket with send/receive staging buffers.
 */
struct ofi_bsock {
	SOCKET sock;
	struct ofi_byteq sq;
	struct ofi_byteq rq;
	size_t zerocopy_size;
	uint32_t async_index;
	uint32_t done_index;
};

static inline void
ofi_bsock_init(struct ofi_bsock *bsock, ssize_t sbuf_size, ssize_t rbuf_size)
{
	bsock->sock = INVALID_SOCKET;
	ofi_byteq_init(&bsock->sq, sbuf_size);
	ofi_byteq_init(&bsock->rq, rbuf_size);
	bsock->zerocopy_size = SIZE_MAX;

	/* first async op will wrap back to 0 as the starting index */
	bsock->async_index = UINT32_MAX;
	bsock->done_index = UINT32_MAX;
}

static inline void ofi_bsock_discard(struct ofi_bsock *bsock)
{
	ofi_byteq_discard(&bsock->rq);
	ofi_byteq_discard(&bsock->sq);
}

static inline size_t ofi_bsock_readable(struct ofi_bsock *bsock)
{
	return ofi_byteq_readable(&bsock->rq);
}

static inline size_t ofi_bsock_tosend(struct ofi_bsock *bsock)
{
	return ofi_byteq_readable(&bsock->sq);
}

ssize_t ofi_bsock_flush(struct ofi_bsock *bsock);
/* For sends started asynchronously, the return value will be -EINPROGRESS,
 * and len will be set to the number of bytes that were queued.
 */
ssize_t ofi_bsock_send(struct ofi_bsock *bsock, const void *buf, size_t *len);
ssize_t ofi_bsock_sendv(struct ofi_bsock *bsock, const struct iovec *iov,
			size_t cnt, size_t *len);
ssize_t ofi_bsock_recv(struct ofi_bsock *bsock, void *buf, size_t len);
ssize_t ofi_bsock_recvv(struct ofi_bsock *bsock, struct iovec *iov,
			size_t cnt);
uint32_t ofi_bsock_async_done(const struct fi_provider *prov,
			      struct ofi_bsock *bsock);


/*
 * Address utility functions
 */

#ifndef AF_IB
#define AF_IB 27
#endif

#define OFI_ADDRSTRLEN (INET6_ADDRSTRLEN + 50)

/*  values taken from librdmacm/rdma_cma.h */
#define OFI_IB_IP_PS_MASK   0xFFFFFFFFFFFF0000ULL
#define OFI_IB_IP_PORT_MASK   0x000000000000FFFFULL

struct ofi_sockaddr_ib {
	unsigned short int  sib_family; /* AF_IB */
	uint16_t            sib_pkey;
	uint32_t            sib_flowinfo;
	uint8_t             sib_addr[16];
	uint64_t            sib_sid;
	uint64_t            sib_sid_mask;
	uint64_t            sib_scope_id;
};

enum ofi_rdma_port_space {
	OFI_RDMA_PS_IPOIB = 0x0002,
	OFI_RDMA_PS_IB    = 0x013F,
	OFI_RDMA_PS_TCP   = 0x0106,
	OFI_RDMA_PS_UDP   = 0x0111,
};

union ofi_sock_ip {
	struct sockaddr			sa;
	struct sockaddr_in		sin;
	struct sockaddr_in6		sin6;
	struct ofi_sockaddr_ib		sib;
	uint8_t				align[48];
};

struct ofi_addr_list_entry {
	struct slist_entry	entry;
	char			ipstr[INET6_ADDRSTRLEN];
	union ofi_sock_ip	ipaddr;
	size_t			speed;
	char			net_name[OFI_ADDRSTRLEN];
	char			ifa_name[OFI_ADDRSTRLEN];
	uint64_t		comm_caps;
};

int ofi_addr_cmp(const struct fi_provider *prov, const struct sockaddr *sa1,
		const struct sockaddr *sa2);
int ofi_getifaddrs(struct ifaddrs **ifap);

void ofi_set_netmask_str(char *netstr, size_t len, struct ifaddrs *ifa);

void ofi_get_list_of_addr(const struct fi_provider *prov, const char *env_name,
			  struct slist *addr_list);
void ofi_free_list_of_addr(struct slist *addr_list);

#define ofi_sa_family(addr) ((struct sockaddr *)(addr))->sa_family
#define ofi_sin_addr(addr) (((struct sockaddr_in *)(addr))->sin_addr)
#define ofi_sin_port(addr) (((struct sockaddr_in *)(addr))->sin_port)

#define ofi_sin6_addr(addr) (((struct sockaddr_in6 *)(addr))->sin6_addr)
#define ofi_sin6_port(addr) (((struct sockaddr_in6 *)(addr))->sin6_port)

#define ofi_sib_addr(addr) (((struct ofi_sockaddr_ib *)(addr))->sib_addr)

static inline size_t ofi_sizeofaddr(const struct sockaddr *addr)
{
	switch (addr->sa_family) {
	case AF_INET:
		return sizeof(struct sockaddr_in);
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
	case AF_IB:
		return sizeof(struct ofi_sockaddr_ib);
	default:
		FI_WARN(&core_prov, FI_LOG_CORE, "Unknown address format\n");
		return 0;
	}
}

static inline size_t ofi_sizeofip(const struct sockaddr *addr)
{
	switch (addr->sa_family) {
	case AF_INET:
		return sizeof(struct in_addr);
	case AF_INET6:
		return sizeof(struct in6_addr);
	case AF_IB:
		return sizeof(ofi_sib_addr(addr));
	default:
		FI_WARN(&core_prov, FI_LOG_CORE, "Unknown address format\n");
		return 0;
	}
}

static inline int ofi_translate_addr_format(int family)
{
	switch (family) {
	case AF_INET:
		return FI_SOCKADDR_IN;
	case AF_INET6:
		return FI_SOCKADDR_IN6;
	case AF_IB:
		return FI_SOCKADDR_IB;
	default:
		return FI_FORMAT_UNSPEC;
	}
}

static inline size_t ofi_sizeof_addr_format(int format)
{
	switch (format) {
	case FI_SOCKADDR_IN:
		return sizeof(struct sockaddr_in);
	case FI_SOCKADDR_IN6:
		return sizeof(struct sockaddr_in6);
	case FI_SOCKADDR_IB:
		return sizeof(struct ofi_sockaddr_ib);
	default:
		FI_WARN(&core_prov, FI_LOG_CORE, "Unsupported address format\n");
		return 0;
	}
}

uint16_t ofi_get_sa_family(const struct fi_info *info);

static inline bool ofi_sin_is_any_addr(const struct sockaddr *sa)
{
	struct in_addr ia_any = {
		.s_addr = INADDR_ANY,
	};

	if (!sa)
		return false;

	return !memcmp(&ofi_sin_addr(sa).s_addr, &ia_any, sizeof(ia_any));

}

static inline bool ofi_sin6_is_any_addr(const struct sockaddr *sa)
{
	struct in6_addr ia6_any = IN6ADDR_ANY_INIT;

	if (!sa)
		return false;

	return !memcmp(&ofi_sin6_addr(sa), &ia6_any, sizeof(ia6_any));
}

static inline bool ofi_sib_is_any_addr(const struct sockaddr *sa)
{
	struct in6_addr ia6_any = IN6ADDR_ANY_INIT;

	if (!sa)
		return false;

	return !memcmp(&ofi_sib_addr(sa), &ia6_any, sizeof(ia6_any));
}

static inline bool ofi_is_any_addr(const struct sockaddr *sa)
{
	if (!sa)
		return false;

	switch(sa->sa_family) {
	case AF_INET:
		return ofi_sin_is_any_addr(sa);
	case AF_INET6:
		return ofi_sin6_is_any_addr(sa);
	case AF_IB:
		return ofi_sib_is_any_addr(sa);
	default:
		FI_WARN(&core_prov, FI_LOG_CORE, "Unknown address format!\n");
		return false;
	}
}

static inline uint16_t ofi_addr_get_port(const struct sockaddr *addr)
{
	if (!addr)
		return 0;

	switch (ofi_sa_family(addr)) {
	case AF_INET:
		return ntohs(ofi_sin_port((const struct sockaddr_in *) addr));
	case AF_INET6:
		return ntohs(ofi_sin6_port((const struct sockaddr_in6 *) addr));
	case AF_IB:
		return (uint16_t)ntohll(((const struct ofi_sockaddr_ib *)addr)->sib_sid);
	default:
		FI_WARN(&core_prov, FI_LOG_FABRIC, "Unknown address format\n");
		return 0;
	}
}

static inline void ofi_addr_set_port(struct sockaddr *addr, uint16_t port)
{
	struct ofi_sockaddr_ib *sib;

	switch (ofi_sa_family(addr)) {
	case AF_INET:
		ofi_sin_port(addr) = htons(port);
		break;
	case AF_INET6:
		ofi_sin6_port(addr) = htons(port);
		break;
    case AF_IB:
		sib = (struct ofi_sockaddr_ib *)addr;
		sib->sib_sid = htonll(((uint64_t)OFI_RDMA_PS_IB << 16) + ntohs(port));
		sib->sib_sid_mask = htonll(OFI_IB_IP_PS_MASK | OFI_IB_IP_PORT_MASK);
		break;
	default:
		FI_WARN(&core_prov, FI_LOG_FABRIC, "Unknown address format\n");
		assert(0);
	}
}

static inline void * ofi_get_ipaddr(const struct sockaddr *addr)
{
	switch (addr->sa_family) {
	case AF_INET:
		return &ofi_sin_addr((const struct sockaddr_in *) addr);
	case AF_INET6:
		return &ofi_sin6_addr((const struct sockaddr_in6 *) addr);
	case AF_IB:
		return &ofi_sib_addr((const struct ofi_sockaddr_ib *) addr);
	default:
		return NULL;
	}
}

static inline bool ofi_valid_dest_ipaddr(const struct sockaddr *addr)
{
	return ofi_addr_get_port(addr) && !ofi_is_any_addr(addr);
}

static inline bool ofi_equals_ipaddr(const struct sockaddr *addr1,
				    const struct sockaddr *addr2)
{
	if (addr1->sa_family != addr2->sa_family)
		return false;

	switch (addr1->sa_family) {
	case AF_INET:
	        return !memcmp(&ofi_sin_addr(addr1), &ofi_sin_addr(addr2),
				sizeof(ofi_sin_addr(addr1)));
	case AF_INET6:
	        return !memcmp(&ofi_sin6_addr(addr1), &ofi_sin6_addr(addr2),
				sizeof(ofi_sin6_addr(addr1)));
	case AF_IB:
	        return !memcmp(&ofi_sib_addr(addr1), &ofi_sib_addr(addr2),
				sizeof(ofi_sib_addr(addr1)));
	default:
		return false;
	}
}

static inline bool ofi_equals_sockaddr(const struct sockaddr *addr1,
				       const struct sockaddr *addr2)
{
        return (ofi_addr_get_port(addr1) == ofi_addr_get_port(addr2)) &&
		ofi_equals_ipaddr(addr1, addr2);
}

bool ofi_is_wildcard_listen_addr(const char *node, const char *service,
				 uint64_t flags, const struct fi_info *hints);

size_t ofi_mask_addr(struct sockaddr *maskaddr, const struct sockaddr *srcaddr,
		     const struct sockaddr *netmask);


/*
 * Address logging
 */
const char *ofi_straddr(char *buf, size_t *len,
			uint32_t addr_format, const void *addr);
uint32_t ofi_addr_format(const char *str);

/* Returns allocated address to caller.  Caller must free.  */
int ofi_str_toaddr(const char *str, uint32_t *addr_format,
		   void **addr, size_t *len);

void ofi_straddr_log_internal(const char *func, int line,
			      const struct fi_provider *prov,
			      enum fi_log_level level,
			      enum fi_log_subsys subsys, char *log_str,
			      const void *addr);

#define ofi_straddr_log(...) \
	ofi_straddr_log_internal(__func__, __LINE__, __VA_ARGS__)

#if ENABLE_DEBUG
#define ofi_straddr_dbg(prov, subsystem, ...) \
	ofi_straddr_log(prov, FI_LOG_DEBUG, subsystem, __VA_ARGS__)
#else
#define ofi_straddr_dbg(prov, subsystem, ...) do {} while(0)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _OFI_NET_H_ */
