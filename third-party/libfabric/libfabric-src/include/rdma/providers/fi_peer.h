/*
 * Copyright (c) 2021-2023 Intel Corporation. All rights reserved.
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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

#ifndef FI_PEER_H
#define FI_PEER_H

#include <stdbool.h>
#include <rdma/fabric.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_endpoint.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/providers/fi_log.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Peer provider AV support.
 */
struct fid_peer_av;

struct fi_ops_av_owner {
	size_t	size;
	int	(*query)(struct fid_peer_av *av, struct fi_av_attr *attr);
	fi_addr_t (*ep_addr)(struct fid_peer_av *av, struct fid_ep *ep);
};

struct fid_peer_av {
	struct fid fid;
	struct fi_ops_av_owner *owner_ops;
};

struct fi_peer_av_context {
	size_t size;
	struct fid_peer_av *av;
};


/*
 * Peer provider AV set support.
 */
struct fid_peer_av_set;

struct fi_ops_av_set_owner {
	size_t	size;
	int	(*members)(struct fid_peer_av_set *av, fi_addr_t *addr,
			   size_t *count);
};

struct fid_peer_av_set {
	struct fid fid;
	struct fi_ops_av_set_owner *owner_ops;
};

struct fi_peer_av_set_context {
	size_t size;
	struct fi_peer_av_set *av_set;
};


/*
 * Peer provider CQ support.
 */
struct fid_peer_cq;

struct fi_ops_cq_owner {
	size_t	size;
	ssize_t (*write)(struct fid_peer_cq *cq, void *context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag,
			fi_addr_t src);
	ssize_t	(*writeerr)(struct fid_peer_cq *cq,
			const struct fi_cq_err_entry *err_entry);
};

struct fid_peer_cq {
	struct fid fid;
	struct fi_ops_cq_owner *owner_ops;
};

struct fi_peer_cq_context {
	size_t size;
	struct fid_peer_cq *cq;
};

/*
 * Peer provider counter support.
 */
struct fid_peer_cntr;

struct fi_ops_cntr_owner {
    size_t size;
    void (*inc)(struct fid_peer_cntr *cntr);
    void (*incerr)(struct fid_peer_cntr *cntr);
};

struct fid_peer_cntr {
    struct fid fid;
    struct fi_ops_cntr_owner *owner_ops;
};

struct fi_peer_cntr_context {
    size_t size;
    struct fid_peer_cntr *cntr;
};

/*
 * Peer provider domain support.
 */
struct fi_peer_domain_context {
	size_t size;
	struct fid_domain *domain;
};


/*
 * Peer provider EQ support.
 */
struct fi_peer_eq_context {
	size_t size;
	struct fid_eq *eq;
};


/*
 * Peer shared rx context
 */
struct fid_peer_srx;

/* Castable to dlist_entry */
struct fi_peer_rx_entry {
	struct fi_peer_rx_entry *next;
	struct fi_peer_rx_entry *prev;
	struct fid_peer_srx *srx;
	fi_addr_t addr;
	size_t size;
	uint64_t tag;
	uint64_t cq_data;
	uint64_t flags;
	void *context;
	size_t count;
	void **desc;
	void *peer_context;
	void *owner_context;
	struct iovec *iov;
};

struct fi_ops_srx_owner {
	size_t	size;
	int	(*get_msg)(struct fid_peer_srx *srx, fi_addr_t addr,
			size_t size, struct fi_peer_rx_entry **entry);
	int	(*get_tag)(struct fid_peer_srx *srx, fi_addr_t addr,
			uint64_t tag, struct fi_peer_rx_entry **entry);
	int	(*queue_msg)(struct fi_peer_rx_entry *entry);
	int	(*queue_tag)(struct fi_peer_rx_entry *entry);
	void	(*foreach_unspec_addr)(struct fid_peer_srx *srx,
			fi_addr_t (*get_addr)(struct fi_peer_rx_entry *));

	void	(*free_entry)(struct fi_peer_rx_entry *entry);
};

struct fi_ops_srx_peer {
	size_t	size;
	int	(*start_msg)(struct fi_peer_rx_entry *entry);
	int	(*start_tag)(struct fi_peer_rx_entry *entry);
	int	(*discard_msg)(struct fi_peer_rx_entry *entry);
	int	(*discard_tag)(struct fi_peer_rx_entry *entry);
};

struct fid_peer_srx {
	struct fid_ep ep_fid;
	struct fi_ops_srx_owner *owner_ops;
	struct fi_ops_srx_peer *peer_ops;
};

struct fi_peer_srx_context {
	size_t size;
	struct fid_peer_srx *srx;
};


/*
 * Peer transfers
 */
struct fi_peer_transfer_context;

struct fi_ops_transfer_peer {
	size_t size;
	ssize_t	(*complete)(struct fid_ep *ep, struct fi_cq_tagged_entry *buf,
			    fi_addr_t src_addr);
	ssize_t	(*comperr)(struct fid_ep *ep, struct fi_cq_err_entry *buf);
};

struct fi_peer_transfer_context {
	size_t size;
	struct fi_info *info;
	struct fid_ep *ep;
	struct fi_ops_transfer_peer *peer_ops;
};


#ifdef __cplusplus
}
#endif

#endif /* FI_PEER_H */
