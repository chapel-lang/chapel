/*
 * Copyright (c) 2016, 2022 Intel Corporation, Inc.  All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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
#include <unistd.h>

#include <ofi_util.h>
#include "rxm.h"


static uint64_t rxm_passthru_cntr_read(struct fid_cntr *cntr_fid)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	cntr->util_cntr.progress(&cntr->util_cntr);
	return fi_cntr_read(cntr->msg_cntr);
}

static uint64_t rxm_passthru_cntr_readerr(struct fid_cntr *cntr_fid)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	cntr->util_cntr.progress(&cntr->util_cntr);
	return fi_cntr_readerr(cntr->msg_cntr);
}

static int rxm_passthru_cntr_add(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	return fi_cntr_add(cntr->msg_cntr, value);
}

static int rxm_passthru_cntr_set(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	return fi_cntr_set(cntr->msg_cntr, value);
}


static int rxm_passthru_cntr_wait(struct fid_cntr *cntr_fid,
				  uint64_t threshold, int timeout)
{
	struct rxm_cntr *cntr;
	uint64_t endtime, errcnt;
	int ret, timeout_quantum;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	errcnt = fi_cntr_readerr(cntr->msg_cntr);
	endtime = ofi_timeout_time(timeout);

	do {
		cntr->util_cntr.progress(&cntr->util_cntr);

		if (errcnt != fi_cntr_readerr(cntr->msg_cntr))
			return -FI_EAVAIL;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		/* We need to ensure that CM events are progressed, so return
		 * periodically to force progress on the EQ.
		 */
		timeout_quantum = (timeout < 0 ? OFI_TIMEOUT_QUANTUM_MS :
				   MIN(OFI_TIMEOUT_QUANTUM_MS, timeout));

		ret = fi_cntr_wait(cntr->msg_cntr, threshold, timeout_quantum);
	} while (ret == -FI_ETIMEDOUT &&
	         (timeout < 0 || timeout_quantum < timeout));

	return ret;
}

static int rxm_passthru_cntr_adderr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	return fi_cntr_adderr(cntr->msg_cntr, value);
}

static int rxm_passthru_cntr_seterr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct rxm_cntr *cntr;

	cntr = container_of(cntr_fid, struct rxm_cntr, util_cntr.cntr_fid);
	return fi_cntr_seterr(cntr->msg_cntr, value);
}

static struct fi_ops_cntr rxm_passthru_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = rxm_passthru_cntr_read,
	.readerr = rxm_passthru_cntr_readerr,
	.add = rxm_passthru_cntr_add,
	.set = rxm_passthru_cntr_set,
	.wait = rxm_passthru_cntr_wait,
	.adderr = rxm_passthru_cntr_adderr,
	.seterr = rxm_passthru_cntr_seterr,
};

static int rxm_passthru_cntr_close(struct fid *fid)
{
	struct rxm_cntr *cntr;
	int ret;

	cntr = container_of(fid, struct rxm_cntr, util_cntr.cntr_fid.fid);

	if (cntr->msg_cntr) {
		ret = fi_close(&cntr->msg_cntr->fid);
		if (ret)
			return ret;
	}

	cntr->msg_cntr = NULL;
	ret = ofi_cntr_cleanup(&cntr->util_cntr);
	if (ret)
		return ret;

	free(cntr);
	return 0;
}

static struct fi_ops rxm_passthru_cntr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_passthru_cntr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops rxm_peer_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = fi_no_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxm_peer_av_query(struct fid_peer_av *av, struct fi_av_attr *attr)
{
	struct rxm_av *rxm_av = container_of(av, struct rxm_av, peer_av);

	memset(attr, 0, sizeof(*attr));

	/* Only count is useful at this moment */
	attr->count = ofi_av_size(&rxm_av->util_av);

	return 0;
}

fi_addr_t rxm_peer_av_ep_addr(struct fid_peer_av *av, struct fid_ep *ep)
{
	struct rxm_av *rxm_av = container_of(av, struct rxm_av, peer_av);
	size_t addrlen;
	char *addr;
	fi_addr_t addr_ret;
	int ret;

	addrlen = 0;
	ret = fi_getname(&ep->fid, NULL, &addrlen);
	if (ret != FI_SUCCESS && addrlen == 0)
		goto err1;

	addr = calloc(1, addrlen);
	if (!addr)
		goto err1;

	ret = fi_getname(&ep->fid, addr, &addrlen);
	if (ret)
		goto err2;

	addr_ret = ofi_av_lookup_fi_addr(&rxm_av->util_av, addr);
	free(addr);
	return addr_ret;

err2:
	free(addr);
err1:
	return FI_ADDR_NOTAVAIL;
}

static struct fi_ops_av_owner rxm_av_owner_ops = {
	.size = sizeof(struct fi_ops_av_owner),
	.query = rxm_peer_av_query,
	.ep_addr = rxm_peer_av_ep_addr,
};

static int
rxm_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
	    struct fid_av **fid_av, void *context)
{
	struct rxm_domain *rxm_domain;
	struct rxm_av *rxm_av;
	struct fid_av *fid_av_new;
	struct fi_peer_av_context peer_context;
	struct fi_av_attr peer_attr = {
		.flags = FI_PEER,
	};
	int ret;

	ret = rxm_util_av_open(domain_fid, attr, &fid_av_new,
			context, sizeof(struct rxm_conn),
			ofi_av_remove_cleanup ? rxm_av_remove_handler : NULL);
	if (ret)
		return ret;

	rxm_av = container_of(fid_av_new, struct rxm_av, util_av.av_fid);
	rxm_domain = container_of(domain_fid, struct rxm_domain,
				  util_domain.domain_fid);

	rxm_av->peer_av.fid.fclass = FI_CLASS_PEER_AV;
        rxm_av->peer_av.fid.ops = &rxm_peer_av_fi_ops;
        rxm_av->peer_av.owner_ops = &rxm_av_owner_ops;
        peer_context.size = sizeof(peer_context);
        peer_context.av = &rxm_av->peer_av;

	if (rxm_domain->util_coll_domain) {
		ret = fi_av_open(rxm_domain->util_coll_domain, &peer_attr,
				 &rxm_av->util_coll_av, &peer_context);
		if (ret)
			goto err1;
	}
	if (rxm_domain->offload_coll_domain) {
		ret = fi_av_open(rxm_domain->offload_coll_domain, &peer_attr,
				 &rxm_av->offload_coll_av, &peer_context);
		if (ret)
			goto err1;
	}
	*fid_av = fid_av_new;
	return 0;

err1:
	fi_close(&fid_av_new->fid);
	return ret;
}

static int
rxm_cntr_open(struct fid_domain *fid_domain, struct fi_cntr_attr *attr,
	      struct fid_cntr **cntr_fid, void *context)
{
	struct rxm_domain *domain;
	struct rxm_cntr *cntr;
	int ret;

	domain = container_of(fid_domain, struct rxm_domain,
			      util_domain.domain_fid.fid);
	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	ret = ofi_cntr_init(&rxm_prov, fid_domain, attr, &cntr->util_cntr,
			    &ofi_cntr_progress, context);
	if (ret)
		goto free;

	if (domain->passthru) {
		ret = fi_cntr_open(domain->msg_domain, attr, &cntr->msg_cntr, cntr);
		if (ret) {
			RXM_WARN_ERR(FI_LOG_CNTR, "fi_cntr_open", ret);
			goto cleanup;
		}

		cntr->util_cntr.cntr_fid.fid.ops = &rxm_passthru_cntr_fi_ops;
		cntr->util_cntr.cntr_fid.ops = &rxm_passthru_cntr_ops;
	}

	*cntr_fid = &cntr->util_cntr.cntr_fid;
	return FI_SUCCESS;

cleanup:
	ofi_cntr_cleanup(&cntr->util_cntr);
free:
	free(cntr);
	return ret;
}

static int rxm_query_collective(struct fid_domain *domain,
				enum fi_collective_op coll,
				struct fi_collective_attr *attr,
				uint64_t flags)
{
	struct rxm_domain *rxm_domain;
	int ret;

	rxm_domain = container_of(domain, struct rxm_domain,
				  util_domain.domain_fid);

	if (!rxm_domain->util_coll_domain)
		return -FI_ENOSYS;

	if (rxm_domain->offload_coll_domain)
		ret = fi_query_collective(rxm_domain->offload_coll_domain,
					  coll, attr, flags);
	else
		ret = -FI_ENOSYS;

	if (ret == FI_SUCCESS || flags & OFI_OFFLOAD_PROV_ONLY)
		return ret;

	return fi_query_collective(rxm_domain->util_coll_domain,
				   coll, attr, flags);
}

static struct fi_ops_domain rxm_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = rxm_av_open,
	.cq_open = rxm_cq_open,
	.endpoint = rxm_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = rxm_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = rxm_ep_query_atomic,
	.query_collective = rxm_query_collective,
};

static void rxm_mr_remove_map_entry(struct rxm_mr *mr)
{
	ofi_genlock_lock(&mr->domain->util_domain.lock);
	(void) ofi_mr_map_remove(&mr->domain->util_domain.mr_map,
				 mr->mr_fid.key);
	ofi_genlock_unlock(&mr->domain->util_domain.lock);
}

static int rxm_mr_add_map_entry(struct util_domain *domain,
				struct fi_mr_attr *msg_attr,
				struct rxm_mr *rxm_mr,
				uint64_t flags)
{
	uint64_t temp_key;
	int ret;

	msg_attr->requested_key = rxm_mr->mr_fid.key;

	ofi_genlock_lock(&domain->lock);
	ret = ofi_mr_map_insert(&domain->mr_map, msg_attr, &temp_key, rxm_mr, flags);
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN,
			"MR map insert for atomic verification failed %d\n",
			ret);
	} else {
		assert(rxm_mr->mr_fid.key == temp_key);
	}
	ofi_genlock_unlock(&domain->lock);

	return ret;
}

struct rxm_mr *rxm_mr_get_map_entry(struct rxm_domain *domain, uint64_t key)
{
	struct rxm_mr *mr;

	ofi_genlock_lock(&domain->util_domain.lock);
	mr = ofi_mr_map_get(&domain->util_domain.mr_map, key);
	ofi_genlock_unlock(&domain->util_domain.lock);

	return mr;
}

static int rxm_domain_close(fid_t fid)
{
	struct rxm_domain *rxm_domain;
	int ret;

	rxm_domain = container_of(fid, struct rxm_domain, util_domain.domain_fid.fid);

	ofi_mutex_destroy(&rxm_domain->amo_bufpool_lock);
	ofi_bufpool_destroy(rxm_domain->amo_bufpool);

	ret = fi_close(&rxm_domain->msg_domain->fid);
	if (ret)
		return ret;

	if (rxm_domain->offload_coll_domain) {
		ret = fi_close(&rxm_domain->offload_coll_domain->fid);
		if (ret)
			return ret;
		rxm_domain->offload_coll_domain = NULL;
	}

	if (rxm_domain->util_coll_domain) {
		ret = fi_close(&rxm_domain->util_coll_domain->fid);
		if (ret)
			return ret;
		rxm_domain->util_coll_domain = NULL;
	}

	ret = ofi_domain_close(&rxm_domain->util_domain);
	if (ret)
		return ret;

	free(rxm_domain);
	return 0;
}

static struct fi_ops rxm_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int rxm_mr_close(fid_t fid)
{
	struct rxm_mr *rxm_mr;
	int ret;

	rxm_mr = container_of(fid, struct rxm_mr, mr_fid.fid);

	if (rxm_mr->domain->util_domain.info_domain_caps & FI_ATOMIC)
		rxm_mr_remove_map_entry(rxm_mr);

	if (rxm_mr->hmem_handle) {
		ofi_hmem_dev_unregister(rxm_mr->iface,
					(uint64_t) rxm_mr->hmem_handle);
	}

	ret = fi_close(&rxm_mr->msg_mr->fid);
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to close MSG MR\n");

	ofi_atomic_dec32(&rxm_mr->domain->util_domain.ref);
	free(rxm_mr);
	return ret;
}

static struct fi_ops rxm_mr_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxm_msg_mr_reg_internal(struct rxm_domain *rxm_domain, const void *buf,
	size_t len, uint64_t acs, uint64_t flags, struct fid_mr **mr)
{
	int ret, tries = 0;
	struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_mr_attr attr = {
		.mr_iov = &iov,
		.iov_count = 1,
		.access = acs,
		.iface = FI_HMEM_SYSTEM,
	};

	if (rxm_detect_hmem_iface)
		attr.iface = ofi_get_hmem_iface(buf, &attr.device.reserved, NULL);

	/* If we can't get a key within 1024 tries, give up */
	do {
		attr.requested_key = rxm_domain->mr_key++ | (1UL << 31);
		ret = fi_mr_regattr(rxm_domain->msg_domain, &attr, flags, mr);
	} while (ret == -FI_ENOKEY && tries++ < 1024);

	return ret;
}

void rxm_msg_mr_closev(struct fid_mr **mr, size_t count)
{
	int ret;
	size_t i;

	for (i = 0; i < count; i++) {
		if (mr[i]) {
			ret = fi_close(&mr[i]->fid);
			if (ret)
				FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
					"Unable to close msg mr: %zu\n", i);
			mr[i] = NULL;
		}
	}
}

int rxm_msg_mr_regv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		    size_t count, size_t reg_limit, uint64_t access,
		    struct fid_mr **mr)
{
	struct rxm_domain *rxm_domain;
	size_t i;
	int ret;

	rxm_domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
				  util_domain);

	for (i = 0; i < count && reg_limit; i++) {
		size_t len = MIN(iov[i].iov_len, reg_limit);
		ret = rxm_msg_mr_reg_internal(rxm_domain, iov[i].iov_base,
					      len, access, 0, &mr[i]);
		if (ret)
			goto err;
		reg_limit -= len;
	}
	return 0;
err:
	rxm_msg_mr_closev(mr, i);
	return ret;
}

/* Large send/recv transfers use RMA rendezvous protocol */
static uint64_t
rxm_mr_get_msg_access(struct rxm_domain *rxm_domain, uint64_t access)
{
	if (access & FI_SEND) {
		access |= rxm_use_write_rndv ? FI_WRITE : FI_REMOTE_READ;
	}

	if (access & FI_RECV) {
		access |= rxm_use_write_rndv ? FI_REMOTE_WRITE : FI_READ;
	}

	return access;
}

static void rxm_mr_init(struct rxm_mr *rxm_mr, struct rxm_domain *domain,
			void *context)
{
	rxm_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	rxm_mr->mr_fid.fid.context = context;
	rxm_mr->mr_fid.fid.ops = &rxm_mr_ops;
	rxm_mr->mr_fid.mem_desc = rxm_mr;
	rxm_mr->mr_fid.key = fi_mr_key(rxm_mr->msg_mr);
	rxm_mr->domain = domain;
	rxm_mr->hmem_flags = 0x0;
	rxm_mr->hmem_handle = NULL;
	ofi_atomic_inc32(&domain->util_domain.ref);
}

static int rxm_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr)
{
	struct rxm_domain *rxm_domain;
	struct fi_mr_attr msg_attr = *attr;
	struct rxm_mr *rxm_mr;
	int ret,gdrerr;

	rxm_domain = container_of(fid, struct rxm_domain,
				  util_domain.domain_fid.fid);

	if (!ofi_hmem_is_initialized(attr->iface)) {
		FI_WARN(&rxm_prov, FI_LOG_MR,
			"Cannot register memory for uninitialized iface\n");
		return -FI_ENOSYS;
	}

	rxm_mr = calloc(1, sizeof(*rxm_mr));
	if (!rxm_mr)
		return -FI_ENOMEM;

	ofi_mr_update_attr(rxm_domain->util_domain.fabric->fabric_fid.api_version,
			   rxm_domain->util_domain.info_domain_caps, attr,
			   &msg_attr, flags);

	if ((flags & FI_HMEM_HOST_ALLOC) && (attr->iface == FI_HMEM_ZE))
		msg_attr.device.ze = -1;

	msg_attr.access = rxm_mr_get_msg_access(rxm_domain, attr->access);

	ret = fi_mr_regattr(rxm_domain->msg_domain, &msg_attr,
			    flags, &rxm_mr->msg_mr);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to register MSG MR\n");
		goto err;
	}
	rxm_mr_init(rxm_mr, rxm_domain, attr->context);
	ofi_mutex_init(&rxm_mr->amo_lock);
	rxm_mr->iface = msg_attr.iface;
	rxm_mr->device = msg_attr.device.reserved;
	*mr = &rxm_mr->mr_fid;

	gdrerr = ofi_hmem_dev_register(rxm_mr->iface, attr->mr_iov->iov_base,
				       attr->mr_iov->iov_len,
				       (uint64_t *) &rxm_mr->hmem_handle);
	if (gdrerr) {
		rxm_mr->hmem_flags = 0x0;
		rxm_mr->hmem_handle = NULL;
	} else {
		rxm_mr->hmem_flags = OFI_HMEM_DATA_DEV_REG_HANDLE;
	}

	if (rxm_domain->util_domain.info_domain_caps & FI_ATOMIC) {
		ret = rxm_mr_add_map_entry(&rxm_domain->util_domain,
					   &msg_attr, rxm_mr, flags);
		if (ret)
			goto map_err;
	}

	return 0;

map_err:
	fi_close(&rxm_mr->mr_fid.fid);
	return ret;
err:
	free(rxm_mr);
	return ret;

}

static int rxm_mr_regv(struct fid *fid, const struct iovec *iov, size_t count,
		       uint64_t access, uint64_t offset, uint64_t requested_key,
		       uint64_t flags, struct fid_mr **mr, void *context)
{
	struct rxm_domain *rxm_domain;
	struct rxm_mr *rxm_mr;
	int ret;
	struct fi_mr_attr msg_attr = {
		.mr_iov = iov,
		.iov_count = count,
		.access = access,
		.offset = offset,
		.requested_key = requested_key,
		.context = context,
	};

	rxm_domain = container_of(fid, struct rxm_domain,
				  util_domain.domain_fid.fid);

	rxm_mr = calloc(1, sizeof(*rxm_mr));
	if (!rxm_mr)
		return -FI_ENOMEM;

	access = rxm_mr_get_msg_access(rxm_domain, access);

	ret = fi_mr_regv(rxm_domain->msg_domain, iov, count, access, offset,
			 requested_key, flags, &rxm_mr->msg_mr, context);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_DOMAIN, "Unable to register MSG MR\n");
		goto err;
	}
	rxm_mr_init(rxm_mr, rxm_domain, context);
	*mr = &rxm_mr->mr_fid;

	if (rxm_domain->util_domain.info_domain_caps & FI_ATOMIC) {
		ret = rxm_mr_add_map_entry(&rxm_domain->util_domain,
					   &msg_attr, rxm_mr, flags);
		if (ret)
			goto map_err;
	}

	return 0;
map_err:
	fi_close(&rxm_mr->mr_fid.fid);
	return ret;
err:
	free(rxm_mr);
	return ret;
}

static int rxm_mr_reg(struct fid *fid, const void *buf, size_t len,
		      uint64_t access, uint64_t offset, uint64_t requested_key,
		      uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return rxm_mr_regv(fid, &iov, 1, access, offset, requested_key,
			   flags, mr, context);
}

static struct fi_ops_mr rxm_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = rxm_mr_reg,
	.regv = rxm_mr_regv,
	.regattr = rxm_mr_regattr,
};

static int
rxm_mr_regattr_thru(struct fid *fid, const struct fi_mr_attr *attr,
		    uint64_t flags, struct fid_mr **mr)
{
	struct rxm_domain *domain;

	domain = container_of(fid, struct rxm_domain,
			      util_domain.domain_fid.fid);
	return fi_mr_regattr(domain->msg_domain, attr, flags, mr);
}

static int
rxm_mr_regv_thru(struct fid *fid, const struct iovec *iov, size_t count,
		 uint64_t access, uint64_t offset, uint64_t requested_key,
		 uint64_t flags, struct fid_mr **mr, void *context)
{
	struct rxm_domain *domain;

	domain = container_of(fid, struct rxm_domain,
			      util_domain.domain_fid.fid);
	return fi_mr_regv(domain->msg_domain, iov, count, access, offset,
			 requested_key, flags, mr, context);
}

static int rxm_mr_reg_thru(struct fid *fid, const void *buf, size_t len,
		      uint64_t access, uint64_t offset, uint64_t requested_key,
		      uint64_t flags, struct fid_mr **mr, void *context)
{
	struct rxm_domain *domain;

	domain = container_of(fid, struct rxm_domain,
			      util_domain.domain_fid.fid);
	return fi_mr_reg(domain->msg_domain, buf, len, access, offset,
			 requested_key, flags, mr, context);
}

static struct fi_ops_mr rxm_domain_mr_thru_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = rxm_mr_reg_thru,
	.regv = rxm_mr_regv_thru,
	.regattr = rxm_mr_regattr_thru,
};

static ssize_t rxm_send_credits(struct fid_ep *ep, uint64_t credits)
{
	struct rxm_conn *rxm_conn = ep->fid.context;
	struct rxm_ep *rxm_ep = rxm_conn->ep;
	struct rxm_deferred_tx_entry *def_tx_entry;
	struct rxm_tx_buf *tx_buf;
	struct iovec iov;
	struct fi_msg msg;
	ssize_t ret;

	tx_buf = ofi_buf_alloc(rxm_ep->tx_pool);
	if (!tx_buf) {
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA,
			"Ran out of buffers from TX credit buffer pool.\n");
		return -FI_ENOMEM;
	}

	tx_buf->hdr.state = RXM_CREDIT_TX;
	rxm_ep_format_tx_buf_pkt(rxm_conn, 0, rxm_ctrl_credit, 0, 0, FI_SEND,
				 &tx_buf->pkt);
	tx_buf->pkt.ctrl_hdr.type = rxm_ctrl_credit;
	tx_buf->pkt.ctrl_hdr.msg_id = ofi_buf_index(tx_buf);
	tx_buf->pkt.ctrl_hdr.ctrl_data = credits;

	if (rxm_conn->state != RXM_CM_CONNECTED)
		goto defer;

	iov.iov_base = &tx_buf->pkt;
	iov.iov_len = sizeof(struct rxm_pkt);
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.context = tx_buf;
	msg.desc = &tx_buf->hdr.desc;

	ret = fi_sendmsg(ep, &msg, OFI_PRIORITY);
	if (!ret)
		return FI_SUCCESS;

defer:
	def_tx_entry = rxm_ep_alloc_deferred_tx_entry(
		rxm_ep, rxm_conn, RXM_DEFERRED_TX_CREDIT_SEND);
	if (!def_tx_entry) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"unable to allocate TX entry for deferred CREDIT mxg\n");
		ofi_buf_free(tx_buf);
		return -FI_ENOMEM;
	}

	def_tx_entry->credit_msg.tx_buf = tx_buf;
	rxm_queue_deferred_tx(def_tx_entry, OFI_LIST_HEAD);
	return FI_SUCCESS;
}

static void rxm_no_add_credits(struct fid_ep *ep_fid, uint64_t credits)
{
}

static void rxm_no_credit_handler(struct fid_domain *domain_fid,
		ssize_t (*credit_handler)(struct fid_ep *ep, uint64_t credits))
{
}

static int rxm_no_enable_flow_ctrl(struct fid_ep *ep_fid, uint64_t threshold)
{
	return -FI_ENOSYS;
}

static bool rxm_no_flow_ctrl_available(struct fid_ep *ep_fid)
{
	return false;
}

struct ofi_ops_flow_ctrl rxm_no_ops_flow_ctrl = {
	.size = sizeof(struct ofi_ops_flow_ctrl),
	.add_credits = rxm_no_add_credits,
	.enable = rxm_no_enable_flow_ctrl,
	.set_send_handler = rxm_no_credit_handler,
	.available = rxm_no_flow_ctrl_available,
};

static int rxm_config_flow_ctrl(struct rxm_domain *domain)
{
	struct ofi_ops_flow_ctrl *flow_ctrl_ops;
	int ret;

	ret = fi_open_ops(&domain->msg_domain->fid, OFI_OPS_FLOW_CTRL, 0,
			  (void **) &flow_ctrl_ops, NULL);
	if (ret) {
		if (ret == -FI_ENOSYS) {
			domain->flow_ctrl_ops = &rxm_no_ops_flow_ctrl;
			return 0;
		}
		return ret;
	}

	assert(flow_ctrl_ops);
	domain->flow_ctrl_ops = flow_ctrl_ops;
	domain->flow_ctrl_ops->set_send_handler(domain->msg_domain,
						rxm_send_credits);
	return 0;
}

static uint64_t rxm_get_coll_caps(struct fid_domain *domain)
{
	struct fi_collective_attr attr;
	uint64_t mask = 0;

	attr.datatype = FI_INT8;
	attr.datatype_attr.count = 1;
	attr.datatype_attr.size = sizeof(int8_t);
	attr.mode = 0;
	for (int i = FI_BARRIER; i <= FI_GATHER; i++) {
		attr.op = (i == FI_BARRIER)? FI_NOOP : FI_MIN;
		if (fi_query_collective(domain, i, &attr, 0) == FI_SUCCESS )
			mask |= BIT(i);
	}
	return mask;
}

int rxm_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **domain, void *context)
{
	struct rxm_domain *rxm_domain;
	struct rxm_fabric *rxm_fabric;
	struct fi_info *msg_info, *base_info;
	struct fi_peer_domain_context peer_context;
	int ret;

	rxm_domain = calloc(1, sizeof(*rxm_domain));
	if (!rxm_domain)
		return -FI_ENOMEM;

	rxm_fabric = container_of(fabric, struct rxm_fabric, util_fabric.fabric_fid);

	base_info = rxm_passthru_info(info) ? &rxm_thru_info : NULL;
	ret = ofi_get_core_info(fabric->api_version, NULL, NULL, 0, &rxm_util_prov,
				info, base_info, rxm_info_to_core, &msg_info);
	if (ret)
		goto err1;

	ret = fi_domain(rxm_fabric->msg_fabric, msg_info,
			&rxm_domain->msg_domain, context);
	if (ret)
		goto err2;

	ret = ofi_domain_init(fabric, info, &rxm_domain->util_domain, context,
			      OFI_LOCK_MUTEX);
	if (ret) {
		goto err3;
	}

	if (info->caps & FI_COLLECTIVE) {
		if (!rxm_fabric->util_coll_fabric) {
			FI_WARN(&rxm_prov, FI_LOG_DOMAIN,
				"Util collective provider unavailable\n");
			goto err4;
		}

		peer_context.size = sizeof(peer_context);
		peer_context.domain = &rxm_domain->util_domain.domain_fid;

		ret = fi_domain2(rxm_fabric->util_coll_fabric,
				 rxm_fabric->util_coll_info,
				 &rxm_domain->util_coll_domain,
				 FI_PEER, &peer_context);
		if (ret)
			goto err4;

		if (rxm_fabric->offload_coll_fabric) {
			ret = fi_domain2(rxm_fabric->offload_coll_fabric,
					 rxm_fabric->offload_coll_info,
					 &rxm_domain->offload_coll_domain,
					 FI_PEER, &peer_context);
			if (ret)
				goto err5;

			rxm_domain->offload_coll_mask |=
			    rxm_get_coll_caps(rxm_domain->offload_coll_domain);
		}
	}

	/* We turn off the mr map mode bit FI_MR_PROV_KEY.  We always use the
	 * key returned by the MSG provider.  That key may be generated by the
	 * MSG provider, or will be provided as input by the rxm provider.
	 */
	rxm_domain->util_domain.mr_map.mode &= ~FI_MR_PROV_KEY;

	rxm_domain->max_atomic_size = rxm_ep_max_atomic_size(info);
	rxm_domain->rx_post_size = rxm_packet_size;

	*domain = &rxm_domain->util_domain.domain_fid;
	(*domain)->fid.ops = &rxm_domain_fi_ops;
	(*domain)->ops = &rxm_domain_ops;

	ret = ofi_bufpool_create(&rxm_domain->amo_bufpool,
				 rxm_domain->max_atomic_size, 64, 0, 0, 0);
	if (ret)
		goto err5;

	ofi_mutex_init(&rxm_domain->amo_bufpool_lock);

	rxm_domain->passthru = rxm_passthru_info(info);
	if (rxm_domain->passthru)
		(*domain)->mr = &rxm_domain_mr_thru_ops;
	else
		(*domain)->mr = &rxm_domain_mr_ops;

	ret = rxm_config_flow_ctrl(rxm_domain);
	if (ret)
		goto err6;

	fi_freeinfo(msg_info);
	return 0;

err6:
	ofi_mutex_destroy(&rxm_domain->amo_bufpool_lock);
	ofi_bufpool_destroy(rxm_domain->amo_bufpool);
err5:
	if (rxm_domain->offload_coll_domain)
		fi_close(&rxm_domain->offload_coll_domain->fid);
	if (rxm_domain->util_coll_domain)
		fi_close(&rxm_domain->util_coll_domain->fid);
err4:
	(void) ofi_domain_close(&rxm_domain->util_domain);
err3:
	fi_close(&rxm_domain->msg_domain->fid);
err2:
	fi_freeinfo(msg_info);
err1:
	free(rxm_domain);
	return ret;
}
