/*
 * Copyright (c) 2019-2022 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _OFI_COLL_H_
#define _OFI_COLL_H_

#include <rdma/fi_collective.h>

#include <ofi_list.h>
#include <ofi_atom.h>
#include <ofi_bitmask.h>
#include <ofi_util.h>

#define OFI_WORLD_GROUP_ID 0
#define OFI_MAX_GROUP_ID 256

enum util_coll_op_type {
	UTIL_COLL_JOIN_OP,
	UTIL_COLL_BARRIER_OP,
	UTIL_COLL_ALLREDUCE_OP,
	UTIL_COLL_BROADCAST_OP,
	UTIL_COLL_ALLGATHER_OP,
	UTIL_COLL_SCATTER_OP,
};

static const char * const log_util_coll_op_type[] = {
	[UTIL_COLL_JOIN_OP] = "COLL_JOIN",
	[UTIL_COLL_BARRIER_OP] = "COLL_BARRIER",
	[UTIL_COLL_ALLREDUCE_OP] = "COLL_ALLREDUCE",
	[UTIL_COLL_BROADCAST_OP] = "COLL_BROADCAST",
	[UTIL_COLL_ALLGATHER_OP] = "COLL_ALLGATHER",
	[UTIL_COLL_SCATTER_OP] = "COLL_SCATTER"
};

enum coll_work_type {
	UTIL_COLL_SEND,
	UTIL_COLL_RECV,
	UTIL_COLL_REDUCE,
	UTIL_COLL_COPY,
	UTIL_COLL_COMP,
};

enum coll_state {
	UTIL_COLL_WAITING,
	UTIL_COLL_PROCESSING,
	UTIL_COLL_COMPLETE
};

static const char * const log_util_coll_state[] = {
	[UTIL_COLL_WAITING] = "COLL_WAITING",
	[UTIL_COLL_PROCESSING] = "COLL_PROCESSING",
	[UTIL_COLL_COMPLETE] = "COLL_COMPLETE"
};

struct util_coll_operation;

struct util_coll_work_item {
	struct slist_entry		ready_entry;
	struct dlist_entry		waiting_entry;
	struct util_coll_operation 	*coll_op;
	enum coll_work_type		type;
	enum coll_state			state;
	int				fence;
};

struct util_coll_xfer_item {
	struct util_coll_work_item	hdr;
	void 				*buf;
	int				count;
	enum fi_datatype		datatype;
	uint64_t			tag;
	int				remote_rank;
};

struct util_coll_copy_item {
	struct util_coll_work_item	hdr;
	void 				*in_buf;
	void				*out_buf;
	int				count;
	enum fi_datatype		datatype;
};

struct util_coll_reduce_item {
	struct util_coll_work_item	hdr;
	void 				*in_buf;
	void 				*inout_buf;
	int				count;
	enum fi_datatype		datatype;
	enum fi_op			op;
};

struct join_data {
	struct util_coll_mc *new_mc;
	struct ofi_bitmask data;
	struct ofi_bitmask tmp;
};

struct barrier_data {
	uint64_t data;
	uint64_t tmp;
};

struct allreduce_data {
	void	*data;
	size_t	size;
};

struct broadcast_data {
	void	*chunk;
	size_t	size;
	void	*scatter;
};

struct util_coll_operation;

typedef void (*util_coll_comp_fn_t)(struct util_coll_operation *coll_op);
struct util_coll_operation {
	enum util_coll_op_type		type;
	uint32_t			cid;
	void				*context;
	struct fid_ep			*ep;
	struct util_coll_mc		*mc;
	struct dlist_entry		work_queue;

	union {
		struct join_data	join;
		struct barrier_data	barrier;
		struct allreduce_data	allreduce;
		void			*scatter;
		struct broadcast_data	broadcast;
	} data;
	util_coll_comp_fn_t		comp_fn;
	uint64_t			flags;
};

struct ofi_coll_cq {
	struct util_cq util_cq;
	struct fid_peer_cq *peer_cq;
};

int ofi_coll_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context);

struct ofi_coll_eq {
	struct util_eq util_eq;
	struct fid_eq *peer_eq;
};

int ofi_coll_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq_fid, void *context);

#endif // _OFI_COLL_H_
