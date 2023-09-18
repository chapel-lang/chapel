/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
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

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "ofi_prov.h"
#include "ofi_net.h"

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_FABRIC, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_FABRIC, __VA_ARGS__)

int sock_pe_waittime = SOCK_PE_WAITTIME;
const char sock_fab_name[] = "IP";
const char sock_dom_name[] = "sockets";
const char sock_prov_name[] = "sockets";
int sock_conn_timeout = SOCK_CM_DEF_TIMEOUT;
int sock_conn_retry = SOCK_CM_DEF_RETRY;
int sock_cm_def_map_sz = SOCK_CMAP_DEF_SZ;
int sock_av_def_sz = SOCK_AV_DEF_SZ;
int sock_cq_def_sz = SOCK_CQ_DEF_SZ;
int sock_eq_def_sz = SOCK_EQ_DEF_SZ;
#if ENABLE_DEBUG
int sock_dgram_drop_rate = 0;
#endif
int sock_keepalive_enable;
int sock_keepalive_time = INT_MAX;
int sock_keepalive_intvl = INT_MAX;
int sock_keepalive_probes = INT_MAX;
int sock_buf_sz = 0;

static struct dlist_entry sock_fab_list;
static struct dlist_entry sock_dom_list;
static ofi_mutex_t sock_list_lock;
static int read_default_params;

void sock_dom_add_to_list(struct sock_domain *domain)
{
	ofi_mutex_lock(&sock_list_lock);
	dlist_insert_tail(&domain->dom_list_entry, &sock_dom_list);
	ofi_mutex_unlock(&sock_list_lock);
}

static inline int sock_dom_check_list_internal(struct sock_domain *domain)
{
	struct dlist_entry *entry;
	struct sock_domain *dom_entry;
	for (entry = sock_dom_list.next; entry != &sock_dom_list;
	     entry = entry->next) {
		dom_entry = container_of(entry, struct sock_domain,
					 dom_list_entry);
		if (dom_entry == domain)
			return 1;
	}
	return 0;
}

int sock_dom_check_list(struct sock_domain *domain)
{
	int found;
	ofi_mutex_lock(&sock_list_lock);
	found = sock_dom_check_list_internal(domain);
	ofi_mutex_unlock(&sock_list_lock);
	return found;
}

void sock_dom_remove_from_list(struct sock_domain *domain)
{
	ofi_mutex_lock(&sock_list_lock);
	if (sock_dom_check_list_internal(domain))
		dlist_remove(&domain->dom_list_entry);

	ofi_mutex_unlock(&sock_list_lock);
}

struct sock_domain *sock_dom_list_head(void)
{
	struct sock_domain *domain;
	ofi_mutex_lock(&sock_list_lock);
	if (dlist_empty(&sock_dom_list)) {
		domain = NULL;
	} else {
		domain = container_of(sock_dom_list.next,
				      struct sock_domain, dom_list_entry);
	}
	ofi_mutex_unlock(&sock_list_lock);
	return domain;
}

int sock_dom_check_manual_progress(struct sock_fabric *fabric)
{
	struct dlist_entry *entry;
	struct sock_domain *dom_entry;
	for (entry = sock_dom_list.next; entry != &sock_dom_list;
	     entry = entry->next) {
		dom_entry = container_of(entry, struct sock_domain,
					 dom_list_entry);
		if (dom_entry->fab == fabric &&
		    dom_entry->progress_mode == FI_PROGRESS_MANUAL)
			return 1;
	}
	return 0;
}

void sock_fab_add_to_list(struct sock_fabric *fabric)
{
	ofi_mutex_lock(&sock_list_lock);
	dlist_insert_tail(&fabric->fab_list_entry, &sock_fab_list);
	ofi_mutex_unlock(&sock_list_lock);
}

static inline int sock_fab_check_list_internal(struct sock_fabric *fabric)
{
	struct dlist_entry *entry;
	struct sock_fabric *fab_entry;
	for (entry = sock_fab_list.next; entry != &sock_fab_list;
	     entry = entry->next) {
		fab_entry = container_of(entry, struct sock_fabric,
					 fab_list_entry);
		if (fab_entry == fabric)
			return 1;
	}
	return 0;
}

int sock_fab_check_list(struct sock_fabric *fabric)
{
	int found;
	ofi_mutex_lock(&sock_list_lock);
	found = sock_fab_check_list_internal(fabric);
	ofi_mutex_unlock(&sock_list_lock);
	return found;
}

void sock_fab_remove_from_list(struct sock_fabric *fabric)
{
	ofi_mutex_lock(&sock_list_lock);
	if (sock_fab_check_list_internal(fabric))
		dlist_remove(&fabric->fab_list_entry);

	ofi_mutex_unlock(&sock_list_lock);
}

struct sock_fabric *sock_fab_list_head(void)
{
	struct sock_fabric *fabric;
	ofi_mutex_lock(&sock_list_lock);
	if (dlist_empty(&sock_fab_list)) {
		fabric = NULL;
	} else {
		fabric = container_of(sock_fab_list.next,
				      struct sock_fabric, fab_list_entry);
	}
	ofi_mutex_unlock(&sock_list_lock);
	return fabric;
}

static int sock_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	/* we're always ready to wait! */
	return 0;
}

static struct fi_ops_fabric sock_fab_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = sock_domain,
	.passive_ep = sock_msg_passive_ep,
	.eq_open = sock_eq_open,
	.wait_open = sock_wait_open,
	.trywait = sock_trywait
};

static int sock_fabric_close(fid_t fid)
{
	struct sock_fabric *fab;
	fab = container_of(fid, struct sock_fabric, fab_fid);
	if (ofi_atomic_get32(&fab->ref))
		return -FI_EBUSY;

	sock_fab_remove_from_list(fab);
	ofi_mutex_destroy(&fab->lock);
	free(fab);
	return 0;
}

static struct fi_ops sock_fab_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static void sock_read_default_params()
{
	if (!read_default_params) {
		fi_param_get_int(&sock_prov, "pe_waittime", &sock_pe_waittime);
		fi_param_get_int(&sock_prov, "conn_timeout", &sock_conn_timeout);
		fi_param_get_int(&sock_prov, "max_conn_retry", &sock_conn_retry);
		fi_param_get_int(&sock_prov, "def_conn_map_sz", &sock_cm_def_map_sz);
		fi_param_get_int(&sock_prov, "def_av_sz", &sock_av_def_sz);
		fi_param_get_int(&sock_prov, "def_cq_sz", &sock_cq_def_sz);
		fi_param_get_int(&sock_prov, "def_eq_sz", &sock_eq_def_sz);
#if ENABLE_DEBUG
		fi_param_get_int(&sock_prov, "dgram_drop_rate", &sock_dgram_drop_rate);
#endif
		fi_param_get_bool(&sock_prov, "keepalive_enable", &sock_keepalive_enable);
		fi_param_get_int(&sock_prov, "keepalive_time", &sock_keepalive_time);
		fi_param_get_int(&sock_prov, "keepalive_intvl", &sock_keepalive_intvl);
		fi_param_get_int(&sock_prov, "keepalive_probes", &sock_keepalive_probes);
		fi_param_get_int(&sock_prov, "max_buf_sz", &sock_buf_sz);

		read_default_params = 1;
	}
}

static int sock_fabric(struct fi_fabric_attr *attr,
		       struct fid_fabric **fabric, void *context)
{
	struct sock_fabric *fab;

	fab = calloc(1, sizeof(*fab));
	if (!fab)
		return -FI_ENOMEM;

	sock_read_default_params();

	ofi_mutex_init(&fab->lock);
	dlist_init(&fab->service_list);

	fab->fab_fid.fid.fclass = FI_CLASS_FABRIC;
	fab->fab_fid.fid.context = context;
	fab->fab_fid.fid.ops = &sock_fab_fi_ops;
	fab->fab_fid.ops = &sock_fab_ops;
	*fabric = &fab->fab_fid;
	ofi_atomic_initialize32(&fab->ref, 0);
#if ENABLE_DEBUG
	fab->num_send_msg = 0;
#endif
	sock_fab_add_to_list(fab);
	return 0;
}

int sock_get_src_addr(union ofi_sock_ip *dest_addr,
		      union ofi_sock_ip *src_addr)
{
	int sock, ret;
	socklen_t len;

	sock = ofi_socket(dest_addr->sa.sa_family, SOCK_DGRAM, 0);
	if (sock < 0)
		return -ofi_sockerr();

	len = (socklen_t) ofi_sizeofaddr(&dest_addr->sa);
	ret = connect(sock, &dest_addr->sa, len);
	if (ret) {
		SOCK_LOG_DBG("Failed to connect udp socket\n");
		ret = sock_get_src_addr_from_hostname(src_addr, NULL,
						      dest_addr->sa.sa_family);
		goto out;
	}

	ret = getsockname(sock, &src_addr->sa, &len);
	ofi_addr_set_port(&src_addr->sa, 0);
	if (ret) {
		SOCK_LOG_DBG("getsockname failed\n");
		ret = -ofi_sockerr();
	}

out:
	ofi_close_socket(sock);
	return ret;
}

static int sock_getinfo(uint32_t version, const char *node, const char *service,
			uint64_t flags, const struct fi_info *hints,
			struct fi_info **info)
{
	return ofi_ip_getinfo(&sock_util_prov, version, node, service, flags,
			      hints, info);
}

static void fi_sockets_fini(void)
{
	ofi_mutex_destroy(&sock_list_lock);
}

struct fi_provider sock_prov = {
	.name = sock_prov_name,
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = sock_getinfo,
	.fabric = sock_fabric,
	.cleanup = fi_sockets_fini
};

struct util_prov sock_util_prov = {
	.prov = &sock_prov,
	.info = &sock_dgram_info,
	.flags = 0,
};

SOCKETS_INI
{
#if HAVE_SOCKETS_DL
	ofi_pmem_init();
#endif

	fi_param_define(&sock_prov, "pe_waittime", FI_PARAM_INT,
			"How many milliseconds to spin while waiting for progress");

	fi_param_define(&sock_prov, "conn_timeout", FI_PARAM_INT,
			"How many milliseconds to wait for one connection establishment");

	fi_param_define(&sock_prov, "max_conn_retry", FI_PARAM_INT,
			"Number of connection retries before reporting as failure");

	fi_param_define(&sock_prov, "def_conn_map_sz", FI_PARAM_INT,
			"Default connection map size");

	fi_param_define(&sock_prov, "def_av_sz", FI_PARAM_INT,
			"Default address vector size");

	fi_param_define(&sock_prov, "def_cq_sz", FI_PARAM_INT,
			"Default completion queue size");

	fi_param_define(&sock_prov, "def_eq_sz", FI_PARAM_INT,
			"Default event queue size");

	fi_param_define(&sock_prov, "pe_affinity", FI_PARAM_STRING,
			"If specified, bind the progress thread to the indicated range(s) of Linux virtual processor ID(s). "
			"This option is currently not supported on OS X and Windows. Usage: id_start[-id_end[:stride]][,]");

	fi_param_define(&sock_prov, "keepalive_enable", FI_PARAM_BOOL,
			"Enable keepalive support");

	fi_param_define(&sock_prov, "keepalive_time", FI_PARAM_INT,
			"Idle time in seconds before sending the first keepalive probe");

	fi_param_define(&sock_prov, "keepalive_intvl", FI_PARAM_INT,
			"Time in seconds between individual keepalive probes");

	fi_param_define(&sock_prov, "keepalive_probes", FI_PARAM_INT,
			"Maximum number of keepalive probes sent before dropping the connection");

	fi_param_define(&sock_prov, "iface", FI_PARAM_STRING,
			"Specify interface name");

	fi_param_define(&sock_prov, "max_buf_sz", FI_PARAM_INT,
                        "Maximum socket send and recv buffer in bytes (i.e. SO_RCVBUF, SO_SNDBUF)");

	ofi_mutex_init(&sock_list_lock);
	dlist_init(&sock_fab_list);
	dlist_init(&sock_dom_list);
#if ENABLE_DEBUG
	fi_param_define(&sock_prov, "dgram_drop_rate", FI_PARAM_INT,
			"Drop every Nth dgram frame (debug only)");
#endif
	return &sock_prov;
}
