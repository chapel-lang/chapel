/*
 * Copyright (c) Intel Corporation. All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include "sm2.h"

int sm2_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	struct util_cq *cq;
	int ret;

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
		attr->wait_obj = FI_WAIT_YIELD;
		/* fall through */
	case FI_WAIT_NONE:
	case FI_WAIT_YIELD:
		break;
	default:
		FI_INFO(&sm2_prov, FI_LOG_CQ, "CQ wait not yet supported\n");
		return -FI_ENOSYS;
	}

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	ret = ofi_cq_init(&sm2_prov, domain, attr, cq, &ofi_cq_progress,
			  context);
	if (ret)
		goto free;

	(*cq_fid) = &cq->cq_fid;
	return 0;

free:
	free(cq);
	return ret;
}
