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
#include <time.h>

#include "fabtest.h"


static int ft_post_recv(void)
{
	struct fi_msg msg;
	int ret;
	struct fi_context *ctx = NULL;
	uint64_t flags = 0;

	if (test_info.msg_flags == FI_COMPLETION)
		flags = test_info.msg_flags;

	if (test_info.test_class & (FI_RMA | FI_ATOMIC) ||
	    ft_generates_rx_comp()) {
		ret = ft_get_ctx(&ft_rx_ctrl, &ctx);
		if (ret)
			return ret;
	}

	if (fabric_info->caps & FI_DIRECTED_RECV)
		ft_rx_ctrl.addr = ft_tx_ctrl.addr;

	switch (test_info.class_function) {
	case FT_FUNC_SENDV:
		ft_format_iov(ft_rx_ctrl.iov, ft_ctrl.iov_array[ft_rx_ctrl.iov_iter],
				ft_rx_ctrl.buf, ft_rx_ctrl.msg_size);
		ret = fi_recvv(ft_rx_ctrl.ep, ft_rx_ctrl.iov, ft_rx_ctrl.iov_desc,
				ft_ctrl.iov_array[ft_rx_ctrl.iov_iter], ft_rx_ctrl.addr, ctx);
		ft_next_iov_cnt(&ft_rx_ctrl, fabric_info->rx_attr->iov_limit);
		break;
	case FT_FUNC_SENDMSG:
		ft_format_iov(ft_rx_ctrl.iov, ft_ctrl.iov_array[ft_rx_ctrl.iov_iter],
				ft_rx_ctrl.buf, ft_rx_ctrl.msg_size);
		msg.msg_iov = ft_rx_ctrl.iov;
		msg.desc = ft_rx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_rx_ctrl.iov_iter];
		msg.addr = ft_rx_ctrl.addr;
		msg.context = ctx;
		msg.data = 0;
		ret = fi_recvmsg(ft_rx_ctrl.ep, &msg, flags);
		ft_next_iov_cnt(&ft_rx_ctrl, fabric_info->rx_attr->iov_limit);
		break;
	default:
		ret = fi_recv(ft_rx_ctrl.ep, ft_rx_ctrl.buf, ft_rx_ctrl.msg_size,
				ft_rx_ctrl.memdesc, ft_rx_ctrl.addr, ctx);
		break;
	}

	return ret;
}

static int ft_post_trecv(void)
{
	struct fi_msg_tagged msg;
	int ret;
	struct fi_context *ctx = NULL;
	uint64_t flags = 0;

	if (test_info.msg_flags == FI_COMPLETION)
		flags = test_info.msg_flags;

	if (ft_generates_rx_comp()) {
		ret = ft_get_ctx(&ft_rx_ctrl, &ctx);
		if (ret)
			return ret;
	}

	if (fabric_info->caps & FI_DIRECTED_RECV)
		ft_rx_ctrl.addr = ft_tx_ctrl.addr;

	switch (test_info.class_function) {
	case FT_FUNC_SENDV:
		ft_format_iov(ft_rx_ctrl.iov, ft_ctrl.iov_array[ft_rx_ctrl.iov_iter],
				ft_rx_ctrl.buf, ft_rx_ctrl.msg_size);
		ret = fi_trecvv(ft_rx_ctrl.ep, ft_rx_ctrl.iov, ft_rx_ctrl.iov_desc,
				ft_ctrl.iov_array[ft_rx_ctrl.iov_iter], ft_rx_ctrl.addr,
				ft_rx_ctrl.tag, 0, ctx);
		ft_next_iov_cnt(&ft_rx_ctrl, fabric_info->rx_attr->iov_limit);
		break;
	case FT_FUNC_SENDMSG:
		ft_format_iov(ft_rx_ctrl.iov, ft_ctrl.iov_array[ft_rx_ctrl.iov_iter],
				ft_rx_ctrl.buf, ft_rx_ctrl.msg_size);
		msg.msg_iov = ft_rx_ctrl.iov;
		msg.desc = ft_rx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_rx_ctrl.iov_iter];
		msg.addr = ft_rx_ctrl.addr;
		msg.tag = ft_rx_ctrl.tag;
		msg.ignore = 0;
		msg.context = ctx;
		ret = fi_trecvmsg(ft_rx_ctrl.ep, &msg, flags);
		ft_next_iov_cnt(&ft_rx_ctrl, fabric_info->rx_attr->iov_limit);
		break;
	default:
		ret = fi_trecv(ft_rx_ctrl.ep, ft_rx_ctrl.buf, ft_rx_ctrl.msg_size,
				ft_rx_ctrl.memdesc, ft_rx_ctrl.addr, ft_rx_ctrl.tag, 0, ctx);
		break;
	}
	return ret;
}

#define ft_send_retry(ret, send, ep, ...)		\
	do {						\
		ret = send(ep, ##__VA_ARGS__);		\
		if (ret == -FI_EAGAIN)			\
			ft_comp_tx(0);			\
	} while (ret == -FI_EAGAIN)

static int ft_post_send(void)
{
	struct fi_msg msg;
	int ret;
	struct fi_context *ctx = NULL;

	if (ft_generates_tx_comp()) {
		ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
		if (ret)
			return ret;
	}

	switch (test_info.class_function) {
	case FT_FUNC_SENDV:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.buf, ft_tx_ctrl.msg_size);
		ft_send_retry(ret, fi_sendv, ft_tx_ctrl.ep, ft_tx_ctrl.iov,
				ft_tx_ctrl.iov_desc, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.addr, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_SENDMSG:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.buf, ft_tx_ctrl.msg_size);
		msg.msg_iov = ft_tx_ctrl.iov;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_tx_ctrl.iov_iter];
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;
		ft_send_retry(ret, fi_sendmsg, ft_tx_ctrl.ep, &msg, test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_INJECT:
		ft_send_retry(ret, fi_inject, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.addr);
		break;
	case FT_FUNC_INJECTDATA:
		ft_send_retry(ret, fi_injectdata, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.remote_cq_data,
				ft_tx_ctrl.addr);
		break;
	case FT_FUNC_SENDDATA:
		ft_send_retry(ret, fi_senddata, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.memdesc,
				ft_tx_ctrl.remote_cq_data, ft_tx_ctrl.addr, ctx);
		break;
	default:
		ft_send_retry(ret, fi_send, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.memdesc,
				ft_tx_ctrl.addr, ctx);
		break;
	}

	if (ft_check_tx_completion())
		ft_tx_ctrl.credits--;

	return ret;
}

static int ft_post_tsend(void)
{
	struct fi_msg_tagged msg;
	int ret;
	struct fi_context *ctx = NULL;

	if (ft_generates_tx_comp()) {
		ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
		if (ret)
			return ret;
	}

	switch (test_info.class_function) {
	case FT_FUNC_SENDV:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.buf, ft_tx_ctrl.msg_size);
		ft_send_retry(ret, fi_tsendv, ft_tx_ctrl.ep, ft_tx_ctrl.iov,
				ft_tx_ctrl.iov_desc, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.addr, ft_tx_ctrl.tag, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_SENDMSG:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
				ft_tx_ctrl.buf, ft_tx_ctrl.msg_size);
		msg.msg_iov = ft_tx_ctrl.iov;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_tx_ctrl.iov_iter];
		msg.addr = ft_tx_ctrl.addr;
		msg.tag = ft_tx_ctrl.tag;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;
		ft_send_retry(ret, fi_tsendmsg, ft_tx_ctrl.ep, &msg, test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_INJECT:
		ft_send_retry(ret, fi_tinject, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.addr, ft_tx_ctrl.tag);
		break;
	case FT_FUNC_INJECTDATA:
		ft_send_retry(ret, fi_tinjectdata, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.remote_cq_data,
				ft_tx_ctrl.addr, ft_tx_ctrl.tag);
		break;
	case FT_FUNC_SENDDATA:
		ft_send_retry(ret, fi_tsenddata, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.memdesc,
				ft_tx_ctrl.remote_cq_data, ft_tx_ctrl.addr,
				ft_tx_ctrl.tag, ctx);
		break;
	default:
		ft_send_retry(ret, fi_tsend, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_tx_ctrl.msg_size, ft_tx_ctrl.memdesc,
				ft_tx_ctrl.addr, ft_tx_ctrl.tag, ctx);
		break;
	}

	if (ft_check_tx_completion())
		ft_tx_ctrl.credits--;

	return ret;
}

static int ft_post_send_rma(void)
{
	int ret, i;
	struct fi_msg_rma msg;
	struct fi_rma_iov rma_iov;
	struct fi_context *ctx = NULL;
	uint64_t read_flags = 0;

	if (test_info.msg_flags == FI_COMPLETION)
		read_flags = test_info.msg_flags;

	if (ft_generates_tx_comp()) {
 		ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
 		if (ret)
 			return ret;
	}

	switch (test_info.class_function) {
	case FT_FUNC_READ:
		ft_send_retry(ret, fi_read, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
			ft_tx_ctrl.rma_msg_size, ft_tx_ctrl.memdesc,
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ctx);
		break;
	case FT_FUNC_READV:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.buf, ft_tx_ctrl.rma_msg_size);
		ft_send_retry(ret, fi_readv, ft_tx_ctrl.ep, ft_tx_ctrl.iov,
			ft_tx_ctrl.iov_desc, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_READMSG:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.buf, ft_tx_ctrl.rma_msg_size);

		msg.msg_iov = ft_tx_ctrl.iov;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_tx_ctrl.iov_iter];
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = 0;

		rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
		rma_iov.key = ft_mr_ctrl.peer_mr_key;
		for (i = 0, rma_iov.len = 0; i < msg.iov_count; i++)
			rma_iov.len += ft_tx_ctrl.iov[i].iov_len;

		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;
		ft_send_retry(ret, fi_readmsg, ft_tx_ctrl.ep, &msg, read_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_WRITEV:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.buf, ft_tx_ctrl.rma_msg_size);
		ft_send_retry(ret, fi_writev, ft_tx_ctrl.ep, ft_tx_ctrl.iov,
			ft_tx_ctrl.iov_desc, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_WRITEMSG:
		ft_format_iov(ft_tx_ctrl.iov, ft_ctrl.iov_array[ft_tx_ctrl.iov_iter],
			ft_tx_ctrl.buf, ft_tx_ctrl.rma_msg_size);

		msg.msg_iov = ft_tx_ctrl.iov;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = ft_ctrl.iov_array[ft_tx_ctrl.iov_iter];
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;

		rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
		rma_iov.key = ft_mr_ctrl.peer_mr_key;
		for (i = 0, rma_iov.len = 0; i < msg.iov_count; i++)
			rma_iov.len += ft_tx_ctrl.iov[i].iov_len;

		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;
		ft_send_retry(ret, fi_writemsg, ft_tx_ctrl.ep, &msg,
			test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_INJECT_WRITE:
		ft_send_retry(ret, fi_inject_write, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
			ft_tx_ctrl.rma_msg_size, ft_tx_ctrl.addr,
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key);
		break;
	case FT_FUNC_WRITEDATA:
		ft_send_retry(ret, fi_writedata, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
			ft_tx_ctrl.rma_msg_size, ft_tx_ctrl.memdesc,
			ft_tx_ctrl.remote_cq_data, ft_tx_ctrl.addr,
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key, ctx);
		break;
	case FT_FUNC_INJECT_WRITEDATA:
		ft_send_retry(ret, fi_inject_writedata, ft_tx_ctrl.ep,
			ft_tx_ctrl.buf, ft_tx_ctrl.rma_msg_size,
			ft_tx_ctrl.remote_cq_data, ft_tx_ctrl.addr,
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key);
		break;
	default:
		ft_send_retry(ret, fi_write, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
			ft_tx_ctrl.rma_msg_size, ft_tx_ctrl.memdesc,
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ctx);
		break;
	}

	if (ft_check_tx_completion())
		ft_tx_ctrl.credits--;

	return ret;
}

int ft_post_send_atomic(void)
{
	int ret, i;
	struct fi_msg_atomic msg;
	struct fi_rma_ioc rma_iov;
	size_t iov_count;
	struct fi_context *ctx = NULL;

	if (ft_generates_tx_comp()) {
 		ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
 		if (ret)
 			return ret;
	}

	switch (test_info.class_function) {
	case FT_FUNC_ATOMICV:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		ft_send_retry(ret, fi_atomicv, ft_tx_ctrl.ep, ft_atom_ctrl.ioc,
			ft_tx_ctrl.iov_desc, iov_count, ft_tx_ctrl.addr,
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key,
			ft_atom_ctrl.datatype, ft_atom_ctrl.op, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_ATOMICMSG:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		msg.msg_iov = ft_atom_ctrl.ioc;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = iov_count;
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;
		msg.op = ft_atom_ctrl.op;
		msg.datatype = ft_atom_ctrl.datatype;

		rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
		rma_iov.key = ft_mr_ctrl.peer_mr_key;

		for (i = 0, rma_iov.count = 0; i < msg.iov_count; i++)
			rma_iov.count += ft_atom_ctrl.ioc[i].count;

		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;
		ft_send_retry(ret, fi_atomicmsg, ft_tx_ctrl.ep, &msg, test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_FETCH_ATOMIC:
		ft_send_retry(ret, fi_fetch_atomic, ft_tx_ctrl.ep,
			ft_tx_ctrl.buf, ft_atom_ctrl.count, ft_tx_ctrl.memdesc,
			ft_atom_ctrl.res_buf, ft_atom_ctrl.res_memdesc,
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ft_atom_ctrl.datatype,
			ft_atom_ctrl.op, ctx);
		break;
	case FT_FUNC_FETCH_ATOMICV:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		ft_send_retry(ret, fi_fetch_atomicv, ft_tx_ctrl.ep,
			ft_atom_ctrl.ioc, ft_tx_ctrl.iov_desc, iov_count,
			ft_atom_ctrl.res_ioc, ft_atom_ctrl.res_memdesc,
			iov_count, ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ft_atom_ctrl.datatype,
			ft_atom_ctrl.op, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_FETCH_ATOMICMSG:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		msg.msg_iov = ft_atom_ctrl.ioc;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = iov_count;
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;
		msg.op = ft_atom_ctrl.op;
		msg.datatype = ft_atom_ctrl.datatype;

		rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
		rma_iov.key = ft_mr_ctrl.peer_mr_key;

		for (i = 0, rma_iov.count = 0; i < msg.iov_count; i++)
			rma_iov.count += ft_atom_ctrl.ioc[i].count;

		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;

		ft_send_retry(ret, fi_fetch_atomicmsg, ft_tx_ctrl.ep, &msg,
			ft_atom_ctrl.res_ioc, ft_atom_ctrl.res_memdesc,
			iov_count, test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_COMPARE_ATOMIC:
		ft_send_retry(ret, fi_compare_atomic, ft_tx_ctrl.ep,
			ft_tx_ctrl.buf, ft_atom_ctrl.count, ft_tx_ctrl.memdesc,
			ft_atom_ctrl.comp_buf, ft_atom_ctrl.comp_memdesc,
			ft_atom_ctrl.res_buf, ft_atom_ctrl.res_memdesc,
			ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
			ft_mr_ctrl.peer_mr_key, ft_atom_ctrl.datatype,
			ft_atom_ctrl.op, ctx);
		break;
	case FT_FUNC_COMPARE_ATOMICV:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		ft_send_retry(ret, fi_compare_atomicv, ft_tx_ctrl.ep,
			ft_atom_ctrl.ioc, ft_tx_ctrl.iov_desc, iov_count,
			ft_atom_ctrl.comp_ioc, ft_atom_ctrl.comp_memdesc,
			iov_count, ft_atom_ctrl.res_ioc,
			ft_atom_ctrl.res_memdesc, iov_count, ft_tx_ctrl.addr,
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key,
			ft_atom_ctrl.datatype, ft_atom_ctrl.op, ctx);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_COMPARE_ATOMICMSG:
		ft_format_iocs(ft_tx_ctrl.iov, &iov_count);
		msg.msg_iov = ft_atom_ctrl.ioc;
		msg.desc = ft_tx_ctrl.iov_desc;
		msg.iov_count = iov_count;
		msg.addr = ft_tx_ctrl.addr;
		msg.context = ctx;
		msg.data = ft_tx_ctrl.remote_cq_data;
		msg.op = ft_atom_ctrl.op;
		msg.datatype = ft_atom_ctrl.datatype;

		rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
		rma_iov.key = ft_mr_ctrl.peer_mr_key;

		for (i = 0, rma_iov.count = 0; i < msg.iov_count; i++)
			rma_iov.count += ft_atom_ctrl.ioc[i].count;

		msg.rma_iov = &rma_iov;
		msg.rma_iov_count = 1;

		ft_send_retry(ret, fi_compare_atomicmsg, ft_tx_ctrl.ep, &msg,
			ft_atom_ctrl.comp_ioc, ft_atom_ctrl.comp_memdesc, iov_count,
			ft_atom_ctrl.res_ioc, ft_atom_ctrl.res_memdesc,
			iov_count, test_info.msg_flags);
		ft_next_iov_cnt(&ft_tx_ctrl, fabric_info->tx_attr->iov_limit);
		break;
	case FT_FUNC_INJECT_ATOMIC:
		ft_send_retry(ret, fi_inject_atomic, ft_tx_ctrl.ep,
			ft_tx_ctrl.buf, ft_atom_ctrl.count, ft_tx_ctrl.addr, 
			ft_mr_ctrl.peer_mr_addr, ft_mr_ctrl.peer_mr_key,
			ft_atom_ctrl.datatype, ft_atom_ctrl.op);
		break;
	default:
		ft_send_retry(ret, fi_atomic, ft_tx_ctrl.ep, ft_tx_ctrl.buf,
				ft_atom_ctrl.count, ft_tx_ctrl.memdesc,
				ft_tx_ctrl.addr, ft_mr_ctrl.peer_mr_addr,
				ft_mr_ctrl.peer_mr_key, ft_atom_ctrl.datatype,
				ft_atom_ctrl.op, ctx);
	}

	if (ft_check_tx_completion())
		ft_tx_ctrl.credits--;

	return ret;
}

int ft_send_rma(void)
{
	int ret;

	while (!ft_tx_ctrl.credits) {
		ret = ft_comp_tx(FT_COMP_TO);
		if (ret < 0)
			return ret;
	}

	if (test_info.test_class & FI_ATOMIC)
		ret = ft_post_send_atomic();
	else 	
		ret = ft_post_send_rma();

	if (ret) {
		FT_PRINTERR("send_rma", ret);
		return ret;
	}

	if (is_inject_func(test_info.class_function) &&
	    test_info.test_type == FT_TEST_UNIT)
		memset(ft_tx_ctrl.buf, 0, ft_tx_ctrl.rma_msg_size);

	if (!ft_tx_ctrl.credits) {
		ret = ft_comp_tx(0);
		if (ret < 0)
			return ret;
	}

	return 0;
}

int ft_post_recv_bufs(void)
{
	int ret;

	for (; ft_rx_ctrl.credits; ft_rx_ctrl.credits--) {
		if (test_info.test_class & FI_TAGGED) {
			ret = ft_post_trecv();
			if (!ret)
				ft_rx_ctrl.tag++;
		} else {
			ret = ft_post_recv();
		}
		if (ret) {
			if (ret == -FI_EAGAIN)
				break;
			FT_PRINTERR("recv", ret);
			return ret;
		}
	}
	return 0;
}

int ft_recv_n_msg(int n)
{
	int ret, recved = 0;

	do {
		if (ft_rx_ctrl.credits > (ft_rx_ctrl.max_credits >> 1)) {
			ret = ft_post_recv_bufs();
			if (ret)
				return ret;
		}

		ret = ft_comp_rx(0);
		if (ret < 0)
			return ret;

		//ft_comp_rx may have found multiple completions (bw testing)
		recved += ret;

		// handle manual progress. we should progress sends if
		// we don't get any recv completions. the send could have
		// been lost.
		// ft_comp_tx(0);
	} while (recved < n);

	return 0;
}

int ft_recv_msg(void)
{
	return ft_recv_n_msg(1);
}

static int ft_rma_sync(void)
{
	int ret;
	struct fi_msg_rma msg;
	struct iovec msg_iov;
	struct fi_rma_iov rma_iov;
	struct fi_context *ctx;

	ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
	if (ret)
		return ret;

	msg_iov.iov_base = (void *) ft_tx_ctrl.buf;
	msg_iov.iov_len = 0;
	msg.msg_iov = &msg_iov;
	msg.desc = ft_tx_ctrl.iov_desc;
	msg.iov_count = 1;
	msg.addr = ft_tx_ctrl.addr;
	msg.context = ctx;
	msg.data = 0;

	rma_iov.addr = ft_mr_ctrl.peer_mr_addr;
	rma_iov.key = ft_mr_ctrl.peer_mr_key;
	rma_iov.len = 0;

	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;

	ft_send_retry(ret, fi_writemsg, ft_tx_ctrl.ep, &msg, FI_COMPLETION);

	ft_tx_ctrl.credits--;
	return ret;
}

static int ft_msg_sync(void)
{
	int ret;
	struct fi_msg msg;
	struct iovec msg_iov;
	struct fi_context *ctx;

	ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
	if (ret)
		return ret;

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = 0;

	msg.msg_iov = &msg_iov;
	msg.desc = ft_tx_ctrl.iov_desc;
	msg.iov_count = 1;
	msg.addr = ft_tx_ctrl.addr;
	msg.context = ctx;
	msg.data = 0;

	ft_send_retry(ret, fi_sendmsg, ft_tx_ctrl.ep, &msg, FI_COMPLETION);
	ft_tx_ctrl.credits--;

	return ret;
}

static int ft_tmsg_sync(void)
{
	int ret;
	struct fi_msg_tagged msg;
	struct iovec msg_iov;
	struct fi_context *ctx;

	ret = ft_get_ctx(&ft_tx_ctrl, &ctx);
	if (ret)
		return ret;

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = 0;

	msg.msg_iov = &msg_iov;
	msg.desc = ft_tx_ctrl.iov_desc;
	msg.iov_count = 1;
	msg.addr = ft_tx_ctrl.addr;
	msg.context = ctx;
	msg.data = 0;
	msg.tag = ft_tx_ctrl.tag++;
	msg.context = ctx;

	ft_send_retry(ret, fi_tsendmsg, ft_tx_ctrl.ep, &msg, FI_COMPLETION);
	ft_tx_ctrl.credits--;

	return ret;
}

int ft_send_sync_msg(void)
{
	int ret;

	while (ft_tx_ctrl.credits != ft_tx_ctrl.max_credits) {
		ret = ft_comp_tx(0);
		if (ret < 0)
			return ret;
	}

	if (test_info.test_class & FI_TAGGED)
		ret = ft_tmsg_sync();
	else if (test_info.caps & FI_MSG)
		ret = ft_msg_sync();
	else
		ret = ft_rma_sync();

	if (ret)
		return ret;

	ret = ft_comp_tx(FT_COMP_TO);
	return ret < 0 ? ret : 0;
}

int ft_send_msg(void)
{
	int ret;

	while (!ft_tx_ctrl.credits) {
		ret = ft_comp_tx(FT_COMP_TO);
		if (ret < 0)
			return ret;
	}

	if (test_info.test_class & FI_TAGGED) {
		ret = ft_post_tsend();
		if (!ret)
			ft_tx_ctrl.tag++;
	} else {
		ret = ft_post_send();
	}

	if (ret) {
		FT_PRINTERR("send", ret);
		return ret;
	}

	if (is_inject_func(test_info.class_function) &&
	    test_info.test_type == FT_TEST_UNIT)
		memset(ft_tx_ctrl.buf, 0, ft_tx_ctrl.msg_size);

	if (!ft_tx_ctrl.credits) {
		ret = ft_comp_tx(0);
		if (ret < 0)
			return ret;
	}

	return 0;
}

int ft_send_dgram(void)
{
	int ret;

	*(uint8_t*) ft_tx_ctrl.buf = ft_tx_ctrl.seqno++;
	ret = ft_send_msg();
	return ret;
}

int ft_send_dgram_flood(void)
{
	int i, ret = 0;

	ft_tx_ctrl.seqno = 0;
	*(uint8_t*) ft_tx_ctrl.buf = 0;
	for (i = 0; i < ft_ctrl.xfer_iter - 1; i++) {
		ret = ft_send_msg();
		if (ret)
			break;
	}

	return ret;
}

int ft_recv_dgram(void)
{
	int ret;

	do {
		if (ft_rx_ctrl.credits > (ft_rx_ctrl.max_credits >> 1)) {
			ret = ft_post_recv_bufs();
			if (ret)
				return ret;
		}

		ret = ft_comp_rx(FT_DGRAM_POLL_TO);
		if (ret && (*(uint8_t *) ft_rx_ctrl.buf == ft_rx_ctrl.seqno)) {
			ft_rx_ctrl.seqno++;
			return 0;
		}
	} while (ret >= 0);

	return (ret == -FI_EAGAIN) ? -FI_ETIMEDOUT : ret;
}

int ft_recv_dgram_flood(size_t *recv_cnt)
{
	int ret;
	size_t cnt = 0;

	do {
		ret = ft_post_recv_bufs();
		if (ret)
			break;

		ret = ft_comp_rx(0);
		cnt += ret;

	} while ((*(uint8_t *) ft_rx_ctrl.buf != (uint8_t) ~0) || !cnt);

	*recv_cnt = cnt;
	return ret < 0 ? ret : 0;
}

int ft_sendrecv_dgram(void)
{
	int ret, try;

	for (try = 0; try < 1000; try++) {
		ret = ft_send_dgram();
		if (ret)
			return ret;

		ret = ft_recv_dgram();
		if (ret != -FI_ETIMEDOUT)
			break;

		/* resend */
		if (test_info.test_class & FI_TAGGED)
			ft_tx_ctrl.tag--;
		ft_tx_ctrl.seqno--;
	}

	return ret;
}
