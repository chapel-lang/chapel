/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"

/*
 * SEP address query protocol:
 *
 * SEP Query REQ:
 *	args[0].u32w0	cmd, version
 *	args[0].u32w1	id
 *	args[1].u64	sep_info
 *	args[2].u64	status
 *
 * SEP Query REP:
 *	args[0].u32w0	cmd, version
 *	args[0].u32w1	error
 *	args[1].u64	sep_info
 *	args[2].u64	status
 *	args[3].u64	n
 *	data		epids
 */

static int psmx2_am_sep_match(struct dlist_entry *entry, const void *arg)
{
	struct psmx2_fid_sep *sep;

	sep = container_of(entry, struct psmx2_fid_sep, entry);
	return ((uintptr_t)sep->id == (uintptr_t)arg);
}

static void psmx2_am_sep_completion(void *buf)
{
	free(buf);
}

int psmx2_am_sep_handler(psm2_am_token_t token, psm2_amarg_t *args,
			 int nargs, void *src, uint32_t len, void *hctx)
{
	struct psmx2_fid_domain *domain;
	psm2_amarg_t rep_args[4];
	int op_error = 0;
	int err = 0;
	int cmd, version;
	int n, i, j;
	uint8_t sep_id;
	struct psmx2_fid_sep *sep;
	struct psmx2_av_sep *sep_info;
	ofi_atomic32_t *status;
	psm2_epid_t *epids;
	psm2_epid_t *buf = NULL;
	int buflen;
	struct dlist_entry *entry;
	struct psmx2_trx_ctxt *trx_ctxt = hctx;

	cmd = PSMX2_AM_GET_OP(args[0].u32w0);
	version = PSMX2_AM_GET_VER(args[0].u32w0);
	if (version != PSMX2_AM_SEP_VERSION) {
		FI_WARN(&psmx2_prov, FI_LOG_AV,
			"AM SEP protocol version mismatch: request %d handler %d\n",
			version, PSMX2_AM_SEP_VERSION);
		return -FI_EINVAL;
	}

	domain = trx_ctxt->domain;

	switch (cmd) {
	case PSMX2_AM_REQ_SEP_QUERY:
		sep_id = args[0].u32w1;
		domain->sep_lock_fn(&domain->sep_lock, 1);
		entry = dlist_find_first_match(&domain->sep_list, psmx2_am_sep_match,
					       (void *)(uintptr_t)sep_id);
		if (!entry) {
			op_error = PSM2_EPID_UNKNOWN;
			n = 0;
			buflen = 0;
		} else {
			sep = container_of(entry, struct psmx2_fid_sep, entry);
			n = sep->ctxt_cnt;
			buflen = n * sizeof(psm2_epid_t);
			if (n) {
				buf = malloc(buflen);
				if (!buf) {
					op_error = PSM2_NO_MEMORY;
					buflen = 0;
					n = 0;
				}
				for (i=0; i< n; i++)
					buf[i] = sep->ctxts[i].trx_ctxt->psm2_epid;
			}
		}
		domain->sep_unlock_fn(&domain->sep_lock, 1);

		rep_args[0].u32w0 = PSMX2_AM_REP_SEP_QUERY;
		PSMX2_AM_SET_VER(rep_args[0].u32w0, PSMX2_AM_SEP_VERSION);
		rep_args[0].u32w1 = op_error;
		rep_args[1].u64 = args[1].u64;
		rep_args[2].u64 = args[2].u64;
		rep_args[3].u64 = n;
		err = psm2_am_reply_short(token, PSMX2_AM_SEP_HANDLER,
					  rep_args, 4, buf, buflen, 0,
					  psmx2_am_sep_completion, buf);
		break;

	case PSMX2_AM_REP_SEP_QUERY:
		op_error = args[0].u32w1;
		sep_info = (struct psmx2_av_sep *)(uintptr_t)args[1].u64;
		status = (void *)(uintptr_t)args[2].u64;
		if (op_error) {
			ofi_atomic_set32(status, psmx2_errno(op_error));
		} else {
			n = args[3].u64;
			epids = malloc(n * sizeof(psm2_epid_t));
			if (!epids) {
				ofi_atomic_set32(status, -FI_ENOMEM);
			} else {
				for (j=0; j<n; j++)
					epids[j] = ((psm2_epid_t *)src)[j];
				/*
				 * the sender of the SEP query request should
				 * have acquired the lock and is waiting for
				 * the response. see psmx2_av_query_sep().
				 */
				sep_info->ctxt_cnt = n;
				sep_info->epids = epids;
				ofi_atomic_set32(status, 0);
			}
		}
		break;

	default:
		err = -FI_EINVAL;
		break;
	}

	return err;
}

static void psmx2_set_epaddr_context(struct psmx2_trx_ctxt *trx_ctxt,
				     psm2_epid_t epid, psm2_epaddr_t epaddr)
{
	struct psmx2_epaddr_context *context;
	struct psmx2_epaddr_context *old_context = NULL;

	context = (void *)psm2_epaddr_getctxt(epaddr);
	if (context) {
		if (context->trx_ctxt != trx_ctxt || context->epid != epid) {
			FI_WARN(&psmx2_prov, FI_LOG_AV,
				"trx_ctxt or epid doesn't match\n");
			old_context = context;
			context = NULL;
		}
	}

	if (context)
		return;

	context = malloc(sizeof *context);
	if (!context) {
		FI_WARN(&psmx2_prov, FI_LOG_AV,
			"cannot allocate context\n");
		return;
	}

	context->trx_ctxt = trx_ctxt;
	context->epid = epid;
	context->epaddr = epaddr;
	psm2_epaddr_setctxt(epaddr, context);
	free(old_context);

	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_insert_before(&context->entry, &trx_ctxt->peer_list);
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);
}

void psmx2_epid_to_epaddr(struct psmx2_trx_ctxt *trx_ctxt,
			  psm2_epid_t epid, psm2_epaddr_t *epaddr)
{
	int err;
	psm2_error_t errors;
	psm2_epconn_t epconn;
	struct psmx2_epaddr_context *context;

	err = psm2_ep_epid_lookup2(trx_ctxt->psm2_ep, epid, &epconn);
	if (err == PSM2_OK) {
		context = psm2_epaddr_getctxt(epconn.addr);
		if (context && context->epid  == epid) {
			*epaddr = epconn.addr;
			return;
		}
	}

	err = psm2_ep_connect(trx_ctxt->psm2_ep, 1, &epid, NULL, &errors, epaddr,
			      (int64_t) psmx2_env.conn_timeout * 1000000000LL);
	if (err == PSM2_OK || err == PSM2_EPID_ALREADY_CONNECTED) {
		psmx2_set_epaddr_context(trx_ctxt, epid, *epaddr);
		return;
	}

	/* call fi_log() directly to always generate the output */
	fi_log(&psmx2_prov, FI_LOG_WARN, FI_LOG_AV, __func__, __LINE__,
		"psm2_ep_connect retured error %s, remote epid=%lx."
		"If it is a timeout error, try setting FI_PSM2_CONN_TIMEOUT "
		"to a larger value (current: %d seconds).\n",
		psm2_error_get_string(err), epid, psmx2_env.conn_timeout);

	abort();
}

/*
 * Must be called with av->lock held
 */
static int psmx2_av_check_space(struct psmx2_fid_av *av, size_t count)
{
	psm2_epaddr_t *new_epaddrs;
	psm2_epaddr_t **new_sepaddrs;
	struct psmx2_av_hdr *new_hdr;
	struct psmx2_av_sep *new_sep_info;
	size_t new_count;
	size_t old_table_size, new_table_size;
	int i;

	new_count = av->count;
	while (new_count < av->hdr->last + count)
		new_count = new_count * 2;

	if ((new_count <= av->count) && av->table)
		return 0;

	old_table_size = PSMX2_AV_TABLE_SIZE(av->count, av->shared);
	new_table_size = PSMX2_AV_TABLE_SIZE(new_count, av->shared);
	if (av->shared) {
		new_hdr = mremap(av->hdr, old_table_size, new_table_size, 0);
		if (new_hdr == MAP_FAILED)
			return -FI_ENOMEM;
		av->hdr = new_hdr;
		av->map = (fi_addr_t *)(av->hdr + 1);
		av->table = (struct psmx2_av_addr *)(av->map + new_count);
		for (i = 0; i < new_count; i++)
			av->map[i] = i;
	} else {
		new_hdr = realloc(av->hdr, new_table_size);
		if (!new_hdr)
			return -FI_ENOMEM;
		av->hdr = new_hdr;
		av->table = (struct psmx2_av_addr *)(av->hdr + 1);
	}

	new_sep_info = realloc(av->sep_info, new_count * sizeof(*new_sep_info));
	if (!new_sep_info)
		return -FI_ENOMEM;
	av->sep_info = new_sep_info;

	for (i = 0; i < av->max_trx_ctxt; i++) {
		if (!av->conn_info[i].trx_ctxt)
			continue;

		new_epaddrs = realloc(av->conn_info[i].epaddrs,
				      new_count * sizeof(*new_epaddrs));
		if (!new_epaddrs)
			return -FI_ENOMEM;
		memset(new_epaddrs + av->hdr->last, 0,
		       (new_count - av->hdr->last)  * sizeof(*new_epaddrs));
		av->conn_info[i].epaddrs = new_epaddrs;

		new_sepaddrs = realloc(av->conn_info[i].sepaddrs,
				       new_count * sizeof(*new_sepaddrs));
		if (!new_sepaddrs)
			return -FI_ENOMEM;
		memset(new_sepaddrs + av->hdr->last, 0,
		       (new_count - av->hdr->last)  * sizeof(*new_sepaddrs));
		av->conn_info[i].sepaddrs = new_sepaddrs;
	}

	av->count = av->hdr->size = new_count;
	return 0;
}

static void psmx2_av_post_completion(struct psmx2_fid_av *av, void *context,
				     uint64_t data, int prov_errno)
{
	if (prov_errno) {
		struct fi_eq_err_entry entry;
		entry.fid = &av->av.fid;
		entry.context = context;
		entry.data = data;
		entry.err = -psmx2_errno(prov_errno);
		entry.prov_errno = prov_errno;
		entry.err_data = NULL;
		entry.err_data_size = 0;
		fi_eq_write(av->eq, FI_AV_COMPLETE, &entry, sizeof(entry),
			    UTIL_FLAG_ERROR);
	} else {
		struct fi_eq_entry entry;
		entry.fid = &av->av.fid;
		entry.context = context;
		entry.data = data;
		fi_eq_write(av->eq, FI_AV_COMPLETE, &entry, sizeof(entry), 0);
	}
}

/*
 * Must be called with av->lock held
 */
int psmx2_av_query_sep(struct psmx2_fid_av *av,
		       struct psmx2_trx_ctxt *trx_ctxt,
		       size_t idx)
{
	ofi_atomic32_t status; /* 1: pending, 0: succ, <0: error */
	psm2_amarg_t args[3];
	int error;

	if (!av->conn_info[trx_ctxt->id].epaddrs[idx])
		psmx2_epid_to_epaddr(trx_ctxt, av->table[idx].epid,
				     &av->conn_info[trx_ctxt->id].epaddrs[idx]);

	psmx2_am_init(trx_ctxt); /* check AM handler installation */

	ofi_atomic_initialize32(&status, 1);

	args[0].u32w0 = PSMX2_AM_REQ_SEP_QUERY;
	PSMX2_AM_SET_VER(args[0].u32w0, PSMX2_AM_SEP_VERSION);
	args[0].u32w1 = av->table[idx].sep_id;
	args[1].u64 = (uint64_t)(uintptr_t)&av->sep_info[idx];
	args[2].u64 = (uint64_t)(uintptr_t)&status;
	error = psm2_am_request_short(av->conn_info[trx_ctxt->id].epaddrs[idx],
				      PSMX2_AM_SEP_HANDLER, args, 3, NULL,
				      0, 0, NULL, NULL);

	if (error)
		return error;

	/*
	 * make sure AM is progressed promptly. don't call
	 * psmx2_progress() which may call functions that
	 * need to access the address vector.
	 */
	while (ofi_atomic_get32(&status) == 1)
		psm2_poll(trx_ctxt->psm2_ep);

	error = (int)(int32_t)ofi_atomic_get32(&status);

	return error;
}

int psmx2_av_add_trx_ctxt(struct psmx2_fid_av *av,
			  struct psmx2_trx_ctxt *trx_ctxt)
{
	int id;
	int err = 0;

	av->domain->av_lock_fn(&av->lock, 1);

	if (av->type == FI_AV_MAP) {
		av->av_map_trx_ctxt = trx_ctxt;
		goto out;
	}

	id = trx_ctxt->id;
	if (id >= av->max_trx_ctxt) {
		FI_WARN(&psmx2_prov, FI_LOG_AV,
			"trx_ctxt->id(%d) exceeds av->max_trx_ctxt(%d).\n",
			id, av->max_trx_ctxt);
		err = -FI_EINVAL;
		goto out;
	}

	if (av->conn_info[id].trx_ctxt) {
		if (av->conn_info[id].trx_ctxt == trx_ctxt) {
			FI_INFO(&psmx2_prov, FI_LOG_AV,
				"trx_ctxt(%p) with id(%d) already added.\n",
				trx_ctxt, id);
			goto out;
		} else {
			FI_INFO(&psmx2_prov, FI_LOG_AV,
				"different trx_ctxt(%p) with same id(%d) already added.\n",
				trx_ctxt, id);
			err = -FI_EINVAL;
			goto out;
		}
	}

	av->conn_info[id].epaddrs = (psm2_epaddr_t *) calloc(av->count,
							  sizeof(psm2_epaddr_t));
	if (!av->conn_info[id].epaddrs) {
		err = -FI_ENOMEM;
		goto out;
	}

	av->conn_info[id].sepaddrs = (psm2_epaddr_t **)calloc(av->count,
							   sizeof(psm2_epaddr_t *));
	if (!av->conn_info[id].sepaddrs) {
		err = -FI_ENOMEM;
		goto out;
	}

	av->conn_info[id].trx_ctxt = trx_ctxt;

out:
	av->domain->av_unlock_fn(&av->lock, 1);
	return err;
}

DIRECT_FN
STATIC int psmx2_av_insert(struct fid_av *av, const void *addr,
			   size_t count, fi_addr_t *fi_addr,
			   uint64_t flags, void *context)
{
	struct psmx2_fid_av *av_priv;
	struct psmx2_ep_name *ep_name;
	const struct psmx2_ep_name *names = addr;
	const char **string_names = (void *)addr;
	psm2_error_t *errors = NULL;
	int error_count = 0;
	int i, idx, ret;

	assert(addr || !count);

	av_priv = container_of(av, struct psmx2_fid_av, av);

	av_priv->domain->av_lock_fn(&av_priv->lock, 1);

	if ((av_priv->flags & FI_EVENT) && !av_priv->eq) {
		ret = -FI_ENOEQ;
		goto out;
	}

	if (av_priv->flags & FI_READ) {
		ret = -FI_EINVAL;
		goto out;
	}

	if (psmx2_av_check_space(av_priv, count)) {
		ret = -FI_ENOMEM;
		goto out;
	}

	errors = calloc(count, sizeof(*errors));
	if (!errors) {
		ret = -FI_ENOMEM;
		goto out;
	}

	/* save the peer address information */
	for (i = 0; i < count; i++) {
		idx = av_priv->hdr->last + i;
		if (av_priv->addr_format == FI_ADDR_STR) {
			ep_name = psmx2_string_to_ep_name(string_names[i]);
			if (!ep_name) {
				ret = -FI_EINVAL;
				goto out;
			}
			av_priv->table[idx].type = ep_name->type;
			av_priv->table[idx].epid = ep_name->epid;
			av_priv->table[idx].sep_id = ep_name->sep_id;
			av_priv->table[idx].valid = 1;
			free(ep_name);
		} else {
			av_priv->table[idx].type = names[i].type;
			av_priv->table[idx].epid = names[i].epid;
			av_priv->table[idx].sep_id = names[i].sep_id;
			av_priv->table[idx].valid = 1;
		}
		av_priv->sep_info[idx].ctxt_cnt = 1;
		av_priv->sep_info[idx].epids = NULL;
	}

	if (fi_addr) {
		for (i = 0; i < count; i++) {
			idx = av_priv->hdr->last + i;
			if (errors[i] != PSM2_OK)
				fi_addr[i] = FI_ADDR_NOTAVAIL;
			else
				fi_addr[i] = idx;
		}
	}

	av_priv->hdr->last += count;

	if (av_priv->flags & FI_EVENT) {
		if (error_count) {
			for (i = 0; i < count; i++)
				psmx2_av_post_completion(av_priv, context, i, errors[i]);
		}
		psmx2_av_post_completion(av_priv, context, count - error_count, 0);
		ret = 0;
	} else {
		if (flags & FI_SYNC_ERR) {
			int *fi_errors = context;
			for (i=0; i<count; i++)
				fi_errors[i] = psmx2_errno(errors[i]);
		}
		ret = count - error_count;
	}

out:
	free(errors);
	av_priv->domain->av_unlock_fn(&av_priv->lock, 1);
	return ret;
}

DIRECT_FN
STATIC int psmx2_av_map_insert(struct fid_av *av, const void *addr,
			       size_t count, fi_addr_t *fi_addr,
			       uint64_t flags, void *context)
{
	struct psmx2_fid_av *av_priv;
	struct psmx2_trx_ctxt *trx_ctxt;
	struct psmx2_ep_name *ep_name;
	const struct psmx2_ep_name *names = addr;
	const char **string_names = (void *)addr;
	psm2_epid_t *epids = NULL;
	psm2_epaddr_t *epaddrs = NULL;
	psm2_error_t *errors = NULL;
	int error_count = 0;
	int i, ret, err = 0;

	assert(addr || !count);

	av_priv = container_of(av, struct psmx2_fid_av, av);

	av_priv->domain->av_lock_fn(&av_priv->lock, 1);

	if (!count)
		goto out;

	epids = calloc(count, sizeof(*epids));
	errors = calloc(count, sizeof(*errors));
	if (!epids || !errors) {
		err = -FI_ENOMEM;
		goto out;
	}

	for (i=0; i<count; i++) {
		if (av_priv->addr_format == FI_ADDR_STR) {
			ep_name = psmx2_string_to_ep_name(string_names[i]);
			if (!ep_name) {
				err = -FI_EINVAL;
				goto out;
			}
			epids[i] = ep_name->epid;
			free(ep_name);
		} else {
			epids[i] = names[i].epid;
		}
	}

	epaddrs = (psm2_epaddr_t *)fi_addr;

	trx_ctxt = av_priv->av_map_trx_ctxt;
	if (!trx_ctxt) {
		FI_WARN(&psmx2_prov, FI_LOG_AV,
			"unable to map address without AV-EP binding\n");
		err = -FI_ENODEV;
		goto out;
	}

	psm2_ep_connect(trx_ctxt->psm2_ep, count, epids, NULL, errors, epaddrs,
			(int64_t) psmx2_env.conn_timeout * count * 1000000000LL);

	for (i=0; i<count; i++) {
		if (errors[i] == PSM2_EPID_ALREADY_CONNECTED)
			errors[i] = PSM2_OK;

		if (errors[i] == PSM2_OK)
			psmx2_set_epaddr_context(trx_ctxt, epids[i], epaddrs[i]);
		else
			error_count++;
	}

out:
	if (av_priv->flags & FI_EVENT) {
		if (!err) {
			if (error_count) {
				for (i = 0; i < count; i++)
					psmx2_av_post_completion(av_priv, context, i, errors[i]);
			}
			psmx2_av_post_completion(av_priv, context, count - error_count, 0);
		}
		ret = err;
	} else {
		if (flags & FI_SYNC_ERR) {
			int *fi_errors = context;
			for (i=0; i<count; i++)
				fi_errors[i] = err ? err : psmx2_errno(errors[i]);
		}
		ret = err ? 0 : count - error_count;
	}

	if (count) {
		free(errors);
		free(epids);
	}

	av_priv->domain->av_unlock_fn(&av_priv->lock, 1);

	return ret;
}

static int psmx2_av_disconnect_addr(int trx_ctxt_id, psm2_epid_t epid,
				    psm2_epaddr_t epaddr)
{
	struct psmx2_epaddr_context *epaddr_context;
	struct psmx2_trx_ctxt *trx_ctxt;
	psm2_error_t errors;
	int err;

	if (!epaddr)
		return 0;

	FI_INFO(&psmx2_prov, FI_LOG_AV,
		"trx_ctxt_id %d epid %lx epaddr %p\n", trx_ctxt_id, epid, epaddr);

	epaddr_context = psm2_epaddr_getctxt(epaddr);
	if (!epaddr_context)
		return -FI_EINVAL;

	trx_ctxt = epaddr_context->trx_ctxt;
	if (trx_ctxt_id != trx_ctxt->id)
		return -FI_EINVAL;

	if (epid != epaddr_context->epid)
		return -FI_EINVAL;

	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	dlist_remove_first_match(&trx_ctxt->peer_list,
				 psmx2_peer_match, epaddr);
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);

	psm2_epaddr_setctxt(epaddr, NULL);

	err = psm2_ep_disconnect2(trx_ctxt->psm2_ep, 1, &epaddr,
				  NULL, &errors, PSM2_EP_DISCONNECT_FORCE, 0);

	free(epaddr_context);
	return psmx2_errno(err);
}

DIRECT_FN
STATIC int psmx2_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			   uint64_t flags)
{
	struct psmx2_fid_av *av_priv;
	int idx, i, j, k;
	int err;

	av_priv = container_of(av, struct psmx2_fid_av, av);

	av_priv->domain->av_lock_fn(&av_priv->lock, 1);

	for (i = 0; i < count; i++) {
		idx = PSMX2_ADDR_IDX(fi_addr[i]);
		if (idx >= av_priv->hdr->last) {
			FI_WARN(&psmx2_prov, FI_LOG_AV,
				"AV index out of range: fi_addr %lx idx %d last %ld\n",
				fi_addr[i], idx, av_priv->hdr->last);
			continue;
		}

		if (av_priv->table[idx].type == PSMX2_EP_REGULAR) {
			for (j = 0; j < av_priv->max_trx_ctxt; j++) {
				if (!av_priv->conn_info[j].trx_ctxt)
					continue;

				err = psmx2_av_disconnect_addr(
						j, av_priv->table[idx].epid,
						av_priv->conn_info[j].epaddrs[idx]);
				if (!err)
					av_priv->conn_info[j].epaddrs[idx] = NULL;
			}
			av_priv->table[idx].epid = 0;
		} else {
			if (!av_priv->sep_info[idx].epids)
				continue;

			for (j = 0; j < av_priv->max_trx_ctxt; j++) {
				if (!av_priv->conn_info[j].trx_ctxt)
					continue;

				if (!av_priv->conn_info[j].sepaddrs[idx])
					continue;

				for (k = 0; k < av_priv->sep_info[idx].ctxt_cnt; k++) {
					err = psmx2_av_disconnect_addr(
							j, av_priv->sep_info[idx].epids[k],
							av_priv->conn_info[j].sepaddrs[idx][k]);
					if (!err)
						av_priv->conn_info[j].sepaddrs[idx][k] = NULL;
				}
			}
			free(av_priv->sep_info[idx].epids);
			av_priv->sep_info[idx].epids = NULL;
		}
		av_priv->table[idx].valid = 0;
	}

	av_priv->domain->av_unlock_fn(&av_priv->lock, 1);

	return 0;
}

DIRECT_FN
STATIC int psmx2_av_map_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			       uint64_t flags)
{
	struct psmx2_fid_av *av_priv;
	struct psmx2_trx_ctxt *trx_ctxt;
	psm2_error_t *errors;
	int i;

	av_priv = container_of(av, struct psmx2_fid_av, av);

	if (!count)
		return 0;

	trx_ctxt = av_priv->av_map_trx_ctxt;
	if (!trx_ctxt)
		return -FI_ENODEV;

	errors = calloc(count, sizeof(*errors));
	if (!errors)
		return -FI_ENOMEM;

	trx_ctxt->domain->peer_lock_fn(&trx_ctxt->peer_lock, 2);
	for (i = 0; i < count; i++) {
		dlist_remove_first_match(&trx_ctxt->peer_list,
					 psmx2_peer_match,
					 (psm2_epaddr_t)(fi_addr[i]));
	}
	trx_ctxt->domain->peer_unlock_fn(&trx_ctxt->peer_lock, 2);

	for (i = 0; i < count; i++)
		psm2_epaddr_setctxt((psm2_epaddr_t)(fi_addr[i]), NULL);

	psm2_ep_disconnect2(trx_ctxt->psm2_ep, count, (psm2_epaddr_t *)fi_addr,
			    NULL, errors, PSM2_EP_DISCONNECT_FORCE, 0);

	free(errors);
	return 0;
}

DIRECT_FN
STATIC int psmx2_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			   size_t *addrlen)
{
	struct psmx2_fid_av *av_priv;
	struct psmx2_ep_name name;
	int idx = PSMX2_ADDR_IDX(fi_addr);
	int err = 0;

	assert(addr);
	assert(addrlen);

	av_priv = container_of(av, struct psmx2_fid_av, av);

	memset(&name, 0, sizeof(name));

	av_priv->domain->av_lock_fn(&av_priv->lock, 1);

	if (idx >= av_priv->hdr->last) {
		err = -FI_EINVAL;
		goto out;
	}

	if (!av_priv->table[idx].valid) {
		err = -FI_EINVAL;
		goto out;
	}

	name.type = av_priv->table[idx].type;
	name.epid = av_priv->table[idx].epid;
	name.sep_id = av_priv->table[idx].sep_id;

	if (av_priv->addr_format == FI_ADDR_STR) {
		ofi_straddr(addr, addrlen, FI_ADDR_PSMX2, &name);
	} else {
		memcpy(addr, &name, MIN(*addrlen, sizeof(name)));
		*addrlen = sizeof(name);
	}

out:
	av_priv->domain->av_unlock_fn(&av_priv->lock, 1);
	return err;
}

DIRECT_FN
STATIC int psmx2_av_map_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			       size_t *addrlen)
{
	struct psmx2_fid_av *av_priv;
	struct psmx2_ep_name name;

	assert(addr);
	assert(addrlen);

	av_priv = container_of(av, struct psmx2_fid_av, av);

	memset(&name, 0, sizeof(name));
	psm2_epaddr_to_epid((psm2_epaddr_t)fi_addr, &name.epid);
	name.type = PSMX2_EP_REGULAR;

	if (av_priv->addr_format == FI_ADDR_STR) {
		ofi_straddr(addr, addrlen, FI_ADDR_PSMX2, &name);
	} else {
		memcpy(addr, &name, MIN(*addrlen, sizeof(name)));
		*addrlen = sizeof(name);
	}

	return 0;
}

fi_addr_t psmx2_av_translate_source(struct psmx2_fid_av *av,
				    psm2_epaddr_t source, int source_sep_id)
{
	psm2_epid_t epid;
	fi_addr_t ret;
	int i, j, found;
	int ep_type = source_sep_id ? PSMX2_EP_SCALABLE : PSMX2_EP_REGULAR;

	if (av->type == FI_AV_MAP)
		return (fi_addr_t) source;

	psm2_epaddr_to_epid(source, &epid);

	av->domain->av_lock_fn(&av->lock, 1);

	ret = FI_ADDR_NOTAVAIL;
	found = 0;
	for (i = av->hdr->last - 1; i >= 0 && !found; i--) {
		if (!av->table[i].valid)
			continue;

		if (av->table[i].type == PSMX2_EP_REGULAR) {
			if (ep_type == PSMX2_EP_SCALABLE)
				continue;
			if (av->table[i].epid == epid) {
				ret = (fi_addr_t)i;
				found = 1;
			}
		} else {
			/*
			 * scalable endpoint must match sep_id exactly.
			 * regular endpoint can match a context of any
			 * scalable endpoint.
			 */
			if (ep_type == PSMX2_EP_SCALABLE &&
			    av->table[i].sep_id != source_sep_id)
				continue;

			if (!av->sep_info[i].epids) {
				for (j = 0; j < av->max_trx_ctxt; j++) {
					if (av->conn_info[j].trx_ctxt)
						break;
				}
				if (j >= av->max_trx_ctxt)
					continue;
				psmx2_av_query_sep(av, av->conn_info[j].trx_ctxt, i);
				if (!av->sep_info[i].epids)
					continue;
			}

			for (j=0; j<av->sep_info[i].ctxt_cnt; j++) {
				if (av->sep_info[i].epids[j] == epid) {
					ret = fi_rx_addr((fi_addr_t)i, j,
							 av->rx_ctx_bits);
					found = 1;
					break;
				}
			}
		}
	}

	av->domain->av_unlock_fn(&av->lock, 1);
	return ret;
}

void psmx2_av_remove_conn(struct psmx2_fid_av *av,
			  struct psmx2_trx_ctxt *trx_ctxt,
			  psm2_epaddr_t epaddr)
{
	psm2_epid_t epid;
	int i, j;

	if (av->type == FI_AV_MAP)
		return;

	psm2_epaddr_to_epid(epaddr, &epid);

	av->domain->av_lock_fn(&av->lock, 1);

	for (i = 0; i < av->hdr->last; i++) {
		if (!av->table[i].valid)
			continue;
		if (av->table[i].type == PSMX2_EP_REGULAR) {
			if (av->table[i].epid == epid &&
			    av->conn_info[trx_ctxt->id].epaddrs[i] == epaddr)
				av->conn_info[trx_ctxt->id].epaddrs[i] = NULL;
		} else {
			if (!av->sep_info[i].epids)
				continue;
			for (j=0; j<av->sep_info[i].ctxt_cnt; j++) {
				if (av->sep_info[i].epids[j] == epid &&
				    av->conn_info[trx_ctxt->id].sepaddrs[i] &&
				    av->conn_info[trx_ctxt->id].sepaddrs[i][j] == epaddr)
					    av->conn_info[trx_ctxt->id].sepaddrs[i][j] = NULL;
			}
		}
	}

	av->domain->av_unlock_fn(&av->lock, 1);
}

DIRECT_FN
STATIC const char *psmx2_av_straddr(struct fid_av *av, const void *addr,
				    char *buf, size_t *len)
{
	return ofi_straddr(buf, len, FI_ADDR_PSMX2, addr);
}

static int psmx2_av_close(fid_t fid)
{
	struct psmx2_fid_av *av;
	int i, j;
	int err;

	av = container_of(fid, struct psmx2_fid_av, av.fid);
	psmx2_domain_release(av->domain);
	ofi_spin_destroy(&av->lock);

	if (av->type == FI_AV_MAP)
		goto out;

	for (i = 0; i < av->max_trx_ctxt; i++) {
		if (!av->conn_info[i].trx_ctxt)
			continue;
		free(av->conn_info[i].epaddrs);
		if (av->conn_info[i].sepaddrs) {
			for (j = 0; j < av->hdr->last; j++)
				free(av->conn_info[i].sepaddrs[j]);
		}
		free(av->conn_info[i].sepaddrs);
	}
	if (av->shared) {
		err = ofi_shm_unmap(&av->shm);
		if (err)
			FI_INFO(&psmx2_prov, FI_LOG_AV,
				"Failed to unmap shared AV: %s.\n",
				strerror(ofi_syserr()));
	} else {
		free(av->hdr);
	}

	free(av->sep_info);
out:
	free(av);
	return 0;
}

DIRECT_FN
STATIC int psmx2_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx2_fid_av *av;

	av = container_of(fid, struct psmx2_fid_av, av.fid);

	assert(bfid);

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		av->eq = (struct fid_eq *)bfid;
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static struct fi_ops psmx2_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_av_close,
	.bind = psmx2_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av psmx2_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = psmx2_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = psmx2_av_remove,
	.lookup = psmx2_av_lookup,
	.straddr = psmx2_av_straddr,
};

static struct fi_ops_av psmx2_av_map_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = psmx2_av_map_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = psmx2_av_map_remove,
	.lookup = psmx2_av_map_lookup,
	.straddr = psmx2_av_straddr,
};

DIRECT_FN
int psmx2_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		  struct fid_av **av, void *context)
{
	struct psmx2_fid_domain *domain_priv;
	struct psmx2_fid_av *av_priv;
	size_t count = PSMX2_AV_DEFAULT_SIZE;
	uint64_t flags = 0;
	int shared = 0;
	int rx_ctx_bits = PSMX2_MAX_RX_CTX_BITS;
	size_t conn_size;
	size_t table_size;
	int av_type = FI_AV_TABLE;
	int err;
	int i;

	domain_priv = container_of(domain, struct psmx2_fid_domain,
				   util_domain.domain_fid);

	if (attr) {
		if (attr->type == FI_AV_MAP) {
			if (psmx2_env.multi_ep) {
				FI_INFO(&psmx2_prov, FI_LOG_AV,
					"FI_AV_MAP asked, but force FI_AV_TABLE for multi-EP support\n");
			} else if (psmx2_env.lazy_conn) {
				FI_INFO(&psmx2_prov, FI_LOG_AV,
					"FI_AV_MAP asked, but force FI_AV_TABLE for lazy connection\n");
			} else if (attr->name) {
				FI_INFO(&psmx2_prov, FI_LOG_AV,
					"FI_AV_MAP asked, but force FI_AV_TABLE for shared AV\n");
			} else {
				FI_INFO(&psmx2_prov, FI_LOG_AV,
					"FI_AV_MAP asked, and granted\n");
				av_type = FI_AV_MAP;
			}
		}

		if (attr->count)
			count = attr->count;

		if (attr->name)
			shared = 1;

		flags = attr->flags;
		if (flags & FI_SYMMETRIC) {
			FI_INFO(&psmx2_prov, FI_LOG_AV,
				"FI_SYMMETRIC flags is no supported\n");
			return -FI_ENOSYS;
		}

		if (attr->rx_ctx_bits > PSMX2_MAX_RX_CTX_BITS) {
			FI_INFO(&psmx2_prov, FI_LOG_AV,
				"attr->rx_ctx_bits=%d, maximum allowed is %d\n",
				attr->rx_ctx_bits, PSMX2_MAX_RX_CTX_BITS);
			return -FI_ENOSYS;
		}

		rx_ctx_bits = attr->rx_ctx_bits;
	}

	if (av_type == FI_AV_MAP)
		conn_size = 0;
	else
		conn_size = psmx2_hfi_info.max_trx_ctxt * sizeof(struct psmx2_av_conn);

	av_priv = (struct psmx2_fid_av *) calloc(1, sizeof(*av_priv) + conn_size);
	if (!av_priv)
		return -FI_ENOMEM;

	if (av_type == FI_AV_MAP)
		goto init_lock;

	av_priv->sep_info = calloc(count, sizeof(struct psmx2_av_sep));
	if (!av_priv->sep_info) {
		err = -FI_ENOMEM;
		goto errout_free;
	}

	table_size = PSMX2_AV_TABLE_SIZE(count, shared);
	if (attr && attr->name) {
		err = ofi_shm_map(&av_priv->shm, attr->name, table_size,
				  flags & FI_READ, (void**)&av_priv->hdr);
		if (err || av_priv->hdr == MAP_FAILED) {
			FI_WARN(&psmx2_prov, FI_LOG_AV,
				"failed to map shared AV: %s\n", attr->name);
			err = -FI_EINVAL;
			goto errout_free;
		}

		if (flags & FI_READ) {
			if (av_priv->hdr->size != count) {
				FI_WARN(&psmx2_prov, FI_LOG_AV,
					"AV size doesn't match: shared %ld, asking %ld\n",
					av_priv->hdr->size, count);
				err = -FI_EINVAL;
				goto errout_free;
			}
		} else {
			av_priv->hdr->size = count;
			av_priv->hdr->last = 0;
		}
		av_priv->shared = 1;
		av_priv->map = (fi_addr_t *)(av_priv->hdr + 1);
		av_priv->table = (struct psmx2_av_addr *)(av_priv->map + count);
		for (i = 0; i < count; i++)
			av_priv->map[i] = i;
	} else {
		av_priv->hdr = calloc(1, table_size);
		if (!av_priv->hdr) {
			err = -FI_ENOMEM;
			goto errout_free;
		}
		av_priv->hdr->size = count;
		av_priv->table = (struct psmx2_av_addr *)(av_priv->hdr + 1);
	}

init_lock:
	ofi_spin_init(&av_priv->lock);

	psmx2_domain_acquire(domain_priv);

	av_priv->domain = domain_priv;
	av_priv->addrlen = sizeof(psm2_epaddr_t);
	av_priv->count = count;
	av_priv->flags = flags;
	av_priv->rx_ctx_bits = rx_ctx_bits;
	av_priv->max_trx_ctxt = psmx2_hfi_info.max_trx_ctxt;
	av_priv->addr_format = domain_priv->addr_format;
	av_priv->type = av_type;

	av_priv->av.fid.fclass = FI_CLASS_AV;
	av_priv->av.fid.context = context;
	av_priv->av.fid.ops = &psmx2_fi_ops;
	if (av_type == FI_AV_MAP)
		av_priv->av.ops = &psmx2_av_map_ops;
	else
		av_priv->av.ops = &psmx2_av_ops;

	*av = &av_priv->av;
	if (attr) {
		attr->type = av_type;
		if (shared)
			attr->map_addr = av_priv->map;
	}

	return 0;

errout_free:
	free(av_priv->sep_info);
	free(av_priv);
	return err;
}

