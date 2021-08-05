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
#include <netinet/in.h>  // for sockaddr
#include <fnmatch.h>


int psmi_ep_device_is_enabled(const psm2_ep_t ep, int devid);

struct psmi_epid_table psmi_epid_table;

/* Iterator to access the epid table.
 * 'ep' can be NULL if remote endpoints from all endpoint handles are requested
 */
void psmi_epid_itor_init(struct psmi_eptab_iterator *itor, psm2_ep_t ep)
{
	itor->i = 0;
	itor->ep = ep;
	pthread_mutex_lock(&psmi_epid_table.tablock);
}

void *psmi_epid_itor_next(struct psmi_eptab_iterator *itor)
{
	int i;
	struct psmi_epid_tabentry *e;

	if (itor->i >= psmi_epid_table.tabsize)
		return NULL;
	for (i = itor->i; i < psmi_epid_table.tabsize; i++) {
		e = &psmi_epid_table.table[i];
		if (!e->entry || e->entry == EPADDR_DELETED)
			continue;
		if (itor->ep && e->ep != itor->ep)
			continue;
		itor->i = i + 1;
		return e->entry;
	}
	itor->i = psmi_epid_table.tabsize;	/* put at end of table */
	return NULL;
}

void psmi_epid_itor_fini(struct psmi_eptab_iterator *itor)
{
	pthread_mutex_unlock(&psmi_epid_table.tablock);
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

psm2_error_t psmi_epid_init()
{
	pthread_mutexattr_t attr;
	psmi_epid_table.table = NULL, psmi_epid_table.tabsize = 0;
	psmi_epid_table.tabsize_used = 0;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&psmi_epid_table.tablock, &attr);
	pthread_mutexattr_destroy(&attr);
	return PSM2_OK;
};

psm2_error_t psmi_epid_fini()
{
	if (psmi_epid_table.table != NULL) {
		psmi_free(psmi_epid_table.table);
		psmi_epid_table.table = NULL;
	}
	psmi_epid_table.tabsize = 0;
	psmi_epid_table.tabsize_used = 0;
	return PSM2_OK;
}

PSMI_ALWAYS_INLINE(
uint64_t
hash_this(const psm2_ep_t ep, const psm2_epid_t epid))
{
	uint64_t ep_i = (uint64_t) (uintptr_t) ep;
	uint64_t epid_i = (uint64_t) epid;
	uint64_t hash = 0x9e3779b97f4a7c13LL;
	mix64(ep_i, epid_i, hash);
	return hash;
}

PSMI_ALWAYS_INLINE(
void *
psmi_epid_lookup_inner(psm2_ep_t ep, psm2_epid_t epid, int remove))
{
	uint64_t key = hash_this(ep, epid);
	struct psmi_epid_tabentry *e;
	void *entry = NULL;
	int idx;

	pthread_mutex_lock(&psmi_epid_table.tablock);
	if (!psmi_epid_table.table)
		goto ret;
	idx = (int)(key % psmi_epid_table.tabsize);
	while (psmi_epid_table.table[idx].entry != NULL) {
		/* An epid can be added twice if there's more than one opened endpoint,
		 * but really we match on epid *and* on endpoint */
		e = &psmi_epid_table.table[idx];
		if (e->entry != EPADDR_DELETED && e->key == key) {
			entry = e->entry;
			if (remove)
				psmi_epid_table.table[idx].entry =
				    EPADDR_DELETED;
			goto ret;
		}
		if (++idx == psmi_epid_table.tabsize)
			idx = 0;
	}
ret:
	pthread_mutex_unlock(&psmi_epid_table.tablock);
	return entry;
}

void *psmi_epid_lookup(psm2_ep_t ep, psm2_epid_t epid)
{
	void *entry = psmi_epid_lookup_inner(ep, epid, 0);
	if (PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("lookup of (%p,%" PRIx64 ") returns %p\n", ep, epid,
			  entry);
	return entry;
}

void *psmi_epid_remove(psm2_ep_t ep, psm2_epid_t epid)
{
	if (PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("remove of (%p,%" PRIx64 ")\n", ep, epid);
	return psmi_epid_lookup_inner(ep, epid, 1);
}

void psmi_epid_remove_all(psm2_ep_t ep)
{
	size_t i;
	struct psmi_epid_tabentry *e;

	pthread_mutex_lock(&psmi_epid_table.tablock);

	for (i = 0; i < psmi_epid_table.tabsize; i++) {
		e = &psmi_epid_table.table[i];

		if (e->entry == NULL || e->entry == EPADDR_DELETED)
			continue;

		if (e->ep == ep) {
			/* unspecified fields implicitly zeroed */
			*e = (struct psmi_epid_tabentry) {
				.entry = EPADDR_DELETED
			};
		}
	}

	pthread_mutex_unlock(&psmi_epid_table.tablock);
}

psm2_error_t psmi_epid_add(psm2_ep_t ep, psm2_epid_t epid, void *entry)
{
	uint64_t key;
	int idx, i, newsz;
	struct psmi_epid_tabentry *e;
	psm2_error_t err = PSM2_OK;

	if (PSMI_EP_HOSTNAME != ep)
		_HFI_VDBG("add of (%p,%" PRIx64 ") with entry %p\n", ep, epid,
			  entry);
	pthread_mutex_lock(&psmi_epid_table.tablock);
	/* Leave this here, mostly for sanity and for the fact that the epid
	 * table is currently not used in the critical path */
	if (++psmi_epid_table.tabsize_used >
	    (int)(psmi_epid_table.tabsize * PSMI_EPID_TABLOAD_FACTOR)) {
		struct psmi_epid_tabentry *newtab;
		newsz = psmi_epid_table.tabsize + PSMI_EPID_TABSIZE_CHUNK;
		newtab = (struct psmi_epid_tabentry *)
		    psmi_calloc(ep, PER_PEER_ENDPOINT,
				newsz, sizeof(struct psmi_epid_tabentry));
		if (newtab == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		if (psmi_epid_table.table) {	/* rehash the table */
			for (i = 0; i < psmi_epid_table.tabsize; i++) {
				e = &psmi_epid_table.table[i];
				if (e->entry == NULL)
					continue;
				/* When rehashing, mark deleted as free again */
				if (e->entry == EPADDR_DELETED) {
					psmi_epid_table.tabsize_used--;
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
			psmi_free(psmi_epid_table.table);
		}
		psmi_epid_table.table = newtab;
		psmi_epid_table.tabsize = newsz;
	}
	key = hash_this(ep, epid);
	idx = (int)(key % psmi_epid_table.tabsize);
	e = &psmi_epid_table.table[idx];
	while (e->entry && e->entry != EPADDR_DELETED) {
		if (++idx == psmi_epid_table.tabsize)
			idx = 0;
		e = &psmi_epid_table.table[idx];
	}
	e->entry = entry;
	e->key = key;
	e->epid = epid;
	e->ep = ep;

fail:
	pthread_mutex_unlock(&psmi_epid_table.tablock);
	return err;
}

static psmi_lock_t psmi_gethostname_lock;

static void __attribute__ ((constructor)) __psmi_gethostname_lock_constructor(void)
{
	psmi_init_lock(&psmi_gethostname_lock);
}

char *psmi_gethostname(void)
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
 */
psm2_error_t
psmi_epid_set_hostname(uint64_t nid, const char *hostname, int overwrite)
{
	size_t hlen;
	char *h;
	psm2_error_t err = PSM2_OK;

	if (hostname == NULL)
		return PSM2_OK;
	/* First see if a hostname already exists */
	if ((h = psmi_epid_lookup(PSMI_EP_HOSTNAME, nid)) != NULL) {
		if (!overwrite)
			return PSM2_OK;

		h = psmi_epid_remove(PSMI_EP_HOSTNAME, nid);
		if (h != NULL)	/* free the previous hostname if so exists */
			psmi_free(h);
	}

	hlen = min(PSMI_EP_HOSTNAME_LEN, strlen(hostname) + 1);
	h = (char *)psmi_malloc(PSMI_EP_NONE, PER_PEER_ENDPOINT, hlen);
	if (h == NULL)
		return PSM2_NO_MEMORY;
	snprintf(h, hlen, "%s", hostname);
	h[hlen - 1] = '\0';
	err = psmi_epid_add(PSMI_EP_HOSTNAME, nid, h);
	return err;
}

/* XXX These three functions are not thread safe, we'll use a rotating buffer
 * trick to make them thread safe because we really only have a few thread
 * (assuming multi_ep has < 8 threads of it's own) */
/* this returns just the addressing */
const char *psmi_epaddr_fmt_addr(psm2_epid_t epid)
{
	static char hostnamebufs[16][PSMI_EP_HOSTNAME_LEN];
	static int bufno;
	char *hostname;

	hostname = hostnamebufs[bufno];
	bufno = (bufno + 1) % 16;

	char buf[INET_ADDRSTRLEN];
	if (PSMI_EPID_GET_EPID_VERSION(epid) == PSMI_EPID_V4)
		snprintf(hostname, PSMI_EP_HOSTNAME_LEN - 1, "IP=%s QP=%d",
			psmi_ipv4_ntop((uint32_t)PSMI_EPID_GET_LID(epid), buf, sizeof(buf)),
			(int)PSMI_EPID_GET_CONTEXT(epid));
	else
		snprintf(hostname, PSMI_EP_HOSTNAME_LEN - 1, "LID=%d QP=%d",
			(int)PSMI_EPID_GET_LID(epid),
			(int)PSMI_EPID_GET_CONTEXT(epid));
	hostname[PSMI_EP_HOSTNAME_LEN - 1] = '\0';
	return hostname;
}

/* this returns the simple name, if not known gives addressing */
const char *psmi_epaddr_get_hostname(psm2_epid_t epid)
{
	uint64_t nid = psm2_epid_nid(epid);
	char *h;

	/* First, if we have registered a host for this epid, just return that, or
	 * else try to return something with lid and context */
	h = psmi_epid_lookup(PSMI_EP_HOSTNAME, nid);
	if (h != NULL)
		return h;
	else {
		return psmi_epaddr_fmt_addr(epid);
	}
}

/* this returns the name and addressing */
/* if not known just gives addressing */
const char *psmi_epaddr_get_name(psm2_epid_t epid)
{
	static char hostnamebufs[4][PSMI_EP_HOSTNAME_LEN];
	static int bufno;
	char *h, *hostname;
	hostname = hostnamebufs[bufno];
	bufno = (bufno + 1) % 4;

	h = psmi_epid_lookup(PSMI_EP_HOSTNAME, psm2_epid_nid(epid));
	if (h == NULL)
		return psmi_epaddr_get_hostname(epid);
	else {
		snprintf(hostname, PSMI_EP_HOSTNAME_LEN - 1, "%s (%s)", h,
				psmi_epaddr_fmt_addr(epid));
		hostname[PSMI_EP_HOSTNAME_LEN - 1] = '\0';
	}
	return hostname;
}


// superset of inet_ntop.  For AF_INET and AF_INET6 outputs address and port
const char *psmi_sockaddr_ntop(struct sockaddr* addr, char *dst, socklen_t size)
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
		// we show just the IPv6 address and port.
		// could also show scope_id and flowinfo
		inet_ntop(AF_INET6,  &in_addr->sin6_addr, dst, size);
		snprintf(dst+strlen(dst), size-strlen(dst), " %u", be16toh(in_addr->sin6_port));
		return dst;
	}
	default:
		snprintf(dst, size, "Unsupported");
		return dst;
	}
}

// subset of inet_ntop.
// formats address or netmask (in host byte order)
// into buf which has >= buf_size bytes available.
// returns a \0 terminated string suitable for use in printf such as:
// { char buf[INET_ADDRSTRLEN];
//		 printf("IP=%s\n", psmi_ipv4_ntop(ip_addr, buf, sizeof(buf));}
// on success pointer returned will be to buf.  For various errors a
// constant string outside of buf may be returned such that caller can safely
// call printf (or similar functions) without checking return value.
// on errors, errno is also set.
// Note IPv4 addresses worse case length is INET_ADDRSTRLEN.
const char *psmi_ipv4_ntop(uint32_t ip_addr, char *dst, socklen_t size)
{
	struct in_addr in_addr;
	if (! dst || size < INET_ADDRSTRLEN) {
		// be strict, keeps it simple
		errno = ENOSPC;
		return "ENOSPC";	// callers just use in a printf
	}
	*dst = '\0';	// be safe
	in_addr.s_addr = __cpu_to_be32(ip_addr);
	// we show the IPv4 address and port
	inet_ntop(AF_INET, &in_addr, dst, size);
	return dst;
}

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

// used for IPv4 netmask processing.  A valid netmask has a sequence of 1s
// and then all other bits are 0.
// This counts how many 1s are in the high end of the netmask and confirms
// the remaining low bits are 0.
int psmi_count_high_bits(uint32_t netmask)
{
	int i=0;
	uint32_t mask = 0x80000000;
	while (mask & netmask) {
		i++; mask >>= 1;
	}
	// confirm all low bits of netmask are 0
	if (netmask != psmi_bit_count_to_mask(i))
			return -1;
	return i;
}

// given an IPv4 address, figure out which ifconfig entry matches and
// return the netmask
int psmi_get_eth_netmask(__be32 ip_addr, __be32 *netmask)
{
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) == 0) {
		for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
			struct sockaddr_in *addr = (struct sockaddr_in  *)ifa->ifa_addr;
			struct sockaddr_in *nmask = (struct sockaddr_in  *)ifa->ifa_netmask;
			__be32 nm;
			char buf[INET_ADDRSTRLEN];
			char buf2[INET_ADDRSTRLEN];

			if (!nmask) continue;
			if (addr->sin_family != AF_INET) continue;
			if (addr->sin_addr.s_addr != ip_addr) continue;

			nm = (__be32)nmask->sin_addr.s_addr;

			if (_HFI_DBG_ON) {
				_HFI_DBG("Related ifaddr[%s]: %s netmask %s\n",
					ifa->ifa_name,
                	psmi_ipv4_ntop(__be32_to_cpu(ip_addr), buf, sizeof(buf)),
                	psmi_ipv4_ntop(__be32_to_cpu(nm), buf2, sizeof(buf2)));
			}
			*netmask = nm;
			break;
		}
		(void)freeifaddrs(ifap);
	} else {
		return -1;
	}
	return 0;
}

/* Wrapper, in case we port to OS xyz that doesn't have sysconf */
uintptr_t psmi_getpagesize(void)
{
	static uintptr_t pagesz = (uintptr_t) -1;
	long sz;
	if (pagesz != (uintptr_t) -1)
		return pagesz;
	sz = sysconf(_SC_PAGESIZE);
	if (sz == -1) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Can't query system page size");
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
static int psmi_parse_val_pattern(const char *env, int def, int def_syntax)
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
				if (hfi_get_myrank() != 0)
					ret = def;
			} else if (0 != fnmatch(p+1, hfi_get_mylabel(),  0
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
static int psmi_getenv_is_verblevel(int printlevel)
{
	if (psmi_getenv_verblevel == -1) {
		char *env = getenv("PSM3_VERBOSE_ENV");
		int nlevel = PSMI_ENVVAR_LEVEL_USER;
		psmi_getenv_verblevel = psmi_parse_val_pattern(env, 0, 2);
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
			if (psmi_getenv_is_verblevel(nlevel))		\
				nlevel = 0; /* output at INFO level */	\
			_HFI_ENVDBG(nlevel, _fmt, ##__VA_ARGS__);	\
		}							\
	} while (0)

int
MOCKABLE(psmi_getenv)(const char *name, const char *descr, int level,
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
		(void)psmi_getenv_is_verblevel(level);			\
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
MOCK_DEF_EPILOGUE(psmi_getenv);

/*
 * Parsing long parameters
 * -1 -> parse error
 */
long psmi_parse_str_long(const char *string)
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
int psmi_parse_str_tuples(const char *string, int ntup, int *vals)
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
int psmi_parse_memmode(void)
{
	union psmi_envvar_val env_mmode;
	int used_default =
	    psmi_getenv("PSM3_MEMORY", "Memory usage mode (min, normal or large)",
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

/* RDMA mode */
// we need this early when setting defaults for RV thresholds in psmi_mq_malloc
// and also want this available when creating the verbs_ep since it may affect
// sizing of CQs and buffers.  But during mq_malloc we don't have an ep or proto
// to save this into
// The value returned is a bitmask of IPS_PROTOEXP_FLAG_* selections
unsigned psmi_parse_rdmamode(void)
{
	union psmi_envvar_val env_rdma;
	static unsigned saved_rdmamode = 0xffffffff;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (saved_rdmamode != 0xffffffff)
		return saved_rdmamode;

	psmi_getenv("PSM3_RDMA",
		    "RDMA proto control (0-no RDMA,"
#ifdef RNDV_MOD
			" 1-kernel RDMA,"
#endif
			" 2-user RDMA, 3-user RC send/RDMA) "
			//" additional flags: 8-interleave, 0x10-serialize"
			// IPS_PROTOEXP_FLAG_TID_DEBUG (0x4)      N/A
			,
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)IPS_PROTOEXP_FLAGS_DEFAULT,
		    &env_rdma);
#ifndef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(env_rdma.e_uint))
		env_rdma.e_uint = 0;
#endif
	saved_rdmamode = env_rdma.e_uint;
	return saved_rdmamode;
}

/* PSM3_IDENTIFY */
// we need in multiple places
int psmi_parse_identify(void)
{
	union psmi_envvar_val myenv;
	static int saved_identify = -1;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (saved_identify >= 0)
		return saved_identify;

	psmi_getenv("PSM3_IDENTIFY", "Identify PSM version being run "
				"(0 - disable, 1 - enable, 1: - limit output to rank 0, "
				"1:pattern - limit output "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
		    	PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		    	(union psmi_envvar_val)"0", &myenv);
	saved_identify = psmi_parse_val_pattern(myenv.e_str, 0, 0);

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
psmi_parse_mpool_env(const psm2_mq_t mq, int level,
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

	psmi_getenv(rlim->env, rlim->descr, rlim->env_level,
		    PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)rlim->mode[mode].obj_max, &env_val);

	val = env_val.e_uint;
	if (val < rlim->minval || val > rlim->maxval) {
		err = psmi_handle_error(NULL, PSM2_PARAM_ERR,
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

uint64_t psmi_cycles_left(uint64_t start_cycles, int64_t timeout_ns)
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

uint32_t psmi_get_ipv4addr()
{
	struct hostent *he;
	uint32_t addr = 0;

	he = gethostbyname(psmi_gethostname());
	if (he != NULL && he->h_addrtype == AF_INET && he->h_addr != NULL) {
		memcpy(&addr, he->h_addr, sizeof(uint32_t));
		return addr;
	} else
		return 0;
}

#define PSMI_EP_IS_PTR(ptr)	    ((ptr) != NULL && (ptr) < PSMI_EP_LOGEVENT)

void
psmi_syslog(psm2_ep_t ep, int to_console, int level, const char *format, ...)
{
	va_list ap;

	/* If we've never syslogged anything from this ep at the PSM level, make
	 * sure we log context information */
	if (PSMI_EP_IS_PTR(ep) && !ep->did_syslog) {
		char uuid_str[64];
		ep->did_syslog = 1;

		memset(&uuid_str, 0, sizeof(uuid_str));
		psmi_uuid_unparse(ep->uuid, uuid_str);
		hfi_syslog("PSM", 0, LOG_WARNING,
			   "uuid_key=%s,unit=%d"
			   ,
			   uuid_str,
			   ep->unit_id
			   );
	}

	va_start(ap, format);
	hfi_vsyslog("PSM", to_console, level, format, ap);
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
uint32_t psmi_crc(unsigned char *buf, int len)
{
	return update_crc(0xffffffff, buf, len) ^ 0xffffffff;
}

int psmi_multi_ep_enabled = 0;
void psmi_multi_ep_init()
{
	union psmi_envvar_val env_fi;

	psmi_getenv("PSM3_MULTI_EP", "PSM3 Multiple Endpoints (yes/no)",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_YESNO,
		    PSMI_ENVVAR_VAL_YES, &env_fi);

	psmi_multi_ep_enabled = env_fi.e_uint;
}

#ifdef PSM_FI

struct psmi_faultinj_spec {
	STAILQ_ENTRY(psmi_faultinj_spec) next;
	char spec_name[PSMI_FAULTINJ_SPEC_NAMELEN];

	uint64_t num_faults;
	uint64_t num_calls;

	struct drand48_data drand48_data;
	int num;
	int denom;
	long int initial_seed;
};

int psmi_faultinj_enabled = 0;
int psmi_faultinj_verbose = 0;
char *psmi_faultinj_outfile = NULL;
int psmi_faultinj_sec_rail = 0;

static struct psmi_faultinj_spec psmi_faultinj_dummy;
static STAILQ_HEAD(, psmi_faultinj_spec) psmi_faultinj_head =
		STAILQ_HEAD_INITIALIZER(psmi_faultinj_head);
int psmi_faultinj_num_entries;

void psmi_faultinj_init()
{
	union psmi_envvar_val env_fi;

	psmi_getenv("PSM3_FI", "PSM Fault Injection "
				"(0 - disable, 1 - enable, 1: - limit to rank 0, "
				"1:pattern - limit "
				"to processes whose label matches "
#ifdef FNM_EXTMATCH
				"extended "
#endif
				"glob pattern)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)"0", &env_fi);
	psmi_faultinj_enabled = psmi_parse_val_pattern(env_fi.e_str, 0, 0);

	if (psmi_faultinj_enabled) {
		char *def = NULL;
		if (!psmi_getenv
		    ("PSM3_FI_TRACEFILE", "PSM Fault Injection output file",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
		     (union psmi_envvar_val)def, &env_fi)) {
			psmi_faultinj_outfile = psmi_strdup(NULL, env_fi.e_str);
		}
		if (!psmi_getenv
		    ("PSM3_FI_VERBOSE", "PSM Fault verbose output",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		     (union psmi_envvar_val)0, &env_fi)) {
			psmi_faultinj_verbose = env_fi.e_int;
		}
		if (!psmi_getenv
		    ("PSM3_FI_RAIL", "PSM Fault Injection rail (0=all, 1=secondary only)",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		     (union psmi_envvar_val)0, &env_fi)) {
			psmi_faultinj_sec_rail = env_fi.e_int;
		}
	}

	return;
}

/* we only grow new entries, so if we fail to allocate, just ignore request */
static void psmi_faultinj_reregister_stats()
{
	struct psmi_stats_entry *entries;
	struct psmi_stats_entry *e;
	int num_entries = 0;
	struct psmi_faultinj_spec *fi;

	entries = psmi_calloc(PSMI_EP_NONE, STATS, psmi_faultinj_num_entries,
			 		sizeof(struct psmi_stats_entry));
	if (! entries)
		return;
	e = entries;
	STAILQ_FOREACH(fi, &psmi_faultinj_head, next) {
		psmi_stats_init_u64(e, fi->spec_name, &fi->num_faults);
		e++; num_entries++;
	}

	psmi_stats_reregister_type("Fault_Injection", PSMI_STATSTYPE_FAULTINJ,
		entries, num_entries, 0, &psmi_faultinj_head);
	psmi_free(entries);
}

void psmi_faultinj_fini()
{
	struct psmi_faultinj_spec *fi;
	FILE *fp;
	int do_fclose = 0;

	if (!psmi_faultinj_enabled)
		return;
	psmi_stats_deregister_type(PSMI_STATSTYPE_FAULTINJ, &psmi_faultinj_head);

	if (psmi_faultinj_outfile == NULL)
		return;
	if (strncmp(psmi_faultinj_outfile, "stdout", 7) == 0)
		fp = stdout;
	else if (strncmp(psmi_faultinj_outfile, "stderr", 7) == 0)
		fp = stderr;
	else {
		char *c = psmi_faultinj_outfile;
		char buf[192];
		int append = 0;
		if (*c == '+') {
			append = 1;
			++c;
		}
		do_fclose = 1;
		snprintf(buf, sizeof(buf) - 1, "%s.%s", c, hfi_get_mylabel());
		buf[sizeof(buf) - 1] = '\0';
		fp = fopen(buf, append ? "a" : "w");
	}

	if (fp != NULL) {
		STAILQ_FOREACH(fi, &psmi_faultinj_head, next) {
			fprintf(fp, "%s:%s PSM3_FI_%-13s %2.3f%% => "
				"%2.3f%% %10"PRIu64" faults/%10"PRIu64" events seed %10ld\n",
				__progname, hfi_get_mylabel(), fi->spec_name,
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

	psmi_free(psmi_faultinj_outfile);
	return;
}

/*
 * Intended to be used only once, not in the critical path
 */
struct psmi_faultinj_spec *psmi_faultinj_getspec(const char *spec_name,
						 const char *help, int num,
						 int denom)
{
	struct psmi_faultinj_spec *fi;

	if (!psmi_faultinj_enabled)
		return &psmi_faultinj_dummy;

	STAILQ_FOREACH(fi, &psmi_faultinj_head, next) {
		if (strcmp(fi->spec_name, spec_name) == 0)
			return fi;
	}

	/* We got here, so no spec -- allocate one */
	fi = psmi_malloc(PSMI_EP_NONE, UNDEFINED,
			 sizeof(struct psmi_faultinj_spec));
	psmi_assert_always(fi != NULL);
	strncpy(fi->spec_name, spec_name, PSMI_FAULTINJ_SPEC_NAMELEN - 1);
	fi->spec_name[PSMI_FAULTINJ_SPEC_NAMELEN - 1] = '\0';
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

		snprintf(fvals_str, sizeof(fvals_str) - 1, "%d:%d:1", num,
			 denom);
		fvals_str[sizeof(fvals_str) - 1] = '\0';
		snprintf(fname, sizeof(fname) - 1, "PSM3_FI_%s", spec_name);
		fname[sizeof(fname) - 1] = '\0';
		snprintf(fdesc, sizeof(fdesc) - 1, "Fault Injection - %s <%s>",
			 help, fvals_str);

		if (!psmi_getenv(fname, fdesc, PSMI_ENVVAR_LEVEL_HIDDEN,
				 PSMI_ENVVAR_TYPE_STR,
				 (union psmi_envvar_val)fvals_str, &env_fi)) {
			/* not using default values */
			int n_parsed =
			    psmi_parse_str_tuples(env_fi.e_str, 3, fvals);
			if (n_parsed >= 1)
				fi->num = fvals[0];
			if (n_parsed >= 2)
				fi->denom = fvals[1];
			if (n_parsed >= 3)
				fi->initial_seed = (long int)fvals[2];
		}
	}
	srand48_r(fi->initial_seed, &fi->drand48_data);

	psmi_faultinj_num_entries++;
	STAILQ_INSERT_TAIL(&psmi_faultinj_head, fi, next);
	psmi_faultinj_reregister_stats();
	return fi;
}

int psmi_faultinj_is_fault(struct psmi_faultinj_spec *fi)
{
	if (!psmi_faultinj_enabled)	/* never fault if disabled */
		return 0;
	if (fi->num == 0)
		return 0;

	fi->num_calls++;
	long int rnum;
	lrand48_r(&fi->drand48_data, &rnum);
	if (((int) (rnum % INT_MAX)) % fi->denom <= fi->num) {
		fi->num_faults++;
		if (psmi_faultinj_verbose) {
			printf("%s: injecting fault: %s\n", hfi_get_mylabel(), fi->spec_name);
			fflush(stdout);
		}
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

struct psmi_stats_malloc psmi_stats_memory;

void psmi_mem_stats_register(void)
{
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECLU64("Total_(current)",
				(uint64_t*)&psmi_stats_memory.m_all_total),
		PSMI_STATS_DECLU64("Total_(max)",
				(uint64_t*)&psmi_stats_memory.m_all_max),
		PSMI_STATS_DECLU64("All_Peers_(current)",
				(uint64_t*)&psmi_stats_memory.m_perpeer_total),
		PSMI_STATS_DECLU64("All_Peers_(max)",
				(uint64_t*)&psmi_stats_memory.m_perpeer_max),
		PSMI_STATS_DECLU64("Network_Buffers_(current)",
				(uint64_t*)&psmi_stats_memory.m_netbufs_total),
		PSMI_STATS_DECLU64("Network_Buffers_(max)",
				(uint64_t*)&psmi_stats_memory.m_netbufs_max),
		PSMI_STATS_DECLU64("PSM_desctors_(current)",
				(uint64_t*)&psmi_stats_memory.m_descriptors_total),
		PSMI_STATS_DECLU64("PSM_desctors_(max)",
				(uint64_t*)&psmi_stats_memory.m_descriptors_max),
		PSMI_STATS_DECLU64("Unexp._buffers_(current)",
				(uint64_t*)&psmi_stats_memory.m_unexpbufs_total),
		PSMI_STATS_DECLU64("Unexp._Buffers_(max)",
				(uint64_t*)&psmi_stats_memory.m_unexpbufs_max),
#ifdef RNDV_MOD
		PSMI_STATS_DECLU64("Peer_Rndv_(current)",
				(uint64_t*)&psmi_stats_memory.m_peerrndv_total),
		PSMI_STATS_DECLU64("Peer_Rndv_(max)",
				(uint64_t*)&psmi_stats_memory.m_peerrndv_max),
#endif
		PSMI_STATS_DECLU64("statistics_(current)",
				(uint64_t*)&psmi_stats_memory.m_stats_total),
		PSMI_STATS_DECLU64("statistics_(max)",
				(uint64_t*)&psmi_stats_memory.m_stats_max),
		PSMI_STATS_DECLU64("Other_(current)",
				(uint64_t*)&psmi_stats_memory.m_undefined_total),
		PSMI_STATS_DECLU64("Other_(max)",
				(uint64_t*)&psmi_stats_memory.m_undefined_max),
	};

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		psmi_stats_register_type("PSM_memory_allocation_statistics",
                    PSMI_STATSTYPE_MEMORY,
                    entries,
                    PSMI_STATS_HOWMANY(entries), 0, &psmi_stats_memory);
	}
}


void psmi_log_memstats(psmi_memtype_t type, int64_t nbytes)
{
#define _add_max_total(type, nbytes)				\
	psmi_stats_memory.m_ ## type ## _total += (nbytes);	\
	psmi_stats_memory.m_ ## type ## _max = max(		\
	    psmi_stats_memory.m_ ## type ## _total,		\
	    psmi_stats_memory.m_ ## type ## _max);

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
#ifdef RNDV_MOD
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
	psmi_stats_memory.m_all_max++;
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

void *psmi_malloc_internal(psm2_ep_t ep, psmi_memtype_t type,
			   size_t sz, const char *curloc)
{
	size_t newsz = sz;
	void *newa;

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY)
	    newsz += sizeof(struct psmi_memtype_hdr);

	newa = my_malloc(newsz,curloc);
	if (newa == NULL) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
				  "Out of memory for malloc at %s", curloc);
		return NULL;
	}

	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		struct psmi_memtype_hdr *hdr = (struct psmi_memtype_hdr *)newa;
		hdr->size = newsz;
		hdr->type = type;
		hdr->magic = 0x8c;
		hdr->original_allocation = newa;
		psmi_log_memstats(type, newsz);
		newa = (void *)(hdr + 1);
		/* _HFI_INFO("alloc is %p\n", newa); */
	}
	return newa;
}

void *psmi_realloc_internal(psm2_ep_t ep, psmi_memtype_t type,
			    void *ptr, size_t nsz, const char *curloc)
{
	if (ptr)
	{
		size_t existingSize = psmi_malloc_usable_size_internal(ptr,curloc);
		if (nsz > existingSize)
		{
			void *newPtr = psmi_malloc_internal(ep,type,nsz,curloc);

			memcpy(newPtr,ptr,existingSize);
			psmi_free_internal(ptr,curloc);
			return newPtr;
		}
		else
			/* We will not support shrinking virtual space
			   for performance reasons. */
			return ptr;
	}
	else
		return psmi_malloc_internal(ep,type,nsz,curloc);
}

#ifdef memalign
#undef memalign
#endif
void *psmi_memalign_internal(psm2_ep_t ep, psmi_memtype_t type,
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
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_NO_MEMORY,
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
		psmi_log_memstats(type, newsz);
		newa = rv;
		/* _HFI_INFO("alloc is %p\n", newa); */
	}
	return newa;
}

#ifdef calloc
#undef calloc
#endif

void *psmi_calloc_internal(psm2_ep_t ep, psmi_memtype_t type, size_t nelem,
			   size_t elemsz, const char *curloc)
{
	void *newa = psmi_malloc_internal(ep, type, nelem * elemsz, curloc);
	if (newa == NULL)	/* error handled above */
		return NULL;
	memset(newa, 0, nelem * elemsz);
	return newa;
}

#ifdef strdup
#undef strdup
#endif

void *psmi_strdup_internal(psm2_ep_t ep, const char *string, const char *curloc)
{
	size_t len = strlen(string) + 1;
	void *newa = psmi_malloc_internal(ep, UNDEFINED, len, curloc);
	if (newa == NULL)
		return NULL;
	memcpy(newa, string, len);	/* copy with \0 */
	return newa;
}

#ifdef free
#undef free
#endif

void MOCKABLE(psmi_free_internal)(void *ptr,const char *curloc)
{
	if_pf(psmi_stats_mask & PSMI_STATSTYPE_MEMORY) {
		struct psmi_memtype_hdr *hdr =
		    (struct psmi_memtype_hdr *)ptr - 1;
		/* _HFI_INFO("hdr is %p, ptr is %p\n", hdr, ptr); */
		psmi_memtype_t type = hdr->type;
		int64_t size = hdr->size;
		int magic = (int)hdr->magic;
		psmi_log_memstats(type, -size);
		psmi_assert_always(magic == 0x8c);
		ptr = hdr->original_allocation;
	}
	my_free(ptr,curloc);
}
MOCK_DEF_EPILOGUE(psmi_free_internal);

#ifdef malloc_usable_size
#undef malloc_usable_size
#endif

size_t psmi_malloc_usable_size_internal(void *ptr, const char *curLoc)
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
			err =  psmi_handle_error(NULL,
					PSM2_PARAM_ERR,
					"Option value length error");
			*optlen = sizeof(unsigned);
			return err;
		}

		if (get) {
			*((unsigned *)optval) = hfi_debug;
		} else
			hfi_debug = *(unsigned *)optval;
		break;
	case PSM2_CORE_OPT_EP_CTXT:
		{
			/* core object is epaddr */
			psm2_epaddr_t epaddr = (psm2_epaddr_t) core_obj;

			/* Sanity check epaddr */
			if (!epaddr) {
				return psmi_handle_error(NULL,
						PSM2_PARAM_ERR,
						"Invalid endpoint address");
			}

			/* Sanity check length */
			if (*optlen < sizeof(unsigned long)) {
				err =  psmi_handle_error(NULL,
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
		err = psmi_handle_error(NULL,
				PSM2_PARAM_ERR,
				"Unknown PSM3_CORE option %u.",
				optname);
		break;
	}
	return err;
}

psm2_error_t psmi_core_setopt(const void *core_obj, int optname,
			     const void *optval, uint64_t optlen)
{
	return psmi_coreopt_ctl(core_obj, optname, (void *)optval, &optlen, 0);
}

psm2_error_t psmi_core_getopt(const void *core_obj, int optname,
			     void *optval, uint64_t *optlen)
{
	return psmi_coreopt_ctl(core_obj, optname, optval, optlen, 1);
}

/* PSM AM component option handling */
PSMI_ALWAYS_INLINE(
psm2_error_t
psmi_amopt_ctl(const void *am_obj, int optname,
	       void *optval, uint64_t *optlen, int get))
{
	psm2_error_t err = PSM2_OK;

	/* AM object is a psm2_epaddr (or NULL for global minimum sz) */
	/* psm2_epaddr_t epaddr = (psm2_epaddr_t) am_obj; */

	/* All AM options are read-only. */
	if (!get) {
		return err =
		    psmi_handle_error(PSMI_EP_LOGEVENT, PSM2_OPT_READONLY,
				      "Attempted to set read-only option value");
	}

	/* Sanity check length -- all AM options are uint32_t. */
	if (*optlen < sizeof(uint32_t)) {
		*optlen = sizeof(uint32_t);
		return err = psmi_handle_error(PSMI_EP_LOGEVENT, PSM2_PARAM_ERR,
					       "Option value length error");
	}

	switch (optname) {
	case PSM2_AM_OPT_FRAG_SZ:
		*((uint32_t *) optval) = psmi_am_parameters.max_request_short;
		break;
	case PSM2_AM_OPT_NARGS:
		*((uint32_t *) optval) = psmi_am_parameters.max_nargs;
		break;
	case PSM2_AM_OPT_HANDLERS:
		*((uint32_t *) optval) = psmi_am_parameters.max_handlers;
		break;
	default:
		err =
		    psmi_handle_error(NULL, PSM2_PARAM_ERR,
				      "Unknown PSM3_AM option %u.", optname);
	}

	return err;
}

psm2_error_t psmi_am_setopt(const void *am_obj, int optname,
			   const void *optval, uint64_t optlen)
{
	return psmi_amopt_ctl(am_obj, optname, (void *)optval, &optlen, 0);
}

psm2_error_t psmi_am_getopt(const void *am_obj, int optname,
			   void *optval, uint64_t *optlen)
{
	return psmi_amopt_ctl(am_obj, optname, optval, optlen, 1);
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
	uint64_t fromepid,toepid;
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
static int compareEpmNode(epmTreeNode *node,int opcode,int txrx,uint64_t fromepid,uint64_t toepid)
{
#define COMPARE_ONE(X) if (node->X != X) return node->X < X ? -1 : 1
	COMPARE_ONE(opcode);
	COMPARE_ONE(txrx);
	COMPARE_ONE(fromepid);
	COMPARE_ONE(toepid);
	return 0;
}

/* Inserts a new node in the tree corresponding to the parameters, or, retrieves the node in the tree.
   In either case, this code returns a pointer to the count in the node. */
static int *insertNodeInEpmTree(epmTreeNode **root,int opcode,int txrx,uint64_t fromepid,uint64_t toepid)
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
static const char * const TxRxString(int txrx)
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
static const char * const OpcodeString(int opcode)
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

static const char     *plmf_fileName_kernel         = "/tmp/psm2_log";
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
		uint64_t         fromepid    = 0;
		uint64_t         toepid      = 0;
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
			fromepid  = va_arg(ap,uint64_t);
			toepid    = va_arg(ap,uint64_t);
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
			if (txrx == PSM2_LOG_RX)
			{
				dumpAddr[1] = va_arg(ap,uint32_t *);
				dumpSize[1] = sizeof(uint64_t);
			}
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
			MY_FPRINTF(IO_PORT,"%s %s from: %" PRIx64
				   ", to: %" PRIx64 ", count: %d, ",
				   TxRxString(txrx),OpcodeString(opcode),
				   fromepid,toepid,*pcount);
			MY_VFPRINTF(IO_PORT,newFormat,ap);
			MY_FPUTC('\n',IO_PORT);
		}
		else if (format == PSM2_LOG_PKT_STRM_MAGIC)
		{
			MY_FPRINTF(IO_PORT,"PKT_STRM: %s: imh: %p%s ", TxRxString(txrx),
				   dumpAddr[0], (txrx == PSM2_LOG_RX) ? "," : "");
			if (txrx == PSM2_LOG_RX)
				MY_FPRINTF(IO_PORT,"rhf: %p ", dumpAddr[1]);
			goto dumpit;
		}
		else if (format == PSM2_LOG_DUMP_MAGIC)
		{
			MY_VFPRINTF(IO_PORT,newFormat,ap);
			MY_FPUTC('\n',IO_PORT);
		dumpit:
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
				goto dumpit;
			}
		}
		MY_FCLOSE(IO_PORT);
	}

	va_end(ap);
}
#endif /* #ifdef PSM_LOG */
