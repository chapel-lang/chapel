/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <asm/types.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <ofi_epoll.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_file.h"

#include "usnic_direct.h"
#include "usdf.h"
#include "usdf_endpoint.h"
#include "usdf_dgram.h"
#include "usdf_av.h"
#include "usdf_cm.h"

/* Given a connection request structure containing data, make a copy of the data
 * that can be accessed in error entries on the EQ. The return value is the size
 * of the data stored in the error entry. If the return value is a non-negative
 * value, then the function has suceeded and the size and output data can be
 * assumed to be valid. If the function fails, then the data will be NULL and
 * the size will be a negative error value.
 */
static int usdf_cm_generate_err_data(struct usdf_eq *eq,
		struct usdf_connreq *crp, void **data)
{
	struct usdf_err_data_entry *err_data_entry;
	struct usdf_connreq_msg *reqp;
	size_t entry_size;
	size_t data_size;

	if (!eq || !crp || !data) {
		USDF_DBG_SYS(EP_CTRL,
				"eq, crp, or data is NULL.\n");
		return -FI_EINVAL;
	}

	/* Initialize to NULL so data can't be used in the error case. */
	*data = NULL;

	reqp = (struct usdf_connreq_msg *) crp->cr_data;

	/* This is a normal case, maybe there was no data. */
	if (!reqp || !reqp->creq_datalen)
		return 0;

	data_size = reqp->creq_datalen;

	entry_size = sizeof(*err_data_entry) + data_size;

	err_data_entry = calloc(1, entry_size);
	if (!err_data_entry) {
		USDF_WARN_SYS(EP_CTRL,
				"failed to allocate err data entry\n");
		return -FI_ENOMEM;
	}

	/* This data should be copied and owned by the provider. Keep
	 * track of it in the EQ, this will be freed in the next EQ read
	 * call after it has been read.
	 */
	memcpy(err_data_entry->err_data, reqp->creq_data, data_size);
	slist_insert_tail(&err_data_entry->entry, &eq->eq_err_data);

	*data = err_data_entry->err_data;

	return data_size;
}

/* Report a connection management related failure. Sometimes there is connection
 * event data that should be copied into the generated event. If the copy_data
 * parameter evaluates to true, then the data will be copied.
 *
 * If data is to be generated for the error entry, then the connection request
 * is assumed to have the data size in host order. If something fails during
 * processing of the error data, then the EQ entry will still be generated
 * without the error data.
 */
void usdf_cm_report_failure(struct usdf_connreq *crp, int error, bool copy_data)
{
	struct fi_eq_err_entry err = {0};
        struct usdf_pep *pep;
        struct usdf_ep *ep;
        struct usdf_eq *eq;
	fid_t fid;
	int ret;

	USDF_DBG_SYS(EP_CTRL, "error=%d (%s)\n", error, fi_strerror(error));

        pep = crp->cr_pep;
        ep = crp->cr_ep;

	if (ep != NULL) {
		fid = ep_utofid(ep);
		eq = ep->ep_eq;
		ep->ep_domain->dom_peer_tab[ep->e.msg.ep_rem_peer_id] = NULL;
	} else {
		fid = pep_utofid(pep);
		eq = pep->pep_eq;
	}

	/* Try to generate the space necessary for the error data. If the
	 * function returns a number greater than or equal to 0, then it was a
	 * success. The return value is the size of the data.
	 */
	if (copy_data) {
		ret = usdf_cm_generate_err_data(eq, crp, &err.err_data);
		if (ret >= 0)
			err.err_data_size = ret;
	}

        err.fid = fid;
        err.err = -error;

        usdf_eq_write_internal(eq, 0, &err, sizeof(err), USDF_EVENT_FLAG_ERROR);
}

/* A wrapper to core function to translate string address to
 * sockaddr_in type. We are expecting a NULL sockaddr_in**.
 * The core function will allocated it for us. The caller HAS TO FREE it.
 */
int usdf_str_toaddr(const char *str, struct sockaddr_in **outaddr)
{
	uint32_t type;
	size_t size;
	int ret;

	type = FI_SOCKADDR_IN;

	/* call the core function. The core always allocate the addr for us. */
	ret = ofi_str_toaddr(str, &type, (void **)outaddr, &size);

#if ENABLE_DEBUG
	char outstr[USDF_ADDR_STR_LEN];
	size_t out_size = USDF_ADDR_STR_LEN;

	inet_ntop(AF_INET, &((*outaddr)->sin_addr), outstr, out_size);
	USDF_DBG_SYS(EP_CTRL,
		    "%s(string) converted to addr :%s:%u(inet)\n",
		    str, outstr, ntohs((*outaddr)->sin_port));
#endif

	return ret;
}

/* A wrapper to core function to translate sockaddr_in address to
 * string. This function is not allocating any memory. We are expected
 * an allocated buffer.
 */
const char *usdf_addr_tostr(const struct sockaddr_in *sin,
			    char *addr_str, size_t *size)
{
	const char *ret;

	ret = ofi_straddr(addr_str, size, FI_SOCKADDR_IN, sin);

#if ENABLE_DEBUG
	char outstr[USDF_ADDR_STR_LEN];
	size_t out_size = USDF_ADDR_STR_LEN;

	inet_ntop(AF_INET, &sin->sin_addr, outstr, out_size);
	USDF_DBG_SYS(EP_CTRL,
		    "%s:%d(inet) converted to %s(string)\n",
		    outstr, ntohs(sin->sin_port), addr_str);
#endif

	return ret;
}

/*
 * Return local address of an EP
 */
static int usdf_cm_copy_name(struct fi_info *info, struct sockaddr_in *sin,
		void *addr, size_t *addrlen)
{
	int ret;
	char addr_str[USDF_ADDR_STR_LEN];
	size_t len;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	ret = FI_SUCCESS;
	switch (info->addr_format) {
	case FI_ADDR_STR:
		len = USDF_ADDR_STR_LEN;
		usdf_addr_tostr(sin, addr_str, &len);
		snprintf(addr, MIN(len, *addrlen), "%s", addr_str);
		break;
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		len = sizeof(*sin);
		memcpy(addr, sin, MIN(len, *addrlen));
		break;
	default:
		return -FI_EINVAL;
	}

	/* If the buffer is too small, tell the user. */
	if (*addrlen < len)
		ret = -FI_ETOOSMALL;

	/* Always return the actual size. */
	*addrlen = len;
	return ret;
}

int usdf_cm_dgram_getname(fid_t fid, void *addr, size_t *addrlen)
{
	int ret;
	struct usdf_ep *ep;
	struct sockaddr_in sin;
	struct fi_info *info;
	socklen_t slen;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	ep = ep_fidtou(fid);
	info = ep->ep_domain->dom_info;

	memset(&sin, 0, sizeof(sin));
	if (ep->e.dg.ep_qp == NULL) {
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr =
			ep->ep_domain->dom_fabric->fab_dev_attrs->uda_ipaddr_be;
		sin.sin_port = 0;
	} else {
		slen = sizeof(sin);
		ret = getsockname(ep->e.dg.ep_sock, (struct sockaddr *)&sin, &slen);
		if (ret == -1) {
			return -errno;
		}
		assert(((struct sockaddr *)&sin)->sa_family == AF_INET);
		assert(slen == sizeof(sin));
		assert(sin.sin_addr.s_addr ==
			ep->ep_domain->dom_fabric->fab_dev_attrs->uda_ipaddr_be);
	}

	return usdf_cm_copy_name(info, &sin, addr, addrlen);
}

/* Checks that the given address is actually a sockaddr_in of appropriate
 * length.  "addr_format" is an FI_ constant like FI_SOCKADDR_IN indicating the
 * claimed type of the given address.
 *
 * Returns true if address is actually a sockaddr_in, false otherwise.
 *
 * Upon successful return, "addr" can be safely cast to either
 * "struct sockaddr_in *" or "struct sockaddr *".
 *
 * "addr" should not be NULL.
 */
bool usdf_cm_addr_is_valid_sin(void *addr, size_t addrlen, uint32_t addr_format)
{
	assert(addr != NULL);

	switch (addr_format) {
	case FI_SOCKADDR_IN:
	case FI_SOCKADDR:
		if (addrlen != sizeof(struct sockaddr_in)) {
			USDF_WARN("addrlen is incorrect\n");
			return false;
		}
		if (((struct sockaddr *)addr)->sa_family != AF_INET) {
			USDF_WARN("unknown/unsupported addr_format\n");
			return false;
		}
		return true;
	default:
		USDF_WARN("unknown/unsupported addr_format\n");
		return false;
	}
}
