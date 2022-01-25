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

#ifndef FI_EXT_H
#define FI_EXT_H

#include <stdbool.h>
#include <rdma/fabric.h>


#ifdef __cplusplus
extern "C" {
#endif


struct fi_fid_export {
	struct fid **fid;
	uint64_t flags;
	void *context;
};

static inline int
fi_export_fid(struct fid *fid, uint64_t flags,
	      struct fid **expfid, void *context)
{
	struct fi_fid_export exp;

	exp.fid = expfid;
	exp.flags = flags;
	exp.context = context;
	return fi_control(fid, FI_EXPORT_FID, &exp);
}

static inline int
fi_import_fid(struct fid *fid, struct fid *expfid, uint64_t flags)
{
	return fid->ops->bind(fid, expfid, flags);
}


/*
 * System memory monitor import extension:
 * To use, open mr_cache fid and import.
 */

struct fid_mem_monitor;

struct fi_ops_mem_monitor {
	size_t	size;
	int	(*start)(struct fid_mem_monitor *monitor);
	void	(*stop)(struct fid_mem_monitor *monitor);
	int	(*subscribe)(struct fid_mem_monitor *monitor,
			const void *addr, size_t len);
	void	(*unsubscribe)(struct fid_mem_monitor *monitor,
			const void *addr, size_t len);
	bool	(*valid)(struct fid_mem_monitor *monitor,
			const void *addr, size_t len);
};

struct fi_ops_mem_notify {
	size_t	size;
	void	(*notify)(struct fid_mem_monitor *monitor, const void *addr,
			size_t len);
};

struct fid_mem_monitor {
	struct fid fid;
	struct fi_ops_mem_monitor *export_ops;
	struct fi_ops_mem_notify *import_ops;
};


#ifdef __cplusplus
}
#endif

#endif /* FI_EXT_H */
