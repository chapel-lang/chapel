/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DIRECT_ENDPOINT_H_
#define _FI_PROV_OPX_DIRECT_ENDPOINT_H_

#define FABRIC_DIRECT_ENDPOINT 1

#ifdef FABRIC_DIRECT

#define fi_send(ep, buf, len, desc, dest_addr, context)			\
	(fi_opx_send_FABRIC_DIRECT(ep, buf, len, desc, dest_addr,	\
		context))

ssize_t
fi_opx_send_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, void *context);


#define fi_recv(ep, buf, len, desc, src_addr, context)			\
	(fi_opx_recv_FABRIC_DIRECT(ep, buf, len, desc, src_addr,	\
		context))

ssize_t
fi_opx_recv_FABRIC_DIRECT(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, void *context);


#define fi_inject(ep, buf, len, dest_addr)				\
	(fi_opx_inject_FABRIC_DIRECT(ep, buf, len, dest_addr))

ssize_t
fi_opx_inject_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr);


#define fi_recvmsg(ep, msg, flags)					\
	(fi_opx_recvmsg_FABRIC_DIRECT(ep, msg, flags))

ssize_t
fi_opx_recvmsg_FABRIC_DIRECT(struct fid_ep *ep, const struct fi_msg *msg,
		uint64_t flags);


#define fi_senddata(ep, buf, len, desc, data, dest_addr, context)	\
	(fi_opx_senddata_FABRIC_DIRECT(ep, buf, len, desc, data,	\
		dest_addr, context))

ssize_t
fi_opx_senddata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, uint64_t data, fi_addr_t dest_addr, void *context);


#define fi_injectdata(ep, buf, len, data, dest_addr)			\
	(fi_opx_injectdata_FABRIC_DIRECT(ep, buf, len, data,		\
		dest_addr))

ssize_t
fi_opx_injectdata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf,
		size_t len, uint64_t data, fi_addr_t dest_addr);


static inline ssize_t
fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	return ep->msg->sendmsg(ep, msg, flags);
}

static inline ssize_t
fi_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, void *context)
{
	return ep->msg->sendv(ep, iov, desc, count, dest_addr, context);
}

static inline int
fi_enable(struct fid_ep *ep)
{
	return ep->fid.ops->control(&ep->fid, FI_ENABLE, NULL);
}

static inline int
fi_cancel(fid_t fid, void *context)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->cancel(fid, context);
}

static inline int
fi_endpoint(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **ep, void *context)
{
	return domain->ops->endpoint(domain, info, ep, context);
}

static inline int
fi_scalable_ep(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **sep, void *context)
{
	return domain->ops->scalable_ep(domain, info, sep, context);
}

static inline int
fi_setopt(fid_t fid, int level, int optname,
		const void *optval, size_t optlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->setopt(fid, level, optname, optval, optlen);
}

static inline int
fi_getopt(fid_t fid, int level, int optname,
		void *optval, size_t *optlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->ops->getopt(fid, level, optname, optval, optlen);
}

static inline int
fi_tx_context(struct fid_ep *ep, int index, struct fi_tx_attr *attr,
	      struct fid_ep **tx_ep, void *context)
{
	return ep->ops->tx_ctx(ep, index, attr, tx_ep, context);
}

static inline int
fi_rx_context(struct fid_ep *ep, int index, struct fi_rx_attr *attr,
	      struct fid_ep **rx_ep, void *context)
{
	return ep->ops->rx_ctx(ep, index, attr, rx_ep, context);
}

static inline int
fi_ep_bind(struct fid_ep *ep, struct fid *bfid, uint64_t flags)
{
	return ep->fid.ops->bind(&ep->fid, bfid, flags);
}

static inline int
fi_scalable_ep_bind(struct fid_ep *sep, struct fid *bfid, uint64_t flags)
{
	return sep->fid.ops->bind(&sep->fid, bfid, flags);
}

static inline int
fi_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr,
	       struct fid_stx **stx, void *context)
{
	return domain->ops->stx_ctx(domain, attr, stx, context);
}

static inline int
fi_ep_alias(struct fid_ep *ep, struct fid_ep **alias_ep, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int
fi_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
	     struct fid_pep **pep, void *context)
{
	return fabric->ops->passive_ep(fabric, info, pep, context);
}

static inline int fi_pep_bind(struct fid_pep *pep, struct fid *bfid, uint64_t flags)
{
	return pep->fid.ops->bind(&pep->fid, bfid, flags);
}

#endif

#endif /* _FI_PROV_OPX_DIRECT_EP_H_ */
