/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
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

#ifndef _FI_DIRECT_DOMAIN_H_
#define _FI_DIRECT_DOMAIN_H_

#define FABRIC_DIRECT_DOMAIN 1

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern int gnix_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			    struct fid_domain **dom, void *context);

extern int gnix_domain_bind(struct fid_domain *domain, struct fid *fid,
			    uint64_t flags);

extern int gnix_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			struct fid_cq **cq, void *context);

extern int gnix_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
			  struct fid_cntr **cntr, void *context);

extern int gnix_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
			  struct fid_wait **waitset);

extern int gnix_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
			  struct fid_poll **pollset);

extern int gnix_mr_reg(struct fid *fid, const void *buf, size_t len,
		       uint64_t access, uint64_t offset, uint64_t requested_key,
		       uint64_t flags, struct fid_mr **mr_o, void *context);

extern int gnix_mr_bind(fid_t fid, struct fid *bfid, uint64_t flags);

extern int gnix_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
			struct fid_av **av, void *context);

extern int gnix_av_bind(struct fid_av *av, struct fid *fid, uint64_t flags);

extern int gnix_av_insert(struct fid_av *av, const void *addr, size_t count,
			  fi_addr_t *fi_addr, uint64_t flags, void *context);

extern int gnix_av_insertsvc(struct fid_av *av, const char *node,
			     const char *service, fi_addr_t *fi_addr,
			     uint64_t flags, void *context);

extern int gnix_av_insertsym(struct fid_av *av, const char *node,
			     size_t nodecnt, const char *service, size_t svccnt,
			     fi_addr_t *fi_addr, uint64_t flags, void *context);

extern int gnix_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			  uint64_t flags);

extern int gnix_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			  size_t *addrlen);

extern const char *gnix_av_straddr(struct fid_av *av, const void *addr,
				   char *buf, size_t *len);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline int fi_domain(struct fid_fabric *fabric, struct fi_info *info,
			    struct fid_domain **domain, void *context)
{
	return gnix_domain_open(fabric, info, domain, context);
}

static inline int fi_domain_bind(struct fid_domain *domain, struct fid *fid,
				 uint64_t flags)
{
	return gnix_domain_bind(domain, fid, flags);
}

static inline int fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			     struct fid_cq **cq, void *context)
{
	return gnix_cq_open(domain, attr, cq, context);
}

static inline int fi_cntr_open(struct fid_domain *domain,
			       struct fi_cntr_attr *attr,
			       struct fid_cntr **cntr, void *context)
{
	return gnix_cntr_open(domain, attr, cntr, context);
}

static inline int fi_wait_open(struct fid_fabric *fabric,
			       struct fi_wait_attr *attr,
			       struct fid_wait **waitset)
{
	return gnix_wait_open(fabric, attr, waitset);
}

static inline int fi_poll_open(struct fid_domain *domain,
			       struct fi_poll_attr *attr,
			       struct fid_poll **pollset)
{
	return gnix_poll_open(domain, attr, pollset);
}

static inline int fi_mr_reg(struct fid_domain *domain, const void *buf,
			    size_t len, uint64_t access, uint64_t offset,
			    uint64_t requested_key, uint64_t flags,
			    struct fid_mr **mr, void *context)
{
	return gnix_mr_reg(&domain->fid, buf, len, access, offset,
			   requested_key, flags, mr, context);
}

static inline void *fi_mr_desc(struct fid_mr *mr)
{
	return mr->mem_desc;
}

static inline uint64_t fi_mr_key(struct fid_mr *mr)
{
	return mr->key;
}

static inline int fi_mr_bind(struct fid_mr *mr, struct fid *bfid,
			     uint64_t flags)
{
	return gnix_mr_bind(&mr->fid, bfid, flags);
}

static inline int fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
			     struct fid_av **av, void *context)
{
	return gnix_av_open(domain, attr, av, context);
}

static inline int fi_av_bind(struct fid_av *av, struct fid *fid, uint64_t flags)
{
	return gnix_av_bind(av, fid, flags);
}

static inline int fi_av_insert(struct fid_av *av, const void *addr,
			       size_t count, fi_addr_t *fi_addr, uint64_t flags,
			       void *context)
{
	return gnix_av_insert(av, addr, count, fi_addr, flags, context);
}

static inline int fi_av_insertsvc(struct fid_av *av, const char *node,
				  const char *service, fi_addr_t *fi_addr,
				  uint64_t flags, void *context)
{
	return gnix_av_insertsvc(av, node, service, fi_addr, flags, context);
}

static inline int fi_av_insertsym(struct fid_av *av, const char *node,
				  size_t nodecnt, const char *service,
				  size_t svccnt, fi_addr_t *fi_addr,
				  uint64_t flags, void *context)
{
	return gnix_av_insertsym(av, node, nodecnt, service, svccnt, fi_addr,
				 flags, context);
}

static inline int fi_av_remove(struct fid_av *av, fi_addr_t *fi_addr,
			       size_t count, uint64_t flags)
{
	return gnix_av_remove(av, fi_addr, count, flags);
}

static inline int fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			       size_t *addrlen)
{
	return gnix_av_lookup(av, fi_addr, addr, addrlen);
}

static inline const char *fi_av_straddr(struct fid_av *av, const void *addr,
					char *buf, size_t *len)
{
	return gnix_av_straddr(av, addr, buf, len);
}

static inline fi_addr_t fi_rx_addr(fi_addr_t fi_addr, int rx_index,
				   int rx_ctx_bits)
{
	return (fi_addr_t)(((uint64_t)rx_index << (64 - rx_ctx_bits)) |
			   fi_addr);
}

#endif /* _FI_DIRECT_DOMAIN_H_ */
