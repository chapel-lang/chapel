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

#include "psmx3.h"

int psmx3_trx_ctxt_cnt = 0;

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
	struct psmx3_trx_ctxt	*trx_ctxt;
	psm2_epaddr_t		epaddr;
};

static void *disconnect_func(void *args)
{
	struct disconnect_args *disconn = args;
	struct psmx3_trx_ctxt *trx_ctxt = disconn->trx_ctxt;
	struct psmx3_epaddr_context *epaddr_context;
	psm2_error_t errors;

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"psm2_ep: %p, epaddr: %p\n", trx_ctxt->psm2_ep, disconn->epaddr);

	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_remove_first_match(&trx_ctxt->peer_list,
				 psmx3_peer_match, disconn->epaddr);
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);
	if (trx_ctxt->ep && trx_ctxt->ep->av)
		psmx3_av_remove_conn(trx_ctxt->ep->av, trx_ctxt, disconn->epaddr);

	epaddr_context = psm3_epaddr_getctxt(disconn->epaddr);
	psm3_epaddr_setctxt(disconn->epaddr, NULL);
	free(epaddr_context);

	psm3_ep_disconnect2(trx_ctxt->psm2_ep, 1, &disconn->epaddr, NULL,
			    &errors, PSM2_EP_DISCONNECT_FORCE, 0);

	free(args);
	return NULL;
}

int psmx3_am_trx_ctxt_handler(psm2_am_token_t token, psm2_amarg_t *args,
			      int nargs, void *src, uint32_t len, void *hctx)
{
	psm2_epaddr_t epaddr;
	int err = 0, err2 = 0;
	int cmd;
	struct disconnect_args *disconn;
	pthread_t disconnect_thread;
	struct psmx3_trx_ctxt *trx_ctxt;
	trx_ctxt = (struct psmx3_trx_ctxt *)hctx;

	psm3_am_get_source(token, &epaddr);
	cmd = PSMX3_AM_GET_OP(args[0].u32w0);

	switch(cmd) {
	case PSMX3_AM_REQ_TRX_CTXT_DISCONNECT:
		/*
		 * we can't call psm3_ep_disconnect from the AM
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
			err2 = pthread_create(&disconnect_thread, NULL,
			                      disconnect_func, disconn);
			if (err2)
				PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				        "cannot create disconnect thread\n");
			else
				pthread_detach(disconnect_thread);
		}
		break;

	default:
		err = -FI_EINVAL;
		break;
	}

	return err;
}

void psmx3_trx_ctxt_disconnect_peers(struct psmx3_trx_ctxt *trx_ctxt)
{
	struct dlist_entry *item, *tmp;
	struct psmx3_epaddr_context *peer;
	struct dlist_entry peer_list;
	psm2_amarg_t arg;
	int err;

	arg.u32w0 = PSMX3_AM_REQ_TRX_CTXT_DISCONNECT;

	/* use local peer_list to avoid entering AM handler while holding the lock */
	dlist_init(&peer_list);
	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_foreach_safe(&trx_ctxt->peer_list, item, tmp) {
		dlist_remove(item);
		dlist_insert_before(item, &peer_list);
	}
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);

	dlist_foreach_safe(&peer_list, item, tmp) {
		peer = container_of(item, struct psmx3_epaddr_context, entry);
		if (psmx3_env.disconnect) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "epaddr: %p\n", peer->epaddr);
			err = psm3_am_request_short(peer->epaddr,
						    PSMX3_AM_TRX_CTXT_HANDLER,
						    &arg, 1, NULL, 0, 0, NULL,
						    NULL);
			if (err)
				PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
					"failed to send disconnect, err %d\n",
					err);
		}
		psm3_epaddr_setctxt(peer->epaddr, NULL);
		free(peer);
	}
}

static const char *psmx3_usage_flags_to_string(int usage_flags)
{
	switch (usage_flags & PSMX3_TX_RX) {
	case PSMX3_TX: return "tx";
	case PSMX3_RX: return "rx";
	default: return "tx+rx";
	}
}

void psmx3_trx_ctxt_free(struct psmx3_trx_ctxt *trx_ctxt, int usage_flags)
{
	int err;
	int old_flags;

	if (!trx_ctxt)
		return;

	old_flags = trx_ctxt->usage_flags;
	trx_ctxt->usage_flags &= ~usage_flags;
	if (trx_ctxt->usage_flags) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "epid: %s (%s -> %s)\n",
			psm3_epid_fmt(trx_ctxt->psm2_epid, 0),
			psmx3_usage_flags_to_string(old_flags),
			psmx3_usage_flags_to_string(trx_ctxt->usage_flags));
		return;
	}

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "epid: %s (%s)\n",
		psm3_epid_fmt(trx_ctxt->psm2_epid, 0), psmx3_usage_flags_to_string(old_flags));

	trx_ctxt->am_progress = 0;
	trx_ctxt->poll_active = 0;

	trx_ctxt->domain->trx_ctxt_lock_fn(&trx_ctxt->domain->trx_ctxt_lock, 1);
	dlist_remove(&trx_ctxt->entry);
	trx_ctxt->domain->trx_ctxt_unlock_fn(&trx_ctxt->domain->trx_ctxt_lock, 1);

	psmx3_trx_ctxt_disconnect_peers(trx_ctxt);

	if (trx_ctxt->am_initialized)
		psmx3_am_fini(trx_ctxt);

#if 0
	/* AM messages could arrive after MQ is finalized, causing segfault
	 * when trying to dereference the MQ pointer. There is no mechanism
	 * to properly shutdown AM. The workaround is to keep MQ valid.
	 */
	psm3_mq_finalize(trx_ctxt->psm2_mq);
#endif

	/* workaround for:
	 * Assertion failure at psm2_ep.c:1059: ep->mctxt_master == ep
	 */
	if (psmx3_env.delay)
		sleep(psmx3_env.delay);

	if (psmx3_env.timeout)
		err = psm3_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_GRACEFUL,
				    (int64_t) psmx3_env.timeout * 1000000000LL);
	else
		err = PSM2_EP_CLOSE_TIMEOUT;

	if (err != PSM2_OK)
		psm3_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_FORCE, 0);

	ofi_bufpool_destroy(trx_ctxt->am_req_pool);
	ofi_spin_destroy(&trx_ctxt->am_req_pool_lock);
	ofi_spin_destroy(&trx_ctxt->poll_lock);
	ofi_spin_destroy(&trx_ctxt->peer_lock);

	if (!ofi_atomic_dec32(&trx_ctxt->poll_refcnt))
		free(trx_ctxt);
}

struct psmx3_trx_ctxt *psmx3_trx_ctxt_alloc(struct psmx3_fid_domain *domain,
					    struct psmx3_ep_name *src_addr,
					    int sep_ctxt_idx,
					    int usage_flags,
					    uint8_t *uuid)
{
	struct psmx3_trx_ctxt *trx_ctxt;
	struct psm3_ep_open_opts opts;
	int err;
	struct dlist_entry *item;
	int asked_flags = usage_flags & PSMX3_TX_RX;
	int compatible_flags = ~asked_flags & PSMX3_TX_RX;

	if (!uuid)
		uuid = domain->fabric->uuid;

	/* Check existing allocations first if only Tx or Rx is needed */
	if (compatible_flags) {
		domain->trx_ctxt_lock_fn(&domain->trx_ctxt_lock, 1);
		dlist_foreach(&domain->trx_ctxt_list, item) {
			trx_ctxt = container_of(item, struct psmx3_trx_ctxt, entry);
			if (compatible_flags == trx_ctxt->usage_flags &&
			    !memcmp(uuid, trx_ctxt->uuid, sizeof(psm2_uuid_t))) {
				trx_ctxt->usage_flags |= asked_flags;
				domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);
				PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
					"use existing context. epid: %s "
					"(%s -> tx+rx).\n", psm3_epid_fmt(trx_ctxt->psm2_epid, 0),
					psmx3_usage_flags_to_string(compatible_flags));
				return trx_ctxt;
			}
		}
		domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);
	}

	if (psmx3_trx_ctxt_cnt >= psmx3_domain_info.max_trx_ctxt) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"number of Tx/Rx contexts exceeds limit (%d).\n",
			psmx3_domain_info.max_trx_ctxt);
		return NULL;
	}

	trx_ctxt = calloc(1, sizeof(*trx_ctxt));
	if (!trx_ctxt) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"failed to allocate trx_ctxt.\n");
		return NULL;
	}

	err = ofi_bufpool_create(&trx_ctxt->am_req_pool,
				 sizeof(struct psmx3_am_request),
				 sizeof(void *), 0, 64, 0);
	if (err) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"failed to allocate am_req_pool.\n");
		goto err_out;
	}

	psm3_ep_open_opts_get_defaults(&opts);
	memcpy(trx_ctxt->uuid, uuid, sizeof(psm2_uuid_t));
	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"uuid: %s\n", psmx3_uuid_to_string(uuid));

	if (src_addr && src_addr->unit >= 0) {
		opts.unit = psmx3_domain_info.unit_id[src_addr->unit];
		opts.addr_index = psmx3_domain_info.addr_index[src_addr->unit];
	} else if (! psmx3_domain_info.default_domain_name[0]) {
		opts.unit = psmx3_domain_info.unit_id[0];
		opts.addr_index = psmx3_domain_info.addr_index[0];
	} else {
		opts.unit = PSMX3_DEFAULT_UNIT;
		opts.addr_index = PSMX3_DEFAULT_ADDR_INDEX;
	}
	opts.port = src_addr ? src_addr->port : PSMX3_DEFAULT_PORT;
	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"ep_open_opts: unit=%d port=%u addr_index=%d\n", opts.unit, opts.port, opts.addr_index);

	err = psm3_ep_open(uuid, &opts,
			   &trx_ctxt->psm2_ep, &trx_ctxt->psm2_epid);
	if (err != PSM2_OK) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"psm3_ep_open returns %d, errno=%d\n", err, errno);
		goto err_out_destroy_pool;
	}

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"epid: %s (%s)\n", psm3_epid_fmt(trx_ctxt->psm2_epid, 0),
		psmx3_usage_flags_to_string(usage_flags));

	err = psm3_mq_init(trx_ctxt->psm2_ep, PSM2_MQ_ORDERMASK_ALL,
			   NULL, 0, &trx_ctxt->psm2_mq);
	if (err != PSM2_OK) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"psm3_mq_init returns %d, errno=%d\n", err, errno);
		goto err_out_close_ep;
	}

	ofi_spin_init(&trx_ctxt->peer_lock);
	ofi_spin_init(&trx_ctxt->poll_lock);
	ofi_spin_init(&trx_ctxt->am_req_pool_lock);
	ofi_spin_init(&trx_ctxt->trigger_queue.lock);
	dlist_init(&trx_ctxt->peer_list);
	slist_init(&trx_ctxt->trigger_queue.list);
	trx_ctxt->id = psmx3_trx_ctxt_cnt++;
	trx_ctxt->domain = domain;
	trx_ctxt->usage_flags = asked_flags;
	trx_ctxt->poll_active = 1;
	ofi_atomic_initialize32(&trx_ctxt->poll_refcnt, 1); /* take one ref for domain->trx_ctxt_list */

	domain->trx_ctxt_lock_fn(&domain->trx_ctxt_lock, 1);
	dlist_insert_before(&trx_ctxt->entry, &domain->trx_ctxt_list);
	domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);

	return trx_ctxt;

err_out_close_ep:
	if (psm3_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_GRACEFUL,
			  (int64_t) psmx3_env.timeout * 1000000000LL) != PSM2_OK)
		psm3_ep_close(trx_ctxt->psm2_ep, PSM2_EP_CLOSE_FORCE, 0);

err_out_destroy_pool:
	ofi_bufpool_destroy(trx_ctxt->am_req_pool);

err_out:
	free(trx_ctxt);
	return NULL;
}

