/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#ifndef _FI_PROV_BGQ_INTERNAL_H_
#define _FI_PROV_BGQ_INTERNAL_H_

#include <config.h>

#include <stdbool.h>
#include <stdlib.h>

#include "rdma/bgq/fi_bgq_node.h"

#include <rdma/fabric.h>
#include <rdma/providers/fi_log.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_atomic.h>

#include <rdma/fi_direct.h>
#include <rdma/fi_direct_domain.h>
#include <rdma/fi_direct_endpoint.h>
#include <rdma/fi_direct_cm.h>
#include <rdma/fi_direct_eq.h>
#include <rdma/fi_direct_tagged.h>
#include <rdma/fi_direct_atomic.h>
#include <rdma/fi_direct_rma.h>

#include "fi_bgq_hwi.h"


#define FI_BGQ_PFX "bgq"

// #define FI_BGQ_TRACE 1

/* --- Will be exposed by fabric.h */
#define FI_BGQ_PROTOCOL		0x0008
#define FI_BGQ_PROTOCOL_VERSION (1)
/* --- end */

#define FI_BGQ_PROVIDER_NAME	"bgq"
#define FI_BGQ_PROVIDER_VERSION (1)
#define FI_BGQ_DEVICE_MAX_PATH_NAME	(32)
#define FI_BGQ_FABRIC_NAME	"BGQ"

#define FI_BGQ_CACHE_LINE_SIZE	(L2_CACHE_LINE_SIZE)

#define FI_BGQ_MAX_STRLEN	(64)

#define EXIT_FAILURE 1

struct fi_bgq_global_data {
	struct fi_info		*info;
	struct fi_domain_attr	*default_domain_attr;
	struct fi_ep_attr	*default_ep_attr;
	struct fi_tx_attr	*default_tx_attr;
	struct fi_rx_attr	*default_rx_attr;
	struct fi_provider 	*prov;
};

extern struct fi_bgq_global_data fi_bgq_global;

static inline void always_assert(bool val, char *msg)
{
	if (!val) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"%s\n", msg);
		exit(EXIT_FAILURE);
	}
}

static inline void fi_bgq_ref_init(struct fi_bgq_node *node,
		struct l2atomic_counter *ref, char *name)
{
	int ret __attribute__ ((unused));
	ret = fi_bgq_node_counter_allocate(node, ref);
	assert(ret == 0);

	FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
			"initializing ref count for (%s) to (%d)\n",
			name, 0);

	return;
}

static inline void fi_bgq_ref_inc(struct l2atomic_counter *ref, char *name)
{
	l2atomic_counter_add(ref, 1);
	return;
}

static inline int fi_bgq_ref_dec(struct l2atomic_counter *ref, char *name)
{
	int64_t value = -1;
	value = (int64_t) l2atomic_counter_decrement(ref);

	if ((value-1) < 0) {

		FI_WARN(fi_bgq_global.prov, FI_LOG_FABRIC,
			"decrement ref for (%s) (ref_cnt %d < 0)\n",
			name, (value-1));

		errno = FI_EOTHER;
		return -errno;
	}
	return 0;
}

static inline int fi_bgq_ref_finalize(struct l2atomic_counter *ref, char *name)
{
	int64_t value = -1;
	value = (int64_t) l2atomic_counter_get(ref);

	if (value != 0) {
		FI_WARN(fi_bgq_global.prov, FI_LOG_FABRIC,
			"error ref for (%s) (ref_cnt %d != 0)\n",
			name, value);
		errno = FI_EBUSY;
		return -errno;
	}
	return 0;
}

static inline int fi_bgq_fid_check(fid_t fid, int fid_class, char *name)
{
	if (!fid) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"NULL %s object", name);
		errno = FI_EINVAL;
		return -errno;
	}
	if (fid->fclass != fid_class) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
			"wrong type of object (%s) expected (%d), got (%d)\n",
			name, fid_class, fid->fclass);
		errno = FI_EINVAL;
		return -errno;
	}
	return 0;
}

struct fi_bgq_fabric {
	struct fid_fabric	fabric_fid;
	struct fi_bgq_node	node;
	struct l2atomic_counter	ref_cnt;
};


int fi_bgq_set_default_info(void);

int fi_bgq_check_info(const struct fi_info *info);

int fi_bgq_fabric(struct fi_fabric_attr *attr,
		struct fid_fabric **fabric, void *context);

int fi_bgq_check_fabric_attr(const struct fi_fabric_attr *attr);

int fi_bgq_domain(struct fid_fabric *fabric,
		struct fi_info *info,
		struct fid_domain **dom, void *context);

int fi_bgq_check_domain_attr(const struct fi_domain_attr *attr);
int fi_bgq_choose_domain(uint64_t caps,
		struct fi_domain_attr *domain_attr,
		const struct fi_domain_attr *hints);

int fi_bgq_alloc_default_domain_attr(struct fi_domain_attr **domain_attr);

int fi_bgq_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr,
	       struct fid_stx **stx, void *context);

int fi_bgq_av_open(struct fid_domain *dom,
		struct fi_av_attr *attr, struct fid_av **av,
		void *context);

int fi_bgq_cq_open(struct fid_domain *dom,
		struct fi_cq_attr *attr,
		struct fid_cq **eq, void *context);

int fi_bgq_endpoint(struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context);

int fi_bgq_alloc_default_ep_attr(struct fi_ep_attr **ep_attr);

int fi_bgq_check_ep_attr(const struct fi_ep_attr *attr);

int fi_bgq_alloc_default_tx_attr(struct fi_tx_attr **tx_attr);
int fi_bgq_check_tx_attr(const struct fi_tx_attr *attr);

int fi_bgq_alloc_default_rx_attr(struct fi_rx_attr **rx_attr);
int fi_bgq_check_rx_attr(const struct fi_rx_attr *attr);

int fi_bgq_scalable_ep(struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context);

int fi_bgq_cntr_open(struct fid_domain *domain,
		struct fi_cntr_attr *attr,
		struct fid_cntr **cntr, void *context);

int fi_bgq_init_mr_ops(struct fi_bgq_domain *bgq_domain, struct fi_info *info);
int fi_bgq_finalize_mr_ops(struct fi_bgq_domain *bgq_domain);

int fi_bgq_init_rma_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info);
int fi_bgq_enable_rma_ops(struct fi_bgq_ep *bgq_ep);
int fi_bgq_finalize_rma_ops(struct fi_bgq_ep *bgq_ep);

int fi_bgq_init_msg_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info);
int fi_bgq_enable_msg_ops(struct fi_bgq_ep *bgq_ep);
int fi_bgq_finalize_msg_ops(struct fi_bgq_ep *bgq_ep);

int fi_bgq_init_atomic_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info);
int fi_bgq_enable_atomic_ops(struct fi_bgq_ep *bgq_ep);
int fi_bgq_finalize_atomic_ops(struct fi_bgq_ep *bgq_ep);

int fi_bgq_init_tagged_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info);
int fi_bgq_enable_tagged_ops(struct fi_bgq_ep *bgq_ep);
int fi_bgq_finalize_tagged_ops(struct fi_bgq_ep *bgq_ep);

int fi_bgq_init_cm_ops(struct fid_ep *ep_fid, struct fi_info *info);
int fi_bgq_finalize_cm_ops(struct fi_bgq_ep *bgq_ep);

int fi_bgq_bind_ep_stx(struct fi_bgq_ep *ep,
		struct fi_bgq_stx *stx, uint64_t flags);
int fi_bgq_bind_ep_cq(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_cq *bgq_cq, uint64_t flags);
int fi_bgq_bind_ep_cntr(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_cntr *bgq_cntr, uint64_t flags);
int fi_bgq_bind_ep_mr(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_mr *bgq_mr, uint64_t flags);
int fi_bgq_bind_ep_av(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_av *bgq_av, uint64_t flags);

#endif /* _FI_PROV_BGQ_INTERNAL_H_ */
