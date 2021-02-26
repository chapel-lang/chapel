/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"

int psmx2_trx_ctxt_cnt = 0;

/*
 * Tx/Rx context disconnect protocol:
 *
 * TRX_CTXT disconnect REQ:
 *	args[0].u32w0	cmd
 *
 * Before a PSM2 endpoint is closed, a TRX_CTXT disconnect REQ is sent to
 * all connected peers. Each peer then calls psm2_ep_disconnet() to clean
 * up the local connection state. This allows a future endpoint with the
 * same epid to connect to the same peers.
 */

struct disconnect_args {
	struct psmx2_trx_ctxt	*trx_ctxt;
	psm2_epaddr_t		epaddr;
};

static void *disconnect_func(void *args)
{
	struct disconnect_args *disconn = args;
	struct psmx2_trx_ctxt *trx_ctxt = disconn->trx_ctxt;
	struct psmx2_epaddr_context *epaddr_context;
	psm2_error_t errors;

	FI_INFO(&psmx2_prov, FI_LOG_CORE,
		"psm2_ep: %p, epaddr: %p\n", trx_ctxt->psm2_ep, disconn->epaddr);

	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_remove_first_match(&trx_ctxt->peer_list,
				 psmx2_peer_match, disconn->epaddr);
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);
	if (trx_ctxt->ep && trx_ctxt->ep->av)
		psmx2_av_remove_conn(trx_ctxt->ep->av, trx_ctxt, disconn->epaddr);

	epaddr_context = psm2_epaddr_getctxt(disconn->epaddr);
	psm2_epaddr_setctxt(disconn->epaddr, NULL);
	free(epaddr_context);

	psm2_ep_disconnect2(trx_ctxt->psm2_ep, 1, &disconn->epaddr, NULL,
			    &errors, PSM2_EP_DISCONNECT_FORCE, 0);

	free(args);
	return NULL;
}

int psmx2_am_trx_ctxt_handler(psm2_am_token_t token, psm2_amarg_t *args,
			      int nargs, void *src, uint32_t len, void *hctx)
{
	psm2_epaddr_t epaddr;
	int err = 0;
	int cmd;
	struct disconnect_args *disconn;
	pthread_t disconnect_thread;
	struct psmx2_trx_ctxt *trx_ctxt;
	trx_ctxt = (struct psmx2_trx_ctxt *)hctx;

	psm2_am_get_source(token, &epaddr);
	cmd = PSMX2_AM_GET_OP(args[0].u32w0);

	switch(cmd) {
	case PSMX2_AM_REQ_TRX_CTXT_DISCONNECT:
		/*
		 * we can't call psm2_ep_disconnect from the AM
		 * handler. instead, create a thread to do the work.
		 * the performance of this operation is not important.
		 *
		 * also put the av cleanup operations into the thread
		 * to avoid deadlock because the AM handler may be
		 * called with the av lock held.
		 */
		disconn = malloc(sizeof(*disconn));
		if (disconn) {
			disconn->trx_ctxt = trx_ctxt;
			disconn->epaddr = epaddr;
			pthread_create(&disconnect_thread, NULL,
				       disconnect_func, disconn);
			pthread_detach(disconnect_thread);
		}
		break;

	default:
		err = -FI_EINVAL;
		break;
	}

	return err;
}

void psmx2_trx_ctxt_disconnect_peers(struct psmx2_trx_ctxt *trx_ctxt)
{
	struct dlist_entry *item, *tmp;
	struct psmx2_epaddr_context *peer;
	struct dlist_entry peer_list;
	psm2_amarg_t arg;

	arg.u32w0 = PSMX2_AM_REQ_TRX_CTXT_DISCONNECT;

	/* use local peer_list to avoid entering AM handler while holding the lock */
	dlist_init(&peer_list);
	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_foreach_safe(&trx_ctxt->peer_list, item, tmp) {
		dlist_remove(item);
		dlist_insert_before(item, &peer_list);
	}
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);

	dlist_foreach_safe(&peer_list, item, tmp) {
		peer = container_of(item, struct psmx2_epaddr_context, entry);
		if (psmx2_env.disconnect) {
			FI_INFO(&psmx2_prov, FI_LOG_CORE, "epaddr: %p\n", peer->epaddr);
			psm2_am_request_short(peer->epaddr, PSMX2_AM_TRX_CTXT_HANDLER,
					      &arg, 1, NULL, 0, 0, NULL, NULL);
		}
		psm2_epaddr_setctxt(peer->epaddr, NULL);
		free(peer);
	}
}

static const char *psmx2_usage_flags_to_string(int usage_flags)
{
	switch (usage_flags & PSMX2_TX_RX) {
	case PSMX2_TX: return "tx";
	case PSMX2_RX: return "rx";
	default: return "tx+rx";
	}
}

void psmx2_trx_ctxt_free(struct psmx2_trx_ctxt *trx_ctxt, int usage_flags)
{
	int err;
	int old_flags;

	if (!trx_ctxt)
		return;

	old_flags = trx_ctxt->usage_flags;
	trx_ctxt->usage_flags &= ~usage_flags;
	if (trx_ctxt->usage_flags) {
		FI_INFO(&psmx2_prov, FI_LOG_CORE, "epid: %016lx (%s -> %s)\n",
			trx_ctxt->psm2_epid,
			psmx2_usage_flags_to_string(old_flags),
			psmx2_usage_flags_to_string(trx_ctxt->usage_flags));
		return;
	}

	FI_INFO(&psmx2_prov, FI_LOG_CORE, "epid: %016lx (%s)\n",
		trx_ctxt->psm2_epid, psmx2_usage_flags_to_string(old_flags));

	trx_ctxt->am_progress = 0;
	trx_ctxt->poll_active = 0;

	trx_ctxt->domain->trx_ctxt_lock_fn(&trx_ctxt->domain->trx_ctxt_lock, 1);
	dlist_remove(&trx_ctxt->entry);
	trx_ctxt->domain->trx_ctxt_unlock_fn(&trx_ctxt->domain->trx_ctxt_lock, 1);

	psmx2_trx_ctxt_disconnect_peers(trx_ctxt);

	if (trx_ctxt->am_initialized)
		psmx2_am_fini(trx_ctxt);

#if 0
	/* AM messages could arrive after MQ is finalized, causing segfault
	 * when trying to dereference the MQ pointer. There is no mechanism
	 * to properly shutdown AM. The workaround is to keep MQ valid.
	 */
	psm2_mq_finalize(trx_ctxt->psm2_mq);
#endif

	/* workaround for:
	 * Assertion failure at psm2_ep.c:1059: ep->mctxt_master == ep
	 */
	if (psmx2_env.delay)
		sleep(psmx2_env.delay);

	if (psmx2_env.timeout)
		err = psm2_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_GRACEFUL,
				    (int64_t) psmx2_env.timeout * 1000000000LL);
	else
		err = PSM2_EP_CLOSE_TIMEOUT;

	if (err != PSM2_OK)
		psm2_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_FORCE, 0);

	ofi_bufpool_destroy(trx_ctxt->am_req_pool);
	fastlock_destroy(&trx_ctxt->am_req_pool_lock);
	fastlock_destroy(&trx_ctxt->poll_lock);
	fastlock_destroy(&trx_ctxt->peer_lock);

	if (!ofi_atomic_dec32(&trx_ctxt->poll_refcnt))
		free(trx_ctxt);
}

struct psmx2_trx_ctxt *psmx2_trx_ctxt_alloc(struct psmx2_fid_domain *domain,
					    struct psmx2_ep_name *src_addr,
					    int sep_ctxt_idx,
					    int usage_flags)
{
	struct psmx2_trx_ctxt *trx_ctxt;
	struct psm2_ep_open_opts opts;
	int should_retry = 0;
	int err;
	struct dlist_entry *item;
	int asked_flags = usage_flags & PSMX2_TX_RX;
	int compatible_flags = ~asked_flags & PSMX2_TX_RX;

	/* Check existing allocations first if only Tx or Rx is needed */
	if (compatible_flags) {
		domain->trx_ctxt_lock_fn(&domain->trx_ctxt_lock, 1);
		dlist_foreach(&domain->trx_ctxt_list, item) {
			trx_ctxt = container_of(item, struct psmx2_trx_ctxt, entry);
			if (compatible_flags == trx_ctxt->usage_flags) {
				trx_ctxt->usage_flags |= asked_flags;
				domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);
				FI_INFO(&psmx2_prov, FI_LOG_CORE,
					"use existing context. epid: %016lx "
					"(%s -> tx+rx).\n", trx_ctxt->psm2_epid,
					psmx2_usage_flags_to_string(compatible_flags));
				return trx_ctxt;
			}
		}
		domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);
	}

	if (psmx2_trx_ctxt_cnt >= psmx2_hfi_info.max_trx_ctxt) {
		FI_WARN(&psmx2_prov, FI_LOG_CORE,
			"number of Tx/Rx contexts exceeds limit (%d).\n",
			psmx2_hfi_info.max_trx_ctxt);
		return NULL;
	}

	trx_ctxt = calloc(1, sizeof(*trx_ctxt));
	if (!trx_ctxt) {
		FI_WARN(&psmx2_prov, FI_LOG_CORE,
			"failed to allocate trx_ctxt.\n");
		return NULL;
	}

	err = ofi_bufpool_create(&trx_ctxt->am_req_pool,
				 sizeof(struct psmx2_am_request),
				 sizeof(void *), 0, 64, 0);
	if (err) {
		FI_WARN(&psmx2_prov, FI_LOG_CORE,
			"failed to allocate am_req_pool.\n");
		goto err_out;
	}

	psm2_ep_open_opts_get_defaults(&opts);
	FI_INFO(&psmx2_prov, FI_LOG_CORE,
		"uuid: %s\n", psmx2_uuid_to_string(domain->fabric->uuid));

	opts.unit = src_addr ? src_addr->unit : PSMX2_DEFAULT_UNIT;
	opts.port = src_addr ? src_addr->port : PSMX2_DEFAULT_PORT;
	FI_INFO(&psmx2_prov, FI_LOG_CORE,
		"ep_open_opts: unit=%d port=%u\n", opts.unit, opts.port);

	if (opts.unit < 0 && sep_ctxt_idx >= 0) {
		should_retry = 1;
		opts.unit = psmx2_get_round_robin_unit(sep_ctxt_idx);
		FI_INFO(&psmx2_prov, FI_LOG_CORE,
			"sep %d: ep_open_opts: unit=%d\n", sep_ctxt_idx, opts.unit);
	}

	err = psm2_ep_open(domain->fabric->uuid, &opts,
			   &trx_ctxt->psm2_ep, &trx_ctxt->psm2_epid);
	if (err != PSM2_OK) {
		FI_WARN(&psmx2_prov, FI_LOG_CORE,
			"psm2_ep_open returns %d, errno=%d\n", err, errno);
		if (!should_retry)
			goto err_out_destroy_pool;

		/* When round-robin fails, retry w/o explicit assignment */
		opts.unit = -1;
		err = psm2_ep_open(domain->fabric->uuid, &opts,
				   &trx_ctxt->psm2_ep, &trx_ctxt->psm2_epid);
		if (err != PSM2_OK) {
			FI_WARN(&psmx2_prov, FI_LOG_CORE,
				"psm2_ep_open retry returns %d, errno=%d\n", err, errno);
			goto err_out_destroy_pool;
		}
	}

	FI_INFO(&psmx2_prov, FI_LOG_CORE,
		"epid: %016lx (%s)\n", trx_ctxt->psm2_epid,
		psmx2_usage_flags_to_string(usage_flags));

	err = psm2_mq_init(trx_ctxt->psm2_ep, PSM2_MQ_ORDERMASK_ALL,
			   NULL, 0, &trx_ctxt->psm2_mq);
	if (err != PSM2_OK) {
		FI_WARN(&psmx2_prov, FI_LOG_CORE,
			"psm2_mq_init returns %d, errno=%d\n", err, errno);
		goto err_out_close_ep;
	}

#if !HAVE_PSM2_MQ_FP_MSG
	fastlock_init(&trx_ctxt->rma_queue.lock);
	slist_init(&trx_ctxt->rma_queue.list);
#endif
	fastlock_init(&trx_ctxt->peer_lock);
	fastlock_init(&trx_ctxt->poll_lock);
	fastlock_init(&trx_ctxt->am_req_pool_lock);
	fastlock_init(&trx_ctxt->trigger_queue.lock);
	dlist_init(&trx_ctxt->peer_list);
	slist_init(&trx_ctxt->trigger_queue.list);
	trx_ctxt->id = psmx2_trx_ctxt_cnt++;
	trx_ctxt->domain = domain;
	trx_ctxt->usage_flags = asked_flags;
	trx_ctxt->poll_active = 1;
	ofi_atomic_initialize32(&trx_ctxt->poll_refcnt, 1); /* take one ref for domain->trx_ctxt_list */

	domain->trx_ctxt_lock_fn(&domain->trx_ctxt_lock, 1);
	dlist_insert_before(&trx_ctxt->entry, &domain->trx_ctxt_list);
	domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);

	return trx_ctxt;

err_out_close_ep:
	if (psm2_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_GRACEFUL,
			  (int64_t) psmx2_env.timeout * 1000000000LL) != PSM2_OK)
		psm2_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_FORCE, 0);

err_out_destroy_pool:
	ofi_bufpool_destroy(trx_ctxt->am_req_pool);

err_out:
	free(trx_ctxt);
	return NULL;
}

