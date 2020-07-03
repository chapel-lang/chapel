/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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

#include "ofi_perf.h"
#include "ofi_prov.h"
#include "hook_prov.h"


const char *perf_counters_str[] = {
	HOOK_FOREACH(OFI_STR)
};


static inline struct ofi_perfset *perf_set(struct hook_ep *ep)
{
	return &container_of(ep->domain->fabric, struct perf_fabric,
			     fabric_hook)->perf_set;
}

static inline struct ofi_perfset *perf_set_cq(struct hook_cq *cq)
{
	return &container_of(cq->domain->fabric, struct perf_fabric,
			     fabric_hook)->perf_set;
}

static inline struct ofi_perfset *perf_set_cntr(struct hook_cntr *cntr)
{
	return &container_of(cntr->domain->fabric, struct perf_fabric,
			     fabric_hook)->perf_set;
}

/*
static ssize_t
perf_atomic_write(struct fid_ep *ep,
		  const void *buf, size_t count, void *desc,
		  fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		  enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomic(myep->hep, buf, count, desc, dest_addr,
			addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_writev(struct fid_ep *ep,
		   const struct fi_ioc *iov, void **desc, size_t count,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicv(myep->hep, iov, desc, count, dest_addr,
			 addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_writemsg(struct fid_ep *ep,
		     const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicmsg(myep->hep, msg, flags);
	return ret;
}

static ssize_t
perf_atomic_inject(struct fid_ep *ep, const void *buf, size_t count,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_atomic(myep->hep, buf, count, dest_addr,
			       addr, key, datatype, op);
	return ret;
}

static ssize_t
perf_atomic_readwrite(struct fid_ep *ep,
		      const void *buf, size_t count, void *desc,
		      void *result, void *result_desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomic(myep->hep, buf, count, desc,
			      result, result_desc, dest_addr,
			      addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_readwritev(struct fid_ep *ep,
		       const struct fi_ioc *iov, void **desc, size_t count,
		       struct fi_ioc *resultv, void **result_desc,
		       size_t result_count, fi_addr_t dest_addr,
		       uint64_t addr, uint64_t key, enum fi_datatype datatype,
		       enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicv(myep->hep, iov, desc, count,
			       resultv, result_desc, result_count,
			       dest_addr, addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_readwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			 struct fi_ioc *resultv, void **result_desc,
			 size_t result_count, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicmsg(myep->hep, msg, resultv, result_desc,
				 result_count, flags);
	return ret;
}

static ssize_t
perf_atomic_compwrite(struct fid_ep *ep,
		      const void *buf, size_t count, void *desc,
		      const void *compare, void *compare_desc,
		      void *result, void *result_desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomic(myep->hep, buf, count, desc,
				compare, compare_desc, result, result_desc,
				dest_addr, addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_compwritev(struct fid_ep *ep,
		       const struct fi_ioc *iov, void **desc, size_t count,
		       const struct fi_ioc *comparev, void **compare_desc,
		       size_t compare_count, struct fi_ioc *resultv,
		       void **result_desc, size_t result_count,
		       fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		       enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicv(myep->hep, iov, desc, count,
				 comparev, compare_desc, compare_count,
				 resultv, result_desc, result_count, dest_addr,
				 addr, key, datatype, op, context);
	return ret;
}

static ssize_t
perf_atomic_compwritemsg(struct fid_ep *ep,
			 const struct fi_msg_atomic *msg,
			 const struct fi_ioc *comparev, void **compare_desc,
			 size_t compare_count, struct fi_ioc *resultv,
			 void **result_desc, size_t result_count,
			 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicmsg(myep->hep, msg,
				   comparev, compare_desc, compare_count,
				   resultv, result_desc, result_count, flags);
	return ret;
}

static int
perf_atomic_writevalid(struct fid_ep *ep, enum fi_datatype datatype,
		       enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

static int
perf_atomic_readwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			   enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_fetch_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

static int
perf_atomic_compwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			   enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_compare_atomicvalid(myep->hep, datatype, op, count);
	return ret;
}

struct fi_ops_atomic perf_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = perf_atomic_write,
	.writev = perf_atomic_writev,
	.writemsg = perf_atomic_writemsg,
	.inject = perf_atomic_inject,
	.readwrite = perf_atomic_readwrite,
	.readwritev = perf_atomic_readwritev,
	.readwritemsg = perf_atomic_readwritemsg,
	.compwrite = perf_atomic_compwrite,
	.compwritev = perf_atomic_compwritev,
	.compwritemsg = perf_atomic_compwritemsg,
	.writevalid = perf_atomic_writevalid,
	.readwritevalid = perf_atomic_readwritevalid,
	.compwritevalid = perf_atomic_compwritevalid,
};
*/


static ssize_t
perf_msg_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
	      fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_recv);
	ret = fi_recv(myep->hep, buf, len, desc, src_addr, context);
	ofi_perfset_end(perf_set(myep), perf_recv);
	return ret;
}

static ssize_t
perf_msg_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_recvv);
	ret = fi_recvv(myep->hep, iov, desc, count, src_addr, context);
	ofi_perfset_end(perf_set(myep), perf_recvv);
	return ret;
}

static ssize_t
perf_msg_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_recvmsg);
	ret = fi_recvmsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_recvmsg);
	return ret;
}

static ssize_t
perf_msg_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	      fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_send);
	ret = fi_send(myep->hep, buf, len, desc, dest_addr, context);
	ofi_perfset_end(perf_set(myep), perf_send);
	return ret;
}

static ssize_t
perf_msg_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_sendv);
	ret = fi_sendv(myep->hep, iov, desc, count, dest_addr, context);
	ofi_perfset_end(perf_set(myep), perf_sendv);
	return ret;
}

static ssize_t
perf_msg_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
		 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_sendmsg);
	ret = fi_sendmsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_sendmsg);
	return ret;
}

static ssize_t
perf_msg_inject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_inject);
	ret = fi_inject(myep->hep, buf, len, dest_addr);
	ofi_perfset_end(perf_set(myep), perf_inject);
	return ret;
}

static ssize_t
perf_msg_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		  uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_senddata);
	ret = fi_senddata(myep->hep, buf, len, desc, data, dest_addr, context);
	ofi_perfset_end(perf_set(myep), perf_senddata);
	return ret;
}

static ssize_t
perf_msg_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_injectdata);
	ret = fi_injectdata(myep->hep, buf, len, data, dest_addr);
	ofi_perfset_end(perf_set(myep), perf_injectdata);
	return ret;
}

static struct fi_ops_msg perf_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = perf_msg_recv,
	.recvv = perf_msg_recvv,
	.recvmsg = perf_msg_recvmsg,
	.send = perf_msg_send,
	.sendv = perf_msg_sendv,
	.sendmsg = perf_msg_sendmsg,
	.inject = perf_msg_inject,
	.senddata = perf_msg_senddata,
	.injectdata = perf_msg_injectdata,
};


static ssize_t
perf_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	      fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_read);
	ret = fi_read(myep->hep, buf, len, desc, src_addr, addr, key, context);
	ofi_perfset_end(perf_set(myep), perf_read);
	return ret;
}

static ssize_t
perf_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	       void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_readv);
	ret = fi_readv(myep->hep, iov, desc, count, src_addr,
		       addr, key, context);
	ofi_perfset_end(perf_set(myep), perf_readv);
	return ret;
}

static ssize_t
perf_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_readmsg);
	ret = fi_readmsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_readmsg);
	return ret;
}

static ssize_t
perf_rma_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	       fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_write);
	ret = fi_write(myep->hep, buf, len, desc, dest_addr,
		       addr, key, context);
	ofi_perfset_end(perf_set(myep), perf_write);
	return ret;
}

static ssize_t
perf_rma_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_writev);
	ret = fi_writev(myep->hep, iov, desc, count, dest_addr,
			addr, key, context);
	ofi_perfset_end(perf_set(myep), perf_writev);
	return ret;
}

static ssize_t
perf_rma_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_writemsg);
	ret = fi_writemsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_writemsg);
	return ret;
}

static ssize_t
perf_rma_inject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_inject_write);
	ret = fi_inject_write(myep->hep, buf, len, dest_addr, addr, key);
	ofi_perfset_end(perf_set(myep), perf_inject_write);
	return ret;
}

static ssize_t
perf_rma_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		   uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_writedata);
	ret = fi_writedata(myep->hep, buf, len, desc, data,
			   dest_addr, addr, key, context);
	ofi_perfset_end(perf_set(myep), perf_writedata);
	return ret;
}

static ssize_t
perf_rma_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		    uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_inject_writedata);
	ret = fi_inject_writedata(myep->hep, buf, len, data, dest_addr,
				  addr, key);
	ofi_perfset_end(perf_set(myep), perf_inject_writedata);
	return ret;
}

static struct fi_ops_rma perf_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = perf_rma_read,
	.readv = perf_rma_readv,
	.readmsg = perf_rma_readmsg,
	.write = perf_rma_write,
	.writev = perf_rma_writev,
	.writemsg = perf_rma_writemsg,
	.inject = perf_rma_inject,
	.writedata = perf_rma_writedata,
	.injectdata = perf_rma_injectdata,
};


static ssize_t
perf_tagged_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		 fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		 void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_trecv);
	ret = fi_trecv(myep->hep, buf, len, desc, src_addr,
		       tag, ignore, context);
	ofi_perfset_end(perf_set(myep), perf_trecv);
	return ret;
}

static ssize_t
perf_tagged_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		  size_t count, fi_addr_t src_addr, uint64_t tag,
		  uint64_t ignore, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_trecvv);
	ret = fi_trecvv(myep->hep, iov, desc, count, src_addr,
			tag, ignore, context);
	ofi_perfset_end(perf_set(myep), perf_trecvv);
	return ret;
}

static ssize_t
perf_tagged_recvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		    uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_trecvmsg);
	ret = fi_trecvmsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_trecvmsg);
	return ret;
}

static ssize_t
perf_tagged_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		 fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tsend);
	ret = fi_tsend(myep->hep, buf, len, desc, dest_addr, tag, context);
	ofi_perfset_end(perf_set(myep), perf_tsend);
	return ret;
}

static ssize_t
perf_tagged_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		  size_t count, fi_addr_t dest_addr, uint64_t tag,
		  void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tsendv);
	ret = fi_tsendv(myep->hep, iov, desc, count, dest_addr, tag, context);
	ofi_perfset_end(perf_set(myep), perf_tsendv);
	return ret;
}

static ssize_t
perf_tagged_sendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		    uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tsendmsg);
	ret = fi_tsendmsg(myep->hep, msg, flags);
	ofi_perfset_end(perf_set(myep), perf_tsendmsg);
	return ret;
}

static ssize_t
perf_tagged_inject(struct fid_ep *ep, const void *buf, size_t len,
		   fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tinject);
	ret = fi_tinject(myep->hep, buf, len, dest_addr, tag);
	ofi_perfset_end(perf_set(myep), perf_tinject);
	return ret;
}

static ssize_t
perf_tagged_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		     uint64_t data, fi_addr_t dest_addr, uint64_t tag,
		     void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tsenddata);
	ret = fi_tsenddata(myep->hep, buf, len, desc, data,
			   dest_addr, tag, context);
	ofi_perfset_end(perf_set(myep), perf_tsenddata);
	return ret;
}

static ssize_t
perf_tagged_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ofi_perfset_start(perf_set(myep), perf_tinjectdata);
	ret = fi_tinjectdata(myep->hep, buf, len, data, dest_addr, tag);
	ofi_perfset_end(perf_set(myep), perf_tinjectdata);
	return ret;
}

static struct fi_ops_tagged perf_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = perf_tagged_recv,
	.recvv = perf_tagged_recvv,
	.recvmsg = perf_tagged_recvmsg,
	.send = perf_tagged_send,
	.sendv = perf_tagged_sendv,
	.sendmsg = perf_tagged_sendmsg,
	.inject = perf_tagged_inject,
	.senddata = perf_tagged_senddata,
	.injectdata = perf_tagged_injectdata,
};


static ssize_t perf_cq_read_op(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_read);
	ret = fi_cq_read(mycq->hcq, buf, count);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_read);
	return ret;
}

static ssize_t
perf_cq_readerr_op(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_readerr);
	ret = fi_cq_readerr(mycq->hcq, buf, flags);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_readerr);
	return ret;
}

static ssize_t
perf_cq_readfrom_op(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_readfrom);
	ret = fi_cq_readfrom(mycq->hcq, buf, count, src_addr);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_readfrom);
	return ret;
}

static ssize_t
perf_cq_sread_op(struct fid_cq *cq, void *buf, size_t count,
	      const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_sread);
	ret = fi_cq_sread(mycq->hcq, buf, count, cond, timeout);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_sread);
	return ret;
}

static ssize_t
perf_cq_sreadfrom_op(struct fid_cq *cq, void *buf, size_t count,
		  fi_addr_t *src_addr, const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_sreadfrom);
	ret = fi_cq_sreadfrom(mycq->hcq, buf, count, src_addr, cond, timeout);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_sreadfrom);
	return ret;
}

static int perf_cq_signal_op(struct fid_cq *cq)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	int ret;

	ofi_perfset_start(perf_set_cq(mycq), perf_cq_signal);
	ret = fi_cq_signal(mycq->hcq);
	ofi_perfset_end(perf_set_cq(mycq), perf_cq_signal);
	return ret;
}

struct fi_ops_cq perf_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = perf_cq_read_op,
	.readfrom = perf_cq_readfrom_op,
	.readerr = perf_cq_readerr_op,
	.sread = perf_cq_sread_op,
	.sreadfrom = perf_cq_sreadfrom_op,
	.signal = perf_cq_signal_op,
	.strerror = hook_cq_strerror,
};


static uint64_t perf_cntr_read_op(struct fid_cntr *cntr)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	uint64_t ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_read);
	ret = fi_cntr_read(mycntr->hcntr);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_read);
	return ret;
}

static uint64_t perf_cntr_readerr_op(struct fid_cntr *cntr)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	uint64_t ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_readerr);
	ret = fi_cntr_readerr(mycntr->hcntr);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_readerr);
	return ret;
}

static int perf_cntr_add_op(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_add);
	ret = fi_cntr_add(mycntr->hcntr, value);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_add);
	return ret;
}

static int perf_cntr_set_op(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_set);
	ret = fi_cntr_set(mycntr->hcntr, value);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_set);
	return ret;
}

static int perf_cntr_wait_op(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_wait);
	ret = fi_cntr_wait(mycntr->hcntr, threshold, timeout);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_wait);
	return ret;
}

static int perf_cntr_adderr_op(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_adderr);
	ret = fi_cntr_adderr(mycntr->hcntr, value);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_adderr);
	return ret;
}

static int perf_cntr_seterr_op(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);
	int ret;

	ofi_perfset_start(perf_set_cntr(mycntr), perf_cntr_seterr);
	ret = fi_cntr_seterr(mycntr->hcntr, value);
	ofi_perfset_end(perf_set_cntr(mycntr), perf_cntr_seterr);
	return ret;
}

struct fi_ops_cntr perf_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = perf_cntr_read_op,
	.readerr = perf_cntr_readerr_op,
	.add = perf_cntr_add_op,
	.set = perf_cntr_set_op,
	.wait = perf_cntr_wait_op,
	.adderr = perf_cntr_adderr_op,
	.seterr = perf_cntr_seterr_op,
};


static struct fi_ops perf_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_perf_destroy,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

int hook_perf_destroy(struct fid *fid)
{
	struct perf_fabric *fab;

	fab = container_of(fid, struct perf_fabric, fabric_hook);
	ofi_perfset_log(&fab->perf_set, perf_counters_str);
	ofi_perfset_close(&fab->perf_set);
	hook_close(fid);

	return FI_SUCCESS;
}

struct hook_prov_ctx hook_perf_ctx;

static int hook_perf_fabric(struct fi_fabric_attr *attr,
			    struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct perf_fabric *fab;
	int ret;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing perf hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	ret = ofi_perfset_create(hprov, &fab->perf_set, perf_size,
				 perf_domain, perf_cntr, perf_flags);
	if (ret) {
		free(fab);
		return ret;
	}

	/*
	 * TODO
	 * comment from GitHub PR #5052:
	 * "I think we want to try replacing HOOK_PERF with a
	 * struct hook_provider * (now called struct hook_prov_ctx)."
	 */
	hook_fabric_init(&fab->fabric_hook, HOOK_PERF, attr->fabric, hprov,
			 &perf_fabric_fid_ops, &hook_perf_ctx);
	*fabric = &fab->fabric_hook.fabric;
	return 0;
}

struct hook_prov_ctx hook_perf_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_perf",
		.getinfo = NULL,
		.fabric = hook_perf_fabric,
		.cleanup = NULL,
	},
};

static int perf_cq_init(struct fid *fid)
{
	struct fid_cq *cq = container_of(fid, struct fid_cq, fid);
	cq->ops = &perf_cq_ops;
	return 0;
}

static int perf_cntr_init(struct fid *fid)
{
	struct fid_cntr *cntr = container_of(fid, struct fid_cntr, fid);
	cntr->ops = &perf_cntr_ops;
	return 0;
}

static int perf_endpoint_init(struct fid *fid)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	ep->msg = &perf_msg_ops;
	ep->rma = &perf_rma_ops;
	ep->tagged = &perf_tagged_ops;
	return 0;
}


HOOK_PERF_INI
{
	hook_perf_ctx.ini_fid[FI_CLASS_CQ] = perf_cq_init;
	hook_perf_ctx.ini_fid[FI_CLASS_CNTR] = perf_cntr_init;
	hook_perf_ctx.ini_fid[FI_CLASS_EP] = perf_endpoint_init;
	return &hook_perf_ctx.prov;
}
