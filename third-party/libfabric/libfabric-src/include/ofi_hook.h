/*
 * Copyright (c) 2018-2019 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL); Version 2, available from the file
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

#ifndef _OFI_HOOK_H_
#define _OFI_HOOK_H_

#include <assert.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_collective.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>

#include <ofi.h>
#include <rdma/providers/fi_prov.h>

/* This field needs to be updated whenever new FI class is added in fabric.h */
#define HOOK_FI_CLASS_MAX (FI_CLASS_NIC + 1)

/*
 * Hooks are installed from top down.
 * Values must start at 0 and increment by one.
 */
// TODO figure out how to remove this now that we have ini/fini calls
enum ofi_hook_class {
	HOOK_NOOP,
	HOOK_PERF,
	HOOK_TRACE,
	HOOK_PROFILE,
	HOOK_DEBUG,
	HOOK_HMEM,
	HOOK_DMABUF_PEER_MEM,
};


/*
 * Default fi_ops members, can be used to construct custom fi_ops
 */
int hook_close(struct fid *fid);
int hook_bind(struct fid *fid, struct fid *bfid, uint64_t flags);
int hook_control(struct fid *fid, int command, void *arg);
int hook_ops_open(struct fid *fid, const char *name,
		  uint64_t flags, void **ops, void *context);

/*
 * Define hook structs so we can cast from fid to parent using simple cast.
 * This lets us have a single close() call.
 */

extern struct fi_ops hook_fid_ops;
struct fid *hook_to_hfid(const struct fid *fid);
struct fid_wait *hook_to_hwait(const struct fid_wait *wait);

/*
 * TODO
 * comment from GitHub PR #5052:
 * "another option would be to store the ini/fini calls in a separate structure
 * that we reference from struct ofi_prov_context. We could even extend the
 * definition of ofi_prov_context with a union that is accessed based on the
 * prov_type. That might work better if we want to support external hooks,
 * without the external hook provider needing to implement everything"
 */
struct hook_prov_ctx {
	struct fi_provider	prov;
	/*
	 * Hooking providers can override ini/fini calls of a specific fid class
	 * to override any initializations that the common code may have done.
	 * For example, this allows overriding any specific op and not having to
	 * hook into every resource creation call until the point where the op
	 * can be overridden. Refer to hook_perf for an example.
	 *
	 * Note: if a hooking provider overrides any of the resource creation calls
	 * (e.g. fi_endpoint) directly, then these ini/fini calls won't be
	 * invoked. */
	int 			(*ini_fid[HOOK_FI_CLASS_MAX])(struct fid *fid);
	int 			(*fini_fid[HOOK_FI_CLASS_MAX])(struct fid *fid);
};

/*
 * TODO
 * comment from GitHub PR #5052:
 * "We could set all ini/fini calls to a no-op as part of hook initialization
 * to avoid this check"
 */
static inline int hook_ini_fid(struct hook_prov_ctx *prov_ctx, struct fid *fid)
{
	return (prov_ctx->ini_fid[fid->fclass] ?
		prov_ctx->ini_fid[fid->fclass](fid) : 0);
}

static inline int hook_fini_fid(struct hook_prov_ctx *prov_ctx, struct fid *fid)
{
	return (prov_ctx->fini_fid[fid->fclass] ?
		prov_ctx->fini_fid[fid->fclass](fid) : 0);
}

struct hook_fabric {
	struct fid_fabric	fabric;
	struct fid_fabric	*hfabric;
	enum ofi_hook_class	hclass;
	struct fi_provider	*hprov;
	struct hook_prov_ctx	*prov_ctx;
};

void hook_fabric_init(struct hook_fabric *fabric, enum ofi_hook_class hclass,
		      struct fid_fabric *hfabric, struct fi_provider *hprov,
		      struct fi_ops *f_ops, struct hook_prov_ctx *prov_ctx);

struct hook_fabric *hook_to_fabric(const struct fid *fid);

static inline struct hook_prov_ctx *hook_to_prov_ctx(const struct fid *fid)
{
	return (hook_to_fabric(fid))->prov_ctx;
}

static inline struct fi_provider *
hook_fabric_to_hprov(const struct hook_fabric *fabric)
{
	return fabric->hprov;
}

static inline struct fi_provider *hook_to_hprov(const struct fid *fid)
{
	return hook_fabric_to_hprov(hook_to_fabric(fid));
}

struct ofi_ops_flow_ctrl;

struct hook_domain {
	struct fid_domain domain;
	struct fid_domain *hdomain;
	struct hook_fabric *fabric;
	struct ofi_ops_flow_ctrl *base_ops_flow_ctrl;
	ssize_t (*base_credit_handler)(struct fid_ep *ep_fid, uint64_t credits);
};

int hook_domain_init(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain, void *context,
		     struct hook_domain *dom);
int hook_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context);


struct hook_av {
	struct fid_av av;
	struct fid_av *hav;
	struct hook_domain *domain;
};

int hook_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context);


struct hook_wait {
	struct fid_wait wait;
	struct fid_wait *hwait;
	struct hook_fabric *fabric;
};

int hook_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset);
int hook_trywait(struct fid_fabric *fabric, struct fid **fids, int count);


struct hook_poll {
	struct fid_poll poll;
	struct fid_poll *hpoll;
	struct hook_domain *domain;
};

int hook_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
		   struct fid_poll **pollset);

/*
 * EQ
 */

extern struct fi_ops_eq hook_eq_ops;

struct hook_eq {
	struct fid_eq eq;
	struct fid_eq *heq;
	struct hook_fabric *fabric;
};

ssize_t hook_eq_read(struct fid_eq *eq, uint32_t *event,
			    void *buf, size_t len, uint64_t flags);
ssize_t hook_eq_sread(struct fid_eq *eq, uint32_t *event,
			     void *buf, size_t len, int timeout, uint64_t flags);
int hook_eq_init(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context, struct hook_eq *myeq);
int hook_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context);

/*
 * CQ
 */

struct hook_cq {
	struct fid_cq cq;
	struct fid_cq *hcq;
	struct hook_domain *domain;
	void *context;
	enum fi_cq_format format;
};

int hook_cq_init(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context, struct hook_cq *mycq);
int hook_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context);
const char *
hook_cq_strerror(struct fid_cq *cq, int prov_errno,
		 const void *err_data, char *buf, size_t len);

struct hook_cntr {
	struct fid_cntr cntr;
	struct fid_cntr *hcntr;
	struct hook_domain *domain;
};

int hook_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context);


struct hook_ep {
	struct fid_ep ep;
	struct fid_ep *hep;
	struct hook_domain *domain;
	void *context;
};

int hook_endpoint_init(struct fid_domain *domain, struct fi_info *info,
		       struct fid_ep **ep, void *context, struct hook_ep *myep);
int hook_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);
int hook_scalable_ep(struct fid_domain *domain, struct fi_info *info,
		     struct fid_ep **sep, void *context);
int hook_srx_ctx(struct fid_domain *domain,
		 struct fi_rx_attr *attr, struct fid_ep **rx_ep,
		 void *context);

int hook_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		  enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags);
int hook_query_collective(struct fid_domain *domain, enum fi_collective_op coll,
			  struct fi_collective_attr *attr, uint64_t flags);

extern struct fi_ops hook_fabric_fid_ops;
extern struct fi_ops hook_domain_fid_ops;
extern struct fi_ops_fabric hook_fabric_ops;
extern struct fi_ops_domain hook_domain_ops;
extern struct fi_ops_cq hook_cq_ops;
extern struct fi_ops_cntr hook_cntr_ops;

extern struct fi_ops_cm hook_cm_ops;
extern struct fi_ops_msg hook_msg_ops;
extern struct fi_ops_rma hook_rma_ops;
extern struct fi_ops_tagged hook_tagged_ops;
extern struct fi_ops_atomic hook_atomic_ops;


struct hook_pep {
	struct fid_pep pep;
	struct fid_pep *hpep;
	struct hook_fabric *fabric;
};

int hook_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep, void *context);


struct hook_stx {
	struct fid_stx stx;
	struct fid_stx *hstx;
	struct hook_domain *domain;
};

int hook_stx_ctx(struct fid_domain *domain,
		 struct fi_tx_attr *attr, struct fid_stx **stx,
		 void *context);


struct hook_mr {
	struct fid_mr mr;
	struct fid_mr *hmr;
	struct hook_domain *domain;
};


#endif /* _OFI_HOOK_H_ */
