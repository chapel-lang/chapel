#ifdef PSM_SOCKETS
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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */


#ifndef _PSMI_IN_USER_H
#error "hal_sockets/sockets_ep.h not meant to be included directly, include psm_user.h instead"
#endif

#ifndef PSM_HAL_SOCKETS_EP_H
#define PSM_HAL_SOCKETS_EP_H

#include <netinet/in.h>

#ifdef RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#include <infiniband/verbs.h>
#include <psm_rndv_mod.h>
#endif
#endif /* RNDV_MOD */

#ifndef SOL_UDP
#define SOL_UDP 17
#endif

#ifndef UDP_SEGMENT
#define UDP_SEGMENT 103
#endif

#ifndef UDP_GRO
#define UDP_GRO 104
#endif

#ifndef SO_ZEROCOPY
#define SO_ZEROCOPY 60
#endif

#ifndef MSG_ZEROCOPY
#define MSG_ZEROCOPY 0x4000000
#endif

#ifndef UDP_MAX_SEGMENTS
#define UDP_MAX_SEGMENTS (1 << 6UL)
#endif

/* mode for EP as selected by PSM3_SOCKETS */
#define PSM3_SOCKETS_TCP 0
#define PSM3_SOCKETS_UDP 1

#define MAX_PSM_HEADER 64			// sizeof(ips_lrh) == 56, round up to 64

#define TCP_MAX_PSM_HEADER 56			// sizeof(ips_lrh) == 56

#define NETDEV_PORT 1			// default port if not specified

#define BUFFER_HEADROOM 0		// how much extra to allocate in buffers
								// as a paranoid headroom for use of more than
								// intended.  Was 64, but seems we can do
								// without it and hence make buffers better
								// page aligned
								// value here should be a multiple of CPU
								// cache size
#define CPU_PAGE_ALIGN	PSMI_PAGESIZE	// boundary to align buffer pools for

#ifndef PSM_TCP_POLL
#include <sys/epoll.h>
#endif
#include <sys/poll.h>

#define TCP_PORT_AUTODETECT 0			// default is to allow system to use any port
#define TCP_PORT_HIGH	TCP_PORT_AUTODETECT	// default TCP port range high end
#define TCP_PORT_LOW	TCP_PORT_AUTODETECT	// default TCP port range low end
#define TCP_BACKLOG	1024			// backlog for socket.
#define TCP_INI_CONN	1024			// initial fds array (see psm3_sockets_ep) size
#define TCP_INC_CONN	128			// fds array grow size
#define TCP_POLL_TO	1000			// timeout for continuous poll in ms. used when no more data in
						// the middle of draining a packet.
#ifndef PSM_TCP_POLL
#define TCP_MAX_EVENTS	1024			// max events to get from epoll
#endif
#define TCP_MAX_PKTLEN	((64*1024-1)*4)	// pktlen in LRH is 16 bits, so the
                                        // max pktlen is (64k-1)*4 = 256k-4
#define TCP_MAX_MTU (TCP_MAX_PKTLEN - TCP_MAX_PSM_HEADER)
#define TCP_DEFAULT_MTU (64*1024)
#define TCP_IOV_SIZE	1024
#define TCP_INACT_SKIP_POLLS	20
#define TCP_ACT_SKIP_POLLS	10
#define TCP_SHRT_BUF_SIZE	512
#define TCP_CONN_MSG_BUF_BLOCK	512
// Direct user buffer doesn't work with multi-rail. The current code intends
// to exclusively use user buffer on direct write. But with short buffer,
// which avoids HOL blocking on small msgs, what may happen is that in one
// socket read we intend to directly write into user buffer, and in another
// socket read, the msg to get fits into short buffer, so we get the whole
// msg and will copy it into user buffer during msg processing. A conflict then
// happens if both happen write the same user buffer. To resolve this issue, we
// will need to lock/sync user buffer at deeper level. Given that so far we do
// not see benefit on direct user buffer on real apps, we turn off this feature
// for now.
#define TCP_DIRECT_USR_BUF	0 // set to ZERO to turn off direct user buffer feature

// this structure can be part of psm2_ep
// one instance of this per local end point (NIC)
// we will create a single UDP socket with related resources to
// permit an eager data movement mechanism
// conceptually similar to a psmi_context_t which refers to an HFI context
// TODO - later could optimize cache hit rates by putting some of the less
// frequently used fields in a different part of psm2_ep struct
struct psm3_sockets_ep {
	unsigned sockets_mode;	// PSM3_SOCKETS_TCP or PSM3_SOCKETS_UDP
	int udp_rx_fd;	// SOCK_DGRAM
	int udp_tx_fd;	// SOCK_DGRAM
	/* fields specific to TCP */
	int listener_fd; // listening socket
	int tcp_incoming_fd; // latest incoming socket
#ifdef PSM_TCP_POLL
	struct pollfd *fds; // one extra for listening socket
#else
	int efd; // epoll fd
	struct epoll_event event; // epoll event
	struct epoll_event *events; // epoll events
	int *fds;
#endif
	int nfds;
	int max_fds;
        struct fd_ctx **map_fds; // map  fd -> fd_ctx
        int map_nfds; // map size
  
	uint32_t snd_pace_thresh; // send pace threshold
	uint32_t shrt_buf_size; // shrt_buf size
	int poll_count; // current poll count
	int inactive_skip_polls; // polls to skip under inactive connections
	int active_skip_polls_offset; // tailored for internal use. it's inactive_skip_polls - active_skip_polls
	struct msghdr snd_msg; // struct used for sendmsg
	/* fields specific to UDP */
	unsigned udp_gso;	// is GSO enabled for UDP, max chunk_size
	uint8_t *sbuf_udp_gso;	// buffer to compose UDP GSO packet sequence
	int udp_gso_zerocopy;	// is UDP GSO Zero copy option enabled
	int udp_gro; // will be used later
	/* fields used for both UDP and TCP */
	uint8_t *sbuf;
	uint8_t *rbuf;
	uint32_t buf_size;
	uint32_t max_buffering;	// max send/recv side buffering below us
	uint32_t if_index;	// index of our local netdev
	in_port_t pri_socket;	// primary socket, UDP/TCP based on sockets_mode
	in_port_t aux_socket;	// for TCP only: aux UDP socket
	in_port_t out_socket;   // for TCP only: local outgoing socket for reuse
	int if_mtu;
	short if_flags;
	// if asked to revisit a packet we save it here
	uint8_t *revisit_buf;
	int revisit_fd;
	uint32_t revisit_payload_size;
	int* rfds; // array of fds that need to revisit
	int nrfd; // number of fds need to revisit
	int max_rfds; // array capacity

	/* remaining fields are for TCP only */
	// read in partial pkt in rbuf
	int rbuf_cur_fd; // socket to continue read
};

extern psm2_error_t psm3_ep_open_sockets(psm2_ep_t ep, int unit, int port,
			int addr_index, psm2_uuid_t const job_key);
extern void psm3_hfp_sockets_context_initstats(psm2_ep_t ep);
extern void psm3_ep_free_sockets(psm2_ep_t ep);
extern psm2_error_t psm3_sockets_ips_proto_init(struct ips_proto *proto,
			uint32_t cksum_sz);
extern psm2_error_t psm3_sockets_ips_proto_update_linkinfo(
			struct ips_proto *proto);
extern int psm3_sockets_poll_type(int poll_type, psm2_ep_t ep);
extern psm2_error_t psm3_tune_tcp_socket(const char *sck_name, psm2_ep_t ep, int fd);
extern unsigned psm3_sockets_parse_inet(int reload);

#endif // PSM_HAL_SOCKETS_EP_H
#endif // PSM_SOCKETS
