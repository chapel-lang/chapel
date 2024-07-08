#ifdef PSM_SOCKETS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2017 Intel Corporation.

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

#ifndef _PSM_HAL_SOCKETS_HAL_H
#define _PSM_HAL_SOCKETS_HAL_H

#include "psm_user.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "psm_mq_internal.h"
#include "sockets_user.h"

/* declare hfp_sockets_t struct, (combines public psmi_hal_instance_t
   together with a private struct) */
typedef struct _hfp_sockets
{
	psmi_hal_instance_t phi;
} hfp_sockets_t;

static const uint8_t EXP_HDR[] =
	{0x00, 0x02, 0x00, 0x01,
	 0x00, 0x00, 0x00, 0x01,
	 0xce, 0x00, 0x80, 0x01};
static const uint8_t EXP_HDR_MASK[] =
	{0xFF, 0xFF, 0xFF, 0xFF,
         0x00, 0x00, 0xFF, 0xFF,
         0xFF, 0x00, 0xFF, 0xFF};

psm2_error_t psm3_sockets_ips_ptl_init_pre_proto_init(struct ptl_ips *ptl);
psm2_error_t psm3_sockets_ips_ptl_init_post_proto_init(struct ptl_ips *ptl);
psm2_error_t psm3_sockets_ips_ptl_fini(struct ptl_ips *ptl);

psm2_error_t psm3_sockets_ips_ptl_pollintr(psm2_ep_t ep,
				struct ips_recvhdrq *recvq, int fd_pipe, int next_timeout,
				uint64_t *pollok, uint64_t *pollcyc, uint64_t *pollintr);

int psm3_sockets_ips_ptl_process_unknown(const struct ips_recvhdrq_event *rcv_ev);

psm2_error_t
psm3_tcp_proto_flow_flush_pio(struct ips_flow *flow, int *nflushed);

psm2_error_t
psm3_sockets_recvhdrq_init(const struct ips_epstate *epstate,
		  const struct ips_proto *proto,
		  const struct ips_recvhdrq_callbacks *callbacks,
		  struct ips_recvhdrq *recvq
		);

psm2_error_t psm3_sockets_udp_recvhdrq_progress(struct ips_recvhdrq *recvq, bool force);
psm2_error_t psm3_sockets_tcp_recvhdrq_progress(struct ips_recvhdrq *recvq, bool force);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
void* psm3_sockets_gdr_convert_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep);
#endif /* PSM_CUDA || PSM_ONEAPI */

#define FD_STATE_NONE 0
#define FD_STATE_READY 1
#define FD_STATE_VALID 2
#define FD_STATE_ESTABLISHED 3
struct fd_ctx {
	/* index in ep fds array. -1 indicates no index */
	int index;
	/* short buffer */
	uint8_t *shrt_buf;
	/* data start position */
	uint32_t offset;
	/* data received */
	uint32_t len;
	/* length of the cur pkt if available */
	uint32_t pkt_len;
	/* extra buffer to store more data, it can be user buffer, or ep rbuf */
	/* extra buffer may share between fds, once we use it we need to stick */
	/* on it until we are done */
	uint8_t *extra_buf;
	/* indicate whether need to revisit data */
	uint8_t revisit;
	/* fd state */
	uint8_t state;
	ips_epaddr_t *ipsaddr;
};

/*
 Function sets elements in fds map to -1
 */
static __inline__
psm2_error_t psm3_sockets_tcp_insert_fd_index_to_map(
		psm2_ep_t ep, int fd, int index, uint8_t state) {
	if (fd < 0) {
		// no actions for -1 value
		return PSM2_OK;
	}

	if (fd >= ep->sockets_ep.map_nfds) {
		// In order to handle map table with
		// fd indexes we have to allocated table at least with (fd + 1) size.
		// Add additional memory slice to exclude often memory allocation.
		int newsz = (fd + 1) + TCP_INC_CONN;
		ep->sockets_ep.map_fds = psmi_realloc(ep, NETWORK_BUFFERS,
				ep->sockets_ep.map_fds, newsz * sizeof(struct fd_ctx *));
		if (ep->sockets_ep.map_fds == NULL) {
			_HFI_ERROR("Unable to allocate memory for fd index map\n");
			return PSM2_NO_MEMORY;
		}
		// we have to initialize new elements after psmi_realloc call
		memset(ep->sockets_ep.map_fds + ep->sockets_ep.map_nfds, 0,
			(newsz - ep->sockets_ep.map_nfds) * sizeof(struct fd_ctx *));

		ep->sockets_ep.map_nfds = newsz;
		_HFI_VDBG("Increased map_fds to size %d\n", ep->sockets_ep.map_nfds);
	}

	if (ep->sockets_ep.map_fds[fd] == NULL) {
		ep->sockets_ep.map_fds[fd] = psmi_calloc(ep, NETWORK_BUFFERS, 1, sizeof(struct fd_ctx));
		if (ep->sockets_ep.map_fds[fd] == NULL) {
			_HFI_ERROR("Unable to allocate memory for fd context\n");
			return PSM2_NO_MEMORY;
		}
		// we set shrt_buf_size during psm3_sockets_ips_proto_init. when we create fd_ctx for
		// listener_fd and aux_rx_fd, shrt_buf_size is still zero. listener_fd doesn't need
		// fd_ctx, so it's fine. For aux_rx_fd, we use TCP_SHRT_BUF_SIZE (512) since this fd
		// is for ctr msg that shall have size < 512.
		uint32_t size = fd == ep->sockets_ep.udp_rx_fd ? TCP_SHRT_BUF_SIZE : ep->sockets_ep.shrt_buf_size;
		if (size) {
			ep->sockets_ep.map_fds[fd]->shrt_buf = (uint8_t *) psmi_calloc(ep, NETWORK_BUFFERS, size, 1);
			if (ep->sockets_ep.map_fds[fd]->shrt_buf == NULL) {
				_HFI_ERROR("Unable to allocate memory for fd short buffer\n");
				return PSM2_NO_MEMORY;
			}
			_HFI_VDBG("Created fd_ctx with shrt_buf_size=%d for fd=%d\n", size, fd);
		} else {
			// no shrt_buf. we use rbuf
			ep->sockets_ep.map_fds[fd]->shrt_buf = ep->sockets_ep.rbuf;
		}
	}
	ep->sockets_ep.map_fds[fd]->index = index;
	ep->sockets_ep.map_fds[fd]->state = state;
	return PSM2_OK;
}

static __inline__
psm2_error_t psm3_sockets_tcp_clear_fd_in_map(psm2_ep_t ep, int fd) {
	if (fd < 0) {
		// no actions for -1 value
		return PSM2_OK;
	}

	if (fd < ep->sockets_ep.map_nfds) {
		if (ep->sockets_ep.map_fds[fd]) {
			if (ep->sockets_ep.map_fds[fd]->shrt_buf != ep->sockets_ep.rbuf
				&& ep->sockets_ep.map_fds[fd]->shrt_buf) {
				psmi_free(ep->sockets_ep.map_fds[fd]->shrt_buf);
			}
			psmi_free(ep->sockets_ep.map_fds[fd]);
			ep->sockets_ep.map_fds[fd] = NULL;
		} else {
			// unexpected situation
			_HFI_INFO("Unexpected fd[%d], fd_ctx already cleared. No actions\n", fd);
		}
	} else {
		_HFI_INFO("Incorrect fd[%d] for clear operation, map size[%d]. No actions.\n", fd,
				ep->sockets_ep.map_nfds);
	}
	return PSM2_OK;
}

static __inline__
struct fd_ctx * psm3_sockets_get_fd_ctx(psm2_ep_t ep, int fd) {
	if (fd >= 0 && fd < ep->sockets_ep.map_nfds && ep->sockets_ep.map_fds[fd]) {
		return ep->sockets_ep.map_fds[fd];
	}
	_HFI_VDBG("No fd_ctx found for fd=%d\n", fd);
	return NULL;
}

static __inline__
int psm3_sockets_get_index_by_fd(psm2_ep_t ep, int fd) {
	if (fd >= 0 && (fd < ep->sockets_ep.map_nfds) && ep->sockets_ep.map_fds[fd]) {
		return ep->sockets_ep.map_fds[fd]->index;
	}
	return -1;
}

static __inline__
psm2_error_t psm3_sockets_tcp_set_fd_state(psm2_ep_t ep, int fd, uint8_t state)
{
	if (fd >= 0 && fd < ep->sockets_ep.map_nfds && ep->sockets_ep.map_fds[fd]) {
		ep->sockets_ep.map_fds[fd]->state = state;
		return PSM2_OK;
	}
	_HFI_VDBG("No fd_ctx found for fd=%d\n", fd);
	return PSM2_INTERNAL_ERR;
}

#ifdef PSM_TCP_POLL
static __inline__
psm2_error_t psm3_sockets_tcp_add_fd(psm2_ep_t ep, int fd, uint8_t state)
{
	if_pf (ep->sockets_ep.nfds >= ep->sockets_ep.max_fds) {
		ep->sockets_ep.max_fds += TCP_INC_CONN;
		ep->sockets_ep.fds = psmi_realloc(ep, NETWORK_BUFFERS,
			ep->sockets_ep.fds, ep->sockets_ep.max_fds * sizeof(struct pollfd));
		if (ep->sockets_ep.fds == NULL) {
			_HFI_ERROR( "Unable to allocate memory for pollfd\n");
			return PSM2_NO_MEMORY;
		}
		_HFI_VDBG("Increased fds to size %d\n", ep->sockets_ep.max_fds);
	}
	ep->sockets_ep.fds[ep->sockets_ep.nfds].fd = fd;
	ep->sockets_ep.fds[ep->sockets_ep.nfds].events = POLLIN;
	if (psm3_sockets_tcp_insert_fd_index_to_map(ep, fd, ep->sockets_ep.nfds, state) ==
			PSM2_NO_MEMORY) {
		return PSM2_NO_MEMORY;
	}
	// New fd is stored only if memory allocated for both: fd array and map
	ep->sockets_ep.nfds += 1;

	return PSM2_OK;
}

static __inline__
void psm3_sockets_tcp_close_fd(psm2_ep_t ep, int fd, int index, struct ips_flow *flow)
{
	// if has remainder data, reset related fields to stop
	// sending them, so no intended operation on closed socket
	if (flow) {
		flow->send_remaining = 0;
		flow->conn_msg_remainder = 0;
	}
	// if has partial received data, reset related fields to discard it
	if (ep->sockets_ep.rbuf_cur_fd == fd) {
		ep->sockets_ep.rbuf_cur_fd = 0;
	}

	if (index < 0) {
		index = psm3_sockets_get_index_by_fd(ep, fd);
	}

	if (index >= 0 && index < ep->sockets_ep.nfds) {
		// clear map element
		psm3_sockets_tcp_clear_fd_in_map(ep, ep->sockets_ep.fds[index].fd);
		// remove from poll list before close it
		ep->sockets_ep.fds[index].fd = -1;
	}
	close(fd);
	_HFI_VDBG("Closed fd=%d\n", fd);
}

static __inline__
void psm3_sockets_adjust_fds(psm2_ep_t ep) {
	for (int i = 0; i < ep->sockets_ep.nfds;) {
		if (ep->sockets_ep.fds[i].fd == -1) {
			ep->sockets_ep.nfds--;
			ep->sockets_ep.fds[i] = ep->sockets_ep.fds[ep->sockets_ep.nfds];

			// insert new fd in map, if fd is -1, it is ignored.
			// we do not expect increasing of the map size at
			// this step
			psm3_sockets_tcp_insert_fd_index_to_map(ep,
				ep->sockets_ep.fds[i].fd, i,
				ep->sockets_ep.fds[i].fd >= 0 && ep->sockets_ep.map_fds[ep->sockets_ep.fds[i].fd] ?
					ep->sockets_ep.map_fds[ep->sockets_ep.fds[i].fd]->state : FD_STATE_NONE);
		} else {
			i++;
		}
	}
}

static __inline__
void psm3_sockets_switch_fds(psm2_ep_t ep, int id1, int id2) {
	if_pf (id1 == id2) {
		return;
	}

	int fd1 = ep->sockets_ep.fds[id1].fd;
	int fd2 = ep->sockets_ep.fds[id2].fd;
	ep->sockets_ep.fds[id1].fd = fd2;
	ep->sockets_ep.fds[id2].fd = fd1;
	if (fd1 >= 0 && ep->sockets_ep.map_fds[fd1]) {
		ep->sockets_ep.map_fds[fd1]->index = id2;
	}
	if (fd2 >= 0 && ep->sockets_ep.map_fds[fd2]) {
		ep->sockets_ep.map_fds[fd2]->index = id1;
	}
}
#else
static __inline__
psm2_error_t psm3_sockets_tcp_add_fd(psm2_ep_t ep, int fd, uint8_t state)
{
	if_pf (ep->sockets_ep.nfds >= ep->sockets_ep.max_fds) {
		ep->sockets_ep.max_fds += TCP_INC_CONN;
		ep->sockets_ep.fds = psmi_realloc(ep, NETWORK_BUFFERS,
			ep->sockets_ep.fds, ep->sockets_ep.max_fds * sizeof(int));
		if (ep->sockets_ep.fds == NULL) {
			_HFI_ERROR( "Unable to allocate memory for pollfd\n");
			return PSM2_NO_MEMORY;
		}
		_HFI_VDBG("Increased fds to size %d\n", ep->sockets_ep.max_fds);
	}
	ep->sockets_ep.fds[ep->sockets_ep.nfds] = fd;
	if (psm3_sockets_tcp_insert_fd_index_to_map(ep, fd, ep->sockets_ep.nfds, state) ==
			PSM2_NO_MEMORY) {
		return PSM2_NO_MEMORY;
	}
	// New fd is stored only if memory allocated for both: fd array and map
	ep->sockets_ep.nfds += 1;

	ep->sockets_ep.event.data.fd = fd;
	ep->sockets_ep.event.events = EPOLLIN;
	int ret = epoll_ctl (ep->sockets_ep.efd, EPOLL_CTL_ADD, fd, &ep->sockets_ep.event);
	if (ret == -1) {
		_HFI_ERROR("Couldn't add fd=%d into epoll. %d: %s\n", fd, errno, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}

	return PSM2_OK;
}

static __inline__
void psm3_sockets_tcp_close_fd(psm2_ep_t ep, int fd, int index, struct ips_flow *flow)
{
	// if has remainder data, reset related fields to stop
	// sending them, so no intended operation on closed socket
	if (flow) {
		flow->send_remaining = 0;
		flow->conn_msg_remainder = 0;
	}
	// if has partial received data, reset related fields to discard it
	if (ep->sockets_ep.rbuf_cur_fd == fd) {
		ep->sockets_ep.rbuf_cur_fd = 0;
	}

	if (index < 0) {
		index = psm3_sockets_get_index_by_fd(ep, fd);
	}

	if (index >= 0 && index < ep->sockets_ep.nfds) {
		// clear map element
		psm3_sockets_tcp_clear_fd_in_map(ep, fd);
		// remove from poll list before close it
		ep->sockets_ep.fds[index] = -1;
	}
	close(fd);
	_HFI_VDBG("Closed fd=%d\n", fd);
}
#endif
#endif /* _PSM_HAL_SOCKETS_HAL_H */
#endif /* PSM_SOCKETS */
