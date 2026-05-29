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

#include "lpp.h"

int lpp_ctx_create(struct lpp_domain *lpp_domainp, struct lpp_ctx *lpp_ctxp, int type, size_t num_entries)
{
	if ((lpp_domainp == NULL) || (lpp_ctxp == NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "NULL parameters (domain: %p ctxp: %p)\n",
			lpp_domainp, lpp_ctxp);
		return -FI_ENOENT;
	}

	if ((type != KLPP_CTX_RX) && (type != KLPP_CTX_STX)) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "invalid ctx type %d\n", type);
		return -FI_ENOENT;
	}

	// Fill in the given parameters.
	memset((void *)lpp_ctxp, 0, sizeof(struct lpp_ctx));

	lpp_ctxp->domain = lpp_domainp;
	lpp_ctxp->type = type;
	lpp_ctxp->num_entries = num_entries;

	// Tell KLPP so we can get a id.
	if (klpp_ctx_create(lpp_ctxp) != 0) {
		return -FI_ENOENT;
	}

	ofi_atomic_initialize32(&lpp_ctxp->ref_count, 0);
	return 0;
}

int lpp_ctx_close(struct lpp_ctx *lpp_ctxp)
{
	return (lpp_ctxp != NULL) ? klpp_ctx_close(lpp_ctxp) : -FI_ENOENT;
}
