/*
 * Copyright (c) 2013-2016 Intel Corporation. All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include "udpx.h"


static int udpx_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct udpx_ep *ep;
	int ret;

	ep = container_of(fid, struct udpx_ep, util_ep.ep_fid.fid);
	ofi_straddr_dbg(&udpx_prov, FI_LOG_EP_CTRL, "bind addr: ", addr);
	ret = bind(ep->sock, addr, (socklen_t)addrlen);
	if (ret) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "bind %d (%s)\n",
			errno, strerror(errno));
		return -errno;
	}
	ep->is_bound = 1;
	return 0;
}

static int udpx_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct udpx_ep *ep =
		container_of(fid, struct udpx_ep, util_ep.ep_fid.fid);
	size_t buflen = *addrlen;

	if (ofi_getsockname(ep->sock, addr, (socklen_t *)addrlen))
		return -ofi_sockerr();

	return buflen < *addrlen ? -FI_ETOOSMALL : 0;
}

static int udpx_mc_close(struct fid *fid)
{
	struct udpx_mc *mc;
	struct ip_mreq mreq;
	int ret;

	mc = container_of(fid, struct udpx_mc, mc_fid.fid);
	mreq.imr_multiaddr = mc->addr.sin.sin_addr;
	mreq.imr_interface.s_addr = INADDR_ANY;
	ret = setsockopt(mc->ep->sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
			 (const void *)&mreq, sizeof(mreq));
	if (ret) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "leave failed %s\n",
			strerror(errno));
		return -errno;
	}

	ofi_atomic_dec32(&mc->ep->ref);
	free(mc);
	return 0;
}

static struct fi_ops udpx_mc_ops = {
	.size = sizeof(struct fi_ops),
	.close = udpx_mc_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static void udpx_mc_init(struct udpx_ep *ep, struct udpx_mc *mc,
			 const void *addr, uint64_t flags, void *context)
{
	mc->mc_fid.fid.fclass = FI_CLASS_MC;
	mc->mc_fid.fid.context = context;
	mc->mc_fid.fid.ops = &udpx_mc_ops;
	mc->mc_fid.fi_addr = (uintptr_t) &mc->addr;

	memcpy(&mc->addr, addr, ofi_sizeofaddr(addr));
	mc->ep = ep;
	ofi_atomic_inc32(&ep->ref);
}

static int udpx_join_ip(struct udpx_mc *mc, const struct sockaddr_in *sin,
			uint64_t flags)
{
	struct fi_eq_err_entry entry;
	struct ip_mreq mreq;
	char str[INET6_ADDRSTRLEN + 8];
	ssize_t bytes;
	int ret;

	bytes = sizeof str;
	FI_INFO(&udpx_prov, FI_LOG_EP_CTRL, "Joining %s\n",
		ofi_straddr(str, (size_t *) &bytes, FI_SOCKADDR_IN, sin));

	memset(&entry, 0, sizeof entry);
	entry.fid = &mc->mc_fid.fid;
	entry.context = mc->mc_fid.fid.context;
	bytes = sizeof(struct fi_eq_entry);

	if (ofi_recv_allowed(mc->ep->util_ep.caps)) {
		mreq.imr_multiaddr = sin->sin_addr;
		mreq.imr_interface.s_addr = INADDR_ANY;
		ret = setsockopt(mc->ep->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				 (const void *)&mreq, sizeof(mreq));
		if (ret) {
			FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "join failed %s\n",
				strerror(errno));
			entry.err = -errno;
			bytes = sizeof(entry);
		}
	}

	bytes = fi_eq_write(&mc->ep->util_ep.eq->eq_fid, FI_JOIN_COMPLETE,
			    &entry, (size_t) bytes, 0);
	return bytes < 0 ? (int) bytes : 0;
}

static int udpx_join(struct fid_ep *ep, const void *addr, uint64_t flags,
		     struct fid_mc **mc, void *context)
{
	struct udpx_ep *udp_ep;
	struct udpx_mc *udp_mc;
	int ret;

	udp_ep = container_of(ep, struct udpx_ep, util_ep.ep_fid);
	if (!udp_ep->util_ep.eq) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "no EQ bound to EP\n");
		return -FI_ENOEQ;
	}

	if (!udp_ep->is_bound) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "EP not enabled\n");
		return -FI_EOPBADSTATE;
	}

	if (((struct sockaddr *) addr)->sa_family != AF_INET) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "only ipv4 supported\n");
		return -FI_ENOSYS;
	}

	udp_mc = calloc(1, sizeof(*udp_mc));
	if (!udp_mc)
		return -FI_ENOMEM;

	udpx_mc_init(udp_ep, udp_mc, addr, flags, context);
	*mc = &udp_mc->mc_fid;
	ret = udpx_join_ip(udp_mc, addr, flags);
	return ret;
}

static struct fi_ops_cm udpx_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = udpx_setname,
	.getname = udpx_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = udpx_join,
};


static int udpx_getopt(fid_t fid, int level, int optname,
		       void *optval, size_t *optlen)
{
	return -FI_ENOPROTOOPT;
}

static int udpx_setopt(fid_t fid, int level, int optname,
		       const void *optval, size_t optlen)
{
	return -FI_ENOPROTOOPT;
}

static struct fi_ops_ep udpx_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = udpx_getopt,
	.setopt = udpx_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};


static void udpx_tx_comp(struct udpx_ep *ep, void *context)
{
	struct fi_cq_tagged_entry *comp;

	comp = ofi_cirque_tail(ep->util_ep.tx_cq->cirq);
	comp->op_context = context;
	comp->flags = FI_SEND;
	comp->len = 0;
	comp->buf = NULL;
	comp->data = 0;
	ofi_cirque_commit(ep->util_ep.tx_cq->cirq);
}

static void udpx_tx_comp_signal(struct udpx_ep *ep, void *context)
{
	udpx_tx_comp(ep, context);
	ep->util_ep.tx_cq->wait->signal(ep->util_ep.tx_cq->wait);
}

static void udpx_rx_comp(struct udpx_ep *ep, void *context, uint64_t flags,
			 size_t len, void *buf, void *addr)
{
	struct fi_cq_tagged_entry *comp;

	comp = ofi_cirque_tail(ep->util_ep.rx_cq->cirq);
	comp->op_context = context;
	comp->flags = FI_RECV | flags;
	comp->len = len;
	comp->buf = buf;
	comp->data = 0;
	ofi_cirque_commit(ep->util_ep.rx_cq->cirq);
}

static void udpx_rx_src_comp(struct udpx_ep *ep, void *context, uint64_t flags,
			     size_t len, void *buf, void *addr)
{
	ep->util_ep.rx_cq->src[ofi_cirque_windex(ep->util_ep.rx_cq->cirq)] =
			ofi_ip_av_get_fi_addr(ep->util_ep.av, addr);
	udpx_rx_comp(ep, context, flags, len, buf, addr);
}

static void udpx_rx_comp_signal(struct udpx_ep *ep, void *context,
			uint64_t flags, size_t len, void *buf, void *addr)
{
	udpx_rx_comp(ep, context, flags, len, buf, addr);
	ep->util_ep.rx_cq->wait->signal(ep->util_ep.rx_cq->wait);
}

static void udpx_rx_src_comp_signal(struct udpx_ep *ep, void *context,
			uint64_t flags, size_t len, void *buf, void *addr)
{
	udpx_rx_src_comp(ep, context, flags, len, buf, addr);
	ep->util_ep.rx_cq->wait->signal(ep->util_ep.rx_cq->wait);
}

static void udpx_ep_progress(struct util_ep *util_ep)
{
	struct udpx_ep *ep;
	struct udpx_ep_entry *entry;
	struct msghdr hdr;
	struct sockaddr_in6 addr;
	ssize_t ret;

	ep = container_of(util_ep, struct udpx_ep, util_ep);
	hdr.msg_name = &addr;
	hdr.msg_namelen = sizeof(addr);
	hdr.msg_control = NULL;
	hdr.msg_controllen = 0;
	hdr.msg_flags = 0;

	fastlock_acquire(&ep->util_ep.rx_cq->cq_lock);
	if (ofi_cirque_isempty(ep->rxq))
		goto out;

	entry = ofi_cirque_head(ep->rxq);
	hdr.msg_iov = entry->iov;
	hdr.msg_iovlen = entry->iov_count;

	ret = ofi_recvmsg_udp(ep->sock, &hdr, 0);
	if (ret >= 0) {
		ep->rx_comp(ep, entry->context, 0, ret, NULL, &addr);
		ofi_cirque_discard(ep->rxq);
	}
out:
	fastlock_release(&ep->util_ep.rx_cq->cq_lock);
}

static ssize_t udpx_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			    uint64_t flags)
{
	struct udpx_ep *ep;
	struct udpx_ep_entry *entry;
	ssize_t ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	fastlock_acquire(&ep->util_ep.rx_cq->cq_lock);
	if (ofi_cirque_isfull(ep->rxq)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	entry = ofi_cirque_tail(ep->rxq);
	entry->context = msg->context;
	for (entry->iov_count = 0; entry->iov_count < msg->iov_count;
	     entry->iov_count++) {
		entry->iov[entry->iov_count] = msg->msg_iov[entry->iov_count];
	}
	entry->flags = 0;

	ofi_cirque_commit(ep->rxq);
	ret = 0;
out:
	fastlock_release(&ep->util_ep.rx_cq->cq_lock);
	return ret;
}

static ssize_t udpx_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t src_addr,
			  void *context)
{
	struct fi_msg msg;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.context = context;
	return udpx_recvmsg(ep_fid, &msg, 0);
}

static ssize_t udpx_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			 void *desc, fi_addr_t src_addr, void *context)
{
	struct udpx_ep *ep;
	struct udpx_ep_entry *entry;
	ssize_t ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	fastlock_acquire(&ep->util_ep.rx_cq->cq_lock);
	if (ofi_cirque_isfull(ep->rxq)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	entry = ofi_cirque_tail(ep->rxq);
	entry->context = context;
	entry->iov_count = 1;
	entry->iov[0].iov_base = buf;
	entry->iov[0].iov_len = len;
	entry->flags = 0;

	ofi_cirque_commit(ep->rxq);
	ret = 0;
out:
	fastlock_release(&ep->util_ep.rx_cq->cq_lock);
	return ret;
}

static const void *
udpx_dest_addr(struct udpx_ep *ep, fi_addr_t addr, uint64_t flags)
{
	return (flags & FI_MULTICAST) ?
	       (const void *) (uintptr_t) addr :
	       ofi_ip_av_get_addr(ep->util_ep.av, (int)addr);
}

static size_t
udpx_dest_addrlen(struct udpx_ep *ep, fi_addr_t addr, uint64_t flags)
{
	return (flags & FI_MULTICAST) ?
		ofi_sizeofaddr((const void *) (uintptr_t) addr) :
		ep->util_ep.av->addrlen;
}

static ssize_t udpx_sendto(struct udpx_ep *ep, const void *buf, size_t len,
			   const void *addr, size_t addrlen, void *context)
{
	ssize_t ret;

	fastlock_acquire(&ep->util_ep.tx_cq->cq_lock);
	if (ofi_cirque_isfull(ep->util_ep.tx_cq->cirq)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	ret = ofi_sendto_socket(ep->sock, buf, len, 0,
				addr, (socklen_t)addrlen);
	if (ret == (ssize_t)len) {
		ep->tx_comp(ep, context);
		ret = 0;
	} else {
		ret = -errno;
	}
out:
	fastlock_release(&ep->util_ep.tx_cq->cq_lock);
	return ret;
}

static ssize_t udpx_send(struct fid_ep *ep_fid, const void *buf, size_t len,
			 void *desc, fi_addr_t dest_addr, void *context)
{
	struct udpx_ep *ep;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	return udpx_sendto(ep, buf, len, ofi_ip_av_get_addr(ep->util_ep.av, (int)dest_addr),
			   ep->util_ep.av->addrlen, context);
}

static ssize_t udpx_send_mc(struct fid_ep *ep_fid, const void *buf, size_t len,
			    void *desc, fi_addr_t dest_addr, void *context)
{
	struct udpx_ep *ep;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	return udpx_sendto(ep, buf, len, (const void *) (uintptr_t) dest_addr,
			   ofi_sizeofaddr((const void *) (uintptr_t) dest_addr),
			   context);
}

static ssize_t udpx_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			    uint64_t flags)
{
	struct udpx_ep *ep;
	struct msghdr hdr;
	ssize_t ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	hdr.msg_name = (void *)udpx_dest_addr(ep, msg->addr, flags);
	hdr.msg_namelen = (int)udpx_dest_addrlen(ep, msg->addr, flags);
	hdr.msg_iov = (struct iovec *)msg->msg_iov;
	hdr.msg_iovlen = msg->iov_count;
	hdr.msg_control = NULL;
	hdr.msg_controllen = 0;
	hdr.msg_flags = 0;

	fastlock_acquire(&ep->util_ep.tx_cq->cq_lock);
	if (ofi_cirque_isfull(ep->util_ep.tx_cq->cirq)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	ret = ofi_sendmsg_udp(ep->sock, &hdr, 0);
	if (ret >= 0) {
		ep->tx_comp(ep, msg->context);
		ret = 0;
	} else {
		ret = -errno;
	}
out:
	fastlock_release(&ep->util_ep.tx_cq->cq_lock);
	return ret;
}

static ssize_t udpx_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t dest_addr,
			  void *context)
{
	struct fi_msg msg;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.addr = dest_addr;
	msg.context = context;

	return udpx_sendmsg(ep_fid, &msg, 0);
}

static ssize_t udpx_sendv_mc(struct fid_ep *ep_fid, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t dest_addr,
			     void *context)
{
	struct fi_msg msg;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.addr = dest_addr;
	msg.context = context;

	return udpx_sendmsg(ep_fid, &msg, FI_MULTICAST);
}

static ssize_t udpx_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
			   fi_addr_t dest_addr)
{
	struct udpx_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	ret = ofi_sendto_socket(ep->sock, buf, len, 0,
				ofi_ip_av_get_addr(ep->util_ep.av, (int)dest_addr),
				(socklen_t)ep->util_ep.av->addrlen);
	return ret == (ssize_t)len ? 0 : -errno;
}

static ssize_t udpx_inject_mc(struct fid_ep *ep_fid, const void *buf,
			      size_t len, fi_addr_t dest_addr)
{
	struct udpx_ep *ep;
	ssize_t ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	ret = ofi_sendto_socket(ep->sock, buf, len, 0,
				(const void *)(uintptr_t)dest_addr,
				(socklen_t)ofi_sizeofaddr((const void *)(uintptr_t)dest_addr));
	return ret == (ssize_t)len ? 0 : -errno;
}

static struct fi_ops_msg udpx_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = udpx_recv,
	.recvv = udpx_recvv,
	.recvmsg = udpx_recvmsg,
	.send = udpx_send,
	.sendv = udpx_sendv,
	.sendmsg = udpx_sendmsg,
	.inject = udpx_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static struct fi_ops_msg udpx_msg_mcast_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = udpx_recv,
	.recvv = udpx_recvv,
	.recvmsg = udpx_recvmsg,
	.send = udpx_send_mc,
	.sendv = udpx_sendv_mc,
	.sendmsg = udpx_sendmsg,
	.inject = udpx_inject_mc,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static int udpx_ep_close(struct fid *fid)
{
	struct udpx_ep *ep;
	struct util_wait_fd *wait;

	ep = container_of(fid, struct udpx_ep, util_ep.ep_fid.fid);
	if (ofi_atomic_get32(&ep->ref)) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "EP busy\n");
		return -FI_EBUSY;
	}

	if (ep->util_ep.rx_cq) {
		if (ep->util_ep.rx_cq->wait) {
			wait = container_of(ep->util_ep.rx_cq->wait,
					    struct util_wait_fd, util_wait);
			ofi_epoll_del(wait->epoll_fd, (int)ep->sock);
		}
		fid_list_remove(&ep->util_ep.rx_cq->ep_list,
				&ep->util_ep.rx_cq->ep_list_lock,
				&ep->util_ep.ep_fid.fid);
	}

	udpx_rx_cirq_free(ep->rxq);
	ofi_close_socket(ep->sock);
	ofi_endpoint_close(&ep->util_ep);
	free(ep);
	return 0;
}

static int udpx_ep_bind_cq(struct udpx_ep *ep, struct util_cq *cq,
			   uint64_t flags)
{
	struct util_wait_fd *wait;
	int ret;

	ret = ofi_check_bind_cq_flags(&ep->util_ep, cq, flags);
	if (ret)
		return ret;

	if (flags & FI_TRANSMIT) {
		ep->util_ep.tx_cq = cq;
		ofi_atomic_inc32(&cq->ref);
		ep->tx_comp = cq->wait ? udpx_tx_comp_signal :
					 udpx_tx_comp;
	}

	if (flags & FI_RECV) {
		ep->util_ep.rx_cq = cq;
		ofi_atomic_inc32(&cq->ref);

		if (cq->wait) {
			ep->rx_comp =
				(cq->domain->info_domain_caps & FI_SOURCE) ?
				udpx_rx_src_comp_signal :
				udpx_rx_comp_signal;

			wait = container_of(cq->wait,
					    struct util_wait_fd, util_wait);
			ret = ofi_epoll_add(wait->epoll_fd, (int)ep->sock,
					   OFI_EPOLL_IN, &ep->util_ep.ep_fid.fid);
			if (ret)
				return ret;
		} else {
			ep->rx_comp =
				(cq->domain->info_domain_caps & FI_SOURCE) ?
				udpx_rx_src_comp : udpx_rx_comp;
		}

		ret = fid_list_insert(&cq->ep_list,
				      &cq->ep_list_lock,
				      &ep->util_ep.ep_fid.fid);
		if (ret)
			return ret;
	}

	return 0;
}

static int udpx_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct udpx_ep *ep;
	struct util_av *av;
	struct util_eq *eq;
	int ret;

	ret = ofi_ep_bind_valid(&udpx_prov, bfid, flags);
	if (ret)
		return ret;

	ep = container_of(ep_fid, struct udpx_ep, util_ep.ep_fid.fid);
	switch (bfid->fclass) {
	case FI_CLASS_AV:
		av = container_of(bfid, struct util_av, av_fid.fid);
		ret = ofi_ep_bind_av(&ep->util_ep, av);
		break;
	case FI_CLASS_CQ:
		ret = udpx_ep_bind_cq(ep, container_of(bfid, struct util_cq,
							cq_fid.fid), flags);
		break;
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct util_eq, eq_fid.fid);
		ret = ofi_ep_bind_eq(&ep->util_ep, eq);
		break;
	default:
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL,
			"invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static void udpx_bind_src_addr(struct udpx_ep *ep)
{
	int ret;
	struct addrinfo ai, *rai = NULL, *cur_ai;
	char hostname[HOST_NAME_MAX];

	memset(&ai, 0, sizeof(ai));
	ai.ai_socktype = SOCK_DGRAM;

	ret = gethostname(hostname, sizeof(hostname));
	ret = getaddrinfo(ret ? "127.0.0.1" : hostname, NULL, &ai, &rai);
	if (ret) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL,
			"getaddrinfo failed\n");
		return;
	}

	for (cur_ai = rai; cur_ai && cur_ai->ai_family != AF_INET;
	     cur_ai = cur_ai->ai_next)
		;

	if (cur_ai) {
		ret = udpx_setname(&ep->util_ep.ep_fid.fid, cur_ai->ai_addr,
				   cur_ai->ai_addrlen);
	} else {
		ret = -FI_EADDRNOTAVAIL;
	}
	if (ret) {
		FI_WARN(&udpx_prov, FI_LOG_EP_CTRL, "failed to set addr\n");
	}
	freeaddrinfo(rai);
}

static int udpx_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct udpx_ep *ep;

	ep = container_of(fid, struct udpx_ep, util_ep.ep_fid.fid);
	switch (command) {
	case FI_ENABLE:
		if (!ep->util_ep.rx_cq || !ep->util_ep.tx_cq)
			return -FI_ENOCQ;
		if (!ep->util_ep.av)
			return -FI_ENOAV;

		if (!ep->is_bound)
			udpx_bind_src_addr(ep);
		break;
	default:
		return -FI_ENOSYS;
	}
	return 0;
}

static struct fi_ops udpx_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = udpx_ep_close,
	.bind = udpx_ep_bind,
	.control = udpx_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int udpx_ep_init(struct udpx_ep *ep, struct fi_info *info)
{
	int family;
	int ret;

	ofi_atomic_initialize32(&ep->ref, 0);
	ep->rxq = udpx_rx_cirq_create(info->rx_attr->size);
	if (!ep->rxq) {
		ret = -FI_ENOMEM;
		return ret;
	}

	family = info->src_addr ?
		 ((struct sockaddr *) info->src_addr)->sa_family : AF_INET;
	ep->sock = socket(family, SOCK_DGRAM, IPPROTO_UDP);
	if (ep->sock < 0) {
		ret = -errno;
		goto err1;
	}

	if (info->src_addr) {
		ret = udpx_setname(&ep->util_ep.ep_fid.fid, info->src_addr,
				   info->src_addrlen);
		if (ret)
			goto err1;
	}

	ret = fi_fd_nonblock((int)ep->sock);
	if (ret)
		goto err2;

	return 0;
err2:
	ofi_close_socket(ep->sock);
err1:
	udpx_rx_cirq_free(ep->rxq);
	return ret;
}

int udpx_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context)
{
	struct udpx_ep *ep;
	int ret;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = ofi_endpoint_init(domain, &udpx_util_prov, info, &ep->util_ep,
				context, udpx_ep_progress);
	if (ret)
		goto err1;

	ret = udpx_ep_init(ep, info);
	if (ret)
		goto err2;

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &udpx_ep_fi_ops;
	(*ep_fid)->ops = &udpx_ep_ops;
	(*ep_fid)->cm = &udpx_cm_ops;
	(*ep_fid)->msg = (info->tx_attr->op_flags & FI_MULTICAST) ?
			 &udpx_msg_mcast_ops : &udpx_msg_ops;

	return 0;
err2:
	ofi_endpoint_close(&ep->util_ep);
err1:
	free(ep);
	return ret;
}
