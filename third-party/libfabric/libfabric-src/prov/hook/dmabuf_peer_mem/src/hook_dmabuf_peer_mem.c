/*
 * Copyright (c) 2021 Intel Corporation. All rights reserved.
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

#include <inttypes.h>
#include <sys/ioctl.h>
#include "ofi.h"
#include "ofi_prov.h"
#include "ofi_hmem.h"
#include "hook_prov.h"
#include "hook_dmabuf_peer_mem.h"
#include "dmabuf_reg.h"

static char *dmabuf_reg_dev_name = "/dev/" DMABUF_REG_DEV_NAME;

/*
 * Add dmabuf info to the registry. Ok if the info matches an existing
 * entry, in which case the reference counter of that entry is increased.
 * Return 0 on success.
 */
static int dmabuf_reg_add(int reg_fd, uint64_t base, uint64_t size, int fd)
{
	struct dmabuf_reg_param args = {
		.op = DMABUF_REG_ADD,
		.base = base,
		.size = size,
		.fd = fd,
	};

	return ioctl(reg_fd, DMABUF_REG_IOCTL, &args) ? -errno : 0;
}

/*
 * Remove a dmabuf entry from the registry, using dmabuf fd as the key.
 */
static void dmabuf_reg_remove(int reg_fd, int fd)
{
	struct dmabuf_reg_param args = {
		.op = DMABUF_REG_REMOVE_FD,
		.fd = fd,
	};

	(void) ioctl(reg_fd, DMABUF_REG_IOCTL, &args);
}

/*
 * Check if <addr,size> is covered by a dmabuf entry in the registry. Entries
 * in the registry don't have overlapping ranges. Valid <addr,size> should
 * be within the range of a single entry, or not at all.
 *
 * Return value:
 *
 *  0 ---------- The range is covered, fd is set to the corresponding dmabuf fd
 *  -ENOENT ---- The range is empty in the registry
 *  Others ----- Various errors including: invalid range (e.g. overflow), range
 * 		 partially overlapping with entries in the registry, and I/O error.
 */
static int dmabuf_reg_query(int reg_fd, uint64_t addr, uint64_t size, int *fd)
{
	struct dmabuf_reg_param args = {
		.op = DMABUF_REG_QUERY,
		.base = addr,
		.size = size,
	};

	if (ioctl(reg_fd, DMABUF_REG_IOCTL, &args))
		return -errno;

	*fd = args.fd;
	return 0;
}

/*
 * IPC handles are not cached in old oneAPI L0. Each time a call is made to
 * get the IPC handle, a new dmabuf fd is created. In this case, the fd needs
 * to be explicitly closed when no longer in use to avoid running out of file
 * descriptors.
 *
 * In newer L0 library, IPC handles are cached. The same dmabuf fd is returned
 * for multiple calls to get IPC handles as long as the buffer is the same. As
 * a result, the fd SHOULD NOT be closed explicitly otherwise later use of the
 * fd will fail.
 *
 * By default assume the IPC handle is cached. Not only this is more up-to-date,
 * but also the side effect is less severe if the assumption turns out wrong.
 */
static bool ze_ipc_handle_is_cached = true;

static inline int get_dmabuf_fd(void *buf, size_t len)
{
	static bool first = true;
	void *handle;
	int fd, fd2;
	int err;

	err = ze_hmem_get_handle(buf, len, &handle);
	if (err)
		return err;

	fd = (int)(uintptr_t)handle;
	assert(fd >= 0);

	if (!first)
		goto end;

	err = ze_hmem_get_handle(buf, len, &handle);
	if (err)
		goto end;

	fd2 = (int)(uintptr_t)handle;
	ze_ipc_handle_is_cached = (fd == fd2);

	if (!ze_ipc_handle_is_cached)
		close(fd2);

	first = false;

end:
	return fd;
}

static inline void put_dmabuf_fd(int fd)
{
	if (!ze_ipc_handle_is_cached)
		close(fd);
}

/*
 * If the MR buffer is associated with a dmabuf, get the dmabuf fd and add to
 * the registry.
 *
 * The assumption is that the memory region to be registered is homogeneous:
 * either all system memory or belong to the same dmabuf object. We only need
 * to check the first non-zero iov.
 *
 * Set the fd to -1 if the buffer is not associated with a dmabuf, or failed
 * to be added to the registry.
 */
static void get_mr_fd(struct dmabuf_peer_mem_mr *mr,
		      size_t iov_count, const struct iovec *iov)
{
	int fd = -1;
	int err;
	struct dmabuf_peer_mem_fabric *fab;

	fab = container_of(mr->mr_hook.domain->fabric,
			   struct dmabuf_peer_mem_fabric, fabric_hook);

	mr->fd = -1;

	while (iov_count && !iov->iov_len) {
		iov_count--;
		iov++;
	}

	if (!iov_count)
		goto out;

	err = ze_hmem_get_base_addr(iov->iov_base, iov->iov_len,
				    (void **)&mr->base, (size_t *)&mr->size);
	if (err)
		goto out;

	ofi_mutex_lock(&fab->mutex);

	err = dmabuf_reg_query(fab->dmabuf_reg_fd, mr->base, mr->size, &fd);
	switch (err) {
	case -ENOENT:
		/*
		 * The region is not covered by any entry in the registry, add a
		 * new entry to the registry now.
		 */
		fd = get_dmabuf_fd(iov->iov_base, iov->iov_len);
		if (fd < 0)
			goto out_unlock;

		err = dmabuf_reg_add(fab->dmabuf_reg_fd, mr->base, mr->size, fd);
		if (err) {
			put_dmabuf_fd(fd);
		} else {
			mr->fd = fd;
			FI_INFO(fab->fabric_hook.hprov, FI_LOG_MR,
				"Add new entry: base 0x%"PRIx64" size %"PRIu64" fd %d\n",
				mr->base, mr->size, mr->fd);
		}
		break;

	case 0:
		/*
		 * The region is already covered by an entry in the registry,
		 * add a reference to it.
		 */
		err = dmabuf_reg_add(fab->dmabuf_reg_fd, mr->base, mr->size, fd);
		if (!err)
			mr->fd = fd;
		break;

	default:
		/*
		 * Error happened: range overflow, range conflict with existing
		 * entries, or I/O error.
		 */
		break;
	}

out_unlock:
	ofi_mutex_unlock(&fab->mutex);

out:
	return;
}

static void release_mr_fd(struct dmabuf_peer_mem_mr *mr)
{
	int fd;
	int err;
	struct dmabuf_peer_mem_fabric *fab;

	fab = container_of(mr->mr_hook.domain->fabric,
			   struct dmabuf_peer_mem_fabric, fabric_hook);

	if (mr->fd < 0)
		return;

	/*
	 * Remove this MR's reference to the fd in the kernel registry. The
	 * fd would be removed from the registry if the refcnt reaches 0.
	 * In that case, the fd is no longer used by any MR and should be
	 * released.
	 */
	ofi_mutex_lock(&fab->mutex);
	dmabuf_reg_remove(fab->dmabuf_reg_fd, mr->fd);
	err = dmabuf_reg_query(fab->dmabuf_reg_fd, mr->base, mr->size, &fd);
	if (err == -ENOENT) {
		FI_INFO(fab->fabric_hook.hprov, FI_LOG_MR,
			"Remove entry: base 0x%"PRIx64" size %"PRIu64" fd %d\n",
			mr->base, mr->size, mr->fd);
		put_dmabuf_fd(mr->fd);
	}
	ofi_mutex_unlock(&fab->mutex);
}

static int hook_dmabuf_peer_mem_mr_close(struct fid *fid)
{
	struct dmabuf_peer_mem_mr *mr;

	mr = container_of(fid, struct dmabuf_peer_mem_mr, mr_hook.mr.fid);

	release_mr_fd(mr);

	hook_close(fid);

	return FI_SUCCESS;
}

static struct fi_ops dmabuf_peer_mem_mr_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_dmabuf_peer_mem_mr_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

static int hook_dmabuf_peer_mem_mr_regattr(struct fid *fid,
					   const struct fi_mr_attr *attr,
					   uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *dom;
	struct dmabuf_peer_mem_mr *mymr;
	int ret;

	mymr = calloc(1, sizeof *mymr);
	if (!mymr)
		return -FI_ENOMEM;

	dom = container_of(fid, struct hook_domain, domain.fid);

	mymr->mr_hook.domain = dom;
	mymr->mr_hook.mr.fid.fclass = FI_CLASS_MR;
	mymr->mr_hook.mr.fid.context = attr->context;
	mymr->mr_hook.mr.fid.ops = &dmabuf_peer_mem_mr_fid_ops;

	get_mr_fd(mymr, attr->iov_count, attr->mr_iov);

	if (mymr->fd != -1 && attr->iface == FI_HMEM_SYSTEM)
		((struct fi_mr_attr *)attr)->iface = FI_HMEM_ZE;

	ret = fi_mr_regattr(dom->hdomain, attr, flags, &mymr->mr_hook.hmr);
	if (ret) {
		release_mr_fd(mymr);
		free(mymr);
	} else {
		mymr->mr_hook.mr.mem_desc = mymr->mr_hook.hmr->mem_desc;
		mymr->mr_hook.mr.key = mymr->mr_hook.hmr->key;
		*mr = &mymr->mr_hook.mr;
	}

	return ret;
}

static int hook_dmabuf_peer_mem_mr_regv(struct fid *fid,
					const struct iovec *iov,
					size_t count, uint64_t access,
					uint64_t offset, uint64_t requested_key,
					uint64_t flags, struct fid_mr **mr,
					void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;
	attr.iface = FI_HMEM_SYSTEM;

	return hook_dmabuf_peer_mem_mr_regattr(fid, &attr, flags, mr);
}

static int hook_dmabuf_peer_mem_mr_reg(struct fid *fid, const void *buf,
				       size_t len, uint64_t access,
				       uint64_t offset, uint64_t requested_key,
				       uint64_t flags, struct fid_mr **mr,
				       void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return hook_dmabuf_peer_mem_mr_regv(fid, &iov, 1, access, offset,
					    requested_key, flags, mr, context);
}

static struct fi_ops_mr hook_dmabuf_peer_mem_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = hook_dmabuf_peer_mem_mr_reg,
	.regv = hook_dmabuf_peer_mem_mr_regv,
	.regattr = hook_dmabuf_peer_mem_mr_regattr,
};

static int hook_dmabuf_peer_mem_domain_init(struct fid *fid)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);
	domain->mr = &hook_dmabuf_peer_mem_mr_ops;
	return 0;
}

static int hook_dmabuf_peer_mem_fabric_close(struct fid *fid)
{
	struct dmabuf_peer_mem_fabric *fab;

	fab = container_of(fid, struct dmabuf_peer_mem_fabric, fabric_hook);
	close(fab->dmabuf_reg_fd);
	ofi_mutex_destroy(&fab->mutex);
	hook_close(fid);

	return FI_SUCCESS;
}

static struct fi_ops dmabuf_peer_mem_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_dmabuf_peer_mem_fabric_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

static int hook_dmabuf_peer_mem_fabric(struct fi_fabric_attr *attr,
				       struct fid_fabric **fabric,
				       void *context)
{
        struct fi_provider *hprov = context;
        struct dmabuf_peer_mem_fabric *fab;
	extern struct hook_prov_ctx hook_dmabuf_peer_mem_ctx;
	int fd;

	if (ofi_prov_ctx(hprov)->type != OFI_PROV_CORE) {
		FI_TRACE(hprov, FI_LOG_FABRIC,
			 "Skip installing dmabuf_peer_mem hook\n");
		return -FI_EINVAL;
	}

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing dmabuf_peer_mem hook\n");

	fd = open(dmabuf_reg_dev_name, 0);
	if (fd < 0) {
		FI_WARN(hprov, FI_LOG_FABRIC,
			"Failed to install dmabuf_peer_mem hook: couldn't open %s\n",
			dmabuf_reg_dev_name);
		return -errno;
	}

	fab = calloc(1, sizeof *fab);
	if (!fab) {
		close(fd);
		return -FI_ENOMEM;
	}

	ofi_mutex_init(&fab->mutex);
	fab->dmabuf_reg_fd = fd;
	hook_fabric_init(&fab->fabric_hook, HOOK_DMABUF_PEER_MEM, attr->fabric,
			 hprov, &dmabuf_peer_mem_fabric_fid_ops,
			 &hook_dmabuf_peer_mem_ctx);
	*fabric = &fab->fabric_hook.fabric;
	return 0;
}

struct hook_prov_ctx hook_dmabuf_peer_mem_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_dmabuf_peer_mem",
		.getinfo = NULL,
		.fabric = hook_dmabuf_peer_mem_fabric,
		.cleanup = NULL,
	},
};

HOOK_DMABUF_PEER_MEM_INI
{
#ifdef HAVE_DMABUF_PEER_MEM_DL
	ze_hmem_init();
#endif

	hook_dmabuf_peer_mem_ctx.ini_fid[FI_CLASS_DOMAIN] =
		hook_dmabuf_peer_mem_domain_init;
	return &hook_dmabuf_peer_mem_ctx.prov;
}
