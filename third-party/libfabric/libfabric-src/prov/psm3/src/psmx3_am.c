/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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

#include "psmx3.h"
#include "psmx3_trigger.h"

int psmx3_am_progress(struct psmx3_trx_ctxt *trx_ctxt)
{
	struct slist_entry *item;
	struct psmx3_trigger *trigger;

	trx_ctxt->domain->trigger_queue_lock_fn(&trx_ctxt->trigger_queue.lock, 2);
	while (!slist_empty(&trx_ctxt->trigger_queue.list)) {
		item = slist_remove_head(&trx_ctxt->trigger_queue.list);
		trigger = container_of(item, struct psmx3_trigger, list_entry);
		trx_ctxt->domain->trigger_queue_unlock_fn(&trx_ctxt->trigger_queue.lock, 2);
		psmx3_process_trigger(trx_ctxt, trigger);
		trx_ctxt->domain->trigger_queue_lock_fn(&trx_ctxt->trigger_queue.lock, 2);
	}
	trx_ctxt->domain->trigger_queue_unlock_fn(&trx_ctxt->trigger_queue.lock, 2);

	return 0;
}

int psmx3_am_init(struct psmx3_trx_ctxt *trx_ctxt)
{
	psm2_am_handler_2_fn_t psmx3_am_handlers[4];
	struct psmx3_trx_ctxt *hctx[4];
	int psmx3_am_handlers_idx[4];
	int num_handlers = 4;

	psm2_ep_t psm2_ep = trx_ctxt->psm2_ep;
	size_t size;
	int err = 0;
	uint32_t max_atomic_size;

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "epid %s\n", psm3_epid_fmt(trx_ctxt->psm2_epid, 0));

	if (!trx_ctxt->am_initialized) {
		err = psm3_am_get_parameters(psm2_ep, &trx_ctxt->psm2_am_param,
					     sizeof(struct psm2_am_parameters),
					     &size);
		if (err)
			return psmx3_errno(err);

		max_atomic_size = trx_ctxt->psm2_am_param.max_request_short;
		if (trx_ctxt->domain->max_atomic_size > max_atomic_size)
			trx_ctxt->domain->max_atomic_size = max_atomic_size;

		psmx3_am_handlers[0] = psmx3_am_rma_handler;
		hctx[0] = trx_ctxt;
		psmx3_am_handlers[1] = psmx3_am_atomic_handler;
		hctx[1] = trx_ctxt;
		psmx3_am_handlers[2] = psmx3_am_sep_handler;
		hctx[2] = trx_ctxt;
		psmx3_am_handlers[3] = psmx3_am_trx_ctxt_handler;
		hctx[3] = trx_ctxt;

		err = psm3_am_register_handlers_2(psm2_ep, psmx3_am_handlers,
						num_handlers, (void **)hctx, psmx3_am_handlers_idx);
		if (err)
			return psmx3_errno(err);

		if ((psmx3_am_handlers_idx[0] != PSMX3_AM_RMA_HANDLER) ||
		    (psmx3_am_handlers_idx[1] != PSMX3_AM_ATOMIC_HANDLER) ||
		    (psmx3_am_handlers_idx[2] != PSMX3_AM_SEP_HANDLER) ||
		    (psmx3_am_handlers_idx[3] != PSMX3_AM_TRX_CTXT_HANDLER)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"failed to register one or more AM handlers "
				"at indecies %d, %d, %d, %d\n", PSMX3_AM_RMA_HANDLER,
				PSMX3_AM_ATOMIC_HANDLER, PSMX3_AM_SEP_HANDLER,
				PSMX3_AM_TRX_CTXT_HANDLER);
			return -FI_EBUSY;
		}

		trx_ctxt->am_initialized = 1;
	}

	return err;
}

void psmx3_am_fini(struct psmx3_trx_ctxt *trx_ctxt)
{
	/* there is no way to unregister AM handlers */
}

