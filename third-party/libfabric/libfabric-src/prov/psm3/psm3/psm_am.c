/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_am.h"
#include "psm_am_internal.h"
#include "psm_mq_internal.h"

int psm3_ep_device_is_enabled(const psm2_ep_t ep, int devid);

/* AM capabilities parameters are initialized once in psm3_am_init_internal
   and copied out in psm3_am_get_parameters.  When debugging is enabled,
   various assertions reference these parameters for sanity checking. */
struct psm2_am_parameters psm3_am_parameters = { 0 };

static int _ignore_handler(PSMI_AM_ARGS_DEFAULT)
{
	return 0;
}

int psm3_abort_handler(PSMI_AM_ARGS_DEFAULT)
{
	abort();
	return 0;
}

static void psm3_am_min_parameters(struct psm2_am_parameters *dest,
				   struct psm2_am_parameters *src)
{
	dest->max_handlers = min(dest->max_handlers, src->max_handlers);
	dest->max_nargs = min(dest->max_nargs, src->max_nargs);
	dest->max_request_short =
	    min(dest->max_request_short, src->max_request_short);
	dest->max_reply_short =
	    min(dest->max_reply_short, src->max_reply_short);
}

psm2_error_t psm3_am_init_internal(psm2_ep_t ep)
{
	int i;
	struct psm2_ep_am_handle_entry *am_htable;
	struct psm2_am_parameters params;

	psm3_am_parameters.max_handlers = INT_MAX;
	psm3_am_parameters.max_nargs = INT_MAX;
	psm3_am_parameters.max_request_short = INT_MAX;
	psm3_am_parameters.max_reply_short = INT_MAX;

	if (psm3_ep_device_is_enabled(ep, PTL_DEVID_SELF)) {
		ep->ptl_self.am_get_parameters(ep, &params);
		psm3_am_min_parameters(&psm3_am_parameters, &params);
	}

	if (psm3_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		ep->ptl_ips.am_get_parameters(ep, &params);
		psm3_am_min_parameters(&psm3_am_parameters, &params);
	}

	if (psm3_ep_device_is_enabled(ep, PTL_DEVID_AMSH)) {
		ep->ptl_amsh.am_get_parameters(ep, &params);
		psm3_am_min_parameters(&psm3_am_parameters, &params);
	}

	ep->am_htable =
	    psmi_malloc(ep, UNDEFINED,
			sizeof(struct psm2_ep_am_handle_entry) * PSMI_AM_NUM_HANDLERS);
	if (ep->am_htable == NULL)
		return PSM2_NO_MEMORY;

	am_htable = (struct psm2_ep_am_handle_entry *) ep->am_htable;
	for (i = 0; i < PSMI_AM_NUM_HANDLERS; i++) {
		am_htable[i].hfn = _ignore_handler;
		am_htable[i].hctx = NULL;
		am_htable[i].version = PSM2_AM_HANDLER_V2;
	}

	return PSM2_OK;

}

void psm3_am_fini_internal(psm2_ep_t ep)
{
	if(ep->am_htable != NULL) {
		psmi_free(ep->am_htable);
	}
}

psm2_error_t
psm3_am_register_handlers(psm2_ep_t ep,
			   const psm2_am_handler_fn_t *handlers,
			   int num_handlers, int *handlers_idx)
{
	int i, j;

	psmi_assert_always(ep->am_htable != NULL);

	PSM2_LOG_MSG("entering");
	/* For now just assign any free one */
	for (i = 0, j = 0; (i < PSMI_AM_NUM_HANDLERS) && (j < num_handlers); i++) {
		if (ep->am_htable[i].hfn == _ignore_handler) {
			ep->am_htable[i].hfn = handlers[j];
			ep->am_htable[i].hctx = NULL;
			ep->am_htable[i].version = PSM2_AM_HANDLER_V1;
			handlers_idx[j] = i;
			if (++j == num_handlers)	/* all registered */
				break;
		}
	}

	if (j < num_handlers) {
		/* Not enough free handlers, restore unused handlers */
		for (i = 0; i < j; i++) {
			ep->am_htable[handlers_idx[i]].hfn = _ignore_handler;
			ep->am_htable[handlers_idx[i]].hctx = NULL;
			ep->am_htable[handlers_idx[i]].version = PSM2_AM_HANDLER_V2;
		}
		PSM2_LOG_MSG("leaving");
		return psm3_handle_error(ep, PSM2_EP_NO_RESOURCES,
					 "Insufficient "
					 "available AM handlers: registered %d of %d requested handlers",
					 j, num_handlers);
	}
	else {
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}
}

psm2_error_t
psm3_am_register_handlers_2(psm2_ep_t ep,
			   const psm2_am_handler_2_fn_t *handlers,
			   int num_handlers, void **hctx, int *handlers_idx)
{
	int i, j;

	psmi_assert_always(ep->am_htable != NULL);

	PSM2_LOG_MSG("entering");
	/* For now just assign any free one */
	for (i = 0, j = 0; (i < PSMI_AM_NUM_HANDLERS) && (j < num_handlers); i++) {
		if (ep->am_htable[i].hfn == _ignore_handler) {
			ep->am_htable[i].hfn = handlers[j];
			ep->am_htable[i].hctx = hctx[j];
			ep->am_htable[i].version = PSM2_AM_HANDLER_V2;
			handlers_idx[j] = i;
			if (++j == num_handlers)	/* all registered */
				break;
		}
	}

	if (j < num_handlers) {
		/* Not enough free handlers, restore unused handlers */
		for (i = 0; i < j; i++) {
			ep->am_htable[handlers_idx[i]].hfn = _ignore_handler;
			ep->am_htable[handlers_idx[i]].hctx = NULL;
			ep->am_htable[handlers_idx[i]].version = PSM2_AM_HANDLER_V2;
		}
		PSM2_LOG_MSG("leaving");
		return psm3_handle_error(ep, PSM2_EP_NO_RESOURCES,
					 "Insufficient "
					 "available AM handlers: registered %d of %d requested handlers",
					 j, num_handlers);
	}
	else {
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}
}

void
psm3_am_unregister_handlers(psm2_ep_t ep)
{
	int i;

	PSM2_LOG_MSG("entering");
	for (i = 0; i < PSMI_AM_NUM_HANDLERS; i++) {
		if (ep->am_htable[i].hfn != _ignore_handler) {
			ep->am_htable[i].hfn = _ignore_handler;
			ep->am_htable[i].hctx = NULL;
			ep->am_htable[i].version = PSM2_AM_HANDLER_V2;
		}
	}
	PSM2_LOG_MSG("leaving");
}

psm2_error_t
psm3_am_request_short(psm2_epaddr_t epaddr, psm2_handler_t handler,
		       psm2_amarg_t *args, int nargs, void *src, size_t len,
		       int flags, psm2_am_completion_fn_t completion_fn,
		       void *completion_ctxt)
{
	psm2_error_t err;
	ptl_ctl_t *ptlc = epaddr->ptlctl;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();
	psmi_assert(epaddr != NULL);
	psmi_assert(handler < psm3_am_parameters.max_handlers);
	psmi_assert(nargs >= 0 && nargs <= psm3_am_parameters.max_nargs);
	psmi_assert(nargs > 0 ? args != NULL : 1);
	psmi_assert(len <= psm3_am_parameters.max_request_short);
	psmi_assert(len > 0 ? src != NULL : 1);

	PSMI_LOCK(ptlc->ep->mq->progress_lock);

	err = ptlc->am_short_request(epaddr, handler, args,
				     nargs, src, len, flags, completion_fn,
				     completion_ctxt);
	PSMI_UNLOCK(ptlc->ep->mq->progress_lock);
	PSM2_LOG_MSG("leaving");

	return err;
}

psm2_error_t
psm3_am_reply_short(psm2_am_token_t token, psm2_handler_t handler,
		     psm2_amarg_t *args, int nargs, void *src, size_t len,
		     int flags, psm2_am_completion_fn_t completion_fn,
		     void *completion_ctxt)
{
	psm2_error_t err;
	struct psmi_am_token *tok;
	psm2_epaddr_t epaddr;
	ptl_ctl_t *ptlc;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();
	psmi_assert_always(token != NULL);
	psmi_assert(handler < psm3_am_parameters.max_handlers);
	psmi_assert(nargs >= 0 && nargs <= psm3_am_parameters.max_nargs);
	psmi_assert(nargs > 0 ? args != NULL : 1);
	psmi_assert(len <= psm3_am_parameters.max_reply_short);
	psmi_assert(len > 0 ? src != NULL : 1);

	tok = (struct psmi_am_token *)token;
	epaddr = tok->epaddr_incoming;
	ptlc = epaddr->ptlctl;

	/* No locking here since we are already within handler context and already
	 * locked */

	err = ptlc->am_short_reply(token, handler, args,
				   nargs, src, len, flags, completion_fn,
				   completion_ctxt);
	PSM2_LOG_MSG("leaving");

	return err;
}

psm2_error_t psm3_am_get_source(psm2_am_token_t token, psm2_epaddr_t *epaddr_out)
{
	struct psmi_am_token *tok;

	PSM2_LOG_MSG("entering");
	if (token == NULL || epaddr_out == NULL) {
		PSM2_LOG_MSG("leaving");
		return psm3_handle_error(NULL, PSM2_PARAM_ERR,
					 "Invalid %s parameters", __FUNCTION__);
	}

	tok = (struct psmi_am_token *)token;
	*epaddr_out = tok->epaddr_incoming;
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

psm2_error_t
psm3_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters,
			size_t sizeof_parameters_in,
			size_t *sizeof_parameters_out)
{
	size_t s;

	PSM2_LOG_MSG("entering");
	if (parameters == NULL) {
		PSM2_LOG_MSG("leaving");
		return psm3_handle_error(NULL, PSM2_PARAM_ERR,
					 "Invalid %s parameters", __FUNCTION__);
	}

	memset(parameters, 0, sizeof_parameters_in);
	s = min(sizeof(psm3_am_parameters), sizeof_parameters_in);
	memcpy(parameters, &psm3_am_parameters, s);
	*sizeof_parameters_out = s;
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}
