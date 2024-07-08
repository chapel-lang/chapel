/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_H
#define EFA_H

#include "config.h"

#include <asm/types.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/epoll.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>

#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

#include "ofi.h"
#include "ofi_iov.h"
#include "ofi_enosys.h"
#include "ofi_list.h"
#include "ofi_util.h"
#include "ofi_file.h"

#include "efa_base_ep.h"
#include "efa_mr.h"
#include "efa_env.h"
#include "efa_shm.h"
#include "efa_prov.h"
#include "efa_hmem.h"
#include "efa_device.h"
#include "efa_domain.h"
#include "efa_errno.h"
#include "efa_user_info.h"
#include "efa_fork_support.h"
#include "rdm/efa_rdm_ep.h"
#include "rdm/efa_rdm_ope.h"
#include "rdm/efa_rdm_pke.h"
#include "rdm/efa_rdm_peer.h"
#include "rdm/efa_rdm_util.h"
#include "fi_ext_efa.h"

#define EFA_ABI_VER_MAX_LEN 8

#define SHM_MAX_INJECT_SIZE 4096

#define EFA_EP_TYPE_IS_RDM(_info) \
	(_info && _info->ep_attr && (_info->ep_attr->type == FI_EP_RDM))

#define EFA_EP_TYPE_IS_DGRAM(_info) \
	(_info && _info->ep_attr && (_info->ep_attr->type == FI_EP_DGRAM))

#define EFA_DGRAM_CONNID (0x0)

#define EFA_DEF_POOL_ALIGNMENT (8)
#define EFA_MEM_ALIGNMENT (64)

#define EFA_DEF_CQ_SIZE 1024


#define EFA_DEFAULT_RUNT_SIZE (307200)
#define EFA_NEURON_RUNT_SIZE (131072)
#define EFA_DEFAULT_INTER_MAX_MEDIUM_MESSAGE_SIZE (65536)
#define EFA_DEFAULT_INTER_MIN_READ_MESSAGE_SIZE (1048576)
#define EFA_DEFAULT_INTER_MIN_READ_WRITE_SIZE (65536)
#define EFA_DEFAULT_INTRA_MAX_GDRCOPY_FROM_DEV_SIZE (3072)

/*
 * The default memory alignment
 */
#define EFA_RDM_DEFAULT_MEMORY_ALIGNMENT (8)

/*
 * The CUDA memory alignment
 */
#define EFA_RDM_CUDA_MEMORY_ALIGNMENT (64)

/*
 * The alignment to support in-order aligned ops.
 */
#define EFA_RDM_IN_ORDER_ALIGNMENT (128)

/*
 * Set alignment to x86 cache line size.
 */
#define EFA_RDM_BUFPOOL_ALIGNMENT	(64)

struct efa_fabric {
	struct util_fabric	util_fabric;
	struct fid_fabric *shm_fabric;
#ifdef EFA_PERF_ENABLED
	struct ofi_perfset perf_set;
#endif
};

static inline
int efa_str_to_ep_addr(const char *node, const char *service, struct efa_ep_addr *addr)
{
	int ret;

	if (!node)
		return -FI_EINVAL;

	memset(addr, 0, sizeof(*addr));

	ret = inet_pton(AF_INET6, node, addr->raw);
	if (ret != 1)
		return -FI_EINVAL;
	if (service)
		addr->qpn = atoi(service);

	return 0;
}

#define EFA_HOST_ID_STRING_LENGTH 19
#define EFA_HOST_ID_PREFIX_LENGTH 3 /* host ID prefix is "i-0" */

static inline
uint64_t efa_get_host_id(char *host_id_file)
{
	FILE *fp = NULL;
	char host_id_str[EFA_HOST_ID_STRING_LENGTH - EFA_HOST_ID_PREFIX_LENGTH + 1];
	char *end_ptr = NULL;
	size_t length = 0;
	uint64_t host_id = 0;

	if (!host_id_file) {
		EFA_WARN(FI_LOG_EP_CTRL, "Host id file is not specified\n");
		goto out;
	}

	fp = fopen(host_id_file, "r");
	if (!fp) {
		EFA_WARN(FI_LOG_EP_CTRL, "Cannot open host id file: %s\n", host_id_file);
		goto out;
	}

	if (fseek(fp, EFA_HOST_ID_PREFIX_LENGTH, SEEK_SET) < 0) {
		EFA_WARN(FI_LOG_EP_CTRL, "Cannot locate host id in file\n");
		goto out;
	}

	length = fread(host_id_str, 1, EFA_HOST_ID_STRING_LENGTH - EFA_HOST_ID_PREFIX_LENGTH, fp);
	if (length != EFA_HOST_ID_STRING_LENGTH - EFA_HOST_ID_PREFIX_LENGTH) {
		EFA_WARN(FI_LOG_EP_CTRL, "Failed to read host id. Read length: %lu Expect length: %d\n",
			 length, EFA_HOST_ID_STRING_LENGTH - EFA_HOST_ID_PREFIX_LENGTH);
		goto out;
	}

	host_id_str[EFA_HOST_ID_STRING_LENGTH - EFA_HOST_ID_PREFIX_LENGTH] = '\0';

	host_id = (uint64_t)strtoul(host_id_str, &end_ptr, 16);
	if (*end_ptr != '\0') {
		EFA_WARN(FI_LOG_EP_CTRL, "Host id is not a valid hex string: %s\n", host_id_str);
		host_id = 0;
	}

out:
	if (fp) {
		fclose(fp);
	}
	return host_id;
}

static inline
bool efa_is_same_addr(struct efa_ep_addr *lhs, struct efa_ep_addr *rhs)
{
	return !memcmp(lhs->raw, rhs->raw, sizeof(lhs->raw)) &&
	       lhs->qpn == rhs->qpn && lhs->qkey == rhs->qkey;
}

int efa_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric_fid,
	       void *context);

/* Performance counter declarations */
#ifdef EFA_PERF_ENABLED
#define EFA_PERF_FOREACH(DECL)	\
	DECL(perf_efa_tx),	\
	DECL(perf_efa_recv),	\
	DECL(efa_perf_size)	\

enum efa_perf_counters {
	EFA_PERF_FOREACH(OFI_ENUM_VAL)
};

extern const char *efa_perf_counters_str[];

static inline void efa_perfset_start(struct efa_rdm_ep *ep, size_t index)
{
	struct efa_domain *domain = efa_rdm_ep_domain(ep);
	struct efa_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct efa_fabric,
						 util_fabric);
	ofi_perfset_start(&fabric->perf_set, index);
}

static inline void efa_perfset_end(struct efa_rdm_ep *ep, size_t index)
{
	struct efa_domain *domain = efa_rdm_ep_domain(ep);
	struct efa_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct efa_fabric,
						 util_fabric);
	ofi_perfset_end(&fabric->perf_set, index);
}
#else
#define efa_perfset_start(ep, index) do {} while (0)
#define efa_perfset_end(ep, index) do {} while (0)
#endif

#endif /* EFA_H */
