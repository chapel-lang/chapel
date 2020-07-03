/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
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

#ifndef _SOCK_UTIL_H_
#define _SOCK_UTIL_H_

#include <sys/mman.h>
#include <rdma/providers/fi_log.h>
#include "sock.h"

extern const char sock_fab_name[];
extern const char sock_dom_name[];
extern const char sock_prov_name[];
extern struct fi_provider sock_prov;
extern int sock_pe_waittime;
extern int sock_conn_timeout;
extern int sock_conn_retry;
extern int sock_cm_def_map_sz;
extern int sock_av_def_sz;
extern int sock_cq_def_sz;
extern int sock_eq_def_sz;
#if ENABLE_DEBUG
extern int sock_dgram_drop_rate;
#endif
extern int sock_keepalive_enable;
extern int sock_keepalive_time;
extern int sock_keepalive_intvl;
extern int sock_keepalive_probes;

#define _SOCK_LOG_DBG(subsys, ...) FI_DBG(&sock_prov, subsys, __VA_ARGS__)
#define _SOCK_LOG_ERROR(subsys, ...) FI_WARN(&sock_prov, subsys, __VA_ARGS__)

static inline int sock_drop_packet(struct sock_ep_attr *ep_attr)
{
#if ENABLE_DEBUG
	if (ep_attr->ep_type == FI_EP_DGRAM && sock_dgram_drop_rate > 0) {
		ep_attr->domain->fab->num_send_msg++;
		if (!(ep_attr->domain->fab->num_send_msg % sock_dgram_drop_rate))
			return 1;
	}
#endif
	return 0;
}

static inline void *sock_mremap(void *old_address, size_t old_size,
				size_t new_size)
{
#ifdef __APPLE__
	return (void *) -1;
#elif defined __FreeBSD__
	return (void *) -1;
#else
	return mremap(old_address, old_size, new_size, 0);
#endif
}

#endif

