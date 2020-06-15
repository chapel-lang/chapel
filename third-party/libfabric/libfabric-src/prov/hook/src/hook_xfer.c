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

#include "ofi_hook.h"


static ssize_t
hook_atomic_write(struct fid_ep *ep,
		  const void *buf, size_t count, void *desc,
		  fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		  enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_atomic(myep->hep, buf, count, desc, dest_addr,
			 addr, key, datatype, op, context);
}

static ssize_t
hook_atomic_writev(struct fid_ep *ep,
		   const struct fi_ioc *iov, void **desc, size_t count,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_atomicv(myep->hep, iov, desc, count, dest_addr,
			  addr, key, datatype, op, context);
}

static ssize_t
hook_atomic_writemsg(struct fid_ep *ep,
		     const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_atomicmsg(myep->hep, msg, flags);
}

static ssize_t
hook_atomic_inject(struct fid_ep *ep, const void *buf, size_t count,
		   fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_inject_atomic(myep->hep, buf, count, dest_addr,
				addr, key, datatype, op);
}

static ssize_t
hook_atomic_readwrite(struct fid_ep *ep,
		      const void *buf, size_t count, void *desc,
		      void *result, void *result_desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_fetch_atomic(myep->hep, buf, count, desc,
			       result, result_desc, dest_addr,
			       addr, key, datatype, op, context);
}

static ssize_t
hook_atomic_readwritev(struct fid_ep *ep,
		       const struct fi_ioc *iov, void **desc, size_t count,
		       struct fi_ioc *resultv, void **result_desc,
		       size_t result_count, fi_addr_t dest_addr,
		       uint64_t addr, uint64_t key, enum fi_datatype datatype,
		       enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_fetch_atomicv(myep->hep, iov, desc, count,
				resultv, result_desc, result_count,
				dest_addr, addr, key, datatype, op, context);
}

static ssize_t
hook_atomic_readwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			 struct fi_ioc *resultv, void **result_desc,
			 size_t result_count, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_fetch_atomicmsg(myep->hep, msg, resultv, result_desc,
				  result_count, flags);
}

static ssize_t
hook_atomic_compwrite(struct fid_ep *ep,
		      const void *buf, size_t count, void *desc,
		      const void *compare, void *compare_desc,
		      void *result, void *result_desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_compare_atomic(myep->hep, buf, count, desc,
				 compare, compare_desc, result, result_desc,
				 dest_addr, addr, key, datatype, op, context);
}

static ssize_t
hook_atomic_compwritev(struct fid_ep *ep,
		       const struct fi_ioc *iov, void **desc, size_t count,
		       const struct fi_ioc *comparev, void **compare_desc,
		       size_t compare_count, struct fi_ioc *resultv,
		       void **result_desc, size_t result_count,
		       fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		       enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_compare_atomicv(myep->hep, iov, desc, count,
				  comparev, compare_desc, compare_count,
				  resultv, result_desc, result_count, dest_addr,
				  addr, key, datatype, op, context);
}
static ssize_t
hook_atomic_compwritemsg(struct fid_ep *ep,
			 const struct fi_msg_atomic *msg,
			 const struct fi_ioc *comparev, void **compare_desc,
			 size_t compare_count, struct fi_ioc *resultv,
			 void **result_desc, size_t result_count,
			 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_compare_atomicmsg(myep->hep, msg,
				    comparev, compare_desc, compare_count,
				    resultv, result_desc, result_count, flags);
}

static int
hook_atomic_writevalid(struct fid_ep *ep, enum fi_datatype datatype,
		       enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_atomicvalid(myep->hep, datatype, op, count);
}

static int
hook_atomic_readwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			   enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_fetch_atomicvalid(myep->hep, datatype, op, count);
}

static int
hook_atomic_compwritevalid(struct fid_ep *ep, enum fi_datatype datatype,
			   enum fi_op op, size_t *count)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_compare_atomicvalid(myep->hep, datatype, op, count);
}

struct fi_ops_atomic hook_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = hook_atomic_write,
	.writev = hook_atomic_writev,
	.writemsg = hook_atomic_writemsg,
	.inject = hook_atomic_inject,
	.readwrite = hook_atomic_readwrite,
	.readwritev = hook_atomic_readwritev,
	.readwritemsg = hook_atomic_readwritemsg,
	.compwrite = hook_atomic_compwrite,
	.compwritev = hook_atomic_compwritev,
	.compwritemsg = hook_atomic_compwritemsg,
	.writevalid = hook_atomic_writevalid,
	.readwritevalid = hook_atomic_readwritevalid,
	.compwritevalid = hook_atomic_compwritevalid,
};


static ssize_t
hook_msg_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
	      fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_recv(myep->hep, buf, len, desc, src_addr, context);
}

static ssize_t
hook_msg_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_recvv(myep->hep, iov, desc, count, src_addr, context);
}

static ssize_t
hook_msg_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_recvmsg(myep->hep, msg, flags);
}

static ssize_t
hook_msg_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	      fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_send(myep->hep, buf, len, desc, dest_addr, context);
}

static ssize_t
hook_msg_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_sendv(myep->hep, iov, desc, count, dest_addr, context);
}

static ssize_t
hook_msg_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
		 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_sendmsg(myep->hep, msg, flags);
}

static ssize_t
hook_msg_inject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_inject(myep->hep, buf, len, dest_addr);
}

static ssize_t
hook_msg_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		  uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_senddata(myep->hep, buf, len, desc, data, dest_addr, context);
}

static ssize_t
hook_msg_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_injectdata(myep->hep, buf, len, data, dest_addr);
}

struct fi_ops_msg hook_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = hook_msg_recv,
	.recvv = hook_msg_recvv,
	.recvmsg = hook_msg_recvmsg,
	.send = hook_msg_send,
	.sendv = hook_msg_sendv,
	.sendmsg = hook_msg_sendmsg,
	.inject = hook_msg_inject,
	.senddata = hook_msg_senddata,
	.injectdata = hook_msg_injectdata,
};


static ssize_t
hook_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	      fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_read(myep->hep, buf, len, desc, src_addr, addr, key, context);
}

static ssize_t
hook_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	       void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_readv(myep->hep, iov, desc, count, src_addr,
			addr, key, context);
}

static ssize_t
hook_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_readmsg(myep->hep, msg, flags);
}

static ssize_t
hook_rma_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
	       fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_write(myep->hep, buf, len, desc, dest_addr,
			addr, key, context);
}

static ssize_t
hook_rma_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_writev(myep->hep, iov, desc, count, dest_addr,
			 addr, key, context);
}

static ssize_t
hook_rma_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_writemsg(myep->hep, msg, flags);
}

static ssize_t
hook_rma_inject(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_inject_write(myep->hep, buf, len, dest_addr, addr, key);
}

static ssize_t
hook_rma_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		   uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_writedata(myep->hep, buf, len, desc, data,
			    dest_addr, addr, key, context);
}

static ssize_t
hook_rma_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		    uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_inject_writedata(myep->hep, buf, len, data, dest_addr,
				   addr, key);
}

struct fi_ops_rma hook_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = hook_rma_read,
	.readv = hook_rma_readv,
	.readmsg = hook_rma_readmsg,
	.write = hook_rma_write,
	.writev = hook_rma_writev,
	.writemsg = hook_rma_writemsg,
	.inject = hook_rma_inject,
	.writedata = hook_rma_writedata,
	.injectdata = hook_rma_injectdata,
};


static ssize_t
hook_tagged_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		 fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		 void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_trecv(myep->hep, buf, len, desc, src_addr,
			tag, ignore, context);
}

static ssize_t
hook_tagged_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		  size_t count, fi_addr_t src_addr, uint64_t tag,
		  uint64_t ignore, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_trecvv(myep->hep, iov, desc, count, src_addr,
			 tag, ignore, context);
}

static ssize_t
hook_tagged_recvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		    uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_trecvmsg(myep->hep, msg, flags);
}

static ssize_t
hook_tagged_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		 fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tsend(myep->hep, buf, len, desc, dest_addr, tag, context);
}

static ssize_t
hook_tagged_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		  size_t count, fi_addr_t dest_addr, uint64_t tag,
		  void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tsendv(myep->hep, iov, desc, count, dest_addr, tag, context);
}

static ssize_t
hook_tagged_sendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		    uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tsendmsg(myep->hep, msg, flags);
}

static ssize_t
hook_tagged_inject(struct fid_ep *ep, const void *buf, size_t len,
		   fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tinject(myep->hep, buf, len, dest_addr, tag);
}

static ssize_t
hook_tagged_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		     uint64_t data, fi_addr_t dest_addr, uint64_t tag,
		     void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tsenddata(myep->hep, buf, len, desc, data,
			    dest_addr, tag, context);
}

static ssize_t
hook_tagged_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);

	return fi_tinjectdata(myep->hep, buf, len, data, dest_addr, tag);
}

struct fi_ops_tagged hook_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = hook_tagged_recv,
	.recvv = hook_tagged_recvv,
	.recvmsg = hook_tagged_recvmsg,
	.send = hook_tagged_send,
	.sendv = hook_tagged_sendv,
	.sendmsg = hook_tagged_sendmsg,
	.inject = hook_tagged_inject,
	.senddata = hook_tagged_senddata,
	.injectdata = hook_tagged_injectdata,
};
