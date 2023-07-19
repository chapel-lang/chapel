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

#ifndef _FI_PSM_VERSION_H_
#define _FI_PSM_VERSION_H_

#if HAVE_PSM2_SRC
#include "psm2/psm2.h"
#include "psm2/psm2_mq.h"
#include "psm2/psm2_am.h"
#ifdef VALGRIND_MAKE_MEM_DEFINED
#undef VALGRIND_MAKE_MEM_DEFINED
#endif
#else
#include <psm2.h>
#include <psm2_mq.h>
#include <psm2_am.h>
#endif

#define PSMX2_PROV_NAME		"psm2"
#define PSMX2_DOMAIN_NAME	"hfi1"
#define PSMX2_FABRIC_NAME	"psm2"

#define PSMX2_DEFAULT_UUID	"00FF00FF-0000-0000-0000-00FF00FF00FF"
#define PROVIDER_INI		PSM2_INI

#if HAVE_PSM2_MQ_REQ_USER

#ifndef PSMX2_USE_REQ_CONTEXT
#define PSMX2_USE_REQ_CONTEXT	1
#endif

#define PSMX2_STATUS_TYPE	struct psm2_mq_req_user
#define PSMX2_STATUS_ERROR(s)	((s)->error_code)
#define PSMX2_STATUS_TAG(s)	((s)->tag)
#define PSMX2_STATUS_RCVLEN(s)	((s)->recv_msglen)
#define PSMX2_STATUS_SNDLEN(s)	((s)->send_msglen)
#define PSMX2_STATUS_PEER(s)	((s)->peer)
#define PSMX2_STATUS_CONTEXT(s)	((s)->context)

#else /* !HAVE_PSM2_MQ_REQ_USER */

#ifdef PSMX2_USE_REQ_CONTEXT
#undef PSMX2_USE_REQ_CONTEXT
#endif
#define PSMX2_USE_REQ_CONTEXT	0

#define PSMX2_STATUS_TYPE	psm2_mq_status2_t
#define PSMX2_STATUS_ERROR(s)	((s)->error_code)
#define PSMX2_STATUS_TAG(s)	((s)->msg_tag)
#define PSMX2_STATUS_RCVLEN(s)	((s)->nbytes)
#define PSMX2_STATUS_SNDLEN(s)	((s)->msg_length)
#define PSMX2_STATUS_PEER(s)	((s)->msg_peer)
#define PSMX2_STATUS_CONTEXT(s)	((s)->context)

#endif /* !HAVE_PSM2_MQ_REQ_USER */

/*
 * Provide backward compatibility for older PSM2 libraries that lack the
 * psm2_am_register_handlers_2 function.
 */

#if !HAVE_PSM2_AM_REGISTER_HANDLERS_2

#define PSMX2_MAX_AM_HANDLERS 1000

typedef int (*psm2_am_handler_2_fn_t) (
			psm2_am_token_t token,
			psm2_amarg_t *args, int nargs,
			void *src, uint32_t len, void *hctx);

extern psm2_am_handler_fn_t	psmx2_am_handlers[];
extern psm2_am_handler_2_fn_t	psmx2_am_handlers_2[];
extern void			*psmx2_am_handler_ctxts[];
extern int			psmx2_am_handler_count;

static inline
psm2_error_t psm2_am_register_handlers_2(
			psm2_ep_t ep,
			const psm2_am_handler_2_fn_t * handlers,
			int num_handlers, void **hctx,
			int *handlers_idx)
{
	int i;
	int start = psmx2_am_handler_count;

	if (start + num_handlers > PSMX2_MAX_AM_HANDLERS)
		return PSM2_EP_NO_RESOURCES;

	psmx2_am_handler_count += num_handlers;

	for (i = 0; i < num_handlers; i++) {
		psmx2_am_handlers_2[start + i] = handlers[i];
		psmx2_am_handler_ctxts[start + i] = hctx[i];
	}

	return psm2_am_register_handlers(ep, psmx2_am_handlers + start,
					 num_handlers, handlers_idx);
}

#endif /* !HAVE_PSM2_AM_REGISTER_HANDLERS_2 */

/*
 * Use reserved space within psm2_mq_req_user for fi_context instead of
 * allocating from a internal queue.
 *
 * Only work with PSM2 that has psm2_mq_req_user defined. Can be turned off by
 * passing "-DPSMX2_USE_REQ_CONTEXT=0" to the compiler.
 */

#if PSMX2_USE_REQ_CONTEXT

#define PSMX2_EP_DECL_OP_CONTEXT

#define PSMX2_EP_INIT_OP_CONTEXT(ep) \
	do { \
		FI_INFO(&psmx2_prov, FI_LOG_EP_CTRL, \
			"skip initialization of op context list.\n"); \
	} while (0)

#define PSMX2_EP_FINI_OP_CONTEXT(ep)

#define PSMX2_EP_GET_OP_CONTEXT(ep, ctx) \
	do { \
		(ctx) = NULL; \
	} while (0)

#define PSMX2_EP_PUT_OP_CONTEXT(ep, ctx)

#define PSMX2_REQ_GET_OP_CONTEXT(req, ctx) \
	do { \
		struct psm2_mq_req_user *req_user = (void *)(req); \
		(ctx) = req_user->context = req_user->user_reserved; \
	} while (0)

#else /* !PSMX2_USE_REQ_CONTEXT */

struct psmx2_context {
        struct fi_context	fi_context;
        struct slist_entry	list_entry;
};

#define PSMX2_EP_DECL_OP_CONTEXT \
	struct slist	free_context_list; \
	ofi_spin_t	context_lock;

#define PSMX2_EP_INIT_OP_CONTEXT(ep) \
	do { \
		struct psmx2_context *item; \
		int i; \
		slist_init(&(ep)->free_context_list); \
		ofi_spin_init(&(ep)->context_lock); \
		for (i = 0; i < 64; i++) { \
			item = calloc(1, sizeof(*item)); \
			if (!item) { \
				FI_WARN(&psmx2_prov, FI_LOG_EP_CTRL, "out of memory.\n"); \
				break; \
			} \
			slist_insert_tail(&item->list_entry, &(ep)->free_context_list); \
		} \
	} while (0)

#define PSMX2_EP_FINI_OP_CONTEXT(ep) \
	do { \
		struct slist_entry *entry; \
		struct psmx2_context *item; \
		while (!slist_empty(&(ep)->free_context_list)) { \
			entry = slist_remove_head(&(ep)->free_context_list); \
			item = container_of(entry, struct psmx2_context, list_entry); \
			free(item); \
		} \
		ofi_spin_destroy(&(ep)->context_lock); \
	} while (0)

#define PSMX2_EP_GET_OP_CONTEXT(ep, ctx) \
	do { \
		struct psmx2_context *context; \
		ep->domain->context_lock_fn(&(ep)->context_lock, 2); \
		if (!slist_empty(&(ep)->free_context_list)) { \
			context = container_of(slist_remove_head(&(ep)->free_context_list), \
					       struct psmx2_context, list_entry); \
			ep->domain->context_unlock_fn(&(ep)->context_lock, 2); \
			(ctx) = &context->fi_context; \
			break; \
		} \
		ep->domain->context_unlock_fn(&(ep)->context_lock, 2); \
		context = malloc(sizeof(*context)); \
		if (!context) { \
			FI_WARN(&psmx2_prov, FI_LOG_EP_DATA, "out of memory.\n"); \
			return -FI_ENOMEM; \
		} \
		(ctx) = &context->fi_context; \
	} while (0)

#define PSMX2_EP_PUT_OP_CONTEXT(ep, ctx) \
	do { \
		struct psmx2_context *context; \
		context = container_of((ctx), struct psmx2_context, fi_context); \
		context->list_entry.next = NULL; \
		ep->domain->context_lock_fn(&(ep)->context_lock, 2); \
		slist_insert_tail(&context->list_entry, &(ep)->free_context_list); \
		ep->domain->context_unlock_fn(&(ep)->context_lock, 2); \
	} while (0)

#endif /* !PSMX2_USE_REQ_CONTEXT */

#endif

