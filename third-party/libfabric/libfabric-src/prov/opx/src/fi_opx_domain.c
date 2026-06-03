/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 by Cornelis Networks.
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
#include "rdma/opx/opx_hfisvc.h"

#define OPX_DOMAIN_HFISVC_NOT_INITIALIZED (0x7FFFFFFFFFFFFFFEll)

#if HAVE_HFISVC
/* OPX does not ibverbs directly, dlopen/dlsym only */
#include <dlfcn.h>
#endif

static int fi_opx_close_domain(fid_t fid)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "close domain\n");

	int		      ret;
	struct fi_opx_domain *opx_domain = container_of(fid, struct fi_opx_domain, domain_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_DOMAIN, "domain");
	if (ret) {
		return ret;
	}

	ret = fi_opx_finalize_mr_ops(&opx_domain->domain_fid);
	if (ret) {
		return ret;
	}

	opx_close_tid_domain(opx_domain->tid_domain, OPX_TID_NO_LOCK_ON_CLEANUP);
	opx_domain->tid_domain = NULL;
#ifdef OPX_HMEM
	opx_hmem_close_domain(opx_domain->hmem_domain, OPX_HMEM_NO_LOCK_ON_CLEANUP);
	opx_domain->hmem_domain = NULL;
#endif

#if HAVE_HFISVC
	if (opx_domain->use_hfisvc) {
		ret = fi_opx_ref_finalize(&opx_domain->hfisvc.ref_cnt, "hfisvc");
		if (ret) {
			return ret;
		}

		opx_hfisvc_keyset_free(opx_domain->hfisvc.access_key_set);

		/* Close HFISVC context, the endpoint should already be closed */
		assert(opx_domain->hfisvc.ctx != NULL);
		opx_hfi1_rdma_context_close(opx_domain->hfisvc.ctx);
	}
#endif

	/* Close rdma-core lib, the endpoint already closed contexts */
	opx_hfi1_rdma_lib_close();

	ret = fi_opx_ref_finalize(&opx_domain->ref_cnt, "domain");
	if (ret) {
		return ret;
	}

	ret = fi_opx_ref_dec(&opx_domain->fabric->ref_cnt, "fabric");
	if (ret) {
		return ret;
	}

	free(opx_domain);

	// opx_domain (the object passed in as fid) is now unusable

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "domain closed\n");
	return 0;
}

static struct fi_ops fi_opx_fi_ops = {.size	= sizeof(struct fi_ops),
				      .close	= fi_opx_close_domain,
				      .bind	= fi_no_bind,
				      .control	= fi_no_control,
				      .ops_open = fi_no_ops_open};

static struct fi_ops_domain fi_opx_domain_ops = {.size	      = sizeof(struct fi_ops_domain),
						 .av_open     = fi_opx_av_open,
						 .cq_open     = fi_opx_cq_open,
						 .endpoint    = fi_opx_endpoint,
						 .scalable_ep = fi_no_scalable_ep,
						 .cntr_open   = fi_opx_cntr_open,
						 .poll_open   = fi_no_poll_open,
						 .stx_ctx     = fi_no_stx_context,
						 .srx_ctx     = fi_no_srx_context};

static inline void opx_util_domain_cleanup(struct fi_opx_domain *opx_domain)
{
	if (opx_domain->tid_domain) {
		opx_close_tid_domain(opx_domain->tid_domain, OPX_TID_NO_LOCK_ON_CLEANUP);
		opx_domain->tid_domain = NULL;
	}
#ifdef OPX_HMEM
	if (opx_domain->hmem_domain) {
		opx_hmem_close_domain(opx_domain->hmem_domain, OPX_HMEM_NO_LOCK_ON_CLEANUP);
		opx_domain->hmem_domain = NULL;
	}
#endif
}

int fi_opx_alloc_default_domain_attr(struct fi_domain_attr **domain_attr)
{
	struct fi_domain_attr *attr;

	attr = calloc(1, sizeof(*attr));
	if (!attr) {
		goto err;
	}

	uint32_t       ppn	  = 1; /* TODO */
	const unsigned tx_ctx_cnt = 160 / ppn;
	const unsigned rx_ctx_cnt = 160 / ppn;

	attr->domain = NULL;
	attr->name   = strdup(FI_OPX_DOMAIN_NAME);

	attr->threading	       = OPX_THREAD;
	attr->control_progress = FI_PROGRESS_MANUAL;
	attr->data_progress    = FI_PROGRESS_MANUAL;
	attr->resource_mgmt    = FI_RM_DISABLED;
	attr->av_type	       = OPX_AV;
	attr->mr_mode	       = FI_OPX_BASE_MR_MODE;
	attr->mr_key_size      = sizeof(uint64_t);
	attr->cq_data_size     = FI_OPX_REMOTE_CQ_DATA_SIZE;
	attr->cq_cnt	       = (size_t) -1;
	attr->ep_cnt	       = 160 / ppn;
	attr->tx_ctx_cnt       = tx_ctx_cnt;
	attr->rx_ctx_cnt       = rx_ctx_cnt;

	// For now, keep these 1-1
	attr->max_ep_tx_ctx = FI_OPX_ADDR_SEP_RX_MAX;
	attr->max_ep_rx_ctx = FI_OPX_ADDR_SEP_RX_MAX;

	attr->max_ep_stx_ctx = 0;
	attr->max_ep_srx_ctx = 0;
	attr->mr_iov_limit   = 1;

	*domain_attr = attr;

	return 0;
err:
	*domain_attr = NULL;
	errno	     = FI_ENOMEM;
	return -1;
}

int fi_opx_choose_domain(uint64_t caps, struct fi_domain_attr *domain_attr, struct fi_domain_attr *hints,
			 enum fi_progress progress)
{
	if (!domain_attr) {
		FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "missing domain attribute structure\n");
		goto err;
	}

	*domain_attr		   = *fi_opx_global.default_domain_attr;
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

#ifdef OPX_HMEM
	domain_attr->mr_mode |= FI_MR_HMEM;
#endif

	if (hints) {
		if (hints->mr_mode & FI_MR_VIRT_ADDR) {
			FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN,
				"Application requests FI_MR_VIRT_ADDR, OPX is turning on that mr_mode bit\n");
			domain_attr->mr_mode |= FI_MR_VIRT_ADDR;
		}

		if (hints->domain) {
			struct fi_opx_domain *opx_domain =
				container_of(hints->domain, struct fi_opx_domain, domain_fid);

			domain_attr->threading	   = opx_domain->threading;
			domain_attr->resource_mgmt = opx_domain->resource_mgmt;
			domain_attr->tx_ctx_cnt	   = fi_opx_domain_get_tx_max(hints->domain);
			domain_attr->rx_ctx_cnt	   = fi_opx_domain_get_rx_max(hints->domain);
			domain_attr->max_ep_tx_ctx = fi_opx_domain_get_tx_max(hints->domain);
			domain_attr->max_ep_rx_ctx = fi_opx_domain_get_rx_max(hints->domain);

		} else {
			if (hints->threading) {
				domain_attr->threading = hints->threading;
			}
			if (hints->resource_mgmt) {
				domain_attr->resource_mgmt = hints->resource_mgmt;
			}
			if (hints->av_type) {
				domain_attr->av_type = hints->av_type;
			}
			if (hints->mr_key_size) {
				domain_attr->mr_key_size = hints->mr_key_size;
			}
			if (hints->cq_data_size) {
				domain_attr->cq_data_size = hints->cq_data_size;
			}
			if (hints->cq_cnt) {
				domain_attr->cq_cnt = hints->cq_cnt;
			}
			if (hints->ep_cnt) {
				domain_attr->ep_cnt = hints->ep_cnt;
			}
			if (hints->tx_ctx_cnt) {
				domain_attr->tx_ctx_cnt = hints->tx_ctx_cnt;
			}
			if (hints->rx_ctx_cnt) {
				domain_attr->rx_ctx_cnt = hints->rx_ctx_cnt;
			}
			if (hints->max_ep_tx_ctx) {
				domain_attr->max_ep_tx_ctx = hints->max_ep_tx_ctx;
			}
			if (hints->max_ep_rx_ctx) {
				domain_attr->max_ep_rx_ctx = hints->max_ep_rx_ctx;
			}
			if (hints->mr_iov_limit) {
				domain_attr->mr_iov_limit = hints->mr_iov_limit;
			}
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

	if (attr->mr_mode == OFI_MR_UNSPEC) {
		attr->mr_mode = FI_OPX_BASE_MR_MODE;
	}

	if (attr->mr_key_size) {
		if (attr->mr_key_size > sizeof(uint64_t)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "memory key size too large\n");
			goto err;
		}
	}
	if (attr->cq_data_size) {
		if (attr->cq_data_size > FI_OPX_REMOTE_CQ_DATA_SIZE) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "max cq data supported is %zu\n",
			       FI_OPX_REMOTE_CQ_DATA_SIZE);
			goto err;
		}
	}

	return 0;

err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_validate_affinity_str(char *str)
{
	int  cols      = 0;
	bool recentCol = true;
	int  iter;

	for (iter = 0; iter < strlen(str); iter++) {
		if (!isdigit(str[iter]) && str[iter] != ':') {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"Invalid program affinity. Progress affinity must be a digit or colon.\n");
			return -1;
		}

		if (str[iter] == ':') {
			if (recentCol) {
				FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
					"Progress Affinity improperly formatted. Must be a : separated triplet.\n");
				return -1;
			} else {
				cols += 1;
				recentCol = true;
			}
		} else {
			recentCol = false;
		}
	}

	if (cols != 2) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
			"Progress Affinity improperly formatted. Must be a : separated triplet.\n");
		return -1;
	}
	return 0;
}

int fi_opx_domain(struct fid_fabric *fabric, struct fi_info *info, struct fid_domain **dom, void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "open domain\n");

	int		      ret	      = 0;
	int		      get_param_check = 0;
	struct fi_opx_domain *opx_domain      = NULL;
	struct fi_opx_fabric *opx_fabric      = container_of(fabric, struct fi_opx_fabric, fabric_fid);

	if (!info) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "no info supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_opx_fid_check(&fabric->fid, FI_CLASS_FABRIC, "fabric");
	if (ret) {
		return ret;
	}

	opx_domain = calloc(1, sizeof(struct fi_opx_domain));
	if (!opx_domain) {
		errno = FI_ENOMEM;
		goto err;
	}
	opx_domain->tid_domain = NULL;
#ifdef OPX_HMEM
	opx_domain->hmem_domain = NULL;
#endif

	if (fi_opx_global.default_domain_attr == NULL) {
		if (fi_opx_alloc_default_domain_attr(&fi_opx_global.default_domain_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN,
			       "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM;
			goto err;
		}
	}

	struct opx_tid_domain *opx_tid_domain;
	struct opx_tid_fabric *opx_tid_fabric = opx_fabric->tid_fabric;

	if (opx_open_tid_domain(opx_tid_fabric, info, &opx_tid_domain)) {
		errno = FI_ENOMEM;
		goto err;
	}
	opx_domain->tid_domain = opx_tid_domain;

#ifdef OPX_HMEM
	struct opx_hmem_domain *opx_hmem_domain;
	struct opx_hmem_fabric *opx_hmem_fabric = opx_fabric->hmem_fabric;

	if (opx_hmem_open_domain(opx_hmem_fabric, info, &opx_hmem_domain)) {
		errno = FI_ENOMEM;
		goto err;
	}
	opx_domain->hmem_domain		    = opx_hmem_domain;
	opx_domain->hmem_domain->opx_domain = opx_domain;

	size_t env_var_threshold;
	get_param_check = fi_param_get_size_t(fi_opx_global.prov, "dev_reg_send_threshold", &env_var_threshold);
	if (get_param_check == FI_SUCCESS) {
		if (env_var_threshold <= OPX_HMEM_DEV_REG_THRESHOLD_MAX) {
			opx_domain->hmem_domain->devreg_copy_from_threshold = env_var_threshold;
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"FI_OPX_DEV_REG_SEND_THRESHOLD must be an integer >= %u and <= %u. Using default value (%u) instead of %zu\n",
				OPX_HMEM_DEV_REG_THRESHOLD_MIN, OPX_HMEM_DEV_REG_THRESHOLD_MAX,
				OPX_HMEM_DEV_REG_SEND_THRESHOLD_DEFAULT, env_var_threshold);
		}
	}

	get_param_check = fi_param_get_size_t(fi_opx_global.prov, "dev_reg_recv_threshold", &env_var_threshold);
	if (get_param_check == FI_SUCCESS) {
		if (env_var_threshold <= OPX_HMEM_DEV_REG_THRESHOLD_MAX) {
			opx_domain->hmem_domain->devreg_copy_to_threshold = env_var_threshold;
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"FI_OPX_DEV_REG_RECV_THRESHOLD must be an integer >= %u and <= %u. Using default value (%u) instead of %zu\n",
				OPX_HMEM_DEV_REG_THRESHOLD_MIN, OPX_HMEM_DEV_REG_THRESHOLD_MAX,
				OPX_HMEM_DEV_REG_RECV_THRESHOLD_DEFAULT, env_var_threshold);
		}
	}
#endif

	/* fill in default domain attributes */
	opx_domain->threading	  = fi_opx_global.default_domain_attr->threading;
	opx_domain->resource_mgmt = fi_opx_global.default_domain_attr->resource_mgmt;
	opx_domain->data_progress = fi_opx_global.default_domain_attr->data_progress;

	if (info->domain_attr) {
		if (info->domain_attr->domain) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "domain cannot be supplied\n");
			goto err;
		}
		ret = fi_opx_check_domain_attr(info->domain_attr);
		if (ret) {
			goto err;
		}
		opx_domain->threading	  = info->domain_attr->threading;
		opx_domain->resource_mgmt = info->domain_attr->resource_mgmt;
		if (fi_opx_global.progress == FI_PROGRESS_UNSPEC) {
			opx_domain->data_progress = info->domain_attr->data_progress;
		}
	}

	opx_domain->fabric = opx_fabric;

	fi_opx_ref_init(&opx_domain->ref_cnt, "domain");

	opx_domain->domain_fid.fid.fclass  = FI_CLASS_DOMAIN;
	opx_domain->domain_fid.fid.context = context;
	opx_domain->domain_fid.fid.ops	   = &fi_opx_fi_ops;
	opx_domain->domain_fid.ops	   = &fi_opx_domain_ops;

	opx_domain->progress_affinity_str = NULL;
	get_param_check = fi_param_get_str(fi_opx_global.prov, "prog_affinity", &opx_domain->progress_affinity_str);
	if (get_param_check == FI_SUCCESS) {
		if (fi_opx_validate_affinity_str(opx_domain->progress_affinity_str) != 0) {
			opx_domain->progress_affinity_str = NULL;
			errno				  = FI_EINVAL;
			goto err;
		}
	}

	// Max UUID consists of 32 hex digits.
	char *env_var_uuid = OPX_DEFAULT_JOB_KEY_STR;
	get_param_check	   = fi_param_get_str(fi_opx_global.prov, "uuid", &env_var_uuid);
	char *impi_uuid	   = getenv("I_MPI_HYDRA_UUID");
	char *slurm_job_id = getenv("SLURM_JOB_ID");

	if (get_param_check == FI_SUCCESS) {
		FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "Detected user specified FI_OPX_UUID\n");
	} else if (slurm_job_id) {
		env_var_uuid = slurm_job_id;
		FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "Found SLURM_JOB_ID.  Using it for FI_OPX_UUID\n");
	} else if (impi_uuid) {
		env_var_uuid = impi_uuid;
		FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "Found I_MPI_HYDRA_UUID.  Using it for FI_OPX_UUID\n");
	} else {
		env_var_uuid = OPX_DEFAULT_JOB_KEY_STR;
	}

	if (strlen(env_var_uuid) >= OPX_JOB_KEY_STR_SIZE) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
			"UUID too long. UUID must consist of 1-32 hexadecimal digits.  Using default OPX uuid instead\n");
		env_var_uuid = OPX_DEFAULT_JOB_KEY_STR;
	}

	int i;
	for (i = 0; i < OPX_JOB_KEY_STR_SIZE && env_var_uuid[i] != 0; i++) {
		if (!isxdigit(env_var_uuid[i])) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"Invalid UUID. UUID must consist solely of hexadecimal digits.  Using default OPX uuid instead\n");
			env_var_uuid = OPX_DEFAULT_JOB_KEY_STR;
		}
	}

	// Copy the job key and guarantee null termination.
	strncpy(opx_domain->unique_job_key_str, env_var_uuid, OPX_JOB_KEY_STR_SIZE - 1);
	opx_domain->unique_job_key_str[OPX_JOB_KEY_STR_SIZE - 1] = '\0';

	int elements_read = sscanf(
		opx_domain->unique_job_key_str,
		"%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&opx_domain->unique_job_key[0], &opx_domain->unique_job_key[1], &opx_domain->unique_job_key[2],
		&opx_domain->unique_job_key[3], &opx_domain->unique_job_key[4], &opx_domain->unique_job_key[5],
		&opx_domain->unique_job_key[6], &opx_domain->unique_job_key[7], &opx_domain->unique_job_key[8],
		&opx_domain->unique_job_key[9], &opx_domain->unique_job_key[10], &opx_domain->unique_job_key[11],
		&opx_domain->unique_job_key[12], &opx_domain->unique_job_key[13], &opx_domain->unique_job_key[14],
		&opx_domain->unique_job_key[15]);
	if (elements_read == EOF) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
			"Error: sscanf encountered an input failure (EOF), unable to parse the unique job key string.\n");
		errno = FI_EINVAL;
		goto err;
	}

	FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "Domain unique job key set to %s\n", opx_domain->unique_job_key_str);
	// TODO: Print out a summary of all domain settings wtih FI_INFO

	opx_domain->use_hfisvc = 0;
#if HAVE_HFISVC
	opx_domain->hfisvc.ref_cnt = OPX_DOMAIN_HFISVC_NOT_INITIALIZED;
#endif

	opx_domain->rx_count = 0;
	opx_domain->tx_count = 0;
	opx_domain->ep_count = 0;

	ret = fi_opx_init_mr_ops(&opx_domain->domain_fid, info);
	if (ret) {
		goto err;
	}

	fi_opx_ref_inc(&opx_fabric->ref_cnt, "fabric");

	*dom = &opx_domain->domain_fid;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "domain opened\n");
	return 0;

err:
	if (opx_domain) {
		fi_opx_finalize_mr_ops(&opx_domain->domain_fid);
		opx_util_domain_cleanup(opx_domain);
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

#if HAVE_HFISVC

extern struct opx_rdma_ops_struct opx_rdma_ops;

int opx_domain_hfisvc_init(struct fi_opx_domain *domain)
{
	if (domain->hfisvc.ref_cnt == OPX_DOMAIN_HFISVC_NOT_INITIALIZED) {
		// Driver support but library/apis not found
		// dlopen is done - once - in hfi1dv rdma-core and copied to hfisvc

		domain->hfisvc.libhfi1verbs = opx_rdma_ops.libhfi1verbs;
		if (domain->hfisvc.libhfi1verbs == NULL) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFISVC] libhfi1verbs not found\n");
			return -FI_ENODEV;
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFISVC] libhfi1verbs found\n");

		domain->hfisvc.initialize = dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_initialize");
		if (domain->hfisvc.initialize == NULL) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFISVC] hfisvc_client_initialize not found\n");
			return -FI_ENODEV;
		}

		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFISVC] hfisvc_client_initialize found\n");

		domain->hfisvc.client_key = dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_key");
		domain->hfisvc.command_queue_open =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_command_queue_open");
		domain->hfisvc.command_queue_close =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_command_queue_close");
		domain->hfisvc.completion_queue_open =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_completion_queue_open");
		domain->hfisvc.completion_queue_close =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_completion_queue_close");
		domain->hfisvc.cq_read = dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cq_read");
		domain->hfisvc.cmd_dma_access_once_va =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_dma_access_once_va");
		domain->hfisvc.cmd_dma_access_once =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_dma_access_once");
		domain->hfisvc.cmd_rdma_read	= dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_rdma_read");
		domain->hfisvc.cmd_rdma_read_va = dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_rdma_read_va");
		domain->hfisvc.cmd_rdma_write	= dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_rdma_write");
		domain->hfisvc.cmd_mr_open	= dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_mr_open");
		domain->hfisvc.cmd_mr_close	= dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_mr_close");
		domain->hfisvc.cmd_dma_access_enable =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_dma_access_enable");
		domain->hfisvc.cmd_dma_access_disable =
			dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_cmd_dma_access_disable");
		domain->hfisvc.doorbell = dlsym(domain->hfisvc.libhfi1verbs, "hfisvc_client_doorbell");

		assert(domain->hfisvc.client_key != NULL);
		assert(domain->hfisvc.command_queue_open != NULL);
		assert(domain->hfisvc.command_queue_close != NULL);
		assert(domain->hfisvc.completion_queue_open != NULL);
		assert(domain->hfisvc.completion_queue_close != NULL);
		assert(domain->hfisvc.cq_read != NULL);
		assert(domain->hfisvc.cmd_dma_access_once_va != NULL);
		assert(domain->hfisvc.cmd_rdma_read_va != NULL);
		assert(domain->hfisvc.doorbell != NULL);

		int ret = (*domain->hfisvc.initialize)(domain->hfisvc.ctx);
		if (ret) {
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN,
				"[HFISVC] hfisvc_client_initialize failed, disable hfisvc\n");
			return -FI_ENODEV;
		}
		ret = (*domain->hfisvc.client_key)(domain->hfisvc.ctx, &domain->hfisvc.key);
		if (ret) {
			abort();
		}
		OPX_HFISVC_DEBUG_LOG("Creating new domain mr command queue\n");
		ret = (*domain->hfisvc.command_queue_open)(&domain->hfisvc.mr_command_queue, domain->hfisvc.ctx);

		if (ret) {
			fprintf(stderr, "(%d) %s:%s():%d Failed creating domain mr command queue! rc=%d\n", getpid(),
				__FILE__, __func__, __LINE__, ret);
			abort();
		}

		OPX_HFISVC_DEBUG_LOG("Creating new domain mr completion queue\n");
		ret = (*domain->hfisvc.completion_queue_open)(&domain->hfisvc.mr_completion_queue, domain->hfisvc.ctx);

		if (ret) {
			fprintf(stderr, "(%d) %s:%s():%d Failed creating domain mr completion queue! rc=%d\n", getpid(),
				__FILE__, __func__, __LINE__, ret);
			abort();
		}
		OPX_HFISVC_DEBUG_LOG("Initializing hfisvc keyset\n");
		ret = opx_hfisvc_keyset_init(0, &domain->hfisvc.access_key_set);
		if (ret) {
			fprintf(stderr, "(%d) %s:%s():%d Failed initializing hfisvc keyset! rc=%d\n", getpid(),
				__FILE__, __func__, __LINE__, ret);
			abort();
		}
		fi_opx_ref_init(&domain->hfisvc.ref_cnt, "hfisvc");
		OPX_HFISVC_DEBUG_LOG("Initialized HFI service with client key %u\n", domain->hfisvc.key);
		domain->use_hfisvc = 1;
	}

	fi_opx_ref_inc(&domain->hfisvc.ref_cnt, "hfisvc");

	return 0;
}
#endif
