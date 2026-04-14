/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2021 Intel Inc. All rights reserved.
 * Copyright (c) 2019-2021 Amazon.com, Inc. or its affiliates.
 *                         All rights reserved.
 * (C) Copyright 2020,2024 Hewlett Packard Enterprise Development LP
 * Copyright (C) 2024 Cornelis Networks. All rights reserved.
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

#include <unistd.h>

#include <ofi_mr.h>
#include <ofi_mem.h>

#ifndef UFFD_USER_MODE_ONLY
#define UFFD_USER_MODE_ONLY 0
#endif

static int ofi_uffd_start(struct ofi_mem_monitor *monitor);
static void ofi_uffd_stop(struct ofi_mem_monitor *monitor);
static int ofi_uffd_subscribe(struct ofi_mem_monitor *monitor,
			      const void *addr, size_t len,
			      union ofi_mr_hmem_info *hmem_info);
static bool ofi_uffd_valid(struct ofi_mem_monitor *monitor,
			   const struct ofi_mr_info *info,
			   struct ofi_mr_entry *entry);

static struct ofi_uffd uffd = {
	.monitor.iface = FI_HMEM_SYSTEM,
	.monitor.init = ofi_monitor_init,
	.monitor.cleanup = ofi_monitor_cleanup,
	.monitor.start = ofi_uffd_start,
	.monitor.stop = ofi_uffd_stop,
	.monitor.subscribe = ofi_uffd_subscribe,

	/* Since UFFD may have many MR cache entries for the same VA range and
	 * ofi_monitor_unsubscribe() is called for every MR cache entry being
	 * freed, UFFD unsubscribe needs to be a noop. Else, MR cache entries
	 * may no longer be monitored.
	 */
	.monitor.unsubscribe = ofi_monitor_unsubscribe_no_op,
	.monitor.valid = ofi_uffd_valid,
	.monitor.name = "uffd",
	.fd = -1,
	.exit_pipe = { -1, -1 },
};
struct ofi_mem_monitor *uffd_monitor = &uffd.monitor;

#if HAVE_UFFD_MONITOR

#include <poll.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/userfaultfd.h>

static void ofi_uffd_pagefault_handler(struct uffd_msg *msg);
static void ofi_uffd_unsubscribe(struct ofi_mem_monitor *monitor,
				 const void *addr, size_t len,
				 union ofi_mr_hmem_info *hmem_info);

/* The userfault fd monitor requires for events that could
 * trigger it to be handled outside of the monitor functions
 * itself. When a fault occurs on a monitored region, the
 * faulting thread is put to sleep until the event is read
 * via the userfault file descriptor. If this fault occurs
 * within the userfault handling thread, no threads will
 * read this event and our threads cannot progress, resulting
 * in a hang.
 */
static void *ofi_uffd_handler(void *arg)
{
	struct uffd_msg msg;
	struct pollfd fds[2];
	int ret;

	fds[0].fd     = uffd.fd;
	fds[0].events = POLLIN;
	fds[1].fd     = uffd.exit_pipe[0];
	fds[1].events = POLLIN;

	for (;;) {
		ret = poll(fds, 2, -1);
		if (ret < 0 || fds[1].revents)
			break;

		pthread_rwlock_rdlock(&mm_list_rwlock);
		pthread_mutex_lock(&mm_lock);
		ret = read(uffd.fd, &msg, sizeof(msg));
		if (ret != sizeof(msg)) {
			pthread_mutex_unlock(&mm_lock);
			pthread_rwlock_unlock(&mm_list_rwlock);
			if (errno != EAGAIN)
				break;
			continue;
		}

		FI_DBG(&core_prov, FI_LOG_MR, "Received UFFD event %d\n", msg.event);

		switch (msg.event) {
		case UFFD_EVENT_REMOVE:
			ofi_uffd_unsubscribe(&uffd.monitor,
				(void *) (uintptr_t) msg.arg.remove.start,
				(size_t) (msg.arg.remove.end -
					  msg.arg.remove.start), NULL);
			/* fall through */
		case UFFD_EVENT_UNMAP:
			ofi_monitor_notify(&uffd.monitor,
				(void *) (uintptr_t) msg.arg.remove.start,
				(size_t) (msg.arg.remove.end -
					  msg.arg.remove.start));
			break;
		case UFFD_EVENT_REMAP:
			ofi_monitor_notify(&uffd.monitor,
				(void *) (uintptr_t) msg.arg.remap.from,
				(size_t) msg.arg.remap.len);
			break;
		case UFFD_EVENT_PAGEFAULT:
			ofi_uffd_pagefault_handler(&msg);
			break;
		default:
			FI_WARN(&core_prov, FI_LOG_MR,
				"Unhandled uffd event %d\n", msg.event);
			break;
		}
		pthread_mutex_unlock(&mm_lock);
		pthread_rwlock_unlock(&mm_list_rwlock);
	}
	return NULL;
}

static void ofi_uffd_pagefault_handler(struct uffd_msg *msg)
{
	struct uffdio_zeropage zp;
	int i;
	int ret;
	void * const address = (void *) (uintptr_t) msg->arg.pagefault.address;
	uint64_t const flags = (uint64_t) msg->arg.pagefault.flags;
#if HAVE_UFFD_THREAD_ID
	uint32_t const ptid = (uint32_t) msg->arg.pagefault.feat.ptid;
#endif
	/* ofi_uffd_register sets the mode to
	 * UFFDIO_REGISTER_MODE_MISSING.  As a result, we can
	 * get read, write or write-protect notifications via
	 * UFFD_EVENT_PAGEFAULT.  The only ones we can sensibly
	 * handle are writes to non-backed pages.
	 * (Read and write-protect notifications are likely
	 * application bugs.)
	 */

	if (flags != UFFD_PAGEFAULT_FLAG_WRITE) {
#if HAVE_UFFD_THREAD_ID
		FI_WARN(&core_prov, FI_LOG_MR,
			"UFFD pagefault with unrecognized flags: %lu, address %p, thread %u\n",
			flags, address, ptid);
#else
		FI_WARN(&core_prov, FI_LOG_MR,
			"UFFD pagefault with unrecognized flags: %lu, address %p\n",
			flags, address);
#endif
		/* The faulting thread is halted at this point. In
		 * theory we could wake it up with UFFDIO_WAKE. In
		 * practice that requires the address range of the
		 * fault, information we don't have from the
		 * pagefault event.
		 */

		return;
	}

	/* The event tells us the address of the fault
	 * (which can be anywhere on the page). It does not
	 * tell us the size of the page so we have to guess
	 * from the list of known page_sizes.
	 *
	 * We employ the standard resolution: install a zeroed page.
	 */

	for (i = 0; i < num_page_sizes; ) {
		/* setup a zeropage reqest for this pagesize */
		zp.range.start = (uint64_t) (uintptr_t)
			ofi_get_page_start(address, page_sizes[i]);
		zp.range.len = (uint64_t) page_sizes[i];
		zp.mode = 0;
		zp.zeropage = 0;

		ret = ioctl(uffd.fd, UFFDIO_ZEROPAGE, &zp);

		if (ret == 0)		/* success */
			return;

		/* Note: the documentation (man ioctl_userfaultfd) says
		 * that the ioctl() returns -1 on error and errno is set
		 * to indicate the error. It also says that the zeropage
		 * member of struct uffdio_zeropage is set to the negated
		 * error.  The unit tests for uffd say
		 *    real retval in uffdio_zeropage.zeropage
		 * so that's what we use here.
		 */

		if (zp.zeropage == -EAGAIN)
			/* This is a tough case.  If the memory map is
			 * changing, the kernel returns EAGAIN before
			 * installing the zeroed page.  So the page
			 * fault has not been rectified.  If we don't try
			 * again, the application will crash.  If we add
			 * a maximum retry count we could still end up
			 * with an unresolved page fault.
			 *
			 * It's likely a kernel bug or (something else
			 * bad like OOM) if it returns EAGAIN forever.
			 * So we retry until we get something besides
			 * EAGAIN.
			 */
			continue;	/* retry this page size */

		i++;			/* try next page size */

		if (zp.zeropage == -EINVAL)     /* wrong page size */
			continue;

		/* If we get here we failed to install the zeroed
		 * page for this page size and it wasn't a size error.
		 * We could either stop trying or go on to the
		 * next pagesize.  We choose to print a message and try
		 * another page size.
		 */

		FI_DBG(&core_prov, FI_LOG_MR,
			"Unable to install zeroed page of size %zu to handle page fault."
			"  address = %p zeropage = %lld errno = %d\n",
			page_sizes[i], address, zp.zeropage, errno);
	}

	FI_WARN(&core_prov, FI_LOG_MR,
		"Unable to handle event UFFD_EVENT_PAGEFAULT for address %p.\n",
		address);
}

static int ofi_uffd_register(const void *addr, size_t len, size_t page_size)
{
	struct uffdio_register reg;
	int ret;

	reg.range.start = (uint64_t) (uintptr_t)
			  ofi_get_page_start(addr, page_size);
	reg.range.len = ofi_get_page_bytes(addr, len, page_size);
	reg.mode = UFFDIO_REGISTER_MODE_MISSING;
	ret = ioctl(uffd.fd, UFFDIO_REGISTER, &reg);
	if (ret < 0) {
		if (errno != EINVAL) {
			FI_WARN(&core_prov, FI_LOG_MR,
				"ioctl/uffd_reg: %s\n", strerror(errno));
		}
		return -errno;
	}
	return 0;
}

static int ofi_uffd_subscribe(struct ofi_mem_monitor *monitor,
			      const void *addr, size_t len,
			      union ofi_mr_hmem_info *hmem_info)
{
	int i;

	assert(monitor == &uffd.monitor);
	for (i = 0; i < num_page_sizes; i++) {
		if (!ofi_uffd_register(addr, len, page_sizes[i]))
			return 0;
	}
	return -FI_EFAULT;
}

static int ofi_uffd_unregister(const void *addr, size_t len, size_t page_size)
{
	struct uffdio_range range;
	int ret;

	range.start = (uint64_t) (uintptr_t)
		      ofi_get_page_start(addr, page_size);
	range.len = ofi_get_page_bytes(addr, len, page_size);
	ret = ioctl(uffd.fd, UFFDIO_UNREGISTER, &range);
	if (ret < 0) {
		if (errno != EINVAL) {
			FI_WARN(&core_prov, FI_LOG_MR,
				"ioctl/uffd_unreg: %s\n", strerror(errno));
		}
		return -errno;
	}
	return 0;
}

/* May be called from mr cache notifier callback */
static void ofi_uffd_unsubscribe(struct ofi_mem_monitor *monitor,
				 const void *addr, size_t len,
				 union ofi_mr_hmem_info *hmem_info)
{
	int i;

	assert(monitor == &uffd.monitor);
	for (i = 0; i < num_page_sizes; i++) {
		if (!ofi_uffd_unregister(addr, len, page_sizes[i]))
			break;
	}
}

static bool ofi_uffd_valid(struct ofi_mem_monitor *monitor,
			   const struct ofi_mr_info *info,
			   struct ofi_mr_entry *entry)
{
	/* no-op */
	return true;
}

static void ofi_uffd_close_fd(struct ofi_uffd *monitor)
{
	close(monitor->fd);
	monitor->fd = -1;
}

static void ofi_uffd_close_pipe(struct ofi_uffd *monitor)
{
	close(monitor->exit_pipe[0]);
	close(monitor->exit_pipe[1]);
	monitor->exit_pipe[0] = -1;
	monitor->exit_pipe[1] = -1;
}

static int ofi_uffd_start(struct ofi_mem_monitor *monitor)
{
	struct uffdio_api api;
	int ret;

	if (uffd.fd >= 0)
		return 0;

	if (!num_page_sizes)
		return -FI_ENODATA;

	ret = pipe(uffd.exit_pipe);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"uffd/pipe: %s\n", strerror(errno));
		return -errno;
	}

	uffd.fd = syscall(__NR_userfaultfd,
			  O_CLOEXEC | O_NONBLOCK | UFFD_USER_MODE_ONLY);
	if (uffd.fd < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"syscall/userfaultfd %s\n", strerror(errno));
		ret = -errno;
		goto close_pipe;
	}

	api.api = UFFD_API;
	api.features = UFFD_FEATURE_EVENT_UNMAP | UFFD_FEATURE_EVENT_REMOVE |
		       UFFD_FEATURE_EVENT_REMAP;
	ret = ioctl(uffd.fd, UFFDIO_API, &api);
	if (ret < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"ioctl/uffdio: %s\n", strerror(errno));
		ret = -errno;
		goto close_fd;
	}

	if (api.api != UFFD_API) {
		FI_WARN(&core_prov, FI_LOG_MR, "uffd features not supported\n");
		ret = -FI_ENOSYS;
		goto close_fd;
	}

	ret = pthread_create(&uffd.thread, NULL, ofi_uffd_handler, &uffd);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"failed to create handler thread %s\n", strerror(ret));
		ret = -ret;
		goto close_fd;
	}

	FI_INFO(&core_prov, FI_LOG_MR,
		"Memory monitor uffd started.\n");

	return 0;

close_fd:

	ofi_uffd_close_fd(&uffd);

close_pipe:

	ofi_uffd_close_pipe(&uffd);

	FI_WARN(&core_prov, FI_LOG_MR,
		"Memory monitor uffd failed to start: %s.\n",
		strerror(-ret));

	return ret;
}

static void ofi_uffd_stop(struct ofi_mem_monitor *monitor)
{
	ssize_t   num_written;

	if (uffd.fd < 0)
		return;

	/* tell the thread to exit with the exit_pipe */

	num_written = write(uffd.exit_pipe[1], "X", 1);
	if (num_written != 1) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"uffd/close: unable to write to exit pipe: %s",
			strerror(errno));
	}

	pthread_join(uffd.thread, NULL);

	ofi_uffd_close_fd(&uffd);
	ofi_uffd_close_pipe(&uffd);

	FI_INFO(&core_prov, FI_LOG_MR,
		"Memory monitor uffd stopped.\n");
}

#else /* HAVE_UFFD_MONITOR */

static int ofi_uffd_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

static void ofi_uffd_stop(struct ofi_mem_monitor *monitor)
{
}

static int ofi_uffd_subscribe(struct ofi_mem_monitor *monitor,
			      const void *addr, size_t len,
			      union ofi_mr_hmem_info *hmem_info)
{
	return -FI_ENOSYS;
}

static bool ofi_uffd_valid(struct ofi_mem_monitor *monitor,
			   const struct ofi_mr_info *info,
			   struct ofi_mr_entry *entry)
{
	return false;
}

#endif /* HAVE_UFFD_MONITOR */
