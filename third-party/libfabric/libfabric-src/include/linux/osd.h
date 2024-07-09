/*
 * Copyright (c) 2015 Los Alamos Nat. Security, LLC. All rights reserved.
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _LINUX_OSD_H_
#define _LINUX_OSD_H_

/*#define _GNU_SOURCE*/

#include <byteswap.h>
#include <endian.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/errqueue.h>
#include <ifaddrs.h>
#include "unix/osd.h"
#include "rdma/fi_errno.h"


static inline int ofi_shm_remap(struct util_shm *shm,
				size_t newsize, void **mapped)
{
	shm->ptr = mremap(shm->ptr, shm->size, newsize, 0);
	shm->size = newsize;
	*mapped = shm->ptr;
	return shm->ptr == MAP_FAILED ? -FI_EINVAL : FI_SUCCESS;
}

ssize_t ofi_get_addr_page_size(const void *addr);
ssize_t ofi_get_hugepage_size(void);

static inline int ofi_alloc_hugepage_buf(void **memptr, size_t size)
{
	return ofi_mmap_anon_pages(memptr, size, MAP_HUGETLB);
}

static inline int ofi_hugepage_enabled(void)
{
	size_t len;
	void *buffer;
	int ret;

	len = ofi_get_hugepage_size();
	if (len <= 0)
		return 0;

	ret = ofi_alloc_hugepage_buf(&buffer, len);
	if (ret)
		return 0;

	ret = ofi_unmap_anon_pages(buffer, len);
	assert(ret == 0);

	return 1;
}

size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa);

#ifndef __NR_process_vm_readv
# define __NR_process_vm_readv 310
#endif

#ifndef __NR_process_vm_writev
# define __NR_process_vm_writev 311
#endif

static inline ssize_t ofi_process_vm_readv(pid_t pid,
			const struct iovec *local_iov,
			unsigned long liovcnt,
			const struct iovec *remote_iov,
			unsigned long riovcnt,
			unsigned long flags)
{
	return syscall(__NR_process_vm_readv, pid, local_iov, liovcnt,
		       remote_iov, riovcnt, flags);
}

static inline size_t ofi_process_vm_writev(pid_t pid,
			 const struct iovec *local_iov,
			 unsigned long liovcnt,
			 const struct iovec *remote_iov,
			 unsigned long riovcnt,
			 unsigned long flags)
{
	return syscall(__NR_process_vm_writev, pid, local_iov, liovcnt,
		       remote_iov, riovcnt, flags);
}

static inline ssize_t ofi_read_socket(SOCKET fd, void *buf, size_t count)
{
	return read(fd, buf, count);
}

static inline ssize_t ofi_write_socket(SOCKET fd, const void *buf, size_t count)
{
	return write(fd, buf, count);
}

static inline ssize_t ofi_recv_socket(SOCKET fd, void *buf, size_t count,
				      int flags)
{
	return recv(fd, buf, count, flags);
}

static inline ssize_t ofi_recvfrom_socket(SOCKET fd, void *buf, size_t count, int flags,
					  struct sockaddr *from, socklen_t *fromlen)
{
	return recvfrom(fd, buf, count, flags, from, fromlen);
}

static inline ssize_t ofi_send_socket(SOCKET fd, const void *buf, size_t count,
				      int flags)
{
	return send(fd, buf, count, flags);
}

static inline ssize_t ofi_sendto_socket(SOCKET fd, const void *buf, size_t count, int flags,
					const struct sockaddr *to, socklen_t tolen)
{
	return sendto(fd, buf, count, flags, to, tolen);
}

static inline ssize_t ofi_writev_socket(SOCKET fd, struct iovec *iov, size_t iov_cnt)
{
	return writev(fd, iov, iov_cnt);
}

static inline ssize_t ofi_readv_socket(SOCKET fd, struct iovec *iov, int iov_cnt)
{
	return readv(fd, iov, iov_cnt);
}

static inline ssize_t
ofi_sendmsg_tcp(SOCKET fd, const struct msghdr *msg, int flags)
{
	return sendmsg(fd, msg, flags);
}

static inline ssize_t
ofi_recvmsg_tcp(SOCKET fd, struct msghdr *msg, int flags)
{
	return recvmsg(fd, msg, flags);
}

static inline ssize_t
ofi_sendv_socket(SOCKET fd, const struct iovec *iov, size_t cnt, int flags)
{
       struct msghdr msg;

       msg.msg_control = NULL;
       msg.msg_controllen = 0;
       msg.msg_flags = 0;
       msg.msg_name = NULL;
       msg.msg_namelen = 0;
       msg.msg_iov = (struct iovec *) iov;
       msg.msg_iovlen = cnt;

       return ofi_sendmsg_tcp(fd, &msg, flags);
}

static inline ssize_t
ofi_recvv_socket(SOCKET fd, const struct iovec *iov, size_t cnt, int flags)
{
       struct msghdr msg;

       msg.msg_control = NULL;
       msg.msg_controllen = 0;
       msg.msg_flags = 0;
       msg.msg_name = NULL;
       msg.msg_namelen = 0;
       msg.msg_iov = (struct iovec *) iov;
       msg.msg_iovlen = cnt;

       return ofi_recvmsg_tcp(fd, &msg, flags);
}

#endif /* _LINUX_OSD_H_ */
