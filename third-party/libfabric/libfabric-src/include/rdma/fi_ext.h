/*
 * Copyright (c) 2021-2023 Intel Corporation. All rights reserved.
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates. All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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
#include <rdma/fi_eq.h>
#include <rdma/fi_endpoint.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/providers/fi_log.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Each provider needs to define an unique 12-bit provider
 * specific code to avoid overlapping with other providers,
 * then bit left shift the code 16 bits. Note that the
 * highest 4 bits are not touched, so they are still left
 * to 0. The lowest 16 bits can be used to define provider
 * specific values. E.g.,
 *
 * define FI_PROV_SPECIFIC_XXX    (0xabc << 16)
 *
 * enum {
 *        FI_PROV_XXX_FOO = -(FI_PROV_SPECIFIC_XXX),
 *        FI_PROV_XXX_BAR,
 * }
 */

#define FI_PROV_SPECIFIC_EFA   (0xefa << 16)
#define FI_PROV_SPECIFIC_TCP   (0x7cb << 16)


/* negative options are provider specific */
enum {
	FI_OPT_EFA_RNR_RETRY = -FI_PROV_SPECIFIC_EFA,
	FI_OPT_EFA_EMULATED_READ,       /* bool */
	FI_OPT_EFA_EMULATED_WRITE,      /* bool */
	FI_OPT_EFA_EMULATED_ATOMICS,    /* bool */
	FI_OPT_EFA_USE_DEVICE_RDMA,	/* bool */
	FI_OPT_EFA_SENDRECV_IN_ORDER_ALIGNED_128_BYTES, /* bool */
	FI_OPT_EFA_WRITE_IN_ORDER_ALIGNED_128_BYTES, /* bool */
};

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


/*
 * System logging import extension:
 * To use, open logging fid and import.
 */

#define FI_LOG_PROV_FILTERED (1ULL << 0) /* Filter provider */

struct fi_ops_log {
	size_t size;
	int (*enabled)(const struct fi_provider *prov, enum fi_log_level level,
		       enum fi_log_subsys subsys, uint64_t flags);
	int (*ready)(const struct fi_provider *prov, enum fi_log_level level,
		     enum fi_log_subsys subsys, uint64_t flags, uint64_t *showtime);
	void (*log)(const struct fi_provider *prov, enum fi_log_level level,
		    enum fi_log_subsys subsys, const char *func, int line,
		    const char *msg);
};

struct fid_logging {
	struct fid          fid;
	struct fi_ops_log   *ops;
};

static inline int fi_import(uint32_t version, const char *name, void *attr,
			    size_t attr_len, uint64_t flags, struct fid *fid,
			    void *context)
{
	struct fid *open_fid;
	int ret;

	ret = fi_open(version, name, attr, attr_len, flags, &open_fid, context);
	if (ret != FI_SUCCESS)
	    return ret;

	ret = fi_import_fid(open_fid, fid, flags);
	fi_close(open_fid);
	return ret;
}

static inline int fi_import_log(uint32_t version, uint64_t flags,
				struct fid_logging *log_fid)
{
	log_fid->fid.fclass = FI_CLASS_LOG;
	log_fid->ops->size = sizeof(struct fi_ops_log);

	return fi_import(version, "logging", NULL, 0, flags, &log_fid->fid,
			 log_fid);
}

#ifdef __cplusplus
}
#endif

#endif /* FI_EXT_H */
