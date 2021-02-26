/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include "psmx.h"

struct psm_am_parameters psmx_am_param;

static psm_am_handler_fn_t psmx_am_handlers[3] = {
	psmx_am_rma_handler,
	psmx_am_msg_handler,
	psmx_am_atomic_handler,
};

static int psmx_am_handlers_idx[3];
static int psmx_am_handlers_initialized = 0;

/* The AM handler signature is different between PSM1 and PSM2. The compat
 * handlers are used when compiled with PSM1 headers and run over the
 * psm2-compat library.
 */

int psmx_am_compat_mode = 0;
static int (*psmx_am_get_source)(psm_am_token_t token, psm_epaddr_t *epaddr);

static int psmx_am_compat_rma_handler(psm_am_token_t token,
				      psm_amarg_t *args, int nargs,
				      void *src, uint32_t len)
{
	psm_epaddr_t epaddr;
	(*psmx_am_get_source)(token, &epaddr);
	return psmx_am_rma_handler(token, epaddr, args, nargs, src, len);
}

static int psmx_am_compat_msg_handler(psm_am_token_t token,
				      psm_amarg_t *args, int nargs,
				      void *src, uint32_t len)
{
	psm_epaddr_t epaddr;
	(*psmx_am_get_source)(token, &epaddr);
	return psmx_am_msg_handler(token, epaddr, args, nargs, src, len);
}

static int psmx_am_compat_atomic_handler(psm_am_token_t token,
					 psm_amarg_t *args, int nargs,
					 void *src, uint32_t len)
{
	psm_epaddr_t epaddr;
	(*psmx_am_get_source)(token, &epaddr);
	return psmx_am_atomic_handler(token, epaddr, args, nargs, src, len);
}

int psmx_am_progress(struct psmx_fid_domain *domain)
{
	struct slist_entry *item;
	struct psmx_am_request *req;
	struct psmx_trigger *trigger;

	if (psmx_env.am_msg) {
		fastlock_acquire(&domain->send_queue.lock);
		while (!slist_empty(&domain->send_queue.list)) {
			item = slist_remove_head(&domain->send_queue.list);
			req = container_of(item, struct psmx_am_request, list_entry);
			fastlock_release(&domain->send_queue.lock);
			psmx_am_process_send(domain, req);
			fastlock_acquire(&domain->send_queue.lock);
		}
		fastlock_release(&domain->send_queue.lock);
	}

	if (psmx_env.tagged_rma) {
		fastlock_acquire(&domain->rma_queue.lock);
		while (!slist_empty(&domain->rma_queue.list)) {
			item = slist_remove_head(&domain->rma_queue.list);
			req = container_of(item, struct psmx_am_request, list_entry);
			fastlock_release(&domain->rma_queue.lock);
			psmx_am_process_rma(domain, req);
			fastlock_acquire(&domain->rma_queue.lock);
		}
		fastlock_release(&domain->rma_queue.lock);
	}

	fastlock_acquire(&domain->trigger_queue.lock);
	while (!slist_empty(&domain->trigger_queue.list)) {
		item = slist_remove_head(&domain->trigger_queue.list);
		trigger = container_of(item, struct psmx_trigger, list_entry);
		fastlock_release(&domain->trigger_queue.lock);
		psmx_process_trigger(domain, trigger);
		fastlock_acquire(&domain->trigger_queue.lock);
	}
	fastlock_release(&domain->trigger_queue.lock);

	return 0;
}

int psmx_am_init(struct psmx_fid_domain *domain)
{
	psm_ep_t psm_ep = domain->psm_ep;
	size_t size;
	int err = 0;

	FI_INFO(&psmx_prov, FI_LOG_CORE, "\n");

	psmx_atomic_init();

	if (!psmx_am_handlers_initialized) {
		err = psm_am_get_parameters(psm_ep, &psmx_am_param,
						sizeof(psmx_am_param), &size);
		if (err)
			return psmx_errno(err);

		if (psmx_am_compat_mode) {
			void *dlsym(void*, const char *);
			psmx_am_get_source = dlsym(NULL, "psm2_am_get_source");
			if (!psmx_am_get_source) {
				FI_WARN(&psmx_prov, FI_LOG_CORE,
					"failed to load function psm2_am_get_source\n");
				return -FI_EOTHER;
			}

			psmx_am_handlers[0] = (void *)psmx_am_compat_rma_handler;
			psmx_am_handlers[1] = (void *)psmx_am_compat_msg_handler;
			psmx_am_handlers[2] = (void *)psmx_am_compat_atomic_handler;
		}

		err = psm_am_register_handlers(psm_ep, psmx_am_handlers, 3,
						psmx_am_handlers_idx);
		if (err)
			return psmx_errno(err);

		if ((psmx_am_handlers_idx[0] != PSMX_AM_RMA_HANDLER) ||
		    (psmx_am_handlers_idx[1] != PSMX_AM_MSG_HANDLER) ||
		    (psmx_am_handlers_idx[2] != PSMX_AM_ATOMIC_HANDLER)) {
			FI_WARN(&psmx_prov, FI_LOG_CORE,
				"failed to register one or more AM handlers "
				"at indecies %d, %d, %d\n", PSMX_AM_RMA_HANDLER,
				PSMX_AM_MSG_HANDLER, PSMX_AM_ATOMIC_HANDLER);
			return -FI_EBUSY;
		}

		psmx_am_handlers_initialized = 1;
	}

	slist_init(&domain->rma_queue.list);
	slist_init(&domain->recv_queue.list);
	slist_init(&domain->unexp_queue.list);
	slist_init(&domain->trigger_queue.list);
	slist_init(&domain->send_queue.list);
	fastlock_init(&domain->rma_queue.lock);
	fastlock_init(&domain->recv_queue.lock);
	fastlock_init(&domain->unexp_queue.lock);
	fastlock_init(&domain->trigger_queue.lock);
	fastlock_init(&domain->send_queue.lock);

	return err;
}

int psmx_am_fini(struct psmx_fid_domain *domain)
{
	fastlock_destroy(&domain->rma_queue.lock);
	fastlock_destroy(&domain->recv_queue.lock);
	fastlock_destroy(&domain->unexp_queue.lock);
	fastlock_destroy(&domain->trigger_queue.lock);
	fastlock_destroy(&domain->send_queue.lock);

	psmx_atomic_fini();

	return 0;
}

