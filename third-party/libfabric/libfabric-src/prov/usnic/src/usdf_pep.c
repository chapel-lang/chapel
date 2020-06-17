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
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ofi_epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_file.h"

#include "fi_ext_usnic.h"
#include "usnic_direct.h"
#include "usd.h"
#include "usdf.h"
#include "usdf_endpoint.h"
#include "usdf_cm.h"

static int
usdf_pep_bind(fid_t fid, fid_t bfid, uint64_t flags)
{
	struct usdf_pep *pep;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	pep = pep_fidtou(fid);

	switch (bfid->fclass) {

	case FI_CLASS_EQ:
		if (pep->pep_eq != NULL) {
			return -FI_EINVAL;
		}
		pep->pep_eq = eq_fidtou(bfid);
		ofi_atomic_inc32(&pep->pep_eq->eq_refcnt);
		break;

	default:
		return -FI_EINVAL;
	}

	return 0;
}

static struct fi_info *
usdf_pep_conn_info(struct usdf_connreq *crp)
{
	struct fi_info *ip;
	struct usdf_pep *pep;
	struct sockaddr_in *sin;
	struct usdf_connreq_msg *reqp;

	pep = crp->cr_pep;
	reqp = (struct usdf_connreq_msg *)crp->cr_data;

	ip = fi_dupinfo(pep->pep_info);
	if (!ip) {
		USDF_WARN_SYS(EP_CTRL, "failed to duplicate pep info\n");
		return NULL;
	}

	/* fill in dest addr */
	ip->dest_addrlen = ip->src_addrlen;
	sin = calloc(1, ip->dest_addrlen);
	if (sin == NULL) {
		goto fail;
	}
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = reqp->creq_ipaddr;
	sin->sin_port = reqp->creq_port;

	ip->dest_addr = usdf_sin_to_format(pep->pep_info, sin,
					   &ip->dest_addrlen);

	ip->handle = (fid_t) crp;
	return ip;
fail:
	fi_freeinfo(ip);
	return NULL;
}

/*
 * Remove connection request from epoll list if not done already.
 * crp->cr_pollitem.pi_rtn is non-NULL when epoll() is active
 */
static int
usdf_pep_creq_epoll_del(struct usdf_connreq *crp)
{
	int ret;
	struct usdf_pep *pep;

	pep = crp->cr_pep;

	if (crp->cr_pollitem.pi_rtn != NULL) {
		ret = ofi_epoll_del(pep->pep_fabric->fab_epollfd,
				    crp->cr_sockfd);
		crp->cr_pollitem.pi_rtn = NULL;
		if (ret != 0) {
			ret = -errno;
		}
	} else {
		ret = 0;
	}
	return ret;
}

static int
usdf_pep_read_connreq(void *v)
{
	struct usdf_connreq *crp;
	struct usdf_pep *pep;
	struct usdf_connreq_msg *reqp;
	struct fi_eq_cm_entry *entry;
	size_t entry_len;
	int ret;
	int n;

	crp = v;
	pep = crp->cr_pep;

	n = read(crp->cr_sockfd, crp->cr_ptr, crp->cr_resid);
	if (n == -1) {
		ret = -errno;
		goto report_failure_skip_data;
	}

	crp->cr_ptr += n;
	crp->cr_resid -= n;

	reqp = (struct usdf_connreq_msg *)crp->cr_data;

	if (crp->cr_resid == 0 && crp->cr_ptr == crp->cr_data + sizeof(*reqp)) {
		reqp->creq_datalen = ntohl(reqp->creq_datalen);
		crp->cr_resid = reqp->creq_datalen;
	}

	/* if resid is 0 now, completely done */
	if (crp->cr_resid == 0) {
		ret = usdf_pep_creq_epoll_del(crp);
		if (ret != 0)
			goto report_failure_skip_data;

		/* create CONNREQ EQ entry */
		entry_len = sizeof(*entry) + reqp->creq_datalen;
		entry = malloc(entry_len);
		if (entry == NULL) {
			ret = -errno;
			goto report_failure_skip_data;
		}

		entry->fid = &pep->pep_fid.fid;
		entry->info = usdf_pep_conn_info(crp);
		if (entry->info == NULL) {
			ret = -FI_ENOMEM;
			goto free_entry_and_report_failure;
		}

		memcpy(entry->data, reqp->creq_data, reqp->creq_datalen);
		ret = usdf_eq_write_internal(pep->pep_eq, FI_CONNREQ, entry,
				entry_len, 0);

		if (ret != (int)entry_len)
			goto free_entry_and_report_failure;

		free(entry);
	}

	return 0;

free_entry_and_report_failure:
	free(entry);
report_failure_skip_data:
	usdf_cm_report_failure(crp, ret, false);
	return 0;
}

static int
usdf_pep_listen_cb(void *v)
{
	struct usdf_pep *pep;
	struct sockaddr_in sin;
	struct usdf_connreq *crp;
	socklen_t socklen;
	int ret;
	int s;

	pep = v;

	socklen = sizeof(sin);
	s = accept(pep->pep_sock, &sin, &socklen);
	if (s == -1) {
		/* ignore early failure */
		return 0;
	}
	crp = NULL;
	pthread_spin_lock(&pep->pep_cr_lock);
	if (!TAILQ_EMPTY(&pep->pep_cr_free)) {
		crp = TAILQ_FIRST(&pep->pep_cr_free);
		TAILQ_REMOVE_MARK(&pep->pep_cr_free, crp, cr_link);
		TAILQ_NEXT(crp, cr_link) = NULL;
	}
	pthread_spin_unlock(&pep->pep_cr_lock);

	/* no room for request, just drop it */
	if (crp == NULL) {
		/* XXX send response? */
		close(s);
		return 0;
	}

	crp->cr_sockfd = s;
	crp->cr_pep = pep;
	crp->cr_ptr = crp->cr_data;
	crp->cr_resid = sizeof(struct usdf_connreq_msg);

	crp->cr_pollitem.pi_rtn = usdf_pep_read_connreq;
	crp->cr_pollitem.pi_context = crp;

	ret = ofi_epoll_add(pep->pep_fabric->fab_epollfd, crp->cr_sockfd,
			    OFI_EPOLL_IN, &crp->cr_pollitem);
	if (ret) {
		usdf_cm_report_failure(crp, ret, false);
		return 0;
	}

	TAILQ_INSERT_TAIL(&pep->pep_cr_pending, crp, cr_link);

	return 0;
}

static int
usdf_pep_listen(struct fid_pep *fpep)
{
	struct usdf_pep *pep;
	struct usdf_fabric *fp;
	struct sockaddr_in *sin;
	socklen_t socklen;
	int ret;
	bool addr_format_str;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	pep = pep_ftou(fpep);
	fp = pep->pep_fabric;
	addr_format_str = (pep->pep_info->addr_format == FI_ADDR_STR);
	sin = NULL;

	switch (pep->pep_state) {
	case USDF_PEP_UNBOUND:
	case USDF_PEP_BOUND:
		break;
	case USDF_PEP_LISTENING:
		USDF_WARN_SYS(EP_CTRL, "PEP already LISTENING!\n");
		return -FI_EOPBADSTATE;
	case USDF_PEP_ROBBED:
		USDF_WARN_SYS(EP_CTRL,
			"PEP already consumed, you may only fi_close() now\n");
		return -FI_EOPBADSTATE;
	default:
		USDF_WARN_SYS(EP_CTRL, "unhandled case! (%d)\n",
			      pep->pep_state);
		abort();
	}

	/* we could already be bound if the user called fi_setname() or if we
	 * already did the bind in a previous call to usdf_pep_listen() and the
	 * listen(2) call failed */
	if (pep->pep_state == USDF_PEP_UNBOUND) {
		sin = usdf_format_to_sin(pep->pep_info, &pep->pep_src_addr);
		if (sin == NULL)
			goto fail;

		ret = bind(pep->pep_sock, sin, sizeof(struct sockaddr_in));
		if (ret == -1) {
			goto fail;
		}

		/* Get the actual port (since we may have requested
		 * port 0)
		 */
		socklen = sizeof(*sin);
		ret = getsockname(pep->pep_sock, sin,
				&socklen);
		if (ret == -1)
			goto fail;

		/* If it's FI_ADDR_STR, we have to update the string
		 * with this method. (FI_SOCKADDR_IN got taken care of, above)
		*/
		if (addr_format_str) {
			pep->pep_info->src_addrlen = USDF_ADDR_STR_LEN;
			usdf_addr_tostr(sin, pep->pep_src_addr.addr_str,
					&pep->pep_info->src_addrlen);
		}

		/* Update the state to bound. */
		pep->pep_state = USDF_PEP_BOUND;
	}

	ret = listen(pep->pep_sock, pep->pep_backlog);
	if (ret != 0) {
		goto fail;
	}
	pep->pep_state = USDF_PEP_LISTENING;

	pep->pep_pollitem.pi_rtn = usdf_pep_listen_cb;
	pep->pep_pollitem.pi_context = pep;
	ret = ofi_epoll_add(fp->fab_epollfd, pep->pep_sock, OFI_EPOLL_IN,
			    &pep->pep_pollitem);
	if (ret) {
		errno = -ret;
		goto fail;
	}

	return 0;

fail:
	usdf_free_sin_if_needed(pep->pep_info, sin);

	return -errno;
}

/* Register as a callback triggered by the socket becoming writeable. Write as
 * much data as can be written in a single write, and keep track of how much
 * data is left. If the data is not fully written, it will finish getting
 * written in another iteration of the progression.
 */
static int usdf_pep_reject_async(void *vreq)
{
	struct usdf_connreq *crp;
	int ret;

	crp = vreq;

	do {
		ret = write(crp->cr_sockfd, crp->cr_ptr, crp->cr_resid);
	} while ((ret < 0) && (errno == EINTR));

	if ((ret <= 0) && (errno != EAGAIN)) {
		USDF_DBG_SYS(EP_CTRL, "write failed: %s\n",
				strerror(errno));
		usdf_cm_report_failure(crp, -errno, false);
		return -errno;
	}

	crp->cr_resid -= ret;
	crp->cr_ptr += ret;

	return FI_SUCCESS;
}

static int usdf_pep_reject(struct fid_pep *fpep, fid_t handle, const void *param,
		size_t paramlen)
{
	struct usdf_pep *pep;
	struct usdf_connreq *crp;
	struct usdf_connreq_msg *reqp;
	int ret;

	if (paramlen > USDF_MAX_CONN_DATA) {
		USDF_WARN_SYS(EP_CTRL,
			      "reject payload size %zu exceeds max %u\n",
			      paramlen, USDF_MAX_CONN_DATA);
		return -FI_EINVAL;
	}

	if (!fpep || !handle) {
		USDF_WARN_SYS(EP_CTRL,
				"handle and passive ep needed for reject\n");
		return -FI_EINVAL;
	}

	if (!param && paramlen > 0) {
		USDF_WARN_SYS(EP_CTRL,
				"NULL data pointer with non-zero data length\n");
		return -FI_EINVAL;
	}

	/* usdf_pep_conn_info stashed the pep pointer into the handle field of
	 * the info struct previously returned
	 */
	crp = (struct usdf_connreq *) handle;
	pep = pep_ftou(fpep);

	crp->cr_ptr = crp->cr_data;
	crp->cr_resid = sizeof(*reqp) + paramlen;

	reqp = (struct usdf_connreq_msg *) crp->cr_data;

	/* The result field is used on the remote end to detect whether the
	 * connection succeeded or failed.
	 */
	reqp->creq_result = htonl(-FI_ECONNREFUSED);
	reqp->creq_datalen = htonl(paramlen);
	memcpy(reqp->creq_data, param, paramlen);

	crp->cr_pollitem.pi_rtn = usdf_pep_reject_async;
	crp->cr_pollitem.pi_context = crp;

	ret = ofi_epoll_add(pep->pep_fabric->fab_epollfd, crp->cr_sockfd,
			    OFI_EPOLL_OUT, &crp->cr_pollitem);
	return ret;
}

static void
usdf_pep_free_cr_lists(struct usdf_pep *pep)
{
	struct usdf_connreq *crp;

	while (!TAILQ_EMPTY(&pep->pep_cr_free)) {
		crp = TAILQ_FIRST(&pep->pep_cr_free);
		TAILQ_REMOVE(&pep->pep_cr_free, crp, cr_link);
		free(crp);
	}

	while (!TAILQ_EMPTY(&pep->pep_cr_pending)) {
		crp = TAILQ_FIRST(&pep->pep_cr_pending);
		TAILQ_REMOVE(&pep->pep_cr_pending, crp, cr_link);
		free(crp);
	}
}

static int
usdf_pep_grow_backlog(struct usdf_pep *pep)
{
	struct usdf_connreq *crp;
	size_t extra;

	extra = sizeof(struct usdf_connreq_msg) + pep->pep_cr_max_data;

	while (pep->pep_cr_alloced < pep->pep_backlog) {
		crp = calloc(1, sizeof(*crp) + extra);
		if (crp == NULL) {
			return -FI_ENOMEM;
		}
		crp->handle.fclass = FI_CLASS_CONNREQ;
		pthread_spin_lock(&pep->pep_cr_lock);
		TAILQ_INSERT_TAIL(&pep->pep_cr_free, crp, cr_link);
		++pep->pep_cr_alloced;
		pthread_spin_unlock(&pep->pep_cr_lock);
	}
	return 0;
}

static int
usdf_pep_close(fid_t fid)
{
	struct usdf_pep *pep;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	pep = pep_fidtou(fid);
	if (ofi_atomic_get32(&pep->pep_refcnt) > 0) {
		return -FI_EBUSY;
	}

	usdf_pep_free_cr_lists(pep);
	close(pep->pep_sock);
	pep->pep_sock = -1;
	if (pep->pep_eq != NULL) {
		ofi_atomic_dec32(&pep->pep_eq->eq_refcnt);
	}
	ofi_atomic_dec32(&pep->pep_fabric->fab_refcnt);
	fi_freeinfo(pep->pep_info);
	free(pep);

	return 0;
}

static int usdf_pep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	int ret;
	struct usdf_pep *pep;
	struct fi_info *info;
	size_t copylen;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	ret = FI_SUCCESS;
	pep = pep_fidtou(fid);
	info = pep->pep_info;

	copylen = info->src_addrlen;
	memcpy(addr, &pep->pep_src_addr, MIN(copylen, *addrlen));

	if (*addrlen < copylen) {
		USDF_WARN_SYS(EP_CTRL, "*addrlen is too short\n");
		ret = -FI_ETOOSMALL;
	}

	*addrlen = copylen;
	return ret;
}

static int usdf_pep_setname(fid_t fid, void *addr, size_t addrlen)
{
	int ret;
	struct usdf_pep *pep;
	struct fi_info *info;
	struct sockaddr_in *sin;
	uint32_t req_addr_be;
	socklen_t socklen;
	char namebuf[INET_ADDRSTRLEN];
	char servbuf[INET_ADDRSTRLEN];
	bool addr_format_str;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	pep = pep_fidtou(fid);
	info = pep->pep_info;
	addr_format_str = (info->addr_format == FI_ADDR_STR);
	sin = NULL;

	if (pep->pep_state != USDF_PEP_UNBOUND) {
		USDF_WARN_SYS(EP_CTRL, "PEP cannot be bound\n");
		return -FI_EOPBADSTATE;
	}

	switch (info->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		/* It is possible for passive endpoint to not have src_addr. */
		if (info->src_addr) {
			ret = usdf_cm_addr_is_valid_sin(info->src_addr,
							info->src_addrlen,
							info->addr_format);
			if (!ret)
				return -FI_EINVAL;
		}
		break;
	case FI_ADDR_STR:
		break;
	default:
		return -FI_EINVAL;
	}

	sin = usdf_format_to_sin(info, addr);
	req_addr_be = sin->sin_addr.s_addr;

	namebuf[0] = '\0';
	servbuf[0] = '\0';
	ret = getnameinfo((struct sockaddr *)sin, sizeof(struct sockaddr_in),
		namebuf, sizeof(namebuf),
		servbuf, sizeof(servbuf),
		NI_NUMERICHOST|NI_NUMERICSERV);
	if (ret != 0)
		USDF_WARN_SYS(EP_CTRL, "unable to getnameinfo(0x%x)\n",
			req_addr_be);

	if (req_addr_be != pep->pep_fabric->fab_dev_attrs->uda_ipaddr_be) {
		USDF_WARN_SYS(EP_CTRL, "requested addr (%s:%s) does not match fabric addr\n",
			namebuf, servbuf);
		return -FI_EADDRNOTAVAIL;
	}

	ret = bind(pep->pep_sock, sin, sizeof(*sin));
	if (ret == -1) {
		return -errno;
	}
	pep->pep_state = USDF_PEP_BOUND;

	/* store the resulting port so that can implement getname() properly */
	socklen = sizeof(*sin);
	ret = getsockname(pep->pep_sock, sin, &socklen);
	if (ret == -1) {
		ret = -errno;
		USDF_WARN_SYS(EP_CTRL, "getsockname failed %d (%s), PEP may be in bad state\n",
			ret, strerror(-ret));
		return ret;
	}

	if (addr_format_str) {
		/* We have to reset src_addrlen here and
		 * the conversion will update it to the correct len.
		 */
		info->src_addrlen = USDF_ADDR_STR_LEN;
		usdf_addr_tostr(sin, pep->pep_src_addr.addr_str,
				&info->src_addrlen);
		free(sin);
	} else {
		memcpy(&pep->pep_src_addr, sin, sizeof(*sin));
	}

	return 0;
}

struct fi_ops usdf_pep_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_pep_close,
	.bind = usdf_pep_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open
};

static struct fi_ops_ep usdf_pep_base_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = usdf_ep_getopt_connected,
	.setopt = usdf_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct fi_ops_cm usdf_pep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = usdf_pep_setname,
	.getname = usdf_pep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = usdf_pep_listen,
	.accept = fi_no_accept,
	.reject = usdf_pep_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

int
usdf_pep_open(struct fid_fabric *fabric, struct fi_info *info,
	    struct fid_pep **pep_o, void *context)
{
	struct usdf_pep *pep;
	struct usdf_fabric *fp;
	struct sockaddr_in *sin;
	int ret;
	int optval;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	if (!info) {
		USDF_DBG_SYS(EP_CTRL, "null fi_info struct is invalid\n");
		return -FI_EINVAL;
	}

	if (info->ep_attr->type != FI_EP_MSG) {
		return -FI_ENODEV;
	}

	switch (info->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		/* It is possible for passive endpoint to not have src_addr. */
		if (info->src_addr) {
			ret = usdf_cm_addr_is_valid_sin(info->src_addr,
							info->src_addrlen,
							info->addr_format);
			if (!ret)
				return -FI_EINVAL;
		}
		break;
	case FI_ADDR_STR:
		break;
	default:
		USDF_WARN_SYS(EP_CTRL, "unknown/unsupported addr_format\n");
		return -FI_EINVAL;
	}

	fp = fab_ftou(fabric);

	pep = calloc(1, sizeof(*pep));
	if (pep == NULL) {
		return -FI_ENOMEM;
	}

	pep->pep_fid.fid.fclass = FI_CLASS_PEP;
	pep->pep_fid.fid.context = context;
	pep->pep_fid.fid.ops = &usdf_pep_ops;
	pep->pep_fid.ops = &usdf_pep_base_ops;
	pep->pep_fid.cm = &usdf_pep_cm_ops;
	pep->pep_fabric = fp;

	pep->pep_state = USDF_PEP_UNBOUND;
	pep->pep_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (pep->pep_sock == -1) {
		ret = -errno;
		goto fail;
	}
	ret = fi_fd_nonblock(pep->pep_sock);
	if (ret) {
		ret = -errno;
		goto fail;
	}

	/* set SO_REUSEADDR to prevent annoying "Address already in use" errors
	 * on successive runs of programs listening on a well known port */
	optval = 1;
	ret = setsockopt(pep->pep_sock, SOL_SOCKET, SO_REUSEADDR, &optval,
				sizeof(optval));
	if (ret == -1) {
		ret = -errno;
		goto fail;
	}

	pep->pep_info = fi_dupinfo(info);
	if (!pep->pep_info) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	if (info->src_addrlen == 0) {
		/* Copy the source address information from the device
		 * attributes.
		 */
		pep->pep_info->src_addrlen = sizeof(struct sockaddr_in);
		sin = calloc(1, pep->pep_info->src_addrlen);
		if (!sin) {
			USDF_WARN_SYS(EP_CTRL,
					"calloc for src address failed\n");
			goto fail;
		}

		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = fp->fab_dev_attrs->uda_ipaddr_be;

		pep->pep_info->src_addr =
			usdf_sin_to_format(pep->pep_info,
					   sin, &pep->pep_info->src_addrlen);
	}

	memcpy(&pep->pep_src_addr, pep->pep_info->src_addr,
	       pep->pep_info->src_addrlen);

	/* initialize connreq freelist */
	ret = pthread_spin_init(&pep->pep_cr_lock, PTHREAD_PROCESS_PRIVATE);
	if (ret != 0) {
		ret = -ret;
		goto fail;
	}
	TAILQ_INIT(&pep->pep_cr_free);
	TAILQ_INIT(&pep->pep_cr_pending);
	pep->pep_backlog = 10;
	pep->pep_cr_max_data = USDF_MAX_CONN_DATA;

	ret = usdf_pep_grow_backlog(pep);
	if (ret != 0) {
		goto fail;
	}

	ofi_atomic_initialize32(&pep->pep_refcnt, 0);
	ofi_atomic_inc32(&fp->fab_refcnt);

	*pep_o = pep_utof(pep);
	return 0;

fail:
	if (pep != NULL) {
		usdf_pep_free_cr_lists(pep);
		if (pep->pep_sock != -1) {
			close(pep->pep_sock);
		}
		fi_freeinfo(pep->pep_info);
		free(pep);
	}
	return ret;
}

/* Steals the socket underpinning the PEP for use by an active endpoint.  After
 * this call, the only valid action a user may take on this PEP is to close it.
 * Sets "*is_bound=1" if the socket was already bound to an address,
 * "*is_bound=0" if not bound, or "*is_bound" will be undefined if this function
 * returns a non-zero error code. */
int usdf_pep_steal_socket(struct usdf_pep *pep, int *is_bound, int *sock_o)
{
	switch (pep->pep_state) {
	case USDF_PEP_UNBOUND:
		if (is_bound != NULL)
			*is_bound = 0;
		break;
	case USDF_PEP_BOUND:
		if (is_bound != NULL)
			*is_bound = 1;
		break;
	case USDF_PEP_LISTENING:
		USDF_WARN_SYS(EP_CTRL,
			"PEP already listening, cannot use as \"handle\" in fi_endpoint()\n");
		return -FI_EOPBADSTATE;
	case USDF_PEP_ROBBED:
		USDF_WARN_SYS(EP_CTRL,
			"PEP already consumed, you may only fi_close() now\n");
		return -FI_EOPBADSTATE;
	}

	*sock_o = pep->pep_sock;
	pep->pep_sock = -1;
	pep->pep_state = USDF_PEP_ROBBED;
	return 0;
}
