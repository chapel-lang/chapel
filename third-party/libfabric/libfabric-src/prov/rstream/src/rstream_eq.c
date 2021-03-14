/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"

static int rstream_eq_events(uint32_t *event, struct fi_eq_cm_entry *cm_entry,
	struct fi_eq_cm_entry *usr_cm_entry, struct rstream_eq *rstream_eq)
{
	int ret = 0;

	if (*event == FI_CONNREQ) {
	/* have to store to transfer to ep during FI_CONNECT */
		if (cm_entry->info) {
			usr_cm_entry->info = cm_entry->info;
			rstream_set_info(usr_cm_entry->info);
		}
	} else if (*event == FI_CONNECTED) {
		struct rstream_ep *rstream_ep = NULL;
		void *itr = rbtFind(rstream_eq->ep_map, cm_entry->fid);
		assert(itr);
		rbtKeyValue(rstream_eq->ep_map, itr,
			(void **) &cm_entry->fid, (void **) &rstream_ep);
		rstream_process_cm_event(rstream_ep, cm_entry->data);
		usr_cm_entry->fid = &rstream_ep->util_ep.ep_fid.fid;
	} else {
		ret = -FI_ENODATA;
	}
	rstream_eq->prev_cm_state = *event;
	return ret;
}

static ssize_t rstream_read(struct fid_eq *eq, uint32_t *event,
	void *buf, size_t len, uint64_t flags)
{
	uint32_t rlen = sizeof(struct fi_eq_cm_entry);
	assert(len == rlen && event);
	struct fi_eq_cm_entry *usr_cm_entry = (struct fi_eq_cm_entry *) buf;
	ssize_t ret;
	struct fi_eq_cm_entry *cm_entry = NULL;

	struct rstream_eq *rstream_eq = container_of(eq,
		struct rstream_eq, util_eq.eq_fid);

	cm_entry = rstream_eq->cm_entry;
	assert(cm_entry);

	if (rstream_eq->prev_cm_state != FI_CONNREQ) {
		rlen = rlen + rstream_eq->cm_data_len;
	}

	ret = fi_eq_read(rstream_eq->eq_fd, event, cm_entry, rlen, flags);
	if (ret == rlen) {
		ret = rstream_eq_events(event, cm_entry, usr_cm_entry, rstream_eq);
		if (ret)
			return ret;
	} else {
		return ret;
	}

	return len;
}

static ssize_t rstream_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf,
	uint64_t flags)
{
	struct rstream_eq *rstream_eq = container_of(eq,
		struct rstream_eq, util_eq.eq_fid);

	return fi_eq_readerr(rstream_eq->eq_fd, buf, flags);
}

static ssize_t rstream_sread(struct fid_eq *eq, uint32_t *event,
	void *buf, size_t len, int timeout, uint64_t flags)
{
	uint32_t rlen = sizeof(struct fi_eq_cm_entry);
	assert(len == rlen && event);
	struct fi_eq_cm_entry *usr_cm_entry = (struct fi_eq_cm_entry *) buf;
	ssize_t ret;
	struct fi_eq_cm_entry *cm_entry = NULL;

	struct rstream_eq *rstream_eq = container_of(eq,
		struct rstream_eq, util_eq.eq_fid);

	cm_entry = rstream_eq->cm_entry;
	assert(cm_entry);

	if (rstream_eq->prev_cm_state != FI_CONNREQ) {
		rlen = rlen + rstream_eq->cm_data_len;
	}

	ret = fi_eq_sread(rstream_eq->eq_fd, event, cm_entry, rlen, timeout,
		flags);
	if (ret == rlen) {
		ret = rstream_eq_events(event, cm_entry, usr_cm_entry, rstream_eq);
		if (ret)
			return ret;
	} else {
		return ret;
	}

	return len;
}

static const char *rstream_strerror(struct fid_eq *eq, int prov_errno,
	const void *err_data, char *buf, size_t len)
{
	struct rstream_eq *rstream_eq = container_of(eq, struct rstream_eq,
		util_eq.eq_fid);

	return fi_eq_strerror(rstream_eq->eq_fd, prov_errno, err_data, buf, len);
}

static int rstream_eq_control(fid_t fid, int command, void *arg)
{
	struct rstream_eq *rstream_eq = container_of(fid, struct rstream_eq,
		util_eq.eq_fid.fid);
	int ret;

	switch (command) {
	case FI_GETWAIT:
		ret = fi_control(&rstream_eq->eq_fd->fid, FI_GETWAIT, arg);
		break;
	default:
		return -FI_ENOSYS;
	}
	return ret;
}

static int rstream_eq_close(fid_t fid)
{
	struct rstream_eq *rstream_eq =
		container_of(fid, struct rstream_eq, util_eq.eq_fid.fid);
	int ret;

	ret = fi_close(&rstream_eq->eq_fd->fid);
	if (ret)
		return ret;

	free(rstream_eq->cm_entry);
	free(rstream_eq);
	return ret;
}

static struct fi_ops_eq rstream_ops_eq = {
	.size = sizeof(struct fi_ops_eq),
	.read = rstream_read,
	.readerr = rstream_readerr,
	.write = fi_no_eq_write,
	.sread = rstream_sread,
	.strerror = rstream_strerror,
};

static struct fi_ops rstream_fid_ops_eq = {
	.size = sizeof(struct fi_ops),
	.close = rstream_eq_close,
	.bind = fi_no_bind,
	.control = rstream_eq_control,
	.ops_open = fi_no_ops_open,
};

/* assumes uint64_t keys */
static int compare_mr_keys(void *key1, void *key2)
{
	uint64_t k1 = *((uint64_t *) key1);
	uint64_t k2 = *((uint64_t *) key2);

	return (k1 < k2) ? -1 : (k1 > k2);
}

int rstream_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
	struct fid_eq **eq, void *context)
{
	struct rstream_fabric *rstream_fabric = NULL;
	struct rstream_eq *rstream_eq;
	int ret;

	rstream_eq = calloc(1, sizeof(*rstream_eq));
	if (!rstream_eq)
		return -FI_ENOMEM;

	rstream_fabric = container_of(fabric, struct rstream_fabric,
		util_fabric.fabric_fid);

	ret = fi_eq_open(rstream_fabric->msg_fabric, attr, &rstream_eq->eq_fd,
		NULL);
	if (ret)
		goto err1;

	(*eq) = &rstream_eq->util_eq.eq_fid;
	(*eq)->fid.fclass = FI_CLASS_EQ;
	(*eq)->fid.context = context;
	(*eq)->ops = &rstream_ops_eq;
	(*eq)->fid.ops = &rstream_fid_ops_eq;
	rstream_eq->cm_data_len = sizeof(struct rstream_cm_data);
	rstream_eq->cm_entry = calloc(1, sizeof(struct fi_eq_cm_entry) +
		rstream_eq->cm_data_len);
	rstream_eq->ep_map = rbtNew(compare_mr_keys);
	rstream_eq->prev_cm_state = FI_NOTIFY;

	return ret;
err1:
	free(rstream_eq);

	return ret;
}
