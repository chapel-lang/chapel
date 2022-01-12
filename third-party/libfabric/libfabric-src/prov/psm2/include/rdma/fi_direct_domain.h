/*
 * Copyright (c) 2016-2018 Intel Corporation, Inc.  All rights reserved.
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

#ifndef FI_DIRECT_DOMAIN_H
#define FI_DIRECT_DOMAIN_H

#define FABRIC_DIRECT_DOMAIN

int psmx2_domain_open(
		struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context);

int psmx2_cq_open(
		struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq, void *context);

int psmx2_cntr_open(
		struct fid_domain *domain, struct fi_cntr_attr *attr,
		struct fid_cntr **cntr, void *context);

int psmx2_wait_open(
		struct fid_fabric *fabric, struct fi_wait_attr *attr,
		struct fid_wait **waitset);

int psmx2_mr_reg(
		struct fid *fid, const void *buf, size_t len,
		uint64_t access, uint64_t offset, uint64_t requested_key,
		uint64_t flags, struct fid_mr **mr, void *context);

int psmx2_mr_regv(
		struct fid *fid, const struct iovec *iov,
		size_t count, uint64_t access,
		uint64_t offset, uint64_t requested_key,
		uint64_t flags, struct fid_mr **mr, void *context);

int psmx2_mr_regattr(
		struct fid *fid, const struct fi_mr_attr *attr,
		uint64_t flags, struct fid_mr **mr);

int psmx2_mr_bind(struct fid *fid, struct fid *bfid, uint64_t flags);

int psmx2_av_open(
		struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

int psmx2_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags);

int psmx2_av_insert(
		struct fid_av *av, const void *addr, size_t count,
		fi_addr_t *fi_addr, uint64_t flags, void *context);

int psmx2_av_remove(
		struct fid_av *av, fi_addr_t *fi_addr, size_t count,
		uint64_t flags);

int psmx2_av_lookup(
		struct fid_av *av, fi_addr_t fi_addr, void *addr,
		size_t *addrlen);

const char *psmx2_av_straddr(
		struct fid_av *av, const void *addr, char *buf, size_t *len);


static inline int
fi_domain(struct fid_fabric *fabric, struct fi_info *info,
	   struct fid_domain **domain, void *context)
{
	return psmx2_domain_open(fabric, info, domain, context);
}

static inline int
fi_domain_bind(struct fid_domain *domain, struct fid *fid, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int
fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	   struct fid_cq **cq, void *context)
{
	return psmx2_cq_open(domain, attr, cq, context);
}

static inline int
fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
	      struct fid_cntr **cntr, void *context)
{
	return psmx2_cntr_open(domain, attr, cntr, context);
}

static inline int
fi_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
	     struct fid_wait **waitset)
{
	return psmx2_wait_open(fabric, attr, waitset);
}

static inline int
fi_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
	     struct fid_poll **pollset)
{
	return domain->ops->poll_open(domain, attr, pollset);
}

static inline int
fi_mr_reg(struct fid_domain *domain, const void *buf, size_t len,
	  uint64_t access, uint64_t offset, uint64_t requested_key,
	  uint64_t flags, struct fid_mr **mr, void *context)
{
	return psmx2_mr_reg(&domain->fid, buf, len, access, offset,
			       requested_key, flags, mr, context);
}

static inline int
fi_mr_regv(struct fid_domain *domain, const struct iovec *iov,
			size_t count, uint64_t access,
			uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context)
{
	return psmx2_mr_regv(&domain->fid, iov, count, access,
			offset, requested_key, flags, mr, context);
}

static inline int
fi_mr_regattr(struct fid_domain *domain, const struct fi_mr_attr *attr,
			uint64_t flags, struct fid_mr **mr)
{
	return psmx2_mr_regattr(&domain->fid, attr, flags, mr);
}

static inline void *fi_mr_desc(struct fid_mr *mr)
{
	return mr->mem_desc;
}

static inline uint64_t fi_mr_key(struct fid_mr *mr)
{
	return mr->key;
}

static inline int
fi_mr_raw_attr(struct fid_mr *mr, uint64_t *base_addr,
	       uint8_t *raw_key, size_t *key_size, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int
fi_mr_map_raw(struct fid_domain *domain, uint64_t base_addr,
	      uint8_t *raw_key, size_t key_size, uint64_t *key, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int
fi_mr_unmap_key(struct fid_domain *domain, uint64_t key)
{
	return -FI_ENOSYS;
}

static inline int fi_mr_bind(struct fid_mr *mr, struct fid *bfid, uint64_t flags)
{
	return psmx2_mr_bind(&mr->fid, bfid, flags);
}

static inline int
fi_mr_refresh(struct fid_mr *mr, const struct iovec *iov, size_t count,
	      uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int fi_mr_enable(struct fid_mr *mr)
{
	return -FI_ENOSYS;
}

static inline int
fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
	   struct fid_av **av, void *context)
{
	return psmx2_av_open(domain, attr, av, context);
}

static inline int
fi_av_bind(struct fid_av *av, struct fid *fid, uint64_t flags)
{
	return psmx2_av_bind(&av->fid, fid, flags);
}

static inline int
fi_av_insert(struct fid_av *av, const void *addr, size_t count,
	     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return psmx2_av_insert(av, addr, count, fi_addr, flags, context);
}

static inline int
fi_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static inline int
fi_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static inline int
fi_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return psmx2_av_remove(av, fi_addr, count, flags);
}

static inline int
fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
        return psmx2_av_lookup(av, fi_addr, addr, addrlen);
}

static inline const char *
fi_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	return psmx2_av_straddr(av, addr, buf, len);
}

static inline fi_addr_t
fi_rx_addr(fi_addr_t fi_addr, int rx_index, int rx_ctx_bits)
{
	return (fi_addr_t) (((uint64_t) rx_index << (64 - rx_ctx_bits)) | fi_addr);
}

#endif /* FI_DIRECT_DOMAIN_H */
