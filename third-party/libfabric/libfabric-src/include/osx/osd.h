/*
 * Copyright (c) 2015 Los Alamos Nat. Security, LLC. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef OSX_OSD_H
#define OSX_OSD_H

#include <sys/time.h>
#include <time.h>
#include <mach/clock.h>
#include <mach/mach.h>

#include <machine/endian.h>
#include <libkern/OSByteOrder.h>

#include <pthread.h>

#include <ifaddrs.h>

#include <limits.h>

#include "unix/osd.h"
#include "rdma/fi_errno.h"
#include "config.h"

#define pthread_yield pthread_yield_np

#define bswap_64 OSSwapInt64

#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX 255
#endif

#ifdef __cplusplus
extern "C" {
#endif

static inline int ofi_shm_remap(struct util_shm *shm, size_t newsize, void **mapped)
{
	return -1;
}

static inline ssize_t ofi_get_hugepage_size(void)
{
	return -FI_ENOSYS;
}

static inline int ofi_alloc_hugepage_buf(void **memptr, size_t size)
{
	return -FI_ENOSYS;
}

static inline int ofi_free_hugepage_buf(void *memptr, size_t size)
{
	return -FI_ENOSYS;
}

static inline size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa)
{
	return 0;
}

static inline int ofi_hugepage_enabled(void)
{
	return 0;
}

static inline ssize_t ofi_process_vm_readv(pid_t pid,
			const struct iovec *local_iov,
			unsigned long liovcnt,
			const struct iovec *remote_iov,
			unsigned long riovcnt,
			unsigned long flags)
{
	return -FI_ENOSYS;
}

static inline size_t ofi_process_vm_writev(pid_t pid,
			 const struct iovec *local_iov,
			 unsigned long liovcnt,
			 const struct iovec *remote_iov,
			 unsigned long riovcnt,
			 unsigned long flags)
{
	return -FI_ENOSYS;
}

static inline ssize_t
ofi_recv_socket(SOCKET fd, void *buf, size_t count, int flags)
{
	size_t len = count > INT_MAX ? INT_MAX : count;
	return recv(fd, buf, len, flags);
}

static inline ssize_t
ofi_send_socket(SOCKET fd, const void *buf, size_t count, int flags)
{
	size_t len = count > INT_MAX ? INT_MAX : count;
	return send(fd, buf, len, flags);
}

static inline ssize_t ofi_read_socket(SOCKET fd, void *buf, size_t count)
{
	return ofi_recv_socket(fd, buf, count, 0);
}

static inline ssize_t ofi_write_socket(SOCKET fd, const void *buf, size_t count)
{
	return ofi_send_socket(fd, buf, count, 0);
}

static inline ssize_t
ofi_recvfrom_socket(SOCKET fd, void *buf, size_t count, int flags,
		    struct sockaddr *from, socklen_t *fromlen)
{
	size_t len = count > INT_MAX ? INT_MAX : count;
	return recvfrom(fd, buf, len, flags, from, fromlen);
}

static inline ssize_t
ofi_sendto_socket(SOCKET fd, const void *buf, size_t count, int flags,
		  const struct sockaddr *to, socklen_t tolen)
{
	size_t len = count > INT_MAX ? INT_MAX : count;
	return sendto(fd, buf, len, flags, to, tolen);
}

ssize_t ofi_writev_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt);
ssize_t ofi_readv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt);
ssize_t ofi_sendmsg_tcp(SOCKET fd, const struct msghdr *msg, int flags);
ssize_t ofi_recvmsg_tcp(SOCKET fd, struct msghdr *msg, int flags);

#ifdef __cplusplus
}
#endif

#endif
