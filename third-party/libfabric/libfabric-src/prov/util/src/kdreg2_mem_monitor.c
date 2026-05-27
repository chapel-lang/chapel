/*
 * (C) Copyright 2022-2023 Hewlett Packard Enterprise Development LP
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

#include "ofi_mr.h"

#if HAVE_KDREG2_MONITOR

#include "ofi_hmem.h"

#define EVICTOR_THREAD_ATTR NULL
#define INFINITE_TIMEOUT -1

static int kdreg2_monitor_subscribe(struct ofi_mem_monitor *monitor,
				    const void *addr, size_t len,
				    union ofi_mr_hmem_info *hmem_info)
{
	struct ofi_kdreg2 *kdreg2 =
		container_of(monitor, struct ofi_kdreg2, monitor);
	uint64_t cookie = ofi_atomic_inc64(&kdreg2->next_cookie);
	struct kdreg2_ioctl_monitor  ioctl_monitor = {
		.addr = addr,
		.length = len,
		.cookie = (kdreg2_cookie_t) cookie,
	};
	int ret;

	ret = ioctl(kdreg2->fd, KDREG2_IOCTL_MONITOR, &ioctl_monitor);
	if (ret)
		return ret;

	hmem_info->kdreg2.cookie            = ioctl_monitor.cookie;
	hmem_info->kdreg2.monitoring_params = ioctl_monitor.monitoring_params;

	return 0;
}

static void kdreg2_monitor_unsubscribe(struct ofi_mem_monitor *monitor,
				       const void *addr, size_t len,
				       union ofi_mr_hmem_info *hmem_info)
{
	struct ofi_kdreg2 *kdreg2 =
		container_of(monitor, struct ofi_kdreg2, monitor);
	struct kdreg2_ioctl_unmonitor  ioctl_unmonitor = {
		.cookie            = hmem_info->kdreg2.cookie,
		.monitoring_params = hmem_info->kdreg2.monitoring_params,
	};

	ioctl(kdreg2->fd, KDREG2_IOCTL_UNMONITOR, &ioctl_unmonitor);
}

static bool kdreg2_monitor_valid(struct ofi_mem_monitor *monitor,
				 const struct ofi_mr_info *info,
				 struct ofi_mr_entry *entry)
{
	struct ofi_kdreg2 *kdreg2 =
		container_of(monitor, struct ofi_kdreg2, monitor);
	struct kdreg2_monitoring_params *params =
		&entry->hmem_info.kdreg2.monitoring_params;

	return !kdreg2_mapping_changed(kdreg2->status_data, params);
}

static int kdreg2_read_evictions(struct ofi_kdreg2 *kdreg2)
{
	struct kdreg2_event event;
	ssize_t bytes;
	int err;

	while (kdreg2_read_counter(&kdreg2->status_data->pending_events) > 0) {

		/* The read should return a multiple of sizeof(event) or
		 * an error.  There should be no partial reads.
		 */

		bytes = read(kdreg2->fd, &event, sizeof(event));
		if (bytes < 0) {
			err = errno;

			/* EINTR means we caught a signal. */
			if (err == EINTR)
				continue;

			/* Nothing left */
			if ((err == EAGAIN) ||
			    (err == EWOULDBLOCK))
				return 0;

			/* All other errors */
			return err;
		}

		switch (event.type) {
		case KDREG2_EVENT_MAPPING_CHANGE:

			pthread_rwlock_rdlock(&mm_list_rwlock);
			pthread_mutex_lock(&mm_lock);

			ofi_monitor_notify(&kdreg2->monitor,
					   event.u.mapping_change.addr,
					   event.u.mapping_change.len);

			pthread_mutex_unlock(&mm_lock);
			pthread_rwlock_unlock(&mm_list_rwlock);

			break;

		default:

			return -ENOMSG;
		}
	}

	return 0;
}

static void kdreg2_close_pipe(struct ofi_kdreg2 *kdreg2)
{
	close(kdreg2->exit_pipe[0]);
	close(kdreg2->exit_pipe[1]);
	kdreg2->exit_pipe[0] = -1;
	kdreg2->exit_pipe[1] = -1;
}

static void kdreg2_close_fd(struct ofi_kdreg2 *kdreg2)
{
	close(kdreg2->fd);
	kdreg2->fd = -1;
	kdreg2->status_data = NULL;
}

static void *kdreg2_evictor(void *arg)
{
	struct ofi_kdreg2 *kdreg2 = (struct ofi_kdreg2 *) arg;
	int ret;
	struct pollfd pollfd[2] = {
		{
			.fd = kdreg2->fd,
			.events = POLLIN,
		},
		{	.fd = kdreg2->exit_pipe[0],
			.events = POLLIN,
		},
	};
	int n;

	while (1) {

		/* wait until there are events to read */
		n = poll(pollfd, 2, INFINITE_TIMEOUT);
		if (n == 0)           /* timeout(?) */
			continue;

		if (n < 0) {
			switch (errno) {
			case EINTR:   /* interrupted */
				continue;
			default:
				ret = -errno;
				goto error_ret;
			}
		}

		/* look for exit message on second fd */
		if (pollfd[1].revents) {
			ret = 0;
			goto error_ret;
		}

		ret = kdreg2_read_evictions(kdreg2);
		if (ret)
			goto error_ret;
	}

error_ret:

	return (void *) (intptr_t) ret;
}


static int kdreg2_monitor_start(struct ofi_mem_monitor *monitor)
{
	struct ofi_kdreg2 *kdreg2 =
		container_of(monitor, struct ofi_kdreg2, monitor);
	int ret = 0;
	struct kdreg2_config_data config_data;

	/* see if already started */
	if (kdreg2->fd >= 0)
		return 0;

	ofi_atomic_initialize64(&kdreg2->next_cookie, 1);

	ret = pipe(kdreg2->exit_pipe);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to create pipe for kdreg2: %s\n",
			strerror(errno));
		return -errno;
	}

	kdreg2->fd = open(KDREG2_DEVICE_NAME, O_RDWR);
	if (kdreg2->fd < 0) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to open %s for monitor kdreg2: %s.\n",
			KDREG2_DEVICE_NAME, strerror(errno));
		ret = -errno;
		goto close_pipe;
	}

	/* configure the monitor with the maximum number of entries */

	config_data.max_regions = cache_params.max_cnt;
	if (!config_data.max_regions) {
		ret = -FI_ENOSPC;
		goto close_fd;
	}

	ret = ioctl(kdreg2->fd, KDREG2_IOCTL_CONFIG_DATA, &config_data);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to get module config data for kdreg2 monitor: %d.\n",
			errno);
		ret = -errno;
		goto close_fd;
	}

	/* Configuring the monitor allocates the status data.  Save the address. */

	kdreg2->status_data = config_data.status_data;

	ret = pthread_create(&kdreg2->thread, EVICTOR_THREAD_ATTR,
			     kdreg2_evictor, kdreg2);
	if (ret) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Failed to start thread for kdreg2 monitor: %d.\n",
			ret);
		goto close_fd;
	}

	FI_INFO(&core_prov, FI_LOG_MR, "Kdreg2 memory monitor started.\n");

	return 0;

close_fd:

	kdreg2_close_fd(kdreg2);

close_pipe:

	kdreg2_close_pipe(kdreg2);

	FI_WARN(&core_prov, FI_LOG_MR,
		"Kdreg2 memory monitor failed to start: %i.\n", ret);

	return ret;
}

static void kdreg2_monitor_stop(struct ofi_mem_monitor *monitor)
{
	ssize_t num_written;
	struct ofi_kdreg2 *kdreg2 =
		container_of(monitor, struct ofi_kdreg2, monitor);

	/* see if it's really running */
	if (kdreg2->fd < 0)
		return;

	num_written = write(kdreg2->exit_pipe[1], "X", 1);
	if (num_written != 1) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"Unable to write to kdreg2 exit pipe: %s\n",
			strerror(errno));
		/* We could call pthread cancel here.  The thread
		 * has probably already exited.  Cancelling would be
		 * benign.  But calling join on an exited thread is
		 * also legal.
		 */
	}

	pthread_join(kdreg2->thread, NULL);

	kdreg2_close_fd(kdreg2);
	kdreg2_close_pipe(kdreg2);

	FI_INFO(&core_prov, FI_LOG_MR, "Kdreg2 memory monitor stopped.\n");
}

#else /* !HAVE_KDREG2_MONITOR */

static int kdreg2_monitor_subscribe(struct ofi_mem_monitor *monitor,
				    const void *addr,
				    size_t len,
				    union ofi_mr_hmem_info *hmem_info)
{
	return -FI_ENOSYS;
}

static void kdreg2_monitor_unsubscribe(struct ofi_mem_monitor *monitor,
				       const void *addr, size_t len,
				       union ofi_mr_hmem_info *hmem_info)
{
}

static bool kdreg2_monitor_valid(struct ofi_mem_monitor *monitor,
				 const struct ofi_mr_info *info,
				 struct ofi_mr_entry *entry)
{
	return false;
}

static int kdreg2_monitor_start(struct ofi_mem_monitor *monitor)
{
	return -FI_ENOSYS;
}

void kdreg2_monitor_stop(struct ofi_mem_monitor *monitor)
{
	/* no-op */
}

#endif /* HAVE_KDREG2_MONITOR */

static struct ofi_kdreg2 kdreg2_mm = {
	.monitor.iface       = FI_HMEM_SYSTEM,
	.monitor.init        = ofi_monitor_init,
	.monitor.cleanup     = ofi_monitor_cleanup,
	.monitor.start       = kdreg2_monitor_start,
	.monitor.stop        = kdreg2_monitor_stop,
	.monitor.subscribe   = kdreg2_monitor_subscribe,
	.monitor.unsubscribe = kdreg2_monitor_unsubscribe,
	.monitor.valid       = kdreg2_monitor_valid,
	.monitor.name        = "kdreg2",
	.fd                  = -1,
	.exit_pipe           = { -1, -1 },
	.status_data         = NULL,
};

struct ofi_mem_monitor *kdreg2_monitor = &kdreg2_mm.monitor;
