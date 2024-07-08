/*
 * Copyright (c) 2016-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#ifndef _FI_UCX_H
#define _FI_UCX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include <ucp/api/ucp.h>
#include <ucs/memory/memtype_cache.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/fabric.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>
#include <rdma/providers/fi_log.h>
#include <ofi.h>
#include <ofi_lock.h>
#include <ofi_list.h>
#include <ofi_enosys.h>
#include <ofi_util.h>
#include <ofi_prov.h>
#include <ofi_mr.h>
#include <ofi_hmem.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <glob.h>

#define FI_UCX_FLUSH ((int)0xFC)

#define FI_UCX_FABRIC_NAME "ucx"
#define FI_UCX_DEFAULT_INJECT_SIZE 1024
#define FI_UCX_DEFAULT_NS_PORT 12345
#define FI_UCX_DEF_CQ_SIZE (1024)
#define FI_UCX_DEF_MR_CNT (1 << 16)

#define FI_UCX_VERSION_MINOR 5
#define FI_UCX_VERSION_MAJOR 1

#define FI_UCX_RKEY_MAX_LEN (256)
#define FI_UCX_MAX_NAME_LEN (1024)

#define FI_UCX_DOM_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM)
#define FI_UCX_RMA_CAPS (FI_RMA | FI_READ | FI_WRITE | FI_REMOTE_READ |\
			 FI_REMOTE_WRITE)
#define FI_UCX_CAPS (FI_SEND | FI_RECV | FI_TAGGED | FI_MSG | FI_MULTI_RECV |\
		     FI_UCX_RMA_CAPS | FI_UCX_DOM_CAPS | FI_HMEM)

#define FI_UCX_MODE (0ULL)
#define FI_UCX_TX_FLAGS (FI_COMPLETION | FI_INJECT)
#define FI_UCX_RX_FLAGS (FI_COMPLETION | FI_MULTI_RECV)
#define FI_UCX_ANY_SERVICE (0)

struct ucx_global_descriptor{
	ucp_config_t *config;
	int use_ns;
	int ns_port;
	struct util_ns name_serv;
	char *localhost;
	int ep_flush;
	int enable_spawn;
	int check_req_leak;
};

struct ucx_fabric {
	struct util_fabric u_fabric;
};

struct ucx_mr {
	struct ofi_mr omr;
	ucp_mem_h memh;
};

#define FI_UCX_PKEY_SIGNATURE	(0x20230116ULL)
#define FI_UCX_MAX_KEY_SIZE	(64)

/* packed raw key, one per MR */
struct ucx_mr_pkey {
	struct dlist_entry rkey_list;
	uint64_t signature;
	size_t pkey_size;
	uint8_t pkey[0];
};

/* unpacked raw key, one per peer per MR */
struct ucx_mr_rkey {
	struct {
		fi_addr_t owner_addr;
		uint64_t key;
	} id;
	ucp_rkey_h rkey;
	UT_hash_handle hh;
	struct dlist_entry entry;
};

struct ucx_domain {
	struct util_domain u_domain;
	ucp_context_h context;
	struct ucx_mr_rkey *remote_keys;
};

struct ucx_ep {
	struct util_ep ep;
	struct ucx_av *av;
	ucp_worker_h worker;
	short service;
	void *addr;
	size_t addr_len;
	struct dlist_entry claimed_list;
	struct {
		size_t mrecv_min_size;
	} ep_opts;
	struct dlist_entry mctx_freelist;
	struct dlist_entry mctx_repost;
};

struct ucx_ave {
	ucp_ep_h uep;
	void *addr;
};

struct ucx_av {
	struct fid_av av;
	struct ucx_domain *domain;
	struct ucx_ep *ep;
	struct util_eq *eq;
	enum fi_av_type type;
	int async;
	size_t count;
	size_t addr_len;
	struct ofi_dyn_arr ave_array;
	struct ofi_dyn_arr addr_array;
};

#define UCX_GET_UCP_EP(EP, ADDR) (((struct ucx_ave*)(ADDR))->uep)

enum ucx_comm_mode {
	UCX_MSG,
	UCX_TAGGED,
};

enum ucx_req_type {
	UCX_REQ_UNSPEC,
	UCX_REQ_READ,
	UCX_REQ_WRITE,
	UCX_REQ_MULTIRECV,
	UCX_REQ_MULTIRECV_UNEXP,
	UCX_REQ_REGULAR,
	UCX_REQ_UNEXPECTED_ERR,
	UCX_REQ_UNEXPECTED,
};

struct ucx_mrecv_ctx {
	struct dlist_entry list;

	/* user buffer state */
	ssize_t remain;
	void *head;

	/* read-only repost/completion info */
	uint64_t tag;
	uint64_t tag_mask;
	uint64_t flags;
	void *context;
};

struct ucx_mrecv_request {
	enum ucx_req_type type;
	struct util_cq* cq;
	struct ucx_ep* ep;

	/* completion status*/
	ssize_t last_recvd;
	ucs_status_t status;

	struct ucx_mrecv_ctx *mrecv_ctx;
};

struct ucx_request {
	enum ucx_req_type type;
	struct util_cq* cq;
	struct ucx_ep* ep;

	/* completion status*/
	ssize_t posted_size;
	ucs_status_t status;

	struct fi_cq_tagged_entry completion;
};

static inline void ucx_req_release(struct ucx_request *req)
{
	req->type = UCX_REQ_UNSPEC;
	ucp_request_free(req);
}

#define UCX_EP_MSG_TAG (~0ULL)
#define UCX_AUX_TAG_MASK (~0ULL)

OFI_DECLARE_CIRQUE(struct fi_cq_tagged_entry, ucx_comp_cirq);

extern int ucx_errcode_translation_table[];

static inline int ucx_translate_errcode(ucs_status_t status)
{
	assert(status <= UCS_INPROGRESS && status > UCS_ERR_LAST);

	return ucx_errcode_translation_table[(-status)+1];
}

extern struct fi_provider ucx_prov;
extern struct ucx_global_descriptor ucx_descriptor;
extern struct util_prov ucx_util_prov;

extern struct fi_fabric_attr ucx_fabric_attrs;

int ucx_fabric_open(struct fi_fabric_attr *attr,
		    struct fid_fabric **fabric,
		    void *context);

int ucx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **fid, void *context);

int ucx_ep_open(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **fid, void *context);

int ucx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq, void *context);

int ucx_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

int ucx_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);

ssize_t ucx_mrecv_repost(struct ucx_ep *ep,
			 struct ucx_mrecv_ctx *mctx);

void ucx_send_callback_no_compl(void *request, ucs_status_t status);
void ucx_send_callback(void *request, ucs_status_t status);
void ucx_recv_callback_no_compl(void *request, ucs_status_t status,
				ucp_tag_recv_info_t *info);
void ucx_recv_callback(void *request, ucs_status_t status,
		       ucp_tag_recv_info_t *info);
void ucx_multi_recv_callback(void *request,
			     ucs_status_t ustatus,
			     ucp_tag_recv_info_t *info);

static inline
int ucx_write_error_completion(struct util_cq *cq, void *context,
			       uint64_t flags, int prov_err, int err,
			       uint64_t olen, uint64_t tag)
{
	struct fi_cq_err_entry cqe = {
		.op_context = context,
		.flags = flags,
		.olen = olen,
		.err = err,
		.prov_errno = prov_err,
		.tag = tag,
	};

	return ofi_cq_write_error(cq, &cqe);
}

#ifdef __cplusplus
}
#endif

#endif
