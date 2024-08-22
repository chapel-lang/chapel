/*
 * Copyright (c) 2018-2023 Intel Corporation. All rights reserved.
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

#include "ofi_hook.h"
#include "ofi_prov.h"
#include "ofi_iov.h"
#include "hook_prov.h"

#include "hook_profile.h"

static inline struct profile_context *profile_ctx(struct hook_ep *ep)
{
	return &container_of(ep->domain->fabric, struct profile_fabric,
	                     fabric_hook)->prof_ctx;
}

static inline struct profile_context *profile_ctx_cq(struct hook_cq *cq)
{
	return &container_of(cq->domain->fabric, struct profile_fabric,
	                     fabric_hook)->prof_ctx;
}

static inline struct profile_context *profile_ctx_domain(struct hook_domain *dom)
{
	return &container_of(dom->fabric, struct profile_fabric,
	                     fabric_hook)->prof_ctx;
}

static inline int prof_size_bucket(size_t len)
{
	if (len <= 64)
		return PROF_SIZE_0_64;
	if (len <= 512)
		return PROF_SIZE_64_512;
	if (len <= 1024)
		return PROF_SIZE_512_1K;
	if (len <= 4096)
		return PROF_SIZE_1K_4K;
	if (len <= 65536)      // 64K
		return PROF_SIZE_4K_64K;
	if (len <= 0x40000)    // 256K
		return PROF_SIZE_64K_256K;
	if (len <= 0x100000)   // 1M
		return PROF_SIZE_256K_1M;
	if (len <= 0x400000)   // 4M
		return PROF_SIZE_1M_4M;
	else
		return PROF_SIZE_4M_UP;
}

static bool
get_cq_unknown_entry(void *buf, int idx, int *group, uint64_t *len)
{
	return false;
}

static bool
get_cq_context_entry(void *buf,  int idx, int *cntr, uint64_t *len)
{
	*cntr = prof_cq_ctx;
	*len = PROF_IGNORE_SIZE;

	return true;
}
static bool
get_cq_msg_entry(void *buf, int idx, int *cntr, uint64_t *len)
{
	struct fi_cq_msg_entry *entry = (struct fi_cq_msg_entry *)buf;

	if (entry[idx].flags & FI_RECV) {
		*len = entry[idx].len;
		*cntr = prof_cq_msg_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = PROF_IGNORE_SIZE;
		*cntr = prof_cq_msg_tx;
	} else {
		return false;
	}

	return true;
}
static bool
get_cq_data_entry(void *buf, int idx, int *cntr, uint64_t *len)
{
	struct fi_cq_data_entry *entry = (struct fi_cq_data_entry *)buf;

	if (entry[idx].flags & FI_RECV) {
		*len = entry[idx].len;
		*cntr = prof_cq_data_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = PROF_IGNORE_SIZE;
		*cntr = prof_cq_data_tx;
	} else {
		return false;
	}

	return true;
}
static bool
get_cq_tagged_entry(void *buf, int idx, int *cntr, uint64_t *len)
{
	struct fi_cq_tagged_entry *entry = (struct fi_cq_tagged_entry *)buf;

	if (entry[idx].flags & FI_RECV) {
		*len = entry[idx].len;
		*cntr = prof_cq_tagged_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = PROF_IGNORE_SIZE;
		*cntr = prof_cq_tagged_tx;
	} else {
		return false;
	}

	return true;
}

static bool (*get_cq_entry[])(void *buf, int idx, int *cntr, uint64_t *len) = {
	get_cq_unknown_entry,
	get_cq_context_entry,
	get_cq_msg_entry,
	get_cq_data_entry,
	get_cq_tagged_entry
};

static inline void
prof_add_cntr(struct profile_context *ctx, int cntr, int index, size_t size) {
	ctx->data[cntr].count[index]++;
	if (size != PROF_IGNORE_SIZE) {
		ctx->data[cntr].sum[index] += size;
	}
}

static inline void
prof_add_cq_cntr(struct profile_context *ctx, int cntr,
                 enum fi_cq_format format, void *buf, int ret)
{
	uint64_t len;
	for (int i = 0; i < ret; i++) {
		if (get_cq_entry[format](buf, i, &cntr, &len))
			prof_add_cntr(ctx, cntr, prof_size_bucket(len), len);
	}
}

/*
 * APIs
 */
static ssize_t
profile_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
             fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recv(myep->hep, buf, len, desc, src_addr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_recv, 0, PROF_IGNORE_SIZE);
	}
	return ret;
}

static ssize_t
profile_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
               size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvv(myep->hep, iov, desc, count, src_addr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_recvv, 0, PROF_IGNORE_SIZE);
	}
	return ret;
}

static ssize_t
profile_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvmsg(myep->hep, msg, flags);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_recvmsg, 0, PROF_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
profile_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
              fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_send(myep->hep, buf, len, desc, dest_addr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_send,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
               size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_sendv(myep->hep, iov, desc, count, dest_addr, context);
	if (!ret) {
		len = ofi_total_iov_len(iov, count);
		prof_add_cntr(profile_ctx(myep), prof_sendv,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_sendmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		prof_add_cntr(profile_ctx(myep), prof_sendmsg,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_inject(struct fid_ep *ep, const void *buf, size_t len,
                fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject(myep->hep, buf, len, dest_addr);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_inject,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
                  uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_senddata(myep->hep, buf, len, desc, data, dest_addr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_senddata,
		              prof_size_bucket(len), len);

	}

	return ret;
}

static ssize_t
profile_injectdata(struct fid_ep *ep, const void *buf, size_t len,
                    uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_injectdata(myep->hep, buf, len, data, dest_addr);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_injectdata,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_msg profile_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = profile_recv,
	.recvv = profile_recvv,
	.recvmsg = profile_recvmsg,
	.send = profile_send,
	.sendv = profile_sendv,
	.sendmsg = profile_sendmsg,
	.inject = profile_inject,
	.senddata = profile_senddata,
	.injectdata = profile_injectdata,
};


static ssize_t
profile_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
              fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_read(myep->hep, buf, len, desc, src_addr, addr, key, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_read,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
               size_t count, fi_addr_t src_addr, uint64_t addr,
               uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_readv(myep->hep, iov, desc, count, src_addr,
	               addr, key, context);
	if (!ret) {
		len = ofi_total_iov_len(iov, count);
		prof_add_cntr(profile_ctx(myep), prof_readv,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
                 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_readmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		prof_add_cntr(profile_ctx(myep), prof_readmsg,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
               fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_write(myep->hep, buf, len, desc, dest_addr, addr, key, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_write,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
                size_t count, fi_addr_t dest_addr, uint64_t addr,
                uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_writev(myep->hep, iov, desc, count, dest_addr,
	                addr, key, context);
	if (!ret) {
		len =  ofi_total_iov_len(iov, count);
		prof_add_cntr(profile_ctx(myep), prof_writev,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_writemsg(myep->hep, msg, flags);
	if (!ret) {
		len =  ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		prof_add_cntr(profile_ctx(myep), prof_writemsg,
		              prof_size_bucket(len), len);
	}
	return ret;
}

static ssize_t
profile_inject_write(struct fid_ep *ep, const void *buf, size_t len,
                      fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_write(myep->hep, buf, len, dest_addr, addr, key);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_inject_write,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		   uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_writedata(myep->hep, buf, len, desc, data,
	                   dest_addr, addr, key, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_writedata,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
                          uint64_t data, fi_addr_t dest_addr,
                          uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_writedata(myep->hep, buf, len, data, dest_addr,
	                          addr, key);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_injectdata,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_rma profile_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = profile_read,
	.readv = profile_readv,
	.readmsg = profile_readmsg,
	.write = profile_write,
	.writev = profile_writev,
	.writemsg = profile_writemsg,
	.inject = profile_inject_write,
	.writedata = profile_writedata,
	.injectdata = profile_inject_writedata,
};


static ssize_t
profile_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
               fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
               void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecv(myep->hep, buf, len, desc, src_addr, tag, ignore, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_trecv, 0, PROF_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
profile_trecvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
                size_t count, fi_addr_t src_addr, uint64_t tag,
                uint64_t ignore, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvv(myep->hep, iov, desc, count, src_addr,
	                tag, ignore, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_trecvv, 0, PROF_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
profile_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvmsg(myep->hep, msg, flags);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_trecvmsg, 0, PROF_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
profile_tsend(struct fid_ep *ep, const void *buf, size_t len, void *desc,
               fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsend(myep->hep, buf, len, desc, dest_addr, tag, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_tsend,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_tsendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
                size_t count, fi_addr_t dest_addr, uint64_t tag,
                void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_tsendv(myep->hep, iov, desc, count, dest_addr, tag, context);
	if (!ret) {
		len = ofi_total_iov_len(iov, count);
		prof_add_cntr(profile_ctx(myep), prof_tsendv,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_tsendmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		prof_add_cntr(profile_ctx(myep), prof_tsendmsg,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_tinject(struct fid_ep *ep, const void *buf, size_t len,
                 fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinject(myep->hep, buf, len, dest_addr, tag);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_tinject,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_tsenddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
                   uint64_t data, fi_addr_t dest_addr, uint64_t tag,
                   void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsenddata(myep->hep, buf, len, desc, data,
	                   dest_addr, tag, context);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_tsenddata,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
profile_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
                     uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinjectdata(myep->hep, buf, len, data, dest_addr, tag);
	if (!ret) {
		prof_add_cntr(profile_ctx(myep), prof_tinjectdata,
		              prof_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_tagged profile_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = profile_trecv,
	.recvv = profile_trecvv,
	.recvmsg = profile_trecvmsg,
	.send = profile_tsend,
	.sendv = profile_tsendv,
	.sendmsg = profile_tsendmsg,
	.inject = profile_tinject,
	.senddata = profile_tsenddata,
	.injectdata = profile_tinjectdata,
};

static ssize_t profile_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_read(mycq->hcq, buf, count);
	if (ret>0) {
		prof_add_cq_cntr(profile_ctx_cq(mycq), prof_cq_read,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static ssize_t
profile_cq_readfrom(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readfrom(mycq->hcq, buf, count, src_addr);
	if (ret>0) {
		prof_add_cq_cntr(profile_ctx_cq(mycq), prof_cq_readfrom,
		                 mycq->format, buf, ret);
	}

	return ret;
}
static ssize_t
profile_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readerr(mycq->hcq, buf, flags);

	return ret;
}

static ssize_t
profile_cq_sread(struct fid_cq *cq, void *buf, size_t count,
		  const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sread(mycq->hcq, buf, count, cond, timeout);
	if (ret > 0) {
		prof_add_cq_cntr(profile_ctx_cq(mycq), prof_cq_sread,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static ssize_t
profile_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
		  fi_addr_t *src_addr, const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sreadfrom(mycq->hcq, buf, count, src_addr, cond, timeout);
	if (ret > 0) {
		prof_add_cq_cntr(profile_ctx_cq(mycq), prof_cq_sreadfrom,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static int profile_cq_signal(struct fid_cq *cq)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	int ret;

	ret = fi_cq_signal(mycq->hcq);
	return ret;
}

struct fi_ops_cq profile_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = profile_cq_read,
	.readfrom = profile_cq_readfrom,
	.readerr = profile_cq_readerr,
	.sread = profile_cq_sread,
	.sreadfrom = profile_cq_sreadfrom,
	.signal = profile_cq_signal,
	.strerror = hook_cq_strerror,
};

static int
profile_mr_reg(struct fid *fid, const void *buf, size_t len,
               uint64_t access, uint64_t offset, uint64_t requested_key,
               uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret = 0;

	ret = fi_mr_reg(dom->hdomain, buf, len, access, offset, requested_key,
					flags, mr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx_domain(dom), prof_mr_reg,
		              FI_HMEM_SYSTEM, len);
	}

	return ret;
}

static int
profile_mr_regv(struct fid *fid, const struct iovec *iov,
              size_t count, uint64_t access,
              uint64_t offset, uint64_t requested_key,
              uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret = 0;

	ret = fi_mr_regv(dom->hdomain, iov, count, access, offset,
					 requested_key, flags, mr, context);
	if (!ret) {
		prof_add_cntr(profile_ctx_domain(dom), prof_mr_regv, FI_HMEM_SYSTEM,
		              ofi_total_iov_len(iov, count));
	}

	return ret;
}

static int
profile_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
                 uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret = 0;

	ret = fi_mr_regattr(dom->hdomain, attr, flags, mr);
	if (!ret) {
		prof_add_cntr(profile_ctx_domain(dom), prof_mr_regattr, attr->iface,
		        ofi_total_iov_len(attr->mr_iov, attr->iov_count));
	}

	return ret;
}

static struct fi_ops_mr profile_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = profile_mr_reg,
	.regv = profile_mr_regv,
	.regattr = profile_mr_regattr,
};

static int profile_domain_init(struct fid *fid)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);
	domain->mr = &profile_mr_ops;

	return 0;
}

static int hook_profile_close(struct fid *fid)
{
	struct profile_context *ctx = 
		&(container_of(fid, struct profile_fabric, fabric_hook)->prof_ctx);

	prof_report(ctx->hprov, ctx->data);

	hook_close(fid);
	return FI_SUCCESS;
}

static struct fi_ops profile_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_profile_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

struct hook_prov_ctx hook_profile_ctx;

static int 
hook_profile_fabric(struct fi_fabric_attr *attr,
                     struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct profile_fabric *fab;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing profile hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	fab->prof_ctx.hprov = hprov;
	memset(&fab->prof_ctx.data, 0, sizeof (fab->prof_ctx.data));
	hook_fabric_init(&fab->fabric_hook, HOOK_PROFILE, attr->fabric, hprov,
	                 &profile_fabric_fid_ops, &hook_profile_ctx);
	*fabric = &fab->fabric_hook.fabric;
	return 0;
}

struct hook_prov_ctx hook_profile_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_profile",
		.getinfo = NULL,
		.fabric = hook_profile_fabric,
		.cleanup = NULL,
	},
};


static int profile_cq_init(struct fid *fid)
{
	struct fid_cq *cq = container_of(fid, struct fid_cq, fid);
	cq->ops = &profile_cq_ops;
	return 0;
}

static int profile_ep_init(struct fid *fid)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	ep->msg = &profile_msg_ops;
	ep->rma = &profile_rma_ops;
	ep->tagged = &profile_tagged_ops;

	return 0;
}

HOOK_PROFILE_INI
{
	hook_profile_ctx.ini_fid[FI_CLASS_DOMAIN] = profile_domain_init;
	hook_profile_ctx.ini_fid[FI_CLASS_CQ] = profile_cq_init;
	hook_profile_ctx.ini_fid[FI_CLASS_EP] = profile_ep_init;

	return &hook_profile_ctx.prov;
}
