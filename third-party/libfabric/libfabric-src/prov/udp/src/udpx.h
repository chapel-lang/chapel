/*
 * Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
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
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_signal.h>
#include <ofi_util.h>

#ifndef _UDPX_H_
#define _UDPX_H_


extern struct fi_provider udpx_prov;
extern struct util_prov udpx_util_prov;
extern struct fi_info udpx_info;


int udpx_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context);
int udpx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context);
int udpx_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);


#define UDPX_FLAG_MULTI_RECV	1
#define UDPX_IOV_LIMIT		4

struct udpx_ep_entry {
	void			*context;
	struct iovec		iov[UDPX_IOV_LIMIT];
	uint8_t			iov_count;
	uint8_t			flags;
	uint8_t			resv[sizeof(size_t) - 2];
};

OFI_DECLARE_CIRQUE(struct udpx_ep_entry, udpx_rx_cirq);

struct udpx_ep;
typedef void (*udpx_rx_comp_func)(struct udpx_ep *ep, void *context,
		uint64_t flags, size_t len, void *buf, void *addr);
typedef void (*udpx_tx_comp_func)(struct udpx_ep *ep, void *context);

struct udpx_ep {
	struct util_ep		util_ep;
	udpx_rx_comp_func	rx_comp;
	udpx_tx_comp_func	tx_comp;
	struct udpx_rx_cirq	*rxq;    /* protected by rx_cq lock */
	SOCKET			sock;
	int			is_bound;
	ofi_atomic32_t		ref;
};

int udpx_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);


int udpx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context);


struct udpx_mc {
	struct fid_mc		mc_fid;
	union {
		struct sockaddr_in	sin;
		struct sockaddr_in6	sin6;
	} addr;
	struct udpx_ep		*ep;
};


#endif
