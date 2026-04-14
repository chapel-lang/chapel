/*
 * Copyright (c) 2025 Philipp A. Friese, Technical University of Munich
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

#include "hook_monitor.h"

#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __linux__
#include <linux/magic.h>
#include <sys/vfs.h>
#elif defined __APPLE__
#include <sys/mman.h>
#include <sys/mount.h>
#endif

#include <fstab.h>
#include <libgen.h>

struct monitor_environment mon_env = {
	.linger = MON_LINGER_DEFAULT,
	.tick_max = MON_TICK_MAX_DEFAULT,
	.file_mode = MON_FILE_MODE_DEFAULT,
	.dir_mode = MON_DIR_MODE_DEFAULT,
	.basepath = MON_BASEPATH_DEFAULT,
};

static inline struct monitor_context *monitor_ctx(struct hook_ep *ep)
{
	return &container_of(ep->domain->fabric, struct monitor_fabric,
	                     fabric_hook)->mon_ctx;
}

static inline struct monitor_context *monitor_ctx_cq(struct hook_cq *cq)
{
	return &container_of(cq->domain->fabric, struct monitor_fabric,
	                     fabric_hook)->mon_ctx;
}

static inline struct monitor_context *monitor_ctx_domain(struct hook_domain *dom)
{
	return &container_of(dom->fabric, struct monitor_fabric,
	                     fabric_hook)->mon_ctx;
}

static inline int mon_size_bucket(size_t len)
{
	if (len <= 64)
		return MON_SIZE_0_64;
	if (len <= 512)
		return MON_SIZE_64_512;
	if (len <= 1024)
		return MON_SIZE_512_1K;
	if (len <= 4096)
		return MON_SIZE_1K_4K;
	if (len <= 65536)      // 64K
		return MON_SIZE_4K_64K;
	if (len <= 0x40000)    // 256K
		return MON_SIZE_64K_256K;
	if (len <= 0x100000)   // 1M
		return MON_SIZE_256K_1M;
	if (len <= 0x400000)   // 4M
		return MON_SIZE_1M_4M;
	else
		return MON_SIZE_4M_UP;
}

static bool
get_cq_unknown_entry(void *buf, int idx, int *group, uint64_t *len)
{
	return false;
}

static bool
get_cq_context_entry(void *buf, int idx, int *cntr, uint64_t *len)
{
	*cntr = mon_cq_ctx;
	*len = MON_IGNORE_SIZE;

	return true;
}
static bool
get_cq_msg_entry(void *buf, int idx, int *cntr, uint64_t *len)
{
	struct fi_cq_msg_entry *entry = (struct fi_cq_msg_entry *)buf;

	if (entry[idx].flags & FI_RECV) {
		*len = entry[idx].len;
		*cntr = mon_cq_msg_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = MON_IGNORE_SIZE;
		*cntr = mon_cq_msg_tx;
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
		*cntr = mon_cq_data_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = MON_IGNORE_SIZE;
		*cntr = mon_cq_data_tx;
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
		*cntr = mon_cq_tagged_rx;
	} else if (entry[idx].flags & FI_SEND) {
		*len = MON_IGNORE_SIZE;
		*cntr = mon_cq_tagged_tx;
	} else {
		return false;
	}

	return true;
}

// order and meaning as in enum fi_cq_format (fi_eq.h)
static bool (*get_cq_entry[])(void *buf, int idx, int *cntr, uint64_t *len) = {
	get_cq_unknown_entry,
	get_cq_context_entry,
	get_cq_msg_entry,
	get_cq_data_entry,
	get_cq_tagged_entry
};

static void
mon_flush(struct monitor_context *ctx) {
	bool request = ctx->share->flags & 0b1;
	if (request) {
		// copy counters to share, clear request flag & reset local counters
		memcpy(ctx->share, ctx->data, sizeof (ctx->data));
		ctx->share->flags ^= 0b1;
		memset(ctx->data, 0, sizeof (ctx->data));
	}
}

static inline void
mon_add_cntr(struct monitor_context *ctx, int cntr, int index, size_t size) {
	ctx->data[cntr].count[index]++;
	if (size != MON_IGNORE_SIZE) {
		ctx->data[cntr].sum[index] += size;
	}
	ctx->tick++;
	if (ctx->tick >= mon_env.tick_max) {
		ctx->tick = 0;
		mon_flush(ctx);
	}
}

static inline void
mon_add_cq_cntr(struct monitor_context *ctx, int cntr,
                 enum fi_cq_format format, void *buf, int ret)
{
	uint64_t len;
	for (int i = 0; i < ret; i++) {
		if (get_cq_entry[format](buf, i, &cntr, &len))
			mon_add_cntr(ctx, cntr, mon_size_bucket(len), len);
	}
}

/*
 * APIs
 */
static ssize_t
monitor_recv(struct fid_ep *ep, void *buf, size_t len, void *desc,
             fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recv(myep->hep, buf, len, desc, src_addr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_recv, 0, MON_IGNORE_SIZE);
	}
	return ret;
}

static ssize_t
monitor_recvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
               size_t count, fi_addr_t src_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvv(myep->hep, iov, desc, count, src_addr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_recvv, 0, MON_IGNORE_SIZE);
	}
	return ret;
}

static ssize_t
monitor_recvmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_recvmsg(myep->hep, msg, flags);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_recvmsg, 0, MON_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
monitor_send(struct fid_ep *ep, const void *buf, size_t len, void *desc,
              fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_send(myep->hep, buf, len, desc, dest_addr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_send,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
               size_t count, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_sendv(myep->hep, iov, desc, count, dest_addr, context);
	if (!ret) {
		len = ofi_total_iov_len(iov, count);
		mon_add_cntr(monitor_ctx(myep), mon_sendv,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_sendmsg(struct fid_ep *ep, const struct fi_msg *msg, uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_sendmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		mon_add_cntr(monitor_ctx(myep), mon_sendmsg,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_inject(struct fid_ep *ep, const void *buf, size_t len,
                fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject(myep->hep, buf, len, dest_addr);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_inject,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_senddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
                  uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_senddata(myep->hep, buf, len, desc, data, dest_addr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_senddata,
		              mon_size_bucket(len), len);

	}

	return ret;
}

static ssize_t
monitor_injectdata(struct fid_ep *ep, const void *buf, size_t len,
                    uint64_t data, fi_addr_t dest_addr)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_injectdata(myep->hep, buf, len, data, dest_addr);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_injectdata,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_msg monitor_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = monitor_recv,
	.recvv = monitor_recvv,
	.recvmsg = monitor_recvmsg,
	.send = monitor_send,
	.sendv = monitor_sendv,
	.sendmsg = monitor_sendmsg,
	.inject = monitor_inject,
	.senddata = monitor_senddata,
	.injectdata = monitor_injectdata,
};


static ssize_t
monitor_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
              fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_read(myep->hep, buf, len, desc, src_addr, addr, key, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_read,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
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
		mon_add_cntr(monitor_ctx(myep), mon_readv,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
                 uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_readmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		mon_add_cntr(monitor_ctx(myep), mon_readmsg,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
               fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_write(myep->hep, buf, len, desc, dest_addr, addr, key, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_write,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
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
		mon_add_cntr(monitor_ctx(myep), mon_writev,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_writemsg(myep->hep, msg, flags);
	if (!ret) {
		len =  ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		mon_add_cntr(monitor_ctx(myep), mon_writemsg,
		              mon_size_bucket(len), len);
	}
	return ret;
}

static ssize_t
monitor_inject_write(struct fid_ep *ep, const void *buf, size_t len,
                      fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_write(myep->hep, buf, len, dest_addr, addr, key);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_inject_write,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_writedata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		   uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_writedata(myep->hep, buf, len, desc, data,
	                   dest_addr, addr, key, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_writedata,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
                          uint64_t data, fi_addr_t dest_addr,
                          uint64_t addr, uint64_t key)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_inject_writedata(myep->hep, buf, len, data, dest_addr,
	                          addr, key);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_injectdata,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_rma monitor_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = monitor_read,
	.readv = monitor_readv,
	.readmsg = monitor_readmsg,
	.write = monitor_write,
	.writev = monitor_writev,
	.writemsg = monitor_writemsg,
	.inject = monitor_inject_write,
	.writedata = monitor_writedata,
	.injectdata = monitor_inject_writedata,
};


static ssize_t
monitor_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
               fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
               void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecv(myep->hep, buf, len, desc, src_addr, tag, ignore, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_trecv, 0, MON_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
monitor_trecvv(struct fid_ep *ep, const struct iovec *iov, void **desc,
                size_t count, fi_addr_t src_addr, uint64_t tag,
                uint64_t ignore, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvv(myep->hep, iov, desc, count, src_addr,
	                tag, ignore, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_trecvv, 0, MON_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
monitor_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_trecvmsg(myep->hep, msg, flags);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_trecvmsg, 0, MON_IGNORE_SIZE);
	}

	return ret;
}

static ssize_t
monitor_tsend(struct fid_ep *ep, const void *buf, size_t len, void *desc,
               fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsend(myep->hep, buf, len, desc, dest_addr, tag, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_tsend,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_tsendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
                size_t count, fi_addr_t dest_addr, uint64_t tag,
                void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_tsendv(myep->hep, iov, desc, count, dest_addr, tag, context);
	if (!ret) {
		len = ofi_total_iov_len(iov, count);
		mon_add_cntr(monitor_ctx(myep), mon_tsendv,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
                  uint64_t flags)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	size_t len;
	ssize_t ret;

	ret = fi_tsendmsg(myep->hep, msg, flags);
	if (!ret) {
		len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
		mon_add_cntr(monitor_ctx(myep), mon_tsendmsg,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_tinject(struct fid_ep *ep, const void *buf, size_t len,
                 fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinject(myep->hep, buf, len, dest_addr, tag);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_tinject,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_tsenddata(struct fid_ep *ep, const void *buf, size_t len, void *desc,
                   uint64_t data, fi_addr_t dest_addr, uint64_t tag,
                   void *context)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tsenddata(myep->hep, buf, len, desc, data,
	                   dest_addr, tag, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_tsenddata,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static ssize_t
monitor_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
                     uint64_t data, fi_addr_t dest_addr, uint64_t tag)
{
	struct hook_ep *myep = container_of(ep, struct hook_ep, ep);
	ssize_t ret;

	ret = fi_tinjectdata(myep->hep, buf, len, data, dest_addr, tag);
	if (!ret) {
		mon_add_cntr(monitor_ctx(myep), mon_tinjectdata,
		              mon_size_bucket(len), len);
	}

	return ret;
}

static struct fi_ops_tagged monitor_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = monitor_trecv,
	.recvv = monitor_trecvv,
	.recvmsg = monitor_trecvmsg,
	.send = monitor_tsend,
	.sendv = monitor_tsendv,
	.sendmsg = monitor_tsendmsg,
	.inject = monitor_tinject,
	.senddata = monitor_tsenddata,
	.injectdata = monitor_tinjectdata,
};

static ssize_t monitor_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_read(mycq->hcq, buf, count);
	if (ret>0) {
		mon_add_cq_cntr(monitor_ctx_cq(mycq), mon_cq_read,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static ssize_t
monitor_cq_readfrom(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readfrom(mycq->hcq, buf, count, src_addr);
	if (ret>0) {
		mon_add_cq_cntr(monitor_ctx_cq(mycq), mon_cq_readfrom,
		                 mycq->format, buf, ret);
	}

	return ret;
}
static ssize_t
monitor_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_readerr(mycq->hcq, buf, flags);

	return ret;
}

static ssize_t
monitor_cq_sread(struct fid_cq *cq, void *buf, size_t count,
		  const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sread(mycq->hcq, buf, count, cond, timeout);
	if (ret > 0) {
		mon_add_cq_cntr(monitor_ctx_cq(mycq), mon_cq_sread,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static ssize_t
monitor_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
		  fi_addr_t *src_addr, const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	ssize_t ret;

	ret = fi_cq_sreadfrom(mycq->hcq, buf, count, src_addr, cond, timeout);
	if (ret > 0) {
		mon_add_cq_cntr(monitor_ctx_cq(mycq), mon_cq_sreadfrom,
		                 mycq->format, buf, ret);
	}
	return ret;
}

static int monitor_cq_signal(struct fid_cq *cq)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);
	int ret;

	ret = fi_cq_signal(mycq->hcq);
	return ret;
}

struct fi_ops_cq monitor_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = monitor_cq_read,
	.readfrom = monitor_cq_readfrom,
	.readerr = monitor_cq_readerr,
	.sread = monitor_cq_sread,
	.sreadfrom = monitor_cq_sreadfrom,
	.signal = monitor_cq_signal,
	.strerror = hook_cq_strerror,
};

static int
monitor_mr_reg(struct fid *fid, const void *buf, size_t len,
               uint64_t access, uint64_t offset, uint64_t requested_key,
               uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret;

	ret = fi_mr_reg(dom->hdomain, buf, len, access, offset, requested_key,
					flags, mr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx_domain(dom), mon_mr_reg,
		              FI_HMEM_SYSTEM, len);
	}

	return ret;
}

static int
monitor_mr_regv(struct fid *fid, const struct iovec *iov,
              size_t count, uint64_t access,
              uint64_t offset, uint64_t requested_key,
              uint64_t flags, struct fid_mr **mr, void *context)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret;

	ret = fi_mr_regv(dom->hdomain, iov, count, access, offset,
					 requested_key, flags, mr, context);
	if (!ret) {
		mon_add_cntr(monitor_ctx_domain(dom), mon_mr_regv, FI_HMEM_SYSTEM,
		              ofi_total_iov_len(iov, count));
	}

	return ret;
}

static int
monitor_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
                 uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	int ret;

	ret = fi_mr_regattr(dom->hdomain, attr, flags, mr);
	if (!ret) {
		mon_add_cntr(monitor_ctx_domain(dom), mon_mr_regattr, attr->iface,
		        ofi_total_iov_len(attr->mr_iov, attr->iov_count));
	}

	return ret;
}

static struct fi_ops_mr monitor_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = monitor_mr_reg,
	.regv = monitor_mr_regv,
	.regattr = monitor_mr_regattr,
};

static int monitor_domain_init(struct fid *fid)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);
	domain->mr = &monitor_mr_ops;

	return 0;
}

static ofi_atomic64_t monitor_id;
static int64_t monitor_get_id()
{
	ofi_atomic_add64(&monitor_id, 1);
	return ofi_atomic_get64(&monitor_id);
}

static int
monitor_shm_init(struct monitor_context *mon_ctx)
{
	const struct fi_provider *hprov = mon_ctx->hprov;
	mode_t old_umask;
	char outpath[PATH_MAX];
	char hostname[PATH_MAX];
	char buffer[PATH_MAX];
	struct statfs info;
	bool is_tmpfs;
	unsigned long job_id;
	const char* slurm_job_id_str;
	unsigned int slurm_job_id;
	char *endptr;
	int fd;

	errno = 0;
	old_umask = umask(0);
	if (mkdir(mon_env.basepath, 01777) != 0) {
		if (errno != EEXIST) {
			FI_WARN(hprov, FI_LOG_FABRIC,
				"Could not create folder at %s: %s\n",
				mon_env.basepath, strerror(errno));
			umask(old_umask);
			return -errno;
		}
	}
	umask(old_umask);

	// append UID to path, try to mkdir
	if (snprintf(outpath, PATH_MAX, "%s/%u", mon_env.basepath, getuid()) < 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to format hook storage path!\n");
		return -EOVERFLOW;
	}
	if (mkdir(outpath, mon_env.dir_mode) != 0) {
		if (errno != EEXIST) {
			FI_WARN(hprov, FI_LOG_FABRIC,
				"Could not create folder at %s: %s\n",
				outpath, strerror(errno));
			return -errno;
		}
	}

	// append hostname to path, try to mkdir
	//  useful for containers which usually set hostname to the container name
	if (gethostname(hostname, PATH_MAX) != 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to call gethostname (%s)!\n",
			strerror(errno));
		return -errno;
	}
	if (snprintf(buffer, PATH_MAX, "%s/%s", outpath, hostname) < 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to format outpath!\n");
		return -EOVERFLOW;
	}
	strncpy(outpath, buffer, PATH_MAX);
	errno = 0;
	if (mkdir(outpath, mon_env.dir_mode) != 0) {
		if (errno != EEXIST) {
			FI_WARN(hprov, FI_LOG_FABRIC,
				"Could not create folder at %s: %s\n",
				outpath, strerror(errno));
			return -errno;
		}
	}
	statfs(outpath, &info);
#ifdef __APPLE__
	is_tmpfs = strncmp(info.f_fstypename, "tmpfs", 5) == 0;
#else
	is_tmpfs = info.f_type == TMPFS_MAGIC;
#endif
	if (!is_tmpfs) {
		FI_WARN(hprov, FI_LOG_FABRIC,
			"Communication file not on tmpfs! (%s) Proceed with caution.\n",
			outpath);
	}

	job_id = 0;
	slurm_job_id_str = getenv("SLURM_JOB_ID");
	if (slurm_job_id_str != NULL) {
		errno = 0;
		slurm_job_id = strtoul(slurm_job_id_str, &endptr, 0);
		if (errno == ERANGE || *endptr != '\0' || slurm_job_id_str == endptr)
			FI_WARN(hprov, FI_LOG_FABRIC,
				"Could not parse SLURM_JOB_ID!\n");
		else
			job_id = slurm_job_id;
	}


	if (snprintf(mon_ctx->shm_name, PATH_MAX, "%s/%u_%u_%lu_%lu_%s",
		     outpath, getppid(), getpid(), monitor_get_id(),job_id,
		     hprov->name) < 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed so format shm name!\n");
		return -EOVERFLOW;
	}

	FI_TRACE(hprov, FI_LOG_FABRIC, "[%s] Using file: %s\n",
		 hprov->name, mon_ctx->shm_name);

	fd = open(mon_ctx->shm_name, O_CREAT | O_RDWR, S_IRUSR|S_IWUSR);
	if (fd < 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to create shm (%s)\n",
			strerror(errno));
		return -FI_ENOENT;
	}

	if (fchmod(fd, mon_env.file_mode) != 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to chmod %s: %s\n",
			mon_ctx->shm_name, strerror(errno));
		goto error;
	}

	if (ftruncate(fd, sizeof (struct monitor_mapped_data)) != 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to truncate %s: %s\n",
			mon_ctx->shm_name, strerror(errno));
		goto error;
	}
	mon_ctx->share = mmap(0, sizeof (struct monitor_mapped_data), PROT_READ|PROT_WRITE,
			      MAP_SHARED, fd, 0);
	if (mon_ctx->share == MAP_FAILED) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to mmap: %s\n",
			strerror(errno));
		goto error;
	}

	// flush data on init
	mon_ctx->share->flags = 0b0;

	close(fd);
	return FI_SUCCESS;
error:
	close(fd);
	return -errno;
}

static int
monitor_shm_close(struct monitor_context *mon_ctx)
{
	const struct fi_provider *hprov = mon_ctx->hprov;
	int ret;

	if (mon_env.linger) {
		uint8_t old_flags = mon_ctx->share->flags;

		// set request flag first to avoid tearing
		mon_ctx->share->flags |= 0b1;
		FI_TRACE(hprov, FI_LOG_FABRIC,
			 "[%s] Lingering enabled, will flush file %s but not delete.\n",
			 hprov->name, mon_ctx->shm_name);

		// check if data in shm has been read and add counters if not
		if (!(old_flags & 0b1)) {
			struct monitor_data old_data[mon_api_size];
			memcpy(old_data, mon_ctx->share, sizeof (old_data));
			for (int i = 0; i < mon_api_size; i++) {
				for(int j = 0; j < MON_SIZE_MAX; j++) {
					mon_ctx->data[i].count[j] += old_data[i].count[j];
					mon_ctx->data[i].sum[j] += old_data[i].sum[j];
				}
			}
		}
		memcpy(mon_ctx->share, mon_ctx->data, sizeof (mon_ctx->data));
		mon_ctx->share->flags |= 0b10; // set fin flag
		mon_ctx->share->flags ^= 0b01; // clear request flag
	} else {
		if ((ret = unlink(mon_ctx->shm_name)) != 0) {
			FI_WARN(hprov, FI_LOG_CORE, "Failed to unlink! (%s)\n",
				strerror(errno));
			return ret;
		}
	}

	if ((ret = munmap(mon_ctx->share, sizeof (struct monitor_mapped_data))) != 0) {
		FI_WARN(hprov, FI_LOG_CORE, "Failed to munmap! (%s)\n", strerror(errno));
		return ret;
	}

	return FI_SUCCESS;
}

static int hook_monitor_close(struct fid *fid)
{
	struct monitor_context *ctx =
		&(container_of(fid, struct monitor_fabric, fabric_hook)->mon_ctx);

	monitor_shm_close(ctx);

	hook_close(fid);
	FI_TRACE(ctx->hprov, FI_LOG_CORE, "[%s] Closing monitor hook\n", ctx->hprov->name);
	return FI_SUCCESS;
}

static struct fi_ops monitor_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_monitor_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

struct hook_prov_ctx hook_monitor_ctx;

static int
hook_monitor_fabric(struct fi_fabric_attr *attr,
                     struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct monitor_fabric *fab;
	int ret;

	FI_TRACE(hprov, FI_LOG_FABRIC, "[%s] Installing monitor hook\n",
		hprov->name);

	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	fab->mon_ctx.hprov = hprov;
	memset(&fab->mon_ctx.data, 0, sizeof (fab->mon_ctx.data));

	ofi_atomic_initialize64(&monitor_id, 0);
	ret = monitor_shm_init(&fab->mon_ctx);
	if (ret != FI_SUCCESS) {
		FI_WARN(hprov, FI_LOG_FABRIC,
			"Could not initialise ofi_hook_monitor!\n");
		return -FI_EACCES;
	}

	hook_fabric_init(&fab->fabric_hook, HOOK_MONITOR, attr->fabric, hprov,
	                 &monitor_fabric_fid_ops, &hook_monitor_ctx);
	*fabric = &fab->fabric_hook.fabric;
	FI_TRACE(hprov, FI_LOG_FABRIC, "[%s] Installed monitor hook\n",
		hprov->name);
	return 0;
}


struct hook_prov_ctx hook_monitor_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_monitor",
		.getinfo = NULL,
		.fabric = hook_monitor_fabric,
		.cleanup = NULL,
	},
};

static int monitor_cq_init(struct fid *fid)
{
	struct fid_cq *cq = container_of(fid, struct fid_cq, fid);
	cq->ops = &monitor_cq_ops;
	return 0;
}

static int monitor_ep_init(struct fid *fid)
{
	struct fid_ep *ep = container_of(fid, struct fid_ep, fid);
	ep->msg = &monitor_msg_ops;
	ep->rma = &monitor_rma_ops;
	ep->tagged = &monitor_tagged_ops;

	return 0;
}

static int monitor_env_init(void)
{
	struct fi_provider *prov = &hook_monitor_ctx.prov;
	int signed_tick_max;
	char *basepath = NULL;

	fi_param_define(prov, "linger", FI_PARAM_BOOL,
			"Whether synchronisation files should linger after termination. (default: %d)",
			mon_env.linger);
	fi_param_get_bool(prov, "linger", &mon_env.linger);

	fi_param_define(prov, "tick_max", FI_PARAM_INT,
			"Number of API calls before synchronisation files are checked for data request. (default: %d)",
			mon_env.tick_max);

	fi_param_get_int(prov, "tick_max", &signed_tick_max);
	if (signed_tick_max < 0) {
		FI_WARN(prov, FI_LOG_CORE, "Tick is negative!\n");
		return -EOVERFLOW;
	}
	mon_env.tick_max = (unsigned int)signed_tick_max;

	fi_param_define(prov, "file_mode", FI_PARAM_INT,
			"POSIX mode/permission for synchronisation files. (default: %04o)",
			mon_env.file_mode);
	fi_param_get_int(prov, "file_mode", &mon_env.file_mode);

	fi_param_define(prov, "dir_mode", FI_PARAM_INT,
			"POSIX mode/permission for directories in basepath. (default: %04o)",
			mon_env.dir_mode);
	fi_param_get_int(prov, "dir_mode", &mon_env.dir_mode);

	fi_param_define(prov, "basepath", FI_PARAM_STRING,
			"String to basepath for synchronisation files. (default: %s)",
			mon_env.basepath);

	fi_param_get_str(prov, "basepath", &basepath);
	if (basepath != NULL && strlen(basepath) < PATH_MAX)
		snprintf(mon_env.basepath, PATH_MAX, "%s", basepath);

	return 0;
}

HOOK_MONITOR_INI
{
	if (monitor_env_init())
		return NULL;

	hook_monitor_ctx.ini_fid[FI_CLASS_DOMAIN] = monitor_domain_init;
	hook_monitor_ctx.ini_fid[FI_CLASS_CQ] = monitor_cq_init;
	hook_monitor_ctx.ini_fid[FI_CLASS_EP] = monitor_ep_init;

	return &hook_monitor_ctx.prov;
}
