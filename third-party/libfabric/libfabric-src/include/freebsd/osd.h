/*
 * Copyright (c) 2016 Intel Corp, Inc. All rights reserved.
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

#ifndef _FREEBSD_OSD_H_
#define _FREEBSD_OSD_H_

#include <sys/endian.h>
#include <pthread_np.h>

#include <ifaddrs.h>

#include "unix/osd.h"
#include "rdma/fi_errno.h"

#define bswap_64 bswap64

#define ENODATA ENOMSG
#define HOST_NAME_MAX  128
#define SOL_TCP IPPROTO_TCP

typedef cpuset_t cpu_set_t;

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

#endif /* _FREEBSD_OSD_H_ */


