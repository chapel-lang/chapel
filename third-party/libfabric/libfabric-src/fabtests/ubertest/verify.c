/*
 * Copyright (c) 2017 Intel Corporation.  All rights reserved.
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
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "ofi_atomic.h"
#include "fabtest.h"

static const char integ_alphabet[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const int integ_alphabet_length = (sizeof(integ_alphabet)/sizeof(*integ_alphabet)) - 1;

#define CHECK_LOCAL(res,local,cnt,ret,TYPE)	\
	do {					\
		int i;				\
		TYPE *r = (res);		\
		TYPE *l = (local);		\
		for (i = 0; i < cnt; i++) {	\
			if (r[i] != l[i]) {	\
				ret = -FI_EIO;	\
				break;		\
			}			\
		}				\
	} while (0)				\


#define FT_FILL(dst,cnt,TYPE)					\
	do {							\
		int i, a = 0;					\
		TYPE *d = (dst);				\
		for (i = 0; i < cnt; i++) {			\
			d[i] = (TYPE) (integ_alphabet[a]);	\
			if (++a >= integ_alphabet_length)	\
				a = 0;				\
		}						\
	} while (0)

#define SWITCH_TYPES(type,FUNC,...)				\
	switch (type) {						\
	case FI_INT8:	FUNC(__VA_ARGS__,int8_t); break;	\
	case FI_UINT8:	FUNC(__VA_ARGS__,uint8_t); break;	\
	case FI_INT16:	FUNC(__VA_ARGS__,int16_t); break;	\
	case FI_UINT16: FUNC(__VA_ARGS__,uint16_t); break;	\
	case FI_INT32:	FUNC(__VA_ARGS__,int32_t); break;	\
	case FI_UINT32: FUNC(__VA_ARGS__,uint32_t); break;	\
	case FI_INT64:	FUNC(__VA_ARGS__,int64_t); break;	\
	case FI_UINT64: FUNC(__VA_ARGS__,uint64_t); break;	\
	case FI_FLOAT:	FUNC(__VA_ARGS__,float); break;		\
	case FI_DOUBLE:	FUNC(__VA_ARGS__,double); break;	\
	case FI_LONG_DOUBLE: FUNC(__VA_ARGS__,long_double); break;		\
	case FI_FLOAT_COMPLEX:	FUNC(__VA_ARGS__,ofi_complex_float); break;	\
	case FI_DOUBLE_COMPLEX:	FUNC(__VA_ARGS__,ofi_complex_double); break;	\
	case FI_LONG_DOUBLE_COMPLEX: FUNC(__VA_ARGS__,ofi_complex_long_double); break;\
	default: return -FI_EOPNOTSUPP;				\
	}

int ft_sync_fill_bufs(size_t size)
{
	int ret;
	ft_sock_sync(0);

	if (test_info.caps & FI_ATOMIC) {
		SWITCH_TYPES(ft_atom_ctrl.datatype, FT_FILL, ft_tx_ctrl.buf,
			     ft_atom_ctrl.count);
		SWITCH_TYPES(ft_atom_ctrl.datatype, FT_FILL, ft_mr_ctrl.buf,
			     ft_atom_ctrl.count);
		memcpy(ft_atom_ctrl.orig_buf, ft_mr_ctrl.buf, size);
		memcpy(ft_tx_ctrl.cpy_buf, ft_tx_ctrl.buf, size);
	} else if (is_read_func(test_info.class_function)) {
		ret = ft_fill_buf(ft_mr_ctrl.buf, size);
		if (ret)
			return ret;
	} else {
		ret = ft_fill_buf(ft_tx_ctrl.buf, size);
		if (ret)
			return ret;

		memcpy(ft_tx_ctrl.cpy_buf, ft_tx_ctrl.buf, size);
	}

	ft_sock_sync(0);

	return 0;
}

static int verify_atomic(void)
{
	int ret = 0;
	void *dst, *src, *cmp, *tmp, *res;
	enum fi_datatype type;
	enum fi_op op;
	size_t count;

	dst = ft_atom_ctrl.orig_buf;
	src = ft_tx_ctrl.cpy_buf;
	cmp = ft_atom_ctrl.comp_buf;
	tmp = ft_rx_ctrl.buf;
	res = ft_atom_ctrl.res_buf;

	type = ft_atom_ctrl.datatype;
	op = ft_atom_ctrl.op;
	count = ft_atom_ctrl.count;

	if (is_fetch_func(test_info.class_function) ||
	    is_compare_func(test_info.class_function)) {
		SWITCH_TYPES(type, CHECK_LOCAL, dst, res, count, ret);
		if (ret)
			return ret;
	}

	if (is_compare_func(test_info.class_function)) {
		ofi_atomic_swap_handler(op, type, dst, src, cmp, tmp, count);
	} else if (is_fetch_func(test_info.class_function)) {
		ofi_atomic_readwrite_handler(op, type, dst, src, tmp, count);
	} else {
		ofi_atomic_write_handler(op, type, dst, src, count);
	}

	SWITCH_TYPES(type, CHECK_LOCAL, dst, ft_mr_ctrl.buf, count, ret);

	return ret;
}

int ft_verify_bufs()
{
	char *compare_buf;
	size_t compare_size;

	if (test_info.caps & FI_ATOMIC)
		return verify_atomic();

	if (test_info.caps & FI_RMA) {
		compare_size = ft_tx_ctrl.rma_msg_size;
		if (is_read_func(test_info.class_function))
			compare_buf = (char *) ft_tx_ctrl.buf;
		else
			compare_buf = (char *) ft_mr_ctrl.buf;
	} else {
		compare_size = ft_tx_ctrl.msg_size;
		compare_buf = (char *) ft_rx_ctrl.buf;
	}

	return ft_check_buf(compare_buf, compare_size);
}

void ft_verify_comp(void *buf)
{
	struct fi_cq_data_entry *comp;

	if (ft_rx_ctrl.cq_format != FI_CQ_FORMAT_DATA)
		return;

	comp = (struct fi_cq_data_entry *) buf;

	if (comp->flags & FI_REMOTE_CQ_DATA) {
		if (comp->data == ft_tx_ctrl.remote_cq_data)
			ft_ctrl.verify_cnt++;
	}
}
