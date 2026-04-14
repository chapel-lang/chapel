/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

#define KLPP_IOCTL(fd, cmd, klppioc) ({										\
	int __status;												\
	if ((__status = ioctl((fd), (cmd), (klppioc))) != 0) {							\
		__status = errno;										\
		FI_DBG(&lpp_prov, FI_LOG_DOMAIN, #cmd " ioctl returned %d\n", __status);			\
		__status = lpp_translate_errno(__status);							\
	}													\
	-__status;												\
})

int klpp_open(int dev_index)
{
	int		fd = -1;
	char		device_file[32];

	if (dev_index >= 0) {
		sprintf(device_file, "/dev/klpp%d", dev_index);
	} else {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "dev_index %d is invalid\n", dev_index);
		return -FI_ENODATA;
	}

	if (fd = open(device_file, O_RDWR), fd < 0) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to open %s, (errno %d)\n", device_file, errno);
		return -FI_ENODATA;
	}

	return fd;
}

void klpp_close(int fd)
{
	close(fd);
}

void *klpp_mmap(struct lpp_domain *lpp_domainp, size_t num_bytes, int prot)
{
	void *address;

	address = mmap(NULL, num_bytes, prot, MAP_SHARED, lpp_domainp->fd, 0);
	if (address == MAP_FAILED) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "mmap failed (errno %d)\n", errno);
		return NULL;
	}

	return address;
}

int klpp_munmap(void *address, size_t num_bytes)
{
	return munmap(address, num_bytes);
}

int klpp_getdevice(int klpp_fd, struct klppioc_lf *klpp_devinfo)
{
	return KLPP_IOCTL(klpp_fd, KLPPIOC_GETDEVICE, klpp_devinfo);
}

int klpp_av_resolve(struct lpp_av *lpp_avp, uint32_t host_index, uint16_t port,
		    struct lpp_fi_addr *lpp_fi_addr)
{
	uint64_t		status = -FI_EINVAL;
	struct klppioc_av	klppioc = { 0 };

	klppioc.host_index = host_index;
	klppioc.port = port;
	status = KLPP_IOCTL(lpp_avp->domain->fd, KLPPIOC_AV_RESOLVE, &klppioc);
	if (status == 0)
		*lpp_fi_addr = klppioc.lpp_fi_addr;

	return status;
}

int klpp_av_lookup(struct lpp_av *lpp_avp, struct lpp_fi_addr lpp_fi_addr,
		   uint32_t *host_index, uint16_t *port)
{
	int status = -FI_EINVAL;
	struct klppioc_av klppioc = { 0 };

	klppioc.lpp_fi_addr = lpp_fi_addr;
	status = KLPP_IOCTL(lpp_avp->domain->fd, KLPPIOC_AV_LOOKUP, &klppioc);
	if (status == 0) {
		*host_index = klppioc.host_index;
		*port = klppioc.port;
	}
	return status;
}

int klpp_mr_register(struct lpp_mr *lpp_mrp, uint64_t flags)
{
	int			status = -FI_ENODATA;
	struct klppioc_mr	klppioc = { 0 };

	klppioc.address = lpp_mrp->attr.mr_iov->iov_base;
	klppioc.length = lpp_mrp->attr.mr_iov->iov_len;

	/* We currently don't support FI_MR_SCALABLE, KLPP will ignore this param */
	klppioc.key = lpp_mrp->attr.requested_key;

	klppioc.flags = flags;
	if (!(lpp_mrp->attr.access & (FI_RECV | FI_READ | FI_REMOTE_WRITE)))
		klppioc.flags |= KLPPIOCF_MR_RO;

	status = KLPP_IOCTL(lpp_mrp->domain->fd, KLPPIOC_MR_REGISTER, &klppioc);
	if (status == 0) {
		lpp_mrp->mr_fid.key = klppioc.key;
		lpp_mrp->id = klppioc.id;
	}

	return status;
}

int klpp_mr_close(struct lpp_mr *lpp_mrp)
{
	int status = -FI_ENODATA;
	struct klppioc_mr klppioc = { 0 };

	klppioc.id = lpp_mrp->id;
	status = KLPP_IOCTL(lpp_mrp->domain->fd, KLPPIOC_MR_CLOSE, &klppioc);

	return status;
}

int klpp_cntr_create(struct lpp_cntr *lpp_cntrp)
{
	int			status = -FI_ENODATA;
	struct klppioc_cntr	klppioc = { 0 };

	klppioc.addr = (uintptr_t)lpp_cntrp->counter;
	status = KLPP_IOCTL(lpp_cntrp->domain->fd, KLPPIOC_CNTR_CREATE, &klppioc);
	if (status == 0)
		lpp_cntrp->id = klppioc.id;

	return status;
}

int klpp_cntr_close(struct lpp_cntr *lpp_cntrp)
{
	int			status = -FI_ENODATA;
	struct klppioc_cntr	klppioc = { 0 };

	klppioc.id = lpp_cntrp->id;
	status = KLPP_IOCTL(lpp_cntrp->domain->fd, KLPPIOC_CNTR_CLOSE, &klppioc);

	return status;
}

int klpp_cq_create(struct lpp_cq *lpp_cqp)
{
	int			status = -FI_ENODATA;
	struct klppioc_cq	klppioc = { 0 };

	klppioc.addr    = (uintptr_t)lpp_cqp->kernel_shared_base;
	klppioc.entries = lpp_cqp->num_entries;
	status = KLPP_IOCTL(lpp_cqp->domain->fd, KLPPIOC_CQ_CREATE, &klppioc);
	if (status == 0)
		lpp_cqp->id = klppioc.id;

	return status;
}

int klpp_cq_close(struct lpp_cq *lpp_cqp)
{
	int			status = -FI_ENODATA;
	struct klppioc_cq	klppioc = { 0 };

	klppioc.id = lpp_cqp->id;
	status = KLPP_IOCTL(lpp_cqp->domain->fd, KLPPIOC_CQ_CLOSE, &klppioc);

	return status;
}

int klpp_ctx_create(struct lpp_ctx *lpp_ctxp)
{
	int			status;
	struct klppioc_ctx	klppioc = { 0 };

	// Setup the shared data stucts.
	klppioc.id		= -1;
	klppioc.entries		= lpp_ctxp->num_entries;
	klppioc.ctx_class	= lpp_ctxp->type;

	status = KLPP_IOCTL(lpp_ctxp->domain->fd, KLPPIOC_CTX_CREATE, &klppioc);
	if (status == 0)
		lpp_ctxp->id = klppioc.id;

	return 0;
}

int klpp_ctx_close(struct lpp_ctx *lpp_ctxp)
{
	int			status = -FI_ENODATA;
	struct klppioc_ctx	klppioc = { 0 };

	klppioc.id = lpp_ctxp->id;
	status = KLPP_IOCTL(lpp_ctxp->domain->fd, KLPPIOC_CTX_CLOSE, &klppioc);
	if (status == 0)
		lpp_ctxp->id = -1;

	return status;
}

#define	PTR_TO_ID(X)	((X) == NULL) ? -1 : (X)->id

int klpp_ep_create(struct lpp_ep *lpp_epp)
{
	int			status = -FI_ENODATA;
	struct klppioc_ep	klppioc = { 0 };

	klppioc.ep_id = -1;
	status = KLPP_IOCTL(lpp_epp->domain->fd, KLPPIOC_EP_CREATE, &klppioc);
	if (status == 0)
		lpp_epp->id = klppioc.ep_id;

	return status;
}

int klpp_ep_close(struct lpp_ep *lpp_epp)
{
	int			status = -FI_ENODATA;
	struct klppioc_ep	klppioc = { 0 };

	klppioc.ep_id = lpp_epp->id;
	status = KLPP_IOCTL(lpp_epp->domain->fd, KLPPIOC_EP_CLOSE, &klppioc);

	return status;
}

int klpp_ep_enable(struct lpp_ep *lpp_epp)
{
	int			status = -FI_ENODATA;
	struct klppioc_ep	klppioc = { 0 };

	klppioc.ep_id = lpp_epp->id;

	klppioc.rx_id  = PTR_TO_ID(lpp_epp->rx);
	klppioc.stx_id = PTR_TO_ID(lpp_epp->stx);

	klppioc.cq_transmit_id = PTR_TO_ID(lpp_epp->cq_transmit);
	klppioc.cq_recv_id = PTR_TO_ID(lpp_epp->cq_recv);

	klppioc.cntr_send_id		= PTR_TO_ID(lpp_epp->cntr_send);
	klppioc.cntr_recv_id		= PTR_TO_ID(lpp_epp->cntr_recv);
	klppioc.cntr_read_id		= PTR_TO_ID(lpp_epp->cntr_read);
	klppioc.cntr_write_id		= PTR_TO_ID(lpp_epp->cntr_write);
	klppioc.cntr_remote_read_id	= PTR_TO_ID(lpp_epp->cntr_remote_read);
	klppioc.cntr_remote_write_id	= PTR_TO_ID(lpp_epp->cntr_remote_write);

	klppioc.io_stat_uaddr = (uint64_t)lpp_epp->io_stat;

	status = KLPP_IOCTL(lpp_epp->domain->fd, KLPPIOC_EP_ENABLE, &klppioc);

	return status;
}

int klpp_ep_disable(struct lpp_ep *lpp_epp)
{
	int			status = -FI_ENODATA;
	struct klppioc_ep	klppioc = { 0 };

	klppioc.ep_id = lpp_epp->id;

	klppioc.rx_id  = PTR_TO_ID(lpp_epp->rx);
	klppioc.stx_id = PTR_TO_ID(lpp_epp->stx);

	klppioc.cq_transmit_id = PTR_TO_ID(lpp_epp->cq_transmit);
	klppioc.cq_recv_id = PTR_TO_ID(lpp_epp->cq_recv);

	klppioc.cntr_send_id		= PTR_TO_ID(lpp_epp->cntr_send);
	klppioc.cntr_recv_id		= PTR_TO_ID(lpp_epp->cntr_recv);
	klppioc.cntr_read_id		= PTR_TO_ID(lpp_epp->cntr_read);
	klppioc.cntr_write_id		= PTR_TO_ID(lpp_epp->cntr_write);
	klppioc.cntr_remote_read_id	= PTR_TO_ID(lpp_epp->cntr_remote_read);
	klppioc.cntr_remote_write_id	= PTR_TO_ID(lpp_epp->cntr_remote_write);

	status = KLPP_IOCTL(lpp_epp->domain->fd, KLPPIOC_EP_DISABLE, &klppioc);

	return status;
}

int klpp_rmr_label(struct lpp_domain *lpp_domainp)
{
	struct klppioc_rmr klppioc;
	int status;
	void *addrlist[] = {
			lpp_domainp->rmr_meta_arr,
			lpp_domainp->rmr_rw,
			lpp_domainp->rmr_ro,
			lpp_domainp->rmr_srq
	};
	int classlist[] = { KLPP_RMR_META, KLPP_RMR_RW, KLPP_RMR_RO, KLPP_RMR_SRQ };

	for (int i = 0; i < sizeof(addrlist) / sizeof(addrlist[0]); i++) {
		if (addrlist[i] == NULL)
			continue;
		klppioc.addr = (uintptr_t)addrlist[i];
		klppioc.region_class = classlist[i];

		status = KLPP_IOCTL(lpp_domainp->fd, KLPPIOC_RMR_LABEL, &klppioc);
		if (status != 0)
			return status;
	}

	return 0;
}

int klpp_progress(struct lpp_domain *lpp_domainp, int umc_id)
{
	struct klpp_ioc_prog ioc_prog;

	ioc_prog.umc_id = umc_id;
	return KLPP_IOCTL(lpp_domainp->fd, KLPPIOC_PROGRESS, &ioc_prog);
}

int klpp_readwrite(struct lpp_ep *lpp_epp, fi_addr_t addr, uint64_t length,
		   uint64_t flags, void *context, void *local_uaddr,
		   uint64_t remote_uaddr, uint64_t remote_key)
{
	union lpp_fi_addr_storage addrstor;
	struct klpp_ioc_readwrite ioc_readwrite;

	addrstor.raw = addr;

	ioc_readwrite.ep_id = lpp_epp->id;
	ioc_readwrite.fi_addr = addrstor.fi_addr;
	ioc_readwrite.local_uaddr = (uint64_t)local_uaddr;
	ioc_readwrite.remote_uaddr = remote_uaddr;
	ioc_readwrite.remote_key = remote_key;
	ioc_readwrite.length = length;
	ioc_readwrite.flags = flags;
	ioc_readwrite.context = (uint64_t)context;

	return KLPP_IOCTL(lpp_epp->domain->fd, KLPPIOC_READWRITE, &ioc_readwrite);
}

int klpp_umc_acquire(struct lpp_domain *lpp_domain, klpp_id_t *umc_id,
		     uint8_t *src_id, uint64_t *generation, void *rx_base,
		     void *tx_base, void *k2u_base, void *u2k_base,
		     struct lpp_fi_addr ep_addr, uint16_t *port)
{
	struct klpp_ioc_umc ioc;
	int ret;

	ioc.rx_base_uaddr = (uint64_t)rx_base;
	ioc.tx_base_uaddr = (uint64_t)tx_base;
	ioc.k2u_base_uaddr = (uint64_t)k2u_base;
	ioc.u2k_base_uaddr = (uint64_t)u2k_base;
	ioc.ep_addr = ep_addr;
	ioc.port = *port;

	ret = KLPP_IOCTL(lpp_domain->fd, KLPPIOC_UMC_ACQUIRE, &ioc);
	if (ret == 0) {
		*umc_id = ioc.umc_id;
		*src_id = ioc.src_id;
		*generation = ioc.umc_gen;
		*port = ioc.port;
	}
	return ret;
}

void klpp_umc_release(struct lpp_domain *lpp_domain, klpp_id_t umc_id)
{
	struct klpp_ioc_umc ioc;

	ioc.umc_id = umc_id;
	KLPP_IOCTL(lpp_domain->fd, KLPPIOC_UMC_RELEASE, &ioc);
}

int klpp_kmc_send_one(struct lpp_ep *lpp_epp, struct lpp_fi_addr dst_addr,
		      struct klpp_msg_hdr *hdr, const struct iovec *iov,
		      size_t iov_count)
{
	struct lpp_domain *lpp_domain = lpp_epp->domain;
	struct klpp_ioc_kmc_send ioc;
	int ret;

	hdr->src_ep = lpp_epp->addr;

	ioc.dst_addr = dst_addr;
	ioc.msg_hdr = *hdr;
	ioc.iov_ptr = (__u64)iov;
	ioc.iov_cnt = (__u64)iov_count;
	ioc.kmc_flags = lpp_disable_dqp ? KLPPIOCF_KMC_NODQP : 0;
	ioc.next_ptr = 0;
	ioc.errval = -FI_ENODATA;

	ret = KLPP_IOCTL(lpp_domain->fd, KLPPIOC_KMC_SEND, &ioc);
	if (ret == 0) {
		ret = -lpp_translate_errno(ioc.errval);
		if (ret && ret != -FI_EAGAIN) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN,
				"KLPPIOC_KMC_SEND returned %d in errval\n", ret);
		}
	}
	return ret;
}

int klpp_kmc_send_list(struct lpp_ep *lpp_epp, struct klpp_ioc_kmc_send *head)
{
	struct lpp_domain *lpp_domain = lpp_epp->domain;

	return KLPP_IOCTL(lpp_domain->fd, KLPPIOC_KMC_SEND, head);
}
