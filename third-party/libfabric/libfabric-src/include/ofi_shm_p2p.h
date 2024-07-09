/*
 * (C) Copyright 2023 UT-Battelle, LLC. All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <ofi_cma.h>
#include <ofi_xpmem.h>
#include <ofi.h>
#include <ofi_iov.h>

#ifndef _OFI_SHM_P2P_H_
#define _OFI_SHM_P2P_H_

enum ofi_shm_p2p_type {
	FI_SHM_P2P_XPMEM,
	FI_SHM_P2P_CMA,
	FI_SHM_P2P_DSA,
};

struct ofi_shm_p2p_ops {
	bool initialized;
	int (*init)(void);
	int (*cleanup)(void);
	int (*copy)(struct iovec *local, unsigned long local_cnt,
		    struct iovec *remote, unsigned long remote_cnt,
		    size_t total, pid_t pid, bool write, void *user_data);
};

static inline int ofi_shm_p2p_no_init(void)
{
	return FI_SUCCESS;
}

static inline int ofi_shm_p2p_no_cleanup(void)
{
	return FI_SUCCESS;
}

static inline int
ofi_shm_p2p_no_copy(struct iovec *local, unsigned long local_cnt,
		    struct iovec *remote, unsigned long remote_cnt,
		    size_t total, pid_t pid, bool write, void *user_data)
{
	return -FI_ENOSYS;
}

static struct ofi_shm_p2p_ops p2p_ops[] = {
	[FI_SHM_P2P_XPMEM] = {
		.initialized = false,
		.init = xpmem_init,
		.cleanup = xpmem_cleanup,
		.copy = xpmem_copy,
	},
	[FI_SHM_P2P_CMA] = {
		.initialized = false,
		.init = ofi_shm_p2p_no_init,
		.cleanup = ofi_shm_p2p_no_cleanup,
		.copy = cma_copy,
	},
	[FI_SHM_P2P_DSA] = {
		.initialized = false,
		.init = ofi_shm_p2p_no_init,
		.cleanup = ofi_shm_p2p_no_cleanup,
		.copy = ofi_shm_p2p_no_copy,
	},
};

static inline void ofi_shm_p2p_init(void)
{
	int rc;
	enum ofi_shm_p2p_type p2p_type;

	for (p2p_type = 0; p2p_type < ARRAY_SIZE(p2p_ops); p2p_type++) {
		rc = p2p_ops[p2p_type].init();
		if (!rc)
			p2p_ops[p2p_type].initialized = true;
	}
}

static inline void ofi_shm_p2p_cleanup(void)
{
	int rc;
	enum ofi_shm_p2p_type p2p_type;

	for (p2p_type = 0; p2p_type < ARRAY_SIZE(p2p_ops); p2p_type++) {
		rc = p2p_ops[p2p_type].cleanup();
		if (!rc)
			p2p_ops[p2p_type].initialized = false;
	}
}

static inline int
ofi_shm_p2p_copy(enum ofi_shm_p2p_type p2p_type, struct iovec *local,
		 unsigned long local_cnt, struct iovec *remote,
		 unsigned long remote_cnt, size_t total, int64_t id,
		 bool write, void *user_data)
{
	return p2p_ops[p2p_type].copy(local, local_cnt, remote,
				      remote_cnt, total, id, write, user_data);
}

#endif /* _OFI_SHM_P2P_H_ */

