/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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

#include <stdlib.h>
#include "ofi_hook.h"


static int hook_eq_std_event(uint32_t event)
{
	return (event > FI_NOTIFY) && (event <= FI_JOIN_COMPLETE);
}

/*
 * Convert the hooked fid reported in the event to ours.  Regardless
 * of the event type, we cast to fi_eq_entry.  This works because we
 * only touch the fid field, which is the first field in all EQ events
 */
static void hook_eq_map_fid(void *buf)
{
	struct fi_eq_entry *entry = buf;

	entry->fid = entry->fid->context;
}

ssize_t hook_eq_read(struct fid_eq *eq, uint32_t *event,
			    void *buf, size_t len, uint64_t flags)
{
	struct hook_eq *myeq = container_of(eq, struct hook_eq, eq);
	ssize_t ret;

	ret = fi_eq_read(myeq->heq, event, buf, len, flags);
	if ((ret > 0) && hook_eq_std_event(*event))
		hook_eq_map_fid(buf);

	return ret;
}

static ssize_t hook_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf,
			       uint64_t flags)
{
	struct hook_eq *myeq = container_of(eq, struct hook_eq, eq);
	ssize_t ret;

	ret = fi_eq_readerr(myeq->heq, buf, flags);
	if (ret > 0)
		buf->fid = buf->fid->context;

	return ret;
}

static ssize_t hook_eq_write(struct fid_eq *eq, uint32_t event,
			     const void *buf, size_t len, uint64_t flags)
{
	struct hook_eq *myeq = container_of(eq, struct hook_eq, eq);

	return fi_eq_write(myeq->heq, event, buf, len, flags);
}

ssize_t hook_eq_sread(struct fid_eq *eq, uint32_t *event,
			     void *buf, size_t len, int timeout, uint64_t flags)
{
	struct hook_eq *myeq = container_of(eq, struct hook_eq, eq);
	ssize_t ret;

	ret = fi_eq_sread(myeq->heq, event, buf, len, timeout, flags);
	if ((ret > 0) && hook_eq_std_event(*event))
		hook_eq_map_fid(buf);

	return ret;
}

static const char *
hook_eq_strerror(struct fid_eq *eq, int prov_errno,
		 const void *err_data, char *buf, size_t len)
{
	struct hook_eq *myeq = container_of(eq, struct hook_eq, eq);

	return fi_eq_strerror(myeq->heq, prov_errno, err_data, buf, len);
}

struct fi_ops_eq hook_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = hook_eq_read,
	.readerr = hook_eq_readerr,
	.write = hook_eq_write,
	.sread = hook_eq_sread,
	.strerror = hook_eq_strerror,
};

int hook_eq_init(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context, struct hook_eq *myeq)
{
	struct hook_fabric *fab = container_of(fabric, struct hook_fabric, fabric);
	struct fi_eq_attr hattr;
	int ret;

	hattr = *attr;
	if (attr->wait_obj == FI_WAIT_SET)
		hattr.wait_set = hook_to_hwait(attr->wait_set);

	ret = fi_eq_open(fab->hfabric, &hattr, &myeq->heq, &myeq->eq.fid);
	if (ret)
		return ret;

	*eq = &myeq->eq;

	myeq->fabric = fab;
	myeq->eq.fid.fclass = FI_CLASS_EQ;
	myeq->eq.fid.context = context;
	myeq->eq.fid.ops = &hook_fid_ops;
	myeq->eq.ops = &hook_eq_ops;

	return 0;
}

int hook_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context)
{
	struct hook_eq *myeq;

	myeq = calloc(1, sizeof *myeq);
	if (!myeq)
		return -FI_ENOMEM;

	return hook_eq_init(fabric, attr, eq, context, myeq);
}
