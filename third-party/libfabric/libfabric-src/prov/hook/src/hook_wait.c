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


static int hook_do_poll(struct fid_poll *pollset, void **context, int count)
{
	struct hook_poll *poll = container_of(pollset, struct hook_poll, poll);
	struct fid *fid;
	int i, ret;

	ret = fi_poll(poll->hpoll, context, count);
	for (i = 0; i < ret; i++) {
		fid = context[i];
		context[i] = fid->context;
	}

	return ret;
}

static int hook_poll_add(struct fid_poll *pollset, struct fid *event_fid,
			 uint64_t flags)
{
	struct hook_poll *poll = container_of(pollset, struct hook_poll, poll);

	return fi_poll_add(poll->hpoll, hook_to_hfid(event_fid), flags);
}

static int hook_poll_del(struct fid_poll *pollset, struct fid *event_fid,
			 uint64_t flags)
{
	struct hook_poll *poll = container_of(pollset, struct hook_poll, poll);

	return fi_poll_del(poll->hpoll, hook_to_hfid(event_fid), flags);
}

static struct fi_ops_poll hook_poll_ops = {
	.size = sizeof(struct fi_ops_poll),
	.poll = hook_do_poll,
	.poll_add = hook_poll_add,
	.poll_del = hook_poll_del,
};

int hook_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
		   struct fid_poll **pollset)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_poll *poll;
	int ret;

	poll = calloc(1, sizeof *poll);
	if (!poll)
		return -FI_ENOMEM;

	poll->domain = dom;
	poll->poll.fid.fclass = FI_CLASS_POLL;
	poll->poll.fid.ops = &hook_fid_ops;
	poll->poll.ops = &hook_poll_ops;

	ret = fi_poll_open(dom->hdomain, attr, &poll->hpoll);
	if (ret)
		free(poll);
	else
		*pollset = &poll->poll;

	return ret;
}


int hook_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	struct hook_fabric *fab = container_of(fabric, struct hook_fabric, fabric);
	struct fid *hfid;
	int i, ret;

	for (i = 0; i < count; i++) {
		hfid = hook_to_hfid(fids[i]);
		if (!hfid)
			return -FI_EINVAL;

		ret = fi_trywait(fab->hfabric, &hfid, 1);
		if (ret)
			return ret;
	}

	return 0;
}

static int hook_do_wait(struct fid_wait *waitset, int timeout)
{
	struct hook_wait *wait = container_of(waitset, struct hook_wait, wait);

	return fi_wait(wait->hwait, timeout);
}

static struct fi_ops_wait hook_wait_ops = {
	.size = sizeof(struct fi_ops_wait),
	.wait = hook_do_wait,
};

int hook_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset)
{
	struct hook_fabric *fab = container_of(fabric, struct hook_fabric, fabric);
	struct hook_wait *wait;
	int ret;

	wait = calloc(1, sizeof *wait);
	if (!wait)
		return -FI_ENOMEM;

	wait->fabric = fab;
	wait->wait.fid.fclass = FI_CLASS_WAIT;
	wait->wait.fid.ops = &hook_fid_ops;
	wait->wait.ops = &hook_wait_ops;

	ret = fi_wait_open(fab->hfabric, attr, &wait->hwait);
	if (ret)
		free(wait);
	else
		*waitset = &wait->wait;

	return ret;
}

