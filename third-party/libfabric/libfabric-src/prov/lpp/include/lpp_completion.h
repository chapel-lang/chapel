/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#ifndef _LPP_COMPLETION_H_
#define _LPP_COMPLETION_H_

#include "lpp.h"

/* Determine if the operation will generate a completion on success. The entry
 * will be flagged with FI_COMPLETION.
 *
 * op_flags may be either the explicit operation flags (as given in a call like
 * fi_sendmsg()) or implicit op_flags from the bound context (as in fi_send()
 * or fi_sendv(), which do not take "flags" arguments). */
static inline uint64_t lpp_cq_comp_flag(uint64_t bind_flags, uint64_t op_flags)
{
	if ((bind_flags & FI_SELECTIVE_COMPLETION))
		return op_flags & FI_COMPLETION;

	return FI_COMPLETION;
}

// Prototypes.
void lpp_completion_transmit(struct lpp_ep *lpp_epp, struct lpp_cntr *cntrp,
			     uint64_t flags, uint64_t data, void *context, int status);
void lpp_completion_recv(struct lpp_ep *lpp_epp, uint64_t flags, uint64_t data, void *context,
			 uint64_t tag, void *buf, size_t len, size_t olen,
			 fi_addr_t src_addr, int status);
void lpp_completion_multirecv(struct lpp_ep *lpp_epp, void *context);

#endif // _LPP_COMPLETION_H_
