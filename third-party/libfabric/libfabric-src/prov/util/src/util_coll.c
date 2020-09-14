/*
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
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

#include "config.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <inttypes.h>

#if HAVE_GETIFADDRS
#include <net/if.h>
#include <ifaddrs.h>
#endif

#include <ofi_util.h>

#include <rdma/fi_collective.h>
#include <rdma/fi_cm.h>
#include <ofi_list.h>
#include <ofi_atomic.h>
#include <ofi_coll.h>
#include <ofi_osd.h>

int ofi_av_set_union(struct fid_av_set *dst, const struct fid_av_set *src)
{
	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	size_t temp_count;
	int i,j;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);
	temp_count = dst_av_set->fi_addr_count;

	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = 0; j < dst_av_set->fi_addr_count; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i])
				break;
		}
		if (j == dst_av_set->fi_addr_count) {
			dst_av_set->fi_addr_array[temp_count++] =
				src_av_set->fi_addr_array[i];
		}
	}

	dst_av_set->fi_addr_count = temp_count;
	return FI_SUCCESS;
}

int ofi_av_set_intersect(struct fid_av_set *dst, const struct fid_av_set *src)
{
	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	int i,j, temp;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);

	temp = 0;
	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = temp; j < dst_av_set->fi_addr_count; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i]) {
				dst_av_set->fi_addr_array[temp++] =
					dst_av_set->fi_addr_array[j];
				break;
			}
		}
	}
	dst_av_set->fi_addr_count = temp;
	return FI_SUCCESS;
}

int ofi_av_set_diff(struct fid_av_set *dst, const struct fid_av_set *src)
{

	struct util_av_set *src_av_set;
	struct util_av_set *dst_av_set;
	int i,j, temp;

	src_av_set = container_of(src, struct util_av_set, av_set_fid);
	dst_av_set = container_of(dst, struct util_av_set, av_set_fid);

	assert(src_av_set->av == dst_av_set->av);

	temp = dst_av_set->fi_addr_count;
	for (i = 0; i < src_av_set->fi_addr_count; i++) {
		for (j = 0; j < temp; j++) {
			if (dst_av_set->fi_addr_array[j] ==
			    src_av_set->fi_addr_array[i]) {
				dst_av_set->fi_addr_array[--temp] =
					dst_av_set->fi_addr_array[j];
				break;
			}
		}
	}
	dst_av_set->fi_addr_count = temp;
	return FI_SUCCESS;
}

int ofi_av_set_insert(struct fid_av_set *set, fi_addr_t addr)
{
	struct util_av_set *av_set;
	int i;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	for (i = 0; i < av_set->fi_addr_count; i++) {
		if (av_set->fi_addr_array[i] == addr)
			return -FI_EINVAL;
	}
	av_set->fi_addr_array[av_set->fi_addr_count++] = addr;
	return FI_SUCCESS;
}

int ofi_av_set_remove(struct fid_av_set *set, fi_addr_t addr)

{
	struct util_av_set *av_set;
	int i;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	for (i = 0; i < av_set->fi_addr_count; i++) {
		if (av_set->fi_addr_array[i] == addr) {
			av_set->fi_addr_array[i] =
				av_set->fi_addr_array[--av_set->fi_addr_count];
			return FI_SUCCESS;
		}
	}
	return -FI_EINVAL;
}

static inline uint64_t util_coll_form_tag(uint32_t coll_id, uint32_t rank)
{
	uint64_t tag;
	uint64_t src_rank = rank;

	tag = coll_id;
	tag |= (src_rank << 32);

	return OFI_COLL_TAG_FLAG | tag;
}

static inline uint32_t util_coll_get_next_id(struct util_coll_mc *coll_mc)
{
	uint32_t cid = coll_mc->group_id;
	return cid << 16 | coll_mc->seq++;
}

static inline int util_coll_op_create(struct util_coll_operation **coll_op,
				    struct util_coll_mc *coll_mc,
				    enum util_coll_op_type type, void *context,
				    util_coll_comp_fn_t comp_fn)
{
	*coll_op = calloc(1, sizeof(**coll_op));
	if (!(*coll_op))
		return -FI_ENOMEM;

	(*coll_op)->cid = util_coll_get_next_id(coll_mc);
	(*coll_op)->mc = coll_mc;
	(*coll_op)->type = type;
	(*coll_op)->context = context;
	(*coll_op)->comp_fn = comp_fn;
	dlist_init(&(*coll_op)->work_queue);

	return FI_SUCCESS;
}

static inline void util_coll_op_log_work(struct util_coll_operation *coll_op)
{
#if ENABLE_DEBUG
	struct util_coll_work_item *cur_item = NULL;
	struct util_coll_xfer_item *xfer_item;
	struct dlist_entry *tmp = NULL;
	size_t count = 0;
	FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ, "Remaining Work for %s:\n",
	       log_util_coll_op_type[coll_op->type]);
	dlist_foreach_container_safe(&coll_op->work_queue, struct util_coll_work_item,
				     cur_item, waiting_entry, tmp)
	{
		switch (cur_item->type) {
		case UTIL_COLL_SEND:
			xfer_item =
				container_of(cur_item, struct util_coll_xfer_item, hdr);
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] SEND TO: 0x%02x FROM: 0x%02lx "
			       "cnt: %d typesize: %ld tag: 0x%02lx }\n",
			       count, cur_item, log_util_coll_state[cur_item->state],
			       xfer_item->remote_rank, coll_op->mc->local_rank,
			       xfer_item->count, ofi_datatype_size(xfer_item->datatype),
			       xfer_item->tag);
			break;
		case UTIL_COLL_RECV:
			xfer_item =
				container_of(cur_item, struct util_coll_xfer_item, hdr);
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] RECV FROM: 0x%02x TO: 0x%02lx "
			       "cnt: %d typesize: %ld tag: 0x%02lx }\n",
			       count, cur_item, log_util_coll_state[cur_item->state],
			       xfer_item->remote_rank, coll_op->mc->local_rank,
			       xfer_item->count, ofi_datatype_size(xfer_item->datatype),
			       xfer_item->tag);
			break;
		case UTIL_COLL_REDUCE:
			//reduce_item = container_of(cur_item, struct util_coll_reduce_item, hdr);
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] REDUCTION }\n", count, cur_item,
			       log_util_coll_state[cur_item->state]);
			break;
		case UTIL_COLL_COPY:
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] COPY }\n", count, cur_item,
			       log_util_coll_state[cur_item->state]);
			break;
		case UTIL_COLL_COMP:
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] COMPLETION }\n", count, cur_item,
			       log_util_coll_state[cur_item->state]);
			break;
		default:
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "\t%ld: { %p [%s] UNKNOWN }\n", count, cur_item,
			       log_util_coll_state[cur_item->state]);
			break;
		}
		count++;
	}
#endif
}

static inline void util_coll_op_progress_work(struct util_ep *util_ep,
				      struct util_coll_operation *coll_op)
{
	struct util_coll_work_item *next_ready = NULL;
	struct util_coll_work_item *cur_item = NULL;
	struct util_coll_work_item *prev_item = NULL;
	struct dlist_entry *tmp = NULL;
	int previous_is_head;

	// clean up any completed items while searching for the next ready
	dlist_foreach_container_safe(&coll_op->work_queue, struct util_coll_work_item,
				     cur_item, waiting_entry, tmp)
	{
		previous_is_head = cur_item->waiting_entry.prev == &cur_item->coll_op->work_queue;
		if (!previous_is_head) {
			prev_item = container_of(cur_item->waiting_entry.prev,
						 struct util_coll_work_item,
						 waiting_entry);
		}

		if (cur_item->state == UTIL_COLL_COMPLETE) {
			// if there is work before cur and cur is fencing, we can't complete
			if (cur_item->fence && !previous_is_head)
				continue;

			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "Removing Completed Work item: %p \n", cur_item);
			dlist_remove(&cur_item->waiting_entry);
			free(cur_item);

			// if the work queue is empty, we're done
			if (dlist_empty(&coll_op->work_queue)) {
				free(coll_op);
				return;
			}
			continue;
		}

		// we can't progress if prior work is fencing
		if (!previous_is_head && prev_item && prev_item->fence) {
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "%p fenced by: %p \n", cur_item, prev_item);
			return;
		}

		// if the current item isn't waiting, it's not the next ready item
		if (cur_item->state != UTIL_COLL_WAITING) {
			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "Work item not waiting: %p [%s]\n", cur_item,
			       log_util_coll_state[cur_item->state]);
			continue;
		}

		FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ, "Ready item: %p \n",
		       cur_item);
		next_ready = cur_item;
		break;
	}

	if (!next_ready)
		return;

	util_coll_op_log_work(coll_op);

	next_ready->state = UTIL_COLL_PROCESSING;
	slist_insert_tail(&next_ready->ready_entry, &util_ep->coll_ready_queue);
}

static inline void util_coll_op_bind_work(struct util_coll_operation *coll_op,
					  struct util_coll_work_item *item)
{
	item->coll_op = coll_op;
	dlist_insert_tail(&item->waiting_entry, &coll_op->work_queue);
}

static int util_coll_sched_send(struct util_coll_operation *coll_op, uint32_t dest,
				void *buf, int count, enum fi_datatype datatype,
				int fence)
{
	struct util_coll_xfer_item *xfer_item;

	xfer_item = calloc(1, sizeof(*xfer_item));
	if (!xfer_item)
		return -FI_ENOMEM;

	xfer_item->hdr.type = UTIL_COLL_SEND;
	xfer_item->hdr.state = UTIL_COLL_WAITING;
	xfer_item->hdr.fence = fence;
	xfer_item->tag = util_coll_form_tag(coll_op->cid, coll_op->mc->local_rank);
	xfer_item->buf = buf;
	xfer_item->count = count;
	xfer_item->datatype = datatype;
	xfer_item->remote_rank = dest;

	util_coll_op_bind_work(coll_op, &xfer_item->hdr);
	return FI_SUCCESS;
}

static int util_coll_sched_recv(struct util_coll_operation *coll_op, uint32_t src,
				void *buf, int count, enum fi_datatype datatype,
				int fence)
{
	struct util_coll_xfer_item *xfer_item;

	xfer_item = calloc(1, sizeof(*xfer_item));
	if (!xfer_item)
		return -FI_ENOMEM;

	xfer_item->hdr.type = UTIL_COLL_RECV;
	xfer_item->hdr.state = UTIL_COLL_WAITING;
	xfer_item->hdr.fence = fence;
	xfer_item->tag = util_coll_form_tag(coll_op->cid, src);
	xfer_item->buf = buf;
	xfer_item->count = count;
	xfer_item->datatype = datatype;
	xfer_item->remote_rank = src;

	util_coll_op_bind_work(coll_op, &xfer_item->hdr);
	return FI_SUCCESS;
}

static int util_coll_sched_reduce(struct util_coll_operation *coll_op, void *in_buf,
				  void *inout_buf, int count, enum fi_datatype datatype,
				  enum fi_op op, int fence)
{
	struct util_coll_reduce_item *reduce_item;

	reduce_item = calloc(1, sizeof(*reduce_item));
	if (!reduce_item)
		return -FI_ENOMEM;

	reduce_item->hdr.type = UTIL_COLL_REDUCE;
	reduce_item->hdr.state = UTIL_COLL_WAITING;
	reduce_item->hdr.fence = fence;
	reduce_item->in_buf = in_buf;
	reduce_item->inout_buf = inout_buf;
	reduce_item->count = count;
	reduce_item->datatype = datatype;
	reduce_item->op = op;

	util_coll_op_bind_work(coll_op, &reduce_item->hdr);
	return FI_SUCCESS;
}

static int util_coll_sched_copy(struct util_coll_operation *coll_op, void *in_buf,
				void *out_buf, int count, enum fi_datatype datatype,
				int fence)
{
	struct util_coll_copy_item *copy_item;

	copy_item = calloc(1, sizeof(*copy_item));
	if (!copy_item)
		return -FI_ENOMEM;

	copy_item->hdr.type = UTIL_COLL_COPY;
	copy_item->hdr.state = UTIL_COLL_WAITING;
	copy_item->hdr.fence = fence;
	copy_item->in_buf = in_buf;
	copy_item->out_buf = out_buf;
	copy_item->count = count;
	copy_item->datatype = datatype;

	util_coll_op_bind_work(coll_op, &copy_item->hdr);
	return FI_SUCCESS;
}

static int util_coll_sched_comp(struct util_coll_operation *coll_op)
{
	struct util_coll_work_item *comp_item;

	comp_item = calloc(1, sizeof(*comp_item));
	if (!comp_item)
		return -FI_ENOMEM;

	comp_item->type = UTIL_COLL_COMP;
	comp_item->state = UTIL_COLL_WAITING;
	comp_item->fence = 1;

	util_coll_op_bind_work(coll_op, comp_item);
	return FI_SUCCESS;
}

/* TODO: when this fails, clean up the already scheduled work in this function */
static int util_coll_allreduce(struct util_coll_operation *coll_op, const void *send_buf,
			void *result, void* tmp_buf, int count, enum fi_datatype datatype,
			enum fi_op op)
{
	uint64_t rem, pof2, my_new_id;
	uint64_t local, remote, next_remote;
	int ret;
	uint64_t mask = 1;

	pof2 = rounddown_power_of_two(coll_op->mc->av_set->fi_addr_count);
	rem = coll_op->mc->av_set->fi_addr_count - pof2;
	local = coll_op->mc->local_rank;

	// copy initial send data to result
	memcpy(result, send_buf, count * ofi_datatype_size(datatype));

	if (local < 2 * rem) {
		if (local % 2 == 0) {
			ret = util_coll_sched_send(coll_op, local + 1, result, count,
						   datatype, 1);
			if (ret)
				return ret;

			my_new_id = -1;
		} else {
			ret = util_coll_sched_recv(coll_op, local - 1,
						   tmp_buf, count, datatype, 1);
			if (ret)
				return ret;

			my_new_id = local / 2;

			ret = util_coll_sched_reduce(coll_op, tmp_buf, result,
						     count, datatype, op, 1);
			if (ret)
				return ret;
		}
	} else {
		my_new_id = local - rem;
	}

	if (my_new_id != -1) {
		while (mask < pof2) {
			next_remote = my_new_id ^ mask;
			remote = (next_remote < rem) ? next_remote * 2 + 1 :
				next_remote + rem;

			// receive remote data into tmp buf
			ret = util_coll_sched_recv(coll_op, remote, tmp_buf, count,
						   datatype, 0);
			if (ret)
				return ret;

			// send result buf, which has the current total
			ret = util_coll_sched_send(coll_op, remote, result, count,
						   datatype, 1);
			if (ret)
				return ret;

			if (remote < local) {
				// reduce received remote into result buf
				ret = util_coll_sched_reduce(coll_op, tmp_buf, result,
							     count, datatype, op, 1);
				if (ret)
					return ret;
			} else {
				// reduce local result into received data
				ret = util_coll_sched_reduce(coll_op, result, tmp_buf,
							     count, datatype, op, 1);
				if (ret)
					return ret;

				// copy total into result
				ret = util_coll_sched_copy(coll_op, tmp_buf, result,
							   count, datatype, 1);
				if (ret)
					return ret;
			}
			mask <<= 1;
		}
	}

	if (local < 2 * rem) {
		if (local % 2) {
			ret = util_coll_sched_send(coll_op, local - 1, result, count,
						   datatype, 1);
			if (ret)
				return ret;
		} else {
			ret = util_coll_sched_recv(coll_op, local + 1, result, count,
						   datatype, 1);
			if (ret)
				return ret;
		}
	}
	return FI_SUCCESS;
}

static int util_coll_allgather(struct util_coll_operation *coll_op, const void *send_buf,
			       void *result, int count, enum fi_datatype datatype)
{
	// allgather implemented using ring algorithm
	int64_t ret, i, cur_offset, next_offset;
	size_t nbytes, numranks;
	uint64_t local_rank, left_rank, right_rank;

	local_rank = coll_op->mc->local_rank;
	nbytes = ofi_datatype_size(datatype) * count;
	numranks = coll_op->mc->av_set->fi_addr_count;

	// copy the local value to the appropriate place in result buffer
	ret = util_coll_sched_copy(coll_op, (void *) send_buf,
				   (char *) result + (local_rank * nbytes), count,
				   datatype, 1);
	if (ret)
		return ret;

	// send to right, recv from left
	left_rank = (numranks + local_rank - 1) % numranks;
	right_rank = (local_rank + 1) % numranks;

	cur_offset = local_rank;
	next_offset = left_rank;

	// fill in result with data going right to left
	for (i = 1; i < numranks; i++) {
		ret = util_coll_sched_send(coll_op, right_rank,
					   (char *) result + (cur_offset * nbytes), count,
					   datatype, 0);
		if (ret)
			return ret;

		ret = util_coll_sched_recv(coll_op, left_rank,
					   (char *) result + (next_offset * nbytes),
					   count, datatype, 1);
		if (ret)
			return ret;

		cur_offset = next_offset;
		next_offset = (numranks + next_offset - 1) % numranks;
	}

	return FI_SUCCESS;
}

static size_t util_binomial_tree_values_to_recv(uint64_t rank, size_t numranks)
{
	size_t nvalues = 0x1 << (ofi_lsb(rank) - 1);
	if (numranks < rank + nvalues)
		nvalues = numranks - rank;

	return nvalues;
}

static int util_coll_scatter(struct util_coll_operation *coll_op, const void *data,
			     void *result, void **temp, size_t count, uint64_t root,
			     enum fi_datatype datatype)
{
	// scatter implemented with binomial tree algorithm
	uint64_t local_rank, relative_rank;
	size_t nbytes, numranks, send_cnt, cur_cnt = 0;
	int ret, mask, remote_rank;
	void *send_data;

	local_rank = coll_op->mc->local_rank;
	numranks = coll_op->mc->av_set->fi_addr_count;
	relative_rank = (local_rank >= root) ? local_rank - root : local_rank - root + numranks;
	nbytes = count * ofi_datatype_size(datatype);

	// check if we need to participate
	if (count == 0)
		return FI_SUCCESS;

	// non-root even nodes get a temp buffer for receiving data
	// these nodes may need to send part of what they receive
	if (relative_rank && !(relative_rank % 2)) {
		cur_cnt = count * util_binomial_tree_values_to_recv(relative_rank, numranks);
		*temp = malloc(cur_cnt * ofi_datatype_size(datatype));
		if (!*temp)
			return -FI_ENOMEM;
	}

	if (local_rank == root) {
		cur_cnt = count * numranks;
		if (root != 0) {
			// if we're root but not rank 0, we need to reorder the send buffer
			// according to destination rank. if we're rank 3, data intended for
			// ranks 0-2 will be moved to the end
			*temp = malloc(cur_cnt * ofi_datatype_size(datatype));
			if (!temp)
				return -FI_ENOMEM;
			ret = util_coll_sched_copy(coll_op,
						   (char *) data + nbytes * local_rank, *temp,
						   (numranks - local_rank) * count, datatype,
						   1);
			if (ret)
				return ret;

			ret = util_coll_sched_copy(coll_op, (char *) data,
						   (char *) *temp +
							   (numranks - local_rank) * nbytes,
						   local_rank * count, datatype, 1);
			if (ret)
				return ret;
		}
	}

	// set up all receives
	mask = 0x1;
	while (mask < numranks) {
		if (relative_rank & mask) {
			remote_rank = local_rank - mask;
			if (remote_rank < 0)
				remote_rank += numranks;

			if (relative_rank % 2) {
				// leaf node, we're receiving the actual data
				ret = util_coll_sched_recv(coll_op, remote_rank, result, count,
							   datatype, 1);
				if (ret)
					return ret;
			} else {
				// branch node, we're receiving data which we've got to forward
				ret = util_coll_sched_recv(coll_op, remote_rank, *temp,
							   cur_cnt, datatype, 1);
				if (ret)
					return ret;
			}
			break;
		}
		mask <<= 1;
	}

	// set up all sends
	send_data = root == local_rank && root == 0 ? (void *) data : *temp;
	mask >>= 1;
	while (mask > 0) {
		if (relative_rank + mask < numranks) {
			// to this point, cur_cnt has represented the number of values
			// to expect to store in our data buf
			// from here on, cur_cnt is the number of values we have left to
			// forward from the data buf
			send_cnt = cur_cnt - count * mask;

			remote_rank = local_rank + mask;
			if (remote_rank >= numranks)
				remote_rank -= numranks;

			FI_DBG(coll_op->mc->av_set->av->prov, FI_LOG_CQ,
			       "MASK: 0x%0x CUR_CNT: %ld SENDING: %ld TO: %d\n", mask,
			       cur_cnt, send_cnt, remote_rank);

			assert(send_cnt > 0);

			ret = util_coll_sched_send(coll_op, remote_rank,
							(char *) send_data +
								nbytes * mask,
							send_cnt, datatype, 1);
			if (ret)
				return ret;

			cur_cnt -= send_cnt;
		}
		mask >>= 1;
	}

	if (!(relative_rank % 2)) {
		// for the root and all even nodes, we've got to copy
		// our local data to the result buffer
		ret = util_coll_sched_copy(coll_op, send_data, result, count, datatype, 1);
	}

	return FI_SUCCESS;
}

static int util_coll_close(struct fid *fid)
{
	struct util_coll_mc *coll_mc;

	coll_mc = container_of(fid, struct util_coll_mc, mc_fid.fid);

	ofi_atomic_dec32(&coll_mc->av_set->ref);
	free(coll_mc);

	return FI_SUCCESS;
}

static struct fi_ops util_coll_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = util_coll_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static inline void util_coll_mc_init(struct util_coll_mc *coll_mc,
				   struct util_av_set *av_set,
				   struct fid_ep *ep, void *context)
{
	coll_mc->mc_fid.fid.fclass = FI_CLASS_MC;
	coll_mc->mc_fid.fid.context = context;
	coll_mc->mc_fid.fid.ops = &util_coll_fi_ops;
	coll_mc->mc_fid.fi_addr = (uintptr_t) coll_mc;
	coll_mc->ep = ep;
	assert(av_set != NULL);
	ofi_atomic_inc32(&av_set->ref);
	coll_mc->av_set = av_set;
}

static int ofi_av_set_addr(struct fid_av_set *set, fi_addr_t *coll_addr)
{
	struct util_av_set *av_set;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	*coll_addr = (uintptr_t) &av_set->coll_mc;

	return FI_SUCCESS;
}

/* TODO: Figure out requirements for using collectives.
 * e.g. require local address to be in AV?
 * Determine best way to handle first join request
 */
static int util_coll_find_local_rank(struct fid_ep *ep, struct util_coll_mc *coll_mc)
{
	size_t addrlen;
	char *addr;
	int ret;
	fi_addr_t my_addr;
	int i;

	addrlen = 0;
	ret = fi_getname(&ep->fid, NULL, &addrlen);
	if (ret != FI_SUCCESS && addrlen == 0)
		return ret;

	addr = calloc(1, addrlen);
	if (!addr)
		return -FI_ENOMEM;

	ret = fi_getname(&ep->fid, addr, &addrlen);
	if (ret) {
		free(addr);
		return ret;
	}
	my_addr = ofi_av_lookup_fi_addr(coll_mc->av_set->av, addr);

	coll_mc->local_rank = FI_ADDR_NOTAVAIL;
	if (my_addr != FI_ADDR_NOTAVAIL) {
		for (i=0; i<coll_mc->av_set->fi_addr_count; i++)
			if (coll_mc->av_set->fi_addr_array[i] == my_addr) {
				coll_mc->local_rank = i;
				break;
			}
	}

	free(addr);

	return FI_SUCCESS;
}

void util_coll_join_comp(struct util_coll_operation *coll_op)
{
	struct fi_eq_entry entry;
	struct util_ep *ep = container_of(coll_op->mc->ep, struct util_ep, ep_fid);

	coll_op->data.join.new_mc->seq = 0;
	coll_op->data.join.new_mc->group_id = ofi_bitmask_get_lsbset(coll_op->data.join.data);
	// mark the local mask bit
	ofi_bitmask_unset(ep->coll_cid_mask, coll_op->data.join.new_mc->group_id);

	/* write to the eq  */
	memset(&entry, 0, sizeof(entry));
	entry.fid = &coll_op->mc->mc_fid.fid;
	entry.context = coll_op->context;

	if (ofi_eq_write(&ep->eq->eq_fid, FI_JOIN_COMPLETE, &entry,
			 sizeof(struct fi_eq_entry), FI_COLLECTIVE) < 0)
		FI_WARN(ep->domain->fabric->prov, FI_LOG_DOMAIN,
			"join collective - eq write failed\n");

	ofi_bitmask_free(&coll_op->data.join.data);
	ofi_bitmask_free(&coll_op->data.join.tmp);
}

void util_coll_collective_comp(struct util_coll_operation *coll_op)
{
	struct util_ep *ep;

	ep = container_of(coll_op->mc->ep, struct util_ep, ep_fid);

	if (ofi_cq_write(ep->tx_cq, coll_op->context, FI_COLLECTIVE, 0, 0, 0, 0))
		FI_WARN(ep->domain->fabric->prov, FI_LOG_DOMAIN,
			"barrier collective - cq write failed\n");

	switch (coll_op->type) {
	case UTIL_COLL_ALLREDUCE_OP:
		free(coll_op->data.allreduce.data);
		break;
	case UTIL_COLL_SCATTER_OP:
		free(coll_op->data.scatter);
		break;
	case UTIL_COLL_BROADCAST_OP:
		free(coll_op->data.broadcast.chunk);
		free(coll_op->data.broadcast.scatter);
		break;
	case UTIL_COLL_JOIN_OP:
	case UTIL_COLL_BARRIER_OP:
	case UTIL_COLL_ALLGATHER_OP:
	default:
		//nothing to clean up
		break;
	}
}

static int util_coll_proc_reduce_item(struct util_coll_reduce_item *reduce_item)
{
	if (FI_MIN <= reduce_item->op && FI_BXOR >= reduce_item->op) {
		ofi_atomic_write_handlers[reduce_item->op]
					 [reduce_item->datatype](
						 reduce_item->inout_buf,
						 reduce_item->in_buf,
						 reduce_item->count);
	} else {
		return -FI_ENOSYS;
	}
	return FI_SUCCESS;
}

int util_coll_process_xfer_item(struct util_coll_xfer_item *item) {
	struct iovec iov;
	struct fi_msg_tagged msg;
	struct util_coll_mc *mc = item->hdr.coll_op->mc;
	int ret;

	msg.msg_iov = &iov;
	msg.desc = NULL;
	msg.iov_count = 1;
	msg.ignore = 0;
	msg.context = item;
	msg.data = 0;
	msg.tag = item->tag;
	msg.addr = mc->av_set->fi_addr_array[item->remote_rank];

	iov.iov_base = item->buf;
	iov.iov_len = (item->count * ofi_datatype_size(item->datatype));

	if (item->hdr.type == UTIL_COLL_SEND) {
		ret = fi_tsendmsg(mc->ep, &msg, FI_COLLECTIVE);
		if (!ret)
			FI_DBG(mc->av_set->av->prov, FI_LOG_CQ,
			       "%p SEND [0x%02lx] -> [0x%02x] cnt: %d sz: %ld\n", item,
			       item->hdr.coll_op->mc->local_rank, item->remote_rank,
			       item->count,
			       item->count * ofi_datatype_size(item->datatype));
		return ret;
	} else if (item->hdr.type == UTIL_COLL_RECV) {
		ret = fi_trecvmsg(mc->ep, &msg, FI_COLLECTIVE);
		if (!ret)
			FI_DBG(mc->av_set->av->prov, FI_LOG_CQ,
			       "%p RECV [0x%02lx] <- [0x%02x] cnt: %d sz: %ld\n", item,
			       item->hdr.coll_op->mc->local_rank, item->remote_rank,
			       item->count,
			       item->count * ofi_datatype_size(item->datatype));
		return ret;
	}

	return -FI_ENOSYS;
}

int ofi_coll_ep_progress(struct fid_ep *ep)
{
	struct util_coll_work_item *work_item;
	struct util_coll_reduce_item *reduce_item;
	struct util_coll_copy_item *copy_item;
	struct util_coll_xfer_item *xfer_item;
	struct util_coll_operation *coll_op;
	struct util_ep *util_ep;
	int ret;

	util_ep  = container_of(ep, struct util_ep, ep_fid);

	while (!slist_empty(&util_ep->coll_ready_queue)) {
		slist_remove_head_container(&util_ep->coll_ready_queue,
					    struct util_coll_work_item, work_item,
					    ready_entry);
		coll_op = work_item->coll_op;
		switch (work_item->type) {
		case UTIL_COLL_SEND:
			xfer_item = container_of(work_item, struct util_coll_xfer_item, hdr);
			ret = util_coll_process_xfer_item(xfer_item);
			if (ret && ret == -FI_EAGAIN) {
				slist_insert_tail(&work_item->ready_entry,
						  &util_ep->coll_ready_queue);
				goto out;
			}
			break;
		case UTIL_COLL_RECV:
			xfer_item = container_of(work_item, struct util_coll_xfer_item, hdr);
			ret = util_coll_process_xfer_item(xfer_item);
			if (ret)
				goto out;
			break;
		case UTIL_COLL_REDUCE:
			reduce_item = container_of(work_item, struct util_coll_reduce_item, hdr);
			ret = util_coll_proc_reduce_item(reduce_item);
			if (ret)
				goto out;

			reduce_item->hdr.state = UTIL_COLL_COMPLETE;
			break;
		case UTIL_COLL_COPY:
			copy_item = container_of(work_item, struct util_coll_copy_item, hdr);
			memcpy(copy_item->out_buf, copy_item->in_buf,
			       copy_item->count * ofi_datatype_size(copy_item->datatype));

			copy_item->hdr.state = UTIL_COLL_COMPLETE;
			break;
		case UTIL_COLL_COMP:
			if (work_item->coll_op->comp_fn)
				work_item->coll_op->comp_fn(work_item->coll_op);

			work_item->state = UTIL_COLL_COMPLETE;
			break;
		default:
			ret = FI_ENOSYS;
			goto out;
		}

		util_coll_op_progress_work(util_ep, coll_op);
	}

	ret = FI_SUCCESS;

out:
	return ret;
}

int ofi_join_collective(struct fid_ep *ep, fi_addr_t coll_addr,
		       const struct fid_av_set *set,
		       uint64_t flags, struct fid_mc **mc, void *context)
{
	struct util_coll_mc *new_coll_mc;
	struct util_av_set *av_set;
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *join_op;
	struct util_ep *util_ep;
	int ret;

	av_set = container_of(set, struct util_av_set, av_set_fid);

	if (coll_addr == FI_ADDR_NOTAVAIL) {
		assert(av_set->av->coll_mc != NULL);
		coll_mc = av_set->av->coll_mc;
	} else {
		coll_mc = (struct util_coll_mc*) ((uintptr_t) coll_addr);
	}

	new_coll_mc = calloc(1, sizeof(*new_coll_mc));
	if (!new_coll_mc)
		return -FI_ENOMEM;

	// set up the new mc for future collectives
	util_coll_mc_init(new_coll_mc, av_set, ep, context);

	coll_mc->ep = ep;

	util_ep = container_of(ep, struct util_ep, ep_fid);

	/* get the rank */
	util_coll_find_local_rank(ep, new_coll_mc);
	util_coll_find_local_rank(ep, coll_mc);

	ret = util_coll_op_create(&join_op, coll_mc, UTIL_COLL_JOIN_OP, context,
				util_coll_join_comp);
	if (ret)
		goto err1;

	join_op->data.join.new_mc = new_coll_mc;

	ret = ofi_bitmask_create(&join_op->data.join.data, OFI_MAX_GROUP_ID);
	if (ret)
		goto err2;

	ret = ofi_bitmask_create(&join_op->data.join.tmp, OFI_MAX_GROUP_ID);
	if (ret)
		goto err3;

	ret = util_coll_allreduce(join_op, util_ep->coll_cid_mask->bytes,
				  join_op->data.join.data.bytes,
				  join_op->data.join.tmp.bytes,
				  ofi_bitmask_bytesize(util_ep->coll_cid_mask),
				  FI_UINT8, FI_BAND);
	if (ret)
		goto err4;

	ret = util_coll_sched_comp(join_op);
	if (ret)
		goto err4;

	util_coll_op_progress_work(util_ep, join_op);

	*mc = &new_coll_mc->mc_fid;
	return FI_SUCCESS;
err4:
	ofi_bitmask_free(&join_op->data.join.tmp);
err3:
	ofi_bitmask_free(&join_op->data.join.data);
err2:
	free(join_op);
err1:
	free(new_coll_mc);
	return ret;
}

static struct fi_ops_av_set util_av_set_ops= {
	.set_union	=	ofi_av_set_union,
	.intersect	=	ofi_av_set_intersect,
	.diff		=	ofi_av_set_diff,
	.insert		=	ofi_av_set_insert,
	.remove		=	ofi_av_set_remove,
	.addr		=	ofi_av_set_addr
};

static int util_coll_copy_from_av(struct util_av *av, void *addr,
			      fi_addr_t fi_addr, void *arg)
{
	struct util_av_set *av_set = (struct util_av_set *) arg;
	av_set->fi_addr_array[av_set->fi_addr_count++] = fi_addr;
	return FI_SUCCESS;
}

static int util_av_set_close(struct fid *fid)
{
	struct util_av_set *av_set;

	av_set = container_of(fid, struct util_av_set, av_set_fid.fid);

	// release reference taken by internal coll_mc embedded in av_set
	ofi_atomic_dec32(&av_set->ref);
	if (ofi_atomic_get32(&av_set->ref) > 0)
		return -FI_EBUSY;

	free(av_set);

	return FI_SUCCESS;
}

static struct fi_ops util_av_set_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = util_av_set_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static inline int util_av_set_init(struct util_av_set *av_set,
				   struct util_av *util_av,
				   void *context)
{
	int ret = FI_SUCCESS;

	av_set->av_set_fid.ops = &util_av_set_ops;
	av_set->av_set_fid.fid.fclass = FI_CLASS_AV_SET;
	av_set->av_set_fid.fid.context = context;
	av_set->av_set_fid.fid.ops = &util_av_set_fi_ops;
	av_set->av = util_av;
	ofi_atomic_initialize32(&av_set->ref, 0);
	ret = fastlock_init(&av_set->lock);

	return ret;
}

static int util_coll_av_init(struct util_av *av)
{
	struct util_coll_mc *coll_mc;
	int ret;

	assert(!av->coll_mc);

	coll_mc = calloc(1, sizeof(*coll_mc));
	if (!coll_mc)
		return -FI_ENOMEM;

	coll_mc->av_set = calloc(1, sizeof(*coll_mc->av_set));
	if (!coll_mc->av_set) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	ret = util_av_set_init(coll_mc->av_set, av, NULL);
	if (ret)
		goto err3;

	coll_mc->av_set->fi_addr_array =
		calloc(av->count, sizeof(*coll_mc->av_set->fi_addr_array));
	if (!coll_mc->av_set->fi_addr_array) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	ret = ofi_av_elements_iter(av, util_coll_copy_from_av,
				   (void *)coll_mc->av_set);
	if (ret)
		goto err4;

	util_coll_mc_init(coll_mc, coll_mc->av_set, NULL, NULL);

	av->coll_mc = coll_mc;
	return FI_SUCCESS;

err4:
	fastlock_destroy(&coll_mc->av_set->lock);
err3:
	free(coll_mc->av_set->fi_addr_array);
err2:
	free(coll_mc->av_set);
err1:
	free(coll_mc);
	return ret;
}

int ofi_av_set(struct fid_av *av, struct fi_av_set_attr *attr,
	       struct fid_av_set **av_set_fid, void * context)
{
	struct util_av *util_av = container_of(av, struct util_av, av_fid);
	struct util_av_set *av_set;
	int ret, iter;

	if (!util_av->coll_mc) {
		ret = util_coll_av_init(util_av);
		if (ret)
			return ret;
	}

	av_set = calloc(1,sizeof(*av_set));
	if (!av_set)
		return -FI_ENOMEM;

	ret = util_av_set_init(av_set, util_av, context);
	if (ret)
		goto err1;

	av_set->fi_addr_array = calloc(util_av->count, sizeof(*av_set->fi_addr_array));
	if (!av_set->fi_addr_array)
		goto err2;

	for (iter = 0; iter < attr->count; iter++) {
		av_set->fi_addr_array[iter] =
			util_av->coll_mc->av_set->fi_addr_array[iter * attr->stride];
		av_set->fi_addr_count++;
	}

	util_coll_mc_init(&av_set->coll_mc, av_set, NULL, context);

	(*av_set_fid) = &av_set->av_set_fid;
	return FI_SUCCESS;
err2:
	fastlock_destroy(&av_set->lock);
err1:
	free(av_set);
	return ret;
}

ssize_t ofi_ep_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context)
{
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *barrier_op;
	struct util_ep *util_ep;
	uint64_t send;
	int ret;

	coll_mc = (struct util_coll_mc*) ((uintptr_t) coll_addr);

	ret = util_coll_op_create(&barrier_op, coll_mc, UTIL_COLL_BARRIER_OP, context,
			  util_coll_collective_comp);
	if (ret)
		return ret;

	send = ~barrier_op->mc->local_rank;
	ret = util_coll_allreduce(barrier_op, &send, &barrier_op->data.barrier.data,
				  &barrier_op->data.barrier.tmp, 1, FI_UINT64, FI_BAND);
	if (ret)
		goto err1;

	ret = util_coll_sched_comp(barrier_op);
	if (ret)
		goto err1;

	util_ep = container_of(ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, barrier_op);

	return FI_SUCCESS;
err1:
	free(barrier_op);
	return ret;
}

ssize_t ofi_ep_allreduce(struct fid_ep *ep, const void *buf, size_t count, void *desc,
			 void *result, void *result_desc, fi_addr_t coll_addr,
			 enum fi_datatype datatype, enum fi_op op, uint64_t flags,
			 void *context)
{
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *allreduce_op;
	struct util_ep *util_ep;
	int ret;

	coll_mc = (struct util_coll_mc *) ((uintptr_t) coll_addr);
	ret = util_coll_op_create(&allreduce_op, coll_mc, UTIL_COLL_ALLREDUCE_OP, context,
				  util_coll_collective_comp);
	if (ret)
		return ret;


	allreduce_op->data.allreduce.size = count * ofi_datatype_size(datatype);
	allreduce_op->data.allreduce.data = calloc(count, ofi_datatype_size(datatype));
	if (!allreduce_op->data.allreduce.data)
		goto err1;

	ret = util_coll_allreduce(allreduce_op, buf, result, allreduce_op->data.allreduce.data, count,
				  datatype, op);
	if (ret)
		goto err2;

	ret = util_coll_sched_comp(allreduce_op);
	if (ret)
		goto err2;

	util_ep = container_of(ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, allreduce_op);

	return FI_SUCCESS;
err2:
	free(allreduce_op->data.allreduce.data);
err1:
	free(allreduce_op);
	return ret;
}

ssize_t ofi_ep_allgather(struct fid_ep *ep, const void *buf, size_t count, void *desc,
			 void *result, void *result_desc, fi_addr_t coll_addr,
			 enum fi_datatype datatype, uint64_t flags, void *context)
{
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *allgather_op;
	struct util_ep *util_ep;
	int ret;

	coll_mc = (struct util_coll_mc *) ((uintptr_t) coll_addr);
	ret = util_coll_op_create(&allgather_op, coll_mc, UTIL_COLL_ALLGATHER_OP, context,
				  util_coll_collective_comp);
	if (ret)
		return ret;

	ret = util_coll_allgather(allgather_op, buf, result, count, datatype);
	if (ret)
		goto err;

	ret = util_coll_sched_comp(allgather_op);
	if (ret)
		goto err;

	util_ep = container_of(ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, allgather_op);

	return FI_SUCCESS;
err:
	free(allgather_op);
	return ret;
}

ssize_t ofi_ep_scatter(struct fid_ep *ep, const void *buf, size_t count, void *desc,
		       void *result, void *result_desc, fi_addr_t coll_addr,
		       fi_addr_t root_addr, enum fi_datatype datatype, uint64_t flags,
		       void *context)
{
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *scatter_op;
	struct util_ep *util_ep;
	int ret;

	coll_mc = (struct util_coll_mc *) ((uintptr_t) coll_addr);
	ret = util_coll_op_create(&scatter_op, coll_mc, UTIL_COLL_SCATTER_OP, context,
				  util_coll_collective_comp);
	if (ret)
		return ret;

	ret = util_coll_scatter(scatter_op, buf, result, &scatter_op->data.scatter, count, root_addr, datatype);
	if (ret)
		goto err;

	ret = util_coll_sched_comp(scatter_op);
	if (ret)
		goto err;

	util_ep = container_of(ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, scatter_op);

	return FI_SUCCESS;
err:
	free(scatter_op);
	return ret;
}

ssize_t ofi_ep_broadcast(struct fid_ep *ep, void *buf, size_t count, void *desc,
			 fi_addr_t coll_addr, fi_addr_t root_addr,
			 enum fi_datatype datatype, uint64_t flags, void *context)
{
	struct util_coll_mc *coll_mc;
	struct util_coll_operation *broadcast_op;
	struct util_ep *util_ep;
	int ret, chunk_cnt, numranks, local;

	coll_mc = (struct util_coll_mc *) ((uintptr_t) coll_addr);
	ret = util_coll_op_create(&broadcast_op, coll_mc, UTIL_COLL_BROADCAST_OP, context,
				  util_coll_collective_comp);
	if (ret)
		return ret;

	local = broadcast_op->mc->local_rank;
	numranks = broadcast_op->mc->av_set->fi_addr_count;
	chunk_cnt = (count + numranks - 1) / numranks;
	if (chunk_cnt * local > count && chunk_cnt * local - (int) count > chunk_cnt)
		chunk_cnt = 0;

	broadcast_op->data.broadcast.chunk = malloc(chunk_cnt * ofi_datatype_size(datatype));
	if (!broadcast_op->data.broadcast.chunk) {
		ret = -FI_ENOMEM;
		goto err1;
	}

	ret = util_coll_scatter(broadcast_op, buf, broadcast_op->data.broadcast.chunk,
				&broadcast_op->data.broadcast.scatter, chunk_cnt,
				root_addr, datatype);
	if (ret)
		goto err2;

	ret = util_coll_allgather(broadcast_op, broadcast_op->data.broadcast.chunk, buf,
				  chunk_cnt, datatype);
	if (ret)
		goto err2;

	ret = util_coll_sched_comp(broadcast_op);
	if (ret)
		goto err2;

	util_ep = container_of(ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, broadcast_op);

	return FI_SUCCESS;
err2:
	free(broadcast_op->data.broadcast.chunk);
err1:
	free(broadcast_op);
	return ret;
}

void ofi_coll_handle_xfer_comp(uint64_t tag, void *ctx)
{
	struct util_ep *util_ep;
	struct util_coll_xfer_item *xfer_item = (struct util_coll_xfer_item *) ctx;
	xfer_item->hdr.state = UTIL_COLL_COMPLETE;

	FI_DBG(xfer_item->hdr.coll_op->mc->av_set->av->prov, FI_LOG_CQ,
	       "\tXfer complete: { %p %s Remote: 0x%02x Local: 0x%02lx cnt: %d typesize: %ld }\n",
	       xfer_item, xfer_item->hdr.type == UTIL_COLL_SEND ? "SEND" : "RECV",
	       xfer_item->remote_rank, xfer_item->hdr.coll_op->mc->local_rank,
	       xfer_item->count, ofi_datatype_size(xfer_item->datatype));
	util_ep = container_of(xfer_item->hdr.coll_op->mc->ep, struct util_ep, ep_fid);
	util_coll_op_progress_work(util_ep, xfer_item->hdr.coll_op);
}

int ofi_query_collective(struct fid_domain *domain, enum fi_collective_op coll,
				struct fi_collective_attr *attr, uint64_t flags)
{
	int ret;

	if (!attr || attr->mode != 0)
		return -FI_EINVAL;

	switch (coll) {
	case FI_BARRIER:
	case FI_ALLGATHER:
	case FI_SCATTER:
	case FI_BROADCAST:
		ret = FI_SUCCESS;
		break;
	case FI_ALLREDUCE:
		if (FI_MIN <= attr->op && FI_BXOR >= attr->op)
			ret = fi_query_atomic(domain, attr->datatype, attr->op,
					      &attr->datatype_attr, flags);
		else
			return -FI_ENOSYS;
		break;
	case FI_ALLTOALL:
	case FI_REDUCE_SCATTER:
	case FI_REDUCE:
	case FI_GATHER:
	default:
		return -FI_ENOSYS;
	}

	if (ret)
		return ret;

	// with the currently implemented software based collective operations
	// the only restriction is the number of ranks we can address, as limited
	// by the size of the rank portion of the collective tag, which is 31 bits.
	// future collectives may impose further restrictions which will need to update
	// the calculation.  For example, operations which require dedicated space in
	// the recieve buffer for each rank would limit the number of members by buffer
	// size and value type (8kB buffer / 64B value = 128 member max).
	// hardware may impose further restrictions
	attr->max_members = ~(0x80000000);

	return FI_SUCCESS;
}
