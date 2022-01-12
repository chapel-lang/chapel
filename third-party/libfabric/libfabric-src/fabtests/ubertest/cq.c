/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
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

#include "fabtest.h"


static size_t comp_entry_cnt[] = {
	[FI_CQ_FORMAT_UNSPEC] = 0,
	[FI_CQ_FORMAT_CONTEXT] = FT_COMP_BUF_SIZE / sizeof(struct fi_cq_entry),
	[FI_CQ_FORMAT_MSG] = FT_COMP_BUF_SIZE / sizeof(struct fi_cq_msg_entry),
	[FI_CQ_FORMAT_DATA] = FT_COMP_BUF_SIZE / sizeof(struct fi_cq_data_entry),
	[FI_CQ_FORMAT_TAGGED] = FT_COMP_BUF_SIZE / sizeof(struct fi_cq_tagged_entry)
};

static size_t comp_entry_size[] = {
	[FI_CQ_FORMAT_UNSPEC] = 0,
	[FI_CQ_FORMAT_CONTEXT] = sizeof(struct fi_cq_entry),
	[FI_CQ_FORMAT_MSG] = sizeof(struct fi_cq_msg_entry),
	[FI_CQ_FORMAT_DATA] = sizeof(struct fi_cq_data_entry),
	[FI_CQ_FORMAT_TAGGED] = sizeof(struct fi_cq_tagged_entry)
};

int ft_use_comp_cntr(enum ft_comp_type comp_type)
{
	if ((comp_type == FT_COMP_CNTR) ||
		(comp_type == FT_COMP_ALL))
		return 1;

	return 0;
}

int ft_use_comp_cq(enum ft_comp_type comp_type)
{
	if ((comp_type == FT_COMP_QUEUE) ||
		(comp_type == FT_COMP_ALL))
		return 1;

	return 0;
}

int ft_generates_rx_comp(void)
{
	if (is_data_func(test_info.class_function) ||
	    (is_msg_func(test_info.class_function) &&
	    test_info.msg_flags & FI_REMOTE_CQ_DATA))
		return 1;

	if (test_info.test_class & (FI_RMA | FI_ATOMIC))
		return 0;

	if (!(test_info.rx_cq_bind_flags & FI_SELECTIVE_COMPLETION))
		return 1;
	if (test_info.rx_op_flags & FI_COMPLETION) {
		if (is_msg_func(test_info.class_function) &&
		    !(test_info.msg_flags & FI_COMPLETION))
			return 0;
		return 1;
	}
	if (is_msg_func(test_info.class_function) &&
	    test_info.msg_flags & FI_COMPLETION)
		return 1;
	return 0;
}

int ft_generates_tx_comp(void)
{
	if (!(test_info.tx_cq_bind_flags & FI_SELECTIVE_COMPLETION)) {
		if (is_inject_func(test_info.class_function))
			return 0;
		return 1;
	}

	if (test_info.tx_op_flags & FI_COMPLETION) {
		if (is_msg_func(test_info.class_function) &&
		    !(test_info.msg_flags & FI_COMPLETION))
			return 0;
		return 1;
	}

	if (test_info.msg_flags & FI_COMPLETION)
		return 1;

	return 0;
}

int ft_check_rx_completion(void)
{
	if (ft_use_comp_cntr(test_info.comp_type) ||
	    ft_generates_rx_comp())
		return 1;
	else
		return 0;
}

int ft_check_tx_completion(void)
{
	if (ft_use_comp_cntr(test_info.comp_type) ||
	    ft_generates_tx_comp())
		return 1;
	else
		return 0;
}

static int ft_open_cntrs(void)
{
	struct fi_cntr_attr attr;
	int ret;

	if (!txcntr) {
		memset(&attr, 0, sizeof attr);
		attr.wait_obj = test_info.cntr_wait_obj;
		ret = fi_cntr_open(domain, &attr, &txcntr, &txcntr);
		if (ret) {
			FT_PRINTERR("fi_cntr_open", ret);
			return ret;
		}
	}
	if (!rxcntr) {
		memset(&attr, 0, sizeof attr);
		attr.wait_obj = test_info.cntr_wait_obj;
		ret = fi_cntr_open(domain, &attr, &rxcntr, &rxcntr);
		if (ret) {
			FT_PRINTERR("fi_cntr_open", ret);
			return ret;
		}
	}
	return 0;
}

static int ft_open_cqs(void)
{
	struct fi_cq_attr attr;
	int ret;

	if (!txcq) {
		memset(&attr, 0, sizeof attr);
		attr.format = ft_tx_ctrl.cq_format;
		attr.wait_obj = test_info.cq_wait_obj;
		attr.size = ft_tx_ctrl.max_credits;

		ret = fi_cq_open(domain, &attr, &txcq, NULL);
		if (ret) {
			FT_PRINTERR("fi_cq_open", ret);
			return ret;
		}
	}

	if (!rxcq) {
		memset(&attr, 0, sizeof attr);
		attr.format = ft_rx_ctrl.cq_format;
		attr.wait_obj = test_info.cq_wait_obj;
		attr.size = ft_rx_ctrl.max_credits;

		ret = fi_cq_open(domain, &attr, &rxcq, NULL);
		if (ret) {
			FT_PRINTERR("fi_cq_open", ret);
			return ret;
		}
	}

	return 0;
}

int ft_open_comp(void)
{
	int ret;

	ret = ft_open_cqs();
	if (ret)
		return ret;

	switch (test_info.comp_type) {
	case FT_COMP_QUEUE:
		break;
	case FT_COMP_ALL:
	case FT_COMP_CNTR:
		ret = ft_open_cntrs();
		break;
	default:
		ret = -FI_ENOSYS;
	}

	return ret;
}

int ft_bind_comp(struct fid_ep *ep)
{
	int ret;
	uint64_t flags;

	if (!ft_use_comp_cq(test_info.comp_type)) {
		test_info.tx_cq_bind_flags |= FI_SELECTIVE_COMPLETION;
		test_info.rx_cq_bind_flags |= FI_SELECTIVE_COMPLETION;
	}
	flags = FI_TRANSMIT | test_info.tx_cq_bind_flags;
	ret = fi_ep_bind(ep, &txcq->fid, flags);
	if (ret) {
		FT_PRINTERR("fi_ep_bind", ret);
		return ret;
	}

	if (ft_use_comp_cntr(test_info.comp_type)) {
		flags = FI_TRANSMIT | FI_READ | FI_WRITE;
		ret = fi_ep_bind(ep, &txcntr->fid, flags);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}
	}

	flags = FI_RECV | test_info.rx_cq_bind_flags;
	ret = fi_ep_bind(ep, &rxcq->fid, flags);
	if (ret) {
		FT_PRINTERR("fi_ep_bind", ret);
		return ret;
	}

	if (ft_use_comp_cntr(test_info.comp_type)) {
		flags = FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE;
		ret = fi_ep_bind(ep, &rxcntr->fid, flags);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}
	}

	return 0;
}

static inline size_t ft_update_credits(void *buf,
			enum fi_cq_format format, size_t count)
{
	char *ptr = (char *) buf;
	size_t credits = 0;
	uint64_t flags;

	if (format == FI_CQ_FORMAT_CONTEXT || test_info.mode & FI_RX_CQ_DATA)
		return count;

	while (count--) {
		flags = ((struct fi_cq_msg_entry *) ptr)->flags;
		if (!(flags & (FI_REMOTE_READ | FI_REMOTE_WRITE)))
			credits++;
		ptr += comp_entry_size[format];
	}

	return credits;
}

/* Read CQ until there are no more completions */
#define ft_cq_read(cq_read, cq, buf, format, credits, completions, str, ret, verify,...)	\
	do {							\
		ret = cq_read(cq, buf, comp_entry_cnt[format], ##__VA_ARGS__);	\
		if (ret < 0) {					\
			if (ret == -FI_EAGAIN)			\
				break;				\
			if (ret == -FI_EAVAIL) {		\
				ret = ft_cq_readerr(cq);	\
			} else {				\
				FT_PRINTERR(#cq_read, ret);	\
			}					\
			return ret;				\
		} else {					\
			completions += ret;			\
			if (verify)				\
				ft_verify_comp(buf);		\
			credits += ft_update_credits(buf, format, ret);	\
		}						\
	} while (ret == comp_entry_cnt[format])

static size_t ft_comp_x(struct fid_cq *cq, struct ft_xcontrol *ft_x,
		const char *x_str, int timeout)
{
	uint8_t buf[FT_COMP_BUF_SIZE], start = 0;
	struct timespec s, e;
	int poll_time = 0;
	int ret, verify = (test_info.test_type == FT_TEST_UNIT && cq == rxcq);
	int completions = 0;

	switch(test_info.cq_wait_obj) {
	case FI_WAIT_NONE:
		do {
			if (!start) {
				clock_gettime(CLOCK_MONOTONIC, &s);
				start = 1;
			}

			ft_cq_read(fi_cq_read, cq, buf, ft_x->cq_format, ft_x->credits,
				   completions, x_str, ret, verify);

			clock_gettime(CLOCK_MONOTONIC, &e);
			poll_time = get_elapsed(&s, &e, MILLI);
		} while (ret == -FI_EAGAIN && poll_time < timeout &&
			 !completions);
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		ft_cq_read(fi_cq_sread, cq, buf, ft_x->cq_format, ft_x->credits,
			   completions, x_str, ret, verify, NULL, timeout);
		break;
	case FI_WAIT_SET:
		FT_ERR("fi_ubertest: Unsupported cq wait object");
		return -1;
	default:
		FT_ERR("Unknown cq wait object");
		return -1;
	}

	if (completions)
		return completions;

	return (ret == -FI_EAGAIN && timeout) ? ret : 0;
}

static int ft_cntr_x(struct fid_cntr *cntr, struct ft_xcontrol *ft_x,
		     int timeout)
{
	uint64_t cntr_val;
	struct timespec s, e;
	int poll_time = clock_gettime(CLOCK_MONOTONIC, &s);
	int recvd = 0;

	do {
		cntr_val = fi_cntr_read(cntr);
		clock_gettime(CLOCK_MONOTONIC, &e);
		poll_time = get_elapsed(&s, &e, MILLI);
	} while (cntr_val == ft_x->total_comp && poll_time < timeout);

	recvd = cntr_val - ft_x->total_comp;
	ft_x->total_comp = cntr_val;

	return recvd;
}

int ft_comp_rx(int timeout)
{
	int ret = 0;
	size_t cur_credits = ft_rx_ctrl.credits;
	int cq_ret;

	if (ft_use_comp_cntr(test_info.comp_type)) {
		ret = ft_cntr_x(rxcntr, &ft_rx_ctrl, timeout);
		if (ret < 0)
			return ret;

		if (ft_generates_rx_comp()) {
			do {
				cq_ret = ft_comp_x(rxcq, &ft_rx_ctrl, "rxcq", 0);
				if (cq_ret < 0)
					return cq_ret;
			} while (cq_ret > 0);
		}
		if (test_info.test_class & (FI_MSG | FI_TAGGED)) {
			ft_rx_ctrl.credits = cur_credits;
			ft_rx_ctrl.credits += ret;
			if (ret)
				return ret;
		}
	}

	if (ft_use_comp_cq(test_info.comp_type)) {
		if (test_info.comp_type == FT_COMP_ALL)
			ft_rx_ctrl.credits = cur_credits;
		ret = ft_comp_x(rxcq, &ft_rx_ctrl, "rxcq", timeout);
		if (ret < 0)
			return ret;

		if (ft_use_comp_cntr(test_info.comp_type))
			ft_rx_ctrl.credits = cur_credits;
	}
	assert(ft_rx_ctrl.credits <= ft_rx_ctrl.max_credits);

	return ret;
}

int ft_comp_tx(int timeout)
{
	int ret = 0;
	size_t cur_credits = ft_tx_ctrl.credits;
	int cq_ret;

	if (ft_use_comp_cntr(test_info.comp_type)) {
		ret = ft_cntr_x(txcntr, &ft_tx_ctrl, timeout);
		if (ret < 0)
			return ret;

		if (ft_generates_tx_comp()) {
			do {
				cq_ret = ft_comp_x(txcq, &ft_tx_ctrl, "txcq", 0);
				if (cq_ret < 0)
					return cq_ret;
			} while (cq_ret > 0);
		}
		ft_tx_ctrl.credits = cur_credits;
		ft_tx_ctrl.credits += ret;
	}

	if (ft_use_comp_cq(test_info.comp_type)) {
		if (test_info.comp_type == FT_COMP_ALL)
			ft_tx_ctrl.credits = cur_credits;
		ret = ft_comp_x(txcq, &ft_tx_ctrl, "txcq", timeout);
		if (ret < 0)
			return ret;

		if (ft_use_comp_cntr(test_info.comp_type))
			ft_tx_ctrl.credits = cur_credits;
	}
	assert(ft_tx_ctrl.credits <= ft_tx_ctrl.max_credits);

	return ret;
}
