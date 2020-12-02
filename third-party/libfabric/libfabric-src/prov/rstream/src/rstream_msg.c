/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"
#include <math.h>
#include <sys/time.h>


ssize_t rstream_process_cq(struct rstream_ep *ep, enum rstream_msg_type type);

static uint32_t rstream_cq_data_get_len(uint32_t cq_data)
{
	return (cq_data & RSTREAM_MR_LEN_MASK);
}

static uint32_t rstream_cq_data_set(struct rstream_cq_data cq_data)
{
	uint32_t credits = cq_data.num_completions;

	assert(cq_data.num_completions < RSTREAM_CREDITS_MAX);
	assert(cq_data.total_len < RSTREAM_MR_MAX);

	credits = credits << RSTREAM_CREDIT_OFFSET;
	return credits | cq_data.total_len;
}

static uint16_t rstream_cq_data_get_credits(uint32_t cq_data)
{
	uint32_t credits = cq_data & RSTREAM_CREDIT_MASK;

	credits = (credits >> RSTREAM_CREDIT_OFFSET);
	assert(credits < RSTREAM_CREDITS_MAX);

	return credits;
}

static uint32_t rstream_iwarp_cq_data_is_msg(uint32_t cq_data) {
	return cq_data & RSTREAM_IWARP_MSG_BIT;
}


static uint32_t rstream_iwarp_cq_data_set_msg_len(uint32_t msg_len)
{
	assert(msg_len < RSTREAM_IWARP_IMM_MSG_LEN);

	uint32_t cq_data = msg_len;

	return cq_data | RSTREAM_IWARP_MSG_BIT;
}

static uint32_t rstream_iwarp_cq_data_get_msg_len(uint32_t cq_data)
{
	uint32_t msg_len = cq_data & RSTREAM_IWARP_MSG_BIT_MASK;

	assert(msg_len < RSTREAM_IWARP_IMM_MSG_LEN);

	return msg_len;
}

static char *rstream_get_next_recv_buffer(struct rstream_ep *ep)
{
	char *base_ptr = (char *)ep->local_mr.tx.data_start +
		ep->local_mr.tx.size;
	uint64_t *offset = &ep->local_mr.recv_buffer_offset;
	const uint32_t full_size = RSTREAM_IWARP_DATA_SIZE *
		ep->qp_win.max_rx_credits;
	char *buffer = base_ptr + *offset;

	assert((void *)buffer < ep->local_mr.rx.data_start);
	*offset = (*offset + RSTREAM_IWARP_DATA_SIZE) % full_size;

	return buffer;
}

/*assuming rx_ctxs are always fully used */
static struct fi_context *rstream_get_rx_ctx(struct rstream_ep *ep)
{
	struct fi_context *ctx;

	if (ep->rx_ctx_index == ep->qp_win.max_rx_credits)
		return NULL;

	ctx = &ep->rx_ctxs[ep->rx_ctx_index];
	ep->rx_ctx_index = ep->rx_ctx_index + 1;

	return ctx;
}

static struct fi_context *rstream_get_tx_ctx(struct rstream_ep *ep, int len)
{
	struct rstream_tx_ctx_fs *fs = ep->tx_ctxs;
	struct rstream_ctx_data *rtn_ctx = freestack_pop(fs);

	if (!rtn_ctx)
		return NULL;

	rtn_ctx->len = len;
	return &rtn_ctx->ctx;
}

static int rstream_return_tx_ctx(struct fi_context *ctx_ptr,
	struct rstream_ep *ep)
{
	int len;
	struct rstream_tx_ctx_fs *fs = ep->tx_ctxs;

	struct rstream_ctx_data *ctx_data = (struct rstream_ctx_data *)ctx_ptr;
	len = ctx_data->len;
	freestack_push(fs, ctx_data);

	return len;
}

static ssize_t rstream_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
	fi_addr_t dest_addr)
{
	return -FI_ENOSYS;
}

static ssize_t rstream_print_cq_error(struct fid_cq *cq)
{
	ssize_t ret;
	struct fi_cq_err_entry cq_entry = {0};
	const char *errmsg;

	ret = fi_cq_readerr(cq, &cq_entry, 0);
	if (cq_entry.err == FI_ENOMSG) {
		ret = FI_ENOMSG;
		return ret;
	}

	errmsg = fi_cq_strerror(cq, cq_entry.prov_errno,
		cq_entry.err_data, NULL, 0);
	fprintf(stderr, "CQ error msg: %s\n", errmsg);

	return ret;
}

static void rstream_update_tx_credits(struct rstream_ep *ep,
	uint16_t num_completions)
{
	assert(num_completions == 1);

	if(ep->qp_win.ctrl_credits < RSTREAM_MAX_CTRL)
		ep->qp_win.ctrl_credits++;
	else
		ep->qp_win.tx_credits++;

	assert(ep->qp_win.tx_credits <= ep->qp_win.max_tx_credits);
}

static int rstream_timer_completed(struct rstream_timer *timer)
{
	if (!timer->poll_time)
		gettimeofday(&timer->start, NULL);

	gettimeofday(&timer->end, NULL);
	timer->poll_time = (timer->end.tv_sec - timer->start.tv_sec) * 1000000 +
		(timer->end.tv_usec - timer->start.tv_usec);

	return (timer->poll_time > RSTREAM_MAX_POLL_TIME);
}

static int rstream_tx_mr_full(struct rstream_ep *ep)
{
	return !(ep->local_mr.tx.avail_size);
}

static int rstream_target_mr_full(struct rstream_ep *ep)
{
	return !(ep->remote_data.mr.avail_size);
}

static int rstream_tx_full(struct rstream_ep *ep)
{
	return (ep->qp_win.tx_credits == 0);
}

static int rstream_target_rx_full(struct rstream_ep *ep)
{
	return ((ep->qp_win.target_rx_credits - RSTREAM_MAX_CTRL) == 0);
}

static uint32_t rstream_calc_contig_len(struct rstream_mr_seg *mr)
{
	if (!mr->avail_size) {
		assert(mr->start_offset == mr->end_offset);
		return 0;
	} else if (mr->start_offset < mr->end_offset) {
		return (mr->end_offset - mr->start_offset);
	} else {
		return (mr->size - mr->start_offset);
	}
}

static uint32_t rstream_alloc_contig_len_available(struct rstream_mr_seg *mr,
	char **data_addr, uint32_t req_len)
{
	uint32_t len_available = rstream_calc_contig_len(mr);
	uint32_t len;

	*data_addr = (char *)mr->data_start;
	assert(len_available <= mr->avail_size);

	if (!len_available)
		return 0;

	*data_addr = *data_addr + mr->start_offset;
	len = (len_available <	req_len) ? len_available : req_len;
	assert(mr->avail_size >= len);
	mr->avail_size = mr->avail_size - len;
	mr->start_offset = (mr->start_offset + len) % mr->size;

	return len;
}

static void rstream_free_contig_len(struct rstream_mr_seg *mr, uint32_t len)
{
	assert((mr->avail_size + len) <= mr->size);
	mr->avail_size = mr->avail_size + len;
	mr->end_offset = (mr->end_offset + len) % mr->size;
}

static ssize_t rstream_send_ctrl_msg(struct rstream_ep *ep, uint32_t cq_data)
{
	ssize_t ret = 0;
	struct fi_msg msg;

	if (!ep->qp_win.ctrl_credits || (ep->qp_win.target_rx_credits == 0)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	if (RSTREAM_USING_IWARP) {
		ret = fi_inject(ep->ep_fd, &cq_data, RSTREAM_IWARP_DATA_SIZE, 0);
		if (ret != 0)
			goto out;
	} else {
		msg.msg_iov = NULL;
		msg.desc = NULL;
		msg.iov_count = 0;
		msg.context = rstream_get_tx_ctx(ep, 0);
		msg.data = cq_data;

		ret = fi_sendmsg(ep->ep_fd, &msg, FI_REMOTE_CQ_DATA);
		if (ret != 0)
			goto out;

		if (ep->qp_win.tx_credits > 0)
			ep->qp_win.tx_credits--;
		else
			ep->qp_win.ctrl_credits--;
	}

	assert(ep->qp_win.target_rx_credits > 0);
	ep->qp_win.target_rx_credits--;

out:
	return ret;
}

/* accumulate data in tx_cq exhaustion case */
static ssize_t rstream_update_target(struct rstream_ep *ep,
	uint16_t num_completions, uint32_t len)
{
	uint32_t cq_data;
	ssize_t ret = 0;

	ep->rx_cq_data.num_completions =
		ep->rx_cq_data.num_completions + num_completions;
	ep->rx_cq_data.total_len = ep->rx_cq_data.total_len + len;

	if ((ep->rx_cq_data.num_completions >= ep->qp_win.max_rx_credits / 2) ||
		(ep->rx_cq_data.total_len >= ep->local_mr.rx.size / 2)) {

		cq_data = rstream_cq_data_set(ep->rx_cq_data);

		ret = rstream_send_ctrl_msg(ep, cq_data);
		if (ret == 0) {
			FI_DBG(&rstream_prov, FI_LOG_EP_CTRL,
				"ctrl msg update %u = completions %u = len \n",
				ep->rx_cq_data.num_completions,
				ep->rx_cq_data.total_len);
			ep->rx_cq_data.num_completions = 0;
			ep->rx_cq_data.total_len = 0;
		}
	}

	return ret;
}

ssize_t rstream_process_rx_cq_data(struct rstream_ep *ep,
	const struct fi_cq_data_entry *cq_entry)
{
	uint16_t recvd_credits;
	uint32_t recvd_len;

	if (cq_entry->data != 0) {
		recvd_credits = rstream_cq_data_get_credits(cq_entry->data);
		recvd_len = rstream_cq_data_get_len(cq_entry->data);

		ep->qp_win.target_rx_credits += recvd_credits;
		assert(ep->qp_win.target_rx_credits <=
			ep->qp_win.max_target_rx_credits);

		rstream_free_contig_len(&ep->remote_data.mr, recvd_len);
		FI_DBG(&rstream_prov, FI_LOG_EP_CTRL,
			"recvd: ctrl msg %u = completions %u = len \n",
			recvd_credits, recvd_len);
	} else {
		rstream_free_contig_len(&ep->local_mr.rx, cq_entry->len);
	}

	return rstream_post_cq_data_recv(ep, cq_entry);
}

static void format_iwarp_cq_data(struct rstream_ep *ep,
	struct fi_cq_data_entry *cq_entry)
{
	uint32_t cq_data;

	cq_entry->buf = rstream_get_next_recv_buffer(ep);
	cq_data = *((uint32_t *)cq_entry->buf);

	if(rstream_iwarp_cq_data_is_msg(cq_data)) {
		cq_entry->data = 0;
		cq_entry->len = rstream_iwarp_cq_data_get_msg_len(cq_data);
	} else {
		cq_entry->data = cq_data;
		cq_entry->len = 0;
	}
}

static enum rstream_msg_type rstream_cqe_msg_type(struct rstream_ep *ep,
	struct fi_cq_data_entry *cq_entry)
{
	enum rstream_msg_type type = RSTREAM_MSG_UNKNOWN;

	if (cq_entry->flags & FI_REMOTE_WRITE || cq_entry->flags & FI_RECV ||
		cq_entry->flags & FI_REMOTE_CQ_DATA) {
		if (RSTREAM_USING_IWARP)
			format_iwarp_cq_data(ep, cq_entry);

		if (cq_entry->data) {
			type = RSTREAM_CTRL_MSG;
		} else {
			type = RSTREAM_RX_MSG_COMP;
		}
	} else if (cq_entry->flags & FI_WRITE || cq_entry->flags & FI_SEND) {
		type = RSTREAM_TX_MSG_COMP;
	}

	return type;
}

static ssize_t rstream_check_cq(struct rstream_ep *ep,
	struct fi_cq_data_entry *completion_entry)
{
	const int max_num = 1;
	ssize_t ret;

	ret = fi_cq_read(ep->cq, completion_entry, max_num);
	if (ret < 0 && ret != -FI_EAGAIN) {
		if (ret == -FI_EAVAIL) {
			ret = rstream_print_cq_error(ep->cq);
			fprintf(stderr, "error from %s:%d\n", __FILE__, __LINE__);
			return ret;
		}
	}
	assert(ret == -FI_EAGAIN || ret == max_num);

	return ret;
}

ssize_t rstream_process_cq(struct rstream_ep *ep, enum rstream_msg_type type)
{
	struct fi_cq_data_entry cq_entry;
	ssize_t ret, data_ret;
	ssize_t found_msg_type = 0;
	uint16_t rx_completions = 0;
	struct rstream_timer timer = {.poll_time = 0};
	enum rstream_msg_type comp_type;
	int len;

	fastlock_acquire(&ep->cq_lock);
	do {
		ret = rstream_check_cq(ep, &cq_entry);
		if (ret == 1) {
			comp_type = rstream_cqe_msg_type(ep, &cq_entry);

			if (comp_type == type)
				found_msg_type++;

			if (comp_type == RSTREAM_CTRL_MSG ||
				comp_type == RSTREAM_RX_MSG_COMP) {
				data_ret = rstream_process_rx_cq_data(ep, &cq_entry);
				if (data_ret) {
					fprintf(stderr, "error from %s:%d\n",
						__FILE__, __LINE__);
					ret = data_ret;
					goto out;
				}
				rx_completions++;
			} else if (comp_type == RSTREAM_TX_MSG_COMP) {
				len = rstream_return_tx_ctx(cq_entry.op_context, ep);
				rstream_update_tx_credits(ep, ret);
				rstream_free_contig_len(&ep->local_mr.tx, len);
			} else {
				ret = -FI_ENOMSG;
				goto out;
			}
		} else if (ret != -FI_EAGAIN) {
			goto out;
		}
	} while ((ret == -FI_EAGAIN && !rstream_timer_completed(&timer) &&
		!found_msg_type) || (found_msg_type && ret > 0));

	ret = rstream_update_target(ep, rx_completions, 0);
	fastlock_release(&ep->cq_lock);
	if (ret)
		return ret;

	if (found_msg_type)
		return found_msg_type;
	else
		return -FI_EAGAIN;
out:
	fastlock_release(&ep->cq_lock);
	return ret;
}

static uint32_t get_send_addrs_and_len(struct rstream_ep *ep, char **tx_addr,
	char **dest_addr, uint32_t requested_len)
{
	uint32_t available_len = 0;

	requested_len = MIN(MIN(requested_len,
		rstream_calc_contig_len(&ep->local_mr.tx)),
		rstream_calc_contig_len(&ep->remote_data.mr));
	if (requested_len == 0)
		return available_len;

	available_len = rstream_alloc_contig_len_available(&ep->local_mr.tx,
		tx_addr, requested_len);
	available_len = rstream_alloc_contig_len_available(&ep->remote_data.mr,
		dest_addr, requested_len);

	return available_len;
}

static ssize_t rstream_can_send(struct rstream_ep *ep)
{
	ssize_t ret;

	if (rstream_tx_mr_full(ep) || rstream_target_mr_full(ep) ||
		rstream_target_rx_full(ep)) {
		ret = rstream_process_cq(ep, RSTREAM_CTRL_MSG);
		if (ret < 0)
			return ret;
	}

	if (rstream_tx_full(ep)) {
		ret = rstream_process_cq(ep, RSTREAM_TX_MSG_COMP);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static ssize_t rstream_send(struct fid_ep *ep_fid, const void *buf, size_t len,
	void *desc, fi_addr_t dest_addr, void *context)
{
	struct rstream_ep *ep = container_of(ep_fid, struct rstream_ep,
		util_ep.ep_fid);
	uint32_t cq_data = 0;
	ssize_t ret;
	char *tx_addr = NULL;
	char *remote_addr = NULL;
	size_t sent_len = 0;
	uint32_t curr_avail_len = len;
	void *ctx;

	fastlock_acquire(&ep->send_lock);
	do {
		ret = rstream_can_send(ep);
		if (ret < 0) {
			if (ret < 0 && ret != -FI_EAGAIN) {
				goto err;
			} else {
				fastlock_release(&ep->send_lock);
				return ((sent_len) ? sent_len : ret);
			}
		}

		curr_avail_len = get_send_addrs_and_len(ep, &tx_addr,
			&remote_addr, curr_avail_len);
		if (curr_avail_len == 0)
			break;

		memcpy(tx_addr, ((char *)buf + sent_len), curr_avail_len);
		sent_len = sent_len + curr_avail_len;
		ctx = rstream_get_tx_ctx(ep, curr_avail_len);

		if (RSTREAM_USING_IWARP) {
			ret = fi_write(ep->ep_fd, tx_addr, curr_avail_len,
				ep->local_mr.ldesc, 0, (uint64_t)remote_addr,
				ep->remote_data.rkey, ctx);
			ret = rstream_send_ctrl_msg(ep,
				rstream_iwarp_cq_data_set_msg_len(curr_avail_len));
		} else {
			ret = fi_writedata(ep->ep_fd, tx_addr, curr_avail_len,
				ep->local_mr.ldesc, cq_data, 0, (uint64_t)remote_addr,
				ep->remote_data.rkey, ctx);
		}
		if (ret != 0) {
			FI_DBG(&rstream_prov, FI_LOG_EP_DATA,
				"error: fi_write failed: %zd", ret);
			goto err;
		}
		curr_avail_len = len - sent_len;

		if (!RSTREAM_USING_IWARP)
			ep->qp_win.target_rx_credits--;

		ep->qp_win.tx_credits--;

	} while(curr_avail_len); /* circle buffer rollover requires two loops */

	fastlock_release(&ep->send_lock);
	return sent_len;

err:
	fastlock_release(&ep->send_lock);
	return ret;
}

static ssize_t rstream_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
	void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	return -FI_ENOSYS;
}

static ssize_t rstream_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
	uint64_t flags)
{
	int ret;
	struct rstream_ep *ep = container_of(ep_fid, struct rstream_ep,
		util_ep.ep_fid);

	if (flags == FI_PEEK) {
		fastlock_acquire(&ep->send_lock);
		ret = rstream_can_send(ep);
		fastlock_release(&ep->send_lock);
		return ret;
	} else {
		return -FI_ENOSYS;
	}
}

/* either posting everything at once or reposting after cq completion */
ssize_t rstream_post_cq_data_recv(struct rstream_ep *ep,
	const struct fi_cq_data_entry *cq_entry)
{
	struct fi_context *context = NULL;
	struct fi_msg msg;
	struct iovec imsg;
	void *buffer;
	ssize_t ret;

	if (!cq_entry || !cq_entry->op_context)
		context = rstream_get_rx_ctx(ep);
	else if (cq_entry && cq_entry->op_context)
		context = cq_entry->op_context;

	if (RSTREAM_USING_IWARP) {
		buffer = (cq_entry && cq_entry->buf) ? cq_entry->buf :
			rstream_get_next_recv_buffer(ep);
		assert(buffer);
		imsg.iov_base = buffer;
		imsg.iov_len = RSTREAM_IWARP_DATA_SIZE;
		msg.msg_iov = &imsg;
		msg.desc = &ep->local_mr.ldesc;
		msg.iov_count = 1;
		msg.context = context;
	} else {
		msg.msg_iov = NULL;
		msg.desc = NULL;
		msg.iov_count = 0;
		msg.context = context;
	}

	ret = fi_recvmsg(ep->ep_fd, &msg, 0);
	if (ret != 0)
		return ret;

	return ret;
}

static uint32_t rstream_copy_out_chunk(struct rstream_ep *ep, void *buf,
	uint32_t len_left)
{
	char *rx_data_ptr = NULL;
	uint32_t current_chunk =
		rstream_alloc_contig_len_available(&ep->local_mr.rx, &rx_data_ptr,
			len_left);

	if (current_chunk) {
		memcpy(buf, rx_data_ptr, current_chunk);
	}

	return current_chunk;
}

static ssize_t rstream_recv(struct fid_ep *ep_fid, void *buf, size_t len,
	void *desc, fi_addr_t src_addr, void *context)
{
	struct rstream_ep *ep = container_of(ep_fid, struct rstream_ep,
		util_ep.ep_fid);
	uint32_t copy_out_len = 0;
	ssize_t ret;

	fastlock_acquire(&ep->recv_lock);

	copy_out_len = rstream_copy_out_chunk(ep, buf, len);

	if ((len - copy_out_len)) {
		ret = rstream_process_cq(ep, RSTREAM_RX_MSG_COMP);
		if(ret < 0 && ret != -FI_EAGAIN) {
			fastlock_release(&ep->recv_lock);
			return ret;
		}

		copy_out_len = copy_out_len + rstream_copy_out_chunk(ep,
			((char *)buf + copy_out_len), (len - copy_out_len));
	}

	fastlock_acquire(&ep->send_lock);
	ret = rstream_update_target(ep, 0, copy_out_len);
	fastlock_release(&ep->send_lock);
	fastlock_release(&ep->recv_lock);
	if(ret < 0 && ret != -FI_EAGAIN) {
		return ret;
	}

	if (copy_out_len) {
		return copy_out_len;
	}

	return -FI_EAGAIN;
}

static ssize_t rstream_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
	void **desc, size_t count, fi_addr_t src_addr, void *context)
{
	return -FI_ENOSYS;
}

/* can't recv if you can't send a ctrl message -- only way to force user
 * to progress ctrl msg, but...Continue to receive any queued data even
 * if the remote side has disconnected (TODO) */
static ssize_t rstream_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
	uint64_t flags)
{
	int ret;
	struct rstream_ep *ep = container_of(ep_fid, struct rstream_ep,
		util_ep.ep_fid);

	if (flags == FI_PEEK) {
		fastlock_acquire(&ep->recv_lock);
		if (!ep->local_mr.rx.avail_size) {
			ret = rstream_process_cq(ep, RSTREAM_RX_MSG_COMP);
			if (ret < 0) {
				fastlock_release(&ep->recv_lock);
				return ret;
			}
		}
		fastlock_release(&ep->recv_lock);

		fastlock_acquire(&ep->send_lock);
		if (rstream_target_rx_full(ep)) {
			ret = rstream_process_cq(ep, RSTREAM_RX_MSG_COMP);
			if (ret < 0) {
				fastlock_release(&ep->send_lock);
				return ret;
			}
		}

		if (!ep->qp_win.ctrl_credits) {
			ret = rstream_process_cq(ep, RSTREAM_TX_MSG_COMP);
			fastlock_release(&ep->send_lock);
			return ret;
		}

		fastlock_release(&ep->send_lock);
		return 0;
	} else {
		return -FI_ENOSYS;
	}
}

struct fi_ops_msg rstream_ops_msg = {
	.size = sizeof(struct fi_ops_msg),
	.recv = rstream_recv,
	.recvv = rstream_recvv,
	.recvmsg = rstream_recvmsg,
	.send = rstream_send,
	.sendv = rstream_sendv,
	.sendmsg = rstream_sendmsg,
	.inject = rstream_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};
