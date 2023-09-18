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

#include <netdb.h>		/* gethostbyname */
#include <malloc.h>             /* malloc_usable_size */
#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_am_internal.h"
#include "psm_mq_internal.h"
#include "ips_proto_params.h"
#include <net/if.h>  // for IFF_LOOPBACK
#include <ifaddrs.h>  // for getifaddrs
#include <netinet/in.h>  // for sockaddr
#include <fnmatch.h>
#include <dlfcn.h>
#include "psm_revision.h"
#ifdef PSM_HAVE_RNDV_MOD
#include "psm_rndv_mod.h"
#endif

/*
 * EPIDs encode the basic information needed to establish
 * datagram traffic so that PSM connection establishment can
 * negotiate and exchange the rest.
 * EPID includes: EPID format, network address, queue ID within NIC
 *
 * these are internal only routines to construct epid's and extract individual
 * fields.  There are multiple EPID formats each with different sets of fields.
 * the epid internals are opaque to PSM API consumers and to most of PSM itself
 *
 * A valid EPID must be non-zero.  A few places in PSM3 make use of
 * psm3_epid_zero to identify uninitialized epid's in tables (such as ashm)
 * and use psm3_epid_zeroed_internal() to create an empty epid (equivalent to calloc or
 * memset to 0 of an epid).
 */

// values for shm_only field in epid
#define PSMI_EPID_IPS_SHM				0 /* ips job */
#define PSMI_EPID_SHM_ONLY				1 /* shm only job */

// values for protocol field in epid.  Note this is NOT PSMI_ETH_PROTO_*
#define PSMI_EPID_ETH_PROTO_ROCE 0
#define PSMI_EPID_ETH_PROTO_INET 1	// UDP or TCP distinguished by aux_sock

// This is cast to and from psm2_epid_t (via the psm2_epid field)
// so that psm2_epid_t contents can remain opaque to psm2 API callers
// who will not see this more detailed psmi_epid_t but will just see psm2_epid_t
// A psm2_nid_t also uses this format, but has 0 in the protocol and process
// specific fields (protocol, context, subcontext, qpn, pri_sock, aux_sock).
typedef union {
	psm2_epid_t psm2_epid;	// to cast to/from psm2_epid_t
	uint64_t w[3];	// word by word access
	struct {	// for generic access to format in any epid style
		uint32_t addr_fmt:3;
		uint32_t rest:29;
	};
	struct {	// shm and self when IPS device disabled
		uint32_t addr_fmt:3;	// = PSMI_ADDR_FMT_SHM
		// TBD don't need shm_only field anymore, EPID will be non-zero
		// due to pid always being supplied
		uint32_t shm_only:1;	// = PSMI_EPID_SHM_ONLY
		uint32_t reserved:28;
		uint32_t pid:32;		// always our pid (self and shm EPID)
		uint64_t gid_hi;		// IP of 1st NIC in IPv6 format
		uint64_t gid_lo;		// IP of 1st NIC in IPv6 format
	} shm;
	// addr_fmt==1 invalid
	// addr_fmt==2 invalid
	struct {	// Verbs on IB or OPA
		uint32_t addr_fmt:3;	// = PSMI_ADDR_FMT_IB
		uint32_t reserved1:5;
		uint32_t qpn:24;
		uint32_t lid:16;	// limited to 16b LID on OPA
		uint32_t reserved2:16;
		uint64_t gid_hi;	// subnet_prefix
		uint64_t gid_lo;	// interface_id
	} v3;
	struct {	// Verbs on IPv4 Ethernet (ROCEv2) or on sockets
		uint32_t addr_fmt:3;	// = PSMI_ADDR_FMT_IPV4
		uint32_t protocol:1;	// PSMI_EPID_ETH_PROTO_ (ROCE or INET)
		uint32_t prefix_len:5; 	// num high bits in IP addr representing IP subnet minus 1
		uint32_t reserved1:23;
		uint32_t ipv4_addr:32;
		union {
			struct { // PSMI_EPID_ETH_PROTO_ROCE
				uint32_t qpn:24;	// UD QPN
				uint32_t reserved:8;
			} roce;
			struct { // PSMI_EPID_ETH_PROTO_INET
				uint16_t pri_sock;	// primary socket
				uint16_t aux_sock;	// UDP socket when TCP
			} sockets;
			uint32_t context;	// for easy zeroing
		};
		uint32_t reserved2;
		uint64_t reserved3;
	} v4;
	struct {	// Verbs on IPv6 Ethernet (ROCEv2) or on sockets
		uint32_t addr_fmt:3;	// = PSMI_ADDR_FMT_IPV6
		uint32_t protocol:1;	// PSMI_EPID_ETH_PROTO_ (ROCE or INET)
		uint32_t prefix_len:7;	// IPv6 subnet prefix_len minus 1
		uint32_t reserved:21;	// TBD - flowinfo (20 bits)?
		union {
			struct { // PSMI_EPID_ETH_PROTO_ROCE
				uint32_t qpn:24;	// UD QPN
				uint32_t reserved:8;
			} roce;
			struct { // PSMI_EPID_ETH_PROTO_INET
				uint16_t pri_sock;	// primary socket
				uint16_t aux_sock;	// UDP socket when TCP
			} sockets;
			uint32_t context;	// for easy zeroing
		};
		uint64_t gid_hi;	// subnet_prefix
		uint64_t gid_lo;	// interface_id
	} v6;
} psmi_epid_t;

int psm3_ep_device_is_enabled(const psm2_ep_t ep, int devid);

// we keep ntop style functions static and provide easier to use _fmt_* routines
static const char *psmi_sockaddr_ntop(struct sockaddr* addr, char *dst,
					socklen_t size);
static const char *psmi_ipv4_ntop(uint32_t ip_addr, uint8_t prefix_len,
					char *dst, socklen_t size);
static const char *psmi_ipv6_ntop(psmi_bare_netaddr128_t ipv6_addr,
					uint8_t prefix_len, char *dst,
					socklen_t size);

struct psm3_epid_table psm3_epid_table;

/* Iterator to access the epid table.
 * 'ep' can be NULL if remote endpoints from all endpoint handles are requested
 * but in this case, both hostnames and epaddr's can be returned as all entries
 * are walked.
 */
void psm3_epid_itor_init(struct psmi_eptab_iterator *itor, psm2_ep_t ep)
{
	itor->i = 0;
	itor->ep = ep;
	pthread_mutex_lock(&psm3_epid_table.tablock);
}

void *psm3_epid_itor_next(struct psmi_eptab_iterator *itor)
{
	int i;
	struct psm3_epid_tabentry *e;

	if (itor->i >= psm3_epid_table.tabsize)
		return NULL;
	for (i = itor->i; i < psm3_epid_table.tabsize; i++) {
		e = &psm3_epid_table.table[i];
		if (!e->entry || e->entry == EPADDR_DELETED)
			continue;
		if (itor->ep && e->ep != itor->ep)
			continue;
		itor->i = i + 1;
		return e->entry;
	}
	itor->i = psm3_epid_table.tabsize;	/* put at end of table */
	return NULL;
}

void psm3_epid_itor_fini(struct psmi_eptab_iterator *itor)
{
	pthread_mutex_unlock(&psm3_epid_table.tablock);
	itor->i = 0;
}

#define mix64(a, b, c) \
{ \
	a -= b; a -= c; a ^= (c>>43); \
	b -= c; b -= a; b ^= (a<<9);  \
	c -= a; c -= b; c ^= (b>>8);  \
	a -= b; a -= c; a ^= (c>>38); \
	b -= c; b -= a; b ^= (a<<23); \
	c -= a; c -= b; c ^= (b>>5);  \
	a -= b; a -= c; a ^= (c>>35); \
	b -= c; b -= a; b ^= (a<<49); \
	c -= a; c -= b; c ^= (b>>11); \
	a -= b; a -= c; a ^= (c>>12); \
	b -= c; b -= a; b ^= (a<<18); \
	c -= a; c -= b; c ^= (b>>22); \
}

// initialize the global epid table and related locks
// The table allows mapping from local ep and a epid to the epaddr or
// from a epid to the remote hostname
// The table's use should be only during connection establishment and
// some diagnostic/debug messages as the searches are linear
psm2_error_t psm3_epid_init()
{
	pthread_mutexattr_t attr;
	psm3_epid_table.table = NULL, psm3_epid_table.tabsize = 0;
	psm3_epid_table.tabsize_used = 0;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&psm3_epid_table.tablock, &attr);
	pthread_mutexattr_destroy(&attr);
	return PSM2_OK;
};

psm2_error_t psm3_epid_fini()
{
	if (psm3_epid_table.table != NULL) {
		psmi_free(psm3_epid_table.table);
		psm3_epid_table.table = NULL;
	}
	psm3_epid_table.tabsize = 0;
	psm3_epid_table.tabsize_used = 0;
	return PSM2_OK;
}

PSMI_ALWAYS_INLINE(
uint64_t
hash_this(const psm2_ep_t ep, const psm2_epid_t epid))
{
	uint64_t ep_i = (uint64_t) (uintptr_t) ep;
	uint64_t epid_i = psm3_epid_hash(epid);
	uint64_t hash = 0x9e3779b97f4a7c13LL;
	mix64(ep_i, epid_i, hash);
	return hash;
}

PSMI_ALWAYS_INLINE(
void *
psm3_epid_lookup_inner(psm2_ep_t ep, psm2_epid_t epid, int remove))
{
	uint64_t key = hash_this(ep, epid);
	struct psm3_epid_tabentry *e;
	void *entry = NULL;
	int idx;

	pthread_mutex_lock(&psm3_epid_table.tablock);
	if (!psm3_epid_table.table)
		goto ret;
	idx = (int)(key % psm3_epid_table.tabsize);
	while (psm3_epid_table.table[idx].entry != NULL) {
		/* An epid can be added twice if there's more than one opened endpoint,
		 * but really we match on epid *and* on endpoint */
		e = &psm3_epid_table.table[idx];
		if (e->entry != EPADDR_DELETED && e->key == key
			&& e->ep == ep && !psm3_epid_cmp_internal(e->epid, epid)) {
			entry = e->entry;
			if (remove)
				psm3_epid_table.table[idx].entry =
				    EPADDR_DELETED;
			goto ret;
		}
		if (++idx == psm3_epid_table.tabsize)
			idx = 0;
	}
ret:
	pthread_mutex_unlock(&psm3_epid_table.tablock);
	return entry;
}

void *psm3_epid_lookup(psm2_ep_t ep, psm2_epid_t epid)
{
	void *entry = psm3_epid_lookup_inner(ep, epid, 0);
	if_pf (_HFI_VDBG_ON && PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("lookup of (%p,%s) returns %p\n", ep, psm3_epid_fmt_internal(epid, 0),
			  entry);
	return entry;
}

void *psm3_epid_remove(psm2_ep_t ep, psm2_epid_t epid)
{
	if_pf (_HFI_VDBG_ON && PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("remove of (%p,%s)\n", ep, psm3_epid_fmt_internal(epid, 0));
	return psm3_epid_lookup_inner(ep, epid, 1);
}

void psm3_epid_remove_all(psm2_ep_t ep)
{
	size_t i;
	struct psm3_epid_tabentry *e;

	pthread_mutex_lock(&psm3_epid_table.tablock);

	for (i = 0; i < psm3_epid_table.tabsize; i++) {
		e = &psm3_epid_table.table[i];

		if (e->entry == NULL || e->entry == EPADDR_DELETED)
			continue;

		if (e->ep == ep) {
			/* unspecified fields implicitly zeroed */
			*e = (struct psm3_epid_tabentry) {
				.entry = EPADDR_DELETED
			};
		}
	}

	pthread_mutex_unlock(&psm3_epid_table.tablock);
}

psm2_error_t psm3_epid_add(psm2_ep_t ep, psm2_epid_t epid, void *entry)
{
	uint64_t key;
	int idx, i, newsz;
	struct psm3_epid_tabentry *e;
	psm2_error_t err = PSM2_OK;

	if_pf (_HFI_VDBG_ON && PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("add of (%p,%s) with entry %p\n", ep, psm3_epid_fmt_internal(epid, 0),
			  entry);
	pthread_mutex_lock(&psm3_epid_table.tablock);
	/* Leave this here, mostly for sanity and for the fact that the epid
	 * table is currently not used in the critical path */
	if (++psm3_epid_table.tabsize_used >
	    (int)(psm3_epid_table.tabsize * PSMI_EPID_TABLOAD_FACTOR)) {
		struct psm3_epid_tabentry *newtab;
		newsz = psm3_epid_table.tabsize + PSMI_EPID_TABSIZE_CHUNK;
		newtab = (struct psm3_epid_tabentry *)
		    psmi_calloc(ep, PER_PEER_ENDPOINT,
				newsz, sizeof(struct psm3_epid_tabentry));
		if (newtab == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		if (psm3_epid_table.table) {	/* rehash the table */
			for (i = 0; i < psm3_epid_table.tabsize; i++) {
				e = &psm3_epid_table.table[i];
				if (e->entry == NULL)
					continue;
				/* When rehashing, mark deleted as free again */
				if (e->entry == EPADDR_DELETED) {
					psm3_epid_table.tabsize_used--;
					continue;
				}
				idx = (int)(e->key % newsz);
				while (newtab[idx].entry != NULL)
					if (++idx == newsz)
						idx = 0;
				newtab[idx].entry = e->entry;
				newtab[idx].key = e->key;
				newtab[idx].ep = e->ep;
				newtab[idx].epid = e->epid;
			}
			psmi_free(psm3_epid_table.table);
		}
		psm3_epid_table.table = newtab;
		psm3_epid_table.tabsize = newsz;
	}
	key = hash_this(ep, epid);
	idx = (int)(key % psm3_epid_table.tabsize);
	e = &psm3_epid_table.table[idx];
	while (e->entry && e->entry != EPADDR_DELETED) {
		if (++idx == psm3_epid_table.tabsize)
			idx = 0;
		e = &psm3_epid_table.table[idx];
	}
	e->entry = entry;
	e->key = key;
	e->epid = epid;
	e->ep = ep;

fail:
	pthread_mutex_unlock(&psm3_epid_table.tablock);
	return err;
}

static psmi_lock_t psmi_gethostname_lock;

static void __attribute__ ((constructor)) __psmi_gethostname_lock_constructor(void)
{
	psmi_init_lock(&psmi_gethostname_lock);
}

char *psm3_gethostname(void)
{
	static char hostname[80] = { '\0' };
	char *c;

	if (hostname[0] == '\0') {
		PSMI_LOCK(psmi_gethostname_lock);
		/* CRITICAL SECTION START */
		if (hostname[0] == '\0') {
			gethostname(hostname, sizeof(hostname));
			hostname[sizeof(hostname) - 1] = '\0';	/* no guarantee of nul termination */
			if ((c = strchr(hostname, '.')))
				*c = '\0';
		}
		PSMI_UNLOCK(psmi_gethostname_lock);
		/* CRITICAL SECTION END */
	}

	return hostname;
}

/*
 * Hostname stuff.  We really only register the network portion of the epid
 * since all epids from the same nid are assumed to have the same hostname.
 * We add entries to the epid_table with a special ep (PSMI_EP_HOSTNAME) and
 * use the nid as the epid.  As such we have only 1 such entry per host.
 */
psm2_error_t
psm3_epid_set_hostname(psm2_nid_t nid, const char *hostname, int overwrite)
{
	size_t hlen;
	char *h;
	psm2_error_t err = PSM2_OK;
	// use PSMI_EP_HOSTNAME as ep to avoid collisions with real ep entries

	if (hostname == NULL)
		return PSM2_OK;
	/* First see if a hostname already exists */
	if ((h = psm3_epid_lookup(PSMI_EP_HOSTNAME, nid)) != NULL) {
		if (!overwrite)
			return PSM2_OK;

		h = psm3_epid_remove(PSMI_EP_HOSTNAME, nid);
		if (h != NULL)	/* free the previous hostname if so exists */
			psmi_free(h);
	}

	hlen = min(PSMI_EP_HOSTNAME_LEN, strlen(hostname) + 1);
	h = (char *)psmi_malloc(PSMI_EP_NONE, PER_PEER_ENDPOINT, hlen);
	if (h == NULL)
		return PSM2_NO_MEMORY;
	snprintf(h, hlen, "%s", hostname);
	err = psm3_epid_add(PSMI_EP_HOSTNAME, nid, h);
	return err;
}

// This converts a prefix_len back into a 32 bit IPv4 netmask
static inline uint32_t psmi_prefix_len_to_ipv4_netmask(int count)
{
	return count ? (uint32_t)(0xffffffff << (32-count)) : 0;
}

// This converts a prefix_len back into a 128 bit IPv6 netmask
static inline psmi_bare_netaddr128_t psmi_prefix_len_to_ipv6_netmask(int count)
{
	psmi_bare_netaddr128_t ret;

	if (count <= 64) {
		ret.hi = (uint64_t)(0xffffffffffffffffULL << (64-count));
		ret.lo = 0;
	} else {
		ret.hi = (uint64_t)0xffffffffffffffffULL;
		ret.lo = (uint64_t)(0xffffffffffffffffULL << (128-count));
	}
	return ret;
}

/* These functions build the local epid */
/* This is a typical multi-node job */
	// IB or OPA with Verbs
psm2_epid_t psm3_epid_pack_ib(uint16_t lid, uint32_t qp_num,
				psmi_naddr128_t addr)
{
	psmi_epid_t epid;

	psmi_assert(addr.fmt == PSMI_ADDR_FMT_IB);
	psmi_assert(addr.prefix_len == 64);
	epid.v3.addr_fmt = PSMI_ADDR_FMT_IB;
	epid.v3.reserved1 = 0;
	epid.v3.qpn = qp_num;
	epid.v3.lid = lid;
	epid.v3.reserved2 = 0;
	epid.v3.gid_hi = addr.bare.hi;
	epid.v3.gid_lo = addr.bare.lo;
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	psmi_assert(psm3_epid_addr_fmt(epid.psm2_epid) == PSMI_ADDR_FMT_IB);
	return epid.psm2_epid;
}

// IPv4 Ethernet (RoCE or UDP/TCP)
// for RoCE context is QPN (24b), for UDP/TCP it's primary socket number (16b)
psm2_epid_t psm3_epid_pack_ipv4(psmi_naddr128_t ipv4_addr,
				psmi_eth_proto_t protocol,
				uint32_t context, uint16_t aux_sock)
{
	psmi_epid_t epid;

	psmi_assert(ipv4_addr.fmt == PSMI_ADDR_FMT_IPV4);
	epid.v4.addr_fmt = PSMI_ADDR_FMT_IPV4;
	// IPv4 prefix len of 0 is not valid, we save a bit by storing len-1
	psmi_assert(ipv4_addr.prefix_len >= 1 && ipv4_addr.prefix_len <= 32);
	epid.v4.prefix_len = ipv4_addr.prefix_len - 1;
	epid.v4.reserved1 = 0;
	epid.v4.ipv4_addr = ipv4_addr.bare.lo;
	switch (protocol) {
	case PSMI_ETH_PROTO_ROCE:
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_ROCE;
		epid.v4.roce.qpn = context;
		epid.v4.roce.reserved = 0;
		psmi_assert(aux_sock == 0);
		break;
	case PSMI_ETH_PROTO_UDP:
		psmi_assert(context <= UINT16_MAX);
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_INET;
		epid.v4.sockets.pri_sock = context;
		epid.v4.sockets.aux_sock = 0;
		psmi_assert(aux_sock == 0);
		break;
	case PSMI_ETH_PROTO_TCP:
		psmi_assert(context <= UINT16_MAX);
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_INET;
		epid.v4.sockets.pri_sock = context;
		epid.v4.sockets.aux_sock = aux_sock;
		psmi_assert(aux_sock != 0);
		break;
	default:
		psmi_assert_always(0);	// unexpected protocol
		break;
	}
	epid.v4.reserved2 = 0;
	epid.v4.reserved3 = 0;
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	psmi_assert(psm3_epid_addr_fmt(epid.psm2_epid) == PSMI_ADDR_FMT_IPV4);
	psmi_assert(psm3_epid_protocol(epid.psm2_epid) == protocol);
	return epid.psm2_epid;
}

// IPv6 Ethernet (RoCE or UDP/TCP)
// for RoCE context is QPN (24b), for UDP/TCP it's primary socket number (16b)
psm2_epid_t psm3_epid_pack_ipv6(psmi_naddr128_t ipv6_addr,
				psmi_eth_proto_t protocol,
				uint32_t context, uint16_t aux_sock)
{
	psmi_epid_t epid;

	psmi_assert(ipv6_addr.fmt == PSMI_ADDR_FMT_IPV6);
	// IPv6 prefix len of 0 is not valid, we save a bit by storing len-1
	psmi_assert(ipv6_addr.prefix_len >= 1 && ipv6_addr.prefix_len <= 128);
	epid.v6.addr_fmt = PSMI_ADDR_FMT_IPV6;
	epid.v6.prefix_len = ipv6_addr.prefix_len - 1;
	epid.v6.reserved = 0;
	switch (protocol) {
	case PSMI_ETH_PROTO_ROCE:
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_ROCE;
		epid.v6.roce.qpn = context;
		epid.v6.roce.reserved = 0;
		psmi_assert(aux_sock == 0);
		break;
	case PSMI_ETH_PROTO_UDP:
		psmi_assert(context <= UINT16_MAX);
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_INET;
		epid.v6.sockets.pri_sock = context;
		epid.v6.sockets.aux_sock = 0;
		psmi_assert(aux_sock == 0);
		break;
	case PSMI_ETH_PROTO_TCP:
		psmi_assert(context <= UINT16_MAX);
		epid.v4.protocol = PSMI_EPID_ETH_PROTO_INET;
		epid.v6.sockets.pri_sock = context;
		epid.v6.sockets.aux_sock = aux_sock;
		psmi_assert(aux_sock != 0);
		break;
	default:
		psmi_assert_always(0);	// unexpected protocol
		break;
	}
	epid.v6.gid_hi = ipv6_addr.bare.hi;
	epid.v6.gid_lo = ipv6_addr.bare.lo;
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	psmi_assert(psm3_epid_addr_fmt(epid.psm2_epid) == PSMI_ADDR_FMT_IPV6);
	psmi_assert(psm3_epid_protocol(epid.psm2_epid) == protocol);
	return epid.psm2_epid;
}

// find the 1st IPv4 or IPv6 address (excluding loopback) in the node
// we will use this as the NID for a FMT_SHM EPID so we can detect incorrect
// attempts to run a multi-node job across shm (or self)
// if we can't find an IP address report 0 and typically pid differences will
// be sufficient to detect multi-node jobs
static void psmi_get_shm_nid(uint64_t *gid_hi, uint64_t *gid_lo)
{
	struct ifaddrs *ifap, *ifa;

	*gid_hi = *gid_lo = 0;
	if (getifaddrs(&ifap) == 0) {
		for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
			// want 1st real interface
			if (ifa->ifa_flags & (IFF_LOOPBACK|IFF_POINTOPOINT))
				continue;
			if (! ifa->ifa_netmask || ! ifa->ifa_addr)
				 continue;
			if (ifa->ifa_addr->sa_family == AF_INET) {
				struct sockaddr_in *addr = (struct sockaddr_in  *)ifa->ifa_addr;
				uint32_t ip_addr;

				ip_addr = __be32_to_cpu(addr->sin_addr.s_addr);
				*gid_hi = PSMI_IPV4_GID_HI(ip_addr);
				*gid_lo = PSMI_IPV4_GID_LO(ip_addr);
				_HFI_DBG("Using ifaddr[%s]: %s as NID\n",
						ifa->ifa_name, psm3_ipv4_fmt(ip_addr, 0, 0));
				break;
			}
			if (ifa->ifa_addr->sa_family == AF_INET6) {
				struct sockaddr_in6 *addr = (struct sockaddr_in6  *)ifa->ifa_addr;
				uint32_t *s6;
				psmi_bare_netaddr128_t bare;
				char buf[INET6_ADDRSTRLEN+4];

				s6 = addr->sin6_addr.s6_addr32;
				bare.hi = *gid_hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
				bare.lo = *gid_lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);
				_HFI_DBG("Using ifaddr[%s]: %s as NID\n",
						ifa->ifa_name, 
						psmi_ipv6_ntop(bare, 0, buf, sizeof(buf)));
				break;
			}
		}
		(void)freeifaddrs(ifap);
	}
	return;
}

/* This is a shm-only epid (single node job) */
psm2_epid_t psm3_epid_pack_shm(const psm2_uuid_t unique_job_key)
{
	psmi_epid_t epid;
// TBD - possible duplicate epid for shm-only job with multi-ep
// but probably not an issue since we don't cross connect shm ep's?
	epid.shm.addr_fmt = PSMI_ADDR_FMT_SHM;
	epid.shm.shm_only = PSMI_EPID_SHM_ONLY;
	epid.shm.reserved = 0;
	epid.shm.pid = getpid();
	psmi_get_shm_nid(&epid.shm.gid_hi, &epid.shm.gid_lo);
	psmi_assert(psm3_epid_addr_fmt(epid.psm2_epid) == PSMI_ADDR_FMT_SHM);
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	return epid.psm2_epid;
}

/* This is a self-only epid (single process job) */
psm2_epid_t psm3_epid_pack_self(void)
{
	psmi_epid_t epid;
// TBD - possible duplicate epid for self-only job with multi-ep
// but probably not an issue since we don't cross connect self ep's?
	epid.shm.addr_fmt = PSMI_ADDR_FMT_SHM;
	epid.shm.shm_only = PSMI_EPID_SHM_ONLY;
	epid.shm.reserved = 0;
	epid.shm.pid = getpid();
	psmi_get_shm_nid(&epid.shm.gid_hi, &epid.shm.gid_lo);
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	return epid.psm2_epid;
}

/*  for internal self-test of epid/epaddr hash and lookup */
psm2_epid_t psm3_epid_pack_diag(int val)
{
	// just need a valid epid which is different for each val given
	psmi_epid_t epid;
	epid.shm.addr_fmt = PSMI_ADDR_FMT_SHM;
	epid.shm.shm_only = PSMI_EPID_SHM_ONLY;
	epid.shm.reserved = 0;
	epid.shm.pid = val;
	psmi_get_shm_nid(&epid.shm.gid_hi, &epid.shm.gid_lo);
	psmi_assert(sizeof(psm2_epid_t) == sizeof(psmi_epid_t));
	return epid.psm2_epid;
}

uint8_t psm3_epid_addr_fmt(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };

	return e.addr_fmt;
}

psmi_eth_proto_t psm3_epid_protocol(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IPV4:
		if (e.v4.protocol == PSMI_EPID_ETH_PROTO_ROCE)
			return PSMI_ETH_PROTO_ROCE;
		else if (e.v4.sockets.aux_sock)
			return PSMI_ETH_PROTO_TCP;
		else
			return PSMI_ETH_PROTO_UDP;
		break;
	case PSMI_ADDR_FMT_IPV6:
		if (e.v6.protocol == PSMI_EPID_ETH_PROTO_ROCE)
			return PSMI_ETH_PROTO_ROCE;
		else if (e.v6.sockets.aux_sock)
			return PSMI_ETH_PROTO_TCP;
		else
			return PSMI_ETH_PROTO_UDP;
		break;
	default:
		return PSMI_ETH_PROTO_NA;	// N/A
		break;
	}
}

// The network id (address) from the epid
// depending on epid addr_fmt this may be a lid/subnet or ipv4 address
// or ipv6 addr.
// For shm-only or self-only jobs, we also normalize shm formats.
// The NID will be globally unique within the job and non-zero
// (addr_fmt != 0 for all but SHM.  IPATH SHM we set reserved1, OPA SHM
// and IB SHM have shm_only != 0)
// We clear protocol to be consistent with psm3_build_nid
psm2_nid_t psm3_epid_nid(psm2_epid_t epid)
{
	psmi_epid_t ret = { .psm2_epid = epid };
	switch (ret.addr_fmt) {
	case PSMI_ADDR_FMT_SHM:
		ret.shm.pid = 0;
		break;
	case PSMI_ADDR_FMT_IB:
		ret.v3.qpn = 0;
		break;
	case PSMI_ADDR_FMT_IPV4:
		ret.v4.protocol = 0;
		ret.v4.context = 0;
		break;
	case PSMI_ADDR_FMT_IPV6:
		ret.v6.protocol = 0;
		ret.v6.context = 0;
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
	return ret.psm2_epid;
}

// extract portion of ep->subnet (eg. a subnet from get_port_subnet)
// which can be compared to psm3_epid_subnet().
// This accounts for the fact that some epid addr_fmt's (IB/OPA) only contain a
// subset of the subnet bits.
// Only valid for subnet used in remote IPS connections
static psmi_subnet128_t psmi_subnet_epid_subset(psmi_subnet128_t subnet)
{
	return subnet;
}

// Get the subnet for the given EPID.
// Only valid for epid's used in remote IPS connections
// Can be compared to psmi_subnet_epid_subset()
psmi_subnet128_t psm3_epid_subnet(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	psmi_subnet128_t ret = { };
	psmi_bare_netaddr128_t nm;
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM: // only called for remote IPS connections
		psmi_assert_always(0);	// unexpected addr_fmt
		return ret; // keep compiler happy, never reached
		break;
	case PSMI_ADDR_FMT_IB:
		ret.bare.hi = e.v3.gid_hi;
		ret.bare.lo = 0;
		ret.fmt = PSMI_ADDR_FMT_IB;
		ret.prefix_len = 64;
		return ret;
		break;
	case PSMI_ADDR_FMT_IPV4:
		ret.bare.hi = 0;
		ret.bare.lo = psmi_prefix_len_to_ipv4_netmask(e.v4.prefix_len + 1) & e.v4.ipv4_addr;
		ret.fmt = PSMI_ADDR_FMT_IPV4;
		ret.prefix_len = e.v4.prefix_len + 1;
		return ret;
		break;
	case PSMI_ADDR_FMT_IPV6:
		nm = psmi_prefix_len_to_ipv6_netmask(e.v6.prefix_len + 1);
		ret.bare.hi = e.v6.gid_hi & nm.hi;
		ret.bare.lo = e.v6.gid_lo & nm.lo;
		ret.fmt = PSMI_ADDR_FMT_IPV6;
		ret.prefix_len = e.v6.prefix_len + 1;
		return ret;
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return ret; // keep compiler happy, never reached
		break;
	}
}

// Get the subnet prefix_len for the given EPID.
// Only valid for epid's used in remote IPS connections
uint8_t psm3_epid_prefix_len(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM: // only called for remote IPS connections
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	case PSMI_ADDR_FMT_IB:
		return 64;
		break;
	case PSMI_ADDR_FMT_IPV4:
		return e.v4.prefix_len + 1;
		break;
	case PSMI_ADDR_FMT_IPV6:
		return e.v6.prefix_len + 1;
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	}
}

// The locally unique identifiers for the HW resources
// OPA Native - Context (also need sub-context)
// Verbs - 24b QPN (IB, OPA and RoCE Verbs)
// Sockets - 16b primary socket number (sin_port) (UDP/TCP)
// This should not be called for psm2_nid_t
uint64_t psm3_epid_context(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM: // can be called by psm3_epid_fmt_addr
		return e.shm.pid;
		break;
	case PSMI_ADDR_FMT_IB:
		return e.v3.qpn;
		break;
	case PSMI_ADDR_FMT_IPV4:
		switch (e.v4.protocol) {
		case PSMI_EPID_ETH_PROTO_ROCE:
			psmi_assert(e.v4.roce.qpn != 0);
			return e.v4.roce.qpn;
		case PSMI_EPID_ETH_PROTO_INET:
			psmi_assert(e.v4.sockets.pri_sock != 0);
			return e.v4.sockets.pri_sock;
		default:
			// might be called in psm3_handle_error, so output something
			// instead of asserting
			return 0;
		}
		break;
	case PSMI_ADDR_FMT_IPV6:
		switch (e.v6.protocol) {
		case PSMI_EPID_ETH_PROTO_ROCE:
			psmi_assert(e.v6.roce.qpn != 0);
			return e.v6.roce.qpn;
		case PSMI_EPID_ETH_PROTO_INET:
			psmi_assert(e.v6.sockets.pri_sock != 0);
			return e.v6.sockets.pri_sock;
		default:
			// might be called in psm3_handle_error, so output something
			// instead of asserting
			return 0;
		}
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	}
}

#ifdef PSM_SOCKETS
// The 16b auxilary UDP socket for a protocol=TCP EPID
// not valid for other protocols
uint16_t psm3_epid_aux_socket(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM: // can be called by psm3_epid_fmt_addr
	case PSMI_ADDR_FMT_IB:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0;
		break;
	case PSMI_ADDR_FMT_IPV4:
		switch (e.v4.protocol) {
		case PSMI_EPID_ETH_PROTO_INET:
			psmi_assert(e.v4.sockets.aux_sock != 0);
			return e.v4.sockets.aux_sock;
		case PSMI_EPID_ETH_PROTO_ROCE:
		default:
			// might be called in psm3_handle_error, so output something
			// instead of asserting
			return 0;
		}
		break;
	case PSMI_ADDR_FMT_IPV6:
		switch (e.v6.protocol) {
		case PSMI_EPID_ETH_PROTO_INET:
			psmi_assert(e.v6.sockets.aux_sock != 0);
			return e.v6.sockets.aux_sock;
		case PSMI_EPID_ETH_PROTO_ROCE:
		default:
			// might be called in psm3_handle_error, so output something
			// instead of asserting
			return 0;
		}
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	}
}
#endif /* PSM_SOCKETS */


// return appropriate LID to use
// for Ethernet, 1 is returned but not used beyond checkng LID != 0
uint16_t psm3_epid_lid(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IB:
		return e.v3.lid;
		break;
	case PSMI_ADDR_FMT_IPV4:
		return 1;	// not really used, but lid must be != 0
		break;
	case PSMI_ADDR_FMT_IPV6:
		return 1;	// not really used, but lid must be != 0
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	}
}

// get information needed to build a verbs AV
// gid returned in format suitable to build an IPv6 GID for AV
// lid and gid in host byte order
void psm3_epid_get_av(psm2_epid_t epid, uint16_t *lid, psmi_gid128_t *gid)
{
	psmi_epid_t e = { .psm2_epid = epid };

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IB:
		*lid = e.v3.lid;
			// we return 0 here because we want LID based routing
			// instead of GRH based routing in caller's AV
		gid->hi = 0;
		gid->lo = 0;
		break;
	case PSMI_ADDR_FMT_IPV4:
		*lid = 1;	// not used, but dlid must be != 0
		gid->hi = PSMI_IPV4_GID_HI(e.v4.ipv4_addr);
		gid->lo = PSMI_IPV4_GID_LO(e.v4.ipv4_addr);
		psmi_assert(gid->lo != 0 || gid->hi != 0);//IPv4 must be != 0
		break;
	case PSMI_ADDR_FMT_IPV6:
		*lid = 1;	// not used, but dlid must be != 0
		gid->hi = e.v6.gid_hi;
		gid->lo = e.v6.gid_lo;
		psmi_assert(gid->lo != 0 || gid->hi != 0);//IPv6 must be != 0
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
}

// get basic rem_addr as used by RV create_conn to decide listener for non-IPv6
uint32_t psm3_epid_get_rem_addr(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IB:
		psmi_assert(e.v3.lid != 0);
		return e.v3.lid;
		break;
	case PSMI_ADDR_FMT_IPV4:
		psmi_assert(e.v4.ipv4_addr != 0);
		return e.v4.ipv4_addr;
		break;
	case PSMI_ADDR_FMT_IPV6:	// donn't care, but return value anyway
		psmi_assert(e.v6.gid_lo != 0 || e.v6.gid_hi != 0);
		return e.v6.gid_lo;
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return 0; // keep compiler happy, never reached
		break;
	}
}

// return a good portion of epid which can be used for hashing and randomizing
uint64_t psm3_epid_hash(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM:
	case PSMI_ADDR_FMT_IPV4:
	default:
		return epid.w[0];
		break;
	case PSMI_ADDR_FMT_IB:
		return e.v3.gid_lo;
		break;
	case PSMI_ADDR_FMT_IPV6:
		return e.v6.gid_lo;
		break;
	}
}

/*
 * returns 1 if the unit name matches the glob pattern
 * (extended glob pattern where applicable) OR if glob
 * pattern is empty. otherwise returns 0
 */
int psm3_is_nic_allowed(int unit)
{
	if (NULL == psm3_nic_wildcard) {
		_HFI_DBG("PSM3_NIC is not specified. Unit %i is allowed\n", unit);
		return 1;
	}

	const char* dev_name = psm3_sysfs_unit_dev_name(unit);
	if (dev_name && *dev_name &&
		(0 == fnmatch(psm3_nic_wildcard, dev_name, 0
#ifdef FNM_EXTMATCH
		                                           | FNM_EXTMATCH
#endif
	))) {
		_HFI_DBG("Unit %d: '%s' matches PSM3_NIC '%s'\n",
			unit, dev_name, psm3_nic_wildcard);
		return 1;
	}
	_HFI_DBG("Skipping Unit %d: '%s' doesn't match PSM3_NIC '%s'\n",
		unit, dev_name ? dev_name : "Unknown", psm3_nic_wildcard);
	return 0;
}

#define MBPS (1000 * 1000)
/*
 * returns 1 if the speed matches the glob pattern
 * (extended glob pattern where applicable) OR if glob
 * pattern is empty. otherwise returns 0
 * speed is supplied in bits/sec
 * Internally this converts to Mbps prior to string comparison
 */
int psm3_is_speed_allowed(int unit, uint64_t speed)
{
	const char* dev_name = psm3_sysfs_unit_dev_name(unit);
	char speed_str[21];	// 20 digits + \0
	dev_name = dev_name ? dev_name : "Unknown";

	if (NULL == psm3_nic_speed_wildcard || 0 == strcmp(psm3_nic_speed_wildcard, "max")) {
		psmi_assert(psm3_nic_speed_max_found);
		if (speed < psm3_nic_speed_max_found) {
			_HFI_DBG("Skipped Unit %d: '%s' speed %"PRIu64" doesn't match the max speed found %"PRIu64"\n",
				unit, dev_name, speed/MBPS, psm3_nic_speed_max_found/MBPS);
			return 0;
		} else {
			_HFI_DBG("Unit %d: '%s' speed %"PRIu64" matches the max speed found %"PRIu64"\n",
				unit, dev_name, speed/MBPS, psm3_nic_speed_max_found/MBPS);
			return 1;
		}
	}
	if (0 == strcmp(psm3_nic_speed_wildcard, "any")) {
		_HFI_DBG("Unit %d: '%s' speed %"PRIu64" matches the speed of 'any'\n",
			unit, dev_name, speed/MBPS);
		return 1;
	}
	snprintf(speed_str, sizeof(speed_str), "%"PRIu64, speed/MBPS);

	if (0 == fnmatch(psm3_nic_speed_wildcard, speed_str, 0
#ifdef FNM_EXTMATCH
		                                           | FNM_EXTMATCH
#endif
	)) {
		_HFI_DBG("Unit %d: '%s' speed %"PRIu64" matches PSM3_NIC_SPEED '%s'\n",
			unit, dev_name, speed/MBPS, psm3_nic_speed_wildcard);
		return 1;
	}
	_HFI_DBG("Skipping Unit %d: '%s' speed %"PRIu64" doesn't match PSM3_NIC_SPEED '%s'\n",
			unit, dev_name, speed/MBPS, psm3_nic_speed_wildcard);
	return 0;
}
#undef MBPS

static int psm3_allow_subnet(const char *subnet, const char *subnet_type)
{
	int i;
	for (i=0;  i < psm3_num_allow_subnets; i++) {
		int ret_on_match = 1;
		char *pattern = psm3_allow_subnets[i];
		if (*pattern == '^') {
			ret_on_match = 0;
			pattern++;
		}
		if (0 == fnmatch(pattern, subnet, 0
#ifdef FNM_EXTMATCH
| FNM_EXTMATCH
#endif
			)) {
			_HFI_DBG("%s subnet %s matches PSM3_SUBNETS entry %s%s\n",
				subnet_type, subnet, ret_on_match?"":"^",
				pattern);
			return ret_on_match;
		}
	}
	_HFI_DBG("%s subnet %s doesn't match PSM3_SUBNETS\n",
			subnet_type, subnet);
	return 0;
}

// compare an IB subnet against the PSM3_SUBNETS which are allowed
// comparison is done as a string extended global pattern compare
// the 0x%016x format of the subnet is used
// return 1 if allowed, 0 if not match (or unable to compare)
int psm3_allow_ib_subnet(uint64_t subnet)
{
	char buf[19];	// 16 hex digits + "0x" + '\0'
	snprintf(buf, sizeof(buf), "0x%016"PRIx64, subnet);
	return psm3_allow_subnet(buf, "IB/OPA");
}

// compare an IPv4 subnet against the PSM3_SUBNETS which are allowed
// comparison is done as a string extended global pattern compare
// the ntop format of the subnet is used
// return 1 if allowed, 0 if not match (or unable to compare)
int psm3_allow_ipv4_subnet(uint32_t subnet, uint8_t prefix_len)
{
	char buf[INET_ADDRSTRLEN+3];
	const char *subnet_str = psmi_ipv4_ntop(subnet, prefix_len, buf, sizeof(buf));
	return  psm3_allow_subnet(subnet_str, "IPv4");
}

// compare an IPv6 subnet against the PSM3_SUBNETS which are allowed
// comparison is done as a string extended global pattern compare
// the ntop format of the subnet is used
// return 1 if allowed, 0 if not match (or unable to compare)
int psm3_allow_ipv6_subnet(psmi_bare_netaddr128_t subnet, uint8_t prefix_len)
{
	char buf[INET6_ADDRSTRLEN+4];
	const char *subnet_str = psmi_ipv6_ntop(subnet, prefix_len, buf, sizeof(buf));
	return  psm3_allow_subnet(subnet_str, "IPv6");
}

// build an IB/OPA subnet from basic addressing information
psmi_subnet128_t psm3_build_ib_subnet128(uint64_t hi)
{
	psmi_subnet128_t subnet;

	subnet.bare.hi = hi;
	subnet.bare.lo = 0;
	subnet.fmt = PSMI_ADDR_FMT_IB;
	subnet.prefix_len = 64;
	return subnet;
}

// build an IB/OPA addr from basic addressing information
psmi_naddr128_t psm3_build_ib_naddr128(psmi_gid128_t gid)
{
	psmi_naddr128_t addr;

	addr.bare.hi = gid.hi;
	addr.bare.lo = gid.lo;
	addr.fmt = PSMI_ADDR_FMT_IB;
	addr.prefix_len = 64;
	return addr;
}

// build an IPv4 subnet from basic addressing information
psmi_subnet128_t psm3_build_ipv4_subnet128(uint32_t ip_addr,
		uint32_t ip_netmask, uint8_t prefix_len)
{
	psmi_subnet128_t subnet;

	subnet.bare.hi = 0;
	subnet.bare.lo = ip_addr & ip_netmask;
	subnet.fmt = PSMI_ADDR_FMT_IPV4;
	subnet.prefix_len = prefix_len;
	return subnet;
}

// build an IPv4 addr from basic addressing information
psmi_naddr128_t psm3_build_ipv4_naddr128(uint32_t ip_addr, uint8_t prefix_len)
{
	psmi_naddr128_t addr;

	addr.bare.hi = 0;
	addr.bare.lo = ip_addr;
	addr.fmt = PSMI_ADDR_FMT_IPV4;
	addr.prefix_len = prefix_len;
	return addr;
}

// build an IPv6 subnet from basic addressing information
psmi_subnet128_t psm3_build_ipv6_subnet128(psmi_bare_netaddr128_t ipv6_addr,
		 psmi_bare_netaddr128_t ipv6_netmask, uint8_t prefix_len)
{
	psmi_subnet128_t subnet;

	subnet.bare = psmi_bare_netaddr128_and(ipv6_addr, ipv6_netmask);
	subnet.fmt = PSMI_ADDR_FMT_IPV6;
	subnet.prefix_len = prefix_len;
	return subnet;
}

// build an IPv6 addr from basic addressing information
psmi_naddr128_t psm3_build_ipv6_naddr128(psmi_bare_netaddr128_t ip_addr,
		uint8_t prefix_len)
{
	psmi_naddr128_t addr;

	addr.bare = ip_addr;
	addr.fmt = PSMI_ADDR_FMT_IPV6;
	addr.prefix_len = prefix_len;
	return addr;
}

// build a NID from basic addressing information for later comparison to
// psm3_epid_nid()
// only valid when IPS enabled
// Considered having a protocol argument, however this is only used for
// internal comparisons and only 1 call to psm3_epid_fmt_nid so no benefit.
// So we just have all NIDs end up with protocol==0
psm2_nid_t psm3_build_nid(uint8_t unit, psmi_naddr128_t addr, unsigned lid)
{
	psm2_nid_t ret = { };	// never used, keep compiler happy below
	switch (addr.fmt) {
	case PSMI_ADDR_FMT_IB:
		return psm3_epid_pack_ib(lid, 0, addr);
		break;
	case PSMI_ADDR_FMT_IPV4:
		return psm3_epid_pack_ipv4(addr, 0, 0, 0);
		break;
	case PSMI_ADDR_FMT_IPV6:
		return psm3_epid_pack_ipv6(addr, 0, 0, 0);
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		return ret; // keep compiler happy, never reached
	}
}



// impose the addr_fmt specific rules for when we allow
// diferent subnets to still be able to connect
int psm3_subnets_match(psmi_subnet128_t a, psmi_subnet128_t b)
{
	int is_eth = PSMI_ADDR_FMT_IS_ETH(a.fmt);
	// note psm3_ep_connect enforces that addr_fmt must match
	// so when we get down to comparing subnets of each rail we
	// impose the same restriction
	if (a.fmt != b.fmt)
		return 0;	// assume no IB/OPA Verbs to Eth RoCE routers
				// nor IPv4 to IPv6 routing
	// if subnet matches, we are ok
	// if ethernet with PSM3_ALLOW_ROUTERS we permit ethernet routing
	//	between different subnets
	return ((a.prefix_len == b.prefix_len
			&& a.bare.hi == b.bare.hi && a.bare.lo == b.bare.lo)
		|| (is_eth && psm3_allow_routers));
}

// compare our local subnet to a remote epids's subnet
// For IPATH/OPA, the epid only holds a subset of the subnet bits so
// we must compare with a subset of bits in our local subnet
int psm3_subnets_match_epid(psmi_subnet128_t subnet, psm2_epid_t epid)
{
	return psm3_subnets_match(psmi_subnet_epid_subset(subnet),
                                 psm3_epid_subnet(epid));
}

#ifdef PSM_SOCKETS
// for now just report 0 -> equal, 1 -> not equal, don't worry about < and >
// we don't worry about flowinfo or scope_id
int psm3_sockaddr_cmp(struct sockaddr_in6 *a, struct sockaddr_in6 *b)
{
	psmi_assert(a->sin6_family == AF_INET6);
	if (a->sin6_family != b->sin6_family) return 1;
	if (a->sin6_port != b->sin6_port) return 1;
	//if (a->sin6_flowinfo != b->sin6_flowinfo) return 1;
	if (a->sin6_addr.s6_addr32[0] != b->sin6_addr.s6_addr32[0]) return 1;
	if (a->sin6_addr.s6_addr32[1] != b->sin6_addr.s6_addr32[1]) return 1;
	if (a->sin6_addr.s6_addr32[2] != b->sin6_addr.s6_addr32[2]) return 1;
	if (a->sin6_addr.s6_addr32[3] != b->sin6_addr.s6_addr32[3]) return 1;
	//if (a->sin6_scope_id != b->sin6_scope_id) return 1;
	return 0;
}

void psm3_build_sockaddr(struct sockaddr_in6 *in6, uint16_t port,
			uint64_t gid_hi, uint64_t gid_lo, uint32_t scope_id)
{
	in6->sin6_family = AF_INET6;
	in6->sin6_port = __cpu_to_be16(port);
	in6->sin6_flowinfo = 0;     // TBD
	in6->sin6_addr.s6_addr32[0] = __cpu_to_be32(gid_hi >> 32);
	in6->sin6_addr.s6_addr32[1] = __cpu_to_be32(gid_hi & 0xffffffff);
	in6->sin6_addr.s6_addr32[2] = __cpu_to_be32(gid_lo >> 32);
	in6->sin6_addr.s6_addr32[3] = __cpu_to_be32(gid_lo & 0xffffffff);
	in6->sin6_scope_id = scope_id;
}

void psm3_epid_build_sockaddr(struct sockaddr_in6 *in6, psm2_epid_t epid,
				uint32_t scope_id)
{
	psmi_epid_t e = { .psm2_epid = epid };

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IPV4:
		psmi_assert(e.v4.protocol == PSMI_EPID_ETH_PROTO_INET);
		psmi_assert(e.v4.sockets.pri_sock != 0);
		psm3_build_sockaddr(in6, e.v4.sockets.pri_sock,
				PSMI_IPV4_GID_HI(e.v4.ipv4_addr),
				PSMI_IPV4_GID_LO(e.v4.ipv4_addr), 0);
		break;
	case PSMI_ADDR_FMT_IPV6:
		psmi_assert(e.v6.protocol == PSMI_EPID_ETH_PROTO_INET);
		psmi_assert(e.v6.sockets.pri_sock != 0);
		psm3_build_sockaddr(in6, e.v6.sockets.pri_sock, e.v6.gid_hi, e.v6.gid_lo,
					scope_id);
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
}

void psm3_epid_build_aux_sockaddr(struct sockaddr_in6 *in6, psm2_epid_t epid,
				uint32_t scope_id)
{
	psmi_epid_t e = { .psm2_epid = epid };

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_IPV4:
		psmi_assert(e.v4.protocol == PSMI_EPID_ETH_PROTO_INET);
		psmi_assert(e.v4.sockets.aux_sock != 0);
		psm3_build_sockaddr(in6, e.v4.sockets.aux_sock,
				PSMI_IPV4_GID_HI(e.v4.ipv4_addr),
				PSMI_IPV4_GID_LO(e.v4.ipv4_addr), 0);
		break;
	case PSMI_ADDR_FMT_IPV6:
		psmi_assert(e.v6.protocol == PSMI_EPID_ETH_PROTO_INET);
		psmi_assert(e.v6.sockets.aux_sock != 0);
		psm3_build_sockaddr(in6, e.v6.sockets.aux_sock, e.v6.gid_hi, e.v6.gid_lo,
					scope_id);
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
}
#endif /* PSM_SOCKETS */

/* this is used to form a psm2_epid_t from individual words found in packets */
psm2_epid_t psm3_epid_pack_words(uint64_t w0, uint64_t w1, uint64_t w2)
{
	psmi_epid_t e;

	e.w[0] = w0;
	e.w[1] = w1;
	e.w[2] = w2;
	psmi_assert(e.addr_fmt == PSMI_ADDR_FMT_SHM || PSMI_IPS_ADDR_FMT_IS_VALID(e.addr_fmt));
#ifdef PSM_DEBUG
	if (e.addr_fmt == PSMI_ADDR_FMT_IPV6 || e.addr_fmt == PSMI_ADDR_FMT_IB)
		psmi_assert(w1 != 0 || w2 != 0);
	else if (e.addr_fmt == PSMI_ADDR_FMT_IPV4)
		psmi_assert(w2 == 0);
#endif
	return e.psm2_epid;
}


#if 0
psm2_epid_t psm2_epid_pack_word(uint64_t w0)
{
	psm2_epid_t rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_epid_pack_word(w0);
	PSM2_LOG_MSG("leaving");
	return rv;
}
#endif

/* These are used to extract individual words in the epid to form packets
 * shorter epids will return 0 for the 2nd and/or 3rd word
 */
uint64_t psm3_epid_w0(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	return e.w[0];
}

uint64_t psm3_epid_w1(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	return e.w[1];
}

uint64_t psm3_epid_w2(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	return e.w[2];
}

psm2_epid_t psm3_epid_zeroed_internal(void)
{
	psmi_epid_t e = { };
	return e.psm2_epid;
}

psm2_epid_t psm3_epid_zeroed(void)
{
	psm2_epid_t rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_epid_zeroed_internal();
	PSM2_LOG_MSG("leaving");
	return rv;
}

/* these routines use thread specific storage for the return string so
 * they are thread safe.  bufno can be 0-5 to allow a single print
 * call to format up to 6 addresses
 */

// size of psm3_epid_fmt_addr
#define PSMI_EP_ADDR_LEN        (max(INET_ADDRSTRLEN + 7 + 1 + 9, 72))
					/* IP=addr QP=# or LID=# QP=#
					 * or IP=addr UDP=#
					 * or IP=addr TCP=# UDP=#
					 * or LID=addr:context:sub_context
					 * IPv6 addr = 48 char + 7 digit QP #
					 * + 9 other characters + 1 \0
					 * IPv6 addr = 48 char + 5 digit TCP #
					 * + 5 digit UDP # + 13 other characters + 1 \0
					 * =70, so round up to next mult of 8
					 */

// thread local storage for temp strings used in *_fmt* routines.  Intended
// use is as arguments to debug prints as buf will be reused on next
// *_fmt* call with same bufno
// outstrbufs sized for largest which is psmi_sockaddr_ntop:
//	IB: IPv6 GID (INET6_ADDRSTRLEN = 48)
//		 + 16 digit sid + 3 " 0x"
//		 + 4 digit pkey + 3 " 0x"
//		<--- largest total at 48 + 19 + 7 = 74
//	IPv6: IPv6 addr/prefix_len (INET6_ADDRSTRLEN = 48)
//		 + 5 digit port + 3 digit scope
//		 + 2 "  "
//	IPv4: IPv4 addr/prefix_len (INET_ADDRSTRLEN = 16)
//		 + 5 digit port +  1 " "
// others are INET6_ADDRSTRLEN+4 or INET_ADDRSTRLEN+3
// hex epid is 53 (0x\0 (3) + 3 * 16 + 2(:))
// context is 9 (max of 2^24 decimal + \0)
// PSMI_EP_ADDR_LEN is 72
// ibv_gid is 48 including \0
#define PSM_ADDRSTRLEN (max(INET6_ADDRSTRLEN+19+7, PSMI_EP_ADDR_LEN))
#define PSM_MAX_BUFNO 5	// bufno 0 to 5 available for generic use
// We allocate 1 extra buffer so psm3_syslog can have an internal buffer
// independent from callers who may supply strings using their own bufno.
// A little paranoid since formatting occurs prior to psm3_syslog call
// but makes it safe if psm3_syslog is called more directly in future
static __thread char outstrbufs[PSM_MAX_BUFNO+2][PSM_ADDRSTRLEN+4];

// output portion of ep->subnet (eg. a subnet returned by get_port_subnet)
// which can be compared to psm3_epid_subnet().
// This accounts for the fact that some epid addr_fmt's (IB/OPA) only contain a
// subset of the subnet.
// Only valid for subnet used in remote IPS connections
// see psmi_subnet_epid_subset and psm3_subnets_match_epid
const char *psm3_subnet_epid_subset_fmt(psmi_subnet128_t subnet, int bufno)
{
	return psm3_subnet128_fmt(psmi_subnet_epid_subset(subnet), bufno);
}

/* this returns just the epid in hex */
const char *psm3_epid_fmt_internal(psm2_epid_t epid, int bufno)
{
	char *outstr = outstrbufs[bufno];
	psmi_epid_t e = { .psm2_epid = epid };

	// Used for parsable EPM messages in logs, human output and shmopen
	snprintf(outstr, sizeof(outstrbufs[0]), "0x%"PRIx64":%"PRIx64":%"PRIx64, e.w[0], e.w[1], e.w[2]);
	return outstr;
}

const char *psm3_epid_fmt(psm2_epid_t epid, int bufno)
{
	// no logging since can be used inside app logging
	return psm3_epid_fmt_internal(epid, bufno);
}

const char *psm3_epid_str_addr_fmt(psm2_epid_t epid)
{
	psmi_epid_t e = { .psm2_epid = epid };
	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM:
		return "shm";
		break;
	case PSMI_ADDR_FMT_IB:
		return "IB/OPA";
		break;
	case PSMI_ADDR_FMT_IPV4:
		return "IPv4";
		break;
	case PSMI_ADDR_FMT_IPV6:
		return "IPv6";
		break;
	default:
		return "Unknown";
		break;
	}
}

// for error messages and psm3_subnet128_fmt_name
// note that psm3_subnet128_fmt_name sets the OFI fi_info
// fabric name
static const char *psm3_protocol_str(psmi_eth_proto_t protocol)
{
	switch (protocol) {
	case PSMI_ETH_PROTO_ROCE:
		return "RoCE";
		break;
	case PSMI_ETH_PROTO_UDP:
		return "UDP";
		break;
	case PSMI_ETH_PROTO_TCP:
		return "TCP";
		break;
	case PSMI_ETH_PROTO_NA:
		return "N/A";	// internal only when N/A
		break;
	default:
		psmi_assert_always(0);	// unexpected protocol
		return "Unknown";	// keep compiler happy
		break;
	}
}

// returns for psmi_epid_fmt_addr a string describing psm3_epid_context
static const char *psm3_protocol_context_str(psmi_eth_proto_t protocol)
{
	switch (protocol) {
	case PSMI_ETH_PROTO_ROCE:
		return "QP";
		break;
	case PSMI_ETH_PROTO_UDP:
		return "UDP";
		break;
	case PSMI_ETH_PROTO_TCP:
		return "TCP";
		break;
	default:
		// might be called in psm3_handle_error, so output something
		// instead of asserting
		return "Unknown";	// keep compiler happy
		break;
	}
}

const char *psm3_epid_str_protocol(psm2_epid_t epid)
{
	return psm3_protocol_str(psm3_epid_protocol(epid));
}

// for sockets just show primary context
// this is used for process label in logs if rank is not available
const char *psm3_epid_fmt_context(psm2_epid_t epid, int bufno)
{
	char *outstr = outstrbufs[bufno];

	snprintf(outstr, sizeof(outstrbufs[0]), "%u",
			(uint32_t)psm3_epid_context(epid));
	return outstr;
}

/* output network address in a more human readable format
 * for IPv4 we show IP address/prefix_len
 * for IPv6 we show full 128b address/prefix_len
 * for IB/OPA we show LID and GID
 * for intra-node we show PID or Rank
 * resulting string <= INET6_ADDRSTRLEN+4
 */
const char *psm3_nid_fmt(psm2_nid_t nid, int bufno)
{
	char *outstr = outstrbufs[bufno];
	psmi_epid_t e = { .psm2_epid = nid };
	psmi_bare_netaddr128_t bare;
	char buf[INET6_ADDRSTRLEN+4];

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM:
		bare.hi = e.v6.gid_hi;
		bare.lo = e.v6.gid_lo;
		snprintf(outstr, sizeof(outstrbufs[0]), "PID=%u IP=%s",
			 (uint32_t)e.shm.pid,
			 psmi_ipv6_ntop(bare, 0, buf, sizeof(buf)));
		break;
	case PSMI_ADDR_FMT_IB:
		snprintf(outstr, sizeof(outstrbufs[0]), "LID=%u GID=0x%"PRIx64":%"PRIx64,
			 (uint16_t)e.v3.lid, e.v3.gid_hi, e.v3.gid_lo);
		break;
	case PSMI_ADDR_FMT_IPV4:
		snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s",
			psmi_ipv4_ntop(e.v4.ipv4_addr, e.v4.prefix_len + 1, buf, sizeof(buf)));
		break;
	case PSMI_ADDR_FMT_IPV6:
		bare.hi = e.v6.gid_hi;
		bare.lo = e.v6.gid_lo;
		snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s",
			psmi_ipv6_ntop(bare, e.v6.prefix_len + 1, buf, sizeof(buf)));
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
	return outstr;
}

/* this returns just the network address from decoding the epid
 * for IPv4 we show IP address/prefix_len
 * for IPv6 we show full 128b address/prefix_len
 * for IB/OPA we show LID and GID
 * for intra-node we show PID or Rank
 * resulting string <= INET6_ADDRSTRLEN+4
 */
const char *psm3_epid_fmt_nid(psm2_epid_t epid, int bufno)
{
	return psm3_nid_fmt(psm3_epid_nid(epid),  bufno);
}

/* this returns the addressing from decoding the epid
 * output has context and nid in a more human readable format
 * for IPv4 we show IP address/prefix_len
 * for IPv6 we show full 128b address/prefix_len
 * for IB/OPA we show LID and GID
 * for intra-node we show PID or Rank
 * resulting string <= PSMI_EP_ADDR_LEN (INET6_ADDRSTRLEN+17+spare)
 *
 * This should not be used for psm2_nid_t, instead use psm3_nid_fmt
 */
const char *psm3_epid_fmt_addr(psm2_epid_t epid, int bufno)
{
	char *outstr = outstrbufs[bufno];
	psmi_epid_t e = { .psm2_epid = epid };
	psmi_bare_netaddr128_t bare;
	char buf[INET6_ADDRSTRLEN+4];

	switch (e.addr_fmt) {
	case PSMI_ADDR_FMT_SHM:
		bare.hi = e.v6.gid_hi;
		bare.lo = e.v6.gid_lo;
		snprintf(outstr, sizeof(outstrbufs[0]), "PID=%u IP=%s",
			 (uint32_t)e.shm.pid,
			 psmi_ipv6_ntop(bare, 0, buf, sizeof(buf)));
		break;
	case PSMI_ADDR_FMT_IB:
		snprintf(outstr, sizeof(outstrbufs[0]), "LID=%u GID=0x%"PRIx64":%"PRIx64" QP=%u",
			(uint16_t)e.v3.lid, e.v3.gid_hi, e.v3.gid_lo,
			(uint32_t)e.v3.qpn);
		break;
	case PSMI_ADDR_FMT_IPV4:
#ifdef PSM_SOCKETS
		if (e.v4.protocol == PSMI_EPID_ETH_PROTO_INET
		    && e.v4.sockets.aux_sock) {
			snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s TCP=%u UDP=%u",
				psmi_ipv4_ntop(e.v4.ipv4_addr, e.v4.prefix_len + 1, buf, sizeof(buf)),
				(uint32_t)psm3_epid_context(epid),
				psm3_epid_aux_socket(epid));
		} else
#endif
		{
			snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s %s=%u",
				psmi_ipv4_ntop(e.v4.ipv4_addr, e.v4.prefix_len + 1, buf, sizeof(buf)),
				psm3_protocol_context_str(psm3_epid_protocol(epid)),
				(uint32_t)psm3_epid_context(epid));
		}
		break;
	case PSMI_ADDR_FMT_IPV6:
		bare.hi = e.v6.gid_hi;
		bare.lo = e.v6.gid_lo;
#ifdef PSM_SOCKETS
		if (e.v6.protocol == PSMI_EPID_ETH_PROTO_INET
		    && e.v6.sockets.aux_sock) {
			snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s TCP=%u UDP=%u",
				psmi_ipv6_ntop(bare, e.v6.prefix_len + 1, buf, sizeof(buf)),
				(uint32_t)psm3_epid_context(epid),
				psm3_epid_aux_socket(epid));
		} else
#endif
		{
			snprintf(outstr, sizeof(outstrbufs[0]), "IP=%s %s=%u",
				psmi_ipv6_ntop(bare, e.v6.prefix_len + 1, buf, sizeof(buf)),
				psm3_protocol_context_str(psm3_epid_protocol(epid)),
				(uint32_t)psm3_epid_context(epid));
		}
		break;
	default:
		// might be called in psm3_handle_error, so output something instead of
		// asserting
		snprintf(outstr, sizeof(outstrbufs[0]), "Invalid Fmt");
		break;
	}
	return outstr;
}

// format a 128b GID.  We use the same style for all GIDs
// beware this is called in psm3_naddr128_fmt so avoid duplicate
// bufno in prints which call both
const char *psm3_gid128_fmt(psmi_gid128_t gid, int bufno)
{
	char *outstr = outstrbufs[bufno];

	snprintf(outstr, sizeof(outstrbufs[0]), "0x%"PRIx64":%"PRIx64, gid.hi, gid.lo);
	return outstr;
}

// format a subnet or netmask appropriately for the given addr_fmt
// prefix_len is optional (0 to omit) and only used for
// IPv4 and IPv6 addresses for the optional /prefix_len suffix
const char *psm3_subnet128_fmt(psmi_subnet128_t subnet, int bufno)
{
	char *outstr = outstrbufs[bufno];

// TBD - handle V_SHM
	if (subnet.fmt == PSMI_ADDR_FMT_IPV4)
		return psmi_ipv4_ntop(subnet.bare.lo, subnet.prefix_len, outstr, sizeof(outstrbufs[0]));
	else if (subnet.fmt == PSMI_ADDR_FMT_IPV6)
		return psmi_ipv6_ntop(subnet.bare, subnet.prefix_len, outstr, sizeof(outstrbufs[0]));
	else
		snprintf(outstr, sizeof(outstrbufs[0]), "0x%"PRIx64, subnet.bare.hi);
	return outstr;
}

void psm3_subnet128_fmt_name(psmi_eth_proto_t protocol, psmi_subnet128_t subnet,
				char *buf, int buflen)
{
	const char *prefix = psm3_protocol_str(protocol);

	switch (subnet.fmt) {
	case PSMI_ADDR_FMT_IB:
		snprintf(buf, buflen, "IB/OPA-%s", psm3_subnet128_fmt(subnet, 0));
		break;
	case PSMI_ADDR_FMT_IPV4:
		if (psm3_allow_routers) {
			snprintf(buf, buflen, "%s-IPv4", prefix);
		} else {
			snprintf(buf, buflen, "%s-%s", prefix,
 				psm3_subnet128_fmt(subnet, 0));
		}
		break;
	case PSMI_ADDR_FMT_IPV6:
		if (psm3_allow_routers) {
			snprintf(buf, buflen, "%s-IPv6", prefix);
		} else {
			snprintf(buf, buflen, "%s-%s", prefix,
 				psm3_subnet128_fmt(subnet, 0));
		}
		break;
	default:
		psmi_assert_always(0);	// unexpected addr_fmt
		break;
	}
}

/* this returns just the subnet from decoding the epid
 * output has subnet in a more human readable format
 * Note for IB/OPA this is only a subset of the subnet
 */
const char *psm3_epid_fmt_subnet(psm2_epid_t epid, int bufno)
{
	return psm3_subnet128_fmt(psm3_epid_subnet(epid), bufno);
}

// format an address (as returned by get_port_subnet addr argument)
// appropriately for the given addr_fmt
// prefix_len is optional (0 to omit) and only used for
// IPv4 and IPv6 addresses for the optional /prefix_len suffix
// IB/OPA addresses shown as a full 128b GID
const char *psm3_naddr128_fmt(psmi_naddr128_t addr, int bufno)
{
	char *outstr = outstrbufs[bufno];

	if (addr.fmt == PSMI_ADDR_FMT_IPV4)
		return psmi_ipv4_ntop(addr.bare.lo, addr.prefix_len, outstr, sizeof(outstrbufs[0]));
	else if (addr.fmt == PSMI_ADDR_FMT_IPV6)
		return psmi_ipv6_ntop(addr.bare, addr.prefix_len, outstr, sizeof(outstrbufs[0]));
	else
		return psm3_gid128_fmt(addr.bare, bufno);
}

#ifdef PSM_VERBS
// format an ibv_gid
const char *psm3_ibv_gid_fmt(union ibv_gid gid, int bufno)
{
	char *outstr = outstrbufs[bufno];

	// ibv_gid raw[] is in big endian byte order
	snprintf(outstr, sizeof(outstrbufs[0]), 
		"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:"
			"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
		gid.raw[0], gid.raw[1], gid.raw[2], gid.raw[3],
		gid.raw[4], gid.raw[5], gid.raw[6], gid.raw[7],
		gid.raw[8], gid.raw[9], gid.raw[10], gid.raw[11],
		gid.raw[12], gid.raw[13], gid.raw[14], gid.raw[15]);

	return outstr;
}

int psm3_nonzero_gid(const union ibv_gid *gid)
{
	static union ibv_gid zero_gid = { };

	return memcmp(gid, &zero_gid, sizeof(*gid)) != 0;
}
#endif
			 
#if 0
psm2_nid_t psm3_epid_nid(psm2_epid_t epid)
{
	psm2_nid_t rv;

	PSM2_LOG_MSG("entering");
	rv = psmi_epid_nid(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}

uint64_t psm3_epid_context(psm2_epid_t epid)
{
	uint64_t rv;

	PSM2_LOG_MSG("entering");
	rv = psmi_epid_context(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}
#endif // 0

uint64_t psm3_epid_port(psm2_epid_t epid)
{
	uint64_t rv;

	PSM2_LOG_MSG("entering");
	// deprecated, replaced by psm3_epid_context
	rv = psm3_epid_context(epid);
	PSM2_LOG_MSG("leaving");
	return rv;
}

static int psm3_epid_cmp_word(uint64_t a, uint64_t b)
{
	if (a == b)
		return 0;
	else if (a < b)
		return -1;
	else
		return 1;
}

/* compare epid a to b
 * returns:
 *	0 - a == b
 *	-1 - a < b
 *	1 - a > b
 */
int psm3_epid_cmp_internal(psm2_epid_t a, psm2_epid_t b)
{
	int ret;

	ret = psm3_epid_cmp_word(a.w[0], b.w[0]);
	if (ret)
		return ret;
	// 1st word's match, so addr_fmt should match
	psmi_assert(psm3_epid_addr_fmt(a) == psm3_epid_addr_fmt(b));
	ret = psm3_epid_cmp_word(a.w[1], b.w[1]);
	if (ret)
		return ret;
	return psm3_epid_cmp_word(a.w[2], b.w[2]);
}

int psm3_epid_cmp(psm2_epid_t a, psm2_epid_t b)
{
	int rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_epid_cmp_internal(a, b);
	PSM2_LOG_MSG("leaving");
	return rv;
}

/* an EPID is 0 when it is undefined/empty
 * returns 0: EPID is non-zero, 1: EPID is zero
 */
int psm3_epid_zero_internal(psm2_epid_t a)
{
	psmi_epid_t ae = { .psm2_epid = a };
	return (ae.w[0] == 0 && ae.w[1] == 0 && ae.w[2] == 0);
}

int psm3_epid_zero(psm2_epid_t a)
{
	int rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_epid_zero_internal(a);
	PSM2_LOG_MSG("leaving");
	return rv;
}

/* compare nid a to b
 * returns:
 *	0 - a == b
 *	-1 - a < b
 *	1 - a > b
 */
int psm3_nid_cmp(psm2_nid_t a, psm2_nid_t b)
{
	int rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_epid_cmp_internal(a, b);
	PSM2_LOG_MSG("leaving");
	return rv;
}

/* an NID is 0 when it is undefined/empty
 * returns 0: NID is non-zero, 1: NID is zero
 */
int psm3_nid_zero(psm2_nid_t a)
{
	int rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_nid_zero_internal(a);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_nid_t psm3_nid_zeroed(void)
{
	psm2_nid_t rv;

	PSM2_LOG_MSG("entering");
	rv = psm3_nid_zeroed_internal();
	PSM2_LOG_MSG("leaving");
	return rv;
}

/* this returns the simple name, if not known gives addressing */
const char *psm3_epaddr_get_hostname(psm2_epid_t epid, int bufno)
{
	char *h;
	psm2_nid_t nid = psm3_epid_nid(epid);

	/* First, if we have registered a host for this epid, just return that,
	 * or else try to return something with lid and context */
	h = psm3_epid_lookup(PSMI_EP_HOSTNAME, nid);
	if (h != NULL)
		return h;
	else {
		return psm3_epid_fmt_addr(epid, bufno);
	}
}

/* this returns the name and addressing */
/* if name not known just gives addressing */
/* allow for up to 2 uses in a thread within a given print, bufno=0-1 */
/* beware can't duplicate bufno used for other *_fmt* functions in same print */
const char *psm3_epaddr_get_name(psm2_epid_t epid, int bufno)
{
	static __thread char hostnamebufs[2][PSMI_EP_HOSTNAME_LEN+PSMI_EP_ADDR_LEN+3];
	char *h, *hostname;
	psm2_nid_t nid = psm3_epid_nid(epid);
	hostname = hostnamebufs[bufno];

	h = psm3_epid_lookup(PSMI_EP_HOSTNAME, nid);
	if (h == NULL)
		return psm3_epid_fmt_addr(epid, bufno);
	else {
		snprintf(hostname, sizeof(hostnamebufs[0]), "%s (%s)", h,
				psm3_epid_fmt_addr(epid, bufno));
	}
	return hostname;
}

// superset of inet_ntop.  For AF_INET and AF_INET6 outputs address and port
// AF_IB is unsupported.
// caller of psm3_sockaddr_fmt has prefix_len in the epddr->epid in some cases
// but in other cases (inbound packets) we do not.  So chose to
// not enable this function with prefix_len argument
static const char *psmi_sockaddr_ntop(struct sockaddr* addr, char *dst,
					socklen_t size)
{
	if (! dst || size < PSM_ADDRSTRLEN) {
		// be strict, keeps it simple
		errno = ENOSPC;
		return "ENOSPC";	// callers just use in a printf
	}
	*dst = '\0';	// be safe
	if (! addr) {
		snprintf(dst, size, "(nil)");
		return dst;
	}
	// show network address and port (or sid)
	switch (addr->sa_family) {
	case AF_INET:
	{
		struct sockaddr_in* in_addr = ((struct sockaddr_in*)addr);
		// we show the IPv4 address and port
		inet_ntop(AF_INET,  &in_addr->sin_addr, dst, size);
		snprintf(dst+strlen(dst), size-strlen(dst), " %u", be16toh(in_addr->sin_port));
		return dst;
	}
	case AF_INET6:
	{
		struct sockaddr_in6* in_addr = ((struct sockaddr_in6*)addr);
		// we show just the IPv6 address, port and scope_id.
		// could also show flowinfo
		inet_ntop(AF_INET6,  &in_addr->sin6_addr, dst, size);
		snprintf(dst+strlen(dst), size-strlen(dst), " %u %u", be16toh(in_addr->sin6_port), in_addr->sin6_scope_id);
		return dst;
	}
	default:
		snprintf(dst, size, "Unsupported");
		return dst;
	}
}

// caller of psm3_sockaddr_fmt has prefix_len in the epddr->epid in some cases
// but in other cases (inbound packets) we do not.  So chose to
// not enable this function with prefix_len argument
const char *psm3_sockaddr_fmt(struct sockaddr* addr, int bufno)
{
	char *outstr = outstrbufs[bufno];

	return psmi_sockaddr_ntop(addr, outstr, sizeof(outstrbufs[0]));
}

// subset of inet_ntop.  Outputs just address
// caller of psm3_sockaddr_fmt_addr may not yet have prefix_len, so chose to
// not enable this function with prefix_len argument
static const char *psmi_sockaddr_ntop_addr(struct sockaddr* addr, char *dst,
						socklen_t size)
{
	if (! dst || size < INET6_ADDRSTRLEN) {
		// be strict, keeps it simple
		errno = ENOSPC;
		return "ENOSPC";	// callers just use in a printf
	}
	*dst = '\0';	// be safe
	if (! addr) {
		snprintf(dst, size, "(nil)");
		return dst;
	}
	// show network address and port (or sid)
	switch (addr->sa_family) {
	case AF_INET:
		// we show just the IPv4 address
		inet_ntop(AF_INET, &((struct sockaddr_in*)addr)->sin_addr,
				dst, size);
		return dst;
	case AF_INET6:
		// we show just the IPv6 address
		inet_ntop(AF_INET6, &((struct sockaddr_in6*)addr)->sin6_addr,
				dst, size);
		return dst;
	default:
		snprintf(dst, size, "Unsupported");
		return dst;
	}
}

// caller of psm3_sockaddr_fmt_addr may not yet have prefix_len, so chose to
// not enable this function with prefix_len argument
const char *psm3_sockaddr_fmt_addr(struct sockaddr* addr, int bufno)
{
	char *outstr = outstrbufs[bufno];

	return psmi_sockaddr_ntop_addr(addr, outstr, sizeof(outstrbufs[0]));
}

// subset of inet_ntop.
// formats IPv4 address, subnet or netmask (in host byte order)
// into dst which has >= size bytes available.
// when prefix_len != 0 supplied, address will have a /len suffix
//	such as 192.168.100.5/24
// returns a \0 terminated string suitable for use in printf such as:
// { char buf[INET_ADDRSTRLEN+3];
//		 printf("IP=%s\n", psmi_ipv4_ntop(ip_addr, prefix_len, buf, sizeof(buf));}
// on success pointer returned will be to dst.  For various errors a
// constant string outside of dst may be returned such that caller can safely
// call printf (or similar functions) without checking return value.
// on errors, errno is also set.
// Note IPv4 addresses worse case length is INET_ADDRSTRLEN w/o prefix_len
// or INET_ADDRSTRLEN+3 w/prefix_len.
static const char *psmi_ipv4_ntop(uint32_t ip_addr, uint8_t prefix_len,
					char *dst, socklen_t size)
{
	struct in_addr in_addr;
	if (! dst || size < INET_ADDRSTRLEN + (prefix_len?3:0) || prefix_len > 32) {
		// be strict, keeps it simple
		errno = ENOSPC;
		return "ENOSPC";	// callers just use in a printf
	}
	*dst = '\0';	// be safe
	in_addr.s_addr = __cpu_to_be32(ip_addr);
	// we show the IPv4 address
	inet_ntop(AF_INET, &in_addr, dst, size);
	if (prefix_len) {
		int l = strlen(dst);
		psmi_assert(l < size);
		snprintf(dst + l, size - l, "/%u", prefix_len);
	}
	return dst;
}

const char *psm3_ipv4_fmt(uint32_t ip_addr, uint8_t prefix_len, int bufno)
{
	char *outstr = outstrbufs[bufno];

	return psmi_ipv4_ntop(ip_addr, prefix_len, outstr, sizeof(outstrbufs[0]));
}

// subset of inet_ntop.
// formats IPv6 address, subnet or netmask (in host byte order)
// into dst which has >= size bytes available.
// when prefix_len != 0 supplied, address will have a /len suffix
//	such as fd00::42a6:b7ff:fe19:380/64
// returns a \0 terminated string suitable for use in printf such as:
// { char buf[INET6_ADDRSTRLEN+4];
//		 printf("IP=%s\n", psmi_ipv6_ntop(addr, prefix_len, buf, sizeof(buf));}
// on success pointer returned will be to dst.  For various errors a
// constant string outside of dst may be returned such that caller can safely
// call printf (or similar functions) without checking return value.
// on errors, errno is also set.
// Note IPv6 addresses worse case length is INET6_ADDRSTRLEN w/o prefix_len
// or INET6_ADDRSTRLEN+4 w/prefix_len.
static const char *psmi_ipv6_ntop(psmi_bare_netaddr128_t ipv6_addr,
					uint8_t prefix_len, char *dst,
					socklen_t size)
{
	struct in6_addr in6_addr;
	if (! dst || size < INET6_ADDRSTRLEN + (prefix_len?4:0) || prefix_len > 128) {
		// be strict, keeps it simple
		errno = ENOSPC;
		return "ENOSPC";	// callers just use in a printf
	}
	*dst = '\0';	// be safe
	in6_addr.s6_addr32[0] = __cpu_to_be32(ipv6_addr.hi >> 32);
	in6_addr.s6_addr32[1] = __cpu_to_be32(ipv6_addr.hi & 0xffffffff);
	in6_addr.s6_addr32[2] = __cpu_to_be32(ipv6_addr.lo >> 32);
	in6_addr.s6_addr32[3] = __cpu_to_be32(ipv6_addr.lo & 0xffffffff);
	// we show the IPv6 address
	inet_ntop(AF_INET6, &in6_addr, dst, size);
	if (prefix_len) {
		int l = strlen(dst);
		psmi_assert(l < size);
		snprintf(dst + l, size - l, "/%u", prefix_len);
	}
	return dst;
}

const char *psm3_ipv6_fmt(psmi_bare_netaddr128_t ipv6_addr, uint8_t prefix_len,
				int bufno)
{
	char *outstr = outstrbufs[bufno];

	return psmi_ipv6_ntop(ipv6_addr, prefix_len, outstr, sizeof(outstrbufs[0]));
}

#if 0
socklen_t psmi_sockaddr_len(struct sockaddr* addr)
{
	switch (addr->sa_family) {
	case AF_INET:
		return (sizeof(struct sockaddr_in));
	case AF_INET6:
		return (sizeof(struct sockaddr_in6));
	default:
		// unknown
		return 0;	// be conservative
	}
}
#endif

#ifdef PSM_VERBS
// given an IPv4 address, figure out which ifconfig entry matches and
// return the netmask
int psm3_get_eth_ipv4_netmask(uint32_t ip_addr, uint32_t *netmask)
{
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) == 0) {
		for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
			struct sockaddr_in *addr = (struct sockaddr_in  *)ifa->ifa_addr;
			struct sockaddr_in *nmask = (struct sockaddr_in  *)ifa->ifa_netmask;
			uint32_t nm;

			if (!nmask) continue;
			if (addr->sin_family != AF_INET) continue;
			if (__be32_to_cpu(addr->sin_addr.s_addr) != ip_addr) continue;

			nm = __be32_to_cpu(nmask->sin_addr.s_addr);

			_HFI_DBG("Related ifaddr[%s]: %s netmask %s\n",
					ifa->ifa_name,
					psm3_ipv4_fmt(ip_addr, 0, 0),
					psm3_ipv4_fmt(nm, 0, 1));
			*netmask = nm;
			break;
		}
		(void)freeifaddrs(ifap);
	} else {
		return -1;
	}
	return 0;
}
#endif /* PSM_VERBS */

// used for IPv4 netmask processing.  A valid netmask has a sequence of 1s
// and then all other bits are 0.
// This counts how many 1s are in the high end of the netmask and confirms
// the remaining low bits are 0.
// returns 0 if netmask is invalid
uint8_t psm3_compute_ipv4_prefix_len(uint32_t netmask)
{
	int i=0;
	uint32_t mask = 0x80000000;
	while (mask & netmask) {
		i++; mask >>= 1;
	}
	// confirm all low bits of netmask are 0
	if (netmask != psmi_prefix_len_to_ipv4_netmask(i))
		return 0; // invalid
	return i;
}

static inline uint64_t psmi_bit_count_to_mask64(int count)
{
	return count ? (uint64_t)(0xffffffffffffffffULL << (64-count)) : 0;
}

// used for IPv6 netmask processing.  A valid netmask has a sequence of 1s
// and then all other bits are 0.
// This counts how many 1s are in the high end of the netmask and confirms
// the remaining low bits are 0.
// returns 0 if netmask is invalid
int psm3_compute_ipv6_prefix_len(psmi_bare_netaddr128_t netmask)
{
	int i=0;
	uint64_t mask = 0x8000000000000000;
	while (mask & netmask.hi) {
		i++; mask >>= 1;
	}
	if (i == 64) {
		mask = 0x8000000000000000;
		while (mask & netmask.lo) {
			i++; mask >>= 1;
		}
		// confirm all low bits of netmask are 0
		if (netmask.lo != psmi_bit_count_to_mask64(i-64))
			return 0; // invalid
		return i;
	} else {
		// confirm all low bits of netmask are 0
		if (netmask.hi != psmi_bit_count_to_mask64(i)
		    || netmask.lo != 0)
			return 0; // invalid
		return i;
	}
}

#ifdef PSM_VERBS
// given an IPv6 address, figure out which ifconfig entry matches and
// return the netmask
// returns 0 if a matching address was found
// returns -1 if can't query addresses or no matching address was found
int psm3_get_eth_ipv6_netmask(psmi_bare_netaddr128_t ipv6_addr,
				psmi_bare_netaddr128_t *netmask)
{
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) == 0) {
		for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
			struct sockaddr_in6 *addr = (struct sockaddr_in6  *)ifa->ifa_addr;
			struct sockaddr_in6 *nmask = (struct sockaddr_in6  *)ifa->ifa_netmask;
			uint32_t *s6;
			uint64_t addr_hi, addr_lo;

			if (!nmask) continue;
			if (addr->sin6_family != AF_INET6) continue;
			s6 = addr->sin6_addr.s6_addr32;
			addr_hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
			addr_lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);

			if (addr_hi != ipv6_addr.hi || addr_lo != ipv6_addr.lo) continue;

			s6 = nmask->sin6_addr.s6_addr32;
			netmask->hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
			netmask->lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);

			_HFI_DBG("Related ifaddr[%s]: %s netmask %s\n",
					ifa->ifa_name,
					psm3_ipv6_fmt(ipv6_addr, 0, 0),
					psm3_ipv6_fmt(*netmask, 0, 1));
			break;	// found, ifa != NULL
		}
		(void)freeifaddrs(ifap);
		return ifa ? 0 : -1;
	} else {
		return -1;
	}
}
#endif /* PSM_VERBS */

#if 0
// Find the 1st IPv6 address for a given netdev interface
// returns 0 if an address was found
// returns -1 if can't query addresses or no address was found
int psm3_get_eth_ipv6(const char *ifname, psmi_bare_netaddr128_t *ipv6_addr)
{
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) == 0) {
		for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
			struct sockaddr *addr = ifa->ifa_addr;
			uint32_t *s6;

			if (!addr) continue;
			if (addr->sa_family != AF_INET6) continue;
			if (strcmp(ifname, ifa->ifa_name)) continue;

			s6 = ((struct sockaddr_in6 *)addr)->sin6_addr.s6_addr32;
			ipv6_addr->hi = (uint64_t)__be32_to_cpu(s6[0]) << 32 | __be32_to_cpu(s6[1]);
			ipv6_addr->lo = (uint64_t)__be32_to_cpu(s6[2]) << 32 | __be32_to_cpu(s6[3]);

			_HFI_DBG("Found ifaddr[%s]: IPv6 %s\n",
					ifa->ifa_name,
					psm3_ipv6_fmt(*ipv6_addr, 0, 0));
			break;	// found, ifa != NULL
		}
		(void)freeifaddrs(ifap);
		return ifa ? 0 : -1;
	} else {
		return -1;
	}
}
#endif /* if 0 */

/* use for single rand num pick */
// if want to pick a series of rand numbers, please use/write different
// function that only init drand48_data once
long int psm3_rand(long int seed)
{
	struct drand48_data drand48_data;
	long int ret = 0;

	srand48_r(seed, &drand48_data);
	lrand48_r(&drand48_data, &ret);

	return ret;
}

/* Wrapper, in case we port to OS xyz that doesn't have sysconf */
uintptr_t psm3_getpagesize(void)
{
	static uintptr_t pagesz = (uintptr_t) 0;
	long sz;
	/* Size of a page in bytes.  Must not be less than 1. */
	if (pagesz >= (uintptr_t) 1)
		return pagesz;
	sz = sysconf(_SC_PAGESIZE);
	if (sz < 1) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Can't query system page size");
		return pagesz; /* assert above */
	}

	pagesz = (uintptr_t) sz;
	return pagesz;
}

/* _CONSUMED_ALL() is a macro which indicates if strtol() consumed all
   of the input passed to it. */
#define _CONSUMED_ALL(CHAR_PTR) (((CHAR_PTR) != NULL) && (*(CHAR_PTR) == 0))

/* parse env of the form 'val' or 'val:' or 'val:pattern'
 * for PSM3_VERBOSE_ENV and PSM3_IDENITFY
 * if nothing provided or doesn't match current process, def is returned
 * if syntax error, def_syntax is returned
 */
int psm3_parse_val_pattern(const char *env, int def, int def_syntax)
{
	int ret = def;

	if (env && *env) {
		char *e = psmi_strdup(NULL, env);
		char *ep;
		char *p;

		psmi_assert_always(e != NULL);
		if (e == NULL)	// for klocwork
			goto done;
		p = strchr(e, ':');
		if (p)
			*p = '\0';
		int val = (int)strtol(e, &ep, 0);
		if (! _CONSUMED_ALL(ep))
			ret = def_syntax;
		else
			ret = val;
		if (val && p) {
			if (! *(p+1)) { // val: -> val:*:rank0
				if (psm3_get_myrank() != 0)
					ret = def;
			} else if (0 != fnmatch(p+1, psm3_get_mylabel(),  0
#ifdef FNM_EXTMATCH
										| FNM_EXTMATCH
#endif
					))
					ret = def;
		}
		psmi_free(e);
	}
done:
	return ret;
}

/* If PSM3_VERBOSE_ENV is set in the environment, we determine
 * what its verbose level is and print the environment at "INFO"
 * level if the environment's level matches the desired printlevel.
 */
static int psmi_getenv_verblevel = -1;
static int psm3_getenv_is_verblevel(int printlevel)
{
	if (psmi_getenv_verblevel == -1) {
		char *env = getenv("PSM3_VERBOSE_ENV");
		int nlevel = PSMI_ENVVAR_LEVEL_USER;
		psmi_getenv_verblevel = psm3_parse_val_pattern(env, 0, 2);
		if (psmi_getenv_verblevel < 0 || psmi_getenv_verblevel > 3)
			psmi_getenv_verblevel = 2;
		if (psmi_getenv_verblevel > 0)
			nlevel = 0; /* output at INFO level */
		if (psmi_getenv_verblevel == 1)
			_HFI_ENVDBG(0, " %-25s => '%s' (default was '%s')\n",
				"PSM3_VERBOSE_ENV", env?env:"", "0");
		else if (env && *env)
			_HFI_ENVDBG(nlevel, " %-25s %-40s => '%s' (default was '%s')\n",
				"PSM3_VERBOSE_ENV",
				"Enable verbose output of environment variables. "
				"(0 - none, 1 - changed w/o help, 2 - user help, "
				"#: - limit output to rank 0, #:pattern - limit output "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
// don't document that 3 and 3: and 3:pattern can output hidden params
				env, "0");
		else	/* defaulted */
			_HFI_ENVDBG(nlevel,
				" %-25s %-40s => '%s'\n",
				"PSM3_VERBOSE_ENV",
				"Enable verbose output of environment variables. "
				"(0 - none, 1 - changed w/o help, 2 - user help, "
				"#: - limit output to rank 0, #:pattern - limit output "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
// don't document that 3 and 3: and 3:pattern can output hidden params
				"0");
	}
	return ((printlevel <= psmi_getenv_verblevel
			&& psmi_getenv_verblevel == 1)
		|| printlevel <= psmi_getenv_verblevel-1);
}

#define GETENV_PRINTF(_level, _fmt, ...)				\
	do {								\
		if ((_level & PSMI_ENVVAR_LEVEL_NEVER_PRINT) == 0)	\
		{							\
			int nlevel = _level;				\
			if (psm3_getenv_is_verblevel(nlevel))		\
				nlevel = 0; /* output at INFO level */	\
			_HFI_ENVDBG(nlevel, _fmt, ##__VA_ARGS__);	\
		}							\
	} while (0)

int
MOCKABLE(psm3_getenv)(const char *name, const char *descr, int level,
	    int type, union psmi_envvar_val defval,
	    union psmi_envvar_val *newval)
{
	int used_default = 0;
	union psmi_envvar_val tval;
	char *env = getenv(name);
#if _HFI_DEBUGGING
	int ishex = (type == PSMI_ENVVAR_TYPE_ULONG_FLAGS ||
		     type == PSMI_ENVVAR_TYPE_UINT_FLAGS);
#endif

	/* for verblevel 1 we only output non-default values with no help
	 * for verblevel>1 we promote to info (verblevel=2 promotes USER,
	 *		verblevel=3 promotes HIDDEN) and show help.
	 * for verblevel< 1 we don't promote anything and show help
	 */
#define _GETENV_PRINT(used_default, fmt, val, defval) \
	do {	\
		(void)psm3_getenv_is_verblevel(level);			\
		if (used_default && psmi_getenv_verblevel != 1)		\
			GETENV_PRINTF(level, "%s%-25s %-40s =>%s" fmt	\
				"\n", level > 1 ? "*" : " ", name,	\
				descr, ishex ? "0x" : " ", val);	\
		else if (! used_default && psmi_getenv_verblevel == 1)	\
			GETENV_PRINTF(1, "%s%-25s =>%s"			\
				fmt " (default was%s" fmt ")\n",	\
				level > 1 ? "*" : " ", name,		\
				ishex ? " 0x" : " ", val,		\
				ishex ? " 0x" : " ", defval);		\
		else if (! used_default && psmi_getenv_verblevel != 1)	\
			GETENV_PRINTF(1, "%s%-25s %-40s =>%s"		\
				fmt " (default was%s" fmt ")\n",	\
				level > 1 ? "*" : " ", name, descr,	\
				ishex ? " 0x" : " ", val,		\
				ishex ? " 0x" : " ", defval);		\
	} while (0)

#define _CONVERT_TO_NUM(DEST,TYPE,STRTOL)						\
	do {										\
		char *ep;								\
		/* Avoid base 8 (octal) on purpose, so don't pass in 0 for radix */	\
		DEST = (TYPE)STRTOL(env, &ep, 10);					\
		if (! _CONSUMED_ALL(ep)) {						\
			DEST = (TYPE)STRTOL(env, &ep, 16);				\
			if (! _CONSUMED_ALL(ep)) {					\
				used_default = 1;					\
				tval = defval;						\
			}								\
		}									\
	} while (0)

	switch (type) {
	case PSMI_ENVVAR_TYPE_YESNO:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else if (env[0] == 'Y' || env[0] == 'y')
			tval.e_int = 1;
		else if (env[0] == 'N' || env[0] == 'n')
			tval.e_int = 0;
		else {
			char *ep;
			tval.e_ulong = strtoul(env, &ep, 0);
			if (ep == env) {
				used_default = 1;
				tval = defval;
			} else if (tval.e_ulong != 0)
				tval.e_ulong = 1;
		}
		_GETENV_PRINT(used_default, "%s", tval.e_long ? "YES" : "NO",
			      defval.e_int ? "YES" : "NO");
		break;

	case PSMI_ENVVAR_TYPE_STR:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else
			tval.e_str = env;
		_GETENV_PRINT(used_default, "'%s'", tval.e_str, defval.e_str);
		break;

	case PSMI_ENVVAR_TYPE_INT:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else {
			_CONVERT_TO_NUM(tval.e_int,int,strtol);
		}
		_GETENV_PRINT(used_default, "%d", tval.e_int, defval.e_int);
		break;

	case PSMI_ENVVAR_TYPE_UINT:
	case PSMI_ENVVAR_TYPE_UINT_FLAGS:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else {
			_CONVERT_TO_NUM(tval.e_int,unsigned int,strtoul);
		}
		if (type == PSMI_ENVVAR_TYPE_UINT_FLAGS)
			_GETENV_PRINT(used_default, "%x", tval.e_uint,
				      defval.e_uint);
		else
			_GETENV_PRINT(used_default, "%u", tval.e_uint,
				      defval.e_uint);
		break;

	case PSMI_ENVVAR_TYPE_LONG:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else {
			_CONVERT_TO_NUM(tval.e_long,long,strtol);
		}
		_GETENV_PRINT(used_default, "%ld", tval.e_long, defval.e_long);
		break;
	case PSMI_ENVVAR_TYPE_ULONG_ULONG:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else {
			_CONVERT_TO_NUM(tval.e_ulonglong,unsigned long long,strtoull);
		}
		_GETENV_PRINT(used_default, "%llu",
			      tval.e_ulonglong, defval.e_ulonglong);
		break;
	case PSMI_ENVVAR_TYPE_ULONG:
	case PSMI_ENVVAR_TYPE_ULONG_FLAGS:
	default:
		if (!env || *env == '\0') {
			tval = defval;
			used_default = 1;
		} else {
			_CONVERT_TO_NUM(tval.e_ulong,unsigned long,strtoul);
		}
		if (type == PSMI_ENVVAR_TYPE_ULONG_FLAGS)
			_GETENV_PRINT(used_default, "%lx", tval.e_ulong,
				      defval.e_ulong);
		else
			_GETENV_PRINT(used_default, "%lu", tval.e_ulong,
				      defval.e_ulong);
		break;
	}
#undef _GETENV_PRINT
	*newval = tval;

	return used_default;
}
MOCK_DEF_EPILOGUE(psm3_getenv);

/*
 * Parsing long parameters
 * -1 -> parse error
 */
long psm3_parse_str_long(const char *string)
{
	char *ep;                               \
	long ret;

	if (! string || ! *string)
		return -1;
	/* Avoid base 8 (octal) on purpose, so don't pass in 0 for radix */
	ret = strtol(string, &ep, 10);
	if (! _CONSUMED_ALL(ep)) {
		ret = strtol(string, &ep, 16);
		if (! _CONSUMED_ALL(ep))
			return -1;
	}
	return ret;
}

/*
 * Parsing int parameters set in string tuples.
 * Output array int *vals should be able to store 'ntup' elements.
 * Values are only overwritten if they are parsed.
 * Tuples are always separated by colons ':'
 */
int psm3_parse_str_tuples(const char *string, int ntup, int *vals)
{
	char *b = (char *)string;
	char *e = b;
	int tup_i = 0;
	int n_parsed = 0;
	char *buf = psmi_strdup(NULL, string);
	psmi_assert_always(buf != NULL);

	while (*e && tup_i < ntup) {
		b = e;
		while (*e && *e != ':')
			e++;
		if (e > b) {	/* something to parse */
			char *ep;
			int len = e - b;
			long int l;
			strncpy(buf, b, len);
			buf[len] = '\0';
			l = strtol(buf, &ep, 0);
			if (ep != buf) {	/* successful conversion */
				vals[tup_i] = (int)l;
				n_parsed++;
			}
		}
		if (*e == ':')
			e++;	/* skip delimiter */
		tup_i++;
	}
	psmi_free(buf);
	return n_parsed;
}

/*
 * Memory footprint/usage mode.
 *
 * This can be used for debug or for separating large installations from
 * small/medium ones.  The default is to assume a medium installation.  Large
 * is not that much larger in memory footprint, but we make a conscious effort
 * an consuming only the amount of memory we need.
 */
int psm3_parse_memmode(void)
{
	union psmi_envvar_val env_mmode;
	int used_default =
	    psm3_getenv("PSM3_MEMORY", "Memory usage mode (min, normal or large)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"normal", &env_mmode);
	if (used_default || !strcasecmp(env_mmode.e_str, "normal"))
		return PSMI_MEMMODE_NORMAL;
	else if (!strcasecmp(env_mmode.e_str, "min"))
		return PSMI_MEMMODE_MINIMAL;
	else if (!strcasecmp(env_mmode.e_str, "large") ||
		 !strcasecmp(env_mmode.e_str, "big"))
		return PSMI_MEMMODE_LARGE;
	else {
		_HFI_PRDBG("PSM3_MEMORY env value %s unrecognized, "
			   "using 'normal' memory mode instead\n",
			   env_mmode.e_str);
		return PSMI_MEMMODE_NORMAL;
	}
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
// we need PSM3_GPUDIRECT config early to influence rdmamode defaults,
// MR Cache mode and whether we need to open RV.
// As such we don't check PSMI_HAL_CAP_GPUDIRECT flag here, but
// will check it after opening HAL and report a fatal error after open
// if PSM3_GPUDIRECT=1 is requested but HAL/RV does not support GPUDIRECT.
//
// These functions are later used to confirm and finalize config for
// ips_proto_init.

// value returned is 0/1 (disable/enable)
unsigned psmi_parse_gpudirect(void)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;

	psm3_getenv("PSM3_GPUDIRECT",
		"Use GPUDirect DMA and RDMA support to allow the NIC to directly read"
		" from the GPU for send DMA and write to the GPU for recv RDMA."
		" Also enable GPUDirect copy for more efficient CPU to/from GPU copies."
		" Requires rv module support.(default is disabled i.e. 0)",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		(union psmi_envvar_val)0, /* Disabled by default */
		&envval);

	saved = envval.e_uint;
	have_value = 1;
	return saved;
}

// value returned is limit >= 0, (0 disables GPUDIRECT Send RDMA)
// force permits HAL to parse this value prior to ips_proto
// without needing to set PSM_HAL_CAP_GPUDIRECT_RDMA first
unsigned psmi_parse_gpudirect_rdma_send_limit(int force)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	if (!force && !psmi_hal_has_cap(PSM_HAL_CAP_GPUDIRECT_RDMA)) {
		saved = 0;
		goto done;
	}
	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;

	/* Default send threshold for Gpu-direct set to UINT_MAX
 	 * (always use GPUDIRECT) */
	psm3_getenv("PSM3_GPUDIRECT_RDMA_SEND_LIMIT",
		    "GPUDirect RDMA feature on send side will be switched off for messages larger than limit.",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)UINT_MAX, &envval);

	saved = envval.e_uint;
done:
	have_value = 1;
	return saved;
}

// value returned is limit >= 0, (0 disables GPUDIRECT Recv RDMA)
// force permits HAL to parse this value prior to ips_proto
// without needing to set PSM_HAL_CAP_GPUDIRECT_RDMA first
unsigned psmi_parse_gpudirect_rdma_recv_limit(int force)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	if (!force && !psmi_hal_has_cap(PSM_HAL_CAP_GPUDIRECT_RDMA)) {
		saved = 0;
		goto done;
	}
	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;

	/* Default receive threshold for Gpu-direct set to UINT_MAX
 	 * (always use GPUDIRECT) */
	psm3_getenv("PSM3_GPUDIRECT_RDMA_RECV_LIMIT",
		    "GPUDirect RDMA feature on receive side will be switched off for messages larger than limit.",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)UINT_MAX, &envval);

	saved = envval.e_uint;
done:
	have_value = 1;
	return saved;
}
#endif // PSM_CUDA || PSM_ONEAPI

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
/* Size of RV GPU Cache - only used for PSM3_GPUDIRECT=1
 * otherwise returns 0
 */
unsigned psmi_parse_gpudirect_rv_gpu_cache_size(int reload)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	if (! reload && have_value)
		return saved;

	// RV defaults are sufficient for default PSM parameters
	// but for HALs with RDMA, if user adjusts ep->hfi_num_send_rdma or
	// mq->hfi_base_window_rv they also need to increase the cache size.
	// psm3_verbs_alloc_mr_cache will verify cache size is sufficient.
	// min size is (HFI_TF_NFLOWS + ep->hfi_num_send_rdma) *
	// chunk size (mq->hfi_base_window_rv after psmi_mq_initialize_params)
	if (PSMI_IS_GPU_ENABLED && psmi_parse_gpudirect() ) {
		psm3_getenv("PSM3_RV_GPU_CACHE_SIZE",
				"kernel space GPU cache size"
				" (MBs, 0 lets rv module decide) [0]",
				PSMI_ENVVAR_LEVEL_USER,
				PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)0, &envval);
		saved = envval.e_uint;
	} else {
		saved = 0;
	}
	have_value = 1;
	return saved;
}

#endif	// PSM_CUDA || PSM_ONEAPI

#ifdef PSM_HAVE_REG_MR
/* Send DMA Enable */
unsigned psm3_parse_senddma(void)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;
	if (!psmi_hal_has_cap(PSM_HAL_CAP_SDMA)) {
		saved = 0;
		goto done;
	}

	psm3_getenv("PSM3_SDMA",
		"UD send dma flags (0 disables send dma, 1 enables), default 0",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		(union psmi_envvar_val)0, &envval);
	saved = envval.e_uint;
done:
	have_value = 1;
	return saved;
}
#endif /* PSM_HAVE_REG_MR */

/* PSM3_IDENTIFY */
// we need in multiple places
int psm3_parse_identify(void)
{
	union psmi_envvar_val myenv;
	static int have_value;
	static int saved_identify;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved_identify;

	psm3_getenv("PSM3_IDENTIFY", "Identify PSM version being run "
				"(0 - disable, 1 - enable, 1: - limit output to rank 0, "
				"1:pattern - limit output "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
		    	PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		    	(union psmi_envvar_val)"0", &myenv);
	saved_identify = psm3_parse_val_pattern(myenv.e_str, 0, 0);
	have_value = 1;

	return saved_identify;
}

static
const char *psmi_memmode_string(int mode)
{
	psmi_assert(mode >= PSMI_MEMMODE_NORMAL && mode < PSMI_MEMMODE_NUM);
	switch (mode) {
	case PSMI_MEMMODE_NORMAL:
		return "normal";
	case PSMI_MEMMODE_MINIMAL:
		return "minimal";
	case PSMI_MEMMODE_LARGE:
		return "large";
	default:
		return "unknown";
	}
}

psm2_error_t
psm3_parse_mpool_env(const psm2_mq_t mq, int level,
		     const struct psmi_rlimit_mpool *rlim,
		     uint32_t *valo, uint32_t *chunkszo)
{
	uint32_t val;
	const char *env = rlim->env;
	int mode = mq->memmode;
	psm2_error_t err = PSM2_OK;
	union psmi_envvar_val env_val;

	psmi_assert_always(mode >= PSMI_MEMMODE_NORMAL
			   && mode < PSMI_MEMMODE_NUM);

	psm3_getenv(rlim->env, rlim->descr, rlim->env_level,
		    PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)rlim->mode[mode].obj_max, &env_val);

	val = env_val.e_uint;
	if (val < rlim->minval || val > rlim->maxval) {
		err = psm3_handle_error(NULL, PSM2_PARAM_ERR,
					"Env. var %s=%u is invalid (valid settings in mode PSM3_MEMORY=%s"
					" are inclusively between %u and %u)",
					env, val, psmi_memmode_string(mode),
					rlim->minval, rlim->maxval);
		goto fail;
	}

	_HFI_VDBG("%s max=%u,chunk=%u (mode=%s(%u),min=%u,max=%u)\n",
		  env, val, rlim->mode[mode].obj_chunk,
		  psmi_memmode_string(mode), mode, rlim->minval, rlim->maxval);

	*valo = val;
	*chunkszo = rlim->mode[mode].obj_chunk;

fail:
	return err;
}

void psm3_print_rank_identify(void)
{
	Dl_info info_psm;
	char ofed_delta[100] = "";
	static int identify_shown = 0;

	if (identify_shown)
		return;
	if (! psm3_parse_identify())
		return;

	identify_shown = 1;
	strcat(strcat(ofed_delta," built for IEFS OFA DELTA "),psm3_IEFS_version);
	printf("%s %s PSM3 v%d.%d%s%s\n"
		"%s %s location %s\n"
		"%s %s build date %s\n"
		"%s %s src checksum %s\n"
		"%s %s git checksum %s\n"
		"%s %s %s\n"
		"%s %s Global Rank %d (%d total) Local Rank %d (%d total)\n"
		"%s %s CPU Core %d NUMA %d PID %d\n",
		psm3_get_mylabel(), psm3_ident_tag,
			PSM2_VERNO_MAJOR,PSM2_VERNO_MINOR,
#ifdef PSM_CUDA
			"-cuda",
#elif defined(PSM_ONEAPI)
			"-oneapi-ze",
#else
			"",
#endif
			(strcmp(psm3_IEFS_version,"") != 0) ? ofed_delta : "",
		psm3_get_mylabel(), psm3_ident_tag,
			dladdr(psm3_init, &info_psm) ?
				info_psm.dli_fname : "PSM3 path not available",
		psm3_get_mylabel(), psm3_ident_tag, psm3_build_timestamp,
		psm3_get_mylabel(), psm3_ident_tag, psm3_sources_checksum,
		psm3_get_mylabel(), psm3_ident_tag,
			(strcmp(psm3_git_checksum,"") != 0) ?
				psm3_git_checksum : "<not available>",
		psm3_get_mylabel(), psm3_ident_tag, psmi_hal_identify(),
		psm3_get_mylabel(), psm3_ident_tag,
			psm3_get_myrank(), psm3_get_myrank_count(),
			psm3_get_mylocalrank(),
			psm3_get_mylocalrank_count(),
		psm3_get_mylabel(), psm3_ident_tag,
			sched_getcpu(), psm3_get_current_proc_location(), (int)getpid()
		);
}

void psm3_print_ep_identify(psm2_ep_t ep)
{
	int node_id;
	uint64_t link_speed=0;

	if (! psm3_parse_identify())
		return;

	if (! psm3_ep_device_is_enabled(ep, PTL_DEVID_IPS)) {
		if (psm3_ep_device_is_enabled(ep, PTL_DEVID_AMSH))
			printf("%s %s EP: shm\n",
				psm3_get_mylabel(), psm3_ident_tag);
		else
			printf("%s %s EP: self\n",
				psm3_get_mylabel(), psm3_ident_tag);
		return;
	}

	(void)psmi_hal_get_port_speed(ep->unit_id, ep->portnum, &link_speed);
	psmi_hal_get_node_id(ep->unit_id, &node_id);
	printf("%s %s NIC %u (%s) Port %u %"PRIu64" Mbps NUMA %d %s\n",
		psm3_get_mylabel(), psm3_ident_tag,
		ep->unit_id,  ep->dev_name,
		ep->portnum, link_speed/(1000*1000),
		node_id, psm3_epid_fmt_addr(ep->epid, 0));
}

uint64_t psm3_cycles_left(uint64_t start_cycles, int64_t timeout_ns)
{
	if (timeout_ns < 0)
		return 0ULL;
	else if (timeout_ns == 0ULL || timeout_ns == ~0ULL)
		return ~0ULL;
	else {
		uint64_t t_end = nanosecs_to_cycles(timeout_ns);
		uint64_t t_now = get_cycles() - start_cycles;

		if (t_now >= t_end)
			return 0ULL;
		else
			return (t_end - t_now);
	}
}

#define PSMI_EP_IS_PTR(ptr)	    ((ptr) != NULL && (ptr) < PSMI_EP_LOGEVENT)

void
psm3_syslog(psm2_ep_t ep, int to_console, int level, const char *format, ...)
{
	va_list ap;

	/* If we've never syslogged anything from this ep at the PSM level, make
	 * sure we log context information */
	if (PSMI_EP_IS_PTR(ep) && !ep->did_syslog) {
		char uuid_str[64];

		if (! psm3_epid_zero_internal(ep->epid))
			ep->did_syslog = 1;

		memset(&uuid_str, 0, sizeof(uuid_str));
		uuid_unparse(ep->uuid, uuid_str);
		psm3_syslog_internal("PSM", 0, LOG_WARNING,
				"%s: uuid_key=%s,unit=%d:%u,addr=%s",
				psm3_get_mylabel(), uuid_str, ep->unit_id, ep->portnum,
				psm3_epid_zero_internal(ep->epid)?"Unknown":
					psm3_epid_fmt_addr(ep->epid, PSM_MAX_BUFNO+1));
	}

	va_start(ap, format);
	psm3_vsyslog("PSM", to_console, level, format, ap);
	va_end(ap);
}

/* Table of CRCs of all 8-bit messages. */
static uint32_t crc_table[256];

/* Flag: has the table been computed? Initially false. */
static int crc_table_computed;

/* Make the table for a fast CRC. */
static void make_crc_table(void)
{
	uint32_t c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (uint32_t) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320 ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
 * should be initialized to all 1's, and the transmitted value
 * is the 1's complement of the final running CRC (see the
 * crc() routine below)).
 */

static uint32_t update_crc(uint32_t crc, unsigned char *buf, int len)
{
	uint32_t c = crc;
	int n;

	if_pf(!crc_table_computed)
	    make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
uint32_t psm3_crc(unsigned char *buf, int len)
{
	return update_crc(0xffffffff, buf, len) ^ 0xffffffff;
}

int psm3_multi_ep_enabled = 0;
void psm3_multi_ep_init()
{
	union psmi_envvar_val env_fi;

	psm3_getenv("PSM3_MULTI_EP", "Allow Open of Multiple Endpoints per process (yes/no)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_YESNO,
		    PSMI_ENVVAR_VAL_YES, &env_fi);

	psm3_multi_ep_enabled = env_fi.e_uint;
}

#ifdef PSM_FI

int psm3_faultinj_enabled = 0;
int psm3_faultinj_verbose = 0;
char *psm3_faultinj_outfile = NULL;
int psm3_faultinj_sec_rail = 0;

static struct psm3_faultinj_spec psm3_faultinj_dummy;
static STAILQ_HEAD(, psm3_faultinj_spec) psm3_faultinj_head =
		STAILQ_HEAD_INITIALIZER(psm3_faultinj_head);
int psm3_faultinj_num_entries;

void psm3_faultinj_init()
{
	union psmi_envvar_val env_fi;

	psm3_getenv("PSM3_FI", "PSM Fault Injection "
				"(0 - disable, 1 - enable, 1: - limit to rank 0, "
				"1:pattern - limit "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)"0", &env_fi);
	psm3_faultinj_enabled = psm3_parse_val_pattern(env_fi.e_str, 0, 0);

	if (psm3_faultinj_enabled) {
		char *def = NULL;
		if (!psm3_getenv
		    ("PSM3_FI_TRACEFILE", "PSM Fault Injection output file",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
		     (union psmi_envvar_val)def, &env_fi)) {
			psm3_faultinj_outfile = psmi_strdup(NULL, env_fi.e_str);
		}
		if (!psm3_getenv
		    ("PSM3_FI_VERBOSE", "PSM Fault verbose output",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		     (union psmi_envvar_val)0, &env_fi)) {
			psm3_faultinj_verbose = env_fi.e_int;
		}
		if (!psm3_getenv
		    ("PSM3_FI_RAIL", "PSM Fault Injection rail (0=all, 1=secondary only)",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		     (union psmi_envvar_val)0, &env_fi)) {
			psm3_faultinj_sec_rail = env_fi.e_int;
		}
	}

	return;
}

/* we only grow new entries, so if we fail to allocate, just ignore request */
static void psm3_faultinj_reregister_stats()
{
	struct psmi_stats_entry *entries;
	struct psmi_stats_entry *e;
	int num_entries = 0;
	struct psm3_faultinj_spec *fi;

	entries = psmi_calloc(PSMI_EP_NONE, STATS, psm3_faultinj_num_entries,
			 		sizeof(struct psmi_stats_entry));
	if (! entries)
		return;
	e = entries;
	STAILQ_FOREACH(fi, &psm3_faultinj_head, next) {
		psmi_stats_init_u64(e, fi->spec_name, &fi->num_faults);
		e++; num_entries++;
	}

	psm3_stats_reregister_type("Fault_Injection", PSMI_STATSTYPE_FAULTINJ,
		entries, num_entries, NULL, &psm3_faultinj_head, NULL);
	psmi_free(entries);
}

void psm3_faultinj_fini()
{
	struct psm3_faultinj_spec *fi;
	FILE *fp;
	int do_fclose = 0;

	if (!psm3_faultinj_enabled)
		return;
	psm3_stats_deregister_type(PSMI_STATSTYPE_FAULTINJ, &psm3_faultinj_head);

	if (psm3_faultinj_outfile == NULL)
		return;
	if (strncmp(psm3_faultinj_outfile, "stdout", 7) == 0)
		fp = stdout;
	else if (strncmp(psm3_faultinj_outfile, "stderr", 7) == 0)
		fp = stderr;
	else {
		char *c = psm3_faultinj_outfile;
		char buf[192];
		int append = 0;
		if (*c == '+') {
			append = 1;
			++c;
		}
		do_fclose = 1;
		snprintf(buf, sizeof(buf), "%s.%s", c, psm3_get_mylabel());
		fp = fopen(buf, append ? "a" : "w");
	}

	if (fp != NULL) {
		STAILQ_FOREACH(fi, &psm3_faultinj_head, next) {
			fprintf(fp, "%s:%s PSM3_FI_%-13s %2.3f%% => "
				"%2.3f%% %10"PRIu64" faults/%10"PRIu64" events seed %10ld\n",
				__progname, psm3_get_mylabel(), fi->spec_name,
				(double)fi->num * 100.0 / fi->denom,
				(fi->num_calls ?
				(double)fi->num_faults * 100.0 / fi->num_calls
				:(double)0.0),
				fi->num_faults, fi->num_calls,
				fi->initial_seed);
		}
		fflush(fp);
		if (do_fclose)
			fclose(fp);
	}

	psmi_free(psm3_faultinj_outfile);
	return;
}

/*
 * Intended to be used only once, not in the critical path
 */
struct psm3_faultinj_spec *psm3_faultinj_getspec(const char *spec_name,
						 const char *help, int num,
						 int denom)
{
	struct psm3_faultinj_spec *fi;

	if (!psm3_faultinj_enabled)
		return &psm3_faultinj_dummy;

	STAILQ_FOREACH(fi, &psm3_faultinj_head, next) {
		if (strcmp(fi->spec_name, spec_name) == 0)
			return fi;
	}

	/* We got here, so no spec -- allocate one */
	fi = psmi_malloc(PSMI_EP_NONE, UNDEFINED,
			 sizeof(struct psm3_faultinj_spec));
	psmi_assert_always(fi != NULL);
	strncpy(fi->spec_name, spec_name, PSM3_FAULTINJ_SPEC_NAMELEN - 1);
	fi->spec_name[PSM3_FAULTINJ_SPEC_NAMELEN - 1] = '\0';
	fi->num = num;
	fi->denom = denom;
	fi->initial_seed = getpid();
	fi->num_faults = 0;
	fi->num_calls = 0;

	/*
	 * See if we get a hint from the environment.
	 * Format is
	 * <num:denom:initial_seed>
	 *
	 * By default, we chose the initial seed to be the 'pid'.  If users need
	 * repeatability, they should set initial_seed to be the 'pid' when the
	 * error was observed or force the initial_seed to be a constant number in
	 * each running process.  Using 'pid' is useful because core dumps store
	 * pids and our backtrace format does as well so if a crash is observed for
	 * a specific seed, programs can reuse the 'pid' to regenerate the same
	 * error condition.
	 */
	{
		int fvals[3] = { num, denom, (int)getpid() };
		union psmi_envvar_val env_fi;
		char fvals_str[128];
		char fname[128];
		char fdesc[300];

		snprintf(fvals_str, sizeof(fvals_str), "%d:%d:1", num,
			 denom);
		snprintf(fname, sizeof(fname), "PSM3_FI_%s", spec_name);
		snprintf(fdesc, sizeof(fdesc), "Fault Injection - %s <%s>",
			 help, fvals_str);

		if (!psm3_getenv(fname, fdesc, PSMI_ENVVAR_LEVEL_HIDDEN,
				 PSMI_ENVVAR_TYPE_STR,
				 (union psmi_envvar_val)fvals_str, &env_fi)) {
			/* not using default values */
			int n_parsed =
			    psm3_parse_str_tuples(env_fi.e_str, 3, fvals);
			if (n_parsed >= 1)
				fi->num = fvals[0];
			if (n_parsed >= 2)
				fi->denom = fvals[1];
			if (n_parsed >= 3)
				fi->initial_seed = (long int)fvals[2];
		}
	}
	srand48_r(fi->initial_seed, &fi->drand48_data);

	psm3_faultinj_num_entries++;
	STAILQ_INSERT_TAIL(&psm3_faultinj_head, fi, next);
	psm3_faultinj_reregister_stats();
	return fi;
}

int psm3_faultinj_is_fault(struct psm3_faultinj_spec *fi, psm2_ep_t ep)
{
	if (!psm3_faultinj_enabled)	/* never fault if disabled */
		return 0;
	if (fi->num == 0)
		return 0;

	if (!psmi_hal_faultinj_allowed(fi->spec_name, ep))
		return 0;
	fi->num_calls++;
	long int rnum;
	lrand48_r(&fi->drand48_data, &rnum);
	if (((int) (rnum % INT_MAX)) % fi->denom <= fi->num) {
		fi->num_faults++;
		return 1;
	} else
		return 0;
}

#endif /* #ifdef PSM_FI */

/* For memory allocation, we kind of break the PSM error handling rules.
 * If the caller gets NULL, it has to assume that the error has been handled
 * and should always return PSM2_NO_MEMORY */

/*
 * Log memory increments or decrements of type memstats_t.
 */
struct psmi_memtype_hdr {
	struct {
		uint64_t size:48;
		uint64_t magic:8;
		uint64_t type:8;
	};
	void *original_allocation;
};

// Memory stats will only be collected under debug builds

#ifdef PSM_DEBUG
#define psmi_stats_mask PSMI_STATSTYPE_MEMORY
#else
#define psmi_stats_mask 0
#endif

struct psmi_stats_malloc psm3_stats_memory;

void psm3_mem_stats_register(void)
{
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECLU64("Total_(current)",
				(uint64_t*)&psm3_stats_memory.m_all_total),
		PSMI_STATS_DECLU64("Total_(max)",
				(uint64_t*)&psm3_stats_memory.m_all_max),
		PSMI_STATS_DECLU64("All_Peers_(current)",
				(uint64_t*)&psm3_stats_memory.m_perpeer_total),
		PSMI_STATS_DECLU64("All_Peers_(max)",
				(uint64_t*)&psm3_stats_memory.m_perpeer_max),
		PSMI_STATS_DECLU64("Network_Buffers_(current)",
				(uint64_t*)&psm3_stats_memory.m_netbufs_total),
		PSMI_STATS_DECLU64("Network_Buffers_(max)",
				(uint64_t*)&psm3_stats_memory.m_netbufs_max),
		PSMI_STATS_DECLU64("PSM_desctors_(current)",
				(uint64_t*)&psm3_stats_memory.m_descriptors_total),
		PSMI_STATS_DECLU64("PSM_desctors_(max)",
				(uint64_t*)&psm3_stats_memory.m_descriptors_max),
		PSMI_STATS_DECLU64("Unexp._buffers_(current)",
				(uint64_t*)&psm3_stats_memory.m_unexpbufs_total),
		PSMI_STATS_DECLU64("Unexp._Buffers_(max)",
				(uint64_t*)&psm3_stats_memory.m_unexpbufs_max),
#ifdef PSM_HAVE_RNDV_MOD
		PSMI_STATS_DECLU64("Peer_Rndv_(current)",
				(uint64_t*)&psm3_stats_memory.m_peerrndv_total),
		PSMI_STATS_DECLU64("Peer_Rndv_(max)",
				(uint64_t*)&psm3_stats_memory.m_peerrndv_max),
#endif
		PSMI_STATS_DECLU64("statistics_(current)",
				(uint64_t*)&psm3_stats_memory.m_stats_total),
		PSMI_STATS_DECLU64("statistics_(max)",
				(uint64_t*)&psm3_stats_memory.m_stats_max),
		PSMI_STATS_DECLU64("Other_(current)",
				(uint64_t*)&psm3_stats_memory.m_undefined_total),
		PSMI_STATS_DECLU64("Other_(max)",
				(uint64_t*)&psm3_stats_memory.m_undefined_max),
	};

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		psm3_stats_register_type("PSM_memory_allocation_statistics",
                    PSMI_STATSTYPE_MEMORY,
                    entries,
                    PSMI_HOWMANY(entries), NULL, &psm3_stats_memory, NULL);
	}
}


void psm3_log_memstats(psmi_memtype_t type, int64_t nbytes)
{
#define _add_max_total(type, nbytes)				\
	psm3_stats_memory.m_ ## type ## _total += (nbytes);	\
	psm3_stats_memory.m_ ## type ## _max = max(		\
	    psm3_stats_memory.m_ ## type ## _total,		\
	    psm3_stats_memory.m_ ## type ## _max);

	switch (type) {
	case PER_PEER_ENDPOINT:
		_add_max_total(perpeer, nbytes);
		break;
	case NETWORK_BUFFERS:
		_add_max_total(netbufs, nbytes);
		break;
	case DESCRIPTORS:
		_add_max_total(descriptors, nbytes);
		break;
	case UNEXPECTED_BUFFERS:
		_add_max_total(unexpbufs, nbytes);
		break;
	case STATS:
		_add_max_total(stats, nbytes);
		break;
#ifdef PSM_HAVE_RNDV_MOD
	case PEER_RNDV:
		_add_max_total(peerrndv, nbytes);
		break;
#endif
	case UNDEFINED:
		_add_max_total(undefined, nbytes);
		break;
	default:
		psmi_assert_always(type == TOTAL);
		break;
	}
	_add_max_total(all, nbytes);
	psm3_stats_memory.m_all_max++;
#undef _add_max_total

	return;
}

#ifdef malloc
#undef malloc
#endif

#ifdef PSM_HEAP_DEBUG

/* PSM HEAP DEBUG documentation:

   In the following code, the acronym: 'HD' is short for "Heap Debug".

   Each actual heap allocation will have a header and a trailer surrounding it,
   and the header itself may have some vacant space preceding it due to alignment
   needs:

   0. This area is the actual return value of posix_memalign and is due to
      alignment requirements.  (This area does not exist for heap allocations
      from malloc()).
   1. HD HEADER
   2. Actual allocation
   3. HD TRAILER

   malloc() / posix_memalign returns area 0 through 3 to the Heap Debug (HD) code,
   then the HD code writes areas 1 and 3, and then returns a pointer to area 2 to
   the caller.  Thereafter, the HD code will inspect areas 1 and 3 of all heap
   allocations to make sure they have retained their integrity.

   Surrounding the actual allocation like this enables:

   1. Checking for heap overrun / underrun of all allocations.
   2. Checking for double frees.
   3. Use of an area that has been freed.
   4. Identifying orphaned heap allocations.

Constant no-mans-land written to areas that no-one should be writing to:

 */

#define HD_NO_MANS_LAND -15

/*   The following is the declaration of the HD header. */

/* Heap debug header magic number type: */
typedef char HD_Hdr_Magic_Type[8];

typedef struct HD_Header_Struct
{
	HD_Hdr_Magic_Type        magic1;         /* Magic number to ensure this
						    allocation has integrity.
						    (guards against heap
						    overrun from above). */
	const char              *allocLoc;       /* Source file name/line
						    number where this heap
						    allocation was made. */
	const char              *freeLoc;        /* Source filename/line number
						    where this heap allocation
						    was freed. */
	struct HD_Header_Struct *nextHD_header;  /* Creates a singly-linked
						    list of all heap
						    allocations. */
	uint64_t                 sizeOfAlloc;    /* size of this heap
						    allocation. */
	void                    *systemAlloc;    /* The actual return value
						    from malloc()/posix_memaligh(). */
	uint64_t                 systemAllocSize;/* The size that is actually allocated
						    by malloc()/posix_memalign(). */
	HD_Hdr_Magic_Type        magic2;         /* Second magic number to
						    ensure this allocation
						    has integrity.
						    (guards against heap
						    underrun from the actual
						    allocation that follows). */
} __attribute__ ((packed)) HD_Header_Type;

typedef struct HD_free_list_struct
{
	HD_Header_Type *freedStuct;
	struct HD_free_list_struct *next_free_struct;
} HD_Free_Struct_Type;

static HD_Free_Struct_Type  *HD_free_list_root   = NULL;
static HD_Free_Struct_Type **HD_free_list_bottom = &HD_free_list_root;

typedef char HD_Trlr_Magic_Type[16];

static const HD_Hdr_Magic_Type  HD_HDR_MGC_1 = "Eric";
static const HD_Hdr_Magic_Type  HD_HDR_MGC_2 = "Emily";
static const HD_Trlr_Magic_Type HD_TRLR_MGC  = "Erin&Elaine";

/* Convert a pointer of an actual allocation to a pointer to its HD header: */
static inline HD_Header_Type *HD_AA_TO_HD_HDR(void *aa)
{
	char *p = (char*)aa;
	return (HD_Header_Type*)(p - sizeof(HD_Header_Type));
}

/* Convert a pointer to an HD header to the actual allocation: */
static inline void *HD_HDR_TO_AA(HD_Header_Type *phdHdr)
{
	char *p = (char*)phdHdr;
	return p + sizeof(HD_Header_Type);
}

/* Get the address of the trailer that follows the actual allocation: */
static inline void *HD_GET_HD_TRLR(HD_Header_Type *phdr)
{
	char *p = (char*)HD_HDR_TO_AA(phdr);
	return p + phdr->sizeOfAlloc;
}

static HD_Header_Type * HD_root_of_list = NULL;   /* Root of singly linked list
						     of all heap allocations */
static HD_Header_Type **HD_end_of_list = &HD_root_of_list;  /* Pointer to the
	       last pointer of the singly linked list of all heap allocations. */

/* Number of allocations in the list.  Maintained to assert the integrity
   of the singly linked list of heap allocations. */
static int n_allocations = 0;

/* HD_check_one_struct() checks one heap allocation for integrity. */
static inline void HD_check_one_struct(HD_Header_Type *p, int checkAA,const char *curloc)
{
	int s=0;

	/* First check the magic values in the header and trailer: */
	s |= memcmp(p->magic1,HD_HDR_MGC_1,sizeof(HD_HDR_MGC_1))       ? 1 : 0;
	s |= memcmp(p->magic2,HD_HDR_MGC_2,sizeof(HD_HDR_MGC_2))       ? 2 : 0;
	s |= memcmp(HD_GET_HD_TRLR(p),HD_TRLR_MGC,sizeof(HD_TRLR_MGC)) ? 4 : 0;

	if (s != 0)
	{
		fprintf(stderr,"header/trailer error: checking location: %s, s: %d, p: %p, "
			"p->allocLoc: %s\n",curloc,s,p,p->allocLoc);
		fprintf(stderr,"actual allocation starts at: %p, length: %" PRIu64  "\n", (char*)HD_HDR_TO_AA(p),p->sizeOfAlloc);
		fflush(0);
		abort();
	}

	/* Next, check the area between systemAlloc and the start of the header */
	signed char *pchr = (signed char *)p->systemAlloc;
	while (pchr < (signed char*)p)
	{
		psmi_assert_always(*pchr == (signed char) HD_NO_MANS_LAND);
		pchr++;
	}

	/* Lastly, check the actual allocation area if directed to do so: */
	if (checkAA)
	{
		uint64_t i;
		signed char *pchr = HD_HDR_TO_AA(p);
		for (i=0;i < p->sizeOfAlloc;i++)
			if (pchr[i] != (signed char) HD_NO_MANS_LAND)
			{
				fprintf(stderr,
					"use after free; ptr: %p,\n"
					" allocated from: %s,\n"
					" validated from: %s\n"
					" freed from: %s\n",
					pchr+i,p->allocLoc,curloc,p->freeLoc);
				fflush(0);
				psmi_assert_always(0);
			}
	}
}

/* _psmi_heapdebug_val_heapallocs() walks the singly linked list and inspects all
 *  heap allocations to ensure all of them have integrity still. */
void _psmi_heapdebug_val_heapallocs(const char *curloc)
{
	/* first check current allocation list: */
	HD_Header_Type *p = HD_root_of_list;
	int cnt = 0;

	while (p)
	{
		HD_check_one_struct(p,0,curloc);
		p = p->nextHD_header;
		cnt++;
	}
	psmi_assert_always(cnt == n_allocations);
	/* Next check free list */
	HD_Free_Struct_Type *pfreestruct = HD_free_list_root;
	while (pfreestruct)
	{
		HD_check_one_struct(pfreestruct->freedStuct,1,curloc);
		pfreestruct = pfreestruct->next_free_struct;
	}
}

/* psmi_heapdebug_finalize() validates the heap and then emits all of the allocations to stdout.
   to help debug heap memory leaks. */
void psmi_heapdebug_finalize(void)
{
	/* First validate the existing heap allocations: */

	psmi_heapdebug_val_heapallocs();

	printf("orphaned heap allocations: %d\n", n_allocations);

	if (n_allocations > 0)
	{
		/* Now, emit all of the alloations to stdout. */

		HD_Header_Type *p = HD_root_of_list;

		while (p)
		{
			printf("orphaned heap allocation: %p allocated at: %s, size: %lu\n",
			       p, p->allocLoc, p->sizeOfAlloc);

			p = p->nextHD_header;
		}
		fflush(0);
		/* Abort if any allocations still exist: */
		abort();
	}
}

/* hd_est_hdr_trlr() establishes the new allocation to the singly linked list, and adds
 * the header and trailer to the allocation.  Lastly, it validates the existing singly-linked
 * list for integrity. */
static void hd_est_hdr_trlr(HD_Header_Type *hd_alloc,
			    void *systemAlloc,
			    uint64_t systemSize,
			    uint64_t actualSize,
			    const char *curloc)
{
	/* First, write HD_NO_MANS_LAND to the entire allocation: */
	memset(systemAlloc,HD_NO_MANS_LAND,systemSize);

	/* Write the HD header info: */
	memcpy(hd_alloc->magic1,HD_HDR_MGC_1,sizeof(HD_HDR_MGC_1));
	hd_alloc->allocLoc = curloc;
	hd_alloc->freeLoc = NULL;
	hd_alloc->nextHD_header = NULL;
	hd_alloc->sizeOfAlloc = actualSize;
	hd_alloc->systemAlloc = systemAlloc;
	hd_alloc->systemAllocSize = systemSize;
	memcpy(hd_alloc->magic2,HD_HDR_MGC_2,sizeof(HD_HDR_MGC_2));
	memcpy(HD_GET_HD_TRLR(hd_alloc),HD_TRLR_MGC,sizeof(HD_TRLR_MGC));
	*HD_end_of_list = hd_alloc;
	HD_end_of_list = &hd_alloc->nextHD_header;
	n_allocations++;
	psmi_heapdebug_val_heapallocs();
}

/* hd_malloc() is the heap debug version of malloc that will create the header and trailer
 * and link the allocation into the singly linked list. */
static inline void *hd_malloc(size_t sz, const char *curloc)
{
	const uint64_t wholeSize = sizeof(HD_Header_Type) + sz + sizeof(HD_TRLR_MGC);
	HD_Header_Type *hd_alloc = (HD_Header_Type*)malloc(wholeSize);

	hd_est_hdr_trlr(hd_alloc,hd_alloc,wholeSize,sz,curloc);
	return HD_HDR_TO_AA(hd_alloc);
}

/* hd_memalign() is the heap debug version of posix_memalign(). */
static inline int hd_memalign(void **ptr,uint64_t alignment, size_t sz, const char *curloc)
{
	void *systemAlloc = NULL;
	const uint64_t alignMask = alignment - 1;
	uint64_t systemSize = sizeof(HD_Header_Type) + alignMask + sz + sizeof(HD_TRLR_MGC);
	int rv = posix_memalign(&systemAlloc,alignment,systemSize);
	char *actualAlloc = NULL;
	const char *endOfSystemAlloc = ((char*)systemAlloc) + systemSize;

	if (rv)
		return rv;

	uint64_t actualAllocu64 = (uint64_t) systemAlloc;
	actualAllocu64 += sizeof(HD_Header_Type) + alignMask;
	actualAllocu64 &= ~ alignMask;
	actualAlloc = (char*)actualAllocu64;
	psmi_assert_always((actualAllocu64 & alignMask) == 0);
	psmi_assert_always((actualAlloc+sz+sizeof(HD_TRLR_MGC)) <= endOfSystemAlloc);
	psmi_assert_always((actualAlloc - (char*)systemAlloc) >= sizeof(HD_Header_Type));

	hd_est_hdr_trlr(HD_AA_TO_HD_HDR(actualAlloc),systemAlloc,systemSize,sz,curloc);
	*ptr = actualAlloc;
	return rv;
}

/* hd_free() is the heap debug version of free().  First, hd_free() ensures that the ptr to be
 * freed in fact is known by the HD code.  Next, hd_free() removes the ptr from the list. Then,
 * hd_free scribbles to the ptr's area and actually frees the heap space. */
static inline void hd_free(void *ptr,const char *curloc)
{
	HD_Header_Type *hd_alloc = HD_AA_TO_HD_HDR(ptr);
	HD_Header_Type *p = HD_root_of_list, *q = NULL;

	psmi_heapdebug_val_heapallocs();
	while (p)
	{
		if (p == hd_alloc)
		{
			/* first, fix the next pointers: */
			if (q)
			{
				q->nextHD_header = p->nextHD_header;
			}
			else
			{
				psmi_assert_always(p == HD_root_of_list);
				HD_root_of_list = p->nextHD_header;
			}
			/* Now, handle the case of removing the last entry in the list. */
			if (&p->nextHD_header == HD_end_of_list)
			{
				if (q)
				{
					q->nextHD_header = NULL;
					HD_end_of_list = &q->nextHD_header;
				}
				else
				{
					HD_root_of_list = NULL;
					HD_end_of_list = &HD_root_of_list;
				}
			}
			/* Scribble to the actual allocation to make further access to the heap
			   area unusable. */
			n_allocations--;
			memset(HD_HDR_TO_AA(hd_alloc),HD_NO_MANS_LAND,hd_alloc->sizeOfAlloc);
			hd_alloc->freeLoc = curloc;
			/* Add this allocation to the free list. */
			HD_Free_Struct_Type *pfreestruct = (HD_Free_Struct_Type*)malloc(sizeof(HD_Free_Struct_Type));
			*HD_free_list_bottom = pfreestruct;
			HD_free_list_bottom = &pfreestruct->next_free_struct;
			pfreestruct->freedStuct = hd_alloc;
			pfreestruct->next_free_struct = NULL;
			psmi_heapdebug_val_heapallocs();
			return;
		}
		q = p;
		p = p->nextHD_header;
	}
	/* trying to free a heap allocation that we did not allocate. */
	psmi_assert_always(0);
}

size_t hd_malloc_usable_size(void *ptr,const char *curloc)
{
	HD_Header_Type *hd_alloc = HD_AA_TO_HD_HDR(ptr);
	return hd_alloc->systemAllocSize;
}

#endif

#ifdef PSM_HEAP_DEBUG

/* For HD code, we retarget the malloc, memaligh and free calls to the hd versions
 * of the code. */

#define my_malloc(SZ,CURLOC)              hd_malloc(SZ,CURLOC)
#define my_memalign(PTR,ALIGN,SZ,CURLOC)  hd_memalign(PTR,ALIGN,SZ,CURLOC)
#define my_free(PTR,CURLOC)               hd_free(PTR,CURLOC)
#define my_malloc_usable_size(PTR,CURLOC) hd_malloc_usable_size(PTR,CURLOC)

#else

/* For non-HD code, we target the code to the usual functions: */
#define my_malloc(SZ,CURLOC)              malloc(SZ)
#define my_memalign(PTR,ALIGN,SZ,CURLOC)  posix_memalign(PTR,ALIGN,SZ)
#define my_free(PTR,CURLOC)               free(PTR)
#define my_malloc_usable_size(PTR,CURLOC) malloc_usable_size(PTR)

#endif

void *psm3_malloc_internal(psm2_ep_t ep, psmi_memtype_t type,
			   size_t sz, const char *curloc)
{
	size_t newsz = sz;
	void *newa;

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY)
	    newsz += sizeof(struct psmi_memtype_hdr);

	newa = my_malloc(newsz,curloc);
	if (newa == NULL) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				  "Out of memory for malloc at %s", curloc);
		return NULL;
	}

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		struct psmi_memtype_hdr *hdr = (struct psmi_memtype_hdr *)newa;
		hdr->size = newsz;
		hdr->type = type;
		hdr->magic = 0x8c;
		hdr->original_allocation = newa;
		psm3_log_memstats(type, newsz);
		newa = (void *)(hdr + 1);
		/* _HFI_INFO("alloc is %p\n", newa); */
	}
	return newa;
}

void *psm3_realloc_internal(psm2_ep_t ep, psmi_memtype_t type,
			    void *ptr, size_t nsz, const char *curloc)
{
	if (ptr)
	{
		size_t existingSize = psm3_malloc_usable_size_internal(ptr,curloc);
		if (nsz > existingSize)
		{
			void *newPtr = psm3_malloc_internal(ep,type,nsz,curloc);

			memcpy(newPtr,ptr,existingSize);
			psm3_free_internal(ptr,curloc);
			return newPtr;
		}
		else
			/* We will not support shrinking virtual space
			   for performance reasons. */
			return ptr;
	}
	else
		return psm3_malloc_internal(ep,type,nsz,curloc);
}

#ifdef memalign
#undef memalign
#endif
void *psm3_memalign_internal(psm2_ep_t ep, psmi_memtype_t type,
			     size_t alignment, size_t sz, const char *curloc)
{
	size_t newsz = sz;
	void *newa;
	int ret, preambleSize = 0;

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY)
	{
		if (sizeof(struct psmi_memtype_hdr) > alignment)
		{
			int n = sizeof(struct psmi_memtype_hdr) / alignment;
			int r = sizeof(struct psmi_memtype_hdr) % alignment;
			if (r)
				n++;
			preambleSize = n * alignment;
		}
		else
			preambleSize = alignment;
		newsz += preambleSize;
	}

	ret = my_memalign(&newa, alignment, newsz, curloc);
	if (ret) {
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				  "Out of memory for malloc at %s", curloc);
		return NULL;
	}

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		void *rv = (void *)((uint8_t *)newa + preambleSize);
		struct psmi_memtype_hdr *hdr = (struct psmi_memtype_hdr *)((uint8_t *)rv - sizeof(struct psmi_memtype_hdr));
		hdr->size = newsz;
		hdr->type = type;
		hdr->magic = 0x8c;
		hdr->original_allocation = newa;
		psm3_log_memstats(type, newsz);
		newa = rv;
		/* _HFI_INFO("alloc is %p\n", newa); */
	}
	return newa;
}

#ifdef calloc
#undef calloc
#endif

void *psm3_calloc_internal(psm2_ep_t ep, psmi_memtype_t type, size_t nelem,
			   size_t elemsz, const char *curloc)
{
	void *newa = psm3_malloc_internal(ep, type, nelem * elemsz, curloc);
	if (newa == NULL)	/* error handled above */
		return NULL;
	memset(newa, 0, nelem * elemsz);
	return newa;
}

#ifdef strdup
#undef strdup
#endif

void *psm3_strdup_internal(psm2_ep_t ep, const char *string, const char *curloc)
{
	size_t len = strlen(string) + 1;
	void *newa = psm3_malloc_internal(ep, UNDEFINED, len, curloc);
	if (newa == NULL)
		return NULL;
	memcpy(newa, string, len);	/* copy with \0 */
	return newa;
}

#ifdef free
#undef free
#endif

void MOCKABLE(psm3_free_internal)(void *ptr,const char *curloc)
{
	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		struct psmi_memtype_hdr *hdr =
		    (struct psmi_memtype_hdr *)ptr - 1;
		/* _HFI_INFO("hdr is %p, ptr is %p\n", hdr, ptr); */
		psmi_memtype_t type = hdr->type;
		int64_t size = hdr->size;
		int magic = (int)hdr->magic;
		psm3_log_memstats(type, -size);
		psmi_assert_always(magic == 0x8c);
		ptr = hdr->original_allocation;
	}
	my_free(ptr,curloc);
}
MOCK_DEF_EPILOGUE(psm3_free_internal);

#ifdef malloc_usable_size
#undef malloc_usable_size
#endif

size_t psm3_malloc_usable_size_internal(void *ptr, const char *curLoc)
{
	return my_malloc_usable_size(ptr,curLoc);
}

PSMI_ALWAYS_INLINE(
psm2_error_t
psmi_coreopt_ctl(const void *core_obj, int optname,
		 void *optval, uint64_t *optlen, int get))
{
	psm2_error_t err = PSM2_OK;

	switch (optname) {
	case PSM2_CORE_OPT_DEBUG:
		/* Sanity check length */
		if (*optlen < sizeof(unsigned)) {
			err =  psm3_handle_error(NULL,
					PSM2_PARAM_ERR,
					"Option value length error");
			*optlen = sizeof(unsigned);
			return err;
		}

		if (get) {
			*((unsigned *)optval) = psm3_dbgmask;
		} else
			psm3_dbgmask = *(unsigned *)optval;
		break;
	case PSM2_CORE_OPT_EP_CTXT:
		{
			/* core object is epaddr */
			psm2_epaddr_t epaddr = (psm2_epaddr_t) core_obj;

			/* Sanity check epaddr */
			if (!epaddr) {
				return psm3_handle_error(NULL,
						PSM2_PARAM_ERR,
						"Invalid endpoint address");
			}

			/* Sanity check length */
			if (*optlen < sizeof(unsigned long)) {
				err =  psm3_handle_error(NULL,
						PSM2_PARAM_ERR,
						"Option value length error");
				*optlen = sizeof(void *);
				return err;
			}

			if (get) {
				*((unsigned long *)optval) =
				    (unsigned long)epaddr->usr_ep_ctxt;
			} else
				epaddr->usr_ep_ctxt = optval;
		}
		break;
	default:
		/* Unknown/unrecognized option */
		err = psm3_handle_error(NULL,
				PSM2_PARAM_ERR,
				"Unknown PSM3_CORE option %u.",
				optname);
		break;
	}
	return err;
}

psm2_error_t psm3_core_setopt(const void *core_obj, int optname,
			     const void *optval, uint64_t optlen)
{
	return psmi_coreopt_ctl(core_obj, optname, (void *)optval, &optlen, 0);
}

psm2_error_t psm3_core_getopt(const void *core_obj, int optname,
			     void *optval, uint64_t *optlen)
{
	return psmi_coreopt_ctl(core_obj, optname, optval, optlen, 1);
}

/* PSM AM component option handling */
PSMI_ALWAYS_INLINE(
psm2_error_t
psm3_amopt_ctl(const void *am_obj, int optname,
	       void *optval, uint64_t *optlen, int get))
{
	psm2_error_t err = PSM2_OK;

	/* AM object is a psm2_epaddr (or NULL for global minimum sz) */
	/* psm2_epaddr_t epaddr = (psm2_epaddr_t) am_obj; */

	/* All AM options are read-only. */
	if (!get) {
		return err =
		    psm3_handle_error(PSMI_EP_LOGEVENT, PSM2_OPT_READONLY,
				      "Attempted to set read-only option value");
	}

	/* Sanity check length -- all AM options are uint32_t. */
	if (*optlen < sizeof(uint32_t)) {
		*optlen = sizeof(uint32_t);
		return err = psm3_handle_error(PSMI_EP_LOGEVENT, PSM2_PARAM_ERR,
					       "Option value length error");
	}

	switch (optname) {
	case PSM2_AM_OPT_FRAG_SZ:
		*((uint32_t *) optval) = psm3_am_parameters.max_request_short;
		break;
	case PSM2_AM_OPT_NARGS:
		*((uint32_t *) optval) = psm3_am_parameters.max_nargs;
		break;
	case PSM2_AM_OPT_HANDLERS:
		*((uint32_t *) optval) = psm3_am_parameters.max_handlers;
		break;
	default:
		err =
		    psm3_handle_error(NULL, PSM2_PARAM_ERR,
				      "Unknown PSM3_AM option %u.", optname);
	}

	return err;
}

psm2_error_t psm3_am_setopt(const void *am_obj, int optname,
			   const void *optval, uint64_t optlen)
{
	return psm3_amopt_ctl(am_obj, optname, (void *)optval, &optlen, 0);
}

psm2_error_t psm3_am_getopt(const void *am_obj, int optname,
			   void *optval, uint64_t *optlen)
{
	return psm3_amopt_ctl(am_obj, optname, optval, optlen, 1);
}

#ifdef PSM_LOG

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fnmatch.h>
#include "ptl_ips/ips_proto_header.h"

/* A treeNode is used to store the list of Function Name Lists that
   are passed to the PSM_LOG facility via environment variables.
   See psm_log.h for more information.

   Note that treeNode is a node in a binary tree data structure. */
typedef struct _treeNode
{
	const char *name;
	int line1,line2;
	struct _treeNode *left,*right;
} treeNode;

/* An epmTreeNode is used to track the number of protocol packets
   that are send/recevied, for a given opcode, and source epid
   to another epid. */
typedef struct _epmTreeNode
{
	int opcode,count,txrx;
	psm2_epid_t fromepid,toepid;
	struct _epmTreeNode *left,*right;
} epmTreeNode;


/* given a line range: [*line1 .. *line2], and another line, line
   'join' the line range to the new line if the line immediately abuts
   the line range.  The new line does not abut the existing range,
   return 0.  Else, return 1.

   For example, take the line range [ 20 .. 30 ] and the line: 19.
   Since 19 comes immediately before 20, the line range can be joined
   resulting in the line rage: [ 19 .. 30 ].  The function returns 1 for this
   case.

   The following other examples gives the new line range given the new line and
   range [ 20 .. 30 ], and gives the return value:

   31 [ 20 .. 31 ] 1
   18 [ 20 .. 30 ] 0
   32 [ 20 .. 30 ] 0
   25 [ 20 .. 30 ] 1 */
static int joinOverlap(int *line1,int *line2,int line)
{
	long long ll_line = line;

	if (ll_line+1 >= *line1 && ll_line-1 <= *line2)
	{
		*line1 = min(*line1,line);
		*line2 = max(*line2,line);
		return 1;
	}
	return 0;
}

/* given two line ranges, determine the range that encompasses both line ranges
   if an overlap has occurred.  Returns 0 if the two ranges do not overlap and
   do not abutt.

   Some examples, if line1=20 and line2=30

   [20 30] [20 30] 2
   [19 30] [19 30] 2
   [19 20] [19 30] 2
   [10 15] [20 30] 0
   [40 50] [20 30] 0 */
static int joinOverlapRange(int *line1,int *line2,int l1,int l2)
{
	return joinOverlap(line1,line2,l1) + joinOverlap(line1,line2,l2);
}

/* inserts a new treeNode into the FNL tree, or, merges the lines that are already
   present in the tree. */
static void insertNodeInTree(treeNode **root,const char *name,int line1,int line2)
{
	if (*root)
	{
		int c = strcmp(name,(*root)->name);
		if (c < 0)
			insertNodeInTree(&((*root)->left),name,line1,line2);
		else if (c > 0)
			insertNodeInTree(&((*root)->right),name,line1,line2);
		else
		{
			if (joinOverlapRange(&(*root)->line1,&(*root)->line2,line1,line2))
				return;
			else if (line1 < (*root)->line1)
				insertNodeInTree(&((*root)->left),name,line1,line2);
			else if (line2 > (*root)->line2)
				insertNodeInTree(&((*root)->right),name,line1,line2);
			else psmi_assert_always(0); /* should never happen. */
		}
	}
	else
	{
		*root = malloc(sizeof(treeNode));
		(*root)->name  = strdup(name);
		(*root)->line1 = line1;
		(*root)->line2 = line2;
		(*root)->left  = (*root)->right  = NULL;
	}
}

/* Returns -1 if the data in the node is less    than the data supplied as parameter, else
   Returns  1 if the data in the node is greater than the data supplied as parameter, else
   Returns  0.
   */
static int compareEpmNode(epmTreeNode *node,int opcode,int txrx,psm2_epid_t fromepid,psm2_epid_t toepid)
{
	int ret;
#define COMPARE_ONE(X) if (node->X != X) return node->X < X ? -1 : 1
	COMPARE_ONE(opcode);
	COMPARE_ONE(txrx);
	ret = psm3_epid_cmp_internal(node->fromepid, fromepid); if (ret) return ret;
	ret = psm3_epid_cmp_internal(node->toepid, toepid); if (ret) return ret;
	return 0;
}

/* Inserts a new node in the tree corresponding to the parameters, or, retrieves the node in the tree.
   In either case, this code returns a pointer to the count in the node. */
static int *insertNodeInEpmTree(epmTreeNode **root,int opcode,int txrx,psm2_epid_t fromepid,psm2_epid_t toepid)
{
	if (*root)
	{
		int a = compareEpmNode((*root),opcode,txrx,fromepid,toepid);
		if (a < 0)
			return insertNodeInEpmTree(&((*root)->left),opcode,txrx,fromepid,toepid);
		else if (a > 0)
			return insertNodeInEpmTree(&((*root)->right),opcode,txrx,fromepid,toepid);
		else
			return &((*root)->count);
	}
	else
	{
		*root = malloc(sizeof(epmTreeNode));
		(*root)->opcode   = opcode;
		(*root)->txrx     = txrx;
		(*root)->count    = 0;
		(*root)->fromepid = fromepid;
		(*root)->toepid   = toepid;
		(*root)->left     = (*root)->right  = NULL;
		return &((*root)->count);
	}
}

/* returns 0, if the node is present, non-zero if it is absent. */
static int lookupNodeInTree(const treeNode *root,const char *name,int line)
{
	if (root)
	{
		int c = strcmp(name,root->name);
		if (c < 0)
			return lookupNodeInTree(root->left,name,line);
		else if (c > 0)
			return lookupNodeInTree(root->right,name,line);
		else
		{
			if (line < root->line1)
				return lookupNodeInTree(root->left,name,line);
			else if (line > root->line2)
				return lookupNodeInTree(root->right,name,line);
			else /* line must be >= root->line1 and line must be <= root->line2. */
				return 0;
		}
	}
	else
	{
		return 1;
	}
}

/* Declare a prototype for a parserFunc - referenced in the following code: */
typedef void parserFunc(char *,int,int,void *);

/* breaks down a string into 'c'-delimited substrings, and calls the parser func for each substring. */
static void parseString(char *ps,char c,parserFunc pf,void *ctx)
{
	int idx,n=0;
	char *p;

	/* first, count the number of instances of c in ps, for use by the parser function: */
	for (idx=0;ps[idx];idx++)
		if (ps[idx] == c)
			n++;
	/* next, break down ps into 'c'-delimited substrings, and call parser function, pf for each substring: */
	for (idx=0,p=ps;p && *p;idx++)
	{
		char *t = strchr(p,c);
		if (!t)
		{
			break;
		}
		else
		{
			*t = 0;
			pf(p,idx,n,ctx);
			p = t+1;
		}
	}
	/* finally, call pf on the final substring. */
	pf(p,idx,n,ctx);
}

/* fncNameCtx is the context used while parsing FNL's (see psm_log.h for more info) from the environment: */
typedef struct
{
	const char *currentFuncName;
	int firstLineNumber;
	treeNode **root;
} funcNameCtx;

/* This is the start of the parser code for parsing FNL's.  Here is the grammar:

  An FNL is a 'Function Name List' that is defined by the following grammar:

  # A LINE1 is either a single line number of a range of line numbers:
(1)  LINE1 :: lineNumber |
(2)           lineNumber1 '-' lineNumber2

  # LINES is a list of LINE1's separated by commas:
(3)  LINES :: LINE1 |
(4)           LINE1 ',' LINES

  # An FN is either a function name, or a function name with a list of lines:
(5)  FN :: functionName |
(6)        functionName ';' LINES

  # A FNL is a list of FN's separated by colons:
(7)  FNL ::  FN |
(8)          FN ':' FNL

  # Examples:
  foo:bar    the two functions foo and bar
  foo;1-10   lines 1 to 10 of function foo.
  bar;1,3,5  lines 1, 3 and 5 of function bar

*/

/* p4() inserts a (function name and line number) pair into the FNL tree or a (function name and line number range) in the FNL tree.
*/
static void p4(char *s,int idx,int n,void *ctx)
{
	funcNameCtx *pfnc = (funcNameCtx *)ctx;

	if (n == 0) /* production (1) */
	{
		pfnc->firstLineNumber = atoi(s);
		insertNodeInTree(pfnc->root,pfnc->currentFuncName,pfnc->firstLineNumber,pfnc->firstLineNumber);
	}
	else if (n == 1) /* production (2) */
	{
		if (idx == 0) /* lhs of production (2) */
			pfnc->firstLineNumber = atoi(s);
		else /* rhs of production (2). */
			insertNodeInTree(pfnc->root,pfnc->currentFuncName,pfnc->firstLineNumber,atoi(s));
	}
}

/* p3 puts an entry into the FNL tree for all of the lines of a given functionname, or, it parses the list of line number ranges and
   uses p4 to spill each individual range (or just one line number) into the tree */
static void p3(char *s,int idx,int n,void *ctx)
{
	funcNameCtx *pfnc = (funcNameCtx *)ctx;

	if (n == 0 && *s == 0) /* production (5)/(7) */
	{
		insertNodeInTree(pfnc->root,pfnc->currentFuncName,0,INT_MAX);
	}
	else if (*s) /* production (2) */
	{
		/* breakdown the string into hyphen-delimited substrings, and further parses each substring with p4: */
		parseString(s,'-',p4,ctx);
	}
}

/* p2 parses the function name, and caches it into the context, and thereafter uses p3 to parse the line number range list. */
static void p2(char *s,int idx,int n,void *ctx)
{
	funcNameCtx *pfnc = (funcNameCtx *)ctx;

	if (n)
	{
		if (idx == 0)
			pfnc->currentFuncName = s;
		else
		{
			/* production (4) */
			/* breakdown the string into comma-delimited substrings, and further parses each substring with p3: */
			parseString(s,',',p3,ctx);
		}
	}
	else
	{
		/* production (7)/(5). */
		insertNodeInTree(pfnc->root,pfnc->currentFuncName=s,0,INT_MAX);
	}
}

/* p1 parses each function name and line range list. */
static void p1(char *s,int idx,int n,void *ctx)
{
	/* production (5)/(6)) */
	/* breakdown the string into semi-colon-delimited substrings, and further parses each substring with p2: */
	parseString(s,';',p2,ctx);
}

static void parseAndInsertInTree(const char *buf,treeNode **root)
{
	funcNameCtx t;
	t.root = root;
	char *p = alloca(strlen(buf)+1);
	strcpy(p,buf);
	/* productions (7)/(8) */
	/* separates string into colon-separated strings, and then parses each substring in p1: */
	parseString(p,':',p1,(void*)&t);
}

/* initialization code for the psmi log mechanism. */
static inline void psmi_initialize(const char **plmf_fileName_kernel,
				   const char **plmf_search_format_string,
				   treeNode   **includeFunctionNamesTreeRoot,
				   treeNode   **excludeFunctionNamesTreeRoot)
{
	static volatile int  plmf_initialized = 0;

	if (!plmf_initialized)
	{
		static pthread_mutex_t plmf_init_mutex = PTHREAD_MUTEX_INITIALIZER;

		if (pthread_mutex_lock(&plmf_init_mutex))
		{
			perror("cannot lock mutex for psmi_log_message facility");
			return;
		}
                /* CRITICAL SECTION BEGIN */
		if (!plmf_initialized)
		{
			/* initializing psmi log message facility here. */
			const char *env = getenv("PSM3_LOG_FILENAME");
			if (env)
				*plmf_fileName_kernel = env;
			env = getenv("PSM3_LOG_SRCH_FORMAT_STRING");
			if (env)
			{
				*plmf_search_format_string = env;
			}
			else
			{
				env = getenv("PSM3_LOG_INC_FUNCTION_NAMES");
				if (env)
				{
					parseAndInsertInTree(env,includeFunctionNamesTreeRoot);
				}
				env = getenv("PSM3_LOG_EXC_FUNCTION_NAMES");
				if (env)
				{
					parseAndInsertInTree(env,excludeFunctionNamesTreeRoot);
				}
			}
			/* initialization of psmi log message facility is completed. */
			plmf_initialized = 1;
		}
		/* CRITICAL SECTION END */
		if (pthread_mutex_unlock(&plmf_init_mutex))
		{
			perror("cannot unlock mutex for psmi_log_message facility");
			return;
		}
	}
}

/* Utility function to map the integer txrx value to the given strings for emitting to the log file. */
static const char * TxRxString(int txrx)
{
	switch(txrx)
	{
	case PSM2_LOG_TX:	return "Sent";
	case PSM2_LOG_RX:	return "Received";
	case PSM2_LOG_PEND:	return "Pending";
	default:		return "Unknown";
	}
}

/* Utility function to map an integer opcode value to the given strings for emitting to the log file. */
static const char * OpcodeString(int opcode)
{
	switch(opcode)
	{
	case OPCODE_LONG_RTS:          return "RTS";
	case OPCODE_LONG_CTS:          return "CTS";
	case OPCODE_LONG_DATA:         return "DATA";
	case OPCODE_ERR_CHK_RDMA:      return "ERR_CHK_RDMA";
	case OPCODE_ERR_CHK_RDMA_RESP: return "ERR_CHK_RDMA_RESP";
	default:                       return "UNKNOWN";
	}
}

static const char     *plmf_fileName_kernel         = "/tmp/psm3_log";
static const char     *plmf_search_format_string    = NULL;
static       treeNode *includeFunctionNamesTreeRoot = NULL;
static       treeNode *excludeFunctionNamesTreeRoot = NULL;

void psmi_log_initialize(void)
{
	/* If not initialized, then, initialize in a single thread of execution. */
	psmi_initialize(&plmf_fileName_kernel,
			&plmf_search_format_string,
			&includeFunctionNamesTreeRoot,
			&excludeFunctionNamesTreeRoot);
}

#ifdef PSM_LOG_FAST_IO

struct psmi_log_io_thread_info
{
	pthread_t thread_id;
	char *buff;
	unsigned long max_buff_length, curr_buff_length;
	pthread_mutex_t flags_mutex;
	volatile int flags;
#define PSMI_LOG_IO_FLAG_IO_IN_PROGRESS 1  /* io is currently in progress */
#define PSMI_LOG_IO_FLAG_IO_SHUTDOWN    2  /* we are shutting down logging. */
};

/* Please note that psmi_log_io_info is in thread local storage. */
static __thread struct psmi_log_io_thread_info psmi_log_io_info =
{
	.thread_id        = 0,
	.buff             = NULL,
	.max_buff_length  = 0,
	.curr_buff_length = 0,
	.flags_mutex      = PTHREAD_MUTEX_INITIALIZER,
	.flags            = 0
};

static struct
{
	unsigned int nTableEntries,maxTableEntries;
	pthread_mutex_t table_mutex;
	struct psmi_log_io_thread_info **table;
} psmi_log_io_table =
{
	.nTableEntries   = 0,
	.maxTableEntries = 0,
	.table_mutex     = PTHREAD_MUTEX_INITIALIZER,
	.table           = NULL
};

void psmi_log_fini()
{
	if (pthread_mutex_lock(&psmi_log_io_table.table_mutex))
	{
		perror("Cannot lock mutex for psmi_log_io_table");
		return;
	}
	/* Start critical section. */

	unsigned int i;
	for (i=0;i < psmi_log_io_table.nTableEntries;i++)
	{
		if (psmi_log_io_table.table[i])
		{
			struct psmi_log_io_thread_info *pti = psmi_log_io_table.table[i];
			int flags;

			if (pthread_mutex_lock(&pti->flags_mutex))
			{
				perror("can't lock the flags mutex.");
				continue;
			}
			/* critical section */
			flags = (pti->flags |= PSMI_LOG_IO_FLAG_IO_SHUTDOWN);
			/* end critical section */
			pthread_mutex_unlock(&pti->flags_mutex);
			/* if io is currenctly in progress, allow it to complete. */
			while (flags & PSMI_LOG_IO_FLAG_IO_IN_PROGRESS)
			{
				sleep(1);
				if (pthread_mutex_lock(&pti->flags_mutex))
				{
					perror("can't lock the flags mutex.");
					continue;
				}
				flags = pti->flags;
				pthread_mutex_unlock(&pti->flags_mutex);
			}
			if (pti->buff)
			{
				char logFileName[256];
				FILE *fout;

				snprintf(logFileName,sizeof(logFileName),"%s.%d.%ld",
					 plmf_fileName_kernel,getpid(),pti->thread_id);
				fout = fopen(logFileName,"w");
				if (!fout)
				{
					perror(logFileName);
					continue;
				}
				fwrite(pti->buff,pti->curr_buff_length,1,fout);
				fclose(fout);
			}
		}
		psmi_log_io_table.table[i] = NULL;
	}
	psmi_log_io_table.nTableEntries = 0;
	psmi_free(psmi_log_io_table.table);
	psmi_log_io_table.table = NULL;
	psmi_log_io_table.maxTableEntries = 0;
	/* End critical section. */
	pthread_mutex_unlock(&psmi_log_io_table.table_mutex);
}

static int psmi_log_register_tls(void)
{
	if (psmi_log_io_info.thread_id != pthread_self())
	{
		psmi_log_io_info.thread_id = pthread_self();
		if (pthread_mutex_lock(&psmi_log_io_table.table_mutex))
		{
			perror("cannot lock table mutex");
			return -1;
		}
		/* critical section start. */
		if (psmi_log_io_table.maxTableEntries < psmi_log_io_table.nTableEntries+1)
		{
			if (psmi_log_io_table.maxTableEntries == 0)
			{
				psmi_log_io_table.maxTableEntries = 2;
				psmi_log_io_table.table = psmi_malloc(PSMI_EP_NONE,
								      PER_PEER_ENDPOINT,
								      psmi_log_io_table.maxTableEntries *
								      sizeof(struct psmi_log_io_thread_info *));
			}
			else
			{
				psmi_log_io_table.maxTableEntries *= 2;
				psmi_log_io_table.table = psmi_realloc(PSMI_EP_NONE,
								       PER_PEER_ENDPOINT,
								       psmi_log_io_table.table,
								       psmi_log_io_table.maxTableEntries *
								       sizeof(struct psmi_log_io_thread_info *));
			}
		}
		psmi_log_io_table.table[psmi_log_io_table.nTableEntries] = &psmi_log_io_info;
		psmi_log_io_table.nTableEntries++;
		/* critical section end. */
		pthread_mutex_unlock(&psmi_log_io_table.table_mutex);
	}
	if (pthread_mutex_lock(&psmi_log_io_info.flags_mutex))
	{
		perror("cannot lock table mutex");
		return -1;
	}
	/* critical section start. */
	int old_flags = psmi_log_io_info.flags;
	int new_flags = old_flags;
	if (0 == (old_flags & PSMI_LOG_IO_FLAG_IO_SHUTDOWN))
		new_flags |= PSMI_LOG_IO_FLAG_IO_IN_PROGRESS;
	psmi_log_io_info.flags = new_flags;
	/* critical section end. */
	pthread_mutex_unlock(&psmi_log_io_info.flags_mutex);
	if (new_flags & PSMI_LOG_IO_FLAG_IO_IN_PROGRESS)
		return 0;
	return -1;
}

static void psmi_buff_fclose(int port)
{
	if (pthread_mutex_lock(&psmi_log_io_info.flags_mutex))
	{
		perror("cannot lock table mutex");
		return;
	}
	/* critical section start. */
	psmi_log_io_info.flags &= ~PSMI_LOG_IO_FLAG_IO_IN_PROGRESS;
	/* critical section end. */
	pthread_mutex_unlock(&psmi_log_io_info.flags_mutex);
}

static void growBuff(size_t minExcess)
{
       while (psmi_log_io_info.curr_buff_length+minExcess > psmi_log_io_info.max_buff_length)
	{
		if (!psmi_log_io_info.buff)
			psmi_log_io_info.buff = (char *)psmi_malloc(PSMI_EP_NONE, PER_PEER_ENDPOINT,
								    psmi_log_io_info.max_buff_length = 1 << 20);
		else
		{
			psmi_log_io_info.max_buff_length *= 2;
			psmi_log_io_info.buff = (char *)psmi_realloc(PSMI_EP_NONE, PER_PEER_ENDPOINT,
								     psmi_log_io_info.buff,
								     psmi_log_io_info.max_buff_length);
		}
	}
}

static int psmi_buff_vfprintf(int port, const char *format, va_list ap)
{
	int done = 0;
	size_t excess = 1024;
	int length;

	while (!done)
	{
		growBuff(excess);

		length = vsnprintf(psmi_log_io_info.buff + psmi_log_io_info.curr_buff_length,
				   excess, format, ap);
		if (length >= excess)
			excess *= 2;
		else
			done = 1;
	}
	psmi_log_io_info.curr_buff_length += length;
	return length;
}

static int psmi_buff_fprintf(int port,const char *format, ...)
{
	int length;
	va_list ap;

	va_start(ap, format);

	length = psmi_buff_vfprintf(port,format,ap);

	va_end(ap);
	return length;
}

static int psmi_buff_fputc(int c, int port)
{
	growBuff(1024);
	psmi_log_io_info.buff[psmi_log_io_info.curr_buff_length] = c;
	psmi_log_io_info.curr_buff_length++;
	return 1;
}
#endif


#define IS_PSMI_LOG_MAGIC(S) ((((uint64_t)(S)) <= ((uint64_t)PSM2_LOG_MIN_MAGIC)) && \
			      (((uint64_t)(S)) >= ((uint64_t)PSM2_LOG_MAX_MAGIC)))

/* plmf is short for 'psm log message facility. All of the PSM_LOG macros defined in psm_log.h
   are serviced from this back end. */
void psmi_log_message(const char *fileName,
		      const char *functionName,
		      int         lineNumber,
		      const char *format, ...)
{
	va_list ap;

	va_start(ap, format);

	/* Next, determine if this log message is signal or noise. */
	if (plmf_search_format_string)
	{
		if (!IS_PSMI_LOG_MAGIC(format))
		{
			if (fnmatch(plmf_search_format_string, format, 0))
			{
				va_end(ap);
				/* tis noise, return. */
				return;
			}
		}
	}
	else
	{
		if (includeFunctionNamesTreeRoot)
		{
			if (lookupNodeInTree(includeFunctionNamesTreeRoot,functionName,lineNumber))
			{
				va_end(ap);
				/* tis noise, return. */
				return;
			}
		}

		if (excludeFunctionNamesTreeRoot)
		{
			if (!lookupNodeInTree(excludeFunctionNamesTreeRoot,functionName,lineNumber))
			{
				va_end(ap);
				/* tis noise, return. */
				return;
			}
		}
	}

	/* At this point, we think that this may be a message that we want to emit to the log.
	   But, there is one more test, to apply to the cases where the format is one of the
	   special formats for backtrack, and packet stream for example. */
	{
		void           **voidarray   = NULL;
		int              nframes     = 0;
		const char      *newFormat   = format;
		int              opcode      = 0;
		psmi_log_tx_rx_t txrx        = 0;
		psm2_epid_t         fromepid    = psm3_epid_zeroed_internal();
		psm2_epid_t         toepid      = psm3_epid_zeroed_internal();
		void            *dumpAddr[2] = {0};
		size_t           dumpSize[2] = {0};

#ifdef PSM_LOG_FAST_IO
#define IO_PORT         0
#define MY_FPRINTF      psmi_buff_fprintf
#define MY_VFPRINTF     psmi_buff_vfprintf
#define MY_FPUTC        psmi_buff_fputc
#define MY_FCLOSE       psmi_buff_fclose
#else
		char logFileName[256];
		FILE *fout;
#define IO_PORT         fout
#define MY_FPRINTF      fprintf
#define MY_VFPRINTF     vfprintf
#define MY_FPUTC        fputc
#define MY_FCLOSE       fclose
#endif
		struct timespec tp;

		/* Pop arguments for the alternative forms of PSM_LOG functionality: */
		if (format == PSM2_LOG_BT_MAGIC)
		{
			voidarray = va_arg(ap,void **);
			nframes   = va_arg(ap,int);
			newFormat = va_arg(ap,const char *);
		}
		else if (format == PSM2_LOG_EPM_MAGIC)
		{
			opcode    = va_arg(ap,int);
			txrx      = va_arg(ap,psmi_log_tx_rx_t);
			fromepid  = va_arg(ap,psm2_epid_t);
			toepid    = va_arg(ap,psm2_epid_t);
			newFormat = va_arg(ap,const char *);
		}
		else if (format == PSM2_LOG_DUMP_MAGIC)
		{
			dumpAddr[0]  = va_arg(ap,void*);
			dumpSize[0]  = va_arg(ap,size_t);
			newFormat    = va_arg(ap,const char *);
		}
		else if (format == PSM2_LOG_PKT_STRM_MAGIC)
		{
			txrx        = va_arg(ap,psmi_log_tx_rx_t);
			dumpAddr[0] = va_arg(ap,struct ips_message_header *);
			newFormat   = va_arg(ap,const char *);
			dumpSize[0] = sizeof(struct ips_message_header);
		}

		/* One last test to make sure that this message is signal: */
		if (plmf_search_format_string && newFormat)
		{
			if (fnmatch(plmf_search_format_string, newFormat, 0))
			{
				va_end(ap);
				/* tis noise, return. */
				return;
			}
		}

#ifdef PSM_LOG_FAST_IO
		if (psmi_log_register_tls() != 0)
		{
			va_end(ap);
			return;
		}
#else
		/* At this point we know that the message is not noise, and it is going to be emitted to the log. */
		snprintf(logFileName,sizeof(logFileName),"%s.%d.%ld",
			 plmf_fileName_kernel,getpid(),
			 pthread_self());
		fout = fopen(logFileName,"a");
		if (!fout)
		{
			va_end(ap);
			return;
		}
#endif

#define M1()	clock_gettime(CLOCK_REALTIME, &tp);				 	\
			MY_FPRINTF(IO_PORT,"%f %s %s:%d: ",				\
			   (double)tp.tv_sec + ((double)tp.tv_nsec/1000000000.0),	\
			   functionName,fileName,lineNumber)

		M1();

		if (!IS_PSMI_LOG_MAGIC(format))
		{
			MY_VFPRINTF(IO_PORT,format,ap);
			MY_FPUTC('\n',IO_PORT);
		}
		else if (format == PSM2_LOG_BT_MAGIC)
		{
			void *newframes[nframes];
			int  newframecnt      = backtrace(newframes,nframes);
			int  pframes          = min(newframecnt,nframes);

			MY_VFPRINTF(IO_PORT,newFormat,ap);
			MY_FPUTC('\n',IO_PORT);

			if (memcmp(voidarray,newframes,pframes * sizeof(void*)))
			{
				int i;
				char **strings;

				memcpy(voidarray,newframes,sizeof(newframes));
				M1();
				MY_FPRINTF(IO_PORT,
					   "backtrace() returned %d addresses\n",
					   newframecnt);
				strings = backtrace_symbols(voidarray, pframes);
				if (strings == NULL)
				{
					perror("backtrace_symbols");
					exit(EXIT_FAILURE);
				}
				for (i = 0; i < pframes; i++)
				{
					M1();
					MY_FPRINTF(IO_PORT,"%s\n", strings[i]);
				}
#undef free
				free(strings);
			}
		}
		else if (format == PSM2_LOG_EPM_MAGIC)
		{
			static epmTreeNode *root = 0;
			static pthread_mutex_t plmf_epm_mutex =
				PTHREAD_MUTEX_INITIALIZER;
			int *pcount = 0;
			if (pthread_mutex_lock(&plmf_epm_mutex))
			{
				perror("cannot lock mutex for "
				       "psmi_log_message facility");
				va_end(ap);
				return;
			}
			/* START OF CRITICAL SECTION */
			pcount = insertNodeInEpmTree(&root,opcode,txrx,
						     fromepid,toepid);
			/* END OF CRITICAL SECTION */
			if (pthread_mutex_unlock(&plmf_epm_mutex))
			{
				perror("cannot unlock mutex for "
				       "psmi_log_message facility");
				va_end(ap);
				return;
			}
			(*pcount)++;
			MY_FPRINTF(IO_PORT,"%s %s from: %s"
				   ", to: %s, count: %d, ",
				   TxRxString(txrx),OpcodeString(opcode),
				   psm3_epid_fmt_internal(fromepid,0),psm3_epid_fmt_internal(toepid,1),*pcount);
			MY_VFPRINTF(IO_PORT,newFormat,ap);
			MY_FPUTC('\n',IO_PORT);
		}
		else if (format == PSM2_LOG_PKT_STRM_MAGIC)
		{
			MY_FPRINTF(IO_PORT,"PKT_STRM: %s: imh: %p ", TxRxString(txrx),
				   dumpAddr[0]);
			goto dumpit;
		}
		else if (format == PSM2_LOG_DUMP_MAGIC)
		{
		dumpit:
			MY_VFPRINTF(IO_PORT,newFormat,ap);
			MY_FPUTC('\n',IO_PORT);
		dumpmore:
			M1();

			uint8_t *pu8 = (uint8_t *)dumpAddr[0];
			size_t   i,cnt=0;
			for (i=0;i < dumpSize[0];i++)
			{
				if ((i != 0) && ((i % 8) == 0))
				{
					MY_FPRINTF(IO_PORT," (%d)\n",(int)(i-8));
					M1();
					cnt = 0;
				}
				else if (cnt)
					MY_FPUTC(',',IO_PORT);
				MY_FPRINTF(IO_PORT,"0x%02x", pu8[i]);
				cnt++;
			}
			if (cnt)
				MY_FPRINTF(IO_PORT," (%d)\n",(int)(i-8));
			if (dumpSize[1])
			{
				dumpSize[0] = dumpSize[1];
				dumpAddr[0] = dumpAddr[1];
				dumpSize[1] = 0;
				goto dumpmore;
			}
		}
		MY_FCLOSE(IO_PORT);
	}

	va_end(ap);
}
#endif /* #ifdef PSM_LOG */

/* touch the pages, with a 32 bit read */
void psm3_touch_mmap(void *m, size_t bytes)
{
	volatile uint32_t *b = (volatile uint32_t *)m, c;
	size_t i;		/* m is always page aligned, so pgcnt exact */
	int pg_sz;

	/* First get the page size */
	pg_sz = sysconf(_SC_PAGESIZE);

	_HFI_VDBG("Touch %lu mmap'ed pages starting at %p\n",
		  (unsigned long)bytes / pg_sz, m);
	bytes /= sizeof(c);
	for (i = 0; i < bytes; i += pg_sz / sizeof(c))
		c = b[i];
}
