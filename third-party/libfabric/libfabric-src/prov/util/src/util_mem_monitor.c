/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
 * Copyright (c) 2017-2021 Intel Inc. All rights reserved.
 * Copyright (c) 2019-2021 Amazon.com, Inc. or its affiliates.
 *                         All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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
#include <ofi_hmem.h>
#include <ofi_enosys.h>
#include <rdma/fi_ext.h>


pthread_mutex_t mm_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mm_state_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t mm_list_rwlock = PTHREAD_RWLOCK_INITIALIZER;

static int ofi_uffd_start(struct ofi_mem_monitor *monitor);
static void ofi_uffd_stop(struct ofi_mem_monitor *monitor);

static struct ofi_uffd uffd = {
	.monitor.iface = FI_HMEM_SYSTEM,
	.monitor.init = ofi_monitor_init,
	.monitor.cleanup = ofi_monitor_cleanup,
	.monitor.start = ofi_uffd_start,
	.monitor.stop = ofi_uffd_stop,
	.monitor.name = "uffd",
};
struct ofi_mem_monitor *uffd_monitor = &uffd.monitor;

struct ofi_mem_monitor *default_monitor;
struct ofi_mem_monitor *default_cuda_monitor;
struct ofi_mem_monitor *default_rocr_monitor;
struct ofi_mem_monitor *default_ze_monitor;

struct ofi_mem_monitor **monitor_list;
size_t monitor_list_size;

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

/**
 * We needed additional locking to run the start/stop functions
 * without holding the rwlock. This allows us to have memory
 * related functions in start/stop without deadlocking. We queue
 * up a list of monitors before handling their start/stop functions
 * all within this function. Due to having to release the rwlock
 * before we enter this function, we need to further ensure thread
 * safety by adding a state system.
 *
 * The state system has 4 expected states, IDLE, STARTING, RUNNING,
 * and STOPPING.
 *
 * We expect states to move without any races from:
 * IDLE -> STARTING
 * STARTING -> RUNNING
 * RUNNING -> STOPPING
 * STOPPING -> IDLE
 *
 * In the case of races, we can also expect:
 * STOPPING -> RUNNING
 * STARTING -> RUNNING
 *
 * We only execute any behavior in this update function when the
 * state is either STARTING or STOPPING.
 *
 * Discussion on this can be found at #7003 and #7063
 *
 */
static int ofi_monitors_update(struct ofi_mem_monitor **monitors)
{
	int ret = 0;
	enum fi_hmem_iface iface;
	struct ofi_mem_monitor *monitor;

	assert(monitors);

	pthread_mutex_lock(&mm_state_lock);
	for (iface = 0; iface < OFI_HMEM_MAX; iface++) {
		monitor = monitors[iface];
		if (monitor == NULL)
			continue;

		assert(monitor->state != FI_MM_STATE_UNSPEC);
		switch (monitor->state) {
		case FI_MM_STATE_STARTING:
			FI_INFO(&core_prov, FI_LOG_MR,
				"Starting memory monitor: %s\n",
				monitor->name);
			ret = monitor->start(monitor);
			if (ret) {
				monitor->state = FI_MM_STATE_IDLE;
				FI_WARN(&core_prov, FI_LOG_MR,
					"Failed to start %s memory monitor: %s\n",
					fi_tostr(&iface, FI_TYPE_HMEM_IFACE), fi_strerror(-ret));

				goto out;
			}
			monitor->state = FI_MM_STATE_RUNNING;
			break;
		case FI_MM_STATE_STOPPING:
			FI_INFO(&core_prov, FI_LOG_MR,
				"Stopping memory monitor: %s\n",
				monitor->name);
			monitor->stop(monitor);
			monitor->state = FI_MM_STATE_IDLE;
			break;
		default:
			break;
		}
	}
out:
	pthread_mutex_unlock(&mm_state_lock);
	return ret;
}


void ofi_monitor_init(struct ofi_mem_monitor *monitor)
{
	dlist_init(&monitor->list);
	monitor->state = FI_MM_STATE_IDLE;
}

void ofi_monitor_cleanup(struct ofi_mem_monitor *monitor)
{
	assert(dlist_empty(&monitor->list));
	assert(monitor->state == FI_MM_STATE_IDLE);
}

static void initialize_monitor_list()
{
	/* Save a copy of the monitor list for cleanup time.
	 * This list can not be static because the pointer
	 * initialization is spread across mulitple modules.
	 */

	struct ofi_mem_monitor *monitors[] = {
		uffd_monitor,
		memhooks_monitor,
		cuda_monitor,
		cuda_ipc_monitor,
		rocr_monitor,
		rocr_ipc_monitor,
		xpmem_monitor,
		ze_monitor,
		import_monitor,
	};

	monitor_list_size = ARRAY_SIZE(monitors);
	monitor_list = calloc(monitor_list_size, sizeof(*monitor_list));

	for (size_t i = 0; i < monitor_list_size; i++) {
		monitor_list[i] = monitors[i];
		assert(monitor_list[i]->name);
	}
}

static void cleanup_monitor_list() {
	free(monitor_list);
	monitor_list = NULL;
	monitor_list_size = 0;
}

/*
 * Initialize all available memory monitors
 */
void ofi_monitors_init(void)
{
	pthread_mutex_init(&mm_lock, NULL);
	pthread_mutex_init(&mm_state_lock, NULL);
	pthread_rwlock_init(&mm_list_rwlock, NULL);

	initialize_monitor_list();

	for (size_t i = 0; i < monitor_list_size; i++) {
		FI_INFO(&core_prov, FI_LOG_MR,
			"Initializing memory monitor %s\n",
			monitor_list[i]->name);
		monitor_list[i]->init(monitor_list[i]);
	}

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
			"Enabled by default.");
	fi_param_define(NULL, "mr_rocr_cache_monitor_enabled", FI_PARAM_BOOL,
			"Enable or disable the ROCR cache memory monitor. "
			"Enabled by default.");
	fi_param_define(NULL, "mr_ze_cache_monitor_enabled", FI_PARAM_BOOL,
			"Enable or disable the oneAPI Level Zero cache memory "
			"monitor.  Enabled by default.");

	fi_param_get_size_t(NULL, "mr_cache_max_size", &cache_params.max_size);
	fi_param_get_size_t(NULL, "mr_cache_max_count", &cache_params.max_cnt);
	fi_param_get_str(NULL, "mr_cache_monitor", &cache_params.monitor);
	fi_param_get_bool(NULL, "mr_cuda_cache_monitor_enabled",
			  &cache_params.cuda_monitor_enabled);
	fi_param_get_bool(NULL, "mr_rocr_cache_monitor_enabled",
			  &cache_params.rocr_monitor_enabled);
	fi_param_get_bool(NULL, "mr_ze_cache_monitor_enabled",
			  &cache_params.ze_monitor_enabled);

	if (!cache_params.max_size)
		cache_params.max_size = ofi_default_cache_size();

	/*
	 * At this time, the import monitor could have set the default monitor,
	 * do not override
	 */
	if (!default_monitor) {
#if HAVE_MEMHOOKS_MONITOR
		default_monitor = memhooks_monitor;
#elif HAVE_UFFD_MONITOR
		default_monitor = uffd_monitor;
#else
		default_monitor = NULL;
#endif
	}

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

	FI_INFO(&core_prov, FI_LOG_MR,
		"Default memory monitor is: %s\n",
		(default_monitor) ? default_monitor->name : "disabled");

	if (cache_params.cuda_monitor_enabled)
		default_cuda_monitor = cuda_monitor;
	else
		default_cuda_monitor = NULL;

	if (cache_params.rocr_monitor_enabled)
		default_rocr_monitor = rocr_monitor;
	else
		default_rocr_monitor = NULL;

	if (cache_params.ze_monitor_enabled)
		default_ze_monitor = ze_monitor;
	else
		default_ze_monitor = NULL;
}

void ofi_monitors_cleanup(void)
{
	for (size_t i = 0; i < monitor_list_size; i++) {
		FI_INFO(&core_prov, FI_LOG_MR,
			"Cleaning up memory monitor %s\n",
			monitor_list[i]->name);
		monitor_list[i]->cleanup(monitor_list[i]);
	}

	cleanup_monitor_list();

	pthread_rwlock_destroy(&mm_list_rwlock);
	pthread_mutex_destroy(&mm_state_lock);
	pthread_mutex_destroy(&mm_lock);
}

/* Monitors array must be of size OFI_HMEM_MAX. */
int ofi_monitors_add_cache(struct ofi_mem_monitor **monitors,
			   struct ofi_mr_cache *cache)
{
	struct ofi_mem_monitor *start_list[OFI_HMEM_MAX];
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
		start_list[iface] = NULL;
		if (!ofi_hmem_is_initialized(iface))
			continue;

		monitor = monitors[iface];
		if (!monitor) {
			FI_DBG(&core_prov, FI_LOG_MR,
			       "MR cache disabled for %s memory\n",
			       fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
			continue;
		}

		if (dlist_empty(&monitor->list)) {
			pthread_mutex_lock(&mm_state_lock);
			start_list[iface] = monitor;
			/* See comment above ofi_monitors_update for details */
			if (monitor->state == FI_MM_STATE_IDLE)
				monitor->state = FI_MM_STATE_STARTING;
			else if (monitor->state == FI_MM_STATE_STOPPING)
				monitor->state = FI_MM_STATE_RUNNING;
			pthread_mutex_unlock(&mm_state_lock);
		}

		success_count++;
		cache->monitors[iface] = monitor;
		dlist_insert_tail(&cache->notify_entries[iface],
				  &monitor->list);
	}
	pthread_rwlock_unlock(&mm_list_rwlock);

	ret = ofi_monitors_update(start_list);
	if (ret)
		goto err;

	return success_count ? FI_SUCCESS : -FI_ENOSYS;

err:
	ofi_monitors_del_cache(cache);
	return ret;
}

void ofi_monitors_del_cache(struct ofi_mr_cache *cache)
{
	struct ofi_mem_monitor *update_list[OFI_HMEM_MAX];
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
		update_list[iface] = NULL;
		monitor = cache->monitors[iface];
		if (!monitor)
			continue;

		dlist_remove(&cache->notify_entries[iface]);

		if (dlist_empty(&monitor->list)) {
			pthread_mutex_lock(&mm_state_lock);
			/* See comment above ofi_monitors_update for details */
			if (monitor->state == FI_MM_STATE_RUNNING) {
				monitor->state = FI_MM_STATE_STOPPING;
				update_list[iface] = monitor;
			} else if (monitor->state == FI_MM_STATE_STARTING) {
				monitor->state = FI_MM_STATE_RUNNING;
				update_list[iface] = monitor;
			}
			pthread_mutex_unlock(&mm_state_lock);
		}

		cache->monitors[iface] = NULL;
	}

	pthread_rwlock_unlock(&mm_list_rwlock);


	ofi_monitors_update(update_list);
	return;
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

int ofi_monitor_start_no_op(struct ofi_mem_monitor *monitor)
{
	return FI_SUCCESS;
}

void ofi_monitor_stop_no_op(struct ofi_mem_monitor *monitor)
{
}

int ofi_monitor_subscribe_no_op(struct ofi_mem_monitor *notifier,
			 const void *addr, size_t len,
			 union ofi_mr_hmem_info *hmem_info)
{
	return FI_SUCCESS;
}

void ofi_monitor_unsubscribe_no_op(struct ofi_mem_monitor *notifier,
			    const void *addr, size_t len,
			    union ofi_mr_hmem_info *hmem_info)
{
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


static void ofi_import_monitor_init(struct ofi_mem_monitor *monitor);
static void ofi_import_monitor_cleanup(struct ofi_mem_monitor *monitor);
static int ofi_import_monitor_start(struct ofi_mem_monitor *monitor);
static void ofi_import_monitor_stop(struct ofi_mem_monitor *monitor);
static int ofi_import_monitor_subscribe(struct ofi_mem_monitor *notifier,
					const void *addr, size_t len,
					union ofi_mr_hmem_info *hmem_info);
static void ofi_import_monitor_unsubscribe(struct ofi_mem_monitor *notifier,
					   const void *addr, size_t len,
					   union ofi_mr_hmem_info *hmem_info);
static bool ofi_import_monitor_valid(struct ofi_mem_monitor *notifier,
				     const struct ofi_mr_info *info,
				     struct ofi_mr_entry *entry);

struct ofi_import_monitor {
	struct ofi_mem_monitor monitor;
	struct fid_mem_monitor *impfid;
};

static struct ofi_import_monitor impmon = {
	.monitor.iface = FI_HMEM_SYSTEM,
	.monitor.init = ofi_import_monitor_init,
	.monitor.cleanup = ofi_import_monitor_cleanup,
	.monitor.start = ofi_import_monitor_start,
	.monitor.stop = ofi_import_monitor_stop,
	.monitor.subscribe = ofi_import_monitor_subscribe,
	.monitor.unsubscribe = ofi_import_monitor_unsubscribe,
	.monitor.valid = ofi_import_monitor_valid,
	.monitor.name = "import",
};

struct ofi_mem_monitor *import_monitor = &impmon.monitor;

static void ofi_import_monitor_init(struct ofi_mem_monitor *monitor)
{
	ofi_monitor_init(monitor);
}

static void ofi_import_monitor_cleanup(struct ofi_mem_monitor *monitor)
{
	assert(!impmon.impfid);
	ofi_monitor_cleanup(monitor);
}

static int ofi_import_monitor_start(struct ofi_mem_monitor *monitor)
{
	if (!impmon.impfid)
		return -FI_ENOSYS;

	return impmon.impfid->export_ops->start(impmon.impfid);
}

static void ofi_import_monitor_stop(struct ofi_mem_monitor *monitor)
{
	assert(impmon.impfid);
	impmon.impfid->export_ops->stop(impmon.impfid);
}

static int ofi_import_monitor_subscribe(struct ofi_mem_monitor *notifier,
					const void *addr, size_t len,
					union ofi_mr_hmem_info *hmem_info)
{
	assert(impmon.impfid);
	return impmon.impfid->export_ops->subscribe(impmon.impfid, addr, len);
}

static void ofi_import_monitor_unsubscribe(struct ofi_mem_monitor *notifier,
					   const void *addr, size_t len,
					   union ofi_mr_hmem_info *hmem_info)
{
	assert(impmon.impfid);
	impmon.impfid->export_ops->unsubscribe(impmon.impfid, addr, len);
}

static bool ofi_import_monitor_valid(struct ofi_mem_monitor *notifier,
				     const struct ofi_mr_info *info,
				     struct ofi_mr_entry *entry)
{
	assert(impmon.impfid);
	return impmon.impfid->export_ops->valid(impmon.impfid,
						entry->info.iov.iov_base,
						entry->info.iov.iov_len);
}

static void ofi_import_monitor_notify(struct fid_mem_monitor *monitor,
				      const void *addr, size_t len)
{
	assert(monitor->fid.context == &impmon);
	pthread_rwlock_rdlock(&mm_list_rwlock);
	pthread_mutex_lock(&mm_lock);
	ofi_monitor_notify(&impmon.monitor, addr, len);
	pthread_mutex_unlock(&mm_lock);
	pthread_rwlock_unlock(&mm_list_rwlock);
}

static int ofi_close_import(struct fid *fid)
{
	pthread_mutex_lock(&mm_state_lock);
	impmon.monitor.state = FI_MM_STATE_IDLE;
	pthread_mutex_unlock(&mm_state_lock);
	impmon.impfid = NULL;
	return 0;
}

static struct fi_ops_mem_notify import_ops = {
	.size = sizeof(struct fi_ops_mem_notify),
	.notify = ofi_import_monitor_notify,
};

static struct fi_ops impfid_ops = {
	.size = sizeof(struct fi_ops),
	.close = ofi_close_import,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
	.tostr = fi_no_tostr,
	.ops_set = fi_no_ops_set,
};

int ofi_monitor_import(struct fid *fid)
{
	struct fid_mem_monitor *impfid;

	if (fid->fclass != FI_CLASS_MEM_MONITOR)
		return -FI_ENOSYS;

	if (impmon.impfid) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"imported monitor already exists\n");
		return -FI_EBUSY;
	}

	if (default_monitor && !dlist_empty(&default_monitor->list)) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"cannot replace active monitor\n");
		return -FI_EBUSY;
	}

	impfid = container_of(fid, struct fid_mem_monitor, fid);
	if (impfid->export_ops->size < sizeof(struct fi_ops_mem_monitor))
		return -FI_EINVAL;

	impmon.impfid = impfid;
	impfid->fid.context = &impmon;
	impfid->fid.ops = &impfid_ops;
	impfid->import_ops = &import_ops;

	FI_INFO(&core_prov, FI_LOG_MR,
		"setting imported memory monitor as default\n");
	default_monitor = &impmon.monitor;
	return 0;
}
