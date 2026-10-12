#ifdef PSM_SOCKETS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#include "psm_user.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#include "utils_user.h"
#include "ips_proto_params.h"
#include "ips_proto_header.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_path_rec.h"
#include "hal_sockets/sockets_service.h"
#include "hal_sockets/sockets_hal.h"
#if defined(PSM_LIBCAP)
#include <sys/capability.h>
#endif

static psm2_error_t udp_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key);
static psm2_error_t tcp_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key);
static psm2_error_t psm3_update_mtu(psm2_ep_t ep);
static psm2_error_t psm3_update_subnet(psm2_ep_t ep);

void psm3_ep_free_sockets(psm2_ep_t ep);
void psm3_ep_sockets_free_buffers(psm2_ep_t ep);
void psm3_dump_sockets_ep(psm2_ep_t ep);

// initialize the ep->sockets_ep portion of the ep for a UDP or TCP NIC
psm2_error_t
psm3_ep_open_udp_internal(psm2_ep_t ep, int unit, int port,
			psm2_uuid_t const job_key,
			uint8_t is_aux)
{
	int flags;
	psm3_sockaddr_in_t loc_addr;
	socklen_t addr_len;
	union psmi_envvar_val env_bdev;
	union psmi_envvar_val env_gso, env_gro;
	union psmi_envvar_val env_zerocopy;
	union psmi_envvar_val env_prate;
	union psmi_envvar_val env_rbuf, env_sbuf;
	int max_rcv_buf;
	int max_snd_buf;

	if (PSM2_OK != udp_open_dev(ep, unit, port, job_key)) {
		// udp_open_dev already posted error.
		goto fail;
	}
	if (!is_aux) {
		if (PSM2_OK != psm3_update_subnet(ep)) {
			goto fail;
		}

		// compute an appropriate PSM payload size based on the UDP MTU
		// and save result into ep->mtu
		if (PSM2_OK != psm3_update_mtu(ep)) {
			goto fail;
		}
	}


	psm3_getenv("PSM3_UDP_BINDTODEVICE",
		"Bind the UDP socket to the device (0 disable)",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val) 0, &env_bdev);
	if (env_bdev.e_int) {
		int so_bd_opt = 1;
#if defined(PSM_LIBCAP)
		int ret_ev = -1, ret_pv = -1;
		cap_flag_value_t cap_net_raw = CAP_CLEAR;
		cap_flag_value_t cap_ev = CAP_CLEAR, cap_pv = CAP_CLEAR;
		cap_t caps = cap_get_proc();
		if (caps) {
			ret_ev = cap_get_flag(caps, CAP_NET_RAW, CAP_EFFECTIVE, &cap_ev);
			ret_pv = cap_get_flag(caps, CAP_NET_RAW, CAP_PERMITTED, &cap_pv);
			if (cap_ev == CAP_SET && cap_pv == CAP_SET && !ret_ev && !ret_pv)
				cap_net_raw = CAP_SET;
			cap_free(caps);
		}
		so_bd_opt = (cap_net_raw == CAP_SET) ? 1 : 0;
#endif
		if (so_bd_opt) {
			if (-1 == setsockopt(ep->sockets_ep.udp_rx_fd, SOL_SOCKET, SO_BINDTODEVICE,
				ep->dev_name, strlen(ep->dev_name)+1)) {
					_HFI_ERROR("Failed to bind UDP socket to %s: %s\n",
						ep->dev_name, strerror(errno));
					goto fail;
				}
		}
	}

	if (!is_aux) {
		psm3_getenv_range("PSM3_UDP_GSO",
				"Enable UDP GSO Segmentation Offload",
				"(0 disables GSO, 1 sets max chunk to 65536, >1 specifies max chunk)",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
				(union psmi_envvar_val)UINT16_MAX,
				(union psmi_envvar_val)0, (union psmi_envvar_val)UINT16_MAX,
				NULL, NULL, &env_gso);
		ep->sockets_ep.udp_gso = env_gso.e_uint;
		if (ep->sockets_ep.udp_gso == 1)
			ep->sockets_ep.udp_gso = UINT16_MAX;
		if (ep->sockets_ep.udp_gso) {
			int gso;
			socklen_t optlen = sizeof(gso);
			if (!getsockopt(ep->sockets_ep.udp_tx_fd, SOL_UDP, UDP_SEGMENT, &gso, &optlen)) {
				_HFI_PRDBG("UDP GSO supported and enabled\n");
			} else {
				ep->sockets_ep.udp_gso = 0;
				_HFI_PRDBG("UDP GSO not supported\n");
			}
		}

		psm3_getenv("PSM3_UDP_GSO_ZEROCOPY",
				"Enable UDP GSO Segmentation Offload with zerocopy (0 disables GSO zerocopy)",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val) 0, &env_zerocopy);
		ep->sockets_ep.udp_gso_zerocopy = env_zerocopy.e_int;
		if (ep->sockets_ep.udp_gso_zerocopy) {
			int zerocopy = 1;
			if (-1 == setsockopt(ep->sockets_ep.udp_tx_fd, SOL_SOCKET, SO_ZEROCOPY, &zerocopy,
				sizeof(zerocopy))) {
				ep->sockets_ep.udp_gso_zerocopy = 0;
				_HFI_PRDBG("UDP GSO zerocopy not supported\n");
			}
		}

		psm3_getenv("PSM3_UDP_GRO",
				"Enable UDP GRO Coalesced Receive Offload (0 disables GRO)",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val) 0, &env_gro);
		ep->sockets_ep.udp_gro = env_gro.e_int;
		if (ep->sockets_ep.udp_gro) {
			int gro;
			socklen_t optlen = sizeof(gro);
			if (!getsockopt(ep->sockets_ep.udp_tx_fd, SOL_UDP, UDP_GRO, &gro, &optlen)) {
				_HFI_PRDBG("UDP GRO supported and enabled\n");
			} else {
				ep->sockets_ep.udp_gro = 0;
				_HFI_PRDBG("UDP GRO not supported\n");
			}
		}

		// additional stuff related to udp_gso
		if (ep->sockets_ep.udp_gso) {
			int val = IP_PMTUDISC_DO;
			if (-1 == setsockopt(ep->sockets_ep.udp_tx_fd, SOL_IP, IP_MTU_DISCOVER, &val, sizeof(val))) {
				_HFI_ERROR("Failed setsockopt GSO for %s: %s\n", ep->dev_name, strerror(errno));
				goto fail;
			}
		}

		// additional stuff related to gro
		if (ep->sockets_ep.udp_gro) {
			int val = 1;
			if (-1 == setsockopt(ep->sockets_ep.udp_tx_fd, SOL_IP, UDP_GRO, &val, sizeof(val))) {
				_HFI_ERROR("Failed setsockopt GRO for %s: %s\n", ep->dev_name, strerror(errno));
				goto fail;
			}
		}
	}

	// TBD - documentation implies this only affects TCP
	// TBD - UINT_MAX is 4GB/sec -> 34Gb/s
	psm3_getenv("PSM3_UDP_MAX_PACING_RATE",
			"Set UDP max send pacing rate in bytes/sec (0=OS default) (default maximum)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) UINT_MAX, &env_prate);
	if (env_prate.e_uint) {
		unsigned int max_pacing_rate = env_prate.e_uint;
		if (-1 == setsockopt(ep->sockets_ep.udp_tx_fd, SOL_SOCKET, SO_MAX_PACING_RATE, &max_pacing_rate,
			sizeof(max_pacing_rate))) {
			_HFI_ERROR("Failed setsockopt max pacing rate for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
	}

	psm3_getenv("PSM3_UDP_RCVBUF",
			"Sets the maximum UDP socket receive buffer (0=use OS default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) 0, &env_rbuf);
	if (env_rbuf.e_uint) {
		max_rcv_buf = env_rbuf.e_uint;
		if (-1 == setsockopt(ep->sockets_ep.udp_rx_fd, SOL_SOCKET, SO_RCVBUF, &max_rcv_buf, sizeof(max_rcv_buf))) {
			_HFI_ERROR("Failed setsockopt set socket receive buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
	} else {
		socklen_t len = sizeof(max_rcv_buf);
		if (-1 == getsockopt(ep->sockets_ep.udp_rx_fd, SOL_SOCKET, SO_RCVBUF, &max_rcv_buf, &len)) {
			_HFI_ERROR("Failed getsockopt get socket receive buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
		if (len != sizeof(max_rcv_buf))  {
			_HFI_ERROR("Unexpected len for getsockopt get socket receive buffer for %s: %u\n", ep->dev_name, (unsigned)len);
			goto fail;
		}
	}
	_HFI_PRDBG("PSM UDP receive buffer size: %u\n",max_rcv_buf);

	psm3_getenv("PSM3_UDP_SNDBUF",
			"Set the maximum UDP socket send buffer (0=use OS default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) 0, &env_sbuf);
	if (env_sbuf.e_uint) {
		max_snd_buf = env_sbuf.e_uint;
		if (-1 == setsockopt(ep->sockets_ep.udp_tx_fd, SOL_SOCKET, SO_SNDBUF, &max_snd_buf, sizeof(max_snd_buf))) {
			_HFI_ERROR("Failed setsockopt set socket send buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
	} else {
		socklen_t len = sizeof(max_snd_buf);
		if (-1 == getsockopt(ep->sockets_ep.udp_tx_fd, SOL_SOCKET, SO_SNDBUF, &max_snd_buf, &len)) {
			_HFI_ERROR("Failed getsockopt set socket send buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
		if (len != sizeof(max_snd_buf))  {
			_HFI_ERROR("Unexpected len for getsockopt get socket send buffer for %s: %u\n", ep->dev_name, (unsigned)len);
			goto fail;
		}
	}
	_HFI_PRDBG("PSM UDP send buffer size: %u\n",max_snd_buf);
	ep->sockets_ep.max_buffering = min(max_rcv_buf, max_snd_buf);

	ep->sockets_ep.if_index = if_nametoindex(ep->dev_name);
	if (!ep->sockets_ep.if_index) {
		_HFI_ERROR( "Failed to get interface index for %s: %s\n", ep->dev_name, strerror(errno));
		goto fail;
	}

	// we can use an AF_INET6 socket to send and receive both IPv4 and IPv6
	// packets.  When we want to use IPv4 we merely need to use an AF_INET6
	// sockaddr with IPv4 style GID (::ffff:<ipaddr>) or a AF_INET4 sockaddr
	// All receives and responses will use AF_INET6 sockaddr, but the
	// sin6_addr will be an IPv4 style GID when IPv4 traffic is involved
	// fill in 0 for port -> let UDP chose a free port (socket)
	psm3_build_sockaddr(&loc_addr, 0, ep->gid.hi, ep->gid.lo,
				ep->sockets_ep.if_index);

	if (bind(ep->sockets_ep.udp_rx_fd, (struct sockaddr *)&loc_addr,
		sizeof(loc_addr)) < 0) {
		_HFI_ERROR( "Failed to bind recv UDP socket for %s: to %s: %s\n",
			ep->dev_name,
			psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0),
			strerror(errno));
		goto fail;
	}

	if (bind(ep->sockets_ep.udp_tx_fd, (struct sockaddr *)&loc_addr,
		sizeof(loc_addr)) < 0) {
		_HFI_ERROR( "Failed to bind send UDP socket for %s: to %s: %s\n",
			ep->dev_name,
			psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0),
			strerror(errno));
		goto fail;
	}

	// figure out what socket we got assigned
	addr_len = sizeof(loc_addr);
	if ( -1 == getsockname(ep->sockets_ep.udp_rx_fd, (struct sockaddr *)&loc_addr, &addr_len)
		|| addr_len > sizeof(loc_addr)) {
		_HFI_ERROR("Failed to query UDP socket address for %s: %s\n", ep->dev_name, strerror(errno));
		goto fail;
	}
	if (!psmi_ip_equal_gid(&loc_addr, ep->gid)) {
		_HFI_ERROR("Unexpected mismatch of bound UDP IP address for %s: %s %s\n",
			ep->dev_name,
			psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0),
			psm3_ipv6_fmt(ep->gid, 0, 1));
		goto fail;
	}
	if (is_aux) {
		ep->sockets_ep.aux_socket = __be16_to_cpu(psm3_socket_port(&loc_addr));
	} else {
		ep->sockets_ep.pri_socket = __be16_to_cpu(psm3_socket_port(&loc_addr));
	}

	_HFI_PRDBG("bound UDP IP address for %s: %s\n",
			ep->dev_name,
			psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0));

	flags = fcntl(ep->sockets_ep.udp_rx_fd, F_GETFL);
	flags |= O_NONBLOCK;
	if (-1 == fcntl(ep->sockets_ep.udp_rx_fd, F_SETFL, flags)) {
		_HFI_ERROR( "Failed to set nonblock for UDP socket for %s: %s\n", ep->dev_name, strerror(errno));
		goto fail;
	}

	if (! is_aux) {
		/* Construct epid for this Endpoint */
		psmi_assert_always (ep->sockets_ep.udp_rx_fd);
		if (ep->addr.fmt == PSMI_ADDR_FMT_IPV4) {
			ep->epid = psm3_epid_pack_ipv4(ep->addr,
							PSMI_ETH_PROTO_UDP,
							ep->sockets_ep.pri_socket,
							0);
			_HFI_VDBG("construct epid ipv4: %s: ip %s UDP %u mtu %d\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->sockets_ep.pri_socket, ep->mtu);
		} else {
			psmi_assert(ep->addr.fmt == PSMI_ADDR_FMT_IPV6);
			ep->epid = psm3_epid_pack_ipv6(ep->addr,
							PSMI_ETH_PROTO_UDP,
							ep->sockets_ep.pri_socket,
							0);
			_HFI_VDBG("construct epid ipv6: %s: ip %s UDP %u mtu %d\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->sockets_ep.pri_socket, ep->mtu);
		}
	}

	// rest of resources initialized by psm3_sockets_ips_proto_init after we
	// have processed ips_proto configuration such as PSM3_MTU
	return PSM2_OK;

fail:
	psm3_ep_free_sockets(ep);
	return PSM2_INTERNAL_ERR;
}

psm2_error_t psm3_tune_tcp_socket(const char *sck_name, psm2_ep_t ep, int fd)
{
	union psmi_envvar_val env;
	int flag = 1;
	int max_rcv_buf;
	int max_snd_buf;

	// all the config below are optional, so we always return PSM2_OK
	if (-1 == setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(flag))) {
		_HFI_ERROR("Failed set %s TCP_NODELAY for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
	} else {
		_HFI_PRDBG("PSM TCP set %s TCP_NODELAY\n", sck_name);
	}
	if (-1 == setsockopt( fd, IPPROTO_TCP, TCP_QUICKACK, (void *)&flag, sizeof(flag))) {
		_HFI_ERROR("Failed set %s TCP_QUICKACK for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
	} else {
		_HFI_PRDBG("PSM TCP set %s TCP_QUICKACK\n", sck_name);
	}

	struct linger lg;
	lg.l_onoff = 1;		/* non-zero value enables linger option in kernel */
        lg.l_linger = 0;	/* timeout interval in seconds */
	if (-1 == setsockopt( fd, SOL_SOCKET, SO_LINGER, (void *)&lg, sizeof(lg))) {
		_HFI_ERROR("Failed set %s SO_LINGER for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
	} else {
		_HFI_PRDBG("PSM TCP set %s SO_LINGER\n", sck_name);
	}
	if (psm3_parse_tcp_reuseport()) {
		if (-1 == setsockopt( fd, SOL_SOCKET, SO_REUSEPORT, (void *)&flag, sizeof(flag))) {
			_HFI_ERROR("Failed set %s SO_REUSEPORTfor %s: %s\n", sck_name, ep->dev_name, strerror(errno));
		} else {
			_HFI_PRDBG("PSM TCP set %s SO_REUSEPORT\n", sck_name);
		}
	}
	psm3_getenv("PSM3_TCP_REUSEADDR",
		"Set TCP socket SO_REUSEADDR for more rapid listener socket reuse",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val) 0, &env);
	if (env.e_int) {
		// NOTE: SO_REUSEADDR can cause ambiguity issues.
		// PSM3_TCP_REUSEADDR is for experiment or workaround
		if (-1 == setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flag, sizeof(flag))) {
			_HFI_ERROR("Failed set %s SO_REUSEADDR for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
		} else {
			_HFI_PRDBG("PSM TCP set %s SO_REUSEADDR\n", sck_name);
		}
	}

	psm3_getenv("PSM3_TCP_BINDTODEVICE",
		"Bind the TCP socket to the device (0 disable)",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val) 0, &env);
	if (env.e_int) {
		int so_bd_opt = 1;
#if defined(PSM_LIBCAP)
		int ret_ev = -1, ret_pv = -1;
		cap_flag_value_t cap_net_raw = CAP_CLEAR;
		cap_flag_value_t cap_ev = CAP_CLEAR, cap_pv = CAP_CLEAR;
		cap_t caps = cap_get_proc();
		if (caps) {
			ret_ev = cap_get_flag(caps, CAP_NET_RAW, CAP_EFFECTIVE, &cap_ev);
			ret_pv = cap_get_flag(caps, CAP_NET_RAW, CAP_PERMITTED, &cap_pv);
			if (cap_ev == CAP_SET && cap_pv == CAP_SET && !ret_ev && !ret_pv)
				cap_net_raw = CAP_SET;
			cap_free(caps);
		}
		so_bd_opt = (cap_net_raw == CAP_SET) ? 1 : 0;
#endif
		if (so_bd_opt) {
			if (-1 == setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,
				ep->dev_name, strlen(ep->dev_name)+1)) {
					_HFI_ERROR("Failed to bind TCP rx socket to %s: %s\n",
						ep->dev_name, strerror(errno));
				}
		}
	}

	// TBD - UINT_MAX is 4GB/sec -> 34Gb/s
	psm3_getenv("PSM3_TCP_MAX_PACING_RATE",
			"Set TCP max send pacing rate in bytes/sec (0=OS default) (default maximum)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) UINT_MAX, &env);
	if (env.e_uint) {
		unsigned int max_pacing_rate = env.e_uint;
		if (-1 == setsockopt(fd, SOL_SOCKET, SO_MAX_PACING_RATE, &max_pacing_rate,
			sizeof(max_pacing_rate))) {
			_HFI_ERROR("Failed set %s max pacing rate for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
		} else {
			_HFI_PRDBG("PSM TCP set %s send pace rate: %u\n", sck_name, max_pacing_rate);
		}
	}

	env.e_uint = 0;
	psm3_getenv("PSM3_TCP_RCVBUF",
			"Sets the maximum TCP socket receive buffer (0=use OS default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) 0, &env);
	if (env.e_uint) {
		max_rcv_buf = env.e_uint;
		if (-1 == setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &max_rcv_buf, sizeof(max_rcv_buf))) {
			_HFI_ERROR("Failed set %s receive buffer for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
			if (fd == ep->sockets_ep.listener_fd)
				goto fail;
		}
	} else if (fd == ep->sockets_ep.listener_fd) {
		socklen_t len = sizeof(max_rcv_buf);
		if (-1 == getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &max_rcv_buf, &len)) {
			_HFI_ERROR("Failed getsockopt get socket receive buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
		if (len != sizeof(max_rcv_buf))  {
			_HFI_ERROR("Unexpected len for getsockopt get socket receiver buffer for %s: %u\n", ep->dev_name, (unsigned)len);
			goto fail;
		}
	}
	if (fd == ep->sockets_ep.listener_fd)
		_HFI_PRDBG("PSM TCP %s receive buffer size: %u\n",sck_name, max_rcv_buf);

	env.e_uint = 0;
	psm3_getenv("PSM3_TCP_SNDBUF",
			"Set the maximum TCP socket send buffer (0=use OS default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) 0, &env);
	if (env.e_uint) {
		max_snd_buf = env.e_uint;
		if (-1 == setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &max_snd_buf, sizeof(max_snd_buf))) {
			_HFI_ERROR("Failed set %s send buffer for %s: %s\n", sck_name, ep->dev_name, strerror(errno));
			if (fd == ep->sockets_ep.listener_fd)
				goto fail;
		}
	} else if (fd == ep->sockets_ep.listener_fd) {
		socklen_t len = sizeof(max_snd_buf);
		if (-1 == getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &max_snd_buf, &len)) {
			_HFI_ERROR("Failed getsockopt get socket send buffer for %s: %s\n", ep->dev_name, strerror(errno));
			goto fail;
		}
		if (len != sizeof(max_snd_buf))  {
			_HFI_ERROR("Unexpected len for getsockopt get socket send buffer for %s: %u\n", ep->dev_name, (unsigned)len);
			goto fail;
		}
	}
	if (fd == ep->sockets_ep.listener_fd)
		_HFI_PRDBG("PSM TCP %s send buffer size: %u\n", sck_name, max_snd_buf);
	// Setting buffer only has effect if we do it early.  Assume
	// socket we get for listener has same buffering as datapath sockets.
	if (fd == ep->sockets_ep.listener_fd)
		ep->sockets_ep.max_buffering = min(max_rcv_buf, max_snd_buf);


	env.e_uint = 0;
	psm3_getenv("PSM3_TCP_ZEROCOPY",
			"Enable TCP zerocopy (0 disables zerocopy)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val) 0, &env);
	if (env.e_uint) {
		flag = 1;
		if (-1 == setsockopt(fd, SOL_SOCKET, SO_ZEROCOPY, &flag, sizeof(flag))) {
			_HFI_PRDBG("%s zerocopy not supported for %s\n", sck_name, ep->dev_name);
		} else {
			_HFI_PRDBG("PSM TCP set %s zerocopy\n", sck_name);
		}
	}

	return PSM2_OK;

fail:
	return PSM2_INTERNAL_ERR;
}

/* parse TCP port range for PSM3_TCP_PORT_RANGE
 * format is low:high
 * low must be <= high and each must be < UINT16_MAX.
 * Either field can be omitted in which case default (input tvals) is used
 * for given field.
 * 0 - successfully parsed, tvals updated
 * -1 - str empty, tvals unchanged
 * -2 - syntax error, tvals may have been changed
 */
static int parse_tcp_port_range(const char *str,
				size_t errstr_size, char errstr[],
				int tvals[2])
{
	psmi_assert(tvals);
	int ret = psm3_parse_str_tuples(str, 2, tvals);
	if (ret < 0)
		return ret;
	if (tvals[0] > UINT16_MAX || tvals[1] > UINT16_MAX) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Max allowed is %u", UINT16_MAX);
		return -2;
	}
	if (tvals[0] < 0 || tvals[1] < 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Negative values not allowed");
		return -2;
	}
	if ((tvals[0] == TCP_PORT_AUTODETECT && tvals[1] != TCP_PORT_AUTODETECT)
		|| (tvals[0] != TCP_PORT_AUTODETECT && tvals[1] == TCP_PORT_AUTODETECT)) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " low of %d only allowed with high of %d", TCP_PORT_AUTODETECT, TCP_PORT_AUTODETECT);
		return -2;
	}
	if (tvals[0] > tvals[1]) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " low (%d) > high (%d)", tvals[0], tvals[1]);
		return -2;
	}
	return 0;
}

static int parse_check_tcp_port_range(int type,
				const union psmi_envvar_val val, void *ptr,
				size_t errstr_size, char errstr[])
{
	// parser will set tvals to result, use a copy to protect input of defaults
	int tvals[2] = { ((int*)ptr)[0], ((int*)ptr)[1] };
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR_TUPLES);
	return parse_tcp_port_range(val.e_str, errstr_size, errstr, tvals);
}

static __inline__
psm2_error_t listen_to_port(psm2_ep_t ep, int sockfd,
	psm3_sockaddr_in_t *addr,
	socklen_t addrlen)
{
	int i, tcp_backlog = TCP_BACKLOG;
	long int port;
	uint16_t start;
	uint16_t end;
	union psmi_envvar_val env_val;
	int tvals[2] = {TCP_PORT_LOW, TCP_PORT_HIGH };
	char range_def[32];
	snprintf(range_def, sizeof(range_def), "%d:%d", tvals[0], tvals[1]);

	(void)psm3_getenv_range("PSM3_TCP_PORT_RANGE",
		"Set the TCP listener port range <low:high>.",
		"The listener will bind to a random port in the range. '0:0'=let OS pick.",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR_TUPLES,
		(union psmi_envvar_val) range_def,
		(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
		parse_check_tcp_port_range, tvals, &env_val);
	if (parse_tcp_port_range(env_val.e_str, 0, NULL, tvals) < 0) {
		// already checked, shouldn't get parse errors nor empty strings
		psmi_assert(0);
	}

	_HFI_DBG("PSM3_TCP_PORT_RANGE = %d:%d\n", tvals[0], tvals[1]);

	if (tvals[0] == TCP_PORT_AUTODETECT && tvals[1] == TCP_PORT_AUTODETECT) {
		// PSM3_TCP_PORT_RANGE not provided or explicitly both set to 0
		port = 0;
		start = 0;
		end = 0;
		_HFI_DBG("Binding to OS provided port\n");
	} else {
		psmi_assert(tvals[0] > 0);
		// start with a random port, find the first available one.
		port = psm3_rand((long int) getpid());
		port = port % (tvals[1] + 1 - tvals[0]) + tvals[0];
		start = (uint16_t)tvals[0];
		end = (uint16_t)tvals[1];
		_HFI_DBG("Binding to port in range [%" PRIu16 ":%" PRIu16 "], starting from %ld\n", start, end, port);
	}

	psm3_getenv("PSM3_TCP_BACKLOG",
		"TCP listener queue depth (aka Backlog)",
		PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		(union psmi_envvar_val) TCP_BACKLOG, &env_val);
	if (env_val.e_int > 0) {
		tcp_backlog = env_val.e_int;
	}

	for (i = start; i <= end; i++, port++) {
		// Continue search from low end if reaches the high end
		if (port > end) {
			port = start;
		}

		psm3_socket_port(addr) = __cpu_to_be16((uint16_t)port);
		if (bind(sockfd, (struct sockaddr *)addr, addrlen)) {
			if (errno == EADDRINUSE || errno == EINVAL) {
				_HFI_VDBG("Port %ld to bind in use\n", port);
				continue;
			}
			_HFI_ERROR( "Failed to bind rx TCP socket to port %ld%s on %s: %s\n",
				port, port ? "" : " (random)", ep->dev_name, strerror(errno));
			return PSM2_INTERNAL_ERR;
		}
		if (listen(sockfd, tcp_backlog)) {
			if (errno == EADDRINUSE) {
				_HFI_VDBG("Port %ld to listen in use\n", port);
				continue;
			}
			_HFI_ERROR( "Failed to listen rx TCP socket on port %ld%s on %s: %s\n",
				port, port ? "" : " (random)", ep->dev_name, strerror(errno));
			return PSM2_INTERNAL_ERR;
		}
		_HFI_PRDBG("Successfully bound to port %ld%s\n", port, port ? "" : " (random)");
		_HFI_PRDBG("PSM TCP listen with backlog: %d\n",tcp_backlog);
		return PSM2_OK;
	}
	_HFI_ERROR( "No available port on %s in range [%" PRIu16 ", %" PRIu16 "]\n", ep->dev_name, start, end);
	return PSM2_INTERNAL_ERR;
}

/* parse TCP skip poll counts for PSM3_TCP_SKIPPOLL_COUNT
 * format is inactive_polls:active_polls
 * inactive_polls must be >= active_polls
 * Either field can be omitted in which case default (input tvals) is used
 * for given field.
 * 0 - successfully parsed, tvals updated
 * -1 - str empty, tvals unchanged
 * -2 - syntax error, tvals may have been changed
 */
static int parse_tcp_skippoll_count(const char *str,
				size_t errstr_size, char errstr[],
				int tvals[2])
{
	psmi_assert(tvals);
	int ret = psm3_parse_str_tuples(str, 2, tvals);
	if (ret < 0)
		return ret;
	if (tvals[0] < 0 || tvals[1] < 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " Negative values not allowed");
		return -2;
	}
	if (tvals[0] < tvals[1]) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " inactive_polls (%d) must be >= active_polls (%d)", tvals[0], tvals[1]);
		return -2;
	}
	return 0;
}

static int parse_check_tcp_skippoll_count(int type,
				const union psmi_envvar_val val, void *ptr,
				size_t errstr_size, char errstr[])
{
	// parser will set tvals to result, use a copy to protect input of defaults
	int tvals[2] = { ((int*)ptr)[0], ((int*)ptr)[1] };
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR_TUPLES);
	return parse_tcp_skippoll_count(val.e_str, errstr_size, errstr, tvals);
}

psm2_error_t
psm3_ep_open_tcp_internal(psm2_ep_t ep, int unit, int port,
			psm2_uuid_t const job_key)
{
	psm3_sockaddr_in_t loc_addr;
	socklen_t addr_len;

#ifdef PSM_TCP_POLL
	// one more for listener socket
	ep->sockets_ep.max_fds = TCP_INI_CONN + 1;
	ep->sockets_ep.fds = (struct pollfd *) psmi_calloc(
		ep, NETWORK_BUFFERS, ep->sockets_ep.max_fds, sizeof(struct pollfd));
	if (ep->sockets_ep.fds == NULL) {
		_HFI_ERROR( "Unable to allocate memory for pollfd\n");
		return PSM2_NO_MEMORY;
	}
#else
	ep->sockets_ep.events = psmi_calloc(ep, NETWORK_BUFFERS, TCP_MAX_EVENTS, sizeof(struct epoll_event));
	if (ep->sockets_ep.events == NULL) {
		_HFI_ERROR( "Unable to allocate memory for epoll events\n");
		return PSM2_NO_MEMORY;
	}
	ep->sockets_ep.max_fds = TCP_INI_CONN + 1;
	ep->sockets_ep.fds = (int *) psmi_calloc(
		ep, NETWORK_BUFFERS, ep->sockets_ep.max_fds, sizeof(int));
	if (ep->sockets_ep.fds == NULL) {
		_HFI_ERROR( "Unable to allocate memory for fds\n");
		return PSM2_NO_MEMORY;
	}
	ep->sockets_ep.efd = epoll_create1 (0);
	if (ep->sockets_ep.efd == -1) {
		_HFI_ERROR("Unable to create epoll. %d: %s\n", errno, strerror(errno));
	}
#endif

	ep->sockets_ep.max_rfds = TCP_INC_CONN;
	ep->sockets_ep.rfds = (int *) psmi_calloc(
		ep, NETWORK_BUFFERS, ep->sockets_ep.max_rfds, sizeof(int));
	if (ep->sockets_ep.rfds == NULL) {
		_HFI_ERROR( "Unable to allocate memory for revisit fds\n");
		return PSM2_NO_MEMORY;
	}

	// Allocate map to handle fd <-> index dependency
	ep->sockets_ep.map_nfds = TCP_INI_CONN + 1;
	ep->sockets_ep.map_fds = (struct fd_ctx **) psmi_calloc(
		ep, NETWORK_BUFFERS, ep->sockets_ep.map_nfds, sizeof(struct fd_ctx *));
	if (ep->sockets_ep.map_fds == NULL) {
		_HFI_ERROR( "Unable to allocate memory for fd map\n");
		return PSM2_NO_MEMORY;
	}

	if (PSM2_OK != tcp_open_dev(ep, unit, port, job_key)) {
		// tcp_open_dev already posted error.
		goto fail;
	}

	if (PSM2_OK != psm3_update_subnet(ep)) {
		goto fail;
	}

	if (PSM2_OK != psm3_update_mtu(ep)) {
		goto fail;
	}

	if (psm3_tune_tcp_socket("listening socket", ep, ep->sockets_ep.listener_fd))
		goto fail;

	// we can use an AF_INET6 socket to send and receive both IPv4 and IPv6
	// packets.  When we want to use IPv4 we merely need to use an AF_INET6
	// sockaddr with IPv4 style GID (::ffff:<ipaddr>) or a AF_INET4 sockaddr
	// All receives and responses will use AF_INET6 sockaddr, but the
	// sin6_addr will be an IPv4 style GID when IPv4 traffic is involved
	psm3_build_sockaddr(&loc_addr, 0, ep->gid.hi, ep->gid.lo,
       				ep->sockets_ep.if_index);

	if (listen_to_port(ep, ep->sockets_ep.listener_fd, &loc_addr, sizeof(loc_addr)) != PSM2_OK) {
		goto fail;
	}

	// figure out what socket we got assigned
	addr_len = sizeof(loc_addr);
	if ( -1 == getsockname(ep->sockets_ep.listener_fd, (struct sockaddr *)&loc_addr, &addr_len)
		|| addr_len > sizeof(loc_addr)) {
		_HFI_ERROR("Failed to query TCP rx socket address for %s: %s\n", ep->dev_name, strerror(errno));
		goto fail;
	}
	if (!psmi_ip_equal_gid(&loc_addr, ep->gid)) {
		_HFI_ERROR("Unexpected mismatch of bound TCP IP address for %s: %s %s\n",
			ep->dev_name,
			psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0),
			psm3_ipv6_fmt(ep->gid, 0, 1));
		goto fail;
	}
	ep->sockets_ep.pri_socket = __be16_to_cpu(psm3_socket_port(&loc_addr));
	_HFI_PRDBG("TCP socket=%u\n", ep->sockets_ep.pri_socket);
	PSM2_LOG_MSG("TCP port=%u", ep->sockets_ep.pri_socket);

	/* Construct epid for this Endpoint */
	psmi_assert_always (ep->sockets_ep.listener_fd);
	if (ep->addr.fmt == PSMI_ADDR_FMT_IPV4) {
		ep->epid = psm3_epid_pack_ipv4(ep->addr,
							PSMI_ETH_PROTO_TCP,
							ep->sockets_ep.pri_socket,
							ep->sockets_ep.aux_socket);
		_HFI_VDBG("construct epid ipv4: %s: ip %s TCP %u UDP %u mtu %d\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->sockets_ep.pri_socket,
						ep->sockets_ep.aux_socket, ep->mtu);
	} else {
		psmi_assert(ep->addr.fmt == PSMI_ADDR_FMT_IPV6);
		ep->epid = psm3_epid_pack_ipv6(ep->addr,
							PSMI_ETH_PROTO_TCP,
							ep->sockets_ep.pri_socket,
							ep->sockets_ep.aux_socket);
		_HFI_VDBG("construct epid ipv6: %s: ip %s TCP %u UDP %u mtu %d\n",
						psm3_epid_fmt_internal(ep->epid, 0),
						psm3_naddr128_fmt(ep->addr, 1),
						ep->sockets_ep.pri_socket,
						ep->sockets_ep.aux_socket, ep->mtu);
	}

	ep->sockets_ep.snd_msg.msg_iov =
		(struct iovec *) psmi_calloc(ep, NETWORK_BUFFERS, TCP_IOV_SIZE, sizeof(struct iovec));
	if (ep->sockets_ep.snd_msg.msg_iov == NULL) {
        		_HFI_ERROR( "Unable to allocate memory for msg_iov\n");
        		return PSM2_NO_MEMORY;
        }
        ep->sockets_ep.snd_msg.msg_iovlen = 0;

	union psmi_envvar_val env_val;
	char buf[32];
	snprintf(buf, sizeof(buf), "%d:%d", TCP_INACT_SKIP_POLLS, TCP_ACT_SKIP_POLLS);
	int tvals[2] = {TCP_INACT_SKIP_POLLS, TCP_ACT_SKIP_POLLS};
	(void)psm3_getenv_range("PSM3_TCP_SKIPPOLL_COUNT",
		"Polls to skip under inactive and active connections <inactive_polls[:active_polls]> ",
		"where inactive_polls >= active_polls.",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR_TUPLES,
		(union psmi_envvar_val) buf,
		(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
		parse_check_tcp_skippoll_count, tvals, &env_val);
	if (parse_tcp_skippoll_count(env_val.e_str, 0, NULL, tvals) < 0) {
		// already checked, shouldn't get parse errors nor empty strings
		psmi_assert(0);
	}
	ep->sockets_ep.inactive_skip_polls = tvals[0];
	ep->sockets_ep.active_skip_polls_offset = tvals[0] - tvals[1];

	// rest of resources initialized by psm3_sockets_ips_proto_init after we
	// have processed ips_proto configuration such as PSM3_MTU
	return PSM2_OK;

fail:
	psm3_ep_free_sockets(ep);
	return PSM2_INTERNAL_ERR;
}

#ifdef RNDV_MOD
#ifdef PSM_HAVE_GPU
/* This function is only called when GPUDirect is enabled */
static psm2_error_t open_rv(psm2_ep_t ep, psm2_uuid_t const job_key)
{
	struct local_info loc_info = { 0 };

	// we always fill in everything we might need in loc_info
	// in some modes, some of the fields are not used by RV
	loc_info.mr_cache_size = 0;

	/* gpu_cache_size ignored unless RV_RDMA_MODE_GPU or GPU_ONLY */
	loc_info.gpu_cache_size = ep->rv_gpu_cache_size;

	// GPU Direct is enabled and we need a GPU Cache
	loc_info.rdma_mode = RV_RDMA_MODE_GPU_ONLY;

	PSM3_GPU_USING_RV_FOR_MRS();

	// need portnum for rdma_mode KERNEL or (USER|GPU)
	loc_info.port_num = ep->portnum;

	ep->rv = psm3_rv_open(ep->dev_name, &loc_info);
	if (! ep->rv) {
		// if error is due to capability mismatch, loc_info.rdma_mode
		// has unavailable modes removed
		if (! loc_info.rdma_mode) {
			psmi_assert(! (loc_info.capability & RV_CAP_GPU_DIRECT));
		} else {
			return PSM2_INTERNAL_ERR;
		}
	}

	// parallel hal_gen1/gen1_hal_inline_i.h handling HFI1_CAP_GPUDIRECT_OT
#ifndef RV_CAP_GPU_DIRECT
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for GPU builds. Must use GPU enabled rv headers"
#endif
	if (loc_info.capability & RV_CAP_GPU_DIRECT)
		psmi_hal_add_cap(PSM_HAL_CAP_GPUDIRECT);
	PSM3_GPU_RV_SET_HAL_CAP(loc_info.capability);
	// sockets does not support PSM_HAL_CAP_GPUDIRECT_SDMA nor RDMA
	ep->rv_mr_cache_size = loc_info.mr_cache_size;
	ep->rv_gpu_cache_size = loc_info.gpu_cache_size;

	return PSM2_OK;
}
#endif /* PSM_HAVE_GPU */
#endif /* RNDV_MOD */

psm2_error_t
psm3_ep_open_sockets(psm2_ep_t ep, int unit, int port, int addr_index, psm2_uuid_t const job_key)
{
	// make sure all fields are empty.
	memset(&ep->sockets_ep,0,sizeof(ep->sockets_ep));

	ep->rdmamode = 0;	// no rendezvous RDMA for sockets
	// no MR cache, leave ep->mr_cache_mode as set by caller (NONE)
#ifdef RNDV_MOD
#ifdef PSM_HAVE_GPU
	ep->rv_gpu_cache_size = psmi_parse_gpudirect_rv_gpu_cache_size(0);
#endif
#endif
	ep->sockets_ep.sockets_mode = psm3_sockets_parse_inet(0);

	if (_HFI_PRDBG_ON) {
		char uuid_str[64];
		memset(&uuid_str, 0, sizeof(uuid_str));
		uuid_unparse(job_key, uuid_str);
		_HFI_PRDBG("job key %s\n", uuid_str);
	}

	// we search sysfs for devices names, alternatively could use
	// SIOCGIFCONF - iface list (see man 7 netdevice)
	const char *unitpath = psm3_sysfs_unit_path(unit);

	// callers tend not to set port, 0 means any
	if (PSM3_NIC_PORT_ANY == port)
		port = NETDEV_PORT;
	ep->portnum = port;	// TBD - N/A
	if (! unitpath) {
		_HFI_ERROR( "NULL sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}

	char *dev_name = strrchr(unitpath, '/');
	if (dev_name == NULL) {
		_HFI_ERROR( "invalid sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}
	dev_name++; // Inc past last '/'

	ep->dev_name = psmi_strdup(ep, dev_name);
	if (! ep->dev_name) {
		_HFI_ERROR( "can't alloc devname");
		return PSM2_INTERNAL_ERR;
	}

	ep->unit_id = unit;
	_HFI_PRDBG("Using unit_id[%d] %s.\n", ep->unit_id, ep->dev_name);

#ifdef RNDV_MOD
#ifdef PSM_HAVE_GPU
	/* Open rv only when GPUDirect is enabled */
	if (PSM3_GPU_IS_ENABLED && psmi_parse_gpudirect() &&
	    open_rv(ep, job_key) != PSM2_OK) {
		_HFI_ERROR("Unable to open rv for port %d of %s.\n",
			   port, ep->dev_name);
		psmi_free((char *)ep->dev_name);
		ep->dev_name = NULL;
		return PSM2_INTERNAL_ERR;
	}
#endif /* PSM_HAVE_GPU */
#endif /* RNDV_MOD */
	ep->wiremode = 0; // TCP vs UDP are separate EPID protocols
	ep->addr_index = addr_index;
	psmi_hal_add_cap(PSM_HAL_CAP_NIC_LOOPBACK);
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		// TCP requires packets be multiple of 32b because in tcp_recv
		// PSM uses the lrh[2] pkt size (multiple of 32b) to decide how
		// much to read from the socket.
		int ret = psm3_ep_open_udp_internal(ep, unit, port, job_key, 1);
		if (ret != PSM2_OK) {
			return ret;
		}
		return psm3_ep_open_tcp_internal(ep, unit, port, job_key);
	} else {
		// UDP, including GSO, can handle odd length packets
		psmi_hal_add_cap(PSM_HAL_CAP_NON_DW_PKT_SIZE);
		return psm3_ep_open_udp_internal(ep, unit, port, job_key, 0);
	}
}


// ep->mtu is now max PSM payload, not including headers and perhaps decreased
// via PSM3_MTU
psm2_error_t
psm3_sockets_ips_proto_init(struct ips_proto *proto, uint32_t cksum_sz)
{
	psm2_ep_t ep = proto->ep;
	union psmi_envvar_val env_mtu;

#ifdef PSM_HAVE_REG_MR
	// defaults for SDMA thresholds.
	// sockets does not support Send DMA, so set large to disable.
        proto->iovec_thresh_eager = proto->iovec_thresh_eager_blocking = ~0U;
#ifdef PSM_HAVE_GPU
        proto->iovec_gpu_thresh_eager = proto->iovec_gpu_thresh_eager_blocking = ~0U;
#endif
#endif

	// hash for dispersive routing
	proto->epinfo.ep_hash = ep->sockets_ep.pri_socket;	// low 8b only

	// at this point ep->mtu is our HW capability found during open
	// and adjusted to allow for all headers so ep->mtu reflects maximum
	// PSM payload (not yet adjusted for optional cksum_sz)

	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		/* For TCP a larger psm payload size is possible */
		char help[128];

		snprintf(help, sizeof(help), "Upper bound on PSM3 payload. It can be higher than the wire MTU (<=0 uses port MTU): 1-7, 1024-%u", TCP_MAX_MTU);
		if (!psm3_getenv("PSM3_MTU", help,
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)TCP_DEFAULT_MTU, &env_mtu)) {
			// use OPA_MTU_MAX so we don't round down to min MTU when
			// OPA enum values mistakenly used here.
			if (env_mtu.e_int >= IBTA_MTU_MIN && env_mtu.e_int <= OPA_MTU_MAX) //enum
				env_mtu.e_int = opa_mtu_enum_to_int((enum opa_mtu)env_mtu.e_int);
			else if (env_mtu.e_int < IBTA_MTU_MIN) // pick default
				env_mtu.e_int = ep->mtu & 0xfffffffc; // use wire mtu
			else
				env_mtu.e_int = env_mtu.e_int & 0xfffffffc;
          
			if (env_mtu.e_int > TCP_MAX_MTU)
				env_mtu.e_int = TCP_MAX_MTU;
		}
		ep->mtu = env_mtu.e_int;
	} else { // PSM3_SOCKETS_UDP
		/* See if user specifies a lower MTU to use */
		if (!psm3_getenv("PSM3_MTU", 
				"Upper bound on packet MTU (<=0 uses port MTU): 1-6,256,512,1024,2048,4096,8192]",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)-1, &env_mtu)) {
			// use OPA_MTU_MAX so we don't round down to min MTU when
			// OPA enum values mistakenly used here.
			if (env_mtu.e_int >= IBTA_MTU_MIN && env_mtu.e_int <= OPA_MTU_MAX) //enum
				env_mtu.e_int = opa_mtu_enum_to_int((enum opa_mtu)env_mtu.e_int);
			else if (env_mtu.e_int < IBTA_MTU_MIN) // pick default
				env_mtu.e_int = 8192; // default high, will use wire MTU
			else // wash through enum to force round up to next valid MTU
				// UDP could allow any value, but
				// IBTA/OPA style powers of 2 may work best
				env_mtu.e_int = opa_mtu_enum_to_int(opa_mtu_int_to_enum(env_mtu.e_int));
			// only allow MTU decrease
			if (ep->mtu > env_mtu.e_int)
				ep->mtu = env_mtu.e_int;
		}
	}
	/* allow space for optional software managed checksum (for debug) */
	ep->mtu -= cksum_sz;
	// ep->mtu is our final choice of local PSM payload we can support
	proto->epinfo.ep_mtu = ep->mtu;

	// want to end up with multiple of cache line (64)
	// be conservative (+BUFFER_HEADROOM)
	// TODO should be aligned with gro ?
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		ep->sockets_ep.buf_size = ep->mtu + TCP_MAX_PSM_HEADER + BUFFER_HEADROOM;
		psmi_assert(ep->sockets_ep.buf_size-BUFFER_HEADROOM <= TCP_MAX_PKTLEN);
	} else {
		ep->sockets_ep.buf_size = ep->mtu + MAX_PSM_HEADER + BUFFER_HEADROOM;
	}
	ep->sockets_ep.rbuf = (uint8_t *)psmi_calloc(ep, NETWORK_BUFFERS,
								ep->sockets_ep.buf_size, 1);
	ep->sockets_ep.sbuf = (uint8_t *)psmi_calloc(ep, NETWORK_BUFFERS,
								ep->sockets_ep.buf_size, 1);

	if (! ep->sockets_ep.rbuf || ! ep->sockets_ep.sbuf) {
		_HFI_ERROR( "Unable to allocate UDP buffers pools\n");
		goto fail;
	}

#ifdef PSM_BYTE_FLOW_CREDITS
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		// let flow_credits be the control
		// unfortunately default TCP max_buffering (16K) is too small
		// so flow_credit_bytes < 16K would prevent getting a good pipeline of
		// packets/ACKs going
		proto->flow_credit_bytes = ep->mtu * proto->max_credits;
	} else {
		// sockets buffering needs to place an upper bound on bytes
		// while flow_credits places an upper bound on pkts
		proto->flow_credit_bytes = ep->sockets_ep.max_buffering/2;
	}
	_HFI_DBG("initial flow_credits %d bytes %d\n",
				proto->flow_credits, proto->flow_credit_bytes);
#else /* PSM_BYTE_FLOW_CREDITS */
	_HFI_DBG("initial flow_credits %d\n", proto->flow_credits);
#endif /* PSM_BYTE_FLOW_CREDITS */

	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		psmi_assert(!ep->sockets_ep.udp_gso);
		union psmi_envvar_val env_val;
		psm3_getenv("PSM3_TCP_SNDPACING_THRESH",
			"PSM level send pacing threshold in bytes. Default is PSM3 MTU. "
			"Zero will disable send pacing.",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val) ep->mtu, &env_val);
		if (env_val.e_int >= 0) {
			ep->sockets_ep.snd_pace_thresh = env_val.e_int;
		} else {
			ep->sockets_ep.snd_pace_thresh = ep->mtu;
		}

		psm3_getenv("PSM3_TCP_SHRTBUF_SIZE",
			"Short buffer size. Shall be a small number because the total "
			"consumed memory grow fast with fabric size. The valid value "
			"is ZERO (turning off short buffer) or a number larger than "
			"PSM message header (" STRINGIFY(TCP_MAX_PSM_HEADER) ") and less than PSM MTU.",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val) TCP_SHRT_BUF_SIZE, &env_val);
		if (env_val.e_int < 0) {
			env_val.e_int = TCP_SHRT_BUF_SIZE;
		}
		if (env_val.e_int > ep->sockets_ep.buf_size) {
			ep->sockets_ep.shrt_buf_size = ep->sockets_ep.buf_size;
			_HFI_DBG("PSM3_TCP_SHRTBUF_SIZE (%d) exceeds upper bounds of PSM MTU (%d), "
				 "adjusting to PSM MTU.\n", env_val.e_int, ep->sockets_ep.shrt_buf_size);
		} else if (env_val.e_int > TCP_MAX_PSM_HEADER || env_val.e_int == 0) {
			ep->sockets_ep.shrt_buf_size = env_val.e_int;
		} else {
			ep->sockets_ep.shrt_buf_size = TCP_MAX_PSM_HEADER;
			_HFI_DBG("PSM3_TCP_SHRTBUF_SIZE (%d) exceeds lower bounds, adjusting to the bound (%d).\n",
				env_val.e_int, ep->sockets_ep.shrt_buf_size);
		}
	}

	if (ep->sockets_ep.udp_gso) {
		// set upper bounds for GSO segmentation
		// OS limitation of 64K (UINT16_MAX) and UDP_MAX_SEGMENTS (64)
		ep->chunk_max_segs = min(UINT16_MAX / (ep->mtu + sizeof(struct ips_message_header)), UDP_MAX_SEGMENTS);
		ep->chunk_max_size = ep->sockets_ep.udp_gso;

		// for acks to pipeline we'll need to limit max_nsegs to
		// < flow_credits/2 and max_size to < flow_credit_bytes/2
		// (ideally 1/4, but that makes GSO too small and is worse)
		ep->chunk_max_segs = min(ep->chunk_max_segs, proto->flow_credits/2);
		ep->chunk_max_segs = max(ep->chunk_max_segs, 1);
#ifdef PSM_BYTE_FLOW_CREDITS
		ep->chunk_max_size = min(ep->chunk_max_size, proto->flow_credit_bytes/2);
		ep->chunk_max_size = max(ep->chunk_max_size, ep->mtu);
#endif
		// For simplicity we use max_segs to compute sbuf_udp_gso size
		// Worst case it's >window_rv needs, but always <=64K
		// Note sbuf_udp_gso needs space for PSM headers and payload
		// Alternative would be to use:
		//     min(segs*size, window + (window/min_mtu)*header size)
		ep->sockets_ep.sbuf_udp_gso = (uint8_t *)psmi_calloc(ep, NETWORK_BUFFERS,
						ep->sockets_ep.buf_size
						* ep->chunk_max_segs, 1);
		if (! ep->sockets_ep.sbuf_udp_gso) {
			_HFI_ERROR( "Unable to allocate UDP GSO buffer\n");
			goto fail;
		}
	} else {
		// no send segmentation, max_segs will constrain
		ep->chunk_max_segs = 1;
		ep->chunk_max_size = ep->mtu;
	}
	if (ep->sockets_ep.udp_gso)
		_HFI_DBG("GSO segs %u bytes %u\n",
			ep->chunk_max_segs, ep->chunk_max_size);

	/*
	 * Pre-calculate the PSN mask to support 31 bit PSN.
	 */
	proto->psn_mask = 0x7FFFFFFF;
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP)
		/* 16 bit pktlen (limit to <= 65535 32 bit words per packet */
		proto->pktlen_mask = 0xFFFF;
	else
		/* 12 bit pktlen (limit to <= 4095 32 bit words per packet */
		proto->pktlen_mask = 0xFFF;

	return PSM2_OK;

fail:
	psm3_ep_sockets_free_buffers(ep);
	return PSM2_INTERNAL_ERR;
}

// Fetch current link state to update linkinfo fields in ips_proto:
// 	ep_base_lid, ep_lmc, ep_link_rate
// These are all fields which can change during a link bounce.
// Note "active" state is not adjusted as on link down PSM will wait for
// the link to become usable again so it's always a viable/active device
// afer initial PSM startup has selected devices.
// Called during initialization of ips_proto during ibta_init as well
// as during a link bounce.
// TBD - may be able to call this from HAL ips_proto_init as well as
// directly within HAL event processing, in which case this could
// be completely internal to HAL and not exposed in HAL API
psm2_error_t psm3_sockets_ips_proto_update_linkinfo(struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;
	uint64_t link_speed;

	// for Ethernet LID of 1 is returned
	proto->epinfo.ep_base_lid = __cpu_to_be16(1);

	// TBD don't yet have multi-path dispersive routing
	// maybe we use env to derrive multiple sequential IP
	// addresses, sort of like an LMC concept
	// or use ECMP or other mechanism
	proto->epinfo.ep_lmc = 0; // No LMC for UD

	if (psm3_hfp_sockets_get_port_speed(proto->ep->unit_id, proto->ep->portnum, &link_speed) < 0) {
		err =
		    psm3_handle_error(proto->ep, PSM2_EP_DEVICE_FAILURE,
				      "Could obtain link rate for unit %u:%u. Error: %s",
				      proto->ep->unit_id,
				      proto->ep->portnum,
				      strerror(errno));
		return err;
	} else {
		proto->epinfo.ep_link_rate = ips_link_speed_to_enum(link_speed);
	}

	return err;
}

int psm3_sockets_poll_type(int poll_type, psm2_ep_t ep)
{
	switch (poll_type) {
	case PSMI_HAL_POLL_TYPE_NONE:
		// no events for solicted and unsolictited recv
		_HFI_PRDBG("disable solicited event - noop\n");
		// this is only done once during PSM shutdown of rcvthread.
		break;
	case PSMI_HAL_POLL_TYPE_URGENT:
		// set for event on solicted recv (urgent PSM protocol pkts)
		_HFI_PRDBG("enable solicited event\n");
		// TBD - sockets does not yet implement urgent interrupt wakeup
		// for rcvThread.  TBD if can do that with sockets
		break;
	case PSMI_HAL_POLL_TYPE_ANYRCV:
		//_HFI_VDBG("enable all events\n");
		// TBD if we want to implement this need sockets pollintr to
		// wait on the actual sockets, perhaps an ep_poll
		return -1;
		break;
	default:
		return -1;
	}

	return 0;
}

// free reources in ep->sockets_ep portion of the ep
void psm3_ep_free_sockets(psm2_ep_t ep)
{
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP
		&& ep->sockets_ep.fds) {
		int i, fd;
#ifdef PSM_TCP_POLL
		for (i = ep->sockets_ep.nfds - 1; i >= 0; i--) {
			if (ep->sockets_ep.fds[i].fd > 0) {
				fd = ep->sockets_ep.fds[i].fd;
				psm3_sockets_tcp_close_fd(ep, fd, i, NULL);
				_HFI_VDBG("Closed fd=%d\n", fd);
			}
		}
#else
		if (ep->sockets_ep.efd > 0) {
			close(ep->sockets_ep.efd);
		}
		for (i = ep->sockets_ep.nfds - 1; i >= 0; i--) {
			if (ep->sockets_ep.fds[i] > 0) {
				fd = ep->sockets_ep.fds[i];
				psm3_sockets_tcp_close_fd(ep, fd, i, NULL);
				_HFI_VDBG("Closed fd=%d\n", fd);
			}
		}
		if (ep->sockets_ep.events) {
			psmi_free(ep->sockets_ep.events);
		}
#endif
		psmi_free(ep->sockets_ep.fds);
		ep->sockets_ep.fds = NULL;
		ep->sockets_ep.nfds = 0;

		psmi_free(ep->sockets_ep.rfds);
		ep->sockets_ep.rfds = NULL;
		ep->sockets_ep.nrfd = 0;

		// Just free map, as we do not need it anymore
		if (ep->sockets_ep.map_fds) {
			psmi_free(ep->sockets_ep.map_fds);
			ep->sockets_ep.map_fds = NULL;
			ep->sockets_ep.map_nfds = 0;
		}
	}
	psm3_ep_sockets_free_buffers(ep);
	if (ep->sockets_ep.udp_tx_fd) {
		close(ep->sockets_ep.udp_tx_fd);
		ep->sockets_ep.udp_tx_fd = 0;
	}
	if (ep->sockets_ep.udp_rx_fd) {
		// already closed when clean up ep fds
		ep->sockets_ep.udp_rx_fd = 0;
	}
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		if (ep->sockets_ep.listener_fd) {
			// shall already closed when we close all fds in sockets_ep.fds
			//close(ep->sockets_ep.listener_fd);
			ep->sockets_ep.listener_fd = 0;
		}
		if (ep->sockets_ep.snd_msg.msg_iov) {
			psmi_free(ep->sockets_ep.snd_msg.msg_iov);
			ep->sockets_ep.snd_msg.msg_iov = NULL;
		}
	}
#ifdef RNDV_MOD
#ifdef PSM_HAVE_GPU
	if (ep->rv) {
		psm3_rv_close(ep->rv);
		ep->rv = NULL;
	}
#endif
#endif
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
}

void psm3_ep_sockets_free_buffers(psm2_ep_t ep)
{
	if (ep->sockets_ep.udp_gso) {
		if (ep->sockets_ep.sbuf_udp_gso) {
			psmi_free(ep->sockets_ep.sbuf_udp_gso);
			ep->sockets_ep.sbuf_udp_gso = NULL;
		}
	}

	if (ep->sockets_ep.sbuf) {
		psmi_free(ep->sockets_ep.sbuf);
		ep->sockets_ep.sbuf = NULL;
	}

	if (ep->sockets_ep.rbuf) {
		psmi_free(ep->sockets_ep.rbuf);
		ep->sockets_ep.rbuf = NULL;
	}
}

void
psm3_hfp_sockets_context_initstats(psm2_ep_t ep)
{
}

static psm2_error_t udp_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key)
{
	int err = PSM2_OK;
	struct ifreq ifrt;

	ep->sockets_ep.udp_tx_fd = socket(psm3_socket_domain, SOCK_DGRAM, 0);
	// 0 is a valid fd, but we treat 0 as an uninitialized value, so make
	// sure we didn't end up with 0 (stdin should have 0)
	if (ep->sockets_ep.udp_tx_fd == 0) {
		// create a copy of the file descriptor using the unused one
		ep->sockets_ep.udp_tx_fd = dup(ep->sockets_ep.udp_tx_fd);
		close(0);
	}
	if (ep->sockets_ep.udp_tx_fd == -1) {
		_HFI_ERROR( "Unable to create UDP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	ep->sockets_ep.udp_rx_fd = socket(psm3_socket_domain, SOCK_DGRAM, 0);
	// 0 is a valid fd, but we treat 0 as an uninitialized value, so make
	// sure we didn't end up with 0 (stdin should have 0)
	if (ep->sockets_ep.udp_rx_fd == 0) {
		// create a copy of the file descriptor
		ep->sockets_ep.udp_rx_fd = dup(ep->sockets_ep.udp_rx_fd);
		close(0);
	}
	if (ep->sockets_ep.udp_rx_fd == -1) {
		_HFI_ERROR( "Unable to create UDP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	ifrt.ifr_addr.sa_family = psm3_socket_domain;	// TBD if this used?
	strncpy(ifrt.ifr_name, ep->dev_name, IFNAMSIZ-1);
	_HFI_PRDBG("Querying %s.\n",ep->dev_name);
	if (-1 == ioctl(ep->sockets_ep.udp_rx_fd, SIOCGIFMTU, &ifrt)) {
		_HFI_ERROR( "Unable to query UDP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->sockets_ep.if_mtu = ifrt.ifr_mtu;

	if (-1 == ioctl(ep->sockets_ep.udp_rx_fd, SIOCGIFFLAGS, &ifrt)) {
		_HFI_ERROR( "Unable to query UDP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->sockets_ep.if_flags = ifrt.ifr_flags;

	_HFI_PRDBG("Opened %s.\n",ep->dev_name);


done:
	return err;

fail:
	if (ep->sockets_ep.udp_rx_fd > 0) {
		close(ep->sockets_ep.udp_rx_fd);
		ep->sockets_ep.udp_rx_fd = 0;
	}
	if (ep->sockets_ep.udp_tx_fd > 0) {
		close(ep->sockets_ep.udp_tx_fd);
		ep->sockets_ep.udp_tx_fd = 0;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
	goto done;
}

static psm2_error_t tcp_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key)
{
	int err = PSM2_OK;
	struct ifreq ifrt;

	ep->sockets_ep.listener_fd = socket(psm3_socket_domain, SOCK_STREAM | SOCK_NONBLOCK, 0);
	// 0 is a valid fd, but we treat 0 as an uninitialized value, so make
	// sure we didn't end up with 0 (stdin should have 0)
	if (ep->sockets_ep.listener_fd == 0) {
		// create a copy of the file descriptor
		ep->sockets_ep.listener_fd = dup(ep->sockets_ep.listener_fd);
		close(0);
	}
	if (ep->sockets_ep.listener_fd == -1) {
		_HFI_ERROR( "Unable to create TCP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ifrt.ifr_addr.sa_family = psm3_socket_domain;
	strncpy(ifrt.ifr_name, ep->dev_name, IFNAMSIZ-1);
	_HFI_PRDBG("Querying %s.\n",ep->dev_name);
	if (-1 == ioctl(ep->sockets_ep.listener_fd, SIOCGIFMTU, &ifrt)) {
		_HFI_ERROR( "Unable to query TCP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->sockets_ep.if_mtu = ifrt.ifr_mtu;

	if (-1 == ioctl(ep->sockets_ep.listener_fd, SIOCGIFFLAGS, &ifrt)) {
		_HFI_ERROR( "Unable to query TCP socket for %s: %s\n", ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->sockets_ep.if_flags = ifrt.ifr_flags;

	_HFI_PRDBG("Opened %s.\n",ep->dev_name);

done:
	if (ep->sockets_ep.listener_fd > 0) {
		psm3_sockets_tcp_add_fd(ep, ep->sockets_ep.listener_fd, FD_STATE_ESTABLISHED);

	}
	if (ep->sockets_ep.udp_rx_fd > 0) {
		psm3_sockets_tcp_add_fd(ep, ep->sockets_ep.udp_rx_fd, FD_STATE_ESTABLISHED);
	}
	ep->sockets_ep.tcp_incoming_fd = 0;
	return err;

fail:
	if (ep->sockets_ep.listener_fd > 0) {
		close(ep->sockets_ep.listener_fd);
		ep->sockets_ep.listener_fd = 0;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}

	goto done;
}

static psm2_error_t psm3_update_mtu(psm2_ep_t ep)
{
	uint32_t active_mtu;

	active_mtu = ep->sockets_ep.if_mtu;
	active_mtu = min(active_mtu, 8192);	// TBD - seems we need to be lower
						// TBD on OPA IPoIB had to be even lower

	if (! (ep->sockets_ep.if_flags & IFF_RUNNING)) {
		_HFI_ERROR( " Port state is not running for %s port %d\n",
						ep->dev_name, ep->portnum);
		return PSM2_INTERNAL_ERR;
	}
	if (! (ep->sockets_ep.if_flags & IFF_UP)) {
		_HFI_ERROR( " Port state is not up for %s port %d\n",
						ep->dev_name, ep->portnum);
		return PSM2_INTERNAL_ERR;
	}

	// compute MTU.
	// ep->mtu is the PSM payload size.  For OPA native mode, this did not
	// include headers as OPA allowed up to an additional 128 bytes of headers.
	// However all UD QP payloads (including PSM headers) are
	// counted toward MTU in UD verbs.  So need to discount by PSM header size
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		ep->mtu = active_mtu - sizeof(struct iphdr) - sizeof(struct tcphdr) - TCP_MAX_PSM_HEADER;
	} else {
		ep->mtu = active_mtu - sizeof(struct iphdr) - sizeof(struct udphdr) - MAX_PSM_HEADER;
	}
	_HFI_PRDBG("Max PSM payload (aka MTU): %u\n", ep->mtu);
	return PSM2_OK;
}

void
psm3_dump_sockets_ep(psm2_ep_t ep)
{
	struct psm3_sockets_ep *uep = &(ep->sockets_ep);

	printf("devname = %s\n", ep->dev_name);
	printf("IP      = %s\n", psm3_naddr128_fmt(ep->addr, 0));
	printf("socket  = %u\n", uep->pri_socket);
	if (ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		printf("aux socket = %u\n", uep->aux_socket);
		printf("nfds = %u\n", uep->nfds);
	}
}

static psm2_error_t psm3_update_subnet(psm2_ep_t ep)
{
	int result = PSM2_OK;
	if (0 != psmi_hal_get_port_subnet(ep->unit_id, ep->portnum, ep->addr_index,
			&ep->subnet, &ep->addr, NULL, &ep->gid)) {
		_HFI_ERROR( "Unable to get subnet for port %u addr %u of %s: %s\n",
				 ep->portnum, ep->addr_index, ep->dev_name, strerror(errno));
		result = PSM2_INTERNAL_ERR;
	} else {
		_HFI_PRDBG("Subnet for port %u addr %u of %s: %s addr %s gid %s\n",
				ep->portnum, ep->addr_index, ep->dev_name,
				psm3_subnet128_fmt(ep->subnet, 0),
				psm3_naddr128_fmt(ep->addr, 1),
				psm3_gid128_fmt(ep->gid, 2));
	}
	return result;
}

/* INET protocol (TCP or UDP) */
// we need this early when setting defaults in psm3_mq_malloc
// and also want this available when creating the sockets_ep since it may affect
// sizing of CQs and buffers.  But during psm3_mq_malloc we don't have an ep
// or proto to save this into.
unsigned psm3_sockets_parse_inet(int reload)
{
#ifndef USE_UDP
	return PSM3_SOCKETS_TCP;
#else
	union psmi_envvar_val envval;
	static int have_value = 0;
	static unsigned saved;
	unsigned default_value;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (!reload && have_value)
		return saved;

	default_value = PSM3_SOCKETS_TCP;
	envval.e_uint = default_value;

	psm3_getenv("PSM3_SOCKETS",
		    "Sockets protocol to use (0-TCP/IP, 1-UDP/IP) [0]",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
		    (union psmi_envvar_val)default_value,
		    &envval);
	if (envval.e_uint > 1) {
		static int logged = 0;
		if (! logged) {
			_HFI_INFO("WARNING: Invalid value for PSM3_NET, using %u\n", default_value);
			logged = 1;
		}
		envval.e_uint = default_value;
	}
	saved = envval.e_uint;
	have_value = 1;
	return saved;
#endif /* USE_UDP */
}
#endif // PSM_SOCKETS
