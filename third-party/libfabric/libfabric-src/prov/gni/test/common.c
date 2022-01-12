/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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

#include "common.h"

void calculate_time_difference(struct timeval *start, struct timeval *end,
		int *secs_out, int *usec_out)
{
	*secs_out = end->tv_sec - start->tv_sec;
	if (end->tv_usec < start->tv_usec) {
		*secs_out = *secs_out - 1;
		*usec_out = (1000000 + end->tv_usec) - start->tv_usec;
	} else {
		*usec_out = end->tv_usec - start->tv_usec;
	}
}

int dump_cq_error(struct fid_cq *cq, void *context, uint64_t flags)
{
	int ret;
	struct fi_cq_err_entry err_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX,
					   UINT_MAX, INT_MAX, INT_MAX,
					   (void *) -1 };

	ret = fi_cq_readerr(cq, &err_cqe, flags);

	if (ret > 0) {
		if (context && ((uint64_t)err_cqe.op_context !=
				(uint64_t)context)) {
			fprintf(stderr, "Bad err context: ctx %p err ctx %p\n",
				context, err_cqe.op_context);
		}

		fprintf(stderr, "err flags 0x%lx\n", err_cqe.flags);
		fprintf(stderr, "err len   %ld\n", err_cqe.len);
		fprintf(stderr, "err data  0x%lx\n", err_cqe.data);
		fprintf(stderr, "err tag   0x%lx\n", err_cqe.tag);
		fprintf(stderr, "err olen  %ld\n", err_cqe.olen);
		fprintf(stderr, "err err   %d\n", err_cqe.err);
		fprintf(stderr, "err prov_errno %d\n", err_cqe.prov_errno);
	}

	return 0;
}
