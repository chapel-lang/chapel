/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2022 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DOMAIN_H_
#define _FI_PROV_OPX_DOMAIN_H_

#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <uuid/uuid.h>
#include <uthash.h>

#include "rdma/fi_domain.h"

#include "rdma/opx/fi_opx_reliability.h"

#include "rdma/opx/fi_opx_tid_domain.h"

//#define OFI_RELIABILITY_CONFIG_STATIC_NONE
//#define OFI_RELIABILITY_CONFIG_STATIC_OFFLOAD
//#define OFI_RELIABILITY_CONFIG_STATIC_ONLOAD

#if defined(OFI_RELIABILITY_CONFIG_STATIC_NONE)
#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_NONE

#elif defined(OFI_RELIABILITY_CONFIG_STATIC_OFFLOAD)
#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_OFFLOAD

#elif defined(OFI_RELIABILITY_CONFIG_STATIC_ONLOAD)
#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_ONLOAD

#else

#ifndef OPX_DOMAIN_RELIABILITY
//#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_NONE
//#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_OFFLOAD
#define OPX_DOMAIN_RELIABILITY OFI_RELIABILITY_KIND_ONLOAD
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

struct fi_opx_ep;	/* forward declaration */


struct fi_opx_tid_fabric;
struct fi_opx_fabric {
	struct fid_fabric	fabric_fid;

	int64_t		ref_cnt;
	struct fi_opx_tid_fabric* tid_fabric;
};


struct fi_opx_node {
	volatile uint64_t	ep_count;
};

#define OPX_JOB_KEY_STR_SIZE 33
#define OPX_DEFAULT_JOB_KEY_STR "00112233445566778899aabbccddeeff"

#define OPX_DEFAULT_PROG_AFFINITY_STR "0:3:1"

#define OPX_MIN_DCOMP_THRESHOLD FI_OPX_SDMA_MIN_LENGTH
#define OPX_DEFAULT_DCOMP_THRESHOLD FI_OPX_SDMA_DC_MIN
#define OPX_MAX_DCOMP_THRESHOLD INT_MAX

struct fi_opx_domain {
	struct fid_domain	domain_fid;
	struct fi_opx_fabric	*fabric;

	enum fi_threading	threading;
	enum fi_resource_mgmt	resource_mgmt;
	enum fi_mr_mode		mr_mode;
	enum fi_progress	data_progress;

	uuid_t			unique_job_key;
	char			unique_job_key_str[OPX_JOB_KEY_STR_SIZE];

	char			progress_affinity_str[OPX_JOB_KEY_STR_SIZE];

	int			auto_progress_interval;

	uint32_t		rx_count;
	uint32_t		tx_count;
	uint8_t			ep_count;

	uint64_t		num_mr_keys;
	struct fi_opx_mr        *mr_hashmap;

	struct fi_opx_reliability_service	reliability_service_offload;	/* OFFLOAD only */
	uint8_t					reliability_rx_offload;		/* OFFLOAD only */
	enum ofi_reliability_kind		reliability_kind;

	struct fi_opx_tid_domain *tid_domain;

	int64_t		ref_cnt;
};

struct fi_opx_av {

	/* == CACHE LINE 0 == */

	struct fid_av		av_fid;		/* 32 bytes */
	struct fi_opx_domain	*domain;
	void			*map_addr;
	int64_t		ref_cnt;
	uint32_t		addr_count;
	enum fi_av_type		type;
	unsigned		ep_tx_count;

	/* == CACHE LINE 1..20 == */

	struct fi_opx_ep	*ep_tx[160];

	/* == ALL OTHER CACHE LINES == */

	union fi_opx_addr *	table_addr; /* allocated buffer to free */
	uint64_t		rx_ctx_bits;
	uint32_t		table_count;/* table, not av, count */
};

struct fi_opx_mr {
	struct fid_mr		mr_fid;
	struct fi_opx_domain	*domain;
	UT_hash_handle	hh;
	const void		*buf;
	size_t			len;
	size_t			offset;
	uint64_t		access;
	uint64_t		flags;
	uint64_t		cntr_bflags;
	struct fi_opx_cntr	*cntr;
	struct fi_opx_ep	*ep;
};

static inline uint32_t
fi_opx_domain_get_tx_max(struct fid_domain *domain) {
	return 160;
}

static inline uint32_t
fi_opx_domain_get_rx_max(struct fid_domain *domain) {
	return 160;
}

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_DOMAIN_H_ */
