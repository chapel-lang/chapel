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
 * Address utility functions
 */

#ifndef AF_IB
#define AF_IB 27
#endif

#define OFI_ADDRSTRLEN (INET6_ADDRSTRLEN + 50)

union ofi_sock_ip {
	struct sockaddr		sa;
	struct sockaddr_in	sin;
	struct sockaddr_in6	sin6;
	uint8_t			align[32];
};

struct ofi_addr_list_entry {
	struct slist_entry	entry;
	char			ipstr[INET6_ADDRSTRLEN];
	union ofi_sock_ip	ipaddr;
	size_t			speed;
	char			net_name[OFI_ADDRSTRLEN];
	char			ifa_name[OFI_ADDRSTRLEN];
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


static inline size_t ofi_sizeofaddr(const struct sockaddr *addr)
{
	switch (addr->sa_family) {
	case AF_INET:
		return sizeof(struct sockaddr_in);
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
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

uint16_t ofi_get_sa_family(const struct fi_info *info);

static inline int ofi_ipv4_is_any_addr(struct sockaddr *sa)
{
	struct in_addr ia_any = {
		.s_addr = INADDR_ANY,
	};

	if (!sa)
		return 0;

	return !memcmp(&ofi_sin_addr(sa).s_addr, &ia_any, sizeof(ia_any));

}

static inline int ofi_ipv6_is_any_addr(struct sockaddr *sa)
{
	struct in6_addr ia6_any = IN6ADDR_ANY_INIT;

	if (!sa)
		return 0;

	return !memcmp(&ofi_sin6_addr(sa), &ia6_any, sizeof(ia6_any));
}

static inline int ofi_is_any_addr(struct sockaddr *sa)
{
	if (!sa)
		return 0;

	switch(sa->sa_family) {
	case AF_INET:
		return ofi_ipv4_is_any_addr(sa);
	case AF_INET6:
		return ofi_ipv6_is_any_addr(sa);
	default:
		FI_WARN(&core_prov, FI_LOG_CORE, "Unknown address format!\n");
		return 0;
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
	default:
		FI_WARN(&core_prov, FI_LOG_FABRIC, "Unknown address format\n");
		assert(0);
		return 0;
	}
}

static inline void ofi_addr_set_port(struct sockaddr *addr, uint16_t port)
{
	switch (ofi_sa_family(addr)) {
	case AF_INET:
		ofi_sin_port(addr) = htons(port);
		break;
	case AF_INET6:
		ofi_sin6_port(addr) = htons(port);
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
	default:
		return NULL;
	}
}

static inline int ofi_equals_ipaddr(const struct sockaddr *addr1,
				    const struct sockaddr *addr2)
{
	if (addr1->sa_family != addr2->sa_family)
		return 0;

	switch (addr1->sa_family) {
	case AF_INET:
	        return !memcmp(&ofi_sin_addr(addr1), &ofi_sin_addr(addr2),
				sizeof(ofi_sin_addr(addr1)));
	case AF_INET6:
	        return !memcmp(&ofi_sin6_addr(addr1), &ofi_sin6_addr(addr2),
				sizeof(ofi_sin6_addr(addr1)));
	default:
		return 0;
	}
}

static inline int ofi_equals_sockaddr(const struct sockaddr *addr1,
				      const struct sockaddr *addr2)
{
        return (ofi_addr_get_port(addr1) == ofi_addr_get_port(addr2)) &&
		ofi_equals_ipaddr(addr1, addr2);
}

int ofi_is_wildcard_listen_addr(const char *node, const char *service,
				uint64_t flags, const struct fi_info *hints);

size_t ofi_mask_addr(struct sockaddr *maskaddr, const struct sockaddr *srcaddr,
		     const struct sockaddr *netmask);


/*
 * Address logging
 */
const char *ofi_straddr(char *buf, size_t *len,
			uint32_t addr_format, const void *addr);

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
