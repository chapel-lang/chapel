/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2019 Intel Inc. All rights reserved.
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 *                         All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <ofi_mr.h>
#include <unistd.h>
#include "ofi_hmem.h"

pthread_mutex_t mm_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t mm_list_rwlock = PTHREAD_RWLOCK_INITIALIZER;

static int ofi_uffd_start(struct ofi_mem_monitor *monitor);
static void ofi_uffd_stop(struct ofi_mem_monitor *monitor);

static struct ofi_uffd uffd = {
	.monitor.iface = FI_HMEM_SYSTEM,
	.monitor.init = ofi_monitor_init,
	.monitor.cleanup = ofi_monitor_cleanup,
	.monitor.start = ofi_uffd_start,
	.monitor.stop = ofi_uffd_stop,
};
struct ofi_mem_monitor *uffd_monitor = &uffd.monitor;

struct ofi_mem_monitor *default_monitor;
struct ofi_mem_monitor *default_cuda_monitor;
struct ofi_mem_monitor *default_rocr_monitor;

static size_t ofi_default_cache_size(void)
{
	long cpu_cnt;
	size_t cache_size;

	cpu_cnt = ofi_sysconf(_SC_NPROCESSORS_ONLN);
	/* disable cache on error */
	if (cpu_cnt <= 0)
		return 0;

	cache_size = ofi_get_mem_size() / (size_t) cpu_cnt / 2;
	FI_INFO(&core_prov, FI_LOG_MR,
		"default cache size=%zu\n", cache_size);
	return cache_size;
}


void ofi_monitor_init(struct ofi_mem_monitor *monitor)
{
	dlist_init(&monitor->list);
}

void ofi_monitor_cleanup(struct ofi_mem_monitor *monitor)
{
	assert(dlist_empty(&monitor->list));
}

/*
 * Initialize all available memory monitors
 */
void ofi_monitors_init(void)
{
	uffd_monitor->init(uffd_monitor);
	memhooks_monitor->init(memhooks_monitor);
	cuda_monitor->init(cuda_monitor);
	rocr_monitor->init(rocr_monitor);

#if HAVE_MEMHOOKS_MONITOR
        default_monitor = memhooks_monitor;
#elif HAVE_UFFD_MONITOR
        default_monitor = uffd_monitor;
#else
        default_monitor = NULL;
#endif

	fi_param_define(NULL, "mr_cache_max_size", FI_PARAM_SIZE_T,
			"Defines the total number of bytes for all memory"
			" regions that may be tracked by the MR cache."
			" Setting this will reduce the amount of memory"
			" not actively in use that may be registered."
			" (default: total memory / number of cpu cores / 2)");
	fi_param_define(NULL, "mr_cache_max_count", FI_PARAM_SIZE_T,
			"Defines the total number of memory regions that"
			" may be store in the cache.  Setting this will"
			" reduce the number of registered regions, regardless"
			" of their size, stored in the cache.  Setting this"
			" to zero will disable MR caching.  (default: 1024)");
	fi_param_define(NULL, "mr_cache_monitor", FI_PARAM_STRING,
			"Define a default memory registration monitor."
			" The monitor checks for virtual to physical memory"
			" address changes.  Options are: userfaultfd, memhooks"
			" and disabled.  Userfaultfd is a Linux kernel feature."
			" Memhooks operates by intercepting memory allocation"
			" and free calls.  Userfaultfd is the default if"
			" available on the system. 'disabled' option disables"
			" memory caching.");
	fi_param_define(NULL, "mr_cuda_cache_monitor_enabled", FI_PARAM_BOOL,
			"Enable or disable the CUDA cache memory monitor."
			"Monitor is enabled by default.");
	fi_param_define(NULL, "mr_rocr_cache_monitor_enabled", FI_PARAM_BOOL,
			"Enable or disable the ROCR cache memory monitor. "
			"Monitor is enabled by default.");

	fi_param_get_size_t(NULL, "mr_cache_max_size", &cache_params.max_size);
	fi_param_get_size_t(NULL, "mr_cache_max_count", &cache_params.max_cnt);
	fi_param_get_str(NULL, "mr_cache_monitor", &cache_params.monitor);
	fi_param_get_bool(NULL, "mr_cuda_cache_monitor_enabled",
			  &cache_params.cuda_monitor_enabled);
	fi_param_get_bool(NULL, "mr_rocr_cache_monitor_enabled",
			  &cache_params.rocr_monitor_enabled);

	if (!cache_params.max_size)
		cache_params.max_size = ofi_default_cache_size();

	if (cache_params.monitor != NULL) {
		if (!strcmp(cache_params.monitor, "userfaultfd")) {
#if HAVE_UFFD_MONITOR
			default_monitor = uffd_monitor;
#else
			FI_WARN(&core_prov, FI_LOG_MR, "userfaultfd monitor not available\n");
			default_monitor = NULL;
#endif
		} else if (!strcmp(cache_params.monitor, "memhooks")) {
#if HAVE_MEMHOOKS_MONITOR
			default_monitor = memhooks_monitor;
#else
			FI_WARN(&core_prov, FI_LOG_MR, "memhooks monitor not available\n");
			default_monitor = NULL;
#endif
		} else if (!strcmp(cache_params.monitor, "disabled")) {
			default_monitor = NULL;
		}
	}

	if (cache_params.cuda_monitor_enabled)
		default_cuda_monitor = cuda_monitor;
	else
		default_cuda_monitor = NULL;

	if (cache_params.rocr_monitor_enabled)
		default_rocr_monitor = rocr_monitor;
	else
		default_rocr_monitor = NULL;
}

void ofi_monitors_cleanup(void)
{
	uffd_monitor->cleanup(uffd_monitor);
	memhooks_monitor->cleanup(memhooks_monitor);
	cuda_monitor->cleanup(cuda_monitor);
	rocr_monitor->cleanup(rocr_monitor);
}

/* Monitors array must be of size OFI_HMEM_MAX. */
int ofi_monitors_add_cache(struct ofi_mem_monitor **monitors,
			   struct ofi_mr_cache *cache)
{
	int ret = 0;
	enum fi_hmem_iface iface;
	struct ofi_mem_monitor *monitor;
	unsigned int success_count = 0;

	if (!monitors) {
		for (iface = FI_HMEM_SYSTEM; iface < OFI_HMEM_MAX; iface++)
			cache->monitors[iface] = NULL;
		return -FI_ENOSYS;
	}

	/* Loops until there are no readers or writers holding the lock */
	do {
		ret = pthread_rwlock_trywrlock(&mm_list_rwlock);
		if (ret && ret != EBUSY) {
			FI_WARN(&core_prov, FI_LOG_MR,
				"add_cache cannot obtain write lock, %d\n",
				ret);
			return ret;
		}
	} while (ret);

	for (iface = FI_HMEM_SYSTEM; iface < OFI_HMEM_MAX; iface++) {
		cache->monitors[iface] = NULL;

		if (!hmem_ops[iface].initialized)
			continue;

		monitor = monitors[iface];
		if (!monitor) {
			FI_DBG(&core_prov, FI_LOG_MR,
			       "MR cache disabled for %s memory\n",
			       fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			continue;
		}

		if (dlist_empty(&monitor->list)) {
			ret = monitor->start(monitor);
			if (ret)
				goto err;
		}

		success_count++;
		cache->monitors[iface] = monitor;
		dlist_insert_tail(&cache->notify_entries[iface],
				  &monitor->list);
	}
	pthread_rwlock_unlock(&mm_list_rwlock);
	return success_count ? FI_SUCCESS : -FI_ENOSYS;

err:
	pthread_rwlock_unlock(&mm_list_rwlock);

	FI_WARN(&core_prov, FI_LOG_MR,
		"Failed to start %s memory monitor: %s\n",
		fi_tostr(&iface, FI_TYPE_HMEM_IFACE), fi_strerror(-ret));
	ofi_monitors_del_cache(cache);

	return ret;
}

void ofi_monitors_del_cache(struct ofi_mr_cache *cache)
{
	struct ofi_mem_monitor *monitor;
	enum fi_hmem_iface iface;
	int ret;

	/* Loops until there are no readers or writers holding the lock */
	do {
		ret = pthread_rwlock_trywrlock(&mm_list_rwlock);
		if (ret && ret != EBUSY) {
			FI_WARN(&core_prov, FI_LOG_MR,
				"del_cache cannot obtain write lock, %d\n",
				ret);
			return;
		}
	} while (ret);

	for (iface = 0; iface < OFI_HMEM_MAX; iface++) {
		monitor = cache->monitors[iface];
		if (!monitor)
			continue;

		dlist_remove(&cache->notify_entries[iface]);

		if (dlist_empty(&monitor->list))
			monitor->stop(monitor);

		cache->monitors[iface] = NULL;
	}

	pthread_rwlock_unlock(&mm_list_rwlock);
}

/* Must be called with locks in place like following
 *	pthread_rwlock_rdlock(&mm_list_rwlock);
 *	pthread_mutex_lock(&mm_lock);
 *	ofi_monitor_notify();
 *	pthread_mutex_unlock(&mm_lock);
 *	pthread_rwlock_unlock(&mm_list_rwlock);
 */
void ofi_monitor_notify(struct ofi_mem_monitor *monitor,
			const void *addr, size_t len)
{
	struct ofi_mr_cache *cache;

	dlist_foreach_container(&monitor->list, struct ofi_mr_cache,
				cache, notify_entries[monitor->iface]) {
		ofi_mr_cache_notify(cache, addr, len);
	}
}

/* Must be called with locks in place like following
 *	pthread_rwlock_rdlock(&mm_list_rwlock);
 *	pthread_mutex_lock(&mm_lock);
 *	ofi_monitor_flush();
 *	pthread_mutex_unlock(&mm_lock);
 *	pthread_rwlock_unlock(&mm_list_rwlock);
 */
void ofi_monitor_flush(struct ofi_mem_monitor *monitor)
{
	struct ofi_mr_cache *cache;

	dlist_foreach_container(&monitor->list, struct ofi_mr_cache,
				cache, notify_entries[monitor->iface]) {
		pthread_mutex_unlock(&mm_lock);
		ofi_mr_cache_flush(cache, false);
		pthread_mutex_lock(&mm_lock);
	}
}

int ofi_monitor_subscribe(struct ofi_mem_monitor *monitor,
			  const void *addr, size_t len,
			  union ofi_mr_hmem_info *hmem_info)
{
	int ret;

	FI_DBG(&core_prov, FI_LOG_MR,
	       "subscribing addr=%p len=%zu\n", addr, len);

	ret = monitor->subscribe(monitor, addr, len, hmem_info);
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed (ret = %d) to monitor addr=%p len=%zu\n",
			ret, addr, len);
	}
	return ret;
}

void ofi_monitor_unsubscribe(struct ofi_mem_monitor *monitor,
			     const void *addr, size_t len,
			     union ofi_mr_hmem_info *hmem_info)
{
	FI_DBG(&core_prov, FI_LOG_MR,
	       "unsubscribing addr=%p len=%zu\n", addr, len);
	monitor->unsubscribe(monitor, addr, len, hmem_info);
}

#if HAVE_UFFD_MONITOR

#include <poll.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/userfaultfd.h>

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
	struct pollfd fds;
	int ret;

	fds.fd = uffd.fd;
	fds.events = POLLIN;
	for (;;) {
		ret = poll(&fds, 1, -1);
		if (ret != 1)
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

		switch (msg.event) {
		case UFFD_EVENT_REMOVE:
			ofi_monitor_unsubscribe(&uffd.monitor,
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
				"ioctl/uffd_unreg: %s\n", strerror(errno));
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

static bool ofi_uffd_valid(struct ofi_mem_monitor *monitor, const void *addr,
			   size_t len, union ofi_mr_hmem_info *hmem_info)
{
	/* no-op */
	return true;
}

static int ofi_uffd_start(struct ofi_mem_monitor *monitor)
{
	struct uffdio_api api;
	int ret;

	uffd.monitor.subscribe = ofi_uffd_subscribe;
	uffd.monitor.unsubscribe = ofi_uffd_unsubscribe;
	uffd.monitor.valid = ofi_uffd_valid;

	if (!num_page_sizes)
		return -FI_ENODATA;

	uffd.fd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
	if (uffd.fd < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"syscall/userfaultfd %s\n", strerror(errno));
		return -errno;
	}

	api.api = UFFD_API;
	api.features = UFFD_FEATURE_EVENT_UNMAP | UFFD_FEATURE_EVENT_REMOVE |
		       UFFD_FEATURE_EVENT_REMAP;
	ret = ioctl(uffd.fd, UFFDIO_API, &api);
	if (ret < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"ioctl/uffdio: %s\n", strerror(errno));
		ret = -errno;
		goto closefd;
	}

	if (api.api != UFFD_API) {
		FI_WARN(&core_prov, FI_LOG_MR, "uffd features not supported\n");
		ret = -FI_ENOSYS;
		goto closefd;
	}

	ret = pthread_create(&uffd.thread, NULL, ofi_uffd_handler, &uffd);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"failed to create handler thread %s\n", strerror(ret));
		ret = -ret;
		goto closefd;
	}
	return 0;

closefd:
	close(uffd.fd);
	return ret;
}

static void ofi_uffd_stop(struct ofi_mem_monitor *monitor)
{
	pthread_cancel(uffd.thread);
	pthread_join(uffd.thread, NULL);
	close(uffd.fd);
}

#else /* HAVE_UFFD_MONITOR */

static int ofi_uffd_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

static void ofi_uffd_stop(struct ofi_mem_monitor *monitor)
{
}

#endif /* HAVE_UFFD_MONITOR */
