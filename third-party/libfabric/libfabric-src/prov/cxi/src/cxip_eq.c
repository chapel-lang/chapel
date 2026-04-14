/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2020-2024 Cray Inc. All rights reserved.
 */

 /*
  * Notes:
  *
  * Implemented as an extension of util_eq.
  *
  * At present, the cxip_wait objects are not implemented as extensions of the
  * util_wait object, so we cannot currently fully implement the EQ with wait
  * states. However, the non-blocking read() and peek() functions work.
  */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EQ, __VA_ARGS__)

static int cxip_eq_close(struct fid *fid)
{
	struct cxip_eq *cxi_eq;

	cxi_eq = container_of(fid, struct cxip_eq, util_eq.eq_fid.fid);

	/* May not close until all bound EPs closed */
	if (ofi_atomic_get32(&cxi_eq->util_eq.ref))
		return -FI_EBUSY;

	ofi_mutex_destroy(&cxi_eq->list_lock);
	ofi_eq_cleanup(&cxi_eq->util_eq.eq_fid.fid);
	free(cxi_eq);

	return FI_SUCCESS;
}

static void cxip_eq_progress(struct cxip_eq *eq)
{
	struct cxip_ep_obj *ep_obj;

	ofi_mutex_lock(&eq->list_lock);
	dlist_foreach_container(&eq->ep_list, struct cxip_ep_obj,
				ep_obj, eq_link) {
		cxip_coll_progress_join(ep_obj);
	}
	ofi_mutex_unlock(&eq->list_lock);
}

/* cxip_cq_strerror() - Converts provider specific error information into a
 * printable string. Not eq-specific.
 */
static const char *cxip_eq_strerror(struct fid_eq *eq, int prov_errno,
				    const void *err_data, char *buf, size_t len)
{
	const char *errmsg = cxip_strerror(prov_errno);
	if (buf && len > 0)
		strncpy(buf, errmsg, len);
	return errmsg;
}

ssize_t cxip_eq_read(struct fid_eq *eq_fid, uint32_t *event,
		     void *buf, size_t len, uint64_t flags)
{
	struct cxip_eq *eq;
	int ret;

	eq = container_of(eq_fid, struct cxip_eq, util_eq.eq_fid.fid);

	ret = ofi_eq_read(eq_fid, event, buf, len, flags);
	if (ret == -FI_EAGAIN)
		cxip_eq_progress(eq);
	return ret;
}

static ssize_t cxip_eq_sread(struct fid_eq *eq_fid, uint32_t *event, void *buf,
			     size_t len, int timeout, uint64_t flags)
{
	struct cxip_eq *eq = container_of(eq_fid, struct cxip_eq,
					  util_eq.eq_fid);
	uint64_t endtime;
	ssize_t ret;

	if (eq->attr.wait_obj == FI_WAIT_NONE)
		return -FI_ENOSYS;

	assert(eq->attr.wait_obj == FI_WAIT_YIELD);

	endtime = ofi_timeout_time(timeout);
	do {
		/* Read initiates EQ progress if empty */
		ret = cxip_eq_read(eq_fid, event, buf, len, flags);
		if (ret != -FI_EAGAIN)
			break;

		/* On timeout return -FI_EAGAIN */
		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_EAGAIN;

		sched_yield();
	} while (ret == -FI_EAGAIN);

	return ret;
}

static int cxip_eq_control(struct fid *fid, int command, void *arg)
{
	struct cxip_eq *eq = container_of(fid, struct cxip_eq, util_eq.eq_fid);
	int ret;

	switch (command) {
	case FI_GETWAITOBJ:
		*(enum fi_wait_obj *) arg = eq->attr.wait_obj;
		ret = FI_SUCCESS;
		break;
	default:
		CXIP_WARN("Unsupported EQ control command: %d\n", command);
		ret = -FI_ENOSYS;
	};

	return ret;
}

static struct fi_ops_eq cxi_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = cxip_eq_read,		// customized
	.readerr = ofi_eq_readerr,
	.sread = cxip_eq_sread,		// customized
	.write = ofi_eq_write,
	.strerror = cxip_eq_strerror,	// customized
};

static struct fi_ops cxi_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_eq_close,		// customized
	.bind = fi_no_bind,
	.control = cxip_eq_control,	// customized
	.ops_open = fi_no_ops_open,
};

static struct fi_eq_attr cxip_eq_def_attr = {
	.size = CXIP_EQ_DEF_SZ,
	.flags = 0,
	.wait_obj = FI_WAIT_NONE,
	.signaling_vector = 0,
	.wait_set = NULL
};

static int cxip_eq_verify_attr(struct fi_eq_attr *attr)
{
	if (!attr)
		return FI_SUCCESS;

	/* Applications should set wait_obj == FI_WAIT_NONE for best
	 * performance. If a wait_obj is required, only FI_WAIT_YIELD
	 * is supported. This is due to collectives not currently
	 * exposing the next internal timeout duration.
	 */
	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
		attr->wait_obj = FI_WAIT_YIELD;
		break;
	case FI_WAIT_NONE:
	case FI_WAIT_YIELD:
		break;
	default:
		CXIP_WARN("Unsupported EQ wait object: %d\n",
			  attr->wait_obj);
		return -FI_ENOSYS;
	}

	return FI_SUCCESS;
}

int cxip_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context)
{
	struct cxip_eq *cxi_eq;
	struct fi_eq_attr temp_attr;
	int ret;

	ret = cxip_eq_verify_attr(attr);
	if (ret != FI_SUCCESS)
		return ret;

	cxi_eq = calloc(1, sizeof(*cxi_eq));
	if (!cxi_eq)
		return -FI_ENOMEM;

	if (!attr)
		cxi_eq->attr = cxip_eq_def_attr;
	else
		cxi_eq->attr = *attr;

	/* CXI does not use common code internal wait object */
	temp_attr = cxi_eq->attr;
	temp_attr.wait_obj = FI_WAIT_NONE;
	ret = ofi_eq_init(fabric, &temp_attr, &cxi_eq->util_eq.eq_fid, context);
	if (ret != FI_SUCCESS)
		goto err0;

	ofi_mutex_init(&cxi_eq->list_lock);
	dlist_init(&cxi_eq->ep_list);
	ofi_atomic_initialize32(&cxi_eq->util_eq.ref, 0);

	/* custom operations */
	cxi_eq->util_eq.eq_fid.fid.ops = &cxi_eq_fi_ops;
	cxi_eq->util_eq.eq_fid.ops = &cxi_eq_ops;

	*eq = &cxi_eq->util_eq.eq_fid;

	return FI_SUCCESS;
err0:
	free(cxi_eq);
	return ret;
}
