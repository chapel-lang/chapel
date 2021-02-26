/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
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

#include "gnix_util.h"
#include "gnix_smrn.h"

static struct gnix_smrn global_smrn;

int _gnix_smrn_init(void)
{
	int ret;

	fastlock_init(&global_smrn.lock);
	global_smrn.references = 0;
	dlist_init(&global_smrn.rq_head);

	ret = _gnix_notifier_init();

	return ret;
}

int _gnix_smrn_open(struct gnix_smrn **smrn)
{
	struct gnix_smrn *tmp = &global_smrn;
	int ret = FI_SUCCESS;

	fastlock_acquire(&tmp->lock);
	if (tmp->references == 0)
		ret = _gnix_notifier_open(&tmp->notifier);

	if (!ret)
		tmp->references += 1;
	fastlock_release(&tmp->lock);

	if (!ret)
		*smrn = tmp;

	return ret;
}

int _gnix_smrn_close(struct gnix_smrn *smrn)
{
	int ret = FI_SUCCESS;

	fastlock_acquire(&smrn->lock);
	if (smrn->references == 0)
		ret = -FI_EINVAL;

	if (smrn->references == 1)
		ret = _gnix_notifier_close(smrn->notifier);

	if (!ret)
		smrn->references -= 1;
	fastlock_release(&smrn->lock);

	return ret;
}

int _gnix_smrn_monitor(struct gnix_smrn *smrn,
	struct gnix_smrn_rq *rq,
	void *addr,
	uint64_t len,
	uint64_t cookie,
	struct gnix_smrn_context *context)
{
	int ret;

	if (!context || !rq || !smrn)
		return -FI_EINVAL;

	context->rq = rq;
	context->cookie = cookie;

	ret = _gnix_notifier_monitor(smrn->notifier, addr,
				len, (uint64_t) context);
	if (ret == FI_SUCCESS)
		GNIX_DEBUG(FI_LOG_FABRIC,
				"monitoring addr=%p len=%d cookie=%p "
				"context=%p rq=%p notifier=%p\n",
				addr, len, context->cookie,
				context, rq, smrn->notifier);
	return ret;
}

int _gnix_smrn_unmonitor(struct gnix_smrn *smrn,
	uint64_t cookie,
	struct gnix_smrn_context *context)
{
	if (!smrn)
		return -FI_EINVAL;

	if (cookie != context->cookie)
		return -FI_EINVAL;

	return _gnix_notifier_unmonitor(smrn->notifier, (uint64_t) context);
}

static void __gnix_smrn_read_events(struct gnix_smrn *smrn)
{
	int ret;
	struct gnix_smrn_context *context;
	struct gnix_smrn_rq *rq;
	int len = sizeof(uint64_t);

	do {
		ret = _gnix_notifier_get_event(smrn->notifier,
			(void *) &context, len);
		if (ret != len) {
			GNIX_DEBUG(FI_LOG_FABRIC,
				"no more events to be read\n");
			break;
		}

		GNIX_DEBUG(FI_LOG_FABRIC,
			"found event, context=%p rq=%p cookie=%lx\n",
			context, context->rq, context->cookie);

		rq = context->rq;
		fastlock_acquire(&rq->lock);
		dlist_insert_tail(&context->entry, &rq->list);
		fastlock_release(&rq->lock);
	} while (ret == len);
}

int _gnix_smrn_get_event(struct gnix_smrn *smrn,
	struct gnix_smrn_rq *rq,
	struct gnix_smrn_context **context)
{
	int ret;

	if (!smrn || !context)
		return -FI_EINVAL;

	__gnix_smrn_read_events(smrn);

	fastlock_acquire(&rq->lock);
	if (!dlist_empty(&rq->list)) {
		dlist_pop_front(&rq->list, struct gnix_smrn_context,
			*context, entry);
		ret = FI_SUCCESS;
	} else
		ret = -FI_EAGAIN;
	fastlock_release(&rq->lock);

	return ret;
}

