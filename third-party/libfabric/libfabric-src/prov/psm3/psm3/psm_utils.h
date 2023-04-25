/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef _PSMI_IN_USER_H
#error psm_utils.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_UTILS_H
#define _PSMI_UTILS_H

#include <arpa/inet.h>		/* ipv4addr */
#include <stdlib.h>		/* malloc/free */
#include <psm_netutils.h>
#ifdef PSM_VERBS
#include <infiniband/verbs.h>
#endif

/* Easy way to ignore the OK_NO_PROGRESS case */
PSMI_ALWAYS_INLINE(psm2_error_t psmi_err_only(psm2_error_t err))
{
	if (err > PSM2_OK_NO_PROGRESS)
		return err;
	else
		return PSM2_OK;
}

PSMI_ALWAYS_INLINE(int psmi_bare_netaddr128_cmp(psmi_bare_netaddr128_t a, psmi_bare_netaddr128_t b))
{
	if (a.hi < b.hi)
		return -1;
	if (a.hi > b.hi)
		return 1;
	if (a.lo < b.lo)
		return -1;
	if (a.lo == b.lo)
		return 0;
	return 1;
}

// for internal use only, use naddr128_cmp and subnet128_cmp instead (below)
PSMI_ALWAYS_INLINE(int psmi_qual_netaddr128_cmp(psmi_naddr128_t a, psmi_naddr128_t b))
{
	int ret;
	if (a.fmt < b.fmt)
		return -1;
	if (a.fmt > b.fmt)
		return 1;
	ret = psmi_bare_netaddr128_cmp(a.bare, b.bare);
	if (ret)
		return ret;
	if (a.prefix_len < b.prefix_len)
		return -1;
	if (a.prefix_len > b.prefix_len)
		return 1;
	return 0;
}

PSMI_ALWAYS_INLINE(int psmi_naddr128_cmp(psmi_naddr128_t a, psmi_naddr128_t b))
{
	return psmi_qual_netaddr128_cmp(a, b);
}

PSMI_ALWAYS_INLINE(int psmi_subnet128_cmp(psmi_subnet128_t a, psmi_subnet128_t b))
{
	return psmi_qual_netaddr128_cmp(a, b);
}

/*
 * See psm_netutils.h for epid_addr_fmt defines, we need those to
 * be accessed within the HAL, so we can't define them here because
 * this file requires psm_user.h which can't be used inside the HAL
 * because the HAL makes use of malloc/free directly
 */

/*
 * Endpoint 'id' hash table, with iterator interface
 */
struct psm3_epid_table {
	struct psm3_epid_tabentry *table;
	int tabsize;
	int tabsize_used;
	pthread_mutex_t tablock;
};
/*
 * Endpoint address hash table
 */
struct psm3_epid_tabentry {
	void *entry;
	uint64_t key;
	psm2_ep_t ep;
	psm2_epid_t epid;
};

extern struct psm3_epid_table psm3_epid_table;
#define EPADDR_DELETED	((void *)-1)	/* tag used to mark deleted entries */

psm2_error_t psm3_epid_init();
psm2_error_t psm3_epid_fini();
void *psm3_epid_lookup(psm2_ep_t ep, psm2_epid_t epid);
void *psm3_epid_remove(psm2_ep_t ep, psm2_epid_t epid);
void psm3_epid_remove_all(psm2_ep_t ep);
psm2_error_t psm3_epid_add(psm2_ep_t ep, psm2_epid_t epid, void *entry);
#define PSMI_EP_HOSTNAME    ((psm2_ep_t) -1)	/* Special endpoint handle we use
						 * to register hostnames */
#define PSMI_EP_CROSSTALK   ((psm2_ep_t) -2)	/* Second special endpoint handle
						 * to log which nodes we've seen
						 * crosstalk from */
struct psmi_eptab_iterator {
	int i;			/* last index looked up */
	psm2_ep_t ep;
};
void psm3_epid_itor_init(struct psmi_eptab_iterator *itor, psm2_ep_t ep);
void *psm3_epid_itor_next(struct psmi_eptab_iterator *itor);
void psm3_epid_itor_fini(struct psmi_eptab_iterator *itor);

/* These functions build the local epid */
// for typical job which includes IPS inter-node comms
psm2_epid_t psm3_epid_pack_ib(uint16_t lid, uint32_t qp_num,
	psmi_naddr128_t addr);
// IPv4 Ethernet (RoCE or UDP/TCP)
// for RoCE context is QPN (24b), for UDP/TCP it's primary socket number (16b)
psm2_epid_t psm3_epid_pack_ipv4(psmi_naddr128_t ipv4_addr,
				psmi_eth_proto_t pprotocol,
				uint32_t context, uint16_t aux_sock);
// IPv6 Ethernet (RoCE or UDP/TCP)
// for RoCE context is QPN (24b), for UDP/TCP it's primary socket number (16b)
psm2_epid_t psm3_epid_pack_ipv6(psmi_naddr128_t ipv6_addr,
				psmi_eth_proto_t protocol,
				uint32_t context, uint16_t aux_sock);

// for a shm-only job (1 node job)
psm2_epid_t psm3_epid_pack_shm(const psm2_uuid_t unique_job_key);

// for a self-only job (1 proc job)
psm2_epid_t psm3_epid_pack_self(void);

// for internal self-test of epid/epaddr hash and lookup
psm2_epid_t psm3_epid_pack_diag(int val);

// These functions extract fields/information from the epid
uint8_t psm3_epid_addr_fmt(psm2_epid_t epid);
psmi_eth_proto_t psm3_epid_protocol(psm2_epid_t epid);
psm2_nid_t psm3_epid_nid(psm2_epid_t epid);
const char *psm3_subnet_epid_subset_fmt(psmi_subnet128_t subnet, int bufno);
psmi_subnet128_t psm3_epid_subnet(psm2_epid_t epid);
uint8_t psm3_epid_prefix_len(psm2_epid_t epid);
uint64_t psm3_epid_port(psm2_epid_t epid);
uint64_t psm3_epid_context(psm2_epid_t epid);
#ifdef PSM_SOCKETS
uint16_t psm3_epid_aux_socket(psm2_epid_t epid);
#endif
void psm3_epid_get_av(psm2_epid_t epid, uint16_t *lid, psmi_gid128_t *gid);
uint32_t psm3_epid_get_rem_addr(psm2_epid_t epid);
uint16_t psm3_epid_lid(psm2_epid_t epid);
uint64_t psm3_epid_hash(psm2_epid_t epid);

psm2_nid_t psm3_build_nid(uint8_t unit, psmi_naddr128_t addr, unsigned lid);

// compare subnets based on comparison rules for given addr_fmt
// This considers whether routing is possible and PSM3_ALLOW_ROUTERS is set
int psm3_subnets_match(psmi_subnet128_t a, psmi_subnet128_t b);

// for some formats the epid only has a subset of the subnet, compare
// just the subset available in epid
int psm3_subnets_match_epid(psmi_subnet128_t subnet, psm2_epid_t epid);

#ifdef PSM_SOCKETS
// manage sockaddr fundamentals
int psm3_sockaddr_cmp(struct sockaddr_in6 *a, struct sockaddr_in6 *b);
// build an AF_INET6 sockaddr
// can be for a IPv4 (GID ::ffff:<ipaddr>) or IPv6 style GID
void psm3_build_sockaddr(struct sockaddr_in6 *in6, uint16_t port,
				uint64_t gid_hi, uint64_t gid_lo,
				uint32_t scope_id);
void psm3_epid_build_sockaddr(struct sockaddr_in6 *in6, psm2_epid_t epid,
				uint32_t scope_id);
void psm3_epid_build_aux_sockaddr(struct sockaddr_in6 *in6, psm2_epid_t epid,
				uint32_t scope_id);
#endif
int psm3_epid_cmp_internal(psm2_epid_t a, psm2_epid_t b);
int psm3_epid_zero_internal(psm2_epid_t a);
psm2_epid_t psm3_epid_zeroed_internal(void);

// NID is just a special subset of epid values where context/subctxt/qp_num == 0
// so we can use the epid function to cmp and clear a psm2_nid_t
PSMI_ALWAYS_INLINE(int psm3_nid_cmp_internal(psm2_nid_t a, psm2_nid_t b))
{
	return psm3_epid_cmp_internal(a, b);
}

PSMI_ALWAYS_INLINE(int psm3_nid_zero_internal(psm2_nid_t a))
{
	return psm3_epid_zero_internal(a);
}

PSMI_ALWAYS_INLINE(psm2_nid_t psm3_nid_zeroed_internal(void))
{
	return psm3_epid_zeroed_internal();
}

#define PSMI_EPID_LEN (sizeof(uint64_t)*3) // in bytes
psm2_epid_t psm3_epid_pack_words(uint64_t w0, uint64_t w1, uint64_t w2);
uint64_t psm3_epid_w0(psm2_epid_t epid);
uint64_t psm3_epid_w1(psm2_epid_t epid);
uint64_t psm3_epid_w2(psm2_epid_t epid);


/*
 * Hostname manipulation
 */
char *psm3_gethostname(void);
const char *psm3_epid_fmt_internal(psm2_epid_t epid, int bufno);
const char *psm3_epid_fmt_context(psm2_epid_t epid, int bufno);
const char *psm3_epid_fmt_nid(psm2_epid_t epid, int bufno);
const char *psm3_epid_fmt_addr(psm2_epid_t epid, int bufno);
const char *psm3_epid_fmt_subnet(psm2_epid_t epid, int bufno);
const char *psm3_epid_str_addr_fmt(psm2_epid_t epid);
const char *psm3_epid_str_protocol(psm2_epid_t epid);
const char *psm3_epaddr_get_hostname(psm2_epid_t epid, int bufno);
const char *psm3_epaddr_get_name(psm2_epid_t epid, int bufno);
psm2_error_t psm3_epid_set_hostname(psm2_nid_t nid, const char *hostname,
				   int overwrite);
const char *psm3_nid_fmt(psm2_nid_t nid, int bufno);

#ifdef PSM_VERBS
const char *psm3_ibv_gid_fmt(union ibv_gid gid, int bufno);
int psm3_nonzero_gid(const union ibv_gid *gid);
#endif

/* PSM3_IDENTIFY output */

void psm3_print_rank_identify(void);
void psm3_print_ep_identify(psm2_ep_t ep);


/*
 * Memory allocation, use macros only.
 *
 * In all calls, ep can be a specific endpoint (valid psm2_ep_t) or PSMI_EP_NONE
 * if no endpoint is available.
 *
 *   psmi_malloc_usable_size(void *ptr)
 *   psmi_malloc(ep, memtype, size)
 *   psmi_realloc(ep, memtype, ptr, newsize)
 *   psmi_memalign(ep, memtype, alignment, size)
 *   psmi_calloc(ep, memtype, elemsz, numelems)
 *   psmi_strdup(ep, ptr)
 *   psmi_free(ptr)
 *
 */
typedef enum psmi_memtype {
	TOTAL = 0,		/* Logged automatically by malloc/calloc */
	UNDEFINED,		/* For tracking "other types" of allocations */
	PER_PEER_ENDPOINT,	/* For tracking "per peer" allocations */
	NETWORK_BUFFERS,	/* For tracking network buffers */
	DESCRIPTORS,		/* For tracking send/recv descriptors */
	UNEXPECTED_BUFFERS,	/* For tracking unexpected recv buffers */
	STATS,			/* For tracking stats-related allocs */
#if defined(PSM_VERBS) || defined(PSM_SOCKETS)
#ifdef RNDV_MOD
	// TBD, should we just tabulate this into PER_PEER_ENDPOINT
	// maybe once debugged we should consolidate?
	PEER_RNDV,			/* for tracking Rendezvous per RC QP resources */
#endif
#endif
} psmi_memtype_t;

/*
 * We track allocation stats.
 */
struct psmi_stats_malloc {
	int64_t m_all_total;
	int64_t m_all_max;
	int64_t m_perpeer_total;
	int64_t m_perpeer_max;
	int64_t m_netbufs_total;
	int64_t m_netbufs_max;
	int64_t m_descriptors_total;
	int64_t m_descriptors_max;
	int64_t m_unexpbufs_total;
	int64_t m_unexpbufs_max;
	int64_t m_undefined_total;
	int64_t m_undefined_max;
	int64_t m_stats_total;
	int64_t m_stats_max;
#if defined(PSM_VERBS) || defined(PSM_SOCKETS)
#ifdef RNDV_MOD
	int64_t m_peerrndv_total;
	int64_t m_peerrndv_max;
#endif
#endif
};

extern struct psmi_stats_malloc psm3_stats_memory;

void psm3_mem_stats_register(void);

void *psm3_malloc_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t sz,
			   const char *curloc);
void *psm3_realloc_internal(psm2_ep_t ep, psmi_memtype_t mt, void *ptr,
			    size_t newSz, const char *curloc);
void *psm3_memalign_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t alignment,
			     size_t sz, const char *curloc);
void *psm3_calloc_internal(psm2_ep_t ep, psmi_memtype_t mt, size_t num,
			   size_t sz, const char *curloc);
void *psm3_strdup_internal(psm2_ep_t ep, const char *string, const char *curloc);

void MOCKABLE(psm3_free_internal)(void *ptr, const char *curLoc);
MOCK_DCL_EPILOGUE(psm3_free_internal);

size_t psm3_malloc_usable_size_internal(void *ptr, const char *curLoc);

#ifdef PSM_HEAP_DEBUG
/* During heap debug code, we can sprinkle function calls:
   psmi_heapdebug_val_heapallocs(), that will examine all of the heap allocations
   to ensure integrity. */
void _psmi_heapdebug_val_heapallocs(const char *curloc);

#define psmi_heapdebug_val_heapallocs() _psmi_heapdebug_val_heapallocs(PSMI_CURLOC)

/* Finialize the heapdebug functionality after tear down of the psm
   session when you are certain that all heap allocations have been
   freed. psmi_heapdebug_finalize() will emit all of the extant
   heap allocations and abort if there are any.  This is to aid
   in debug of heap leaks. */
void psmi_heapdebug_finalize(void);

#else

#define psmi_heapdebug_val_heapallocs() /* nothing */
#define psmi_heapdebug_finalize() /* nothing */

#endif

#define psmi_strdup(ep, string) psm3_strdup_internal(ep, string, PSMI_CURLOC)
#define psmi_calloc(ep, mt, nelem, elemsz) \
	psm3_calloc_internal(ep, mt, nelem, elemsz, PSMI_CURLOC)
#define psmi_malloc(ep, mt, sz) psm3_malloc_internal(ep, mt, sz, PSMI_CURLOC)
#define psmi_realloc(ep, mt, ptr, nsz) psm3_realloc_internal(ep, mt, ptr, nsz, PSMI_CURLOC)
#define psmi_memalign(ep, mt, al, sz) \
	psm3_memalign_internal(ep, mt, al, sz, PSMI_CURLOC)
#define psmi_free(ptr)	psm3_free_internal(ptr, PSMI_CURLOC)
#define psmi_malloc_usable_size(ptr) psm3_malloc_usable_size_internal(ptr, PSMI_CURLOC)
#ifndef PSM_IS_TEST
#define malloc(sz)        _use_psmi_malloc_instead_of_plain_malloc
#define realloc(ptr,nsz)  _use_psmi_realloc_instead_of_plain_realloc
#define memalign(algn,sz) _use_psmi_memalign_instead_of_plain_memalign
#define calloc(sz, nelm)  _use_psmi_calloc_instead_of_plain_calloc
#ifdef strdup
#undef strdup
#endif
#define strdup(ptr)             _use_psmi_strdup_instead_of_plain_strdup
#define free(ptr)               _use_psmi_free_instead_of_plain_free
#define malloc_usable_size(ptr) _use_psmi_malloc_usable_size_instead_of_plain_malloc_usable_size
#endif /* PSM_IS_TEST */

void psm3_log_memstats(psmi_memtype_t type, int64_t nbytes);

/*
 * Parse int parameters
 * -1 -> parse error
 */
long psm3_parse_str_long(const char *str);

/*
 * Parsing int parameters set in string tuples.
 */
int psm3_parse_str_tuples(const char *str, int ntup, int *vals);

/*
 * Resource Limiting based on PSM memory mode.
 */
#define PSMI_MEMMODE_NORMAL  0
#define PSMI_MEMMODE_MINIMAL 1
#define PSMI_MEMMODE_LARGE   2
#define PSMI_MEMMODE_NUM     3

struct psmi_rlimit_mpool {
	const char *env;
	const char *descr;
	int env_level;
	uint32_t minval;
	uint32_t maxval;
	struct {
		uint32_t obj_chunk;
		uint32_t obj_max;
	} mode[PSMI_MEMMODE_NUM];
};
psm2_error_t psm3_parse_mpool_env(const psm2_mq_t mq, int level,
				 const struct psmi_rlimit_mpool *rlim,
				 uint32_t *valo, uint32_t *chunkszo);
int psm3_parse_memmode(void);
int psm3_parse_identify(void);
#ifdef PSM_HAVE_REG_MR
unsigned psm3_parse_senddma(void);
#endif
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
unsigned psmi_parse_gpudirect(void);
unsigned psmi_parse_gpudirect_rdma_send_limit(int force);
unsigned psmi_parse_gpudirect_rdma_recv_limit(int force);
unsigned psmi_parse_gpudirect_rv_gpu_cache_size(int reload);
#endif

/*
 * Parsing environment variables
 */

union psmi_envvar_val {
	void *e_void;
	char *e_str;
	int e_int;
	unsigned int e_uint;
	long e_long;
	unsigned long e_ulong;
	unsigned long long e_ulonglong;
};

#define PSMI_ENVVAR_LEVEL_USER	         1
#define PSMI_ENVVAR_LEVEL_HIDDEN         2
#define PSMI_ENVVAR_LEVEL_NEVER_PRINT    4

#define PSMI_ENVVAR_TYPE_YESNO		0
#define PSMI_ENVVAR_TYPE_STR		1
#define PSMI_ENVVAR_TYPE_INT		2
#define PSMI_ENVVAR_TYPE_UINT		3
#define PSMI_ENVVAR_TYPE_UINT_FLAGS	4
#define PSMI_ENVVAR_TYPE_LONG		5
#define PSMI_ENVVAR_TYPE_ULONG		6
#define PSMI_ENVVAR_TYPE_ULONG_FLAGS	7
#define PSMI_ENVVAR_TYPE_ULONG_ULONG    8

#define PSMI_ENVVAR_VAL_YES ((union psmi_envvar_val) 1)
#define PSMI_ENVVAR_VAL_NO  ((union psmi_envvar_val) 0)

int
MOCKABLE(psm3_getenv)(const char *name, const char *descr, int level,
		int type, union psmi_envvar_val defval,
		union psmi_envvar_val *newval);
MOCK_DCL_EPILOGUE(psm3_getenv);
int psm3_parse_val_pattern(const char *env, int def, int def_syntax);
/*
 * Misc functionality
 */
long int psm3_rand(long int seed);
uintptr_t psm3_getpagesize(void);
uint64_t psm3_cycles_left(uint64_t start_cycles, int64_t timeout_ns);
void psm3_syslog(psm2_ep_t ep, int to_console, int level,
		 const char *format, ...);
void *psm3_memcpyo(void *dst, const void *src, size_t n);
uint32_t psm3_crc(unsigned char *buf, int len);

/*
 * Internal CPUID detection
 */
#define CPUID_FAMILY_MASK       0x00000f00
#define CPUID_MODEL_MASK        0x000000f0
#define CPUID_EXMODEL_MASK      0x000f0000

/*
 * CPUID return values
 */
#define CPUID_FAMILY_XEON       0x00000600
#define CPUID_MODEL_PHI_GEN2    87
#define CPUID_MODEL_PHI_GEN2M   133
/*
 * cpuid function 0, returns "GeniuneIntel" in EBX,ECX,EDX
 * due to Little Endian and Hex it is not so obvious
 */
#define CPUID_GENUINE_INTEL_EBX 0x756e6547 /* "uneG" - Little Endian "Genu" */
#define CPUID_GENUINE_INTEL_ECX 0x6c65746e /* "Ieni" - Little Endian "ineI" */
#define CPUID_GENUINE_INTEL_EDX 0x49656e69 /* "letn" - Little Endian "ntel" */

/*
 * These values are internal only, not real register values
 */
#define CPUID_GENUINE_INTEL     0xf0000000
#define CPUID_MODEL_UNDEFINED   -1

/*
 * Global model so we can tune defaults better for specific cpu's
 */
extern uint32_t psm3_cpu_model;

/*
 * Diagnostics, all in psm_diags.c
 */
int psm3_diags(void);

/*
 * Multiple Endpoints
 */
extern int psm3_multi_ep_enabled;
void psm3_multi_ep_init();

#ifdef PSM_FI
/*
 * Fault injection
 * Controlled by:
 *	PSM3_FI=0/1 - enable
 *	PSM3_FI_TRACEFILE - where to put summary stats at end of run
 *		"stdout", "stderr", of prefix for per process filename
 *	PSM3_FI_VERBOSE - output to std when generate fault
 *	PSM3_FI_RAIL - only generate for secondary EPs/Rails/QPs
 *	PSM3_FI_X - for each fault type: num:denom:seed
 *			fault num/denom of events, seed random for reproducing
 *		recvlost - discard packet on receive before processing
 *		recvpart - partial packet receive in TCP
 *		rq_lkey - RQ WQE with bad lkey
 *		rc_rdma_lkey - User RC SQ WQE with bad lkey
 *		rc_rdma_rkey - User RC SQ WQE with bad rkey
 *		rv_rdma_len - RV SQ WQE with bad len
 *		rv_rdma_rkey - RV SQ WQE with bad rkey
 *		sq_lkey - SQ WQE with bad lkey
 *		sendlost - discard packet on send before sending
 *		sendpart - partial packet send in TCP
 *		sendfull - report no resources on send (pio_flush)
 *		sendfullctrl - report no resources on send ctrl message
 *		sendfullcb - report no resources ctrl msg retry timer callback
 *		reg_mr - register MR failure (ENOMEM)
 *		nonpri_reg_mr - non-priority register MR failure (ENOMEM)
 *		pri_reg_mr - priority register MR failure (ENOMEM)
 *		gdrmmap - GPU gdrcopy pin and mmap failure
 */
extern int psm3_faultinj_enabled; /* use macro to test */
extern int psm3_faultinj_verbose; /* use IS_FAULT macro to test */
extern int psm3_faultinj_sec_rail;/* faults only on secondary rails or EPs */

struct psm3_faultinj_spec {
	STAILQ_ENTRY(psm3_faultinj_spec) next;
	char spec_name[PSM3_FAULTINJ_SPEC_NAMELEN];

	uint64_t num_faults;
	uint64_t num_calls;

	struct drand48_data drand48_data;
	int num;
	int denom;
	long int initial_seed;
};

#define PSM3_FAULTINJ_ENABLED()	(!!psm3_faultinj_enabled)
#define PSM3_FAULTINJ_ENABLED_EP(ep)	(PSM3_FAULTINJ_ENABLED() \
		&& (!psm3_faultinj_sec_rail || ((ep)->mctxt_master != (ep))))

// callers must use macro, this declaration is only for use by macro
int psm3_faultinj_is_fault(struct psm3_faultinj_spec *fi, psm2_ep_t ep);
#define PSM3_FAULTINJ_IS_FAULT(fi, ep, fmt, ...) \
	(psm3_faultinj_is_fault(fi, ep)? \
			psm3_faultinj_verbose? \
				(printf("%s: injecting fault: %s" fmt "\n", psm3_get_mylabel(), fi->spec_name, ##__VA_ARGS__), fflush(stdout), 1) \
				: 1 \
			: 0)

void psm3_faultinj_init();
void psm3_faultinj_fini();
struct psm3_faultinj_spec *psm3_faultinj_getspec(const char *spec_name,
						 const char *help,
						 int num, int denom);
#define PSM3_FAULTINJ_STATIC_DECL(var, spec_name, help, num, denom)	\
	static struct psm3_faultinj_spec *var;			\
	if_pf(PSM3_FAULTINJ_ENABLED() && (var) == NULL)			\
	    (var) = psm3_faultinj_getspec((spec_name), (help), (num), (denom));

#else
#define PSM3_FAULTINJ_ENABLED()	0
#define PSM3_FAULTINJ_ENABLED_EP(ep)	0
#define PSM3_FAULTINJ_IS_FAULT(fi, ep, fmt, ...) 0
#define PSM3_FAULTINJ_STATIC_DECL(var, spec_name, help, num, denom)
#endif /* #ifdef PSM_FI */
/*
 * PSM core component set/get options
 */
psm2_error_t psm3_core_setopt(const void *core_obj, int optname,
			     const void *optval, uint64_t optlen);

psm2_error_t psm3_core_getopt(const void *core_obj, int optname,
			     void *optval, uint64_t *optlen);

/*
 * PSM AM component set/get options
 */
psm2_error_t psm3_am_setopt(const void *am_obj, int optname,
			   const void *optval, uint64_t optlen);

psm2_error_t psm3_am_getopt(const void *am_obj, int optname,
			   void *optval, uint64_t *optlen);

/* touch the pages, with a 32 bit read */
void psm3_touch_mmap(void *m, size_t bytes);
#endif /* _PSMI_UTILS_H */
