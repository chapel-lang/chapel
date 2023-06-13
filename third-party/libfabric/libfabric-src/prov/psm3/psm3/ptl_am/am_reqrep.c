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
#include "psm_mq_internal.h"
#include "psm_am_internal.h"

psm2_error_t
psm3_amsh_am_short_request(psm2_epaddr_t epaddr,
			   psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			   void *src, size_t len, int flags,
			   psm2_am_completion_fn_t completion_fn,
			   void *completion_ctxt)
{
	psm2_amarg_t req_args[NSHORT_ARGS + NBULK_ARGS];

	/* All sends are synchronous. Ignore PSM2_AM_FLAG_ASYNC.
	 * Treat PSM2_AM_FLAG_NOREPLY as "advisory". This was mainly
	 * used to optimize the IPS path though we could put a stricter interpretation
	 * on it to disallow any replies.
	 */

	/* For now less than NSHORT_ARGS+NBULK_ARGS-1. We use the first arg to carry
	 * the handler index.
	 */
	psmi_assert(nargs <= (NSHORT_ARGS + NBULK_ARGS - 1));
	psmi_assert(epaddr->ptlctl->ptl != NULL);

	req_args[0].u32w0 = (uint32_t) handler;
	psm3_mq_mtucpy((void *)&req_args[1], (const void *)args,
		       (nargs * sizeof(psm2_amarg_t)));
	psm3_amsh_short_request(epaddr->ptlctl->ptl, epaddr, am_handler_hidx,
				req_args, nargs + 1, src, len, 0);

	if (completion_fn)
		completion_fn(completion_ctxt);

	return PSM2_OK;
}

psm2_error_t
psm3_amsh_am_short_reply(psm2_am_token_t tok,
			 psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			 void *src, size_t len, int flags,
			 psm2_am_completion_fn_t completion_fn,
			 void *completion_ctxt)
{
	psm2_amarg_t rep_args[NSHORT_ARGS + NBULK_ARGS];

	/* For now less than NSHORT_ARGS+NBULK_ARGS-1. We use the first arg to carry
	 * the handler index.
	 */
	psmi_assert(nargs <= (NSHORT_ARGS + NBULK_ARGS - 1));
	rep_args[0].u32w0 = (uint32_t) handler;
	psm3_mq_mtucpy((void *)&rep_args[1], (const void *)args,
		       (nargs * sizeof(psm2_amarg_t)));

	psm3_amsh_short_reply((amsh_am_token_t *) tok, am_handler_hidx,
			      rep_args, nargs + 1, src, len, 0);

	if (completion_fn)
		completion_fn(completion_ctxt);

	return PSM2_OK;
}
