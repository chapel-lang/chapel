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

#ifndef _PSMI_NETUTILS_H
#define _PSMI_NETUTILS_H

#include <arpa/inet.h>		/* ipv4addr */
#include <stdlib.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <psm_help.h>

// network function subset of psm_utils.c so that HAL can use this without
// needing psm_ep_t, psm_epid_t, and psm2_nid_t from psm_user.h

#ifdef PSM_TCP_IPV4
typedef struct sockaddr_in psm3_sockaddr_in_t;
#define psm3_socket_port(in) (in)->sin_port
#define psm3_socket_domain AF_INET
#else
typedef struct sockaddr_in6 psm3_sockaddr_in_t;
#define psm3_socket_port(in) (in)->sin6_port
#define psm3_socket_domain AF_INET6
#endif

/* a bare 128 bit network address, such as a verbs GID or IPv6 address
 * stored in host byte order.  Mainly for use internal to low level utils.
 * Instead use psmi_gid128_t, psmi_naddr128_t or psmi_subnet128_t where
 * possible.
 */
typedef struct psmi_bare_netaddr28 {
	uint64_t lo;
	uint64_t hi;
} psmi_bare_netaddr128_t;

/* a 128 bit network address qualified with addr_fmt and prefix_len
 * stored in host byte order.  Mainly for use internal to low level utils.
 * Instead use psmi_naddr128_t or psmi_subnet128_t where possible.
 */
typedef struct psmi_qual_netaddr128 {
	psmi_bare_netaddr128_t bare;
	uint8_t fmt;
	uint8_t prefix_len;
} psmi_qual_netaddr128_addr_t;

// full gid, for IPv4 this is ::ffff:<addr>
// for IB/OPA/iPATH/IPv6 it is same as addr
// Note for all formats: (gid | addr) == gid
typedef psmi_bare_netaddr128_t psmi_gid128_t;

// in naddr128 and subnet128 the prefix_len is:
// OPA/iPATH/IB: always 64
// IPv4: 1-31
// IPv6: 1-128

// OPA/iPATH: full 128b GID
// IB: full 128b GID
// IPv4: full 32b IPv4 addr in lo, 0 in hi
// IPv6: full 128b IPv6 address
// Note for all formats: (subnet | addr) == addr
typedef psmi_qual_netaddr128_addr_t psmi_naddr128_t;

// OPA/iPATH: 64b subnet_prefix in hi, 0 in lo
// IB: 64b subnet_prefix in hi, 0 in lo
// IPv4: 32b IPv4 subnet in lo, 0 in hi
// IPv6: 128b subnet_prefix
typedef psmi_qual_netaddr128_addr_t psmi_subnet128_t;

static inline psmi_bare_netaddr128_t psmi_bare_netaddr128_and(
			psmi_bare_netaddr128_t a, psmi_bare_netaddr128_t b)
{
	psmi_bare_netaddr128_t ret;
	ret.hi = a.hi & b.hi;
	ret.lo = a.lo & b.lo;
	return ret;
}

// IPv4 GID (RoCEv2, UDP) <=> ::ffff:<ipv4>
// These macros require ip_addr and GIDs in host byte order
// compose an IPv6 style GID from an IPv4 address
#define PSMI_IPV4_GID_HI(ip_addr) ((uint64_t)0ULL)
#define PSMI_IPV4_GID_LO(ip_addr) ((uint64_t)0xffff00000000ULL | (ip_addr))

static inline int psmi_is_ipv4_gid(psmi_gid128_t gid)
{
	return (gid.hi == 0x0 && (gid.lo >> 32) == 0x0000ffff);
}

static inline uint32_t psmi_ipv4_from_gid(psmi_gid128_t gid)
{
	return  (uint32_t)(gid.lo & 0xffffffff);
}

// compares an AF_INET6 sock_addr against a GID (as returned by get_port_subnet)
// The GID may contain an IPv4 ::ffff:addr or an IPv6 address
// sockaddr in big endian.
static inline int
	psmi_ip_equal_gid(const psm3_sockaddr_in_t *s, psmi_gid128_t gid)
{
#ifdef PSM_TCP_IPV4
	return (s->sin_family == AF_INET
	  && __be32_to_cpu(s->sin_addr.s_addr) == psmi_ipv4_from_gid(gid));
#else
	return (s->sin6_family == AF_INET6
	  && __be32_to_cpu(s->sin6_addr.s6_addr32[0]) == (gid.hi >> 32)
	  && __be32_to_cpu(s->sin6_addr.s6_addr32[1]) == (gid.hi & 0xffffffff)
	  && __be32_to_cpu(s->sin6_addr.s6_addr32[2]) == (gid.lo >> 32)
	  && __be32_to_cpu(s->sin6_addr.s6_addr32[3]) == (gid.lo & 0xffffffff));
#endif
}



// PSM3_ADDR_FMT sets this value, default of PSMI_ADDR_FMT_DEFAULT
// 0 = auto selects per NIC/rail from FMT_IB, FMT_IPV4 or FMT_IPV6
// 	IB/OPA always select 1st GID for subnet and address
// 	Ethernet prefers 1st IPv4 address, 2nd choice is 1st IPv6 address
// FMT_IB, FMT_IPV4 or FMT_IPV6 forces all NIC/rail to select only ports
//	with the specified address (OPA/IB, IPv4 or IPv6)
extern uint8_t psm3_addr_fmt;	// PSM3_ADDR_FMT

// PSM3_ADDR_PER_NIC sets this value, default of 1
extern unsigned int psm3_addr_per_nic;

#define PSMI_ADDR_FMT_SHM 		0	// shm-only or self-only
#define PSMI_ADDR_FMT_IB		3	// IB/OPA UD Verbs
#define PSMI_ADDR_FMT_IPV4		4	// Eth UD/UDP IPv4
					//5	// unused
#define PSMI_ADDR_FMT_IPV6		6	// Eth UD/UDP IPv6

#define PSMI_MAX_ADDR_FMT_SUPPORTED	6
#define PSMI_MIN_ADDR_FMT_SUPPORTED	3
#define PSMI_ADDR_FMT_DEFAULT		0	// allows 3, 4 or 6 based on NIC

#define	PSMI_IPS_ADDR_FMT_IS_VALID(addr_fmt) ((addr_fmt) == PSMI_ADDR_FMT_IB \
					|| (addr_fmt) == PSMI_ADDR_FMT_IPV4 \
					|| (addr_fmt) == PSMI_ADDR_FMT_IPV6)
#define	PSMI_ADDR_FMT_IS_ETH(addr_fmt) ((addr_fmt) == PSMI_ADDR_FMT_IPV4 \
					|| (addr_fmt) == PSMI_ADDR_FMT_IPV6)

// For Ethernet NICs, protocol is used to detect the basic protocol for the
// NICs selected.  When defaulted on Ethernet we could select NICs with
// incompatible protocols (RoCE vs UDP vs TCP) on some nodes and want to detect
// that early in job launch without waiting for connect timeouts.
// HW specific HALs (verbs or sockets) must negotiate further details or
// features during connection establishment.
// For sockets NICs we distinguish UDP vs TCP to ensure incompatibilities
// during connection establishement are detected quickly. Although all
// nodes should have the same basic UDP vs TCP env variable selections,
// there can be build time selections and this detects any build inconsistency.
// For ADDR_FMT_IB protocol is N/A. we assume subnet will imply connectivity
// and a verbs HAL (perhaps HW specific) will be used.  As such IB and OPA
// devices should not be given the same subnet prefix when they are planes
// in the same cluster.
typedef enum {
	PSMI_ETH_PROTO_ROCE	=0,	// RoCE
	PSMI_ETH_PROTO_UDP	=1,	// UDP
	PSMI_ETH_PROTO_TCP	=2,	// TCP
	PSMI_ETH_PROTO_NA	=0xff	// internal only when N/A
} psmi_eth_proto_t;

// build addresses from basic information, mostly for use in low level
// routines like get_port_subnet in HAL
psmi_subnet128_t psm3_build_ib_subnet128(uint64_t hi);
psmi_naddr128_t psm3_build_ib_naddr128(psmi_gid128_t gid);
psmi_subnet128_t psm3_build_ipv4_subnet128(uint32_t ip_addr,
		uint32_t ip_netmask, uint8_t prefix_len);
psmi_naddr128_t psm3_build_ipv4_naddr128(uint32_t ip_addr, uint8_t prefix_len);
psmi_subnet128_t psm3_build_ipv6_subnet128(psmi_bare_netaddr128_t ipv6_addr,
		psmi_bare_netaddr128_t ipv6_netmask, uint8_t prefix_len);
psmi_naddr128_t psm3_build_ipv6_naddr128(psmi_bare_netaddr128_t ip_addr,
		uint8_t prefix_len);

// PSM3_SUBNETS specifies a comma separated list of Ethernet subnets which will
// be considered for Ethernet ports.  Ports which do not match any of the
// entries will not be considered for use by PSM3.
// Each entry is a extended glob pattern style string as supported by
// fnmatch(PSM3_SUBNETS, subnet, FNM_EXTMATCH).
// Each of the specified subnet patterns are compared to the subnets for the
// port's GIDs as output by inet_ntop for IPv4 GIDs and inet6_ntop for IPv6 GIDs
// plus a /prefix_len suffix
// Note that due to differences in format style (dots for IPv4 vs : for IPv6)
// a given subnet in PSM3_SUBNETS will typically only match one type of address.
// If a subnet begins with ^, this means all subnets matching the pattern
// will not be considered for use by PSM3.
// Entries in the list are processed in order and once a GID has been matched
// by an entry, processing is completed.  GIDs are also processed in order and
// once a acceptable matching GID is found, other GIDs are not examined.
// Examples:
//	PSM3_SUBNETS=^fe[89ab]?:*/*,*
//		- the default.  Ignore IPv6 link-local prefix, accept all others
//		- Note a link local address is any IPv6 address whose upper 10
//		  bits are binary 1111 1110 10xx  (HEX F E 8-B)
//	PSM3_SUBNETS=^fe[89ab]?:*/*,192.168.100.0/24,fd*
//		- Ignore IPv6 link local prefix
//		- allow IPv4 subnet 192.168.100.0/24
//		- allow IPv6 subnets which begin with fd
//		- Ignore all other subnets
// This env variable is most useful to filter and select subnets when ports
// have more than 1 valid IP address.
// The filtering is applied after PSM3_ADDR_FMT has potentially filtered out
// which types of addresses should be considered.
#define PSMI_MAX_SUBNETS 64
extern char *psm3_allow_subnets[PSMI_MAX_SUBNETS];     // PSM3_SUBNETS
extern int psm3_num_allow_subnets;
#define PSMI_SUBNETS_DEFAULT "^fe[89ab]?:*/*,*"

// return 1 is subnet is allowed, 0 if subnet should be ignored
int psm3_allow_ib_subnet(uint64_t subnet);
int psm3_allow_ipv4_subnet(uint32_t subnet, uint8_t prefix_len);
int psm3_allow_ipv6_subnet(psmi_bare_netaddr128_t subnet, uint8_t prefix_len);

// variable to store NIC name wildcard if specified (def. psm.c)
extern const char *psm3_nic_wildcard;

/*
 * returns 1 if the unit name matches the glob pattern
 * (extended glob pattern where applicable) OR if glob
 * pattern is empty. otherwise returns 0
 */
extern int psm3_is_nic_allowed(int unit);

// variable to store NIC speed wildcard if specified (def. psm.c)
extern const char *psm3_nic_speed_wildcard;
extern uint64_t psm3_nic_speed_max_found;

/*
 * returns 1 if the speed matches the psmi_nic_speed_wildcard
 * (extended glob pattern where applicable)
 * otherwise returns 0
 */
int psm3_is_speed_allowed(int unit, uint64_t speed);

/*
 * network address manipulation
 */
const char *psm3_sockaddr_fmt(struct sockaddr* addr, int bufno);
const char *psm3_sockaddr_fmt_addr(struct sockaddr* addr, int bufno);
const char *psm3_ipv4_fmt(uint32_t addr, uint8_t prefix_len, int bufno);
const char *psm3_ipv6_fmt(psmi_bare_netaddr128_t ipv6_addr, uint8_t prefix_len,
				int bufno);
const char *psm3_gid128_fmt(psmi_gid128_t gid, int bufno);
const char *psm3_subnet128_fmt(psmi_subnet128_t subnet, int bufno);
void psm3_subnet128_fmt_name(psmi_eth_proto_t protocol, psmi_subnet128_t subnet,
				char *buf, int buflen);
const char *psm3_naddr128_fmt(psmi_naddr128_t addr, int bufno);

// used for IPv4 netmask processing.  A valid netmask has a sequence of 1s
// and then all other bits are 0.
// This counts how many 1s are in the high end of the netmask and confirms
// the remaining low bits are 0.
// returns 0 if netmask is invalid
uint8_t psm3_compute_ipv4_prefix_len(uint32_t netmask);

// used for IPv6 netmask processing.  A valid netmask has a sequence of 1s
// and then all other bits are 0.
// This counts how many 1s are in the high end of the netmask and confirms
// the remaining low bits are 0.
// returns 0 if netmask is invalid
int psm3_compute_ipv6_prefix_len(psmi_bare_netaddr128_t netmask);

#ifdef PSM_VERBS
// search ifconfig for the given IPv4 ip_addr and return it's netmask
int psm3_get_eth_ipv4_netmask(uint32_t ip_addr, uint32_t *netmask);

// search ifconfig for the given IPv6 ipv6_addr and return it's netmask
int psm3_get_eth_ipv6_netmask(psmi_bare_netaddr128_t ipv6_addr,
				psmi_bare_netaddr128_t *netmask);
#endif
#if 0
// search ifconfig for the given ifname and return it's first IPv6 address
int psm3_get_eth_ipv6(const char *ifname, psmi_bare_netaddr128_t *ipv6_addr);
#endif

#endif /* _PSMI_NETUTILS_H */
