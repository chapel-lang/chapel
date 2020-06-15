/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/syscall.h>

#include "alps/alps.h"
#include "alps/alps_toolAssist.h"
#include "alps/libalpsutil.h"
#include "alps/libalpslli.h"

#include "gnix.h"
#include "gnix_util.h"

static bool app_init;
/* Filled in by __gnix_app_init */
static uint8_t gnix_app_ptag;
static uint32_t gnix_app_cookie;
static uint32_t gnix_pes_on_node;
static int gnix_pe_node_rank = -1;
#if HAVE_CRITERION
int gnix_first_pe_on_node; /* globally visible for  criterion */
#else
static int gnix_first_pe_on_node;
#endif
/* CCM/ccmlogin specific stuff */
static bool ccm_init;
/* This file provides ccm_alps_info */
#define CCM_ALPS_INFO_FILE "/tmp/ccm_alps_info"
typedef struct ccm_alps_info {
	uint32_t version;
	uint8_t ptag;
	uint32_t cookie;
} ccm_alps_info_t;
/* Format for the nodelist filename: $HOME/.crayccm/ccmnodlist.<WLM jobid> */
#define CCM_NODELIST_FN ".crayccm/ccm_nodelist."
/* alps specific stuff */
static uint64_t gnix_apid;
static alpsAppLayout_t gnix_appLayout;
static uint32_t gnix_device_id;
static int gnix_cq_limit;
/* These are not used currently and could be static to gnix_alps_init */
static int alps_init;
static int *gnix_app_placementList;
static int *gnix_app_targetNids;
static int *gnix_app_targetPes;
static int *gnix_app_targetLen;
static struct in_addr *gnix_app_targetIps;
static int *gnix_app_startPe;
static int *gnix_app_totalPes;
static int *gnix_app_nodePes;
static int *gnix_app_peCpus;

fastlock_t __gnix_alps_lock;

int _gnix_get_cq_limit(void)
{
	return gnix_cq_limit;
}

static inline void __gnix_ccm_cleanup(void)
{
	ccm_init = false;
}

static inline void __gnix_alps_cleanup(void)
{
	alps_app_lli_lock();

	if (gnix_app_placementList)
		free(gnix_app_placementList);
	if (gnix_app_targetNids)
		free(gnix_app_targetNids);
	if (gnix_app_targetPes)
		free(gnix_app_targetPes);
	if (gnix_app_targetLen)
		free(gnix_app_targetLen);
	if (gnix_app_targetIps)
		free(gnix_app_targetIps);
	if (gnix_app_startPe)
		free(gnix_app_startPe);
	if (gnix_app_totalPes)
		free(gnix_app_totalPes);
	if (gnix_app_nodePes)
		free(gnix_app_nodePes);
	if (gnix_app_peCpus)
		free(gnix_app_peCpus);

	alps_init = false;

	alps_app_lli_unlock();
}

void _gnix_app_cleanup(void)
{
	if (alps_init) {
		__gnix_alps_cleanup();
	} else if (ccm_init) {
		__gnix_ccm_cleanup();
	}
}

/* There are two types of errors that can happen in this function:
 * - CCM ALPS info file not found
 * - Failure while trying to get ptag, cookie and PEs/node
 *  Currently we don't distinguish between the two.
 */
static int __gnix_ccm_init(void)
{
	int rc, fd;
	FILE *f;
	char *nodefile;
	char nodelist[PATH_MAX];
	const char *home;
	ccm_alps_info_t info;
	uint32_t num_nids = 0;

	GNIX_DEBUG(FI_LOG_FABRIC, "Reading job info file %s\n",
		   CCM_ALPS_INFO_FILE);

	fd = open(CCM_ALPS_INFO_FILE, O_RDONLY);
	if (fd < 0) {
		return -FI_EIO;
	}

	rc = read(fd, &info, sizeof(ccm_alps_info_t));
	if (rc != sizeof(ccm_alps_info_t))
		return -FI_EIO;

	gnix_app_ptag = info.ptag;
	gnix_app_cookie = info.cookie;

	close(fd);
	GNIX_DEBUG(FI_LOG_FABRIC, "Ptag=0x%x, cookie=0x%x\n",
		   gnix_app_ptag, gnix_app_cookie);

	home = getenv("HOME");
	/* use the WLM node file if using PBS */
	nodefile = getenv("PBS_NODEFILE");
	if (!nodefile) {
		const char *jobid = getenv("SLURM_JOB_ID");
		if (!jobid) {
			jobid = getenv("SLURM_JOBID");
		}
		snprintf(nodelist, PATH_MAX, "%s/%s%s", home ? home : ".",
			 CCM_NODELIST_FN, jobid ? jobid : "sdb");
		nodefile = nodelist;
	}
	f = fopen(nodefile, "r");
	if (f) {
		char mynid[PATH_MAX];
		char next_nid[PATH_MAX];

		rc = gethostname(mynid, PATH_MAX);
		if (rc) {
			/* use the first address */
			rc = fscanf(f, "%s\n", mynid);
			/* assume this one worked, error case is same */
			num_nids++;
		}
		while (true) {
			rc = fscanf(f, "%s\n", next_nid);
			if (rc == 1) {
				if (strcmp(mynid, next_nid) == 0) {
					num_nids++;
				}
			} else {
				break;
			}
		}
		gnix_pes_on_node = num_nids;
		fclose(f);
	} else {
		/* what would be a better default? */
		GNIX_WARN(FI_LOG_FABRIC,
			  "CCM nodelist not found.  Assuming 1 PE per node\n");
		gnix_pes_on_node = 1;
	}
	GNIX_DEBUG(FI_LOG_FABRIC, "pes per node=%u\n", gnix_pes_on_node);

	/* Don't really need to do this here, but wanted to be clear */
	gnix_app_placementList = NULL;
	gnix_app_targetNids = NULL;
	gnix_app_targetPes = NULL;
	gnix_app_targetLen = NULL;
	gnix_app_targetIps = NULL;
	gnix_app_startPe = NULL;
	gnix_app_totalPes = NULL;
	gnix_app_nodePes = NULL;
	gnix_app_peCpus = NULL;

	ccm_init = true;
	return FI_SUCCESS;
}

static int __gnix_alps_init(void)
{
	char *cptr = NULL;
	int ret = FI_SUCCESS;
	int my_pe = -1;
	int alps_status = 0;
	size_t alps_count;
	alpsAppLLIGni_t *rdmacred_rsp = NULL;
	alpsAppGni_t *rdmacred_buf = NULL;

	fastlock_acquire(&__gnix_alps_lock);
	/* lli_lock doesn't return anything useful */
	ret = alps_app_lli_lock();

	if (alps_init) {
		/* alps lli lock protects alps_init for now */
		alps_app_lli_unlock();
		fastlock_release(&__gnix_alps_lock);
		return ret;
	}

	/*
	 * First get our apid
	 */
	ret = alps_app_lli_put_request(ALPS_APP_LLI_ALPS_REQ_APID, NULL, 0);
	if (ret != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC, "lli put failed, ret=%d(%s)\n", ret,
			  strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	ret = alps_app_lli_get_response(&alps_status, &alps_count);
	if (alps_status != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC, "lli get response failed, "
			  "alps_status=%d(%s)\n", alps_status,
			  strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	ret = alps_app_lli_get_response_bytes(&gnix_apid, sizeof(gnix_apid));
	if (ret != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "lli get response failed, ret=%d(%s)\n",
			  ret, strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	/*
	 * now get the GNI rdma credentials info
	 */
	ret = alps_app_lli_put_request(ALPS_APP_LLI_ALPS_REQ_GNI, NULL, 0);
	if (ret != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC, "lli put failed, ret=%d(%s)\n",
			  ret, strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	ret = alps_app_lli_get_response(&alps_status, &alps_count);
	if (alps_status != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "lli get response failed, alps_status=%d(%s)\n",
			  alps_status, strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	rdmacred_rsp = malloc(alps_count);
	if (rdmacred_rsp == NULL) {
		ret = -FI_ENOMEM;
		goto err;
	}

	memset(rdmacred_rsp, 0, alps_count);

	ret = alps_app_lli_get_response_bytes(rdmacred_rsp, alps_count);
	if (ret != ALPS_APP_LLI_ALPS_STAT_OK) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "lli get response failed, ret=%d(%s)\n",
			  ret, strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	rdmacred_buf = (alpsAppGni_t *) rdmacred_rsp->u.buf;

	/*
	 * just use the first ptag/cookie for now
	 */

	gnix_app_ptag = rdmacred_buf[0].ptag;
	gnix_app_cookie = rdmacred_buf[0].cookie;

	/*
	 * alps_get_placement_info(uint64_t apid, alpsAppLayout_t *appLayout,
	 *	int **placementList, int **targetNids, int **targetPes,
	 *	int **targetLen, struct in_addr **targetIps, int **startPe,
	 *	int **totalPes, int **nodePes, int **peCpus);
	 */
	ret = alps_get_placement_info(gnix_apid, &gnix_appLayout,
				      &gnix_app_placementList,
				      &gnix_app_targetNids,
				      &gnix_app_targetPes,
				      &gnix_app_targetLen,
				      &gnix_app_targetIps,
				      &gnix_app_startPe,
				      &gnix_app_totalPes,
				      &gnix_app_nodePes,
				      &gnix_app_peCpus);
	if (ret != 1) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "alps_get_placement_info failed, ret=%d(%s)\n",
			  ret, strerror(errno));
		ret = -FI_EIO;
		goto err;
	}

	gnix_pes_on_node = gnix_appLayout.numPesHere;
	gnix_first_pe_on_node = gnix_appLayout.firstPe;

	if ((cptr = getenv("PMI_FORK_RANK")) != NULL) {
		my_pe = atoi(cptr);
	} else {
		if ((cptr = getenv("ALPS_APP_PE")) != NULL) {
			my_pe = atoi(cptr);
		}
	}

	/*
 	 * compute local pe rank, assuming we got our global PE rank
 	 * via either an ALPS (or ALPS SLURM plugin) or Cray PMI,
 	 * otherwise set to -1.
 	 */
	if (my_pe != -1)
		gnix_pe_node_rank = my_pe - gnix_first_pe_on_node;

	alps_init = true;

	ret = 0;
err:
	alps_app_lli_unlock();
	fastlock_release(&__gnix_alps_lock);
	if (rdmacred_rsp != NULL) {
		free(rdmacred_rsp);
	}

	return ret;
}

static int __gnix_app_init(void)
{
	int ret;

	if (app_init) {
		return FI_SUCCESS;
	}

	/* Try CCM first */
	ret = __gnix_ccm_init();
	if (ret) {
		ret = __gnix_alps_init();
	}

	if (ret == FI_SUCCESS) {
		app_init = true;
	}

	gnix_device_id = 0;
	return ret;

}

int gnixu_get_rdma_credentials(void *addr, uint8_t *ptag, uint32_t *cookie)
{
	int ret = FI_SUCCESS;

	/*TODO: If addr is used, ensure that ep->info->addr_format is checked*/

	if ((ptag == NULL) || (cookie == NULL)) {
		return -FI_EINVAL;
	}

	ret = __gnix_app_init();
	if (ret) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "__gnix_app_init() failed, ret=%d(%s)\n",
			  ret, strerror(errno));
		return ret;
	}

	/*
	 * TODO: need to handle non null addr differently at some point,
	 * a non-NULL addr can be used to acquire RDMA credentials other than
	 * those assigned by ALPS/nativized slurm.
	 */
	*ptag = gnix_app_ptag;
	*cookie = gnix_app_cookie;

	return ret;
}


#define NUM_GNI_RC (GNI_RC_ERROR_NOMEM+1)
static int gnix_rc_table[NUM_GNI_RC] = {
	[GNI_RC_SUCCESS] = FI_SUCCESS,
	[GNI_RC_NOT_DONE] = -FI_EAGAIN,
	[GNI_RC_INVALID_PARAM] = -FI_EINVAL,
	[GNI_RC_ERROR_RESOURCE] = -FI_EBUSY,
	[GNI_RC_TIMEOUT] = -FI_ETIMEDOUT,
	[GNI_RC_PERMISSION_ERROR] = -FI_EACCES,
	[GNI_RC_DESCRIPTOR_ERROR] = -FI_EOTHER,
	[GNI_RC_ALIGNMENT_ERROR] = -FI_EINVAL,
	[GNI_RC_INVALID_STATE] = -FI_EOPBADSTATE,
	[GNI_RC_NO_MATCH] = -FI_EINVAL,
	[GNI_RC_SIZE_ERROR] = -FI_ETOOSMALL,
	[GNI_RC_TRANSACTION_ERROR] = -FI_ECANCELED,
	[GNI_RC_ILLEGAL_OP] = -FI_EOPNOTSUPP,
	[GNI_RC_ERROR_NOMEM] = -FI_ENOMEM
};

int gnixu_to_fi_errno(int err)
{
	if (err >= 0 && err < NUM_GNI_RC)
		return gnix_rc_table[err];
	else
		return -FI_EOTHER;
}

/* Indicate that the next task spawned will be restricted to cores assigned to
 * corespec. */
int _gnix_task_is_not_app(void)
{
	size_t count;
	int fd;
	char filename[PATH_MAX];
	int rc = 0;
	char val_str[] = "0";
	int val_str_len = strlen(val_str);

	snprintf(filename, PATH_MAX, "/proc/self/task/%ld/task_is_app",
		      syscall(SYS_gettid));
	fd = open(filename, O_WRONLY);
	if (fd < 0) {
		GNIX_WARN(FI_LOG_FABRIC, "open(%s) failed, errno=%s\n",
			  filename, strerror(errno));
		return -errno;
	}

	count = write(fd, val_str, val_str_len);
	if (count != val_str_len) {
		GNIX_WARN(FI_LOG_FABRIC, "write(%s, %s) failed, errno=%s\n",
			  filename, val_str, strerror(errno));
		rc = -errno;
	}
	close(fd);

	return rc;
}

static int gnix_write_proc_job(char *val_str)
{
	size_t count;
	int fd;
	int rc = 0;
	char *filename = "/proc/job";
	int val_str_len = strlen(val_str);

	fd = open(filename, O_WRONLY);
	if (fd < 0) {
		GNIX_WARN(FI_LOG_FABRIC, "open(%s) failed, errno=%s\n",
			  filename, strerror(errno));
		return -errno;
	}

	count = write(fd, val_str, val_str_len);
	if (count != val_str_len) {
		GNIX_WARN(FI_LOG_FABRIC, "write(%s) failed, errno=%s\n",
			  val_str, strerror(errno));
		rc = -errno;
	}
	close(fd);

	return rc;
}

/* Indicate that the next task spawned will be restricted to CPUs that are not
 * assigned to the app and not assigned to corespec. */
int _gnix_job_enable_unassigned_cpus(void)
{
	return gnix_write_proc_job("enable_affinity_unassigned_cpus");
}

/* Indicate that the next task spawned will be restricted to CPUs that are
 * assigned to the app. */
int _gnix_job_disable_unassigned_cpus(void)
{
	return gnix_write_proc_job("disable_affinity_unassigned_cpus");
}

/* Indicate that the next task spawned should adhere to the affinity rules. */
int _gnix_job_enable_affinity_apply(void)
{
	return gnix_write_proc_job("enable_affinity_apply");
}

/* Indicate that the next task spawned should avoid the affinity rules and be
 * allowed to run anywhere in the app cpuset. */
int _gnix_job_disable_affinity_apply(void)
{
	return gnix_write_proc_job("disable_affinity_apply");
}


int _gnix_job_fma_limit(uint32_t dev_id, uint8_t ptag, uint32_t *limit)
{
	gni_return_t status;
	gni_job_res_desc_t job_res_desc;

	if (!limit) {
		return -FI_EINVAL;
	}

	status = GNI_GetJobResInfo(dev_id, ptag, GNI_JOB_RES_FMA, &job_res_desc);
	if (status) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "GNI_GetJobResInfo(%d, %d) failed, status=%s\n",
			  dev_id, ptag, gni_err_str[status]);
		return -FI_EINVAL;
	}

	*limit = job_res_desc.limit;
	GNIX_INFO(FI_LOG_FABRIC, "fma_limit: %u\n", job_res_desc.limit);

	return FI_SUCCESS;
}

int _gnix_job_cq_limit(uint32_t dev_id, uint8_t ptag, uint32_t *limit)
{
	gni_return_t status;
	gni_job_res_desc_t job_res_desc;

	if (!limit) {
		return -FI_EINVAL;
	}

	status = GNI_GetJobResInfo(dev_id, ptag, GNI_JOB_RES_CQ, &job_res_desc);
	if (status) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "GNI_GetJobResInfo(%d, %d) failed, status=%s\n",
			  dev_id, ptag, gni_err_str[status]);
		return -FI_EINVAL;
	}

	*limit = job_res_desc.limit;
	GNIX_INFO(FI_LOG_FABRIC, "cq_limit: %u\n", job_res_desc.limit);

	return FI_SUCCESS;
}

int _gnix_pes_on_node(uint32_t *num_pes)
{
	int rc;

	if (!num_pes) {
		return -FI_EINVAL;
	}

	rc = __gnix_app_init();
	if (rc) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "__gnix_app_init() failed, ret=%d(%s)\n",
			  rc, strerror(errno));
		return rc;
	}

	*num_pes = gnix_pes_on_node;
	GNIX_INFO(FI_LOG_FABRIC, "num_pes: %u\n", gnix_appLayout.numPesHere);

	return FI_SUCCESS;
}

int _gnix_pe_node_rank(int *pe_node_rank)
{
	int rc;

	if (!pe_node_rank) {
		return -FI_EINVAL;
	}

	rc = __gnix_app_init();
	if (rc) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "__gnix_app_init() failed, ret=%d(%s)\n",
			  rc, strerror(errno));
		return rc;
	}

	if (gnix_pe_node_rank != -1) {
		*pe_node_rank = gnix_pe_node_rank;
		rc = FI_SUCCESS;
	} else
		rc = -FI_EADDRNOTAVAIL;

	GNIX_INFO(FI_LOG_FABRIC, "pe_node_rank: %u\n", gnix_pe_node_rank);

	return rc;
}

int _gnix_nics_per_rank(uint32_t *nics_per_rank)
{
	int rc;
	uint32_t npes, fmas, cqs, limiting_resource;

	if (!nics_per_rank) {
		return -FI_EINVAL;
	}

	rc = __gnix_app_init();
	if (rc) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "__gnix_app_init() failed, ret=%d(%s)\n",
			  rc, strerror(errno));
		return rc;
	}

	rc = _gnix_job_fma_limit(gnix_device_id, gnix_app_ptag, &fmas);
	if (rc) {
		return rc;
	}

	rc = _gnix_job_cq_limit(gnix_device_id, gnix_app_ptag, &cqs);
	if (rc) {
		return rc;
	}

	gnix_cq_limit = cqs;
	cqs /= GNIX_CQS_PER_EP;

	rc = _gnix_pes_on_node(&npes);
	if (rc) {
		return rc;
	}

	limiting_resource = fmas > cqs ? cqs : fmas;

	*nics_per_rank = limiting_resource / npes;

	return FI_SUCCESS;
}

void _gnix_dump_gni_res(uint8_t ptag)
{
	int i;
	gni_return_t status;
	gni_dev_res_desc_t dev_res_desc;
	gni_job_res_desc_t job_res_desc;
#define BUF_SZ 4096
	char buf[BUF_SZ];
	int size = BUF_SZ, written = 0;

	if (!fi_log_enabled(&gnix_prov, FI_LOG_WARN, FI_LOG_FABRIC))
		return;

	written += snprintf(buf + written, size - written,
			    "Device Resources:\n");
	for (i = GNI_DEV_RES_FIRST+1; i < GNI_DEV_RES_LAST; i++) {
		status = GNI_GetDevResInfo(0, i, &dev_res_desc);
		if (status == GNI_RC_SUCCESS) {
			written += snprintf(buf + written, size - written,
					    "dev res: %9s, avail: %lu res: %lu held: %lu total: %lu\n",
					    gni_dev_res_to_str(i),
					    dev_res_desc.available,
					    dev_res_desc.reserved,
					    dev_res_desc.held,
					    dev_res_desc.total);
		}
	}

	GNIX_WARN(FI_LOG_FABRIC, "%s", buf);

	written = 0;
	written += snprintf(buf + written, size - written,
			    "Job Resources:\n");
	for (i = GNI_JOB_RES_FIRST+1; i < GNI_JOB_RES_LAST; i++) {
		status = GNI_GetJobResInfo(0, ptag, i, &job_res_desc);
		if (status == GNI_RC_SUCCESS) {
			written += snprintf(buf + written, size - written,
					    "ptag[%d] job res: %9s used: %lu limit: %lu\n",
					    ptag, gni_job_res_to_str(i),
					    job_res_desc.used,
					    job_res_desc.limit);
		}
	}

	GNIX_WARN(FI_LOG_FABRIC, "%s", buf);
}

int _gnix_get_num_corespec_cpus(uint32_t *num_core_spec_cpus)
{
	int ret = -FI_ENODATA;
	int ncpus = 0;
	FILE *fd = NULL;
	char buffer[4096], *line, *field;
	static bool already_called;
	static uint32_t cached_num_corespec_cpus;

	if (num_core_spec_cpus == NULL)
		return -FI_EINVAL;

	if (already_called == true) {
		*num_core_spec_cpus = cached_num_corespec_cpus;
		return FI_SUCCESS;
	}

	fd = fopen("/proc/job", "r");
	if (!fd) {
		GNIX_WARN(FI_LOG_FABRIC,
			  "open of /proc/job returned %s", strerror(errno));
		return -errno;
	}

	while (1) {
		line = fgets(buffer, sizeof(buffer), fd);
		if (!line)
			break;

		line = strstr(line, "corespec");
		if (line != NULL) {
			field  = strtok(line, " ");
			field  = strtok(NULL, " ");
			if (!strcmp(field, "num_sys_cpus")) {
				field = strtok(NULL, " ");
				ncpus = atoi(field);
			}
			ret = FI_SUCCESS;
			break;
		}
	}

	*num_core_spec_cpus = ncpus;
	cached_num_corespec_cpus = ncpus;

	already_called = true;

	fclose(fd);

	return ret;
}

