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
#ifndef _FI_PROV_OPX_DIRECT_DOMAIN_H_
#define _FI_PROV_OPX_DIRECT_DOMAIN_H_

#define FABRIC_DIRECT_DOMAIN 1

#ifdef FABRIC_DIRECT

#ifdef __cplusplus
extern "C" {
#endif

static inline int
fi_domain(struct fid_fabric *fabric, struct fi_info *info,
	   struct fid_domain **domain, void *context)
{
	return fabric->ops->domain(fabric, info, domain, context);
}

static inline int
fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	   struct fid_cq **cq, void *context)
{
	return domain->ops->cq_open(domain, attr, cq, context);
}

static inline int
fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
	      struct fid_cntr **cntr, void *context)
{
	return domain->ops->cntr_open(domain, attr, cntr, context);
}

static inline int
fi_mr_reg(struct fid_domain *domain, const void *buf, size_t len,
	  uint64_t access, uint64_t offset, uint64_t requested_key,
	  uint64_t flags, struct fid_mr **mr, void *context)
{
	return domain->mr->reg(&domain->fid, buf, len, access, offset,
			       requested_key, flags, mr, context);
}

static inline
int fi_mr_bind(struct fid_mr *mr, struct fid *bfid, uint64_t flags)
{
	return mr->fid.ops->bind(&mr->fid, bfid, flags);
}

static inline
void *fi_mr_desc(struct fid_mr *mr)
{
	return mr->mem_desc;
}

static inline
uint64_t fi_mr_key(struct fid_mr *mr)
{
	return mr->key;
}

static inline int
fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
	   struct fid_av **av, void *context)
{
	return domain->ops->av_open(domain, attr, av, context);
}

static inline int
fi_av_insert(struct fid_av *av, const void *addr, size_t count,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insert(av, addr, count, fi_addr, flags, context);
}

static inline int
fi_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsvc(av, node, service, fi_addr, flags, context);
}

static inline int
fi_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsym(av, node, nodecnt, service, svccnt, fi_addr, flags, context);
}

static inline int
fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
	return av->ops->lookup(av, fi_addr, addr, addrlen);
}

static inline int
fi_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return av->ops->remove(av, fi_addr, count, flags);
}

static inline fi_addr_t
fi_rx_addr(fi_addr_t fi_addr, int rx_index, int rx_ctx_bits)
{
//	assert(rx_ctx_bits <= 8);
	return (fi_addr_t) (((uint64_t) rx_index << (64 - rx_ctx_bits)) | fi_addr);
}

static inline int fi_wait_open(struct fid_fabric *fabric,
			       struct fi_wait_attr *attr,
			       struct fid_wait **waitset)
{
	return -FI_ENOSYS;		/* TODO - implement this */
}


#ifdef __cplusplus
}
#endif

#endif /* FABRIC_DIRECT */

#endif /* _FI_PROV_OPX_DIRECT_DOMAIN_H_ */
