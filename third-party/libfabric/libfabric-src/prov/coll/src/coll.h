
/*
 * Copyright (c) 2022 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _COLL_H_
#define _COLL_H_

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_util.h>
#include <ofi_list.h>
#include <ofi_lock.h>
#include <ofi_proto.h>
#include <ofi_iov.h>
#include <ofi_hmem.h>
#include <ofi_prov.h>
#include <ofi_atomic.h>
#include <ofi_coll.h>

#define COLL_IOV_LIMIT 4
#define COLL_MR_MODES	(OFI_MR_BASIC_MAP | FI_MR_LOCAL)
#define COLL_TX_OP_FLAGS (0)
#define COLL_RX_OP_FLAGS (0)

enum {
	COLL_RX_SIZE = 65536,
	COLL_TX_SIZE = 16384,
};

struct coll_domain {
	struct util_domain util_domain;
	struct fid_domain *peer_domain;
};

struct coll_av {
	struct util_av util_av;
	struct fid_peer_av *peer_av;
};

struct coll_ep {
	struct util_ep util_ep;
	struct fi_info *coll_info;

	/*
	 * Peer ep from the main provider.
	 * Used for pt2pt communications that the collectives are built on.
	 */
	struct fi_info *peer_info;
	struct fid_ep *peer_ep;
};

struct coll_mr {
	struct fid_mr mr_fid;
	struct fid_mr *msg_mr;
	struct coll_domain *domain;
	enum fi_hmem_iface iface;
	uint64_t device;
	ofi_mutex_t amo_lock;
};

static inline enum fi_hmem_iface
coll_mr_desc_to_hmem_iface_dev(void **desc, size_t count, uint64_t *device)
{
	if (!count || !desc || !desc[0]) {
		*device = 0;
		return FI_HMEM_SYSTEM;
	}

	*device = ((struct coll_mr *) desc[0])->device;
	return ((struct coll_mr *) desc[0])->iface;
}

extern struct fi_provider coll_prov;
extern struct util_prov coll_util_prov;
extern struct fi_fabric_attr coll_fabric_attr;
extern struct fi_info coll_info;

int coll_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context);

int coll_domain_open2(struct fid_fabric *fabric, struct fi_info *info,
		      struct fid_domain **dom, uint64_t flags, void *context);

int coll_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);

int coll_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		 struct fid_av **fid_av, void *context);

int coll_av_set(struct fid_av *av_fid, struct fi_av_set_attr *attr,
		struct fid_av_set **av_set_fid, void *context);

ssize_t coll_peer_xfer_complete(struct fid_ep *ep,
				struct fi_cq_tagged_entry *cqe,
				fi_addr_t src_addr);

ssize_t coll_peer_xfer_error(struct fid_ep *ep, struct fi_cq_err_entry *cqerr);

int coll_query_collective(struct fid_domain *dom_fid,
			  enum fi_collective_op coll,
			  struct fi_collective_attr *attr, uint64_t flags);

int coll_join_collective(struct fid_ep *ep, const void *addr,
		         uint64_t flags, struct fid_mc **mc, void *context);

void coll_ep_progress(struct util_ep *util_ep);

ssize_t coll_ep_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context);

ssize_t coll_ep_barrier2(struct fid_ep *ep, fi_addr_t coll_addr, uint64_t flags,
			 void *context);

ssize_t coll_ep_allreduce(struct fid_ep *ep, const void *buf, size_t count,
			  void *desc, void *result, void *result_desc,
			  fi_addr_t coll_addr, enum fi_datatype datatype,
			  enum fi_op op, uint64_t flags, void *context);

ssize_t coll_ep_allgather(struct fid_ep *ep, const void *buf, size_t count,
			  void *desc, void *result, void *result_desc,
			  fi_addr_t coll_addr, enum fi_datatype datatype,
			  uint64_t flags, void *context);

ssize_t coll_ep_scatter(struct fid_ep *ep, const void *buf, size_t count,
			void *desc, void *result, void *result_desc,
			fi_addr_t coll_addr, fi_addr_t root_addr,
			enum fi_datatype datatype, uint64_t flags,
		        void *context);

ssize_t coll_ep_broadcast(struct fid_ep *ep, void *buf, size_t count,
			  void *desc, fi_addr_t coll_addr, fi_addr_t root_addr,
			  enum fi_datatype datatype, uint64_t flags,
			  void *context);
#endif /* _COLL_H_ */

