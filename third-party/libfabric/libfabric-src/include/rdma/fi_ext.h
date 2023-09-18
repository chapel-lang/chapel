/*
 * Copyright (c) 2021 Intel Corporation. All rights reserved.
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
 * Peer provider AV support.
 */
struct fid_peer_av;

struct fi_ops_av_owner {
	size_t	size;
	int	(*query)(struct fid_peer_av *av, struct fi_av_attr *attr);
	fi_addr_t (*ep_addr)(struct fid_peer_av *av, struct fid_ep *ep);
};

struct fid_peer_av {
	struct fid fid;
	struct fi_ops_av_owner *owner_ops;
};

struct fi_peer_av_context {
	size_t size;
	struct fid_peer_av *av;
};


/*
 * Peer provider AV set support.
 */
struct fid_peer_av_set;

struct fi_ops_av_set_owner {
	size_t	size;
	int	(*members)(struct fid_peer_av_set *av, fi_addr_t *addr,
			   size_t *count);
};

struct fid_peer_av_set {
	struct fid fid;
	struct fi_ops_av_set_owner *owner_ops;
};

struct fi_peer_av_set_context {
	size_t size;
	struct fi_peer_av_set *av_set;
};


/*
 * Peer provider CQ support.
 */
struct fid_peer_cq;

struct fi_ops_cq_owner {
	size_t	size;
	ssize_t (*write)(struct fid_peer_cq *cq, void *context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag,
			fi_addr_t src);
	ssize_t	(*writeerr)(struct fid_peer_cq *cq,
			const struct fi_cq_err_entry *err_entry);
};

struct fid_peer_cq {
	struct fid fid;
	struct fi_ops_cq_owner *owner_ops;
};

struct fi_peer_cq_context {
	size_t size;
	struct fid_peer_cq *cq;
};


/*
 * Peer provider domain support.
 */
struct fi_peer_domain_context {
	size_t size;
	struct fid_domain *domain;
};


/*
 * Peer provider EQ support.
 */
struct fi_peer_eq_context {
	size_t size;
	struct fid_eq *eq;
};


/*
 * Peer shared rx context
 */
struct fid_peer_srx;

/* Castable to dlist_entry */
struct fi_peer_rx_entry {
	struct fi_peer_rx_entry *next;
	struct fi_peer_rx_entry *prev;
	struct fid_peer_srx *srx;
	fi_addr_t addr;
	size_t size;
	uint64_t tag;
	uint64_t flags;
	void *context;
	size_t count;
	void **desc;
	void *peer_context;
	void *owner_context;
	struct iovec *iov;
};

struct fi_ops_srx_owner {
	size_t	size;
	int	(*get_msg)(struct fid_peer_srx *srx, fi_addr_t addr,
			size_t size, struct fi_peer_rx_entry **entry);
	int	(*get_tag)(struct fid_peer_srx *srx, fi_addr_t addr,
			uint64_t tag, struct fi_peer_rx_entry **entry);
	int	(*queue_msg)(struct fi_peer_rx_entry *entry);
	int	(*queue_tag)(struct fi_peer_rx_entry *entry);

	void	(*free_entry)(struct fi_peer_rx_entry *entry);
};

struct fi_ops_srx_peer {
	size_t	size;
	int	(*start_msg)(struct fi_peer_rx_entry *entry);
	int	(*start_tag)(struct fi_peer_rx_entry *entry);
	int	(*discard_msg)(struct fi_peer_rx_entry *entry);
	int	(*discard_tag)(struct fi_peer_rx_entry *entry);
};

struct fid_peer_srx {
	struct fid_ep ep_fid;
	struct fi_ops_srx_owner *owner_ops;
	struct fi_ops_srx_peer *peer_ops;
};

struct fi_peer_srx_context {
	size_t size;
	struct fid_peer_srx *srx;
};


/*
 * Peer transfers
 */
struct fi_peer_transfer_context;

struct fi_ops_transfer_peer {
	size_t size;
	ssize_t	(*complete)(struct fid_ep *ep, struct fi_cq_tagged_entry *buf,
			    fi_addr_t src_addr);
	ssize_t	(*comperr)(struct fid_ep *ep, struct fi_cq_err_entry *buf);
};

struct fi_peer_transfer_context {
	size_t size;
	struct fi_info *info;
	struct fid_ep *ep;
	struct fi_ops_transfer_peer *peer_ops;
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

	return fi_import(version, "logging", NULL, 0, flags, &log_fid->fid, log_fid);
}

#ifdef __cplusplus
}
#endif

#endif /* FI_EXT_H */
