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
#include <ofi_enosys.h>
#include <rdma/fi_ext.h>

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
