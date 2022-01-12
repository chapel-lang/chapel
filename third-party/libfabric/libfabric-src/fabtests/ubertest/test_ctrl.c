/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "fabtest.h"

void ft_record_error(int error)
{
	if (!ft_ctrl.error) {
		fprintf(stderr, "ERROR [%s], continuing with test",
			fi_strerror(error));
		ft_ctrl.error = error;
	}
}

static int ft_init_xcontrol(struct ft_xcontrol *ctrl)
{
	memset(ctrl, 0, sizeof *ctrl);
	ctrl->credits = FT_DEFAULT_CREDITS;
	ctrl->max_credits =  FT_DEFAULT_CREDITS;

	ctrl->iov = calloc(ft_ctrl.iov_array[ft_ctrl.iov_cnt - 1], sizeof *ctrl->iov);
	ctrl->iov_desc = calloc(ft_ctrl.iov_array[ft_ctrl.iov_cnt - 1],
				sizeof *ctrl->iov_desc);
	ctrl->ctx = calloc(ctrl->max_credits, sizeof *ctrl->ctx);
	if (!ctrl->iov || !ctrl->iov_desc || !ctrl->ctx)
		return -FI_ENOMEM;

	return 0;
}

static int ft_init_rx_control(void)
{
	int ret;

	ret = ft_init_xcontrol(&ft_rx_ctrl);
	if (ret)
		return ret;

	ft_rx_ctrl.cq_format = FI_CQ_FORMAT_DATA;
	ft_rx_ctrl.addr = FI_ADDR_UNSPEC;

	ft_rx_ctrl.msg_size = ft_ctrl.size_array[ft_ctrl.size_cnt - 1];
	if (fabric_info && fabric_info->ep_attr &&
	    fabric_info->ep_attr->max_msg_size &&
	    fabric_info->ep_attr->max_msg_size < ft_rx_ctrl.msg_size)
		ft_rx_ctrl.msg_size = fabric_info->ep_attr->max_msg_size;

	return 0;
}

static int ft_init_tx_control(void)
{
	int ret;

	ret = ft_init_xcontrol(&ft_tx_ctrl);
	if (ret)
		return ret;

	ft_tx_ctrl.cq_format = FI_CQ_FORMAT_CONTEXT;
	ft_tx_ctrl.remote_cq_data = ft_init_cq_data(fabric_info);
	return 0;
}

static int ft_init_atomic_control(struct ft_atomic_control *ctrl)
{
	memset(ctrl, 0, sizeof *ctrl);
	ctrl->op = test_info.op;

	ctrl->ioc = calloc(ft_ctrl.iov_array[ft_ctrl.iov_cnt - 1], sizeof *ctrl->ioc);
	ctrl->res_ioc = calloc(ft_ctrl.iov_array[ft_ctrl.iov_cnt - 1],
				sizeof *ctrl->res_ioc);
	ctrl->comp_ioc = calloc(ft_ctrl.iov_array[ft_ctrl.iov_cnt - 1],
				sizeof *ctrl->comp_ioc);

	if (!ctrl->ioc || !ctrl->res_ioc || !ctrl->comp_ioc)
		return -FI_ENOMEM;

	return 0;
}

static int ft_init_control(void)
{
	int ret;

	memset(&ft_ctrl, 0, sizeof ft_ctrl);
	ft_ctrl.xfer_iter = FT_DEFAULT_CREDITS;
	ft_ctrl.inc_step = test_info.test_flags & FT_FLAG_QUICKTEST ? 4 : 1;

	ft_ctrl.iov_array = sm_size_array;
	ft_ctrl.iov_cnt = sm_size_cnt;

	ft_ctrl.size_array = lg_size_array;
	ft_ctrl.size_cnt = lg_size_cnt;

	ret = ft_init_rx_control();
	if (ret)
		return ret;

	ret = ft_init_tx_control();
	if (ret)
		return ret;

	ret = ft_init_atomic_control(&ft_atom_ctrl);
	return ret;
}

static void ft_cleanup_xcontrol(struct ft_xcontrol *ctrl)
{
	free(ctrl->buf);
	free(ctrl->iov);
	free(ctrl->iov_desc);
	free(ctrl->ctx);
	memset(ctrl, 0, sizeof *ctrl);
}

static void ft_cleanup_atomic_control(struct ft_atomic_control *ctrl)
{
	free(ctrl->res_buf);
	free(ctrl->comp_buf);
	free(ctrl->ioc);
	free(ctrl->res_ioc);
	free(ctrl->comp_ioc);
	free(ctrl->orig_buf);
	memset(ctrl, 0, sizeof *ctrl);
}

static void ft_cleanup_mr_control(struct ft_mr_control *ctrl)
{
	free(ctrl->buf);
	memset(ctrl, 0, sizeof *ctrl);
}

static void ft_format_iov_distributed(struct iovec *iov, size_t cnt, char *buf,
		size_t len)
{
	size_t offset;
	int i;

	for (i = 0, offset = 0; i < cnt - 1; i++) {
		iov[i].iov_base = buf + offset;
		iov[i].iov_len = len / cnt;
		offset += iov[i].iov_len;
	}
	iov[i].iov_base = buf + offset;
	iov[i].iov_len = len - offset;
}

/* One class of bugs is issues involving IOV length handling. The regular
 * ft_format_iov does not catch this class because it evenly partitions the
 * entries. Instead partition them proportional to their position in the iovec.
 */
static void _ft_format_iov_weighted(struct iovec *iov, size_t cnt, char *buf,
		size_t len, int reversed)
{
	double total_parts;
	double portion;
	size_t offset;
	size_t weight;
	size_t size;
	size_t i;

	/* Get the sum of the element positions in the list and calculate the
	 * base weight.
	 */
	total_parts = ((cnt + 1.0) * cnt) / 2.0;
	portion = len / total_parts;

	for (offset = 0, i = 0; i < cnt; i++) {
		if (reversed)
			weight = cnt - i;
		else
			weight = i + 1;

		/* Get the weight for this iovec entry and round it to the
		 * nearest integer.
		 */
		size = (portion * weight) + .5;

		iov[i].iov_base = buf + offset;
		iov[i].iov_len = size;

		offset += size;
	}
}

static void ft_format_iov_weighted(struct iovec *iov, size_t cnt, char *buf,
		size_t len)
{
	_ft_format_iov_weighted(iov, cnt, buf, len, 0);
}

static void ft_format_iov_reversed(struct iovec *iov, size_t cnt, char *buf,
		size_t len)
{
	_ft_format_iov_weighted(iov, cnt, buf, len, 1);
}

static void ft_format_iov_random(struct iovec *iov, size_t cnt, char *buf,
		size_t len)
{
	size_t offset;
	size_t weight;
	size_t i;

	offset = 0;
	for (i = 0; i < cnt; i++) {
		/* If last IOV then use remaining data. */
		if (i == (cnt - 1)) {
			weight = len;
		} else {
			/* Get a weight between 1 and the remaining length minus
			 * the remaining IOV count. This is so we can reserve at
			 * least a length of 1 for every IOV.
			 */
			weight = (rand() % (len - (cnt - i) + 1)) + 1;
		}

		len -= weight;

		iov[i].iov_base = buf + offset;
		iov[i].iov_len = weight;

		offset += weight;
	}
}

void ft_format_iov(struct iovec *iov, size_t cnt, char *buf, size_t len)
{
	typedef void (*iov_formatter)(struct iovec *iov, size_t cnt, char *buf,
			size_t len);
	size_t choice;

	static iov_formatter options[] = {
		ft_format_iov_distributed,
		ft_format_iov_weighted,
		ft_format_iov_reversed,
		ft_format_iov_random
	};

	choice = rand() % ARRAY_SIZE(options);

	options[choice](iov, cnt, buf, len);
}

static void ft_iov_to_ioc(struct iovec *iov, struct fi_ioc *ioc, size_t cnt,
		   enum fi_datatype datatype, char *buf)
{
	int i;
	size_t offset = 0;
	for (i = 0; i < cnt; i++) {
		ioc[i].count = iov[i].iov_len;
		ioc[i].addr = buf + offset;
		offset += ioc[i].count * ft_atom_ctrl.datatype_size;
	}
}

void ft_format_iocs(struct iovec *iov, size_t *iov_count)
{
	while(ft_ctrl.iov_array[ft_tx_ctrl.iov_iter] > ft_atom_ctrl.count)
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);

	*iov_count = ft_ctrl.iov_array[ft_tx_ctrl.iov_iter];
	ft_format_iov(iov, *iov_count, ft_tx_ctrl.buf, ft_atom_ctrl.count);
	ft_iov_to_ioc(iov, ft_atom_ctrl.ioc, *iov_count,
			ft_atom_ctrl.datatype, ft_tx_ctrl.buf);
	ft_iov_to_ioc(iov, ft_atom_ctrl.res_ioc, *iov_count,
			ft_atom_ctrl.datatype, ft_atom_ctrl.res_buf);
	ft_iov_to_ioc(iov, ft_atom_ctrl.comp_ioc, *iov_count,
			ft_atom_ctrl.datatype, ft_atom_ctrl.comp_buf);
}

void ft_next_iov_cnt(struct ft_xcontrol *ctrl, size_t max_iov_cnt)
{
	ctrl->iov_iter++;
	if (ctrl->iov_iter > ft_ctrl.iov_cnt ||
	    ft_ctrl.iov_array[ctrl->iov_iter] > max_iov_cnt)
		ctrl->iov_iter = 0;
}

int ft_get_ctx(struct ft_xcontrol *ctrl, struct fi_context **ctx)
{
	int ret;

	ctrl->curr_ctx++;
	if (ctrl->curr_ctx >= ctrl->max_credits) {
		if (ctrl == &ft_tx_ctrl) {
			while (ctrl->credits < ctrl->max_credits) {
				ret = ft_comp_tx(FT_COMP_TO);
				if (ret < 0 && ret != -FI_EAGAIN)
					return ret;
			}
		}
		ctrl->curr_ctx = 0;
	}
	*ctx = &(ctrl->ctx[ctrl->curr_ctx]);
	return 0;
}

static int check_atomic(size_t *count)
{
	int ret;

	switch (test_info.class_function) {
	case FT_FUNC_ATOMIC:
	case FT_FUNC_ATOMICV:
	case FT_FUNC_ATOMICMSG:
	case FT_FUNC_INJECT_ATOMIC:
		ret = check_base_atomic_op(ft_tx_ctrl.ep, ft_atom_ctrl.op,
			ft_atom_ctrl.datatype, count);
		break;
	case FT_FUNC_FETCH_ATOMIC:
	case FT_FUNC_FETCH_ATOMICV:
	case FT_FUNC_FETCH_ATOMICMSG:
		ret = check_fetch_atomic_op(ft_tx_ctrl.ep, ft_atom_ctrl.op,
			ft_atom_ctrl.datatype, count);
		break;
	case FT_FUNC_COMPARE_ATOMIC:
	case FT_FUNC_COMPARE_ATOMICV:
	default:
		ret = check_compare_atomic_op(ft_tx_ctrl.ep, ft_atom_ctrl.op,
			ft_atom_ctrl.datatype, count);
	}

	ft_atom_ctrl.datatype_size = datatype_to_size(ft_atom_ctrl.datatype);

	return ret;
}

static int ft_sync_test(int value)
{
	int ret;

	ret = ft_reset_ep();
	if (ret)
		return ret;

	return ft_sock_sync(value);
}

static int ft_sync_manual()
{
	int ret, value = 0, result = -FI_EOTHER;

	if (listen_sock < 0) {
		ret = send(sock, &value, sizeof(value), 0);
		if (ret != sizeof(value))
			return -FI_EOTHER;

		do {
			ret = recv(sock, &result, sizeof(result), MSG_DONTWAIT);
			if (ret == sizeof(result))
				break;

			ret = ft_comp_rx(0);
			if (ret < 0)
				return ret;
		} while (1);
	} else {
		do {
			ret = recv(sock, &result, sizeof(result), MSG_DONTWAIT);
			if (ret == sizeof(result))
				break;

			ret = ft_comp_rx(0);
			if (ret < 0)
				return ret;
		} while (1);

		ret = send(sock, &value, sizeof(value), 0);
		if (ret != sizeof(value))
			return -FI_EOTHER;
	}
	return 0;
}

static int ft_sync_progress(int value)
{
	if (test_info.progress == FI_PROGRESS_MANUAL)
		return ft_sync_manual();
	return ft_sock_sync(value);
}

static int ft_sync_msg_needed(void)
{
	return ft_check_rx_completion() ? 0 : ft_send_sync_msg();
}

static int ft_check_verify_cnt()
{
	if (test_info.msg_flags == FI_REMOTE_CQ_DATA &&
	    ft_ctrl.verify_cnt != ft_ctrl.xfer_iter)
		return -FI_EIO;
	return 0;
}

static int ft_pingpong_rma(void)
{
	int ret = 0, i;
	size_t count;

	if (test_info.test_class & FI_ATOMIC) {
		ret = check_atomic(&count);

		ft_atom_ctrl.count = ft_tx_ctrl.rma_msg_size / ft_atom_ctrl.datatype_size;
		if (ret == -FI_ENOSYS || ret == -FI_EOPNOTSUPP ||
		    ft_atom_ctrl.count > count || ft_atom_ctrl.count == 0) {
			return 0;
		}
		if (ret)
			return ret;
	}

	if (listen_sock < 0) {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_send_rma();
			if (ret)
				return ret;

			if (ft_check_tx_completion()) {
				ret = ft_comp_tx(FT_COMP_TO);
				if (ret < 0)
					return ret;
			}
			ret = ft_sync_msg_needed();
			if (ret)
				return ret;

			ret = ft_recv_msg();
			if (ret)
				return ret;
		}
	} else {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_recv_msg();
			if (ret)
				return ret;

			ret = ft_send_rma();
			if (ret)
				return ret;

			if (ft_check_tx_completion()) {
				ret = ft_comp_tx(FT_COMP_TO);
				if (ret < 0)
					return ret;
			}

			ret = ft_sync_msg_needed();
			if (ret)
				return ret;
		}
	}
	return ret;
}

static int ft_pingpong(void)
{
	int ret, i;

	if (test_info.test_class & (FI_RMA | FI_ATOMIC))
		return ft_pingpong_rma();

	if (listen_sock < 0) {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_send_msg();
			if (ret)
				return ret;

			if (ft_check_tx_completion()) {
				ret = ft_comp_tx(FT_COMP_TO);
				if (ret < 0)
					return ret;
			}

			ret = ft_recv_msg();
			if (ret)
				return ret;
		}
	} else {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_recv_msg();
			if (ret)
				return ret;

			ret = ft_send_msg();
			if (ret)
				return ret;

			if (ft_check_tx_completion()) {
				ret = ft_comp_tx(FT_COMP_TO);
				if (ret < 0)
					return ret;
			}
		}
	}

	return 0;
}

static int ft_pingpong_dgram(void)
{
	int ret, i;

	if (listen_sock < 0) {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_sendrecv_dgram();
			if (ret)
				return ret;
		}
	} else {
		for (i = 0; i < 1000; i++) {
			ret = ft_recv_dgram();
			if (!ret)
				break;
			else if (ret != -FI_ETIMEDOUT)
				return ret;
		}

		for (i = 0; i < ft_ctrl.xfer_iter - 1; i++) {
			ret = ft_sendrecv_dgram();
			if (ret)
				return ret;
		}

		ret = ft_send_dgram();
		if (ret)
			return ret;
	}

	return 0;
}

static int ft_run_latency(void)
{
	int ret, i;

	for (i = 0; i < ft_ctrl.size_cnt; i += ft_ctrl.inc_step) {
		if (ft_ctrl.size_array[i] > fabric_info->ep_attr->max_msg_size)
			break;

		if (test_info.test_class & (FI_RMA | FI_ATOMIC)) {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[0];
			ft_tx_ctrl.rma_msg_size = ft_ctrl.size_array[i];
		} else {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[i];
		}

		if (is_inject_func(test_info.class_function) &&
			(ft_ctrl.size_array[i] > fabric_info->tx_attr->inject_size))
			break;

		ft_ctrl.xfer_iter = test_info.test_flags & FT_FLAG_QUICKTEST ?
				5 : size_to_count(ft_ctrl.size_array[i]);

		ret = ft_sync_test(0);
		if (ret)
			return ret;

		ret = ft_post_recv_bufs();
		if (ret)
			return ret;

		clock_gettime(CLOCK_MONOTONIC, &start);
		ret = (test_info.ep_type == FI_EP_DGRAM) ?
			ft_pingpong_dgram() : ft_pingpong();
		clock_gettime(CLOCK_MONOTONIC, &end);
		if (ret) {
			FT_PRINTERR("latency test failed!", ret);
			return ret;
		}

		ret = ft_sync_progress(0);
		if (ret)
			return ret;

		show_perf("lat", ft_ctrl.size_array[i], ft_ctrl.xfer_iter, &start, &end, 2);
	}

	return 0;
}

static int ft_bw_rma(void)
{
	int ret, i;
	size_t count;

	if (test_info.test_class & FI_ATOMIC) {
		ret = check_atomic(&count);

		ft_atom_ctrl.count = ft_tx_ctrl.rma_msg_size / ft_atom_ctrl.datatype_size;
		if (ret == -FI_ENOSYS || ret == -FI_EOPNOTSUPP ||
		    ft_atom_ctrl.count > count || ft_atom_ctrl.count == 0) {
			return 0;
		}
		if (ret)
			return ret;
	}

	if (listen_sock < 0) {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_send_rma();
			if (ret)
				return ret;
		}
		ret = ft_sync_msg_needed();
		if (ret)
			return ret;

		ret = ft_recv_msg();
		if (ret)
			return ret;
	} else {
		ret = ft_recv_n_msg(ft_check_rx_completion() ?
				    ft_ctrl.xfer_iter : 1);
		if (ret)
			return ret;

		ret = ft_send_sync_msg();
		if (ret)
			return ret;
	}
	return 0;
}

static int ft_bw(void)
{
	int ret, i;

	if (test_info.test_class & (FI_RMA | FI_ATOMIC))
		return ft_bw_rma();

	if (listen_sock < 0) {
		for (i = 0; i < ft_ctrl.xfer_iter; i++) {
			ret = ft_send_msg();
			if (ret)
				return ret;
		}

		ret = ft_sync_msg_needed();
		if (ret)
			return ret;

		ret = ft_recv_msg();
		if (ret)
			return ret;
	} else {
		ret = ft_recv_n_msg(ft_check_rx_completion() ?
				    ft_ctrl.xfer_iter : 1);
		if (ret)
			return ret;

		ret = ft_send_sync_msg();
		if (ret)
			return ret;
	}

	return 0;
}

/*
 * The datagram streaming test sends datagrams with the initial byte
 * of the message cleared until we're ready to end the test.  The first
 * byte is then set to 0xFF.  On the receive side, we count the number
 * of completions until that message is seen.  Only the receiving side
 * reports any performance data.  The sender does not know how many
 * packets were dropped in flight.
 *
 * Because we re-use the same buffer for all messages, the receiving
 * side can notice that the first byte has changed and end the test
 * before the completion associated with the last message has been
 * written to the CQ.  As a result, the number of messages that were
 * counted as received may be slightly lower than the number of messages
 * that were actually received.
 *
 * For a significantly large number of transfers, this falls into the
 * noise, but it is visible if the number of iterations is small, such
 * as when running the quick test.  The fix for this would either to use
 * CQ data to exchange the end of test marker, or to allocate separate
 * buffers for each receive operation.
 *
 * The message with the end of test marker is retried until until the
 * receiver acknowledges it.  If the receiver ack message is lost, the
 * bandwidth test will hang.  However, this is the only message that the
 * receiver sends, so there's a reasonably good chance of it being transmitted
 * successfully.
 */
static int ft_bw_dgram(size_t *recv_cnt)
{
	int ret;

	if (listen_sock < 0) {
		*recv_cnt = 0;
		ret = ft_send_dgram_flood();
		if (ret)
			return ret;

		ft_tx_ctrl.seqno = ~0;
		ret = ft_sendrecv_dgram();
	} else {
		ret = ft_recv_dgram_flood(recv_cnt);
		if (ret)
			return ret;

		ret = ft_send_dgram();
	}

	return ret;
}

static int ft_run_bandwidth(void)
{
	size_t recv_cnt;
	int ret, i;

	for (i = 0; i < ft_ctrl.size_cnt; i += ft_ctrl.inc_step) {
		if (ft_ctrl.size_array[i] > fabric_info->ep_attr->max_msg_size)
			break;

		if (test_info.test_class & (FI_RMA | FI_ATOMIC)) {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[0];
			ft_tx_ctrl.rma_msg_size = ft_ctrl.size_array[i];
		} else {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[i];
		}

		if (is_inject_func(test_info.class_function) &&
			(ft_ctrl.size_array[i] > fabric_info->tx_attr->inject_size))
			break;

		ft_ctrl.xfer_iter = test_info.test_flags & FT_FLAG_QUICKTEST ?
				5 : size_to_count(ft_ctrl.size_array[i]);
		recv_cnt = ft_ctrl.xfer_iter;

		ret = ft_sync_test(0);
		if (ret)
			return ret;

		ret = ft_post_recv_bufs();
		if (ret)
			return ret;

		clock_gettime(CLOCK_MONOTONIC, &start);
		ret = (test_info.ep_type == FI_EP_DGRAM) ?
			ft_bw_dgram(&recv_cnt) : ft_bw();
		clock_gettime(CLOCK_MONOTONIC, &end);
		if (ret) {
			FT_PRINTERR("bw test failed!", ret);
			return ret;
		}

		ret = ft_sync_progress(0);
		if (ret)
			return ret;

		show_perf("bw", ft_ctrl.size_array[i], recv_cnt, &start, &end, 1);
	}

	return 0;
}

static int ft_unit_rma(void)
{
	int ret, i, fail = 0;

	for (i = 0; i < ft_ctrl.xfer_iter; i++) {
		ft_sync_fill_bufs(ft_tx_ctrl.rma_msg_size);

		ret = ft_send_rma();
		if (ret)
			return ret;

		if (!is_inject_func(test_info.class_function)) {
			ret = ft_comp_tx(FT_COMP_TO);
			if (ret < 0)
				return ret;
		}

		ret = ft_sync_msg_needed();
		if (ret)
			return ret;

		ret = ft_recv_msg();
		if (ret)
			return ret;

		ret = ft_verify_bufs();
		if (ret)
			fail = -FI_EIO;
	}

	ret = ft_check_verify_cnt();
	if (ret)
		return ret;

	return fail;
}

static int ft_unit_atomic(void)
{
	int ret, i, fail = 0;
	size_t count;

	ret = check_atomic(&count);

	ft_atom_ctrl.count = ft_tx_ctrl.rma_msg_size / ft_atom_ctrl.datatype_size;
	if (ret == -FI_ENOSYS || ret == -FI_EOPNOTSUPP ||
	    ft_atom_ctrl.count > count || ft_atom_ctrl.count == 0) {
		return 0;
	}
	if (ret)
		return ret;

	for (i = 0; i < ft_ctrl.xfer_iter; i++) {
		ft_sync_fill_bufs(ft_tx_ctrl.rma_msg_size);

		ret = ft_send_rma();
		if (ret)
			return ret;

		if (!is_inject_func(test_info.class_function)) {
			ret = ft_comp_tx(FT_COMP_TO);
			if (ret < 0)
				return ret;
		}
		ret = ft_sync_msg_needed();
		if (ret)
			return ret;

		ret = ft_recv_msg();
		if (ret)
			return ret;

		ret = ft_verify_bufs();
		if (ret)
			fail = -FI_EIO;
	}

	ret = ft_check_verify_cnt();
	if (ret)
		return ret;
	return fail;
}

static int ft_unit(void)
{
	int ret, i, fail = 0;

	ft_ctrl.verify_cnt = 0;
	if (test_info.test_class & FI_RMA)
		return ft_unit_rma();
	else if (test_info.test_class & FI_ATOMIC)
		return ft_unit_atomic();

	for (i = 0; i < ft_ctrl.xfer_iter; i++) {
		ft_sync_fill_bufs(ft_tx_ctrl.msg_size);

		ret = ft_send_msg();
		if (ret)
			return ret;

		if (ft_check_tx_completion()) {
			ret = ft_comp_tx(FT_COMP_TO);
			if (ret < 0)
				return ret;
		}

		ret = ft_sync_msg_needed();
		if (ret)
			return ret;

		ret = ft_recv_msg();
		if (ret)
			return ret;

		ret = ft_verify_bufs();
		if (ret)
			fail = -FI_EIO;
	}
	ret = ft_check_verify_cnt();
	if (ret)
		return ret;
	return fail;
}

static int ft_run_unit(void)
{
	int i, ret, fail;

	fail = ret = 0;

	for (i = 0; i < ft_ctrl.size_cnt; i += ft_ctrl.inc_step) {
		if (ft_ctrl.size_array[i] > fabric_info->ep_attr->max_msg_size)
			break;

		if (test_info.test_class & (FI_RMA | FI_ATOMIC)) {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[0];
			ft_tx_ctrl.rma_msg_size = ft_ctrl.size_array[i];
		} else {
			ft_tx_ctrl.msg_size = ft_ctrl.size_array[i];
		}

		if (is_inject_func(test_info.class_function) &&
			(ft_ctrl.size_array[i] > fabric_info->tx_attr->inject_size))
			break;

		ft_ctrl.xfer_iter = test_info.test_flags & FT_FLAG_QUICKTEST ?
				5 : size_to_count(ft_ctrl.size_array[i]);

		ret = ft_sync_test(0);
		if (ret)
			return ret;

		ret = ft_post_recv_bufs();
		if (ret)
			return ret;

		ret = ft_unit();
		if (ret) {
			if (ret != -FI_EIO)
				return ret;
			fail = -FI_EIO;
		}
	}
	if (fail)
		printf("unit test FAILED\n");
	else
		printf("unit test PASSED\n");

	return fail;
}

void ft_cleanup(void)
{
	FT_CLOSE_FID(ft_rx_ctrl.mr);
	FT_CLOSE_FID(ft_tx_ctrl.mr);
	FT_CLOSE_FID(ft_mr_ctrl.mr);
	FT_CLOSE_FID(ft_atom_ctrl.res_mr);
	FT_CLOSE_FID(ft_atom_ctrl.comp_mr);
	ft_free_res();
	ft_cleanup_xcontrol(&ft_rx_ctrl);
	ft_cleanup_xcontrol(&ft_tx_ctrl);
	ft_cleanup_mr_control(&ft_mr_ctrl);
	ft_cleanup_atomic_control(&ft_atom_ctrl);
	memset(&ft_ctrl, 0, sizeof ft_ctrl);
}

static int ft_exchange_mr_addr_key(void)
{
	struct fi_rma_iov local_rma_iov = {0};
	struct fi_rma_iov peer_rma_iov = {0};
	int ret;

	if (!(test_info.mr_mode & (FI_MR_VIRT_ADDR | FI_MR_PROV_KEY)))
		return 0;

	if (test_info.mr_mode & FI_MR_VIRT_ADDR)
		local_rma_iov.addr = (uint64_t) ft_mr_ctrl.buf;

	if (test_info.mr_mode & FI_MR_PROV_KEY)
		local_rma_iov.key = ft_mr_ctrl.mr_key;

	ret = ft_sock_send(sock, &local_rma_iov, sizeof local_rma_iov);
	if (ret) {
		FT_PRINTERR("ft_sock_send", ret);
		return ret;
	}

	ret = ft_sock_recv(sock, &peer_rma_iov, sizeof peer_rma_iov);
	if (ret) {
		FT_PRINTERR("ft_sock_recv", ret);
		return ret;
	}

	ft_mr_ctrl.peer_mr_addr = peer_rma_iov.addr;
	if (test_info.mr_mode & FI_MR_PROV_KEY)
		ft_mr_ctrl.peer_mr_key = peer_rma_iov.key;

	return 0;
}

int ft_open_res()
{
	int ret;

	ret = ft_init_control();
	if (ret) {
		FT_PRINTERR("ft_init_control", ret);
		goto cleanup;
	}

	ret = ft_open_control();
	if (ret) {
		FT_PRINTERR("ft_open_control", ret);
		goto cleanup;
	}
	if (test_info.ep_type == FI_EP_MSG && listen_sock >= 0) {
		ret = ft_open_passive();
		if (ret) {
			FT_PRINTERR("ft_open_passive", ret);
			goto cleanup;
		}
	}
	else {
		ret = ft_open_active();
		if (ret) {
			FT_PRINTERR("ft_open_active", ret);
			goto cleanup;
		}
	}

	return 0;
cleanup:
	ft_cleanup();
	return ret;
}

int ft_init_test()
{
	int ret;

	ft_sock_sync(0);

	ret = ft_enable_comm();
	if (ret) {
		FT_PRINTERR("ft_enable_comm", ret);
		goto cleanup;
	}

	ret = ft_post_recv_bufs();
	if (ret)
		return ret;

	ret = ft_exchange_mr_addr_key();
	if (ret) {
		FT_PRINTERR("ft_exchange_mr_address", ret);
		goto cleanup;
	}

	return 0;
cleanup:
	ft_cleanup();
	return ret;
}

int ft_run_test()
{
	int ret;

	switch (test_info.test_type) {
	case FT_TEST_UNIT:
		ret = ft_run_unit();
		if (ret)
			FT_PRINTERR("ft_run_unit", ret);
		break;
	case FT_TEST_LATENCY:
		ret = ft_run_latency();
		if (ret)
			FT_PRINTERR("ft_run_latency", ret);
		break;
	case FT_TEST_BANDWIDTH:
		ret = ft_run_bandwidth();
		if (ret)
			FT_PRINTERR("ft_run_bandwidth", ret);
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	ft_sync_test(0);
	ft_cleanup();

	return ret ? ret : -ft_ctrl.error;
}
