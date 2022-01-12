/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
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

#ifndef _FI_DIRECT_ENDPOINT_H_
#define _FI_DIRECT_ENDPOINT_H_

#define FABRIC_DIRECT_ENDPOINT 1

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern int gnix_pep_open(struct fid_fabric *fabric, struct fi_info *info,
			 struct fid_pep **pep, void *context);

extern int gnix_ep_open(struct fid_domain *domain, struct fi_info *info,
			struct fid_ep **ep, void *context);

extern int gnix_sep_open(struct fid_domain *domain,
			 struct fi_info *info, struct fid_ep **ep,
			 void *context);

extern int gnix_ep_bind(fid_t fid, fid_t bfid, uint64_t flags);

extern int gnix_pep_bind(fid_t pep, struct fid *bfid, uint64_t flags);

extern int gnix_sep_bind(fid_t sep, fid_t bfid, uint64_t flags);

extern int gnix_ep_control(fid_t fid, int command, void *arg);

extern ssize_t gnix_ep_cancel(fid_t fid, void *context);

extern int gnix_ep_setopt(fid_t fid, int level, int optname, const void *optval,
			  size_t optlen);

extern int gnix_getopt(fid_t fid, int level, int optname, void *optval,
		       size_t *optlen);

extern int gnix_tx_context(struct fid_ep *ep, int index,
			   struct fi_tx_attr *attr, struct fid_ep **tx_ep,
			   void *context);

extern int gnix_rx_context(struct fid_ep *ep, int index,
			   struct fi_rx_attr *attr, struct fid_ep **rx_ep,
			   void *context);

extern ssize_t gnix_ep_tx_size_left(struct fid_ep *ep);

extern ssize_t gnix_ep_rx_size_left(struct fid_ep *ep);

extern int gnix_stx_open(struct fid_domain *dom, struct fi_tx_attr *tx_attr,
			 struct fid_stx **stx, void *context);

extern int gnix_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
			    struct fid_ep **rx_ep, void *context);

extern ssize_t gnix_ep_recv(struct fid_ep *ep, void *buf, size_t len,
			    void *desc, fi_addr_t src_addr, void *context);

extern ssize_t gnix_ep_recvv(struct fid_ep *ep, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t src_addr,
			     void *context);

extern ssize_t gnix_ep_recvmsg(struct fid_ep *ep, const struct fi_msg *msg,
			       uint64_t flags);

extern ssize_t gnix_ep_send(struct fid_ep *ep, const void *buf, size_t len,
			    void *desc, fi_addr_t dest_addr, void *context);

extern ssize_t gnix_ep_sendv(struct fid_ep *ep, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t dest_addr,
			     void *context);

extern ssize_t gnix_ep_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			       uint64_t flags);

extern ssize_t gnix_ep_msg_inject(struct fid_ep *ep, const void *buf,
				  size_t len, fi_addr_t dest_addr);

extern ssize_t gnix_ep_senddata(struct fid_ep *ep, const void *buf, size_t len,
				void *desc, uint64_t data, fi_addr_t dest_addr,
				void *context);

extern ssize_t gnix_ep_msg_injectdata(struct fid_ep *ep, const void *buf,
				      size_t len, uint64_t data,
				      fi_addr_t dest_addr);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline int fi_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
				struct fid_pep **pep, void *context)
{
	return gnix_pep_open(fabric, info, pep, context);
}

static inline int fi_endpoint(struct fid_domain *domain, struct fi_info *info,
			      struct fid_ep **ep, void *context)
{
	return gnix_ep_open(domain, info, ep, context);
}

static inline int fi_scalable_ep(struct fid_domain *domain,
				 struct fi_info *info, struct fid_ep **sep,
				 void *context)
{
	return gnix_sep_open(domain, info, sep, context);
}

static inline int fi_ep_bind(struct fid_ep *ep, fid_t bfid, uint64_t flags)
{
	return gnix_ep_bind(&ep->fid, bfid, flags);
}

static inline int fi_pep_bind(struct fid_pep *pep, fid_t bfid, uint64_t flags)
{
	return gnix_pep_bind(&pep->fid, bfid, flags);
}

static inline int fi_scalable_ep_bind(struct fid_ep *sep, fid_t bfid,
				      uint64_t flags)
{
	return gnix_sep_bind(&sep->fid, bfid, flags);
}

static inline int fi_enable(struct fid_ep *ep)
{
	return gnix_ep_control(&ep->fid, FI_ENABLE, NULL);
}

static inline ssize_t fi_cancel(fid_t fid, void *context)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return gnix_ep_cancel(&ep->fid, context);
}

static inline int fi_setopt(fid_t fid, int level, int optname,
			    const void *optval, size_t optlen)
{
	return gnix_ep_setopt(fid, level, optname, optval, optlen);
}

static inline int fi_getopt(fid_t fid, int level, int optname, void *optval,
			    size_t *optlen)
{
	return gnix_getopt(fid, level, optname, optval, optlen);
}

static inline int fi_tx_context(struct fid_ep *ep, int index,
				struct fi_tx_attr *attr, struct fid_ep **tx_ep,
				void *context)
{
	return gnix_tx_context(ep, index, attr, tx_ep, context);
}

static inline int fi_rx_context(struct fid_ep *ep, int index,
				struct fi_rx_attr *attr, struct fid_ep **rx_ep,
				void *context)
{
	return gnix_rx_context(ep, index, attr, rx_ep, context);
}

static inline ssize_t fi_rx_size_left(struct fid_ep *ep)
{
	return gnix_ep_rx_size_left(ep);
}

static inline ssize_t fi_tx_size_left(struct fid_ep *ep)
{
	return gnix_ep_tx_size_left(ep);
}

static inline int fi_stx_context(struct fid_domain *domain,
				 struct fi_tx_attr *attr, struct fid_stx **stx,
				 void *context)
{
	return gnix_stx_open(domain, attr, stx, context);
}

static inline int fi_srx_context(struct fid_domain *domain,
				 struct fi_rx_attr *attr, struct fid_ep **rx_ep,
				 void *context)
{
	return gnix_srx_context(domain, attr, rx_ep, context);
}

static inline ssize_t fi_recv(struct fid_ep *ep, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, void *context)
{
	return gnix_ep_recv(ep, buf, len, desc, src_addr, context);
}

static inline ssize_t fi_recvv(struct fid_ep *ep, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t src_addr,
			       void *context)
{
	return gnix_ep_recvv(ep, iov, desc, count, src_addr, context);
}

static inline ssize_t fi_recvmsg(struct fid_ep *ep, const struct fi_msg *msg,
				 uint64_t flags)
{
	return gnix_ep_recvmsg(ep, msg, flags);
}

static inline ssize_t fi_send(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, fi_addr_t dest_addr, void *context)
{
	return gnix_ep_send(ep, buf, len, desc, dest_addr, context);
}

static inline ssize_t fi_sendv(struct fid_ep *ep, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t dest_addr,
			       void *context)
{
	return gnix_ep_sendv(ep, iov, desc, count, dest_addr, context);
}

static inline ssize_t fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
				 uint64_t flags)
{
	return gnix_ep_sendmsg(ep, msg, flags);
}

static inline ssize_t fi_inject(struct fid_ep *ep, const void *buf, size_t len,
				fi_addr_t dest_addr)
{
	return gnix_ep_msg_inject(ep, buf, len, dest_addr);
}

static inline ssize_t fi_senddata(struct fid_ep *ep, const void *buf,
				  size_t len, void *desc, uint64_t data,
				  fi_addr_t dest_addr, void *context)
{
	return gnix_ep_senddata(ep, buf, len, desc, data, dest_addr, context);
}

static inline ssize_t fi_injectdata(struct fid_ep *ep, const void *buf,
				    size_t len, uint64_t data,
				    fi_addr_t dest_addr)
{
	return gnix_ep_msg_injectdata(ep, buf, len, data, dest_addr);
}

#endif /* _FI_DIRECT_ENDPOINT_H_ */
