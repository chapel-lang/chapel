/*
 * Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006-2017 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2013-2018 Intel Corp., Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos Nat. Security, LLC. All rights reserved.
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

#include "config.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <sys/time.h>

#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#if HAVE_GETIFADDRS
#include <net/if.h>
#include <ifaddrs.h>
#endif

#include <ofi_signal.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/fi_errno.h>
#include <ofi.h>
#include <ofi_util.h>
#include <ofi_epoll.h>
#include <ofi_list.h>
#include <ofi_osd.h>
#include <shared/ofi_str.h>

struct fi_provider core_prov = {
	.name = "core",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST
};

struct ofi_common_locks common_locks = {
	.ini_lock = PTHREAD_MUTEX_INITIALIZER,
	.util_fabric_lock = PTHREAD_MUTEX_INITIALIZER,
};

int fi_poll_fd(int fd, int timeout)
{
	struct pollfd fds;
	int ret;

	fds.fd = fd;
	fds.events = POLLIN;
	ret = poll(&fds, 1, timeout);
	return ret == SOCKET_ERROR ? -ofi_sockerr() : ret;
}

uint64_t ofi_max_tag(uint64_t mem_tag_format)
{
	return mem_tag_format ? UINT64_MAX >> (64 - ofi_msb(mem_tag_format)) : 0;
}

uint64_t ofi_tag_format(uint64_t max_tag)
{
	return max_tag ? FI_TAG_GENERIC >> (64 - ofi_msb(max_tag)) : 0;
}

uint8_t ofi_msb(uint64_t num)
{
	uint8_t msb = 0;

	while (num) {
		msb++;
		num >>= 1;
	}
	return msb;
}

uint8_t ofi_lsb(uint64_t num)
{
	return ofi_msb(num & (~(num - 1)));
}

int ofi_send_allowed(uint64_t caps)
{
	if (caps & FI_MSG ||
		caps & FI_TAGGED) {
		if (caps & FI_SEND)
			return 1;
		if (caps & FI_RECV)
			return 0;
		return 1;
	}

	return 0;
}

int ofi_recv_allowed(uint64_t caps)
{
	if (caps & FI_MSG ||
		caps & FI_TAGGED) {
		if (caps & FI_RECV)
			return 1;
		if (caps & FI_SEND)
			return 0;
		return 1;
	}

	return 0;
}

int ofi_rma_initiate_allowed(uint64_t caps)
{
	if (caps & FI_RMA ||
		caps & FI_ATOMICS) {
		if (caps & FI_WRITE ||
			caps & FI_READ)
			return 1;
		if (caps & FI_REMOTE_WRITE ||
			caps & FI_REMOTE_READ)
			return 0;
		return 1;
	}

	return 0;
}

int ofi_rma_target_allowed(uint64_t caps)
{
	if (caps & FI_RMA ||
		caps & FI_ATOMICS) {
		if (caps & FI_REMOTE_WRITE ||
			caps & FI_REMOTE_READ)
			return 1;
		if (caps & FI_WRITE ||
			caps & FI_READ)
			return 0;
		return 1;
	}

	return 0;
}

int ofi_ep_bind_valid(const struct fi_provider *prov, struct fid *bfid, uint64_t flags)
{
	if (!bfid) {
		FI_WARN(prov, FI_LOG_EP_CTRL, "NULL bind fid\n");
		return -FI_EINVAL;
	}

	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		if (flags & ~(FI_TRANSMIT | FI_RECV | FI_SELECTIVE_COMPLETION)) {
			FI_WARN(prov, FI_LOG_EP_CTRL, "invalid CQ flags\n");
			return -FI_EBADFLAGS;
		}
		break;
	case FI_CLASS_CNTR:
		if (flags & ~(FI_SEND | FI_RECV | FI_READ | FI_WRITE |
			      FI_REMOTE_READ | FI_REMOTE_WRITE)) {
			FI_WARN(prov, FI_LOG_EP_CTRL, "invalid cntr flags\n");
			return -FI_EBADFLAGS;
		}
		break;
	default:
		if (flags) {
			FI_WARN(prov, FI_LOG_EP_CTRL, "invalid bind flags\n");
			return -FI_EBADFLAGS;
		}
		break;
	}
	return FI_SUCCESS;
}

int ofi_check_rx_mode(const struct fi_info *info, uint64_t flags)
{
	if (!info)
		return 0;

	if (info->rx_attr && (info->rx_attr->mode & flags))
		return 1;

	return (info->mode & flags) ? 1 : 0;
}

uint64_t ofi_gettime_ns(void)
{
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec * 1000000000 + now.tv_nsec;
}

uint64_t ofi_gettime_us(void)
{
	return ofi_gettime_ns() / 1000;
}

uint64_t ofi_gettime_ms(void)
{
	return ofi_gettime_ns() / 1000000;
}

uint16_t ofi_get_sa_family(const struct fi_info *info)
{
	if (!info)
		return 0;

	switch (info->addr_format) {
	case FI_SOCKADDR_IN:
		return AF_INET;
	case FI_SOCKADDR_IN6:
		return AF_INET6;
	case FI_SOCKADDR_IB:
		return AF_IB;
	case FI_SOCKADDR:
	case FI_FORMAT_UNSPEC:
		if (info->src_addr)
			return ((struct sockaddr *) info->src_addr)->sa_family;

		if (info->dest_addr)
			return ((struct sockaddr *) info->dest_addr)->sa_family;
		/* fall through */
	default:
		return 0;
	}
}

const char *ofi_straddr(char *buf, size_t *len,
			uint32_t addr_format, const void *addr)
{
	const struct sockaddr *sock_addr;
	const struct sockaddr_in6 *sin6;
	const struct sockaddr_in *sin;
	char str[INET6_ADDRSTRLEN + 8];
	size_t size;

	if (!addr || !len)
		return NULL;

	switch (addr_format) {
	case FI_SOCKADDR:
		sock_addr = addr;
		switch (sock_addr->sa_family) {
		case AF_INET:
			goto sa_sin;
		case AF_INET6:
			goto sa_sin6;
		default:
			return NULL;
		}
		break;
	case FI_SOCKADDR_IN:
sa_sin:
		sin = addr;
		if (!inet_ntop(sin->sin_family, &sin->sin_addr, str,
			       sizeof(str)))
			return NULL;

		size = snprintf(buf, MIN(*len, sizeof(str)),
				"fi_sockaddr_in://%s:%" PRIu16, str,
				ntohs(sin->sin_port));
		break;
	case FI_SOCKADDR_IN6:
sa_sin6:
		sin6 = addr;
		if (!inet_ntop(sin6->sin6_family, &sin6->sin6_addr, str,
			       sizeof(str)))
			return NULL;

		size = snprintf(buf, MIN(*len, sizeof(str)),
				"fi_sockaddr_in6://[%s]:%" PRIu16, str,
				ntohs(sin6->sin6_port));
		break;
	case FI_ADDR_EFA:
		memset(str, 0, sizeof(str));
		if (!inet_ntop(AF_INET6, addr, str, INET6_ADDRSTRLEN))
			return NULL;
		size = snprintf(buf, *len, "fi_addr_efa://[%s]:%" PRIu16 ":%" PRIu32,
				str, *((uint16_t *)addr + 8), *((uint32_t *)addr + 5));
		break;
	case FI_SOCKADDR_IB:
		size = snprintf(buf, *len, "fi_sockaddr_ib://%p", addr);
		break;
	case FI_ADDR_PSMX:
		size = snprintf(buf, *len, "fi_addr_psmx://%" PRIx64,
				*(uint64_t *)addr);
		break;
	case FI_ADDR_PSMX2:
		size =
		    snprintf(buf, *len, "fi_addr_psmx2://%" PRIx64 ":%" PRIx64,
			     *(uint64_t *)addr, *((uint64_t *)addr + 1));
		break;
	case FI_ADDR_GNI:
		size = snprintf(buf, *len, "fi_addr_gni://%" PRIx64,
				*(uint64_t *)addr);
		break;
	case FI_ADDR_BGQ:
		size = snprintf(buf, *len, "fi_addr_bgq://%p", addr);
		break;
	case FI_ADDR_MLX:
		size = snprintf(buf, *len, "fi_addr_mlx://%p", addr);
		break;
	case FI_ADDR_IB_UD:
		memset(str, 0, sizeof(str));
		if (!inet_ntop(AF_INET6, addr, str, INET6_ADDRSTRLEN))
			return NULL;
		size = snprintf(buf, *len, "fi_addr_ib_ud://"
				"%s" /* GID */ ":%" PRIx32 /* QPN */
				"/%" PRIx16 /* LID */ "/%" PRIx16 /* P_Key */
				"/%" PRIx8 /* SL */,
				str, *((uint32_t *)addr + 4),
				*((uint16_t *)addr + 10),
				*((uint16_t *)addr + 11),
				*((uint8_t *)addr + 26));
		break;
	case FI_ADDR_STR:
		size = snprintf(buf, *len, "%s", (const char *) addr);
		break;
	default:
		return NULL;
	}

	/* Make sure that possibly truncated messages have a null terminator. */
	if (buf && *len)
		buf[*len - 1] = '\0';
	*len = size + 1;
	return buf;
}

static uint32_t ofi_addr_format(const char *str)
{
	char fmt[16];
	int ret;

	ret = sscanf(str, "%16[^:]://", fmt);
	if (ret != 1)
		return FI_FORMAT_UNSPEC;

	fmt[sizeof(fmt) - 1] = '\0';
	if (!strcasecmp(fmt, "fi_sockaddr_in"))
		return FI_SOCKADDR_IN;
	else if (!strcasecmp(fmt, "fi_sockaddr_in6"))
		return FI_SOCKADDR_IN6;
	else if (!strcasecmp(fmt, "fi_sockaddr_ib"))
		return FI_SOCKADDR_IB;
	else if (!strcasecmp(fmt, "fi_addr_psmx"))
		return FI_ADDR_PSMX;
	else if (!strcasecmp(fmt, "fi_addr_psmx2"))
		return FI_ADDR_PSMX2;
	else if (!strcasecmp(fmt, "fi_addr_gni"))
		return FI_ADDR_GNI;
	else if (!strcasecmp(fmt, "fi_addr_bgq"))
		return FI_ADDR_BGQ;
	else if (!strcasecmp(fmt, "fi_addr_efa"))
		return FI_ADDR_EFA;
	else if (!strcasecmp(fmt, "fi_addr_mlx"))
		return FI_ADDR_MLX;
	else if (!strcasecmp(fmt, "fi_addr_ib_ud"))
		return FI_ADDR_IB_UD;

	return FI_FORMAT_UNSPEC;
}

static int ofi_str_to_psmx(const char *str, void **addr, size_t *len)
{
	int ret;

	*len = sizeof(uint64_t);
	*addr = calloc(1, *len);
	if (!(*addr))
		return -FI_ENOMEM;

	ret = sscanf(str, "%*[^:]://%" SCNx64, (uint64_t *) *addr);
	if (ret == 1)
		return 0;

	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_psmx2(const char *str, void **addr, size_t *len)
{
	int ret;

	*len = 2 * sizeof(uint64_t);
	*addr = calloc(1, *len);
	if (!(*addr))
		return -FI_ENOMEM;

	ret = sscanf(str, "%*[^:]://%" SCNx64 ":%" SCNx64,
		     (uint64_t *) *addr, (uint64_t *) *addr + 1);
	if (ret == 2)
		return 0;

	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_ib_ud(const char *str, void **addr, size_t *len)
{
	int ret;
	char gid[INET6_ADDRSTRLEN];

	memset(gid, 0, sizeof(gid));

	*len = 32;
	*addr = calloc(1, *len);
	if(!(*addr))
		return -FI_ENOMEM;

	ret = sscanf(str, "%*[^:]://"
		     "%s" /* GID */ ":%" SCNx32 /* QPN */
		     ":%" SCNx16 /* LID */ ":%" SCNx16 /* P_Key */
		     ":%" SCNx8 /* SL */,
		     gid, (uint32_t *)*addr + 4,
		     (uint16_t *)*addr + 10,
		     (uint16_t *)*addr + 11,
		     (uint8_t *)*addr + 26);
	if ((ret == 5) && (inet_pton(AF_INET6, gid, *addr) > 0))
		return FI_SUCCESS;

	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_efa(const char *str, void **addr, size_t *len)
{
	char gid[INET6_ADDRSTRLEN];
	uint16_t *qpn;
	uint32_t *qkey;
	int ret;

	memset(gid, 0, sizeof(gid));

	*len = 24;
	*addr = calloc(1, *len);
	if (!*addr)
		return -FI_ENOMEM;
	qpn = (uint16_t *)*addr + 8;
	qkey = (uint32_t *)*addr + 5;
	ret = sscanf(str, "%*[^:]://[%64[^]]]:%" SCNu16 ":%" SCNu32, gid, qpn, qkey);
	if (ret < 1)
		goto err;

	if (inet_pton(AF_INET6, gid, *addr) > 0)
		return FI_SUCCESS;

err:
	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_sin(const char *str, void **addr, size_t *len)
{
	struct sockaddr_in *sin;
	char ip[64];
	int ret;

	*len = sizeof(*sin);
	sin = calloc(1, *len);
	if (!sin)
		return -FI_ENOMEM;

	sin->sin_family = AF_INET;
	ret = sscanf(str, "%*[^:]://:%" SCNu16, &sin->sin_port);
	if (ret == 1)
		goto match_port;

	ret = sscanf(str, "%*[^:]://%64[^:]:%" SCNu16, ip, &sin->sin_port);
	if (ret == 2)
		goto match_ip;

	ret = sscanf(str, "%*[^:]://%64[^:/]", ip);
	if (ret == 1)
		goto match_ip;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Malformed FI_ADDR_STR: %s\n", str);
err:
	free(sin);
	return -FI_EINVAL;

match_ip:
	ip[sizeof(ip) - 1] = '\0';
	ret = inet_pton(AF_INET, ip, &sin->sin_addr);
	if (ret != 1) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to convert IPv4 address: %s\n", ip);
		goto err;
	}

match_port:
	sin->sin_port = htons(sin->sin_port);
	*addr = sin;
	return 0;
}

static int ofi_str_to_sin6(const char *str, void **addr, size_t *len)
{
	struct sockaddr_in6 *sin6;
	char ip[64];
	int ret;

	*len = sizeof(*sin6);
	sin6 = calloc(1, *len);
	if (!sin6)
		return -FI_ENOMEM;

	sin6->sin6_family = AF_INET6;
	ret = sscanf(str, "%*[^:]://:%" SCNu16, &sin6->sin6_port);
	if (ret == 1)
		goto match_port;

	ret = sscanf(str, "%*[^:]://[%64[^]]]:%" SCNu16, ip, &sin6->sin6_port);
	if (ret == 2)
		goto match_ip;

	ret = sscanf(str, "%*[^:]://[%64[^]]", ip);
	if (ret == 1)
		goto match_ip;

	FI_WARN(&core_prov, FI_LOG_CORE,
		"Malformed FI_ADDR_STR: %s\n", str);
err:
	free(sin6);
	return -FI_EINVAL;

match_ip:
	ip[sizeof(ip) - 1] = '\0';
	ret = inet_pton(AF_INET6, ip, &sin6->sin6_addr);
	if (ret != 1) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unable to convert IPv6 address: %s\n", ip);
		goto err;
	}

match_port:
	sin6->sin6_port = htons(sin6->sin6_port);
	*addr = sin6;
	return 0;
}

static int ofi_hostname_toaddr(const char *name, uint32_t *addr_format,
			       void **addr, size_t *len)
{
	struct addrinfo *ai;
	int ret;

	ret = getaddrinfo(name, NULL, NULL, &ai);
	if (ret)
		return ret;

	*addr_format = (ai->ai_family == AF_INET6) ? FI_SOCKADDR_IN6 : FI_SOCKADDR_IN;
	*len = ai->ai_addrlen;
	*addr = calloc(1, *len);
	if (!*addr) {
		ret = -FI_ENOMEM;
		goto out;
	}

	memcpy(*addr, ai->ai_addr, *len);

out:
	freeaddrinfo(ai);
	return ret;
}

static int ofi_ifname_toaddr(const char *name, uint32_t *addr_format,
			     void **addr, size_t *len)
{
#if HAVE_GETIFADDRS
	struct ifaddrs *ifaddrs, *ifa;
	int ret;

	ret = ofi_getifaddrs(&ifaddrs);
	if (ret)
		return ret;

	for (ifa = ifaddrs; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family != AF_INET &&
		    ifa->ifa_addr->sa_family != AF_INET6)
			continue;
		if (!strcmp(name, ifa->ifa_name))
			break;
	}

	if (!ifa) {
		ret = -FI_EINVAL;
		goto out;
	}

	if (ifa->ifa_addr->sa_family == AF_INET6) {
		*addr_format = FI_SOCKADDR_IN6;
		*len = sizeof(struct sockaddr_in6);
	} else {
		*addr_format = FI_SOCKADDR_IN;
		*len = sizeof(struct sockaddr_in);
	}

	*addr = calloc(1, *len);
	if (!*addr) {
		ret = -FI_ENOMEM;
		goto out;
	}

	memcpy(*addr, ifa->ifa_addr, *len);

out:
	freeifaddrs(ifaddrs);
	return ret;
#else
	return -FI_ENOSYS;
#endif
}

int ofi_str_toaddr(const char *str, uint32_t *addr_format,
		   void **addr, size_t *len)
{
	*addr_format = ofi_addr_format(str);

	switch (*addr_format) {
	case FI_FORMAT_UNSPEC:
		if (!ofi_ifname_toaddr(str, addr_format, addr, len))
			return 0;
		if (!ofi_hostname_toaddr(str, addr_format, addr, len))
			return 0;
		return -FI_EINVAL;
	case FI_SOCKADDR_IN:
		return ofi_str_to_sin(str, addr, len);
	case FI_SOCKADDR_IN6:
		return ofi_str_to_sin6(str, addr, len);
	case FI_ADDR_PSMX:
		return ofi_str_to_psmx(str, addr, len);
	case FI_ADDR_PSMX2:
		return ofi_str_to_psmx2(str, addr, len);
	case FI_ADDR_IB_UD:
		return ofi_str_to_ib_ud(str, addr, len);
	case FI_ADDR_EFA:
		return ofi_str_to_efa(str, addr, len);
	case FI_SOCKADDR_IB:
	case FI_ADDR_GNI:
	case FI_ADDR_BGQ:
	case FI_ADDR_MLX:
	default:
		return -FI_ENOSYS;
	}
}

const char *ofi_hex_str(const uint8_t *data, size_t len)
{
	static char str[64];
	const char hex[] = "0123456789abcdef";
	size_t i, p;

	if (len >= (sizeof(str) >> 1))
		len = (sizeof(str) >> 1) - 1;

	for (p = 0, i = 0; i < len; i++) {
		str[p++] = hex[data[i] >> 4];
		str[p++] = hex[data[i] & 0xF];
	}

	if (len == (sizeof(str) >> 1) - 1)
		str[p++] = '~';

	str[p] = '\0';
	return str;
}

int ofi_addr_cmp(const struct fi_provider *prov, const struct sockaddr *sa1,
		 const struct sockaddr *sa2)
{
	int cmp;

	switch (sa1->sa_family) {
	case AF_INET:
		cmp = memcmp(&ofi_sin_addr(sa1), &ofi_sin_addr(sa2),
			     sizeof(ofi_sin_addr(sa1)));
		return cmp ? cmp : memcmp(&ofi_sin_port(sa1),
					  &ofi_sin_port(sa2),
					  sizeof(ofi_sin_port(sa1)));
	case AF_INET6:
		cmp = memcmp(&ofi_sin6_addr(sa1), &ofi_sin6_addr(sa2),
			     sizeof(ofi_sin6_addr(sa1)));
		return cmp ? cmp : memcmp(&ofi_sin6_port(sa1),
					  &ofi_sin_port(sa2),
					  sizeof(ofi_sin6_port(sa1)));
	default:
		FI_WARN(prov, FI_LOG_FABRIC, "Invalid address format!\n");
		assert(0);
		return 0;
	}
}

static int ofi_is_any_addr_port(struct sockaddr *addr)
{
	switch (ofi_sa_family(addr)) {
	case AF_INET:
		return (ofi_ipv4_is_any_addr(addr) &&
			ofi_sin_port(addr));
	case AF_INET6:
		return (ofi_ipv6_is_any_addr(addr) &&
			ofi_sin6_port(addr));
	default:
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unknown address format\n");
		return 0;
	}
}

int ofi_is_wildcard_listen_addr(const char *node, const char *service,
				uint64_t flags, const struct fi_info *hints)
{
	struct addrinfo *res = NULL;
	int ret;

	if (hints && hints->addr_format != FI_FORMAT_UNSPEC &&
	    hints->addr_format != FI_SOCKADDR &&
	    hints->addr_format != FI_SOCKADDR_IN &&
	    hints->addr_format != FI_SOCKADDR_IN6)
		return 0;

	/* else it's okay to call getaddrinfo, proceed with processing */

	if (node) {
		if (!(flags & FI_SOURCE))
			return 0;
		ret = getaddrinfo(node, service, NULL, &res);
		if (ret) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"getaddrinfo failed!\n");
			return 0;
		}
		if (ofi_is_any_addr_port(res->ai_addr)) {
			freeaddrinfo(res);
			goto out;
		}
		freeaddrinfo(res);
		return 0;
	}

	if (hints) {
		if (hints->dest_addr)
			return 0;

		if (!hints->src_addr)
			goto out;

		return ofi_is_any_addr_port(hints->src_addr);
	}
out:
	return ((flags & FI_SOURCE) && service) ? 1 : 0;
}

size_t ofi_mask_addr(struct sockaddr *maskaddr, const struct sockaddr *srcaddr,
		     const struct sockaddr *netmask)
{
	size_t i, size, len = 0;
	uint8_t *ip, *mask, bits;

	memcpy(maskaddr, srcaddr, ofi_sizeofaddr(srcaddr));
	size = ofi_sizeofip(srcaddr);
	ip = ofi_get_ipaddr(maskaddr);
	mask = ofi_get_ipaddr(netmask);

	if (!size || !ip || !mask)
		return 0;

	for (i = 0; i < size; i++) {
		ip[i] &= mask[i];

		if (mask[i] == 0xff) {
			len += 8;
		} else {
			for (bits = mask[i]; bits; bits >>= 1) {
				if (bits & 0x1)
					len++;
			}
		}
	}
	return len;
}

void ofi_straddr_log_internal(const char *func, int line,
			      const struct fi_provider *prov,
			      enum fi_log_level level,
			      enum fi_log_subsys subsys, char *log_str,
			      const void *addr)
{
	char buf[OFI_ADDRSTRLEN];
	uint32_t addr_format;
	size_t len = sizeof(buf);

	if (fi_log_enabled(prov, level, subsys)) {
		addr_format = ofi_translate_addr_format(ofi_sa_family(addr));
		fi_log(prov, level, subsys, func, line, "%s: %s\n", log_str,
		       ofi_straddr(buf, &len, addr_format, addr));
	}
}

int ofi_discard_socket(SOCKET sock, size_t len)
{
	char buf;
	ssize_t ret = 0;

	for (; len && !ret; len--)
		ret = ofi_recvall_socket(sock, &buf, 1);
	return ret;
}


int ofi_pollfds_create(struct ofi_pollfds **pfds)
{
	int ret;

	*pfds = calloc(1, sizeof(struct ofi_pollfds));
	if (!*pfds)
		return -FI_ENOMEM;

	(*pfds)->size = 64;
	(*pfds)->fds = calloc((*pfds)->size, sizeof(*(*pfds)->fds) +
			    sizeof(*(*pfds)->context));
	if (!(*pfds)->fds) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	(*pfds)->context = (void *)((*pfds)->fds + (*pfds)->size);

	ret = fd_signal_init(&(*pfds)->signal);
	if (ret)
		goto err2;

	(*pfds)->fds[(*pfds)->nfds].fd = (*pfds)->signal.fd[FI_READ_FD];
	(*pfds)->fds[(*pfds)->nfds].events = POLLIN;
	(*pfds)->context[(*pfds)->nfds++] = NULL;
	slist_init(&(*pfds)->work_item_list);
	fastlock_init(&(*pfds)->lock);
	return FI_SUCCESS;
err2:
	free((*pfds)->fds);
err1:
	free(*pfds);
	return ret;
}

static int ofi_pollfds_ctl(struct ofi_pollfds *pfds, enum ofi_pollfds_ctl op,
			   int fd, uint32_t events, void *context)
{
	struct ofi_pollfds_work_item *item;

	item = calloc(1,sizeof(*item));
	if (!item)
		return -FI_ENOMEM;

	item->fd = fd;
	item->events = events;
	item->context = context;
	item->type = op;
	fastlock_acquire(&pfds->lock);
	slist_insert_tail(&item->entry, &pfds->work_item_list);
	fd_signal_set(&pfds->signal);
	fastlock_release(&pfds->lock);
	return 0;
}

int ofi_pollfds_add(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context)
{
	return ofi_pollfds_ctl(pfds, POLLFDS_CTL_ADD, fd, events, context);
}

int ofi_pollfds_mod(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context)
{
	return ofi_pollfds_ctl(pfds, POLLFDS_CTL_MOD, fd, events, context);
}

int ofi_pollfds_del(struct ofi_pollfds *pfds, int fd)
{
	return ofi_pollfds_ctl(pfds, POLLFDS_CTL_DEL, fd, 0, NULL);
}

static int ofi_pollfds_array(struct ofi_pollfds *pfds)
{
	struct pollfd *fds;
	void *contexts;

	fds = calloc(pfds->size + 64,
		     sizeof(*pfds->fds) + sizeof(*pfds->context));
	if (!fds)
		return -FI_ENOMEM;

	pfds->size += 64;
	contexts = fds + pfds->size;

	memcpy(fds, pfds->fds, pfds->nfds * sizeof(*pfds->fds));
	memcpy(contexts, pfds->context, pfds->nfds * sizeof(*pfds->context));
	free(pfds->fds);
	pfds->fds = fds;
	pfds->context = contexts;
	return FI_SUCCESS;
}

static void ofi_pollfds_cleanup(struct ofi_pollfds *pfds)
{
	int i;

	for (i = 0; i < pfds->nfds; i++) {
		while (pfds->fds[i].fd == INVALID_SOCKET) {
			pfds->fds[i].fd = pfds->fds[pfds->nfds-1].fd;
			pfds->fds[i].events = pfds->fds[pfds->nfds-1].events;
			pfds->fds[i].revents = pfds->fds[pfds->nfds-1].revents;
			pfds->context[i] = pfds->context[pfds->nfds-1];
			pfds->nfds--;
			if (i == pfds->nfds)
				break;
		}
	}
}

static void ofi_pollfds_process_work(struct ofi_pollfds *pfds)
{
	struct slist_entry *entry;
	struct ofi_pollfds_work_item *item;
	int i;

	while (!slist_empty(&pfds->work_item_list)) {
		if ((pfds->nfds == pfds->size) &&
		    ofi_pollfds_array(pfds))
			continue;

		entry = slist_remove_head(&pfds->work_item_list);
		item = container_of(entry, struct ofi_pollfds_work_item, entry);

		switch (item->type) {
		case POLLFDS_CTL_ADD:
			pfds->fds[pfds->nfds].fd = item->fd;
			pfds->fds[pfds->nfds].events = item->events;
			pfds->fds[pfds->nfds].revents = 0;
			pfds->context[pfds->nfds] = item->context;
			pfds->nfds++;
			break;
		case POLLFDS_CTL_DEL:
			for (i = 0; i < pfds->nfds; i++) {
				if (pfds->fds[i].fd == item->fd) {
					pfds->fds[i].fd = INVALID_SOCKET;
					break;
				}
			}
			break;
		case POLLFDS_CTL_MOD:
			for (i = 0; i < pfds->nfds; i++) {
				if (pfds->fds[i].fd == item->fd) {
					pfds->fds[i].events = item->events;
					pfds->fds[i].revents &= item->events;
					pfds->context[i] = item->context;
					break;
				}
			}
			break;
		default:
			assert(0);
			goto out;
		}
		free(item);
	}
out:
	ofi_pollfds_cleanup(pfds);
}

int ofi_pollfds_wait(struct ofi_pollfds *pfds, void **contexts,
		     int max_contexts, int timeout)
{
	int i, ret;
	int found = 0;
	uint64_t start = (timeout >= 0) ? ofi_gettime_ms() : 0;

	do {
		ret = poll(pfds->fds, pfds->nfds, timeout);
		if (ret == SOCKET_ERROR)
			return -ofi_sockerr();
		else if (ret == 0)
			return 0;

		if (pfds->fds[0].revents)
			fd_signal_reset(&pfds->signal);

		fastlock_acquire(&pfds->lock);
		if (!slist_empty(&pfds->work_item_list))
			ofi_pollfds_process_work(pfds);

		fastlock_release(&pfds->lock);

		/* Index 0 is the internal signaling fd, skip it */
		for (i = pfds->index; i < pfds->nfds && found < max_contexts; i++) {
			if (pfds->fds[i].revents && i) {
				contexts[found++] = pfds->context[i];
				pfds->index = i;
			}
		}
		for (i = 0; i < pfds->index && found < max_contexts; i++) {
			if (pfds->fds[i].revents && i) {
				contexts[found++] = pfds->context[i];
				pfds->index = i;
			}
		}

		if (timeout > 0)
			timeout -= (int) (ofi_gettime_ms() - start);

	} while (timeout > 0 && !found);

	return found;
}

void ofi_pollfds_close(struct ofi_pollfds *pfds)
{
	struct ofi_pollfds_work_item *item;
	struct slist_entry *entry;

	if (pfds) {
		while (!slist_empty(&pfds->work_item_list)) {
			entry = slist_remove_head(&pfds->work_item_list);
			item = container_of(entry,
					    struct ofi_pollfds_work_item,
					    entry);
			free(item);
		}
		fastlock_destroy(&pfds->lock);
		fd_signal_free(&pfds->signal);
		free(pfds->fds);
		free(pfds);
	}
}


void ofi_free_list_of_addr(struct slist *addr_list)
{
	struct ofi_addr_list_entry *addr_entry;

	while (!slist_empty(addr_list)) {
		slist_remove_head_container(addr_list, struct ofi_addr_list_entry,
					    addr_entry, entry);
		free(addr_entry);
	}
}

static inline
void ofi_insert_loopback_addr(const struct fi_provider *prov, struct slist *addr_list)
{
	struct ofi_addr_list_entry *addr_entry;

	addr_entry = calloc(1, sizeof(struct ofi_addr_list_entry));
	if (!addr_entry)
		return;

	addr_entry->ipaddr.sin.sin_family = AF_INET;
	addr_entry->ipaddr.sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	ofi_straddr_log(prov, FI_LOG_INFO, FI_LOG_CORE,
			"available addr: ", &addr_entry->ipaddr);

	strncpy(addr_entry->ipstr, "127.0.0.1", sizeof(addr_entry->ipstr));
	strncpy(addr_entry->net_name, "127.0.0.1/32", sizeof(addr_entry->net_name));
	strncpy(addr_entry->ifa_name, "lo", sizeof(addr_entry->ifa_name));
	slist_insert_tail(&addr_entry->entry, addr_list);

	addr_entry = calloc(1, sizeof(struct ofi_addr_list_entry));
	if (!addr_entry)
		return;

	addr_entry->ipaddr.sin6.sin6_family = AF_INET6;
	addr_entry->ipaddr.sin6.sin6_addr = in6addr_loopback;
	ofi_straddr_log(prov, FI_LOG_INFO, FI_LOG_CORE,
			"available addr: ", &addr_entry->ipaddr);

	strncpy(addr_entry->ipstr, "::1", sizeof(addr_entry->ipstr));
	strncpy(addr_entry->net_name, "::1/128", sizeof(addr_entry->net_name));
	strncpy(addr_entry->ifa_name, "lo", sizeof(addr_entry->ifa_name));
	slist_insert_tail(&addr_entry->entry, addr_list);
}

#if HAVE_GETIFADDRS

/* getifaddrs can fail when connecting the netlink socket. Try again
 * as this is a temporary error. After the 2nd retry, sleep a bit as
 * well in case the host is really busy. */
#define MAX_GIA_RETRIES 10
int ofi_getifaddrs(struct ifaddrs **ifaddr)
{
	unsigned int retries;
	int ret;

	for (retries = 0; retries < MAX_GIA_RETRIES; retries++) {
		if (retries > 1) {
			/* Exponentiation sleep after the 2nd try.
			 * 1000 << 9 is 512000, which respects the 1s
			 * constraint for usleep. */
			usleep(1000 << retries);
		}

		ret = getifaddrs(ifaddr);
		if (ret == 0 || errno != ECONNREFUSED)
			break;
	}

	if (ret != 0)
		return -errno;

	return FI_SUCCESS;
}

static int
ofi_addr_list_entry_comp_speed(struct slist_entry *cur, const void *insert)
{
	const struct ofi_addr_list_entry *cur_addr =
		container_of(cur, struct ofi_addr_list_entry, entry);
	const struct ofi_addr_list_entry *insert_addr =
		container_of((const struct slist_entry *) insert,
			     struct ofi_addr_list_entry, entry);

	return (cur_addr->speed < insert_addr->speed);
}

void ofi_set_netmask_str(char *netstr, size_t len, struct ifaddrs *ifa)
{
	union ofi_sock_ip addr;
	size_t prefix_len;

	netstr[0] = '\0';
	prefix_len = ofi_mask_addr(&addr.sa, ifa->ifa_addr, ifa->ifa_netmask);

	switch (addr.sa.sa_family) {
	case AF_INET:
		inet_ntop(AF_INET, &addr.sin.sin_addr, netstr, len);
		break;
	case AF_INET6:
		inet_ntop(AF_INET6, &addr.sin6.sin6_addr, netstr, len);
		break;
	default:
		snprintf(netstr, len, "%s", "<unknown>");
		netstr[len - 1] = '\0';
		break;
	}

	snprintf(netstr + strlen(netstr), len - strlen(netstr),
		 "%s%d", "/", (int) prefix_len);
	netstr[len - 1] = '\0';
}

void ofi_get_list_of_addr(const struct fi_provider *prov, const char *env_name,
			  struct slist *addr_list)
{
	int ret;
	char *iface = NULL;
	struct ofi_addr_list_entry *addr_entry;
	struct ifaddrs *ifaddrs, *ifa;

	fi_param_get_str((struct fi_provider *) prov, env_name, &iface);

	ret = ofi_getifaddrs(&ifaddrs);
	if (ret)
		goto insert_lo;

	if (iface) {
		for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
			if (strncmp(iface, ifa->ifa_name,
					strlen(iface)) == 0) {
				break;
			}
		}
		if (ifa == NULL) {
			FI_INFO(prov, FI_LOG_CORE,
				"Can't set filter to unknown interface: (%s)\n",
				iface);
			iface = NULL;
		}
	}
	for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL ||
			!(ifa->ifa_flags & IFF_UP) ||
			(ifa->ifa_flags & IFF_LOOPBACK) ||
			((ifa->ifa_addr->sa_family != AF_INET) &&
			(ifa->ifa_addr->sa_family != AF_INET6)))
			continue;
		if (iface && strncmp(iface, ifa->ifa_name, strlen(iface)) != 0) {
			FI_DBG(prov, FI_LOG_CORE,
				"Skip (%s) interface\n", ifa->ifa_name);
			continue;
		}

		addr_entry = calloc(1, sizeof(*addr_entry));
		if (!addr_entry)
			continue;

		memcpy(&addr_entry->ipaddr, ifa->ifa_addr,
			ofi_sizeofaddr(ifa->ifa_addr));
		strncpy(addr_entry->ifa_name, ifa->ifa_name,
			sizeof(addr_entry->ifa_name));
		ofi_set_netmask_str(addr_entry->net_name,
				    sizeof(addr_entry->net_name), ifa);

		if (!inet_ntop(ifa->ifa_addr->sa_family,
				ofi_get_ipaddr(ifa->ifa_addr),
				addr_entry->ipstr,
				sizeof(addr_entry->ipstr))) {
			FI_DBG(prov, FI_LOG_CORE,
				"inet_ntop failed: %d\n", errno);
			free(addr_entry);
			continue;
		}

		addr_entry->speed = ofi_ifaddr_get_speed(ifa);
		FI_INFO(prov, FI_LOG_CORE, "Available addr: %s, "
			"iface name: %s, speed: %zu\n",
			addr_entry->ipstr, ifa->ifa_name, addr_entry->speed);

		slist_insert_before_first_match(addr_list, ofi_addr_list_entry_comp_speed,
						&addr_entry->entry);
	}

	freeifaddrs(ifaddrs);

insert_lo:
	/* Always add loopback address at the end */
	ofi_insert_loopback_addr(prov, addr_list);
}

#elif defined HAVE_MIB_IPADDRTABLE

void ofi_get_list_of_addr(const struct fi_provider *prov, const char *env_name,
			  struct slist *addr_list)
{
	struct ofi_addr_list_entry *addr_entry;
	DWORD i;
	MIB_IPADDRTABLE _iptbl;
	MIB_IPADDRTABLE *iptbl = &_iptbl;
	ULONG ips = 1;
	ULONG res;

	res = GetIpAddrTable(iptbl, &ips, 0);
	if (res == ERROR_INSUFFICIENT_BUFFER) {
		iptbl = malloc(ips);
		if (!iptbl)
			return;

		res = GetIpAddrTable(iptbl, &ips, 0);
	}

	if (res != NO_ERROR)
		goto out;

	for (i = 0; i < iptbl->dwNumEntries; i++) {
		if (iptbl->table[i].dwAddr &&
		    (iptbl->table[i].dwAddr != htonl(INADDR_LOOPBACK))) {
			addr_entry = calloc(1, sizeof(*addr_entry));
			if (!addr_entry)
				break;

			addr_entry->ipaddr.sin.sin_family = AF_INET;
			addr_entry->ipaddr.sin.sin_addr.s_addr =
						iptbl->table[i].dwAddr;
			inet_ntop(AF_INET, &iptbl->table[i].dwAddr,
				  addr_entry->ipstr,
				  sizeof(addr_entry->ipstr));
			slist_insert_tail(&addr_entry->entry, addr_list);
		}
	}

	/* Always add loopback address at the end */
	ofi_insert_loopback_addr(prov, addr_list);

out:
	if (iptbl != &_iptbl)
		free(iptbl);
}

#else /* !HAVE_MIB_IPADDRTABLE && !HAVE_MIB_IPADDRTABLE */

void ofi_get_list_of_addr(const struct fi_provider *prov, const char *env_name,
			  struct slist *addr_list)
{
	ofi_insert_loopback_addr(prov, addr_list);
}
#endif

int ofi_cpu_supports(unsigned func, unsigned reg, unsigned bit)
{
	unsigned cpuinfo[4] = { 0 };

	ofi_cpuid(0, 0, cpuinfo);
	if (cpuinfo[0] < func)
		return 0;

	ofi_cpuid(func, 0, cpuinfo);
	return cpuinfo[reg] & bit;
}

void ofi_remove_comma(char *buffer)
{
	size_t sz = strlen(buffer);
	if (sz < 2)
		return;
	if (strcmp(&buffer[sz-2], ", ") == 0)
		buffer[sz-2] = '\0';
}

void ofi_strncatf(char *dest, size_t n, const char *fmt, ...)
{
	size_t len = strnlen(dest, n);
	va_list arglist;

	va_start(arglist, fmt);
	vsnprintf(&dest[len], n - 1 - len, fmt, arglist);
	va_end(arglist);
}

/* The provider must free any prov_attr data prior to calling this
 * routine.
 */
int ofi_nic_close(struct fid *fid)
{
	struct fid_nic *nic = (struct fid_nic *) fid;

	assert(fid && fid->fclass == FI_CLASS_NIC);

	if (nic->device_attr) {
		free(nic->device_attr->name);
		free(nic->device_attr->device_id);
		free(nic->device_attr->device_version);
		free(nic->device_attr->vendor_id);
		free(nic->device_attr->driver);
		free(nic->device_attr->firmware);
		free(nic->device_attr);
	}

	free(nic->bus_attr);

	if (nic->link_attr) {
		free(nic->link_attr->address);
		free(nic->link_attr->network_type);
		free(nic->link_attr);
	}

	free(nic);
	return 0;
}

int ofi_nic_control(struct fid *fid, int command, void *arg)
{
	struct fid_nic *nic = container_of(fid, struct fid_nic, fid);
	struct fid_nic **dup = (struct fid_nic **) arg;

	switch(command) {
	case FI_DUP:
		*dup = ofi_nic_dup(nic);
		return *dup ? FI_SUCCESS : -FI_ENOMEM;
	default:
		return -FI_ENOSYS;
	}
}

static void ofi_tostr_device_attr(char *buf, size_t len,
				  const struct fi_device_attr *attr)
{
	const char *prefix = TAB TAB;

	ofi_strncatf(buf, len, "%sfi_device_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%sname: %s\n", prefix, attr->name);
	ofi_strncatf(buf, len, "%sdevice_id: %s\n", prefix, attr->device_id);
	ofi_strncatf(buf, len, "%sdevice_version: %s\n", prefix,
		     attr->device_version);
	ofi_strncatf(buf, len, "%svendor_id: %s\n", prefix, attr->vendor_id);
	ofi_strncatf(buf, len, "%sdriver: %s\n", prefix, attr->driver);
	ofi_strncatf(buf, len, "%sfirmware: %s\n", prefix, attr->firmware);
}

static void ofi_tostr_pci_attr(char *buf, size_t len,
			       const struct fi_pci_attr *attr)
{
	const char *prefix = TAB TAB TAB;

	ofi_strncatf(buf, len, "%sfi_pci_attr:\n", prefix);

	prefix = TAB TAB TAB TAB;
	ofi_strncatf(buf, len, "%sdomain_id: %u\n", prefix, attr->domain_id);
	ofi_strncatf(buf, len, "%sbus_id: %u\n", prefix, attr->bus_id);
	ofi_strncatf(buf, len, "%sdevice_id: %u\n", prefix, attr->device_id);
	ofi_strncatf(buf, len, "%sfunction_id: %u\n", prefix, attr->function_id);
}

static void ofi_tostr_bus_type(char *buf, size_t len, int type)
{
	switch (type) {
	CASEENUMSTRN(FI_BUS_UNKNOWN, len);
	CASEENUMSTRN(FI_BUS_PCI, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_bus_attr(char *buf, size_t len,
			       const struct fi_bus_attr *attr)
{
	const char *prefix = TAB TAB;

	ofi_strncatf(buf, len, "%sfi_bus_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%sfi_bus_type: ", prefix);
	ofi_tostr_bus_type(buf, len, attr->bus_type);
	ofi_strncatf(buf, len, "\n");

	switch (attr->bus_type) {
	case FI_BUS_PCI:
		ofi_tostr_pci_attr(buf, len, &attr->attr.pci);
		break;
	default:
		break;
	}
}

static void ofi_tostr_link_state(char *buf, size_t len, int state)
{
	switch (state) {
	CASEENUMSTRN(FI_LINK_UNKNOWN, len);
	CASEENUMSTRN(FI_LINK_DOWN, len);
	CASEENUMSTRN(FI_LINK_UP, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_link_attr(char *buf, size_t len,
				const struct fi_link_attr *attr)
{
	const char *prefix = TAB TAB;
	ofi_strncatf(buf, len, "%sfi_link_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%saddress: %s\n", prefix, attr->address);
	ofi_strncatf(buf, len, "%smtu: %zu\n", prefix, attr->mtu);
	ofi_strncatf(buf, len, "%sspeed: %zu\n", prefix, attr->speed);
	ofi_strncatf(buf, len, "%sstate: ", prefix);
	ofi_tostr_link_state(buf, len, attr->state);
	ofi_strncatf(buf, len, "\n%snetwork_type: %s\n", prefix,
		     attr->network_type);
}

int ofi_nic_tostr(const struct fid *fid_nic, char *buf, size_t len)
{
	const struct fid_nic *nic = (const struct fid_nic*) fid_nic;

	assert(fid_nic->fclass == FI_CLASS_NIC);
	ofi_strncatf(buf, len, "%sfid_nic:\n", TAB);

	ofi_tostr_device_attr(buf, len, nic->device_attr);
	ofi_tostr_bus_attr(buf, len, nic->bus_attr);
	ofi_tostr_link_attr(buf, len, nic->link_attr);
	return 0;
}

struct fi_ops default_nic_ops = {
	.size = sizeof(struct fi_ops),
	.close = ofi_nic_close,
	.control = ofi_nic_control,
	.tostr = ofi_nic_tostr,
};

static int ofi_dup_dev_attr(const struct fi_device_attr *attr,
			    struct fi_device_attr **dup_attr)
{
	*dup_attr = calloc(1, sizeof(**dup_attr));
	if (!*dup_attr)
		return -FI_ENOMEM;

	if (ofi_str_dup(attr->name, &(*dup_attr)->name) ||
	    ofi_str_dup(attr->device_id, &(*dup_attr)->device_id) ||
	    ofi_str_dup(attr->device_version, &(*dup_attr)->device_version) ||
	    ofi_str_dup(attr->vendor_id, &(*dup_attr)->vendor_id) ||
	    ofi_str_dup(attr->driver, &(*dup_attr)->driver) ||
	    ofi_str_dup(attr->firmware, &(*dup_attr)->firmware))
		return -FI_ENOMEM;

	return 0;
}

static int ofi_dup_bus_attr(const struct fi_bus_attr *attr,
			    struct fi_bus_attr **dup_attr)
{
	*dup_attr = calloc(1, sizeof(**dup_attr));
	if (!*dup_attr)
		return -FI_ENOMEM;

	**dup_attr = *attr;
	return 0;
}

static int ofi_dup_link_attr(const struct fi_link_attr *attr,
			     struct fi_link_attr **dup_attr)
{
	*dup_attr = calloc(1, sizeof(**dup_attr));
	if (!*dup_attr)
		return -FI_ENOMEM;

	if (ofi_str_dup(attr->address, &(*dup_attr)->address) ||
	    ofi_str_dup(attr->network_type, &(*dup_attr)->network_type))
		return -FI_ENOMEM;

	(*dup_attr)->mtu = attr->mtu;
	(*dup_attr)->speed = attr->speed;
	(*dup_attr)->state = attr->state;
	return 0;
}

struct fid_nic *ofi_nic_dup(const struct fid_nic *nic)
{
	struct fid_nic *dup_nic;
	int ret;

	dup_nic = calloc(1, sizeof(*dup_nic));
	if (!dup_nic)
		return NULL;

	if (!nic) {
		dup_nic->fid.fclass = FI_CLASS_NIC;
		dup_nic->device_attr = calloc(1, sizeof(*dup_nic->device_attr));
		dup_nic->bus_attr = calloc(1, sizeof(*dup_nic->bus_attr));
		dup_nic->link_attr = calloc(1, sizeof(*dup_nic->link_attr));

		if (!dup_nic->device_attr || !dup_nic->bus_attr ||
		    !dup_nic->link_attr)
			goto fail;

		dup_nic->fid.ops = &default_nic_ops;
		return dup_nic;
	}

	assert(nic->fid.fclass == FI_CLASS_NIC);
	dup_nic->fid = nic->fid;

	if (nic->device_attr) {
		ret = ofi_dup_dev_attr(nic->device_attr, &dup_nic->device_attr);
		if (ret)
			goto fail;
	}

	if (nic->bus_attr) {
		ret = ofi_dup_bus_attr(nic->bus_attr, &dup_nic->bus_attr);
		if (ret)
			goto fail;
	}

	if (nic->link_attr) {
		ret = ofi_dup_link_attr(nic->link_attr, &dup_nic->link_attr);
		if (ret)
			goto fail;
	}

	return dup_nic;

fail:
	ofi_nic_close(&dup_nic->fid);
	return NULL;
}
