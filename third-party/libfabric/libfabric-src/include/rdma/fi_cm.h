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

#ifndef FI_CM_H
#define FI_CM_H

#include <rdma/fi_endpoint.h>


#ifdef __cplusplus
extern "C" {
#endif


struct fid_mc {
	struct fid		fid;
	fi_addr_t		fi_addr;
};

struct fi_ops_cm {
	size_t	size;
	int	(*setname)(fid_t fid, void *addr, size_t addrlen);
	int	(*getname)(fid_t fid, void *addr, size_t *addrlen);
	int	(*getpeer)(struct fid_ep *ep, void *addr, size_t *addrlen);
	int	(*connect)(struct fid_ep *ep, const void *addr,
			const void *param, size_t paramlen);
	int	(*listen)(struct fid_pep *pep);
	int	(*accept)(struct fid_ep *ep, const void *param, size_t paramlen);
	int	(*reject)(struct fid_pep *pep, fid_t handle,
			const void *param, size_t paramlen);
	int	(*shutdown)(struct fid_ep *ep, uint64_t flags);
	int	(*join)(struct fid_ep *ep, const void *addr, uint64_t flags,
			struct fid_mc **mc, void *context);
};


#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_cm.h>
#endif	/* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_CM

static inline int fi_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->cm->setname(fid, addr, addrlen);
}

static inline int fi_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	return ep->cm->getname(fid, addr, addrlen);
}

static inline int fi_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	return ep->cm->getpeer(ep, addr, addrlen);
}

static inline int fi_listen(struct fid_pep *pep)
{
	return pep->cm->listen(pep);
}

static inline int
fi_connect(struct fid_ep *ep, const void *addr,
	   const void *param, size_t paramlen)
{
	return ep->cm->connect(ep, addr, param, paramlen);
}

static inline int
fi_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	return ep->cm->accept(ep, param, paramlen);
}

static inline int
fi_reject(struct fid_pep *pep, fid_t handle,
	  const void *param, size_t paramlen)
{
	return pep->cm->reject(pep, handle, param, paramlen);
}

static inline int fi_shutdown(struct fid_ep *ep, uint64_t flags)
{
	return ep->cm->shutdown(ep, flags);
}

static inline int fi_join(struct fid_ep *ep, const void *addr, uint64_t flags,
			  struct fid_mc **mc, void *context)
{
	return FI_CHECK_OP(ep->cm, struct fi_ops_cm, join) ?
		ep->cm->join(ep, addr, flags, mc, context) : -FI_ENOSYS;
}

static inline fi_addr_t fi_mc_addr(struct fid_mc *mc)
{
	return mc->fi_addr;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* FI_CM_H */
