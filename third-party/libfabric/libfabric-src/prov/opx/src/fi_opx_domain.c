/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 by Cornelis Networks.
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


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>

#include "rdma/fabric.h"

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_hfi1.h"

#include <ofi_enosys.h>

#include "rdma/opx/fi_opx.h"

static int fi_opx_close_domain(fid_t fid)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "close domain\n");

	int ret;
	struct fi_opx_domain *opx_domain =
		container_of(fid, struct fi_opx_domain, domain_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	ret = fi_opx_finalize_mr_ops(&opx_domain->domain_fid);
	if (ret)
		return ret;

	fi_opx_close_tid_domain(opx_domain->tid_domain);

	ret = fi_opx_ref_finalize(&opx_domain->ref_cnt, "domain");
	if (ret)
		return ret;

	ret = fi_opx_ref_dec(&opx_domain->fabric->ref_cnt, "fabric");
	if (ret)
		return ret;


	free(opx_domain);
	opx_domain = NULL;
	//opx_domain (the object passed in as fid) is now unusable

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "domain closed\n");
	return 0;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_domain,
	.bind		= fi_no_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

static struct fi_ops_domain fi_opx_domain_ops = {
	.size		= sizeof(struct fi_ops_domain),
	.av_open	= fi_opx_av_open,
	.cq_open	= fi_opx_cq_open,
	.endpoint	= fi_opx_endpoint,
	.scalable_ep = fi_opx_scalable_ep,
	.cntr_open	= fi_opx_cntr_open,
	.poll_open	= fi_no_poll_open,
	.stx_ctx	= fi_opx_stx_context,
	.srx_ctx	= fi_no_srx_context
};


int fi_opx_alloc_default_domain_attr(struct fi_domain_attr **domain_attr)
{
	struct fi_domain_attr *attr;

	attr = calloc(1, sizeof(*attr));
	if (!attr)
		goto err;

	uint32_t ppn = 1; /* TODO */
	const unsigned tx_ctx_cnt = 160 / ppn;
	const unsigned rx_ctx_cnt = 160 / ppn;

	attr->domain		= NULL;
	attr->name  		= strdup(FI_OPX_DOMAIN_NAME);

	attr->threading		= OPX_THREAD;
	attr->control_progress 	= FI_PROGRESS_MANUAL;
	attr->data_progress	= FI_PROGRESS_MANUAL;
	attr->resource_mgmt	= FI_RM_DISABLED;
	attr->av_type		= OPX_AV;
	attr->mr_mode		= OPX_MR;
	attr->mr_key_size 	= sizeof(uint64_t);
	attr->cq_data_size 	= FI_OPX_REMOTE_CQ_DATA_SIZE;
	attr->cq_cnt		= (size_t)-1;
	attr->ep_cnt		= 160/ppn;
	attr->tx_ctx_cnt	= tx_ctx_cnt;
	attr->rx_ctx_cnt	= rx_ctx_cnt;

    // For now, keep these 1-1
	attr->max_ep_tx_ctx	= FI_OPX_ADDR_SEP_RX_MAX;
	attr->max_ep_rx_ctx	= FI_OPX_ADDR_SEP_RX_MAX;

	attr->max_ep_stx_ctx	= SIZE_MAX;
	attr->max_ep_srx_ctx	= SIZE_MAX;
	attr->mr_iov_limit	= 1;

	*domain_attr = attr;

	return 0;
err:
	*domain_attr = NULL;
	errno = FI_ENOMEM;
	return -1;
}

int fi_opx_choose_domain(uint64_t caps, struct fi_domain_attr *domain_attr, struct fi_domain_attr *hints, enum fi_progress progress)
{
	if (!domain_attr) {
		FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "missing domain attribute structure\n");
		goto err;
	}

	*domain_attr = *fi_opx_global.default_domain_attr;
	domain_attr->data_progress = progress;

#ifdef OPX_ENABLED
	/* Set the data progress mode to the option used in the configure.
 	 * Ignore any setting by the application.
 	 */

	/* Set the mr_mode to the option used in the configure.
 	 * Ignore any setting by the application - the checkinfo should have verified
 	 * it was set to the same setting.
 	 */
	domain_attr->mr_mode = OPX_MR;
#endif

	if (hints) {
		if (hints->domain) {
			struct fi_opx_domain *opx_domain = container_of(hints->domain, struct fi_opx_domain, domain_fid);

			domain_attr->threading		= opx_domain->threading;
			domain_attr->resource_mgmt	= opx_domain->resource_mgmt;
			domain_attr->tx_ctx_cnt		= fi_opx_domain_get_tx_max(hints->domain);
			domain_attr->rx_ctx_cnt		= fi_opx_domain_get_rx_max(hints->domain);
			domain_attr->max_ep_tx_ctx	= fi_opx_domain_get_tx_max(hints->domain);
			domain_attr->max_ep_rx_ctx	= fi_opx_domain_get_rx_max(hints->domain);
			domain_attr->max_ep_stx_ctx	= fi_opx_domain_get_tx_max(hints->domain);

		} else {

			if (hints->threading)		domain_attr->threading = hints->threading;
			if (hints->control_progress)	domain_attr->control_progress = hints->control_progress;
			if (hints->resource_mgmt)	domain_attr->resource_mgmt = hints->resource_mgmt;
			if (hints->av_type)		domain_attr->av_type = hints->av_type;
			if (hints->mr_key_size)		domain_attr->mr_key_size = hints->mr_key_size;
			if (hints->cq_data_size)	domain_attr->cq_data_size = hints->cq_data_size;
			if (hints->cq_cnt)		domain_attr->cq_cnt = hints->cq_cnt;
			if (hints->ep_cnt)		domain_attr->ep_cnt = hints->ep_cnt;
			if (hints->tx_ctx_cnt)		domain_attr->tx_ctx_cnt = hints->tx_ctx_cnt;
			if (hints->rx_ctx_cnt)		domain_attr->rx_ctx_cnt = hints->rx_ctx_cnt;
			if (hints->max_ep_tx_ctx)	domain_attr->max_ep_tx_ctx = hints->max_ep_tx_ctx;
			if (hints->max_ep_rx_ctx)	domain_attr->max_ep_rx_ctx = hints->max_ep_rx_ctx;
			if (hints->max_ep_stx_ctx)	domain_attr->max_ep_stx_ctx = hints->max_ep_stx_ctx;
			if (hints->max_ep_srx_ctx)	domain_attr->max_ep_srx_ctx = hints->max_ep_srx_ctx;
			if (hints->mr_iov_limit)	domain_attr->mr_iov_limit = hints->mr_iov_limit;
		}
	}

	domain_attr->name = strdup(FI_OPX_DOMAIN_NAME);

	if (!domain_attr->name) {
		FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "no memory\n");
		errno = FI_ENOMEM;
		return -errno;
	}

	domain_attr->cq_data_size = FI_OPX_REMOTE_CQ_DATA_SIZE;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_check_domain_attr(struct fi_domain_attr *attr)
{
	if (OFI_UNLIKELY(fi_opx_threading_unknown(attr->threading))) {
		FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "incorrect threading level\n");
		goto err;
	}
	
	if (attr->mr_mode == FI_MR_UNSPEC) {
		attr->mr_mode = OPX_MR == FI_MR_UNSPEC ? FI_MR_BASIC : OPX_MR;
	}

	if (attr->mr_key_size) {
		if (attr->mr_key_size > sizeof(uint64_t)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN,
					"memory key size too large\n");
			goto err;
		}
	}
	if (attr->cq_data_size) {
		if (attr->cq_data_size > FI_OPX_REMOTE_CQ_DATA_SIZE) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN,
					"max cq data supported is %zu\n",
					FI_OPX_REMOTE_CQ_DATA_SIZE);
			goto err;
		}
	}

	return 0;

err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_domain(struct fid_fabric *fabric,
		struct fi_info *info,
		struct fid_domain **dom, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "open domain\n");

	int ret;
	int get_param_check;
	struct fi_opx_domain 	*opx_domain = NULL;
	struct fi_opx_fabric 	*opx_fabric =
		container_of(fabric, struct fi_opx_fabric, fabric_fid);

	if (!info) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"no info supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_opx_fid_check(&fabric->fid, FI_CLASS_FABRIC, "fabric");
	if (ret)
		return ret;

	opx_domain = calloc(1, sizeof(struct fi_opx_domain));
	if (!opx_domain) {
		errno = FI_ENOMEM;
		goto err;
	}

	if (fi_opx_global.default_domain_attr == NULL) {
		if (fi_opx_alloc_default_domain_attr(&fi_opx_global.default_domain_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM; 
			goto err;
		}
	}
  
	struct fi_opx_tid_domain *opx_tid_domain;
	struct fi_opx_tid_fabric *opx_tid_fabric = opx_fabric->tid_fabric;

	if(fi_opx_tid_domain(opx_tid_fabric, info, &opx_tid_domain)){
		errno = FI_ENOMEM;
		goto err;
	}
	opx_domain->tid_domain = opx_tid_domain;

	/* fill in default domain attributes */
	opx_domain->threading		= fi_opx_global.default_domain_attr->threading;
	opx_domain->resource_mgmt	= fi_opx_global.default_domain_attr->resource_mgmt;
	opx_domain->data_progress	= fi_opx_global.default_domain_attr->data_progress;

	int env_var_progress_interval = 0;
	get_param_check = fi_param_get_int(fi_opx_global.prov, "auto_progress_interval_usec", &env_var_progress_interval);
	if (get_param_check == FI_SUCCESS) {
		if (env_var_progress_interval < 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"FI_OPX_AUTO_PROGRESS_INTERVAL_USEC must be an integer >= 0 using default value\n");
			env_var_progress_interval = 0;
		}
	} else {
		env_var_progress_interval = 0;
	}
	opx_domain->auto_progress_interval = env_var_progress_interval;

	if (info->domain_attr) {
		if (info->domain_attr->domain) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
					"domain cannot be supplied\n");
			goto err;
		}
		ret = fi_opx_check_domain_attr(info->domain_attr);
		if (ret)
			goto err;
		opx_domain->threading = info->domain_attr->threading;
		opx_domain->resource_mgmt = info->domain_attr->resource_mgmt;
		if (fi_opx_global.progress == FI_PROGRESS_UNSPEC) {
			opx_domain->data_progress = info->domain_attr->data_progress;
		}
	}

	opx_domain->fabric = opx_fabric;

	fi_opx_ref_init(&opx_domain->ref_cnt, "domain");

	opx_domain->domain_fid.fid.fclass  = FI_CLASS_DOMAIN;
	opx_domain->domain_fid.fid.context = context;
	opx_domain->domain_fid.fid.ops     = &fi_opx_fi_ops;
	opx_domain->domain_fid.ops	   = &fi_opx_domain_ops;
	
	char * env_var_prog_affinity = OPX_DEFAULT_PROG_AFFINITY_STR;
	get_param_check = fi_param_get_str(fi_opx_global.prov, "prog_affinity", &env_var_prog_affinity);
	if (get_param_check == FI_SUCCESS) {
		if (strlen(env_var_prog_affinity) >= OPX_JOB_KEY_STR_SIZE) {
                	env_var_prog_affinity[OPX_JOB_KEY_STR_SIZE-1] = 0;
                	FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
                        	"Progress Affinity too long. Must be no more than 32 characters total, using default.\n");
			env_var_prog_affinity = OPX_DEFAULT_PROG_AFFINITY_STR;
        	}
	} else {
		env_var_prog_affinity = OPX_DEFAULT_PROG_AFFINITY_STR;
	}
	

	if (strncmp(env_var_prog_affinity, OPX_DEFAULT_PROG_AFFINITY_STR, OPX_JOB_KEY_STR_SIZE)){
		goto skip;
	}

	int cols = 0;
	bool recentCol = true;
	int iter;
	for (iter=0; iter < OPX_JOB_KEY_STR_SIZE && env_var_prog_affinity[iter] != 0; iter++) {
		if (!isdigit(env_var_prog_affinity[iter]) && env_var_prog_affinity[iter] != ':'){
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"Invalid program affinity. Progress affinity must be a digit or colon.\n");
			errno=FI_EINVAL;
			goto err;
		}
		if (env_var_prog_affinity[iter] == ':'){
			if (recentCol){
				FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
					"Progress Affinity improperly formatted. Must be a : separated triplet.\n");
				errno=FI_EINVAL;
				goto err;
			}
			else{
				cols += 1;
				recentCol = true;
			}
		}
		else
			recentCol = false;
	}

	if (cols != 2){
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
			"Progress Affinity improperly formatted. Must be a : separated triplet.\n");
		errno=FI_EINVAL;
		goto err;
	}

skip:
	strncpy(opx_domain->progress_affinity_str, env_var_prog_affinity, OPX_JOB_KEY_STR_SIZE-1);
        opx_domain->progress_affinity_str[OPX_JOB_KEY_STR_SIZE-1] = '\0';

	// Max UUID consists of 32 hex digits.
	char * env_var_uuid = OPX_DEFAULT_JOB_KEY_STR;
	fi_param_get_str(fi_opx_global.prov, "uuid", &env_var_uuid);

	if (strlen(env_var_uuid) >= OPX_JOB_KEY_STR_SIZE) {
		env_var_uuid[OPX_JOB_KEY_STR_SIZE-1] = 0;
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
			"UUID too long. UUID must consist of 1-32 hexadecimal digits.\n");
		errno=FI_EINVAL;
		goto err;
	} 

	int i;
	for (i=0; i < OPX_JOB_KEY_STR_SIZE && env_var_uuid[i] != 0; i++) {
		if (!isxdigit(env_var_uuid[i])) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"Invalid UUID. UUID must consist solely of hexadecimal digits.\n");
			errno=FI_EINVAL;
			goto err;
		}
	}
	

	// Copy the job key and guarantee null termination.
	strncpy(opx_domain->unique_job_key_str, env_var_uuid, OPX_JOB_KEY_STR_SIZE-1);
	opx_domain->unique_job_key_str[OPX_JOB_KEY_STR_SIZE-1] = '\0';

	sscanf(opx_domain->unique_job_key_str, "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&opx_domain->unique_job_key[0],
		&opx_domain->unique_job_key[1],
		&opx_domain->unique_job_key[2],
		&opx_domain->unique_job_key[3],
		&opx_domain->unique_job_key[4],
		&opx_domain->unique_job_key[5],
		&opx_domain->unique_job_key[6],
		&opx_domain->unique_job_key[7],
		&opx_domain->unique_job_key[8],
		&opx_domain->unique_job_key[9],
		&opx_domain->unique_job_key[10],
		&opx_domain->unique_job_key[11],
		&opx_domain->unique_job_key[12],
		&opx_domain->unique_job_key[13],
		&opx_domain->unique_job_key[14],
		&opx_domain->unique_job_key[15]);

	opx_domain->rx_count = 0;
	opx_domain->tx_count = 0;
	opx_domain->ep_count = 0;

	ret = fi_opx_init_mr_ops(&opx_domain->domain_fid, info);
	if (ret)
		goto err;

	opx_domain->reliability_kind = OPX_DOMAIN_RELIABILITY;

	fi_opx_ref_inc(&opx_fabric->ref_cnt, "fabric");

	*dom = &opx_domain->domain_fid;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "domain opened\n");
	return 0;

err:
	fi_opx_finalize_mr_ops(&opx_domain->domain_fid);
	if (opx_domain) {
		free(opx_domain);
		opx_domain = NULL;
	}

	if (fi_opx_global.default_domain_attr != NULL) {
		if (fi_opx_global.default_domain_attr->name != NULL) {
			free(fi_opx_global.default_domain_attr->name);
			fi_opx_global.default_domain_attr->name = NULL;
		}
		free(fi_opx_global.default_domain_attr);
		fi_opx_global.default_domain_attr = NULL;
	}
	return -errno;
}
