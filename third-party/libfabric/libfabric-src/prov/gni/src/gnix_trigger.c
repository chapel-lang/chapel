/*
 * Copyright (c) 2016 Cray Inc. All rights reserved.
 * Copyright (c) 2016 Los Alamos National Security, LLC. All rights reserved.
 *
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

/*
 * Triggered operations handling.
 */

#include "gnix_trigger.h"
#include "gnix_vc.h"
#include "gnix.h"

int _gnix_trigger_queue_req(struct gnix_fab_req *req)
{
	struct fi_triggered_context *trigger_context;
	struct fi_trigger_threshold *threshold;
	struct gnix_fid_cntr *cntr;
	struct gnix_fab_req *r;
	size_t req_thresh;

	trigger_context = (struct fi_triggered_context *)
				req->user_context;
	threshold = &trigger_context->trigger.threshold;
	cntr = container_of(threshold->cntr, struct gnix_fid_cntr, cntr_fid);

	if (ofi_atomic_get32(&cntr->cnt) >= threshold->threshold) {
		GNIX_INFO(FI_LOG_EP_DATA,
			  "Trigger condition met: %p\n",
			  req);

		/* Trigger condition has already been met. */
		return 1;
	}

	GNIX_INFO(FI_LOG_EP_DATA,
		  "Queueing triggered op: %p\n",
		  req);

	fastlock_acquire(&cntr->trigger_lock);
	if (dlist_empty(&cntr->trigger_list)) {
		dlist_init(&req->dlist);
		dlist_insert_head(&req->dlist, &cntr->trigger_list);
	} else {
		req_thresh = threshold->threshold;

		dlist_for_each(&cntr->trigger_list, r, dlist) {
			trigger_context = (struct fi_triggered_context *)
						r->user_context;
			threshold = &trigger_context->trigger.threshold;

			/* Insert new req. after those with equal threshold and
			 * before those with greater threshold. */
			if (req_thresh < threshold->threshold) {
				break;
			}
		}

		dlist_init(&req->dlist);
		dlist_insert_before(&req->dlist, &r->dlist);
	}
	fastlock_release(&cntr->trigger_lock);

	return FI_SUCCESS;
}

void _gnix_trigger_check_cntr(struct gnix_fid_cntr *cntr)
{
	struct fi_triggered_context *trigger_context;
	struct fi_trigger_threshold *threshold;
	struct gnix_fab_req *req, *req2;
	size_t count;

	if (OFI_LIKELY(dlist_empty(&cntr->trigger_list))) {
		return;
	}

	 count = ofi_atomic_get32(&cntr->cnt);

	fastlock_acquire(&cntr->trigger_lock);
	dlist_for_each_safe(&cntr->trigger_list, req, req2, dlist) {
		trigger_context = (struct fi_triggered_context *)
					req->user_context;
		threshold = &trigger_context->trigger.threshold;

		if (count >= threshold->threshold) {
			GNIX_INFO(FI_LOG_EP_DATA,
				  "Trigger condition met: %p\n",
				  req);

			dlist_remove_init(&req->dlist);
			req->flags &= ~FI_TRIGGER;
			_gnix_vc_queue_tx_req(req);
		} else {
			break;
		}
	}
	fastlock_release(&cntr->trigger_lock);
}
