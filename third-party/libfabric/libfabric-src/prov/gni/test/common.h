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

#ifndef PROV_GNI_TEST_COMMON_H_
#define PROV_GNI_TEST_COMMON_H_

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "gnix_rdma_headers.h"
#include "gnix.h"
#include "ofi_util.h"

#define BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

#define CACHE_RO 0
#define CACHE_RW 1

#define GET_DOMAIN_RO_CACHE(domain) \
    ({ domain->mr_cache_info[domain->auth_key->ptag].mr_cache_ro; })
#define GET_DOMAIN_RW_CACHE(domain) \
    ({ domain->mr_cache_info[domain->auth_key->ptag].mr_cache_rw; })

/* defined in rdm_atomic.c */
extern int supported_compare_atomic_ops[FI_ATOMIC_OP_LAST][FI_DATATYPE_LAST];
extern int supported_fetch_atomic_ops[FI_ATOMIC_OP_LAST][FI_DATATYPE_LAST];

void calculate_time_difference(struct timeval *start, struct timeval *end,
		int *secs_out, int *usec_out);
int dump_cq_error(struct fid_cq *cq, void *context, uint64_t flags);

static inline struct gnix_fid_ep *get_gnix_ep(struct fid_ep *fid_ep)
{
	return container_of(fid_ep, struct gnix_fid_ep, ep_fid);
}

#define GNIX_MR_BASIC (FI_MR_BASIC)
#define GNIX_MR_SCALABLE (FI_MR_MMU_NOTIFY)
#define GNIX_DEFAULT_MR_MODE GNIX_MR_BASIC

#define LOC_ADDR(base, addr) (addr)
#define REM_ADDR(base, addr) \
	((gnit_use_scalable != 0) ? \
		((uint64_t)(addr) - (uint64_t)(base)) : (uint64_t) (addr))

#define GNIT_ALIGNMENT_ORDER 12
#define GNIT_ALIGNMENT_PGSIZE (1 << GNIT_ALIGNMENT_ORDER)
#define GNIT_ALIGNMENT_MASK ((uint64_t) (GNIT_ALIGNMENT_PGSIZE - 1))

#define GNIT_ALIGN_LEN(len) (((uint64_t) len) + GNIT_ALIGNMENT_PGSIZE)
#define GNIT_ALIGN_BUFFER(type, addr) \
	((((uint64_t) (addr)) & GNIT_ALIGNMENT_MASK) ? \
		(type)((((uint64_t) (addr)) + \
			GNIT_ALIGNMENT_PGSIZE) & ~(GNIT_ALIGNMENT_MASK)) \
			: (type)(addr))

#define USING_SCALABLE(hints) \
	(!(((hints)->domain_attr->mr_mode & FI_MR_VIRT_ADDR) || \
		((hints->domain_attr->mr_mode == FI_MR_BASIC))))

#define _REM_ADDR(info, base, addr) \
	(USING_SCALABLE(info) ? \
		((uint64_t)(addr) - (uint64_t)(base)) : (uint64_t) (addr))

#define SKIP_IF(cond, message) \
	do { \
		if (cond) \
			cr_skip_test(message); \
	} while (0)

#define SKIP_IF_SCALABLE_LT_1_5(version, mr_mode) \
	SKIP_IF((FI_VERSION_LT((version), FI_VERSION(1, 5)) && \
		!((mr_mode) & (FI_MR_BASIC | FI_MR_VIRT_ADDR))), \
		"scalable is not supported for versions less than " \
		"FI 1.5")

#define MR_ENABLE(mr, addr, len) \
	do { \
		struct iovec __iov = { \
			.iov_base = (void *) (addr),\
			.iov_len = (len), \
		}; \
		int enable_ret; \
	\
		enable_ret = fi_mr_refresh((mr), &__iov, 1, 0); \
		cr_assert_eq(enable_ret, FI_SUCCESS, "failed to enable mr"); \
	} while (0)

#endif /* PROV_GNI_TEST_COMMON_H_ */
