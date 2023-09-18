/*
 * Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006-2017 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2013-2018 Intel Corp., Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos Nat. Security, LLC. All rights reserved.
 * Copyright (c) 2020 Amazon.com, Inc. or its affiliates.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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
#include <netinet/ip.h>
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
#include <ofi_lock.h>
#include <ofi_osd.h>
#include <ofi_iov.h>
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

size_t ofi_universe_size = 1024;
int ofi_av_remove_cleanup;


int ofi_genlock_init(struct ofi_genlock *lock,
		     enum ofi_lock_type lock_type)
{
	int ret;

	lock->lock_type = lock_type;
	switch (lock->lock_type) {
	case OFI_LOCK_SPINLOCK:
		ret = ofi_spin_init(&lock->base.spinlock);
		lock->lock = (ofi_genlock_lockop_t) ofi_spin_lock_op;
		lock->unlock = (ofi_genlock_lockop_t) ofi_spin_unlock_op;
		lock->held = (ofi_genlock_lockheld_t) ofi_spin_held_op;
		break;
	case OFI_LOCK_MUTEX:
		ret = ofi_mutex_init(&lock->base.mutex);
		lock->lock = (ofi_genlock_lockop_t) ofi_mutex_lock_op;
		lock->unlock = (ofi_genlock_lockop_t) ofi_mutex_unlock_op;
		lock->held = (ofi_genlock_lockheld_t) ofi_mutex_held_op;
		break;
	case OFI_LOCK_NOOP:
		/* Use mutex for debug no-op support */
		ret = ofi_mutex_init(&lock->base.mutex);
		lock->lock = (ofi_genlock_lockop_t) ofi_mutex_lock_noop;
		lock->unlock = (ofi_genlock_lockop_t) ofi_mutex_unlock_noop;
		lock->held = (ofi_genlock_lockheld_t) ofi_mutex_held_op;
		break;
	case OFI_LOCK_NONE:
		ret = 0;
		lock->base.nolock = NULL;
		lock->lock = (ofi_genlock_lockop_t) ofi_nolock_lock_op;
		lock->unlock = (ofi_genlock_lockop_t) ofi_nolock_unlock_op;
		lock->held = (ofi_genlock_lockheld_t) ofi_nolock_held_op;
		break;
	default:
		ret = -FI_EINVAL;
		break;
	};

	return ret;
}

void ofi_genlock_destroy(struct ofi_genlock *lock)
{
	switch (lock->lock_type) {
	case OFI_LOCK_SPINLOCK:
		ofi_spin_destroy(&lock->base.spinlock);
		break;
	case OFI_LOCK_MUTEX:
	case OFI_LOCK_NOOP:
		ofi_mutex_destroy(&lock->base.mutex);
		break;
	case OFI_LOCK_NONE:
		break;
	default:
		assert(0);
		break;
	};
}


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

bool ofi_send_allowed(uint64_t caps)
{
	if ((caps & FI_MSG) || (caps & FI_TAGGED)) {
		if (caps & FI_SEND)
			return true;
		if (caps & FI_RECV)
			return false;
		return true;
	}

	return false;
}

bool ofi_recv_allowed(uint64_t caps)
{
	if ((caps & FI_MSG) || (caps & FI_TAGGED)) {
		if (caps & FI_RECV)
			return true;
		if (caps & FI_SEND)
			return false;
		return true;
	}

	return false;
}

bool ofi_rma_initiate_allowed(uint64_t caps)
{
	if ((caps & FI_RMA) || (caps & FI_ATOMICS)) {
		if ((caps & FI_WRITE) || (caps & FI_READ))
			return true;
		if ((caps & FI_REMOTE_WRITE) || (caps & FI_REMOTE_READ))
			return false;
		return true;
	}

	return false;
}

bool ofi_rma_target_allowed(uint64_t caps)
{
	if ((caps & FI_RMA) || (caps & FI_ATOMICS)) {
		if ((caps & FI_REMOTE_WRITE) || (caps & FI_REMOTE_READ))
			return true;
		if ((caps & FI_WRITE) || (caps & FI_READ))
			return false;
		return true;
	}

	return false;
}

bool ofi_needs_tx(uint64_t caps)
{
	return ofi_send_allowed(caps) || ofi_rma_initiate_allowed(caps);
}

bool ofi_needs_rx(uint64_t caps)
{
	return ofi_recv_allowed(caps);
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

uint32_t ofi_generate_seed(void)
{
	/* Time returns long; keep the lower and most significant 32 bits */
	uint32_t rand_seed;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	rand_seed = ((getpid() & 0xffffffff) << 16);

	/* Mix the PID into the upper bits */
	rand_seed |= (uint32_t) tv.tv_usec;

	return rand_seed;
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
	const struct ofi_sockaddr_ib *sib;
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

		size = snprintf(buf, *len, "fi_sockaddr_in://%s:%" PRIu16,
				str, ntohs(sin->sin_port));
		break;
	case FI_SOCKADDR_IN6:
sa_sin6:
		sin6 = addr;
		if (!inet_ntop(sin6->sin6_family, &sin6->sin6_addr, str,
			       sizeof(str)))
			return NULL;

		size = snprintf(buf, *len, "fi_sockaddr_in6://[%s]:%" PRIu16,
				str, ntohs(sin6->sin6_port));
		break;
	case FI_ADDR_EFA:
		memset(str, 0, sizeof(str));
		if (!inet_ntop(AF_INET6, addr, str, INET6_ADDRSTRLEN))
			return NULL;
		size = snprintf(buf, *len, "fi_addr_efa://[%s]:%" PRIu16 ":%" PRIu32,
				str, *((uint16_t *)addr + 8), *((uint32_t *)addr + 5));
		break;
	case FI_SOCKADDR_IB:
		sib = addr;
		memset(str, 0, sizeof(str));
		if (!inet_ntop(AF_INET6, sib->sib_addr, str, INET6_ADDRSTRLEN))
			return NULL;

		size = snprintf(buf, *len, "fi_sockaddr_ib://[%s]" /* GID */
			     ":0x%" PRIx16 /* P_Key */
			     ":0x%" PRIx16 /* port space */
			     ":0x%" PRIx8 /* Scope ID */,
			     str, /* GID */
			     ntohs(sib->sib_pkey), /* P_Key */
			     (uint16_t)(ntohll(sib->sib_sid) >> 16) & 0xfff, /* port space */
				 (uint8_t)ntohll(sib->sib_scope_id) & 0xff);
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
	case FI_ADDR_PSMX3:
		size =
		    snprintf(buf, *len, "fi_addr_psmx3://%" PRIx64 ":%" PRIx64 ":%" PRIx64 ":%" PRIx64,
			     *(uint64_t *)addr, *((uint64_t *)addr + 1),
			     *((uint64_t *)addr + 2), *((uint64_t *)addr + 3));
		break;
	case FI_ADDR_GNI:
		size = snprintf(buf, *len, "fi_addr_gni://%" PRIx64,
				*(uint64_t *)addr);
		break;
	case FI_ADDR_BGQ:
		size = snprintf(buf, *len, "fi_addr_bgq://%p", addr);
		break;
	case FI_ADDR_OPX:
		size = snprintf(buf, *len, "fi_addr_opx://%016lx", *(uint64_t *)addr);
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
	case FI_ADDR_CXI:
		size = snprintf(buf, *len, "fi_addr_cxi://0x%08" PRIx32,
				*(uint32_t *)addr);
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

uint32_t ofi_addr_format(const char *str)
{
	char fmt[17];
	int ret;

	memset(fmt, 0, sizeof(fmt));
	ret = sscanf(str, "%16[^:]://", fmt);
	if (ret != 1)
		return FI_FORMAT_UNSPEC;

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
	else if (!strcasecmp(fmt, "fi_addr_psmx3"))
		return FI_ADDR_PSMX3;
	else if (!strcasecmp(fmt, "fi_addr_gni"))
		return FI_ADDR_GNI;
	else if (!strcasecmp(fmt, "fi_addr_bgq"))
		return FI_ADDR_BGQ;
	else if (!strcasecmp(fmt, "fi_addr_opx"))
		return FI_ADDR_OPX;
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

static int ofi_str_to_psmx3(const char *str, void **addr, size_t *len)
{
	int ret;

	*len = 4 * sizeof(uint64_t);
	*addr = calloc(1, *len);
	if (!(*addr))
		return -FI_ENOMEM;

	ret = sscanf(str, "%*[^:]://%" SCNx64 ":%" SCNx64 ":%" SCNx64 ":%" SCNx64,
		     (uint64_t *) *addr, (uint64_t *) *addr + 1,
		     (uint64_t *) *addr + 2, (uint64_t *) *addr + 3);
	if (ret == 4)
		return 0;

	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_opx(const char *str, void **addr, size_t *len)
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

static int ofi_str_to_sib(const char *str, void **addr, size_t *len)
{
	int ret;
	char *tok, *endptr, *saveptr;
	struct ofi_sockaddr_ib *sib;
	uint16_t pkey;
	uint16_t ps;
	uint64_t scope_id;
	uint16_t port;
	char gid[64 + 1];
	char extra_str[64 + 1];

	memset(gid, 0, sizeof(gid));

	ret = sscanf(str, "%*[^:]://[%64[^]]]" /* GID */
		     ":%64s", /* P_Key : port_space : Scope ID : port */
		     gid, extra_str);
	if (ret != 2) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid GID in address: %s\n", str);
		return -FI_EINVAL;
	}

	tok = strtok_r(extra_str, ":", &saveptr);
	if (!tok) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid pkey in address: %s\n", str);
		return -FI_EINVAL;
	}

	pkey = (uint16_t) strtol(tok, &endptr, 0);
	if (*endptr) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid pkey in address: %s\n", str);
		return -FI_EINVAL;
	}

	tok = strtok_r(NULL, ":", &saveptr);
	if (!tok) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid port space in address: %s\n", str);
		return -FI_EINVAL;
	}

	ps = (uint16_t) strtol(tok, &endptr, 0);
	if (*endptr) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid port space in address: %s\n", str);
		return -FI_EINVAL;
	}

	tok = strtok_r(NULL, ":", &saveptr);
	if (!tok) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid scope id in address: %s\n", str);
		return -FI_EINVAL;
	}

	scope_id = strtol(tok, &endptr, 0);
	if (*endptr) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Invalid scope id in address: %s\n", str);
		return -FI_EINVAL;
	}

	/* Port is optional */
	tok = strtok_r(NULL, ":", &saveptr);
	if (tok)
		port = (uint16_t) strtol(tok, &endptr, 0);
	else
		port = 0;

	*len = sizeof(struct ofi_sockaddr_ib);
	*addr = calloc(1, *len);
	if (!*addr)
		return -FI_ENOMEM;

	sib = (struct ofi_sockaddr_ib *)(*addr);

	if (inet_pton(AF_INET6, gid, sib->sib_addr) > 0) {
		sib->sib_family = AF_IB;
		sib->sib_pkey = htons(pkey);
		if (ps && port) {
			sib->sib_sid = htonll(((uint64_t) ps << 16) + port);
			sib->sib_sid_mask = htonll(OFI_IB_IP_PS_MASK |
			                           OFI_IB_IP_PORT_MASK);
		}
		sib->sib_scope_id = htonll(scope_id);
		return FI_SUCCESS;
	}

	free(*addr);
	return -FI_EINVAL;
}

static int ofi_str_to_efa(const char *str, void **addr, size_t *len)
{
	char gid[INET6_ADDRSTRLEN + 1];
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
	ret = sscanf(str, "%*[^:]://[%46[^]]]:%" SCNu16 ":%" SCNu32, gid, qpn, qkey);
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
	char ip[65];
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
	char ip[65];
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
	case FI_ADDR_PSMX3:
		return ofi_str_to_psmx3(str, addr, len);
	case FI_ADDR_OPX:
		return ofi_str_to_opx(str, addr, len);
	case FI_ADDR_IB_UD:
		return ofi_str_to_ib_ud(str, addr, len);
	case FI_ADDR_EFA:
		return ofi_str_to_efa(str, addr, len);
	case FI_SOCKADDR_IB:
		return ofi_str_to_sib(str, addr, len);
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
		return (ofi_sin_is_any_addr(addr) &&
			ofi_sin_port(addr));
	case AF_INET6:
		return (ofi_sin6_is_any_addr(addr) &&
			ofi_sin6_port(addr));
	default:
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Unknown address format\n");
		return 0;
	}
}

bool ofi_is_wildcard_listen_addr(const char *node, const char *service,
				 uint64_t flags, const struct fi_info *hints)
{
	struct addrinfo *res = NULL;
	int ret;

	if (hints && hints->addr_format != FI_FORMAT_UNSPEC &&
	    hints->addr_format != FI_SOCKADDR &&
	    hints->addr_format != FI_SOCKADDR_IN &&
	    hints->addr_format != FI_SOCKADDR_IN6)
		return false;

	/* else it's okay to call getaddrinfo, proceed with processing */

	if (node) {
		if (!(flags & FI_SOURCE))
			return false;
		ret = getaddrinfo(node, service, NULL, &res);
		if (ret) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"getaddrinfo failed!\n");
			return false;
		}
		if (ofi_is_any_addr_port(res->ai_addr)) {
			freeaddrinfo(res);
			goto out;
		}
		freeaddrinfo(res);
		return false;
	}

	if (hints) {
		if (hints->dest_addr)
			return false;

		if (!hints->src_addr)
			goto out;

		return ofi_is_any_addr_port(hints->src_addr);
	}
out:
	return ((flags & FI_SOURCE) && service);
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
		if (addr) {
			addr_format = ofi_translate_addr_format(ofi_sa_family(addr));
			fi_log(prov, level, subsys, func, line, "%s: %s\n", log_str,
			       ofi_straddr(buf, &len, addr_format, addr));
		} else {
			fi_log(prov, level, subsys, func, line, "%s: (null)\n", log_str);
		}
	}
}

ssize_t ofi_discard_socket(SOCKET sock, size_t len)
{
	char buf;
	ssize_t ret = 0;

	for (; len && !ret; len--)
		ret = ofi_recvall_socket(sock, &buf, 1);
	return ret;
}

size_t ofi_byteq_readv(struct ofi_byteq *byteq, struct iovec *iov,
		       size_t cnt, size_t offset)
{
	size_t avail, len;

	if (cnt == 1 && !offset)
		return ofi_byteq_read(byteq, iov[0].iov_base, iov[0].iov_len);

	avail = ofi_byteq_readable(byteq);
	if (!avail)
		return 0;

	len = ofi_copy_iov_buf(iov, cnt, offset, &byteq->data[byteq->head],
			       avail, OFI_COPY_BUF_TO_IOV);
	if (len < avail) {
		byteq->head += (unsigned) len;
	} else {
		byteq->head = 0;
		byteq->tail = 0;
	}
	return len;
}

void ofi_byteq_writev(struct ofi_byteq *byteq, const struct iovec *iov,
		      size_t cnt)
{
	size_t i;

	assert(ofi_total_iov_len(iov, cnt) <= ofi_byteq_writeable(byteq));

	if (cnt == 1) {
		ofi_byteq_write(byteq, iov[0].iov_base, iov[0].iov_len);
		return;
	}

	for (i = 0; i < cnt; i++) {
		memcpy(&byteq->data[byteq->tail], iov[i].iov_base,
		       iov[i].iov_len);
		byteq->tail += (unsigned) iov[i].iov_len;
	}
}


ssize_t ofi_bsock_flush(struct ofi_bsock *bsock)
{
	size_t avail;
	ssize_t ret;
	int err;

	if (!ofi_bsock_tosend(bsock))
		return 0;

	avail = ofi_byteq_readable(&bsock->sq);
	assert(avail);
	ret = bsock->sockapi->send(bsock->sockapi, bsock->sock,
				   &bsock->sq.data[bsock->sq.head],
				   avail, MSG_NOSIGNAL, bsock);
	if (ret < 0) {
		if (ret == -OFI_EINPROGRESS_URING)
		    return ret;

		err = ofi_sockerr();
		if (err == EPIPE)
			return -FI_ENOTCONN;
		if (err == EWOULDBLOCK)
			return -FI_EAGAIN;
		return -err;
	} else {
		ofi_byteq_consume(&bsock->sq, (size_t) ret);
	}

	return ofi_bsock_tosend(bsock) ? -FI_EAGAIN : 0;
}

ssize_t ofi_bsock_flush_sync(struct ofi_bsock *bsock)
{
	size_t avail;
	ssize_t ret;
	int err;

	if (!ofi_bsock_tosend(bsock))
		return 0;

	avail = ofi_byteq_readable(&bsock->sq);
	assert(avail);
	ret = ofi_send_socket(bsock->sock, &bsock->sq.data[bsock->sq.head],
			      avail, MSG_NOSIGNAL);
	if (ret < 0) {
		err = ofi_sockerr();
		if (err == EPIPE)
			return -FI_ENOTCONN;
		if (err == EWOULDBLOCK)
			return -FI_EAGAIN;
		return -err;
	} else {
		ofi_byteq_consume(&bsock->sq, (size_t) ret);
	}

	return ofi_bsock_tosend(bsock) ? -FI_EAGAIN : 0;
}

ssize_t ofi_bsock_send(struct ofi_bsock *bsock, const void *buf, size_t *len)
{
	size_t avail;
	ssize_t ret;

	avail = ofi_bsock_tosend(bsock);
	if (avail) {
		if (*len < ofi_byteq_writeable(&bsock->sq)) {
			ofi_byteq_write(&bsock->sq, buf, *len);
			ret = ofi_bsock_flush(bsock);
			return !ret || ret == -FI_EAGAIN ? *len : ret;
		}

		ret = ofi_bsock_flush(bsock);
		if (ret)
			return ret;
	}

	assert(!ofi_bsock_tosend(bsock));
	if (*len > bsock->zerocopy_size) {
		ret = bsock->sockapi->send(bsock->sockapi, bsock->sock, buf, *len,
					   MSG_NOSIGNAL | OFI_ZEROCOPY, bsock);
		if (ret >= 0) {
			bsock->async_index++;
			*len = ret;
			return -OFI_EINPROGRESS_ASYNC;
		}
	} else {
		ret = bsock->sockapi->send(bsock->sockapi, bsock->sock, buf, *len,
					   MSG_NOSIGNAL, bsock);
	}
	if (ret < 0) {
		if (ret == -OFI_EINPROGRESS_URING)
			return ret;

		if (OFI_SOCK_TRY_SND_RCV_AGAIN(ofi_sockerr()) &&
		    *len < ofi_byteq_writeable(&bsock->sq)) {
			ofi_byteq_write(&bsock->sq, buf, *len);
			return *len;
		}
		return ofi_sockerr() == EPIPE ? -FI_ENOTCONN : -ofi_sockerr();
	}
	*len = ret;
	return ret;
}

ssize_t ofi_bsock_sendv(struct ofi_bsock *bsock, const struct iovec *iov,
			size_t cnt, size_t *len)
{
	size_t avail;
	ssize_t ret;

	if (cnt == 1) {
		*len = iov[0].iov_len;
		return ofi_bsock_send(bsock, iov[0].iov_base, len);
	}

	*len = ofi_total_iov_len(iov, cnt);
	avail = ofi_bsock_tosend(bsock);
	if (avail) {
		if (*len < ofi_byteq_writeable(&bsock->sq)) {
			ofi_byteq_writev(&bsock->sq, iov, cnt);
			ret = ofi_bsock_flush(bsock);
			return !ret || ret == -FI_EAGAIN ? *len : ret;
		}

		ret = ofi_bsock_flush(bsock);
		if (ret)
			return ret;
	}

	assert(!ofi_bsock_tosend(bsock));

	if (*len > bsock->zerocopy_size) {
		ret = bsock->sockapi->sendv(bsock->sockapi, bsock->sock, iov, cnt,
					    MSG_NOSIGNAL | OFI_ZEROCOPY, bsock);
		if (ret >= 0) {
			bsock->async_index++;
			*len = ret;
			return -OFI_EINPROGRESS_ASYNC;
		}
	} else {
		ret = bsock->sockapi->sendv(bsock->sockapi, bsock->sock, iov, cnt,
					    MSG_NOSIGNAL, bsock);
	}
	if (ret < 0) {
		if (ret == -OFI_EINPROGRESS_URING)
			return ret;

		if (OFI_SOCK_TRY_SND_RCV_AGAIN(ofi_sockerr()) &&
		    *len < ofi_byteq_writeable(&bsock->sq)) {
			ofi_byteq_writev(&bsock->sq, iov, cnt);
			return *len;
		}
		return ofi_sockerr() == EPIPE ? -FI_ENOTCONN : -ofi_sockerr();
	}
	*len = ret;
	return ret;
}

ssize_t ofi_bsock_recv(struct ofi_bsock *bsock, void *buf, size_t len)
{
	size_t bytes, avail;
	ssize_t ret;

	bytes = ofi_byteq_read(&bsock->rq, buf, len);
	if (bytes) {
		if (bytes == len)
			return len;

		buf = (char *) buf + bytes;
		len -= bytes;
	}

	assert(!ofi_bsock_readable(bsock));
	if (len < (bsock->rq.size >> 1)) {
		avail = ofi_byteq_writeable(&bsock->rq);
		assert(avail);
		ret = bsock->sockapi->recv(bsock->sockapi, bsock->sock,
					   &bsock->rq.data[bsock->rq.tail],
					   avail, MSG_NOSIGNAL, bsock);
		if (ret <= 0)
			goto out;

		ofi_byteq_add(&bsock->rq, (size_t) ret);
		assert(ofi_bsock_readable(bsock));
		bytes += ofi_byteq_read(&bsock->rq, buf, len);
		return bytes;
	}

	ret = bsock->sockapi->recv(bsock->sockapi, bsock->sock, buf, len,
				   MSG_NOSIGNAL, bsock);
	if (ret > 0)
		return bytes + ret;

out:
	assert(ret != -OFI_EINPROGRESS_URING);
	if (bytes)
		return bytes;
	return ret ? -ofi_sockerr(): -FI_ENOTCONN;
}

ssize_t ofi_bsock_recvv(struct ofi_bsock *bsock, struct iovec *iov, size_t cnt)
{
	size_t len, bytes, avail;
	ssize_t ret;

	if (cnt == 1)
		return ofi_bsock_recv(bsock, iov[0].iov_base, iov[0].iov_len);

	len = ofi_total_iov_len(iov, cnt);
	if (ofi_byteq_readable(&bsock->rq)) {
		bytes = ofi_byteq_readv(&bsock->rq, iov, cnt, 0);
		if (bytes == len)
			return len;

		len -= bytes;
	} else {
		bytes = 0;
	}

	assert(!ofi_bsock_readable(bsock));
	if (len < (bsock->rq.size >> 1)) {
		avail = ofi_byteq_writeable(&bsock->rq);
		assert(avail);
		ret = bsock->sockapi->recv(bsock->sockapi, bsock->sock,
					   &bsock->rq.data[bsock->rq.tail],
					   avail, MSG_NOSIGNAL, bsock);
		if (ret <= 0)
			goto out;

		ofi_byteq_add(&bsock->rq, (size_t) ret);
		assert(ofi_bsock_readable(bsock));
		bytes += ofi_byteq_readv(&bsock->rq, iov, cnt, bytes);
		return bytes;
	}

	/* It's too difficult to adjust the iov without copying it, so return
	 * what data we have.  The caller will consume the iov and retry.
	 */
	if (bytes)
		return bytes;

	ret = bsock->sockapi->recvv(bsock->sockapi, bsock->sock, iov, cnt,
				    MSG_NOSIGNAL, bsock);
	if (ret > 0)
		return ret;
out:
	assert(ret != -OFI_EINPROGRESS_URING);
	if (bytes)
		return bytes;
	return ret ? -ofi_sockerr(): -FI_ENOTCONN;
}

#ifdef MSG_ZEROCOPY
uint32_t ofi_bsock_async_done(const struct fi_provider *prov,
			      struct ofi_bsock *bsock)
{
	struct msghdr msg = {};
	struct sock_extended_err *serr;
	struct cmsghdr *cmsg;
	/* x2 is arbitrary but avoids truncation */
	uint8_t ctrl[CMSG_SPACE(sizeof(*serr) * 2)];
	int ret;

	msg.msg_control = &ctrl;
	msg.msg_controllen = sizeof(ctrl);
	ret = recvmsg(bsock->sock, &msg, MSG_ERRQUEUE);
	if (ret < 0) {
		FI_WARN(prov, FI_LOG_EP_DATA,
			"Error reading MSG_ERRQUEUE (%s)\n", strerror(errno));
		goto disable;
	}

	assert(!(msg.msg_flags & MSG_CTRUNC));
	cmsg = CMSG_FIRSTHDR(&msg);
	if ((cmsg->cmsg_level != SOL_IP && cmsg->cmsg_type != IP_RECVERR) &&
	    (cmsg->cmsg_level != SOL_IPV6 && cmsg->cmsg_type != IPV6_RECVERR)) {
		FI_WARN(prov, FI_LOG_EP_DATA,
			"Unexpected cmsg level (!IP) or type (!RECVERR)\n");
		goto disable;
	}

	serr = (void *) CMSG_DATA(cmsg);
	if ((serr->ee_origin != SO_EE_ORIGIN_ZEROCOPY) || serr->ee_errno) {
		FI_WARN(prov, FI_LOG_EP_DATA,
			"Unexpected sock err origin or errno\n");
		goto disable;
	}

	bsock->done_index = serr->ee_data;
	if (serr->ee_code & SO_EE_CODE_ZEROCOPY_COPIED) {
		FI_WARN(prov, FI_LOG_EP_DATA,
			"Zerocopy data was copied\n");
disable:
		if (bsock->zerocopy_size != SIZE_MAX) {
			FI_WARN(prov, FI_LOG_EP_DATA, "disabling zerocopy\n");
			bsock->zerocopy_size = SIZE_MAX;
		}
	}
	return bsock->done_index;
}
#else
uint32_t ofi_bsock_async_done(const struct fi_provider *prov,
			      struct ofi_bsock *bsock)
{
	return 0;
}
#endif


/* Poll file descriptor set abstraction - emulates epoll over poll */
int ofi_pollfds_grow(struct ofi_pollfds *pfds, int max_size)
{
	struct pollfd *fds;
	struct ofi_pollfds_ctx *ctx;
	size_t size;

	assert(ofi_genlock_held(&pfds->lock));
	if (max_size < pfds->size)
		return FI_SUCCESS;

	size = max_size + 1;
	if (size < pfds->size + 64)
		size = pfds->size + 64;

	fds = calloc(size, sizeof(*pfds->fds) + sizeof(*pfds->ctx));
	if (!fds)
		return -FI_ENOMEM;

	ctx = (struct ofi_pollfds_ctx *) (fds + size);
	if (pfds->size) {
		memcpy(fds, pfds->fds, pfds->size * sizeof(*pfds->fds));
		memcpy(ctx, pfds->ctx, pfds->size * sizeof(*pfds->ctx));
		free(pfds->fds);
	}

	while (pfds->size < size) {
		ctx[pfds->size].index = -1;
		fds[pfds->size++].fd = INVALID_SOCKET;
	}

	pfds->fds = fds;
	pfds->ctx = ctx;
	return FI_SUCCESS;
}

static int ofi_pollfds_match_fd(struct slist_entry *entry, const void *arg)
{
	struct ofi_pollfds_work_item *item;
	int fd = (int) (uintptr_t) arg;

	item = container_of(entry, struct ofi_pollfds_work_item, entry);
	return item->fd == fd;
}

static struct ofi_pollfds_work_item *
ofi_pollfds_find_item(struct ofi_pollfds *pfds, int fd)
{
	struct slist_entry *entry;

	assert(ofi_genlock_held(&pfds->lock));
	entry = slist_find_first_match(&pfds->work_item_list,
				       ofi_pollfds_match_fd,
				       (void *) (uintptr_t) fd);
	if (!entry)
		return NULL;
	return container_of(entry, struct ofi_pollfds_work_item, entry);
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
	item->op = op;
	ofi_genlock_lock(&pfds->lock);
	slist_insert_tail(&item->entry, &pfds->work_item_list);
	fd_signal_set(&pfds->signal);
	ofi_genlock_unlock(&pfds->lock);
	return 0;
}

int ofi_pollfds_add_ctl(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context)
{
	return ofi_pollfds_ctl(pfds, POLLFDS_CTL_ADD, fd, events, context);
}

/* We're not changing the fds, just fields.  This is always 'racy' if
 * the app modifies the events being monitored for an fd while another
 * thread waits on the fds.  The other thread can always return before
 * the modifications have been made.  The caller must be prepared to
 * handle this, same as if epoll were used directly.
 *
 * Updating the events is a common case, so handle this immediately
 * without the overhead of queuing a work item.
 */
int ofi_pollfds_mod(struct ofi_pollfds *pfds, int fd, uint32_t events,
		    void *context)
{
	struct ofi_pollfds_ctx *ctx;
	struct ofi_pollfds_work_item *item;

	ofi_genlock_lock(&pfds->lock);
	ctx = ofi_pollfds_get_ctx(pfds, fd);
	if (ctx) {
		pfds->fds[ctx->index].events = (short) events;
		ctx->context = context;
		goto signal;
	}

	/* fd may be queued for insertion */
	item = ofi_pollfds_find_item(pfds, fd);
	if (item) {
		item->events = events;
		item->context = context;
	}

signal:
	fd_signal_set(&pfds->signal);
	ofi_genlock_unlock(&pfds->lock);
	return 0;
}

int ofi_pollfds_del_ctl(struct ofi_pollfds *pfds, int fd)
{
	return ofi_pollfds_ctl(pfds, POLLFDS_CTL_DEL, fd, 0, NULL);
}

static int ofi_pollfds_do_del(struct ofi_pollfds *pfds, int fd)
{
	struct ofi_pollfds_ctx *ctx, *swap_ctx;
	struct pollfd *swap_pfd;

	assert(ofi_genlock_held(&pfds->lock));
	ctx = ofi_pollfds_get_ctx(pfds, fd);
	if (!ctx)
		return 0;

	if (ctx->index < pfds->nfds - 1) {
		swap_pfd = &pfds->fds[pfds->nfds - 1];
		swap_ctx = ofi_pollfds_get_ctx(pfds, swap_pfd->fd);
		assert(swap_ctx);
		swap_ctx->index = ctx->index;
		pfds->fds[swap_ctx->index] = *swap_pfd;

		swap_pfd->fd = INVALID_SOCKET;
		swap_pfd->events = 0;
		swap_pfd->revents = 0;
	}
	pfds->nfds--;
	ctx->index = -1;
	return 0;
}

static int
ofi_pollfds_sync_del(struct ofi_pollfds *pfds, int fd)
{
	int ret;

	ofi_genlock_lock(&pfds->lock);
	ret = ofi_pollfds_do_del(pfds, fd);
	ofi_genlock_unlock(&pfds->lock);
	return ret;
}

static int
ofi_pollfds_do_add(struct ofi_pollfds *pfds, int fd, uint32_t events,
		   void *context)
{
	struct ofi_pollfds_ctx *ctx;

	assert(ofi_genlock_held(&pfds->lock));
	ctx = ofi_pollfds_get_ctx(pfds, fd);
	if (!ctx) {
		ctx = ofi_pollfds_alloc_ctx(pfds, fd);
		if (!ctx) {
			assert(0);
			return - FI_ENOMEM;
		}
	}

	ctx->context = context;
	pfds->fds[ctx->index].fd = fd;
	pfds->fds[ctx->index].events = (short) events;
	pfds->fds[ctx->index].revents = 0;
	return 0;
}

static int
ofi_pollfds_sync_add(struct ofi_pollfds *pfds, int fd, uint32_t events,
		     void *context)
{
	int ret;

	ofi_genlock_lock(&pfds->lock);
	ret = ofi_pollfds_do_add(pfds, fd, events, context);
	ofi_genlock_unlock(&pfds->lock);
	return ret;
}

static void ofi_pollfds_process_work(struct ofi_pollfds *pfds)
{
	struct slist_entry *entry;
	struct ofi_pollfds_work_item *item;

	assert(ofi_genlock_held(&pfds->lock));
	while (!slist_empty(&pfds->work_item_list)) {
		entry = slist_remove_head(&pfds->work_item_list);
		item = container_of(entry, struct ofi_pollfds_work_item, entry);

		switch (item->op) {
		case POLLFDS_CTL_ADD:
			ofi_pollfds_do_add(pfds, item->fd, item->events,
					   item->context);
			break;
		case POLLFDS_CTL_DEL:
			ofi_pollfds_do_del(pfds, item->fd);
			break;
		default:
			assert(0);
			break;
		}
		free(item);
	}
}

int ofi_pollfds_wait(struct ofi_pollfds *pfds,
		     struct ofi_epollfds_event *events,
		     int maxevents, int timeout)
{
	struct ofi_pollfds_ctx *ctx;
	uint64_t endtime;
	int cnt, i, skip, ret = 0;

	ofi_genlock_lock(&pfds->lock);
	if (!slist_empty(&pfds->work_item_list))
		ofi_pollfds_process_work(pfds);

	skip = (timeout == 0);
	endtime = ofi_timeout_time(timeout);
	do {
		ofi_genlock_unlock(&pfds->lock);
		cnt = poll(pfds->fds + skip, pfds->nfds - skip, timeout);
		if (cnt == SOCKET_ERROR)
			return -ofi_sockerr();
		else if (cnt == 0)
			return 0;

		ofi_genlock_lock(&pfds->lock);
		if (!skip && pfds->fds[0].revents) {
			assert(cnt > 0);
			fd_signal_reset(&pfds->signal);
			cnt--;
		}

		if (!slist_empty(&pfds->work_item_list))
			ofi_pollfds_process_work(pfds);

		/* Index 0 is the internal signaling fd, skip it */
		cnt = MIN(maxevents, cnt);
		for (i = 1; cnt && i < pfds->nfds; i++) {
			if (pfds->fds[i].revents) {
				ctx = ofi_pollfds_get_ctx(pfds, pfds->fds[i].fd);
				if (ctx) {
					events[ret].events = pfds->fds[i].revents;
					events[ret++].data.ptr = ctx->context;
				}
				cnt--;
			}
		}
	} while (!ret && !ofi_adjust_timeout(endtime, &timeout));

	ofi_genlock_unlock(&pfds->lock);
	return ret;
}

void ofi_pollfds_close(struct ofi_pollfds *pfds)
{
	struct ofi_pollfds_work_item *item;
	struct slist_entry *entry;

	if (!pfds)
		return;

	while (!slist_empty(&pfds->work_item_list)) {
		entry = slist_remove_head(&pfds->work_item_list);
		item = container_of(entry,
					struct ofi_pollfds_work_item,
					entry);
		free(item);
	}
	ofi_genlock_destroy(&pfds->lock);
	fd_signal_free(&pfds->signal);
	free(pfds->fds);
	free(pfds);
}

int ofi_pollfds_create_(struct ofi_pollfds **pfds, enum ofi_lock_type lock_type)
{
	int ret;

	*pfds = calloc(1, sizeof(struct ofi_pollfds));
	if (!*pfds)
		return -FI_ENOMEM;

	ret = ofi_genlock_init(&(*pfds)->lock, lock_type);
	if (ret)
		goto err0;

	ofi_genlock_lock(&(*pfds)->lock);
	ret = ofi_pollfds_grow(*pfds, 63);
	ofi_genlock_unlock(&(*pfds)->lock);
	if (ret)
		goto err1;

	ret = fd_signal_init(&(*pfds)->signal);
	if (ret)
		goto err2;

	(*pfds)->fds[0].fd = (*pfds)->signal.fd[FI_READ_FD];
	(*pfds)->fds[0].events = POLLIN;
	(*pfds)->nfds++;
	slist_init(&(*pfds)->work_item_list);
	if (lock_type == OFI_LOCK_NONE || lock_type == OFI_LOCK_NOOP) {
		(*pfds)->add = ofi_pollfds_sync_add;
		(*pfds)->del = ofi_pollfds_sync_del;
	} else {
		(*pfds)->add = ofi_pollfds_add_ctl;
		(*pfds)->del = ofi_pollfds_del_ctl;
	}
	return FI_SUCCESS;
err2:
	free((*pfds)->fds);
err1:
	ofi_genlock_destroy(&(*pfds)->lock);
err0:
	free(*pfds);
	return ret;
}

int ofi_pollfds_create(struct ofi_pollfds **pfds)
{
	return ofi_pollfds_create_(pfds, OFI_LOCK_MUTEX);
}


/* Dynamic poll */
static int
ofi_dynpoll_add_epoll(struct ofi_dynpoll *dynpoll, int fd,
		      uint32_t events, void *context)
{
	assert(dynpoll->type == OFI_DYNPOLL_EPOLL);
	return ofi_epoll_add(dynpoll->ep, fd, events, context);
}

static int
ofi_dynpoll_mod_epoll(struct ofi_dynpoll *dynpoll, int fd,
		      uint32_t events, void *context)
{
	assert(dynpoll->type == OFI_DYNPOLL_EPOLL);
	return ofi_epoll_mod(dynpoll->ep, fd, events, context);
}

static int ofi_dynpoll_del_epoll(struct ofi_dynpoll *dynpoll, int fd)
{
	assert(dynpoll->type == OFI_DYNPOLL_EPOLL);
	return ofi_epoll_del(dynpoll->ep, fd);
}

static int
ofi_dynpoll_wait_epoll(struct ofi_dynpoll *dynpoll,
		       struct ofi_epollfds_event *events,
		       int maxevents, int timeout)
{
	assert(dynpoll->type == OFI_DYNPOLL_EPOLL);
	return ofi_epoll_wait(dynpoll->ep, events, maxevents, timeout);
}

static int
ofi_dynpoll_get_fd_epoll(struct ofi_dynpoll *dynpoll)
{
	assert(dynpoll->type == OFI_DYNPOLL_EPOLL);
	return dynpoll->ep;
}

static int
ofi_dynpoll_add_poll(struct ofi_dynpoll *dynpoll, int fd,
		     uint32_t events, void *context)
{
	assert(dynpoll->type == OFI_DYNPOLL_POLL);
	return ofi_pollfds_add(dynpoll->pfds, fd, events, context);
}

static int
ofi_dynpoll_mod_poll(struct ofi_dynpoll *dynpoll, int fd,
		     uint32_t events, void *context)
{
	assert(dynpoll->type == OFI_DYNPOLL_POLL);
	return ofi_pollfds_mod(dynpoll->pfds, fd, events, context);
}

static int ofi_dynpoll_del_poll(struct ofi_dynpoll *dynpoll, int fd)
{
	assert(dynpoll->type == OFI_DYNPOLL_POLL);
	return ofi_pollfds_del(dynpoll->pfds, fd);
}

static int
ofi_dynpoll_wait_poll(struct ofi_dynpoll *dynpoll,
		      struct ofi_epollfds_event *events,
		      int maxevents, int timeout)
{
	assert(dynpoll->type == OFI_DYNPOLL_POLL);
	return ofi_pollfds_wait(dynpoll->pfds, events, maxevents, timeout);
}

static int
ofi_dynpoll_get_fd_poll(struct ofi_dynpoll *dynpoll)
{
	assert(dynpoll->type == OFI_DYNPOLL_POLL);
	return INVALID_SOCKET;  /* Unsupported */
}

void ofi_dynpoll_close(struct ofi_dynpoll *dynpoll)
{
	switch (dynpoll->type) {
	case OFI_DYNPOLL_EPOLL:
		ofi_epoll_close(dynpoll->ep);
		break;
	case OFI_DYNPOLL_POLL:
		ofi_pollfds_close(dynpoll->pfds);
		break;
	default:
		assert(0);
		break;
	}
}

int ofi_dynpoll_create(struct ofi_dynpoll *dynpoll, enum ofi_dynpoll_type type,
		       enum ofi_lock_type lock_type)
{
	int ret;

	dynpoll->type = type;
	switch (type) {
	case OFI_DYNPOLL_EPOLL:
		ret = ofi_epoll_create(&dynpoll->ep);
		dynpoll->add = ofi_dynpoll_add_epoll;
		dynpoll->mod = ofi_dynpoll_mod_epoll;
		dynpoll->del = ofi_dynpoll_del_epoll;
		dynpoll->wait = ofi_dynpoll_wait_epoll;
		dynpoll->get_fd = ofi_dynpoll_get_fd_epoll;
		break;
	case OFI_DYNPOLL_POLL:
		ret = ofi_pollfds_create_(&dynpoll->pfds, lock_type);
		dynpoll->add = ofi_dynpoll_add_poll;
		dynpoll->mod = ofi_dynpoll_mod_poll;
		dynpoll->del = ofi_dynpoll_del_poll;
		dynpoll->wait = ofi_dynpoll_wait_poll;
		dynpoll->get_fd = ofi_dynpoll_get_fd_poll;
		break;
	default:
		assert(0);
		ret = -FI_EINVAL;
		break;
	}

	return ret;
};

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

	addr_entry = calloc(1, sizeof(*addr_entry));
	if (!addr_entry)
		return;

	addr_entry->comm_caps = FI_LOCAL_COMM;
	addr_entry->ipaddr.sin.sin_family = AF_INET;
	addr_entry->ipaddr.sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	ofi_straddr_log(prov, FI_LOG_INFO, FI_LOG_CORE,
			"available addr: ", &addr_entry->ipaddr);

	strncpy(addr_entry->ipstr, "127.0.0.1", sizeof(addr_entry->ipstr));
	strncpy(addr_entry->net_name, "127.0.0.1/32", sizeof(addr_entry->net_name));
	strncpy(addr_entry->ifa_name, "lo", sizeof(addr_entry->ifa_name));
	slist_insert_tail(&addr_entry->entry, addr_list);

	addr_entry = calloc(1, sizeof(*addr_entry));
	if (!addr_entry)
		return;

	addr_entry->comm_caps = FI_LOCAL_COMM;
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

/* Sort based on:
 * 1. link speed, 2. SA family, 3. address
 */
static int
ofi_compare_addr_entry(struct slist_entry *cur, const void *insert)
{
	const struct ofi_addr_list_entry *cur_addr, *insert_addr;

	cur_addr = container_of(cur, struct ofi_addr_list_entry, entry);
	insert_addr = container_of((const struct slist_entry *) insert,
			     struct ofi_addr_list_entry, entry);

	if (insert_addr->speed > cur_addr->speed)
		return 1;
	if (insert_addr->speed < cur_addr->speed)
		return 0;

	if (insert_addr->ipaddr.sa.sa_family < cur_addr->ipaddr.sa.sa_family)
		return 1;
	if (insert_addr->ipaddr.sa.sa_family > cur_addr->ipaddr.sa.sa_family)
		return 0;

	return ofi_addr_cmp(&core_prov, &insert_addr->ipaddr.sa,
			    &cur_addr->ipaddr.sa);
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
			if (!strncmp(iface, ifa->ifa_name, strlen(iface) + 1))
				break;
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
			!(ifa->ifa_flags & IFF_RUNNING) ||
			(ifa->ifa_flags & IFF_LOOPBACK) ||
			((ifa->ifa_addr->sa_family != AF_INET) &&
			(ifa->ifa_addr->sa_family != AF_INET6)))
			continue;
		if (iface && strncmp(iface, ifa->ifa_name, strlen(iface) + 1)) {
			FI_DBG(prov, FI_LOG_CORE,
				"Skip (%s) interface\n", ifa->ifa_name);
			continue;
		}

		addr_entry = calloc(1, sizeof(*addr_entry));
		if (!addr_entry)
			continue;

		addr_entry->comm_caps = FI_LOCAL_COMM | FI_REMOTE_COMM;
		memcpy(&addr_entry->ipaddr, ifa->ifa_addr,
			ofi_sizeofaddr(ifa->ifa_addr));
		strncpy(addr_entry->ifa_name, ifa->ifa_name,
			sizeof(addr_entry->ifa_name) - 1);
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

		slist_insert_before_first_match(addr_list, ofi_compare_addr_entry,
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

			addr_entry->comm_caps = FI_LOCAL_COMM | FI_REMOTE_COMM;
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
	ofi_strncatf(buf, len, "%sbus_type: ", prefix);
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
	ofi_strncatf(buf, len, "%snic:\n", TAB);

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

/*
 * Calculate bits per second based on verbs port active_speed and active_width.
 */
size_t ofi_vrb_speed(uint8_t speed, uint8_t width)
{
	const size_t gbit_2_bit_coef = 1000 * 1000 * 1000;
	size_t width_val, speed_val;

	switch (speed) {
	case 1:
		speed_val = (size_t) (2.5 * (float) gbit_2_bit_coef);
		break;
	case 2:
		speed_val = 5 * gbit_2_bit_coef;
		break;
	case 4:
	case 8:
		speed_val = 8 * gbit_2_bit_coef;
		break;
	case 16:
		speed_val = 14 * gbit_2_bit_coef;
		break;
	case 32:
		speed_val = 25 * gbit_2_bit_coef;
		break;
	default:
		speed_val = 0;
		break;
	}

	switch (width) {
	case 1:
		width_val = 1;
		break;
	case 2:
		width_val = 4;
		break;
	case 4:
		width_val = 8;
		break;
	case 8:
		width_val = 12;
		break;
	default:
		width_val = 0;
		break;
	}

	return width_val * speed_val;
}

/* log_prefix is used by fi_log and by prov/util */
const char *log_prefix = "";
