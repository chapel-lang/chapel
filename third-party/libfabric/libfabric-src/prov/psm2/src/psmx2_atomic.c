/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"
#include "psmx2_trigger.h"

/* Atomics protocol:
 *
 * Atomics REQ:
 *	args[0].u32w0	cmd
 *	args[0].u32w1	count
 *	args[1].u64	req
 *	args[2].u64	addr
 *	args[3].u64	key
 *	args[4].u32w0	datatype
 *	args[4].u32w1	op
 *
 * Atomics REP:
 *	args[0].u32w0	cmd
 *	args[0].u32w1	error
 *	args[1].u64	req
 */

static ofi_spin_t psmx2_atomic_lock;

void psmx2_atomic_global_init(void)
{
	ofi_spin_init(&psmx2_atomic_lock);
}

void psmx2_atomic_global_fini(void)
{
	ofi_spin_destroy(&psmx2_atomic_lock);
}

static inline void psmx2_ioc_read(const struct fi_ioc *ioc, size_t count,
				  int datatype, uint8_t *buf, size_t len)
{
	int i;
	size_t copy_len;

	for (i=0; i<count && len; i++) {
		copy_len = ofi_datatype_size(datatype) * ioc[i].count;
		if (copy_len > len)
			copy_len = len;
		memcpy(buf, ioc[i].addr, copy_len);
		buf += copy_len;
		len -= copy_len;
	}
}

static inline void psmx2_ioc_write(struct fi_ioc *ioc, size_t count,
				   int datatype, const uint8_t *buf, size_t len)
{
	int i;
	size_t copy_len;

	for (i=0; i<count && len; i++) {
		copy_len = ofi_datatype_size(datatype) * ioc[i].count;
		if (copy_len > len)
			copy_len = len;
		memcpy(ioc[i].addr, buf, copy_len);
		buf += copy_len;
		len -= copy_len;
	}
}

static inline size_t psmx2_ioc_size(const struct fi_ioc *ioc, size_t count,
				    int datatype)
{
	int i;
	size_t len = 0;

	for (i=0; i<count; i++)
		len += ofi_datatype_size(datatype) * ioc[i].count;

	return len;
}

#define CASE_INT_TYPE(FUNC,...) \
		case FI_INT8:	FUNC(__VA_ARGS__,int8_t); break; \
		case FI_UINT8:	FUNC(__VA_ARGS__,uint8_t); break; \
		case FI_INT16:	FUNC(__VA_ARGS__,int16_t); break; \
		case FI_UINT16: FUNC(__VA_ARGS__,uint16_t); break; \
		case FI_INT32:	FUNC(__VA_ARGS__,int32_t); break; \
		case FI_UINT32: FUNC(__VA_ARGS__,uint32_t); break; \
		case FI_INT64:	FUNC(__VA_ARGS__,int64_t); break; \
		case FI_UINT64: FUNC(__VA_ARGS__,uint64_t); break;

#define CASE_FP_TYPE(FUNC,...) \
		case FI_FLOAT:	FUNC(__VA_ARGS__,float); break; \
		case FI_DOUBLE:	FUNC(__VA_ARGS__,double); break; \
		case FI_LONG_DOUBLE: FUNC(__VA_ARGS__,long double); break;

#define CASE_COMPLEX_TYPE(FUNC,...) \
		case FI_FLOAT_COMPLEX:	FUNC(__VA_ARGS__,float complex); break; \
		case FI_DOUBLE_COMPLEX:	FUNC(__VA_ARGS__,double complex); break; \
		case FI_LONG_DOUBLE_COMPLEX: FUNC(__VA_ARGS__,long double complex); break;

#define SWITCH_INT_TYPE(type,...) \
		switch (type) { \
		CASE_INT_TYPE(__VA_ARGS__) \
		default: return -FI_EOPNOTSUPP; \
		}

#define SWITCH_ORD_TYPE(type,...) \
		switch (type) { \
		CASE_INT_TYPE(__VA_ARGS__) \
		CASE_FP_TYPE(__VA_ARGS__) \
		default: return -FI_EOPNOTSUPP; \
		}

#define SWITCH_ALL_TYPE(type,...) \
		switch (type) { \
		CASE_INT_TYPE(__VA_ARGS__) \
		CASE_FP_TYPE(__VA_ARGS__) \
		CASE_COMPLEX_TYPE(__VA_ARGS__) \
		default: return -FI_EOPNOTSUPP; \
		}

#define PSMX2_MIN(dst,src)	if ((dst) > (src)) (dst) = (src)
#define PSMX2_MAX(dst,src)	if ((dst) < (src)) (dst) = (src)
#define PSMX2_SUM(dst,src)	(dst) += (src)
#define PSMX2_PROD(dst,src)	(dst) *= (src)
#define PSMX2_LOR(dst,src)	(dst) = (dst) || (src)
#define PSMX2_LAND(dst,src)	(dst) = (dst) && (src)
#define PSMX2_BOR(dst,src)	(dst) |= (src)
#define PSMX2_BAND(dst,src)	(dst) &= (src)
#define PSMX2_LXOR(dst,src)	(dst) = ((dst) && !(src)) || (!(dst) && (src))
#define PSMX2_BXOR(dst,src)	(dst) ^= (src)
#define PSMX2_COPY(dst,src)	(dst) = (src)

#define PSMX2_ATOMIC_READ(dst,res,cnt,TYPE) \
		do { \
			int i; \
			TYPE *d = (dst); \
			TYPE *r = (res); \
			psmx2_lock(&psmx2_atomic_lock, 1); \
			for (i=0; i<(cnt); i++) \
				r[i] = d[i]; \
			psmx2_unlock(&psmx2_atomic_lock, 1); \
		} while (0)

#define PSMX2_ATOMIC_WRITE(dst,src,cnt,OP,TYPE) \
		do { \
			int i; \
			TYPE *d = (dst); \
			TYPE *s = (src); \
			psmx2_lock(&psmx2_atomic_lock, 1); \
			for (i=0; i<cnt; i++) \
				OP(d[i],s[i]); \
			psmx2_unlock(&psmx2_atomic_lock, 1); \
		} while (0)

#define PSMX2_ATOMIC_READWRITE(dst,src,res,cnt,OP,TYPE) \
		do { \
			int i; \
			TYPE *d = (dst); \
			TYPE *s = (src); \
			TYPE *r = (res); \
			psmx2_lock(&psmx2_atomic_lock, 1); \
			for (i=0; i<(cnt); i++) {\
				r[i] = d[i]; \
				OP(d[i],s[i]); \
			} \
			psmx2_unlock(&psmx2_atomic_lock, 1); \
		} while (0)

#define PSMX2_ATOMIC_CSWAP(dst,src,cmp,res,cnt,CMP_OP,TYPE) \
		do { \
			int i; \
			TYPE *d = (dst); \
			TYPE *s = (src); \
			TYPE *c = (cmp); \
			TYPE *r = (res); \
			psmx2_lock(&psmx2_atomic_lock, 1); \
			for (i=0; i<(cnt); i++) { \
				r[i] = d[i]; \
				if (c[i] CMP_OP d[i]) \
					d[i] = s[i]; \
			} \
			psmx2_unlock(&psmx2_atomic_lock, 1); \
		} while (0)

#define PSMX2_ATOMIC_MSWAP(dst,src,cmp,res,cnt,TYPE) \
		do { \
			int i; \
			TYPE *d = (dst); \
			TYPE *s = (src); \
			TYPE *c = (cmp); \
			TYPE *r = (res); \
			psmx2_lock(&psmx2_atomic_lock, 1); \
			for (i=0; i<(cnt); i++) { \
				r[i] = d[i]; \
				d[i] = (s[i] & c[i]) | (d[i] & ~c[i]); \
			} \
			psmx2_unlock(&psmx2_atomic_lock, 1); \
		} while (0)

static int psmx2_atomic_do_write(void *dest, void *src,
				 int datatype, int op, int count)
{
	switch (op) {
	case FI_MIN:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_MIN);
		break;

	case FI_MAX:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_MAX);
		break;

	case FI_SUM:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_SUM);
		break;

	case FI_PROD:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_PROD);
		break;

	case FI_LOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_LOR);
		break;

	case FI_LAND:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_LAND);
		break;

	case FI_BOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_BOR);
		break;

	case FI_BAND:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_BAND);
		break;

	case FI_LXOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_LXOR);
		break;

	case FI_BXOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_BXOR);
		break;

	case FI_ATOMIC_WRITE:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_WRITE,
				dest,src,count,PSMX2_COPY);
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	return 0;
}

static int psmx2_atomic_do_readwrite(void *dest, void *src, void *result,
				     int datatype, int op, int count)
{
	switch (op) {
	case FI_MIN:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_MIN);
		break;

	case FI_MAX:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_MAX);
		break;

	case FI_SUM:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_SUM);
		break;

	case FI_PROD:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_PROD);
		break;

	case FI_LOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_LOR);
		break;

	case FI_LAND:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_LAND);
		break;

	case FI_BOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_BOR);
		break;

	case FI_BAND:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_BAND);
		break;

	case FI_LXOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_LXOR);
		break;

	case FI_BXOR:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_BXOR);
		break;

	case FI_ATOMIC_READ:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_READ,
				dest,result,count);
		break;

	case FI_ATOMIC_WRITE:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_READWRITE,
				dest,src,result,count,PSMX2_COPY);
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	return 0;
}

static int psmx2_atomic_do_compwrite(void *dest, void *src, void *compare,
				     void *result, int datatype, int op,
				     int count)
{
	switch (op) {
	case FI_CSWAP:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,==);
		break;

	case FI_CSWAP_NE:
		SWITCH_ALL_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,!=);
		break;

	case FI_CSWAP_LE:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,<=);
		break;

	case FI_CSWAP_LT:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,<);
		break;

	case FI_CSWAP_GE:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,>=);
		break;

	case FI_CSWAP_GT:
		SWITCH_ORD_TYPE(datatype,PSMX2_ATOMIC_CSWAP,
				dest,src,compare,result,count,>);
		break;

	case FI_MSWAP:
		SWITCH_INT_TYPE(datatype,PSMX2_ATOMIC_MSWAP,
				dest,src,compare,result,count);
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	return 0;
}

int psmx2_am_atomic_handler(psm2_am_token_t token,
				psm2_amarg_t *args, int nargs, void *src,
				uint32_t len, void *hctx)
{
	psm2_amarg_t rep_args[8];
	int count;
	uint8_t *addr;
	uint64_t key;
	int datatype, op;
	int err = 0;
	int op_error = 0;
	struct psmx2_am_request *req;
	struct psmx2_cq_event *event;
	struct psmx2_fid_mr *mr;
	struct psmx2_fid_cntr *cntr = NULL;
	struct psmx2_fid_cntr *mr_cntr = NULL;
	void *tmp_buf;
	psm2_epaddr_t epaddr;
	int cmd;
	struct psmx2_trx_ctxt *rx;

	psm2_am_get_source(token, &epaddr);
	cmd = PSMX2_AM_GET_OP(args[0].u32w0);

	switch (cmd) {
	case PSMX2_AM_REQ_ATOMIC_WRITE:
		rx = (struct psmx2_trx_ctxt *)hctx;
		count = args[0].u32w1;
		addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		datatype = args[4].u32w0;
		op = args[4].u32w1;
		assert(len == ofi_datatype_size(datatype) * count);

		mr = psmx2_mr_get(rx->domain, key);
		op_error = mr ?
			psmx2_mr_validate(mr, (uint64_t)addr, len, FI_REMOTE_WRITE) :
			-FI_EINVAL;

		if (!op_error) {
			addr += mr->offset;
			psmx2_atomic_do_write(addr, src, datatype, op, count);

			if (rx->ep->caps & FI_RMA_EVENT) {
				cntr = rx->ep->remote_write_cntr;
				mr_cntr = mr->cntr;

				if (cntr)
					psmx2_cntr_inc(cntr, 0);

				if (mr_cntr && mr_cntr != cntr)
					psmx2_cntr_inc(mr_cntr, 0);
			}
		}

		rep_args[0].u32w0 = PSMX2_AM_REP_ATOMIC_WRITE;
		rep_args[0].u32w1 = op_error;
		rep_args[1].u64 = args[1].u64;
		err = psm2_am_reply_short(token, PSMX2_AM_ATOMIC_HANDLER,
					  rep_args, 2, NULL, 0, 0,
					  NULL, NULL );
		break;

	case PSMX2_AM_REQ_ATOMIC_READWRITE:
		rx = (struct psmx2_trx_ctxt *)hctx;
		count = args[0].u32w1;
		addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		datatype = args[4].u32w0;
		op = args[4].u32w1;

		if (op == FI_ATOMIC_READ)
			len = ofi_datatype_size(datatype) * count;

		assert(len == ofi_datatype_size(datatype) * count);

		mr = psmx2_mr_get(rx->domain, key);
		op_error = mr ?
			psmx2_mr_validate(mr, (uint64_t)addr, len,
					  FI_REMOTE_READ|FI_REMOTE_WRITE) :
			-FI_EINVAL;

		if (!op_error) {
			addr += mr->offset;
			tmp_buf = malloc(len);
			if (tmp_buf)
				psmx2_atomic_do_readwrite(addr, src, tmp_buf,
							  datatype, op, count);
			else
				op_error = -FI_ENOMEM;

			if (rx->ep->caps & FI_RMA_EVENT) {
				if (op == FI_ATOMIC_READ) {
					cntr = rx->ep->remote_read_cntr;
				} else {
					cntr = rx->ep->remote_write_cntr;
					mr_cntr = mr->cntr;
				}

				if (cntr)
					psmx2_cntr_inc(cntr, 0);

				if (mr_cntr && mr_cntr != cntr)
					psmx2_cntr_inc(mr_cntr, 0);
			}
		} else {
			tmp_buf = NULL;
		}

		rep_args[0].u32w0 = PSMX2_AM_REP_ATOMIC_READWRITE;
		rep_args[0].u32w1 = op_error;
		rep_args[1].u64 = args[1].u64;
		err = psm2_am_reply_short(token, PSMX2_AM_ATOMIC_HANDLER,
					  rep_args, 2, tmp_buf,
					  (tmp_buf ? len : 0),
					  0, free, tmp_buf );
		break;

	case PSMX2_AM_REQ_ATOMIC_COMPWRITE:
		rx = (struct psmx2_trx_ctxt *)hctx;
		count = args[0].u32w1;
		addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		datatype = args[4].u32w0;
		op = args[4].u32w1;
		len /= 2;
		assert(len == ofi_datatype_size(datatype) * count);

		mr = psmx2_mr_get(rx->domain, key);
		op_error = mr ?
			psmx2_mr_validate(mr, (uint64_t)addr, len,
					  FI_REMOTE_READ|FI_REMOTE_WRITE) :
			-FI_EINVAL;

		if (!op_error) {
			addr += mr->offset;
			tmp_buf = malloc(len);
			if (tmp_buf)
				psmx2_atomic_do_compwrite(addr, src, (uint8_t *)src + len,
							  tmp_buf, datatype,
							  op, count);
			else
				op_error = -FI_ENOMEM;

			if (rx->ep->caps & FI_RMA_EVENT) {
				cntr = rx->ep->remote_write_cntr;
				mr_cntr = mr->cntr;

				if (cntr)
					psmx2_cntr_inc(cntr, 0);

				if (mr_cntr && mr_cntr != cntr)
					psmx2_cntr_inc(mr_cntr, 0);
			}
		} else {
			tmp_buf = NULL;
		}

		rep_args[0].u32w0 = PSMX2_AM_REP_ATOMIC_READWRITE;
		rep_args[0].u32w1 = op_error;
		rep_args[1].u64 = args[1].u64;
		err = psm2_am_reply_short(token, PSMX2_AM_ATOMIC_HANDLER,
					  rep_args, 2, tmp_buf,
					  (tmp_buf ? len : 0),
					  0, free, tmp_buf );
		break;

	case PSMX2_AM_REP_ATOMIC_WRITE:
		req = (struct psmx2_am_request *)(uintptr_t)args[1].u64;
		op_error = (int)args[0].u32w1;
		assert(req->op == PSMX2_AM_REQ_ATOMIC_WRITE);
		if (req->ep->send_cq && (!req->no_event || op_error)) {
			event = psmx2_cq_create_event(
					req->ep->send_cq,
					req->atomic.context,
					req->atomic.buf,
					req->cq_flags,
					req->atomic.len,
					0, /* data */
					0, /* tag */
					0, /* olen */
					op_error);
			if (event)
				psmx2_cq_enqueue_event(req->ep->send_cq, event);
			else
				err = -FI_ENOMEM;
		}

		if (req->ep->write_cntr)
			psmx2_cntr_inc(req->ep->write_cntr, op_error);

		free(req->tmpbuf);
		psmx2_am_request_free(req->ep->tx, req);
		break;

	case PSMX2_AM_REP_ATOMIC_READWRITE:
	case PSMX2_AM_REP_ATOMIC_COMPWRITE:
		req = (struct psmx2_am_request *)(uintptr_t)args[1].u64;
		op_error = (int)args[0].u32w1;
		assert(op_error || req->atomic.len == len);

		if (!op_error) {
			if (req->atomic.result)
				memcpy(req->atomic.result, src, len);
			else
				psmx2_ioc_write(req->ioc, req->atomic.iov_count,
						req->atomic.datatype, src, len);
		}

		if (req->ep->send_cq && (!req->no_event || op_error)) {
			event = psmx2_cq_create_event(
					req->ep->send_cq,
					req->atomic.context,
					req->atomic.buf,
					req->cq_flags,
					req->atomic.len,
					0, /* data */
					0, /* tag */
					0, /* olen */
					op_error);
			if (event)
				psmx2_cq_enqueue_event(req->ep->send_cq, event);
			else
				err = -FI_ENOMEM;
		}

		if (req->ep->read_cntr)
			psmx2_cntr_inc(req->ep->read_cntr, op_error);

		free(req->tmpbuf);
		psmx2_am_request_free(req->ep->tx, req);
		break;

	default:
		err = -FI_EINVAL;
	}
	return err;
}

static int psmx2_atomic_self(int am_cmd,
			     struct psmx2_fid_ep *ep,
			     const void *buf,
			     size_t count, void *desc,
			     const void *compare, void *compare_desc,
			     void *result, void *result_desc,
			     uint64_t addr, uint64_t key,
			     enum fi_datatype datatype,
			     enum fi_op op, void *context,
			     uint64_t flags)
{
	struct psmx2_fid_mr *mr;
	struct psmx2_cq_event *event;
	struct psmx2_fid_cntr *cntr = NULL;
	struct psmx2_fid_cntr *mr_cntr = NULL;
	void *tmp_buf;
	size_t len;
	int no_event;
	int err = 0;
	int op_error;
	int access;
	uint64_t cq_flags = 0;

	if (am_cmd == PSMX2_AM_REQ_ATOMIC_WRITE)
		access = FI_REMOTE_WRITE;
	else
		access = FI_REMOTE_READ | FI_REMOTE_WRITE;

	len = ofi_datatype_size(datatype) * count;
	mr = psmx2_mr_get(ep->domain, key);
	op_error = mr ?  psmx2_mr_validate(mr, addr, len, access) : -FI_EINVAL;

	if (op_error)
		goto gen_local_event;

	addr += mr->offset;

	switch (am_cmd) {
	case PSMX2_AM_REQ_ATOMIC_WRITE:
		err = psmx2_atomic_do_write((void *)addr, (void *)buf,
					    (int)datatype, (int)op, (int)count);
		cq_flags = FI_WRITE | FI_ATOMIC;
		break;

	case PSMX2_AM_REQ_ATOMIC_READWRITE:
		if (result != buf) {
			err = psmx2_atomic_do_readwrite((void *)addr, (void *)buf,
							(void *)result, (int)datatype,
							(int)op, (int)count);
		} else {
			tmp_buf = malloc(len);
			if (tmp_buf) {
				memcpy(tmp_buf, result, len);
				err = psmx2_atomic_do_readwrite((void *)addr, (void *)buf,
								tmp_buf, (int)datatype,
								(int)op, (int)count);
				memcpy(result, tmp_buf, len);
				free(tmp_buf);
			} else {
				err = -FI_ENOMEM;
			}
		}
		if (op == FI_ATOMIC_READ)
			cq_flags = FI_READ | FI_ATOMIC;
		else
			cq_flags = FI_WRITE | FI_ATOMIC;
		break;

	case PSMX2_AM_REQ_ATOMIC_COMPWRITE:
		if (result != buf && result != compare) {
			err = psmx2_atomic_do_compwrite((void *)addr, (void *)buf,
							(void *)compare, (void *)result,
							(int)datatype, (int)op, (int)count);
		} else {
			tmp_buf = malloc(len);
			if (tmp_buf) {
				memcpy(tmp_buf, result, len);
				err = psmx2_atomic_do_compwrite((void *)addr, (void *)buf,
								(void *)compare, tmp_buf,
								(int)datatype, (int)op, (int)count);
				memcpy(result, tmp_buf, len);
				free(tmp_buf);
			} else {
				err = -FI_ENOMEM;
			}
		}
		cq_flags = FI_WRITE | FI_ATOMIC;
		break;
	}

	if (ep->caps & FI_RMA_EVENT) {
		if (op == FI_ATOMIC_READ) {
			cntr = ep->remote_read_cntr;
		} else {
			cntr = ep->remote_write_cntr;
			mr_cntr = mr->cntr;
		}

		if (cntr)
			psmx2_cntr_inc(cntr, 0);

		if (mr_cntr && mr_cntr != cntr)
			psmx2_cntr_inc(mr_cntr, 0);
	}

	op_error = err;

gen_local_event:
	no_event = ((flags & PSMX2_NO_COMPLETION) ||
		    (ep->send_selective_completion && !(flags & FI_COMPLETION)));
	if (ep->send_cq && (!no_event || op_error)) {
		event = psmx2_cq_create_event(
				ep->send_cq,
				context,
				(void *)buf,
				cq_flags,
				len,
				0, /* data */
				0, /* tag */
				0, /* olen */
				op_error);
		if (event)
			psmx2_cq_enqueue_event(ep->send_cq, event);
		else
			err = -FI_ENOMEM;
	}

	switch (am_cmd) {
	case PSMX2_AM_REQ_ATOMIC_WRITE:
		if (ep->write_cntr)
			psmx2_cntr_inc(ep->write_cntr, op_error);
		break;
	case PSMX2_AM_REQ_ATOMIC_READWRITE:
	case PSMX2_AM_REQ_ATOMIC_COMPWRITE:
		if (ep->read_cntr)
			psmx2_cntr_inc(ep->read_cntr, op_error);
		break;
	}

	return err;
}

ssize_t psmx2_atomic_write_generic(struct fid_ep *ep,
				   const void *buf,
				   size_t count, void *desc,
				   fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key,
				   enum fi_datatype datatype,
				   enum fi_op op, void *context,
				   uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size, len;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_write(ep, buf, count, desc,
							dest_addr, addr, key,
							datatype, op, context,
							flags);

	assert(buf);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid)
		return psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_WRITE, ep_priv,
					 buf, count, desc, NULL, NULL, NULL,
					 NULL, addr, key, datatype, op,
					 context, flags);

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	len = ofi_datatype_size(datatype)* count;
	if (len > chunk_size)
		return -FI_EMSGSIZE;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if (flags & FI_INJECT) {
		req->tmpbuf = malloc(len);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		memcpy(req->tmpbuf, (void *)buf, len);
		buf = req->tmpbuf;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_WRITE;
	req->atomic.buf = (void *)buf;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_WRITE;
	args[0].u32w1 = count;
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    (void *)buf, len, am_flags, NULL, NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

ssize_t psmx2_atomic_writev_generic(struct fid_ep *ep,
				   const struct fi_ioc *iov,
				   void **desc, size_t count,
				   fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key,
				   enum fi_datatype datatype,
				   enum fi_op op, void *context,
				   uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size;
	size_t len;
	uint8_t *buf;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_writev(ep, iov, desc, count,
							 dest_addr, addr, key,
							 datatype, op, context,
							 flags);

	assert(iov);
	assert(count);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	while (count && !iov[count-1].count)
		count--;

	av = ep_priv->av;
	assert(av);

	len = psmx2_ioc_size(iov, count, datatype);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid) {
		buf = malloc(len);
		if (!buf)
			return -FI_ENOMEM;

		psmx2_ioc_read(iov, count, datatype, buf, len);

		err = psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_WRITE, ep_priv,
					buf, len / ofi_datatype_size(datatype),
					NULL, NULL, NULL, NULL, NULL, addr,
					key, datatype, op, context, flags);

		free(buf);
		return err;
	}

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	if (len > chunk_size)
		return -FI_EMSGSIZE;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if (count > 1) {
		req->tmpbuf = malloc(len);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		buf = req->tmpbuf;
		psmx2_ioc_read(iov, count, datatype, buf, len);
	} else {
		buf = iov[0].addr;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_WRITE;
	req->atomic.buf = (void *)buf;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_WRITE;
	args[0].u32w1 = len / ofi_datatype_size(datatype);
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    (void *)buf, len, am_flags, NULL, NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_write(struct fid_ep *ep,
				  const void *buf,
				  size_t count, void *desc,
				  fi_addr_t dest_addr,
				  uint64_t addr, uint64_t key,
				  enum fi_datatype datatype,
				  enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	return psmx2_atomic_write_generic(ep, buf, count, desc, dest_addr,
					  addr, key, datatype, op, context,
					  ep_priv->tx_flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_writemsg(struct fid_ep *ep,
				const struct fi_msg_atomic *msg,
				uint64_t flags)
{
	assert(msg);
	assert(msg->iov_count);
	assert(msg->msg_iov);
	assert(msg->rma_iov);
	assert(msg->rma_iov_count == 1);

	if (msg->iov_count > 1)
		return psmx2_atomic_writev_generic(ep, msg->msg_iov, msg->desc,
						   msg->iov_count, msg->addr,
						   msg->rma_iov[0].addr,
						   msg->rma_iov[0].key,
						   msg->datatype, msg->op,
						   msg->context, flags);

	return psmx2_atomic_write_generic(ep, msg->msg_iov[0].addr,
					  msg->msg_iov[0].count,
					  msg->desc ? msg->desc[0] : NULL,
					  msg->addr, msg->rma_iov[0].addr,
					  msg->rma_iov[0].key, msg->datatype,
					  msg->op, msg->context, flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_writev(struct fid_ep *ep,
				   const struct fi_ioc *iov,
				   void **desc, size_t count,
				   fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key,
				   enum fi_datatype datatype,
				   enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	assert(iov);
	assert(count);

	if (count > 1)
		return psmx2_atomic_writev_generic(ep, iov, desc, count,
						   dest_addr, addr, key,
						   datatype, op, context,
						   ep_priv->tx_flags);

	return psmx2_atomic_write_generic(ep, iov->addr, iov->count,
					  desc ? desc[0] : NULL, dest_addr,
					  addr, key, datatype, op, context,
					  ep_priv->tx_flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_inject(struct fid_ep *ep,
				   const void *buf,
				   size_t count, /*void *desc,*/
				   fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key,
				   enum fi_datatype datatype,
				   enum fi_op op)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	return psmx2_atomic_write_generic(ep, buf, count, NULL/*desc*/,
					  dest_addr, addr, key,
					  datatype, op, NULL,
					  ep_priv->tx_flags | FI_INJECT | PSMX2_NO_COMPLETION);
}

ssize_t psmx2_atomic_readwrite_generic(struct fid_ep *ep,
				       const void *buf,
				       size_t count, void *desc,
				       void *result, void *result_desc,
				       fi_addr_t dest_addr,
				       uint64_t addr, uint64_t key,
				       enum fi_datatype datatype,
				       enum fi_op op, void *context,
				       uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size, len;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_readwrite(ep, buf, count,
							    desc, result,
							    result_desc,
							    dest_addr, addr,
							    key, datatype, op,
							    context, flags);

	assert(buf || op == FI_ATOMIC_READ);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid)
		return psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_READWRITE, ep_priv,
					 buf, count, desc, NULL, NULL, result,
					 result_desc, addr, key, datatype, op,
					 context, flags);

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	len = ofi_datatype_size(datatype) * count;
	if (len > chunk_size)
		return -FI_EMSGSIZE;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if ((flags & FI_INJECT) && op != FI_ATOMIC_READ) {
		req->tmpbuf = malloc(len);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		memcpy(req->tmpbuf, (void *)buf, len);
		buf = req->tmpbuf;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_READWRITE;
	req->atomic.buf = (void *)buf;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.result = result;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	if (op == FI_ATOMIC_READ)
		req->cq_flags = FI_READ | FI_ATOMIC;
	else
		req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_READWRITE;
	args[0].u32w1 = count;
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    (void *)buf, (buf?len:0), am_flags, NULL,
				    NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

ssize_t psmx2_atomic_readwritev_generic(struct fid_ep *ep,
					const struct fi_ioc *iov,
					void **desc, size_t count,
					struct fi_ioc *resultv,
					void **result_desc,
					size_t result_count,
					fi_addr_t dest_addr,
					uint64_t addr, uint64_t key,
					enum fi_datatype datatype,
					enum fi_op op, void *context,
					uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size;
	size_t len, result_len, iov_size;
	uint8_t *buf, *result;
	void *desc0, *result_desc0;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_readwritev(ep, iov, desc,
							     count, resultv,
							     result_desc,
							     result_count,
							     dest_addr, addr,
							     key, datatype, op,
							     context, flags);

	assert((iov && count) || op == FI_ATOMIC_READ);
	assert(resultv);
	assert(result_count);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	if (iov) {
		while (count && !iov[count-1].count)
			count--;
	}

	while (result_count && !resultv[result_count-1].count)
		result_count--;

	result_len = psmx2_ioc_size(resultv, result_count, datatype);

	if (op != FI_ATOMIC_READ) {
		buf = iov[0].addr; /* as default for count == 1 */
		len = psmx2_ioc_size(iov, count, datatype);
		desc0 = desc ? desc[0] : NULL;
	} else {
		buf = NULL;
		len = result_len;
		desc0 = NULL;
	}

	assert(result_len >= len);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid) {
		if (buf && count > 1) {
			buf = malloc(len);
			psmx2_ioc_read(iov, count, datatype, buf, len);
			desc0 = NULL;
		}

		if (result_count > 1) {
			result = malloc(len);
			if (!result) {
				if (buf && count > 1)
					free(buf);
				return -FI_ENOMEM;
			}
			result_desc0 = result_desc ? result_desc[0] : NULL;
		} else {
			result = resultv[0].addr;
			result_desc0 = NULL;
		}

		err = psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_READWRITE, ep_priv,
					buf, len / ofi_datatype_size(datatype),
					desc0, NULL, NULL, result, result_desc0,
					addr, key, datatype, op, context, flags);

		if (result_count > 1) {
			psmx2_ioc_write(resultv, result_count, datatype, result, len);
			free(result);
		}

		if (buf && count > 1)
			free(buf);

		return err;
	}

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	if (len > chunk_size)
		return -FI_EMSGSIZE;

	iov_size = result_count > 1 ? result_count * sizeof(struct fi_ioc) : 0;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if (((flags & FI_INJECT) || count > 1) && op != FI_ATOMIC_READ) {
		req->tmpbuf = malloc(iov_size + len);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		buf = (uint8_t *)req->tmpbuf + iov_size;
		psmx2_ioc_read(iov, count, datatype, buf, len);
	} else {
		req->tmpbuf = malloc(iov_size);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}
	}

	req->ioc = req->tmpbuf;
	if (iov_size) {
		memcpy(req->ioc, resultv, iov_size);
		req->atomic.iov_count = result_count;
		req->atomic.result = NULL;
	} else {
		req->atomic.buf = buf;
		req->atomic.result = resultv[0].addr;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_READWRITE;
	req->atomic.buf = (void *)buf;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	if (op == FI_ATOMIC_READ)
		req->cq_flags = FI_READ | FI_ATOMIC;
	else
		req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_READWRITE;
	args[0].u32w1 = len / ofi_datatype_size(datatype);
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    (void *)buf, (buf?len:0), am_flags, NULL,
				    NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_readwrite(struct fid_ep *ep,
				      const void *buf,
				      size_t count, void *desc,
				      void *result, void *result_desc,
				      fi_addr_t dest_addr,
				      uint64_t addr, uint64_t key,
				      enum fi_datatype datatype,
				      enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	return psmx2_atomic_readwrite_generic(ep, buf, count, desc,
					      result, result_desc, dest_addr,
					      addr, key, datatype, op,
					      context, ep_priv->tx_flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_readwritemsg(struct fid_ep *ep,
					 const struct fi_msg_atomic *msg,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 uint64_t flags)
{
	void *buf;
	size_t count;
	void *desc;

	assert(msg);
	assert(msg->rma_iov);
	assert(msg->rma_iov_count ==1);
	assert(resultv);
	assert(result_count);
	assert((msg->msg_iov && msg->iov_count) || msg->op == FI_ATOMIC_READ);

	if ((msg->op != FI_ATOMIC_READ && msg->iov_count > 1) ||
	    result_count > 1)
		return psmx2_atomic_readwritev_generic(ep, msg->msg_iov, msg->desc,
						       msg->iov_count, resultv,
						       result_desc, result_count,
						       msg->addr,
						       msg->rma_iov[0].addr,
						       msg->rma_iov[0].key,
						       msg->datatype, msg->op,
						       msg->context, flags);

	if (msg->op == FI_ATOMIC_READ) {
		buf = NULL;
		count = resultv[0].count;
		desc = result_desc ? result_desc[0] : NULL;
	} else {
		buf = msg->msg_iov[0].addr;
		count = msg->msg_iov[0].count;
		desc = msg->desc ? msg->desc[0] : NULL;
	}

	return psmx2_atomic_readwrite_generic(ep, buf, count, desc, resultv[0].addr,
					      result_desc ? result_desc[0] : NULL,
					      msg->addr, msg->rma_iov[0].addr,
					      msg->rma_iov[0].key, msg->datatype,
					      msg->op, msg->context, flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_readwritev(struct fid_ep *ep,
				       const struct fi_ioc *iov,
				       void **desc, size_t count,
				       struct fi_ioc *resultv,
				       void **result_desc, size_t result_count,
				       fi_addr_t dest_addr,
				       uint64_t addr, uint64_t key,
				       enum fi_datatype datatype,
				       enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;
	void *buf;
	void *src_desc;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	assert(resultv);
	assert(result_count);
	assert((iov && count) || op == FI_ATOMIC_READ);

	if ((op != FI_ATOMIC_READ && count > 1) || result_count > 1)
		return psmx2_atomic_readwritev_generic(ep, iov, desc, count,
					      resultv, result_desc, result_count,
					      dest_addr, addr, key, datatype, op,
					      context, ep_priv->tx_flags);

	if (op == FI_ATOMIC_READ) {
		buf = NULL;
		count = resultv[0].count;
		src_desc = result_desc ? result_desc[0] : NULL;
	} else {
		buf = iov[0].addr;
		count = iov[0].count;
		src_desc = desc ? desc[0] : NULL;
	}

	return psmx2_atomic_readwrite_generic(ep, buf, count, src_desc, resultv[0].addr,
					      result_desc ? result_desc[0] : NULL,
					      dest_addr, addr, key, datatype, op,
					      context, ep_priv->tx_flags);
}

ssize_t psmx2_atomic_compwrite_generic(struct fid_ep *ep,
				       const void *buf,
				       size_t count, void *desc,
				       const void *compare, void *compare_desc,
				       void *result, void *result_desc,
				       fi_addr_t dest_addr,
				       uint64_t addr, uint64_t key,
				       enum fi_datatype datatype,
				       enum fi_op op, void *context,
				       uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size, len;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_compwrite(ep, buf, count,
							    desc, compare,
							    compare_desc,
							    result, result_desc,
							    dest_addr, addr,
							    key, datatype, op,
							    context, flags);

	assert(buf);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid)
		return psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_COMPWRITE, ep_priv,
					 buf, count, desc, compare,
					 compare_desc, result, result_desc,
					 addr, key, datatype, op,
					 context, flags);

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	len = ofi_datatype_size(datatype) * count;
	if (len * 2 > chunk_size)
		return -FI_EMSGSIZE;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if ((flags & FI_INJECT) ||
	    ((uintptr_t)compare != (uintptr_t)buf + len)) {
		req->tmpbuf = malloc(len * 2);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}
		memcpy(req->tmpbuf, buf, len);
		memcpy((uint8_t *)req->tmpbuf + len, compare, len);
		buf = req->tmpbuf;
		compare = (uint8_t *)buf + len;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_COMPWRITE;
	req->atomic.buf = (void *)buf;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.result = result;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_COMPWRITE;
	args[0].u32w1 = count;
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    (void *)buf, len * 2, am_flags,
				    NULL, NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

ssize_t psmx2_atomic_compwritev_generic(struct fid_ep *ep,
					const struct fi_ioc *iov,
					void **desc, size_t count,
					const struct fi_ioc *comparev,
					void **compare_desc,
					size_t compare_count,
					struct fi_ioc *resultv,
					void **result_desc,
					size_t result_count,
					fi_addr_t dest_addr,
					uint64_t addr, uint64_t key,
					enum fi_datatype datatype,
					enum fi_op op, void *context,
					uint64_t flags)
{
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_av *av;
	struct psmx2_am_request *req;
	psm2_amarg_t args[8];
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size;
	size_t len, iov_size;
	uint8_t *buf, *compare, *result;
	void *desc0, *compare_desc0, *result_desc0;
	int err;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx2_trigger_queue_atomic_compwritev(ep, iov, desc,
							     count, comparev,
							     compare_desc,
							     compare_count,
							     resultv,
							     result_desc,
							     result_count,
							     dest_addr, addr,
							     key, datatype, op,
							     context, flags);

	assert(iov);
	assert(count);
	assert(comparev);
	assert(compare_count);
	assert(resultv);
	assert(result_count);
	assert((int)datatype >= 0 && (int)datatype < FI_DATATYPE_LAST);
	assert((int)op >= 0 && (int)op < FI_ATOMIC_OP_LAST);

	while (count && !iov[count-1].count)
		count--;

	while (compare_count && !comparev[compare_count-1].count)
		compare_count--;

	while (result_count && !resultv[result_count-1].count)
		result_count--;

	len = psmx2_ioc_size(iov, count, datatype);

	assert(psmx2_ioc_size(comparev, compare_count, datatype) >= len);
	assert(psmx2_ioc_size(resultv, result_count, datatype) >= len);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx2_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm2_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (psm2_epid == ep_priv->tx->psm2_epid) {
		if (count > 1) {
			buf = malloc(len);
			if (!buf)
				return -FI_ENOMEM;
			psmx2_ioc_read(iov, count, datatype, buf, len);
			desc0 = NULL;
		} else {
			buf = iov[0].addr;
			desc0 = desc ? desc[0] : NULL;
		}

		if (compare_count > 1) {
			compare = malloc(len);
			if (!compare) {
				if (count > 1)
					free(buf);
				return -FI_ENOMEM;
			}
			psmx2_ioc_read(comparev, compare_count, datatype, compare, len);
			compare_desc0 = NULL;
		} else {
			compare = comparev[0].addr;
			compare_desc0 = compare_desc ? compare_desc[0] : NULL;
		}

		if (result_count > 1) {
			result = malloc(len);
			if (!result) {
				if (compare_count > 1)
					free(compare);
				if (count > 1)
					free(buf);
				return -FI_ENOMEM;
			}
			result_desc0 = NULL;
		} else {
			result = resultv[0].addr;
			result_desc0 = result_desc ? result_desc[0] : NULL;
		}

		err = psmx2_atomic_self(PSMX2_AM_REQ_ATOMIC_COMPWRITE, ep_priv,
					buf, len / ofi_datatype_size(datatype), desc0,
					compare, compare_desc0, result, result_desc0,
					addr, key, datatype, op, context, flags);

		if (result_count > 1) {
			psmx2_ioc_write(resultv, result_count, datatype, result, len);
			free(result);
		}

		if (compare_count > 1)
			free(compare);

		if (count > 1)
			free(buf);

		return err;
	}

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	if (len * 2 > chunk_size)
		return -FI_EMSGSIZE;

	iov_size = result_count > 1 ? result_count * sizeof(struct fi_ioc) : 0;

	req = psmx2_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if ((flags & FI_INJECT) || count > 1 || compare_count > 1 ||
	    (uintptr_t)comparev[0].addr != (uintptr_t)iov[0].addr + len) {
		req->tmpbuf = malloc(iov_size + len + len);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}
		buf = (uint8_t *)req->tmpbuf + iov_size;
		psmx2_ioc_read(iov, count, datatype, buf, len);
		psmx2_ioc_read(comparev, compare_count, datatype, buf + len, len);
	} else {
		req->tmpbuf = malloc(iov_size);
		if (!req->tmpbuf) {
			psmx2_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}
		buf = iov[0].addr;
	}

	req->ioc = req->tmpbuf;
	if (iov_size) {
		memcpy(req->ioc, resultv, iov_size);
		req->atomic.iov_count = result_count;
		req->atomic.result = NULL;
	} else {
		req->atomic.buf = buf;
		req->atomic.result = resultv[0].addr;
	}

	req->no_event = (flags & PSMX2_NO_COMPLETION) ||
			(ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req->op = PSMX2_AM_REQ_ATOMIC_COMPWRITE;
	req->atomic.len = len;
	req->atomic.addr = addr;
	req->atomic.key = key;
	req->atomic.context = context;
	req->atomic.datatype = datatype;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_ATOMIC;

	args[0].u32w0 = PSMX2_AM_REQ_ATOMIC_COMPWRITE;
	args[0].u32w1 = len / ofi_datatype_size(datatype);
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	args[4].u32w0 = datatype;
	args[4].u32w1 = op;
	err = psm2_am_request_short(psm2_epaddr,
				    PSMX2_AM_ATOMIC_HANDLER, args, 5,
				    buf, len * 2, am_flags, NULL, NULL);
	if (err) {
		free(req->tmpbuf);
		psmx2_am_request_free(ep_priv->tx, req);
		return psmx2_errno(err);
	}

	psmx2_am_poll(ep_priv->tx);
	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_compwrite(struct fid_ep *ep,
				      const void *buf,
				      size_t count, void *desc,
				      const void *compare, void *compare_desc,
				      void *result, void *result_desc,
				      fi_addr_t dest_addr,
				      uint64_t addr, uint64_t key,
				      enum fi_datatype datatype,
				      enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	return psmx2_atomic_compwrite_generic(ep, buf, count, desc,
					      compare, compare_desc,
					      result, result_desc,
					      dest_addr, addr, key,
					      datatype, op, context, ep_priv->tx_flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_compwritemsg(struct fid_ep *ep,
					 const struct fi_msg_atomic *msg,
					 const struct fi_ioc *comparev,
					 void **compare_desc,
					 size_t compare_count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 uint64_t flags)
{
	assert(msg);
	assert(msg->msg_iov);
	assert(msg->iov_count);
	assert(msg->rma_iov);
	assert(msg->rma_iov_count == 1);
	assert(comparev);
	assert(compare_count);
	assert(resultv);
	assert(result_count);

	if (msg->iov_count > 1 || compare_count > 1 || result_count > 1)
		return psmx2_atomic_compwritev_generic(ep, msg->msg_iov, msg->desc,
						       msg->iov_count, comparev,
						       compare_desc, compare_count,
						       resultv, result_desc, result_count,
						       msg->addr, msg->rma_iov[0].addr,
						       msg->rma_iov[0].key, msg->datatype,
						       msg->op, msg->context, flags);

	return psmx2_atomic_compwrite_generic(ep, msg->msg_iov[0].addr,
					      msg->msg_iov[0].count,
					      msg->desc ? msg->desc[0] : NULL,
					      comparev[0].addr,
					      compare_desc ? compare_desc[0] : NULL,
					      resultv[0].addr,
					      result_desc ? result_desc[0] : NULL,
					      msg->addr, msg->rma_iov[0].addr,
					      msg->rma_iov[0].key, msg->datatype,
					      msg->op, msg->context, flags);
}

DIRECT_FN
STATIC ssize_t psmx2_atomic_compwritev(struct fid_ep *ep,
				       const struct fi_ioc *iov,
				       void **desc, size_t count,
				       const struct fi_ioc *comparev,
				       void **compare_desc,
				       size_t compare_count,
				       struct fi_ioc *resultv,
				       void **result_desc,
				       size_t result_count,
				       fi_addr_t dest_addr,
				       uint64_t addr, uint64_t key,
				       enum fi_datatype datatype,
				       enum fi_op op, void *context)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);

	assert(iov);
	assert(count);
	assert(comparev);
	assert(compare_count);
	assert(resultv);
	assert(result_count);

	if (count > 1 || compare_count > 1 || result_count > 1)
		return psmx2_atomic_compwritev_generic(ep, iov, desc, count,
						       comparev, compare_desc,
						       compare_count, resultv,
						       result_desc, result_count,
						       dest_addr, addr, key,
						       datatype, op, context,
						       ep_priv->tx_flags);

	return psmx2_atomic_compwrite_generic(ep, iov->addr, iov->count,
					      desc ? desc[0] : NULL,
					      comparev[0].addr,
					      compare_desc ? compare_desc[0] : NULL,
					      resultv[0].addr,
					      result_desc ? result_desc[0] : NULL,
					      dest_addr, addr, key, datatype, op,
					      context, ep_priv->tx_flags);
}

static int psmx2_atomic_writevalid_internal(size_t chunk_size,
					    enum fi_datatype datatype,
					    enum fi_op op, size_t *count)
{
	if (datatype >= FI_DATATYPE_LAST)
		return -FI_EOPNOTSUPP;

	switch (op) {
	case FI_MIN:
	case FI_MAX:
	case FI_SUM:
	case FI_PROD:
	case FI_LOR:
	case FI_LAND:
	case FI_BOR:
	case FI_BAND:
	case FI_LXOR:
	case FI_BXOR:
	case FI_ATOMIC_WRITE:
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	if (count)
		*count = chunk_size / ofi_datatype_size(datatype);

	return 0;
}

static int psmx2_atomic_readwritevalid_internal(size_t chunk_size,
						enum fi_datatype datatype,
						enum fi_op op, size_t *count)
{
	if (datatype >= FI_DATATYPE_LAST)
		return -FI_EOPNOTSUPP;

	switch (op) {
	case FI_MIN:
	case FI_MAX:
	case FI_SUM:
	case FI_PROD:
	case FI_LOR:
	case FI_LAND:
	case FI_BOR:
	case FI_BAND:
	case FI_LXOR:
	case FI_BXOR:
	case FI_ATOMIC_READ:
	case FI_ATOMIC_WRITE:
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	if (count)
		*count = chunk_size / ofi_datatype_size(datatype);

	return 0;
}

static int psmx2_atomic_compwritevalid_internal(size_t chunk_size,
						enum fi_datatype datatype,
						enum fi_op op, size_t *count)
{

	if (datatype >= FI_DATATYPE_LAST)
		return -FI_EOPNOTSUPP;

	switch (op) {
	case FI_CSWAP:
	case FI_CSWAP_NE:
		break;

	case FI_CSWAP_LE:
	case FI_CSWAP_LT:
	case FI_CSWAP_GE:
	case FI_CSWAP_GT:
		if (datatype == FI_FLOAT_COMPLEX ||
		    datatype == FI_DOUBLE_COMPLEX ||
		    datatype == FI_LONG_DOUBLE_COMPLEX)
			return -FI_EOPNOTSUPP;
		break;

	case FI_MSWAP:
		if (datatype == FI_FLOAT_COMPLEX ||
		    datatype == FI_DOUBLE_COMPLEX ||
		    datatype == FI_LONG_DOUBLE_COMPLEX ||
		    datatype == FI_FLOAT ||
		    datatype == FI_DOUBLE ||
		    datatype == FI_LONG_DOUBLE)
			return -FI_EOPNOTSUPP;
		break;

	default:
		return -FI_EOPNOTSUPP;
	}

	if (count)
		*count = chunk_size / (2 * ofi_datatype_size(datatype));

	return 0;
}

DIRECT_FN
STATIC int psmx2_atomic_writevalid(struct fid_ep *ep,
				   enum fi_datatype datatype,
				   enum fi_op op, size_t *count)
{
	struct psmx2_fid_ep *ep_priv;
	size_t chunk_size;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	return psmx2_atomic_writevalid_internal(chunk_size, datatype, op, count);
}

DIRECT_FN
STATIC int psmx2_atomic_readwritevalid(struct fid_ep *ep,
				       enum fi_datatype datatype,
				       enum fi_op op, size_t *count)
{
	struct psmx2_fid_ep *ep_priv;
	size_t chunk_size;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	return psmx2_atomic_readwritevalid_internal(chunk_size, datatype, op, count);
}

DIRECT_FN
STATIC int psmx2_atomic_compwritevalid(struct fid_ep *ep,
				       enum fi_datatype datatype,
				       enum fi_op op, size_t *count)
{
	struct psmx2_fid_ep *ep_priv;
	size_t chunk_size;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;
	return psmx2_atomic_compwritevalid_internal(chunk_size, datatype, op, count);
}

DIRECT_FN
int psmx2_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		       enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	struct psmx2_fid_domain *domain_priv;
	size_t chunk_size;
	size_t count;
	int ret;

	domain_priv = container_of(domain, struct psmx2_fid_domain, util_domain.domain_fid);
	chunk_size = domain_priv->max_atomic_size;

	if (flags & FI_TAGGED)
		return -FI_EOPNOTSUPP;

	if (flags & FI_COMPARE_ATOMIC) {
		if (flags & FI_FETCH_ATOMIC)
			return -FI_EINVAL;
		ret = psmx2_atomic_compwritevalid_internal(chunk_size, datatype,
							   op, &count);
	} else if (flags & FI_FETCH_ATOMIC) {
		ret = psmx2_atomic_readwritevalid_internal(chunk_size, datatype,
							   op, &count);
	} else {
		ret = psmx2_atomic_writevalid_internal(chunk_size, datatype,
						       op, &count);
	}

	if (attr && !ret) {
		attr->size = ofi_datatype_size(datatype);
		attr->count = count;
	}

	return ret;
}

struct fi_ops_atomic psmx2_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = psmx2_atomic_write,
	.writev = psmx2_atomic_writev,
	.writemsg = psmx2_atomic_writemsg,
	.inject = psmx2_atomic_inject,
	.readwrite = psmx2_atomic_readwrite,
	.readwritev = psmx2_atomic_readwritev,
	.readwritemsg = psmx2_atomic_readwritemsg,
	.compwrite = psmx2_atomic_compwrite,
	.compwritev = psmx2_atomic_compwritev,
	.compwritemsg = psmx2_atomic_compwritemsg,
	.writevalid = psmx2_atomic_writevalid,
	.readwritevalid = psmx2_atomic_readwritevalid,
	.compwritevalid = psmx2_atomic_compwritevalid,
};

