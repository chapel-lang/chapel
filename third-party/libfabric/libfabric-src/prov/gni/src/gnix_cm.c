/*
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved.
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gnix_cm.h"
#include "gnix.h"
#include "gnix_util.h"
#include "gnix_nic.h"
#include "gnix_cm_nic.h"
#include "gnix_nameserver.h"
#include "gnix_eq.h"
#include "gnix_vc.h"
#include "gnix_av.h"

struct fi_ops gnix_pep_fi_ops;
struct fi_ops_ep gnix_pep_ops_ep;
struct fi_ops_cm gnix_pep_ops_cm;

int _gnix_ep_name_to_str(struct gnix_ep_name *ep_name, char **out_buf)
{
	char *str;
	size_t len = GNIX_FI_ADDR_STR_LEN;

	GNIX_TRACE(FI_LOG_TRACE, "\n");

	if (*out_buf == NULL) {
		str = calloc(len, sizeof(char));
		if (str == NULL) {
			GNIX_WARN(FI_LOG_FABRIC, fi_strerror(FI_ENOMEM));
			return -FI_ENOMEM;
		}
	} else {
		str = *out_buf;
	}

	/* Convert raw address info to string */
	snprintf(str, len, "gni;NONE;NONE;%04i;0x%08" PRIx32 ";0x%08" PRIx32
		";%02i;0x%06" PRIx32 ";0x%08" PRIx32 ";%02i",
		 GNIX_AV_STR_ADDR_VERSION,
		 ep_name->gnix_addr.device_addr,
		 ep_name->gnix_addr.cdm_id,
		 ep_name->name_type,
		 ep_name->cm_nic_cdm_id,
		 ep_name->cookie,
		 ep_name->rx_ctx_cnt);

	return FI_SUCCESS;
}

int _gnix_ep_name_from_str(const char *addr,
			    struct gnix_ep_name *resolved_addr)
{
	char *tok, *endptr;
	int ret;
	struct gnix_ep_name ep_name;
	long tok_val;
	char *dup_addr;

	GNIX_TRACE(FI_LOG_TRACE, "\n");

	if (!addr || !resolved_addr) {
		GNIX_WARN(FI_LOG_WARN, "NULL parameter in "
			"__gnix_resolved_name_from_str");
		return -FI_EINVAL;
	}

	dup_addr = strdup(addr);
	if (!dup_addr) {
		return -FI_ENOMEM;
	}

	tok = strtok(dup_addr, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		return -FI_EINVAL;
	}

	ret = memcmp(tok, "gni", 3);
	if (ret) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}

	tok = strtok(NULL, ";");/*node*/
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}

	tok = strtok(NULL, ";");/*service*/
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}

	tok = strtok(NULL, ";");/*GNIX_AV_STR_ADDR_VERSION*/
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}

	/*device_addr*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 16);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid device_addr.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.gnix_addr.device_addr = (uint32_t) tok_val;

	/*cdm_id*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 16);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid cdm_id.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.gnix_addr.cdm_id = (uint32_t) tok_val;

	/*name_type*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 10);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid name_type.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.name_type = (uint32_t) tok_val;

	/*cm_nic_cdm_id*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 16);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid cm_nic_cdm_id.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.cm_nic_cdm_id = (uint32_t) tok_val;

	/*cookie*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 16);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid cookie.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.cookie = (uint32_t) tok_val;

	/*rx_ctx_cnt*/
	tok = strtok(NULL, ";");
	if (!tok) {
		GNIX_WARN(FI_LOG_WARN, "Invalid address.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	tok_val = strtol(tok, &endptr, 10);
	if (*endptr) {
		GNIX_WARN(FI_LOG_WARN, "Invalid rx_ctx_cnt.\n");
		free(dup_addr);
		return -FI_EINVAL;
	}
	ep_name.rx_ctx_cnt = (uint32_t) tok_val;

	*resolved_addr = ep_name;
	free(dup_addr);

	return FI_SUCCESS;
}

/******************************************************************************
 *
 * Common CM handling (supported for all types of endpoints).
 *
 *****************************************************************************/

/**
 * Retrieve the local endpoint address.
 *
 * addrlen: Should indicate the size of the addr buffer. On output will contain
 *     the size necessary to copy the proper address structure.
 *
 * addr: Pointer to memory that will contain the address structure. Should be
 *     allocated and of size addrlen. If addrlen is less than necessary to copy
 *     the proper address structure then addr will contain a truncated address.
 *     Depending on what hints were used during setup, addr will either be in
 *     the FI_ADDR_STR or FI_ADDR_GNI format.
 *
 * return: FI_SUCCESS or negative error value.
 */
DIRECT_FN STATIC int gnix_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct gnix_fid_ep *ep = NULL;
	struct gnix_fid_sep *sep = NULL;
	struct gnix_fid_pep *pep = NULL;
	size_t len = 0, cpylen;
	bool is_fi_addr_str;
	struct fi_info *info;
	struct gnix_ep_name *ep_name;
	int ret;

	if (OFI_UNLIKELY(addrlen == NULL)) {
		GNIX_INFO(FI_LOG_EP_CTRL, "parameter \"addrlen\" is NULL in "
			"gnix_getname\n");
		return -FI_EINVAL;
	}

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct gnix_fid_ep, ep_fid.fid);
		info = ep->info;
		ep_name = &ep->src_addr;
		break;
	case FI_CLASS_SEP:
		sep = container_of(fid, struct gnix_fid_sep, ep_fid);
		info = sep->info;
		ep_name = &sep->my_name;
		break;
	case FI_CLASS_PEP:
		pep = container_of(fid, struct gnix_fid_pep,
				   pep_fid.fid);
		info = pep->info;
		ep_name = &pep->src_addr;
		break;
	default:
		GNIX_INFO(FI_LOG_EP_CTRL,
			  "Invalid fid class: %d\n",
			  fid->fclass);
		return -FI_EINVAL;
	}

	is_fi_addr_str = info->addr_format == FI_ADDR_STR;

	if (!addr) {
		if (OFI_UNLIKELY(is_fi_addr_str)) {
			*addrlen = GNIX_FI_ADDR_STR_LEN;
		} else {
			*addrlen = sizeof(struct gnix_ep_name);
		}

		return -FI_ETOOSMALL;
	}

	if (OFI_UNLIKELY(is_fi_addr_str)) {
		ret = _gnix_ep_name_to_str(ep_name, (char **) &addr);

		if (ret)
			return ret;

		len = GNIX_FI_ADDR_STR_LEN;
		cpylen = MIN(len, *addrlen);
	} else {
		len = sizeof(struct gnix_ep_name);
		cpylen = MIN(len, *addrlen);
		memcpy(addr, ep_name, cpylen);
	}

	*addrlen = len;
	return (len == cpylen) ? FI_SUCCESS : -FI_ETOOSMALL;
}

DIRECT_FN STATIC int gnix_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct gnix_fid_ep *ep = NULL;
	struct gnix_fid_sep *sep = NULL;
	struct gnix_fid_pep *pep = NULL;
	struct fi_info *info;
	struct gnix_ep_name *ep_name;
	size_t len;
	int ret;

	if (OFI_UNLIKELY(addr == NULL)) {
		GNIX_INFO(FI_LOG_EP_CTRL, "parameter \"addr\" is NULL in "
			"gnix_setname\n");
		return -FI_EINVAL;
	}

	len = sizeof(struct gnix_ep_name);

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct gnix_fid_ep, ep_fid.fid);
		info = ep->info;
		ep_name = &ep->src_addr;
		break;
	case FI_CLASS_SEP:
		sep = container_of(fid, struct gnix_fid_sep, ep_fid);
		info = sep->info;
		ep_name = &sep->my_name;
		break;
	case FI_CLASS_PEP:
		pep = container_of(fid, struct gnix_fid_pep, pep_fid.fid);
		/* TODO: make sure we're unconnected. */
		pep->bound = 1;
		info = pep->info;
		ep_name = &pep->src_addr;
		break;
	default:
		GNIX_INFO(FI_LOG_EP_CTRL, "Invalid fid class: %d\n",
			  fid->fclass);
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(info->addr_format == FI_ADDR_STR)) {
		len = GNIX_FI_ADDR_STR_LEN;

		if (addrlen != len)
			return -FI_EINVAL;

		ret = _gnix_ep_name_from_str((const char *) addr,
					     ep_name);

		if (ret)
			return ret;

		return FI_SUCCESS;
	}

	if (addrlen != len)
		return -FI_EINVAL;

	memcpy(ep_name, addr, len);

	return FI_SUCCESS;
}

DIRECT_FN STATIC int gnix_getpeer(struct fid_ep *ep, void *addr,
				  size_t *addrlen)
{
	struct gnix_fid_ep *ep_priv = NULL;
	struct gnix_fid_sep *sep_priv = NULL;
	struct gnix_ep_name *ep_name = NULL;
	size_t len = 0, cpylen = 0;
	struct fi_info *info = NULL;
	int ret;

	if (OFI_UNLIKELY(addrlen == NULL || addr == NULL)) {
		GNIX_INFO(FI_LOG_EP_CTRL,
			  "parameter is NULL in gnix_getpeer\n");
		return -FI_EINVAL;
	}

	switch (ep->fid.fclass) {
	case FI_CLASS_EP:
		ep_priv = container_of(ep, struct gnix_fid_ep, ep_fid.fid);
		info = ep_priv->info;
		ep_name = &ep_priv->dest_addr;
		break;

	case FI_CLASS_SEP:
		sep_priv = container_of(ep, struct gnix_fid_sep, ep_fid);
		info = sep_priv->info;
		ep_name = info->dest_addr;
		break;

	default:
		GNIX_INFO(FI_LOG_EP_CTRL, "Invalid fid class: %d\n",
			  ep->fid.fclass);
			return -FI_EINVAL;
	}

	if (info->addr_format == FI_ADDR_STR) {
		ret = _gnix_ep_name_to_str(ep_name, (char **) &addr);

		if (ret)
			return ret;

		len = GNIX_FI_ADDR_STR_LEN;
		cpylen = MIN(len, *addrlen);
	} else {
		len = sizeof(struct gnix_ep_name);
		cpylen = MIN(len, *addrlen);
		memcpy(addr, ep_name, cpylen);
	}

	*addrlen = len;

	return (len == cpylen) ? FI_SUCCESS : -FI_ETOOSMALL;
}

struct fi_ops_cm gnix_ep_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = gnix_setname,
	.getname = gnix_getname,
	.getpeer = gnix_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

/******************************************************************************
 *
 * FI_EP_MSG endpoint handling
 *
 *****************************************************************************/

/* Process a connection response on an FI_EP_MSG. */
static int __gnix_ep_connresp(struct gnix_fid_ep *ep,
			      struct gnix_pep_sock_connresp *resp)
{
	int ret = FI_SUCCESS;
	struct fi_eq_cm_entry *eq_entry;
	int eqe_size;

	switch (resp->cmd) {
	case GNIX_PEP_SOCK_RESP_ACCEPT:
		ep->vc->peer_caps = resp->peer_caps;
		ep->vc->peer_key_offset = resp->key_offset;
		ep->vc->peer_id = resp->vc_id;

		/* Initialize the GNI connection. */
		ret = _gnix_vc_smsg_init(ep->vc, resp->vc_id,
					 &resp->vc_mbox_attr,
					 &resp->cq_irq_mdh);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "_gnix_vc_smsg_init returned %s\n",
				  fi_strerror(-ret));
			return ret;
		}

		ep->vc->conn_state = GNIX_VC_CONNECTED;
		ep->conn_state = GNIX_EP_CONNECTED;

		/* Notify user that this side is connected. */
		eq_entry = (struct fi_eq_cm_entry *)resp->eqe_buf;
		eq_entry->fid = &ep->ep_fid.fid;

		eqe_size = sizeof(*eq_entry) + resp->cm_data_len;
		ret = fi_eq_write(&ep->eq->eq_fid, FI_CONNECTED, eq_entry,
				  eqe_size, 0);
		if (ret != eqe_size) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "fi_eq_write failed, err: %d\n", ret);
			return ret;
		}

		GNIX_DEBUG(FI_LOG_EP_CTRL, "Received conn accept: %p\n", ep);

		break;
	case GNIX_PEP_SOCK_RESP_REJECT:
		/* Undo the connect and generate a failure EQE. */
		close(ep->conn_fd);
		ep->conn_fd = -1;

		_gnix_mbox_free(ep->vc->smsg_mbox);
		ep->vc->smsg_mbox = NULL;

		_gnix_vc_destroy(ep->vc);
		ep->vc = NULL;

		ep->conn_state = GNIX_EP_UNCONNECTED;

		/* Generate EQE. */
		eq_entry = (struct fi_eq_cm_entry *)resp->eqe_buf;
		eq_entry->fid = &ep->ep_fid.fid;

		eq_entry = (struct fi_eq_cm_entry *)resp->eqe_buf;
		ret = _gnix_eq_write_error(ep->eq, &ep->ep_fid.fid, NULL, 0,
					   FI_ECONNREFUSED, FI_ECONNREFUSED,
					   &eq_entry->data, resp->cm_data_len);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "fi_eq_write failed, err: %d\n", ret);
			return ret;
		}

		GNIX_DEBUG(FI_LOG_EP_CTRL, "Conn rejected: %p\n", ep);

		break;
	default:
		GNIX_INFO(FI_LOG_EP_CTRL, "Invalid response command: %d\n",
			  resp->cmd);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

/* Check for a connection response on an FI_EP_MSG. */
int _gnix_ep_progress(struct gnix_fid_ep *ep)
{
	int ret, bytes_read, errno_keep;
	struct gnix_pep_sock_connresp resp;

	/* No lock, fast exit. */
	if (ep->conn_state != GNIX_EP_CONNECTING) {
		return FI_SUCCESS;
	}

	COND_ACQUIRE(ep->requires_lock, &ep->vc_lock);

	if (ep->conn_state != GNIX_EP_CONNECTING) {
		COND_RELEASE(ep->requires_lock, &ep->vc_lock);
		return FI_SUCCESS;
	}

	/* Check for a connection response. */
	bytes_read = read(ep->conn_fd, &resp, sizeof(resp));
	if (bytes_read >= 0) {
		if (bytes_read == sizeof(resp)) {
			/* Received response. */
			ret = __gnix_ep_connresp(ep, &resp);
			if (ret != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "__gnix_pep_connreq failed, %d\n",
					  ret);
			}
		} else {
			errno_keep = errno;
			GNIX_FATAL(FI_LOG_EP_CTRL,
				   "Unexpected read size: %d err: %s\n",
				   bytes_read, strerror(errno_keep));
		}
	} else if (errno != EAGAIN) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL, "Read error: %s\n",
				strerror(errno_keep));
	}

	COND_RELEASE(ep->requires_lock, &ep->vc_lock);

	return FI_SUCCESS;
}

DIRECT_FN STATIC int gnix_connect(struct fid_ep *ep, const void *addr,
				  const void *param, size_t paramlen)
{
	int ret, errno_keep;
	struct gnix_fid_ep *ep_priv;
	struct sockaddr_in saddr;
	struct gnix_pep_sock_connreq req;
	struct fi_eq_cm_entry *eqe_ptr;
	struct gnix_vc *vc;
	struct gnix_mbox *mbox = NULL;
	struct gnix_av_addr_entry av_entry;

	if (!ep || !addr || (paramlen && !param) ||
	    paramlen > GNIX_CM_DATA_MAX_SIZE)
		return -FI_EINVAL;

	ep_priv = container_of(ep, struct gnix_fid_ep, ep_fid.fid);

	COND_ACQUIRE(ep_priv->requires_lock, &ep_priv->vc_lock);

	if (ep_priv->conn_state != GNIX_EP_UNCONNECTED) {
		ret = -FI_EINVAL;
		goto err_unlock;
	}

	ret = _gnix_pe_to_ip(addr, &saddr);
	if (ret != FI_SUCCESS) {
		GNIX_INFO(FI_LOG_EP_CTRL,
			  "Failed to translate gnix_ep_name to IP\n");
		goto err_unlock;
	}

	/* Create new VC without CM data. */
	av_entry.gnix_addr = ep_priv->dest_addr.gnix_addr;
	av_entry.cm_nic_cdm_id = ep_priv->dest_addr.cm_nic_cdm_id;
	ret = _gnix_vc_alloc(ep_priv, &av_entry, &vc);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to create VC:: %d\n",
			  ret);
		goto err_unlock;
	}
	ep_priv->vc = vc;

	ret = _gnix_mbox_alloc(vc->ep->nic->mbox_hndl, &mbox);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_DATA,
			  "_gnix_mbox_alloc returned %s\n",
			  fi_strerror(-ret));
		goto err_mbox_alloc;
	}
	vc->smsg_mbox = mbox;

	ep_priv->conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (ep_priv->conn_fd < 0) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to create connect socket, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_ENOSPC;
		goto err_socket;
	}

	/* Currently blocks until connected. */
	ret = connect(ep_priv->conn_fd, (struct sockaddr *)&saddr,
		      sizeof(saddr));
	if (ret) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to connect, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_EIO;
		goto err_connect;
	}

	memset(&req, 0, sizeof(req));
	req.info = *ep_priv->info;

	/* Note addrs are swapped. */
	memcpy(&req.dest_addr, (void *)&ep_priv->src_addr,
	       sizeof(req.dest_addr));
	memcpy(&ep_priv->dest_addr, addr, sizeof(ep_priv->dest_addr));
	memcpy(&req.src_addr, addr, sizeof(req.src_addr));

	if (ep_priv->info->tx_attr)
		req.tx_attr = *ep_priv->info->tx_attr;
	if (ep_priv->info->rx_attr)
		req.rx_attr = *ep_priv->info->rx_attr;
	if (ep_priv->info->ep_attr)
		req.ep_attr = *ep_priv->info->ep_attr;
	if (ep_priv->info->domain_attr)
		req.domain_attr = *ep_priv->info->domain_attr;
	if (ep_priv->info->fabric_attr)
		req.fabric_attr = *ep_priv->info->fabric_attr;

	req.vc_id = vc->vc_id;
	req.vc_mbox_attr.msg_type = GNI_SMSG_TYPE_MBOX_AUTO_RETRANSMIT;
	req.vc_mbox_attr.msg_buffer = mbox->base;
	req.vc_mbox_attr.buff_size =  vc->ep->nic->mem_per_mbox;
	req.vc_mbox_attr.mem_hndl = *mbox->memory_handle;
	req.vc_mbox_attr.mbox_offset = (uint64_t)mbox->offset;
	req.vc_mbox_attr.mbox_maxcredit =
			ep_priv->domain->params.mbox_maxcredit;
	req.vc_mbox_attr.msg_maxsize = ep_priv->domain->params.mbox_msg_maxsize;
	req.cq_irq_mdh = ep_priv->nic->irq_mem_hndl;
	req.peer_caps = ep_priv->caps;
	req.key_offset = ep_priv->auth_key->key_offset;

	req.cm_data_len = paramlen;
	if (paramlen) {
		eqe_ptr = (struct fi_eq_cm_entry *)req.eqe_buf;
		memcpy(eqe_ptr->data, param, paramlen);
	}

	ret = write(ep_priv->conn_fd, &req, sizeof(req));
	if (ret != sizeof(req)) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to send req, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_EIO;
		goto err_write;
	}
	/* set fd to non-blocking now since we can't block within the eq
	 * progress system
	 */
	fi_fd_nonblock(ep_priv->conn_fd);

	ep_priv->conn_state = GNIX_EP_CONNECTING;

	COND_RELEASE(ep_priv->requires_lock, &ep_priv->vc_lock);

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Sent conn req: %p, %s\n",
		   ep_priv, inet_ntoa(saddr.sin_addr));

	return FI_SUCCESS;

err_write:
err_connect:
	close(ep_priv->conn_fd);
	ep_priv->conn_fd = -1;
err_socket:
	_gnix_mbox_free(ep_priv->vc->smsg_mbox);
	ep_priv->vc->smsg_mbox = NULL;
err_mbox_alloc:
	_gnix_vc_destroy(ep_priv->vc);
	ep_priv->vc = NULL;
err_unlock:
	COND_RELEASE(ep_priv->requires_lock, &ep_priv->vc_lock);

	return ret;
}

DIRECT_FN STATIC int gnix_accept(struct fid_ep *ep, const void *param,
				 size_t paramlen)
{
	int ret, errno_keep;
	struct gnix_vc *vc;
	struct gnix_fid_ep *ep_priv;
	struct gnix_pep_sock_conn *conn;
	struct gnix_pep_sock_connresp resp;
	struct fi_eq_cm_entry eq_entry, *eqe_ptr;
	struct gnix_mbox *mbox = NULL;
	struct gnix_av_addr_entry av_entry;

	if (!ep || (paramlen && !param) || paramlen > GNIX_CM_DATA_MAX_SIZE)
		return -FI_EINVAL;

	ep_priv = container_of(ep, struct gnix_fid_ep, ep_fid.fid);

	COND_ACQUIRE(ep_priv->requires_lock, &ep_priv->vc_lock);

	/* Look up and unpack the connection request used to create this EP. */
	conn = (struct gnix_pep_sock_conn *)ep_priv->info->handle;
	if (!conn || conn->fid.fclass != FI_CLASS_CONNREQ) {
		ret = -FI_EINVAL;
		goto err_unlock;
	}

	/* Create new VC without CM data. */
	av_entry.gnix_addr = ep_priv->dest_addr.gnix_addr;
	av_entry.cm_nic_cdm_id = ep_priv->dest_addr.cm_nic_cdm_id;
	ret = _gnix_vc_alloc(ep_priv, &av_entry, &vc);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Failed to create VC: %d\n", ret);
		goto err_unlock;
	}
	ep_priv->vc = vc;
	ep_priv->vc->peer_caps = conn->req.peer_caps;
	ep_priv->vc->peer_key_offset = conn->req.key_offset;
	ep_priv->vc->peer_id = conn->req.vc_id;

	ret = _gnix_mbox_alloc(vc->ep->nic->mbox_hndl, &mbox);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_DATA, "_gnix_mbox_alloc returned %s\n",
			  fi_strerror(-ret));
		goto err_mbox_alloc;
	}
	vc->smsg_mbox = mbox;

	/* Initialize the GNI connection. */
	ret = _gnix_vc_smsg_init(vc, conn->req.vc_id,
				 &conn->req.vc_mbox_attr,
				 &conn->req.cq_irq_mdh);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "_gnix_vc_smsg_init returned %s\n",
			  fi_strerror(-ret));
		goto err_smsg_init;
	}

	vc->conn_state = GNIX_VC_CONNECTED;

	/* Send ACK with VC attrs to allow peer to initialize GNI connection. */
	resp.cmd = GNIX_PEP_SOCK_RESP_ACCEPT;

	resp.vc_id = vc->vc_id;
	resp.vc_mbox_attr.msg_type = GNI_SMSG_TYPE_MBOX_AUTO_RETRANSMIT;
	resp.vc_mbox_attr.msg_buffer = mbox->base;
	resp.vc_mbox_attr.buff_size =  vc->ep->nic->mem_per_mbox;
	resp.vc_mbox_attr.mem_hndl = *mbox->memory_handle;
	resp.vc_mbox_attr.mbox_offset = (uint64_t)mbox->offset;
	resp.vc_mbox_attr.mbox_maxcredit =
			ep_priv->domain->params.mbox_maxcredit;
	resp.vc_mbox_attr.msg_maxsize =
			ep_priv->domain->params.mbox_msg_maxsize;
	resp.cq_irq_mdh = ep_priv->nic->irq_mem_hndl;
	resp.peer_caps = ep_priv->caps;
	resp.key_offset = ep_priv->auth_key->key_offset;

	resp.cm_data_len = paramlen;
	if (paramlen) {
		eqe_ptr = (struct fi_eq_cm_entry *)resp.eqe_buf;
		memcpy(eqe_ptr->data, param, paramlen);
	}

	ret = write(conn->sock_fd, &resp, sizeof(resp));
	if (ret != sizeof(resp)) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to send resp, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_EIO;
		goto err_write;
	}

	/* Notify user that this side is connected. */
	eq_entry.fid = &ep_priv->ep_fid.fid;

	ret = fi_eq_write(&ep_priv->eq->eq_fid, FI_CONNECTED, &eq_entry,
			  sizeof(eq_entry), 0);
	if (ret != sizeof(eq_entry)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "fi_eq_write failed, err: %d\n", ret);
		goto err_eq_write;
	}

	/* Free the connection request. */
	free(conn);

	ep_priv->conn_state = GNIX_EP_CONNECTED;

	COND_RELEASE(ep_priv->requires_lock, &ep_priv->vc_lock);

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Sent conn accept: %p\n", ep_priv);

	return FI_SUCCESS;

err_eq_write:
err_write:
err_smsg_init:
	_gnix_mbox_free(ep_priv->vc->smsg_mbox);
	ep_priv->vc->smsg_mbox = NULL;
err_mbox_alloc:
	_gnix_vc_destroy(ep_priv->vc);
	ep_priv->vc = NULL;
err_unlock:
	COND_RELEASE(ep_priv->requires_lock, &ep_priv->vc_lock);

	return ret;
}

DIRECT_FN STATIC int gnix_shutdown(struct fid_ep *ep, uint64_t flags)
{
	int ret;
	struct gnix_fid_ep *ep_priv;
	struct fi_eq_cm_entry eq_entry = {0};

	if (!ep)
		return -FI_EINVAL;

	ep_priv = container_of(ep, struct gnix_fid_ep, ep_fid.fid);

	COND_ACQUIRE(ep_priv->requires_lock, &ep_priv->vc_lock);

	eq_entry.fid = &ep_priv->ep_fid.fid;

	ret = fi_eq_write(&ep_priv->eq->eq_fid, FI_SHUTDOWN, &eq_entry,
			  sizeof(eq_entry), 0);
	if (ret != sizeof(eq_entry)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "fi_eq_write failed, err: %d\n", ret);
	} else {
		ret = FI_SUCCESS;
	}

	COND_RELEASE(ep_priv->requires_lock, &ep_priv->vc_lock);

	return ret;
}

struct fi_ops_cm gnix_ep_msg_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = gnix_setname,
	.getname = gnix_getname,
	.getpeer = gnix_getpeer,
	.connect = gnix_connect,
	.listen = fi_no_listen,
	.accept = gnix_accept,
	.reject = fi_no_reject,
	.shutdown = gnix_shutdown,
	.join = fi_no_join,
};

/******************************************************************************
 *
 * Passive endpoint handling
 *
 *****************************************************************************/

DIRECT_FN STATIC int gnix_pep_getopt(fid_t fid, int level, int optname,
				     void *optval, size_t *optlen)
{
	if (!fid || !optval || !optlen)
		return -FI_EINVAL;
	else if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_CM_DATA_SIZE:
		*(size_t *)optval = GNIX_CM_DATA_MAX_SIZE;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

/* Process an incoming connection request at a listening PEP. */
static int __gnix_pep_connreq(struct gnix_fid_pep *pep, int fd)
{
	int ret;
	struct gnix_pep_sock_conn *conn;
	struct fi_eq_cm_entry *eq_entry;
	int eqe_size;

	/* Create and initialize a new connection request. */
	conn = calloc(1, sizeof(*conn));
	if (!conn) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to alloc accepted socket conn\n");
		return -FI_ENOMEM;
	}

	conn->fid.fclass = FI_CLASS_CONNREQ;
	conn->fid.context = pep;
	conn->sock_fd = fd;

	/* Pull request data from the listening socket. */
	conn->bytes_read += read(fd, &conn->req, sizeof(conn->req));
	if (conn->bytes_read != sizeof(conn->req)) {
		/* TODO Wait for more bytes. */
		GNIX_FATAL(FI_LOG_EP_CTRL, "Unexpected read size\n");
	}

	conn->req.info.src_addr = &conn->req.src_addr;
	conn->req.info.dest_addr = &conn->req.dest_addr;
	conn->req.info.tx_attr = &conn->req.tx_attr;
	conn->req.info.rx_attr = &conn->req.rx_attr;
	conn->req.info.ep_attr = &conn->req.ep_attr;
	conn->req.info.domain_attr = &conn->req.domain_attr;
	conn->req.info.fabric_attr = &conn->req.fabric_attr;
	conn->req.info.domain_attr->name = NULL;
	conn->req.info.fabric_attr->name = NULL;
	conn->req.info.fabric_attr->prov_name = NULL;

	conn->info = &conn->req.info;
	conn->info->handle = &conn->fid;

	/* Tell user of a new conn req via the EQ. */
	eq_entry = (struct fi_eq_cm_entry *)conn->req.eqe_buf;
	eq_entry->fid = &pep->pep_fid.fid;
	eq_entry->info = fi_dupinfo(conn->info);

	eqe_size = sizeof(*eq_entry) + conn->req.cm_data_len;
	ret = fi_eq_write(&pep->eq->eq_fid, FI_CONNREQ, eq_entry, eqe_size, 0);
	if (ret != eqe_size) {
		GNIX_WARN(FI_LOG_EP_CTRL, "fi_eq_write failed, err: %d\n", ret);
		fi_freeinfo(conn->info);
		free(conn);
		return ret;
	}

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Added FI_CONNREQ EQE: %p, %p\n",
		   pep->eq, pep);

	return FI_SUCCESS;
}

/* Process incoming connection requests on a listening PEP. */
int _gnix_pep_progress(struct gnix_fid_pep *pep)
{
	int accept_fd, ret, errno_keep;

	fastlock_acquire(&pep->lock);

	accept_fd = accept(pep->listen_fd, NULL, NULL);
	if (accept_fd >= 0) {
		/* New Connection. */
		ret = __gnix_pep_connreq(pep, accept_fd);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "__gnix_pep_connreq failed, err: %d\n",
				  ret);
		}
	} else if (errno != EAGAIN) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "(accept) Unexpected errno on listen socket: %s\n",
			  strerror(errno_keep));
	}

	fastlock_release(&pep->lock);

	return FI_SUCCESS;
}

static void __pep_destruct(void *obj)
{
	struct gnix_fid_pep *pep = (struct gnix_fid_pep *)obj;

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Destroying PEP: %p\n", pep);

	fastlock_destroy(&pep->lock);

	if (pep->listen_fd >= 0)
		close(pep->listen_fd);

	if (pep->eq) {
		_gnix_eq_poll_obj_rem(pep->eq, &pep->pep_fid.fid);
		_gnix_ref_put(pep->eq);
	}

	free(pep);
}

static int gnix_pep_close(fid_t fid)
{
	int ret = FI_SUCCESS;
	struct gnix_fid_pep *pep;
	int references_held;

	pep = container_of(fid, struct gnix_fid_pep, pep_fid.fid);

	references_held = _gnix_ref_put(pep);
	if (references_held)
		GNIX_INFO(FI_LOG_EP_CTRL, "failed to fully close pep due "
			  "to lingering references. references=%i pep=%p\n",
			  references_held, pep);

	return ret;
}

DIRECT_FN int gnix_pep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int ret = FI_SUCCESS;
	struct gnix_fid_pep  *pep;
	struct gnix_fid_eq *eq;

	if (!fid || !bfid)
		return -FI_EINVAL;

	pep = container_of(fid, struct gnix_fid_pep, pep_fid.fid);

	fastlock_acquire(&pep->lock);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct gnix_fid_eq, eq_fid.fid);
		if (pep->fabric != eq->fabric) {
			ret = -FI_EINVAL;
			break;
		}

		if (pep->eq) {
			ret = -FI_EINVAL;
			break;
		}

		pep->eq = eq;
		_gnix_eq_poll_obj_add(eq, &pep->pep_fid.fid);
		_gnix_ref_get(eq);

		GNIX_DEBUG(FI_LOG_EP_CTRL, "Bound EQ to PEP: %p, %p\n",
			   eq, pep);
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	fastlock_release(&pep->lock);

	return ret;
}

DIRECT_FN int gnix_pep_listen(struct fid_pep *pep)
{
	int ret, errno_keep;
	struct gnix_fid_pep *pep_priv;
	struct sockaddr_in saddr;
	int sockopt = 1;

	if (!pep)
		return -FI_EINVAL;

	pep_priv = container_of(pep, struct gnix_fid_pep, pep_fid.fid);

	fastlock_acquire(&pep_priv->lock);

	if (!pep_priv->eq) {
		ret = -FI_EINVAL;
		goto err_unlock;
	}

	pep_priv->listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (pep_priv->listen_fd < 0) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to create listening socket, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_ENOSPC;
		goto err_unlock;
	}

	ret = setsockopt(pep_priv->listen_fd, SOL_SOCKET, SO_REUSEADDR,
			 &sockopt, sizeof(sockopt));
	if (ret < 0) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "setsockopt(SO_REUSEADDR) failed, err: %s\n",
			  strerror(errno_keep));
	}

	/* Bind to the ipogif interface using resolved service number as CDM
	 * ID. */
	ret = _gnix_local_ipaddr(&saddr);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Failed to find local IP\n");
		ret = -FI_ENOSPC;
		goto err_sock;
	}

	/* If source addr was not specified, use auto assigned port. */
	if (pep_priv->bound)
		saddr.sin_port = pep_priv->src_addr.gnix_addr.cdm_id;
	else
		saddr.sin_port = 0;

	ret = bind(pep_priv->listen_fd, &saddr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to bind listening socket, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_ENOSPC;
		goto err_sock;
	}

	ret = listen(pep_priv->listen_fd, pep_priv->backlog);
	if (ret < 0) {
		errno_keep = errno;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to start listening socket, err: %s\n",
			  strerror(errno_keep));
		ret = -FI_ENOSPC;
		goto err_sock;
	}

	fastlock_release(&pep_priv->lock);

	GNIX_DEBUG(FI_LOG_EP_CTRL,
		   "Configured PEP for listening: %p (%s:%d)\n",
		   pep, inet_ntoa(saddr.sin_addr), saddr.sin_port);

	return FI_SUCCESS;

err_sock:
	close(pep_priv->listen_fd);
err_unlock:
	fastlock_release(&pep_priv->lock);
	return ret;
}

__attribute__((unused))
DIRECT_FN STATIC int gnix_listen(struct fid_pep *pep)
{
        return -FI_ENOSYS;
}

DIRECT_FN STATIC int gnix_reject(struct fid_pep *pep, fid_t handle,
				 const void *param, size_t paramlen)
{
	struct gnix_fid_pep *pep_priv;
	struct gnix_pep_sock_conn *conn;
	struct gnix_pep_sock_connresp resp;
	struct fi_eq_cm_entry *eqe_ptr;
	int ret;

	if (!pep)
		return -FI_EINVAL;

	pep_priv = container_of(pep, struct gnix_fid_pep, pep_fid.fid);

	fastlock_acquire(&pep_priv->lock);

	conn = (struct gnix_pep_sock_conn *)handle;
	if (!conn || conn->fid.fclass != FI_CLASS_CONNREQ) {
		fastlock_release(&pep_priv->lock);
		return -FI_EINVAL;
	}

	resp.cmd = GNIX_PEP_SOCK_RESP_REJECT;

	resp.cm_data_len = paramlen;
	if (paramlen) {
		eqe_ptr = (struct fi_eq_cm_entry *)resp.eqe_buf;
		memcpy(eqe_ptr->data, param, paramlen);
	}

	ret = write(conn->sock_fd, &resp, sizeof(resp));
	if (ret != sizeof(resp)) {
		fastlock_release(&pep_priv->lock);
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Failed to send resp, errno: %d\n",
			  errno);
		return -FI_EIO;
	}

	close(conn->sock_fd);
	free(conn);

	fastlock_release(&pep_priv->lock);

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Sent conn reject: %p\n", pep_priv);

	return FI_SUCCESS;
}

DIRECT_FN int gnix_pep_open(struct fid_fabric *fabric,
			    struct fi_info *info, struct fid_pep **pep,
			    void *context)
{
	struct gnix_fid_fabric *fabric_priv;
	struct gnix_fid_pep *pep_priv;
	struct gnix_ep_name *ep_name;

	if (!fabric || !info || !pep)
		return -FI_EINVAL;

	fabric_priv = container_of(fabric, struct gnix_fid_fabric, fab_fid);

	pep_priv = calloc(1, sizeof(*pep_priv));
	if (!pep_priv)
		return -FI_ENOMEM;

	pep_priv->pep_fid.fid.fclass = FI_CLASS_PEP;
	pep_priv->pep_fid.fid.context = context;

	pep_priv->pep_fid.fid.ops = &gnix_pep_fi_ops;
	pep_priv->pep_fid.ops = &gnix_pep_ops_ep;
	pep_priv->pep_fid.cm = &gnix_pep_ops_cm;
	pep_priv->fabric = fabric_priv;
	pep_priv->info = fi_dupinfo(info);
	pep_priv->info->addr_format = info->addr_format;

	pep_priv->listen_fd = -1;
	pep_priv->backlog = 5; /* TODO set via fi_control parameter. */
	fastlock_init(&pep_priv->lock);

	if (info->src_addr) {
		ep_name = info->src_addr;
		info->src_addrlen = sizeof(struct sockaddr_in);

		pep_priv->bound = 1;
		memcpy(&pep_priv->src_addr, ep_name, info->src_addrlen);
	} else {
		pep_priv->bound = 0;
	}

	_gnix_ref_init(&pep_priv->ref_cnt, 1, __pep_destruct);

	*pep = &pep_priv->pep_fid;

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Opened PEP: %p\n", pep_priv);

	return FI_SUCCESS;
}

struct fi_ops gnix_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_pep_close,
	.bind = gnix_pep_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

struct fi_ops_ep gnix_pep_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = gnix_pep_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

struct fi_ops_cm gnix_pep_ops_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = gnix_setname,
	.getname = gnix_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = gnix_pep_listen,
	.accept = fi_no_accept,
	.reject = gnix_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

