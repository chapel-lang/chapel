/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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

#include <sys/types.h>
#include <errno.h>

#include <ofi_prov.h>
#include "xnet.h"


static ssize_t
xnet_rdm_recv(struct fid_ep *ep_fid, void *buf, size_t len,
	      void *desc, fi_addr_t src_addr, void *context)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_recv(&rdm->srx->rx_fid, buf, len, desc, src_addr, context);
}

static ssize_t
xnet_rdm_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
	  void **desc, size_t count, fi_addr_t src_addr, void *context)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_recvv(&rdm->srx->rx_fid, iov, desc, count, src_addr, context);
}

static ssize_t
xnet_rdm_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
		 uint64_t flags)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_recvmsg(&rdm->srx->rx_fid, msg, flags);
}

static ssize_t
xnet_rdm_send(struct fid_ep *ep_fid, const void *buf, size_t len,
	      void *desc, fi_addr_t dest_addr, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_send(&conn->ep->util_ep.ep_fid, buf, len, desc, 0, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
	       void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_sendv(&conn->ep->util_ep.ep_fid, iov, desc, count, 0, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
		 uint64_t flags)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_sendmsg(&conn->ep->util_ep.ep_fid, msg, flags);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_inject(struct fid_ep *ep_fid, const void *buf,
		size_t len, fi_addr_t dest_addr)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_inject(&conn->ep->util_ep.ep_fid, buf, len, 0);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
		  void *desc, uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_senddata(&conn->ep->util_ep.ep_fid, buf, len, desc, data, 0,
			  context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_injectdata(&conn->ep->util_ep.ep_fid, buf, len, data, 0);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static struct fi_ops_msg xnet_rdm_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = xnet_rdm_recv,
	.recvv = xnet_rdm_recvv,
	.recvmsg = xnet_rdm_recvmsg,
	.send = xnet_rdm_send,
	.sendv = xnet_rdm_sendv,
	.sendmsg = xnet_rdm_sendmsg,
	.inject = xnet_rdm_inject,
	.senddata = xnet_rdm_senddata,
	.injectdata = xnet_rdm_injectdata,
};

static ssize_t
xnet_rdm_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
	       void *desc, fi_addr_t src_addr,
	       uint64_t tag, uint64_t ignore, void *context)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_trecv(&rdm->srx->rx_fid, buf, len, desc, src_addr,
			tag, ignore, context);
}

static ssize_t
xnet_rdm_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t src_addr,
		uint64_t tag, uint64_t ignore, void *context)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_trecvv(&rdm->srx->rx_fid, iov, desc, count, src_addr,
			 tag, ignore, context);
}

static ssize_t
xnet_rdm_trecvmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		  uint64_t flags)
{
	struct xnet_rdm *rdm;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	return fi_trecvmsg(&rdm->srx->rx_fid, msg, flags);
}

static ssize_t
xnet_rdm_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
	       void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsend(&conn->ep->util_ep.ep_fid, buf, len, desc, 0, tag,
		       context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr,
		uint64_t tag, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsendv(&conn->ep->util_ep.ep_fid, iov, desc, count, 0, tag,
			context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_tsendmsg(struct fid_ep *ep_fid, const struct fi_msg_tagged *msg,
		  uint64_t flags)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsendmsg(&conn->ep->util_ep.ep_fid, msg, flags);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_tinject(struct fid_ep *ep_fid, const void *buf,
		 size_t len, fi_addr_t dest_addr, uint64_t tag)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tinject(&conn->ep->util_ep.ep_fid, buf, len, 0, tag);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
		   void *desc, uint64_t data, fi_addr_t dest_addr,
		   uint64_t tag, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tsenddata(&conn->ep->util_ep.ep_fid, buf, len, desc, data, 0,
			   tag, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_tinjectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_tinjectdata(&conn->ep->util_ep.ep_fid, buf, len, data, 0, tag);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static struct fi_ops_tagged xnet_rdm_tagged_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = xnet_rdm_trecv,
	.recvv = xnet_rdm_trecvv,
	.recvmsg = xnet_rdm_trecvmsg,
	.send = xnet_rdm_tsend,
	.sendv = xnet_rdm_tsendv,
	.sendmsg = xnet_rdm_tsendmsg,
	.inject = xnet_rdm_tinject,
	.senddata = xnet_rdm_tsenddata,
	.injectdata = xnet_rdm_tinjectdata,
};

static ssize_t
xnet_rdm_read(struct fid_ep *ep_fid, void *buf, size_t len,
	      void *desc, fi_addr_t src_addr, uint64_t addr,
	      uint64_t key, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, src_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_read(&conn->ep->util_ep.ep_fid, buf, len, desc, src_addr, addr,
		      key, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_readv(struct fid_ep *ep_fid, const struct iovec *iov,
	       void **desc, size_t count, fi_addr_t src_addr,
	       uint64_t addr, uint64_t key, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, src_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_readv(&conn->ep->util_ep.ep_fid, iov, desc, count, src_addr, addr,
		       key, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_readmsg(&conn->ep->util_ep.ep_fid, msg, flags);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_write(struct fid_ep *ep_fid, const void *buf,
	       size_t len, void *desc, fi_addr_t dest_addr,
	       uint64_t addr, uint64_t key, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_write(&conn->ep->util_ep.ep_fid, buf, len, desc, dest_addr,
		       addr, key, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_writev(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writev(&conn->ep->util_ep.ep_fid, iov, desc, count, dest_addr,
			addr, key, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		  uint64_t flags)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writemsg(&conn->ep->util_ep.ep_fid, msg, flags);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_inject_write(struct fid_ep *ep_fid, const void *buf,
		      size_t len, fi_addr_t dest_addr,
		      uint64_t addr, uint64_t key)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_inject_write(&conn->ep->util_ep.ep_fid, buf, len, dest_addr,
			      addr, key);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_writedata(struct fid_ep *ep_fid, const void *buf,
		   size_t len, void *desc, uint64_t data,
		   fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writedata(&conn->ep->util_ep.ep_fid, buf, len, desc, data,
			   dest_addr, addr, key, context);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static ssize_t
xnet_rdm_inject_writedata(struct fid_ep *ep_fid, const void *buf,
			  size_t len, uint64_t data, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key)
{
	struct xnet_rdm *rdm;
	struct xnet_conn *conn;
	ssize_t ret;

	rdm = container_of(ep_fid, struct xnet_rdm, util_ep.ep_fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = xnet_get_conn(rdm, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_inject_writedata(&conn->ep->util_ep.ep_fid, buf, len, data,
				  dest_addr, addr, key);
unlock:
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
	return ret;
}

static struct fi_ops_rma xnet_rdm_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = xnet_rdm_read,
	.readv = xnet_rdm_readv,
	.readmsg = xnet_rdm_readmsg,
	.write = xnet_rdm_write,
	.writev = xnet_rdm_writev,
	.writemsg = xnet_rdm_writemsg,
	.inject = xnet_rdm_inject_write,
	.writedata = xnet_rdm_writedata,
	.injectdata = xnet_rdm_inject_writedata,
};

static struct fi_ops_atomic xnet_rdm_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = fi_no_atomic_write,
	.writev = fi_no_atomic_writev,
	.writemsg = fi_no_atomic_writemsg,
	.inject = fi_no_atomic_inject,
	.readwrite = fi_no_atomic_readwrite,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = fi_no_atomic_readwritemsg,
	.compwrite = fi_no_atomic_compwrite,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = fi_no_atomic_compwritemsg,
	.writevalid = fi_no_atomic_writevalid,
	.readwritevalid = fi_no_atomic_readwritevalid,
	.compwritevalid = fi_no_atomic_compwritevalid,
};

static int xnet_rdm_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	return fi_setname(&rdm->pep->util_pep.pep_fid.fid, addr, addrlen);
}

static int xnet_rdm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	return fi_getname(&rdm->pep->util_pep.pep_fid.fid, addr, addrlen);
}

static struct fi_ops_cm xnet_rdm_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = xnet_rdm_setname,
	.getname = xnet_rdm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static ssize_t xnet_rdm_cancel(struct fid *fid, void *context)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	return fi_cancel(&rdm->srx->rx_fid.fid, context);
}

static int xnet_rdm_getopt(struct fid *fid, int level, int optname,
			   void *optval, size_t *optlen)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	if (level != FI_OPT_ENDPOINT)
		return -ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (*optlen < sizeof(size_t)) {
			*optlen = sizeof(size_t);
			return -FI_ETOOSMALL;
		}
		*((size_t *) optval) = rdm->srx->min_multi_recv_size;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_FIREWALL_ADDR:
		if (*optlen < sizeof(int)) {
			*optlen = sizeof(int);
			return -FI_ETOOSMALL;
		}
		*((int *) optval) = xnet_firewall_addr;
		*optlen = sizeof(int);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}
	return FI_SUCCESS;
}

static int xnet_rdm_setopt(struct fid *fid, int level, int optname,
			   const void *optval, size_t optlen)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		rdm->srx->min_multi_recv_size = *(size_t *) optval;
		FI_INFO(&xnet_prov, FI_LOG_EP_CTRL,
			"FI_OPT_MIN_MULTI_RECV set to %zu\n",
			rdm->srx->min_multi_recv_size);
		break;
	default:
		return -ENOPROTOOPT;
	}
	return FI_SUCCESS;
}

static struct fi_ops_ep xnet_rdm_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = xnet_rdm_cancel,
	.getopt = xnet_rdm_getopt,
	.setopt = xnet_rdm_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static int xnet_mplex_av_dup(struct util_ep *ep, struct xnet_mplex_av *mplex_av,
			     struct xnet_domain *subdomain, struct fid_av **av_fid)
{
	int ret, i;
	struct util_av *subav;
	size_t addr_size = sizeof(struct sockaddr_in6);
	char addr[sizeof(struct sockaddr_in6)];
	struct fi_av_attr av_attr = {
		.type = ep->domain->av_type,
		.count = mplex_av->util_av.av_entry_pool->entry_cnt,
		.flags = 0,
	};

	assert(ofi_genlock_held(&mplex_av->lock));
	ret = fi_av_open(&subdomain->util_domain.domain_fid, &av_attr, av_fid, NULL);
	if (ret)
		return ret;

	subav = container_of(*av_fid, struct util_av, av_fid);
	for (i = 0; i < mplex_av->util_av.av_entry_pool->entry_cnt; i++) {
		if (!ofi_ip_av_is_valid(&mplex_av->util_av.av_fid, i))
			continue;

		ret = ofi_ip_av_lookup(&mplex_av->util_av.av_fid, i, addr, &addr_size);
		if (ret)
			continue;

		ofi_genlock_lock(&subav->lock);
		ret = ofi_av_insert_addr_at(subav, addr, i);
		ofi_genlock_unlock(&subav->lock);
		if (ret)
			return ret;
	}
	fid_list_insert(&mplex_av->subav_list, NULL, &subav->av_fid.fid);
	return FI_SUCCESS;
};

static int xnet_set_subav(struct util_ep *ep, struct xnet_domain *subdomain)
{
	int ret;
	struct fid_list_entry *item;
	struct util_av *subav;
	struct fid_av *subav_fid;
	struct xnet_mplex_av *xnet_av;

	xnet_av = container_of(&ep->av->av_fid, struct xnet_mplex_av, util_av.av_fid);

	ofi_genlock_lock(&xnet_av->lock);
	dlist_foreach_container(&xnet_av->subav_list, struct fid_list_entry, item, entry) {
		subav = container_of(item->fid, struct util_av, av_fid.fid);
		if (subav->domain == &subdomain->util_domain)
			goto move_av;
	}
	ret = xnet_mplex_av_dup(ep, xnet_av, subdomain, &subav_fid);
	if (ret)
		goto out;
	subav = container_of(subav_fid, struct util_av, av_fid.fid);
move_av:
	ofi_genlock_lock(&ep->av->ep_list_lock);
	dlist_remove(&ep->av_entry);
	ofi_genlock_unlock(&ep->av->ep_list_lock);
	ofi_atomic_dec32(&ep->av->ref);
	ep->av = NULL;
	ret = ofi_ep_bind_av(ep, subav);
out:
	ofi_genlock_unlock(&xnet_av->lock);
	return ret;
}

static int xnet_reg_subdomain_mr(struct ofi_rbmap *map, struct ofi_rbnode *node, void *context)
{
	int ret;
	struct fi_mr_attr *attr = (struct fi_mr_attr *)node->data;
	struct xnet_domain *subdomain = context;

	ret = ofi_mr_map_insert(&subdomain->util_domain.mr_map, attr,
				&attr->requested_key, attr->context,
				((struct ofi_mr*)attr->context)->flags);
	if (ret) {
		XNET_WARN_ERR(FI_LOG_MR, "ofi_mr_map_insert", ret);
		return ret;
	}

	ofi_atomic_inc32(&subdomain->util_domain.ref);
	return FI_SUCCESS;
}

static struct xnet_domain *xnet_find_subdomain(struct xnet_rdm *rdm)
{
	int i;
	struct util_cntr *cntr;

	if (!xnet_domain_multiplexed(&rdm->util_ep.rx_cq->domain->domain_fid)) {
		return container_of(&rdm->util_ep.rx_cq->domain->domain_fid,
				    struct xnet_domain, util_domain.domain_fid);
	}

	if (!xnet_domain_multiplexed(&rdm->util_ep.tx_cq->domain->domain_fid)) {
		return container_of(&rdm->util_ep.tx_cq->domain->domain_fid,
					 struct xnet_domain, util_domain.domain_fid);
	}

	for (i = 0; i < CNTR_CNT; i++) {
		cntr = rdm->util_ep.cntrs[i];
		if (!cntr)
			continue;

		if (!xnet_domain_multiplexed(&cntr->domain->domain_fid)) {
			return container_of(&cntr->domain->domain_fid,
					 struct xnet_domain, util_domain.domain_fid);
		}
	}

	return NULL;
}

static void xnet_set_subdomain(struct xnet_rdm *rdm, struct xnet_domain *domain,
			  struct xnet_domain *subdomain)
{
	int i;
	struct util_cntr *cntr;

	assert(ofi_genlock_held(&domain->util_domain.lock));

	if (rdm->util_ep.rx_cq->domain == &domain->util_domain) {
		ofi_atomic_dec32(&rdm->util_ep.rx_cq->domain->ref);
		ofi_atomic_inc32(&subdomain->util_domain.ref);
		rdm->util_ep.rx_cq->domain = &subdomain->util_domain;
	}
	assert(rdm->util_ep.rx_cq->domain == &subdomain->util_domain);

	if (rdm->util_ep.tx_cq->domain == &domain->util_domain) {
		ofi_atomic_dec32(&rdm->util_ep.tx_cq->domain->ref);
		ofi_atomic_inc32(&subdomain->util_domain.ref);
		rdm->util_ep.tx_cq->domain = &subdomain->util_domain;
	}
	assert(rdm->util_ep.tx_cq->domain == &subdomain->util_domain);

	for (i = 0; i < CNTR_CNT; i++) {
		cntr = rdm->util_ep.cntrs[i];
		if (!cntr)
			continue;

		if (cntr->domain == &domain->util_domain) {
			ofi_atomic_dec32(&cntr->domain->ref);
			ofi_atomic_inc32(&subdomain->util_domain.ref);
			cntr->domain = &subdomain->util_domain;
		}
		assert(cntr->domain == &subdomain->util_domain);
	}
}

int xnet_rdm_resolve_domains(struct xnet_rdm *rdm)
{
	int ret;
	struct fid_domain *subdomain_fid;
	struct xnet_domain *subdomain;
	struct xnet_domain *domain;

	domain = container_of(rdm->util_ep.domain, struct xnet_domain, util_domain);
	ofi_genlock_lock(&domain->util_domain.lock);
	subdomain = xnet_find_subdomain(rdm);
	if (!subdomain) {
		ret = fi_domain(&domain->util_domain.fabric->fabric_fid,
				domain->subdomain_info,
				&subdomain_fid, NULL);
		if (ret)
			goto out;

		subdomain = container_of(subdomain_fid, struct xnet_domain,
					 util_domain.domain_fid);
		ret = fid_list_insert2(&domain->subdomain_list,
				       &domain->subdomain_list_lock,
				       &subdomain_fid->fid);
		if (ret) {
			fi_close(&subdomain_fid->fid);
			goto out;
		}

		ret = ofi_rbmap_foreach(domain->util_domain.mr_map.rbtree,
					domain->util_domain.mr_map.rbtree->root,
					xnet_reg_subdomain_mr, subdomain);
		if (ret)
			goto out;
	}

	xnet_set_subdomain(rdm, domain, subdomain);
	ret = xnet_set_subav(&rdm->util_ep, subdomain);
	if (ret)
		goto out;

	ofi_atomic_dec32(&rdm->util_ep.domain->ref);
	ofi_atomic_inc32(&subdomain->util_domain.ref);
	rdm->util_ep.domain = &subdomain->util_domain;

	ofi_atomic_dec32(&rdm->srx->domain->util_domain.ref);
	ofi_atomic_inc32(&subdomain->util_domain.ref);
	rdm->srx->domain = subdomain;

out:
	ofi_genlock_unlock(&domain->util_domain.lock);
	return ret;
}

static int xnet_enable_rdm(struct xnet_rdm *rdm)
{
	struct xnet_progress *progress;
	struct fi_info *info;
	size_t len;
	int ret;

	if (xnet_domain_multiplexed(&rdm->util_ep.domain->domain_fid)) {
		ret = xnet_rdm_resolve_domains(rdm);
		if (ret)
			return ret;
	}

	(void) fi_ep_bind(&rdm->srx->rx_fid, &rdm->util_ep.rx_cq->cq_fid.fid,
			  FI_RECV);
	if (rdm->util_ep.cntrs[CNTR_RX]) {
		(void) fi_ep_bind(&rdm->srx->rx_fid,
				  &rdm->util_ep.cntrs[CNTR_RX]->cntr_fid.fid, FI_RECV);
	}
	(void) fi_ep_bind(&rdm->srx->rx_fid, &rdm->util_ep.ep_fid.fid,
			  FI_TAGGED | FI_MSG);
	progress = xnet_rdm2_progress(rdm);
	ofi_genlock_lock(&progress->rdm_lock);

	ret = xnet_listen(rdm->pep, progress);
	if (ret)
		goto unlock;

	/* TODO: Move updating the src_addr to pep_listen(). */
	len = sizeof(rdm->addr);
	ret = fi_getname(&rdm->pep->util_pep.pep_fid.fid, &rdm->addr, &len);
	if (ret) {
		XNET_WARN_ERR(FI_LOG_EP_CTRL, "fi_getname", ret);
		goto unlock;
	}

	/* Update src_addr that will be used for active endpoints.
	 * Zero out the port to avoid address conflicts, as we will
	 * create multiple msg ep's for a single rdm ep.
	 */
	info = rdm->pep->info;
	free(info->src_addr);
	info->src_addr = NULL;
	info->src_addrlen = 0;

	info->src_addr = mem_dup(&rdm->addr, len);
	if (!info->src_addr) {
		ret = -FI_ENOMEM;
		goto unlock;
	}

	info->src_addrlen = len;
	ofi_addr_set_port(info->src_addr, 0);

unlock:
	ofi_genlock_unlock(&progress->rdm_lock);
	return ret;
}

static int xnet_rdm_ctrl(struct fid *fid, int command, void *arg)
{
	struct xnet_rdm *rdm;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	switch (command) {
	case FI_ENABLE:
		if (!rdm->util_ep.av)
			return -FI_EOPBADSTATE;

		if (!rdm->util_ep.tx_cq || !rdm->util_ep.rx_cq)
			return -FI_ENOCQ;

		return xnet_enable_rdm(rdm);
	default:
		return -FI_ENOSYS;
	}
	return 0;
}

static int xnet_rdm_close(struct fid *fid)
{
	struct xnet_rdm *rdm;
	int ret;

	rdm = container_of(fid, struct xnet_rdm, util_ep.ep_fid.fid);
	ofi_genlock_lock(&xnet_rdm2_progress(rdm)->rdm_lock);
	ret = fi_close(&rdm->pep->util_pep.pep_fid.fid);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, \
			"Unable to close passive endpoint\n");
		ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);
		return ret;
	}

	xnet_freeall_conns(rdm);
	ofi_genlock_unlock(&xnet_rdm2_progress(rdm)->rdm_lock);

	ret = fi_close(&rdm->srx->rx_fid.fid);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, \
			"Unable to close msg shared ctx\n");
		return ret;
	}

	ofi_endpoint_close(&rdm->util_ep);
	free(rdm);
	return 0;
}

static struct fi_ops xnet_rdm_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_rdm_close,
	.bind = ofi_ep_fid_bind,
	.control = xnet_rdm_ctrl,
	.ops_open = xnet_rdm_ops_open,
};

static int xnet_init_rdm(struct xnet_rdm *rdm, struct fi_info *info)
{
	struct fi_info *msg_info;
	struct fid_ep *srx;
	struct fid_pep *pep;
	int ret;

	msg_info = fi_dupinfo(&xnet_srx_info);
	if (!msg_info)
		return -FI_ENOMEM;

	msg_info->caps &= info->caps;
	msg_info->mode = info->mode;
	msg_info->addr_format = info->addr_format;
	if (info->src_addrlen) {
		msg_info->src_addr = mem_dup(info->src_addr, info->src_addrlen);
		if (!msg_info->src_addr) {
			ret = -FI_ENOMEM;
			goto err1;
		}
		msg_info->src_addrlen = info->src_addrlen;
	}
	msg_info->domain_attr->caps &= info->domain_attr->caps;
	msg_info->domain_attr->mr_mode = info->domain_attr->mr_mode;
	msg_info->tx_attr->caps &= info->tx_attr->caps;
	msg_info->tx_attr->op_flags = info->tx_attr->op_flags;
	msg_info->rx_attr->caps &= info->rx_attr->caps;
	msg_info->rx_attr->op_flags = info->rx_attr->op_flags;

	ret = fi_srx_context(&rdm->util_ep.domain->domain_fid, info->rx_attr,
			     &srx, rdm);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Unable to open shared receive context\n");
		goto err1;
	}

	ret = fi_passive_ep(&rdm->util_ep.domain->fabric->fabric_fid, msg_info,
			    &pep, rdm);
	if (ret) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"Unable to open passive ep\n");
		goto err2;
	}

	rdm->srx = container_of(srx, struct xnet_srx, rx_fid);
	rdm->pep = container_of(pep, struct xnet_pep, util_pep);
	fi_freeinfo(msg_info);
	return 0;

err2:
	fi_close(&srx->fid);
err1:
	fi_freeinfo(msg_info);
	return ret;
}

int xnet_rdm_ep(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep_fid, void *context)
{
	struct xnet_rdm *rdm;
	int ret;

	rdm = calloc(1, sizeof(*rdm));
	if (!rdm)
		return -FI_ENOMEM;

	ret = ofi_endpoint_init(domain, &xnet_util_prov, info, &rdm->util_ep,
				context, NULL);
	if (ret)
		goto err1;

	assert(info->ep_attr->type == FI_EP_RDM);
	ret = xnet_init_rdm(rdm, info);
	if (ret)
		goto err2;

	*ep_fid = &rdm->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &xnet_rdm_fid_ops;
	(*ep_fid)->ops = &xnet_rdm_ep_ops;
	(*ep_fid)->cm = &xnet_rdm_cm_ops;
	(*ep_fid)->msg = &xnet_rdm_msg_ops;
	(*ep_fid)->rma = &xnet_rdm_rma_ops;
	(*ep_fid)->tagged = &xnet_rdm_tagged_ops;
	(*ep_fid)->atomic = &xnet_rdm_atomic_ops;

	return 0;

err2:
	ofi_endpoint_close(&rdm->util_ep);
err1:
	free(rdm);
	return ret;
}
