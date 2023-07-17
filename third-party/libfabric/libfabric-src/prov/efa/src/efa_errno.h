/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EFA_ERRNO_H
#define EFA_ERRNO_H

enum efa_errno {
	/*
	 * Status codes 0..4095 reserved for rdma-core. Any enums here should
	 * correspond to EFA_IO_COMP_STATUS_xxx codes as defined in
	 * rdma-core/providers/efa/efa-io-defs.h
	 */
	FI_EFA_OK                            = 0,    /* Successful completion */
	FI_EFA_FLUSHED                       = 1,    /* Flushed during QP destroy */
	FI_EFA_LOCAL_ERROR_QP_INTERNAL_ERROR = 2,    /* Internal QP error */
	FI_EFA_LOCAL_ERROR_INVALID_OP_TYPE   = 3,    /* Bad operation type */
	FI_EFA_LOCAL_ERROR_INVALID_AH        = 4,    /* Bad AH */
	FI_EFA_LOCAL_ERROR_INVALID_LKEY      = 5,    /* LKEY not registered or does not match IOVA */
	FI_EFA_LOCAL_ERROR_BAD_LENGTH        = 6,    /* Message too long */
	FI_EFA_REMOTE_ERROR_BAD_ADDRESS      = 7,    /* Destination ENI is down or does not run EFA */
	FI_EFA_REMOTE_ERROR_ABORT            = 8,    /* Connection was reset by remote side */
	FI_EFA_REMOTE_ERROR_BAD_DEST_QPN     = 9,    /* Bad dest QP number (QP does not exist or is in error state) */
	FI_EFA_REMOTE_ERROR_RNR              = 10,   /* Destination resource not ready (no WQEs posted on RQ) */
	FI_EFA_REMOTE_ERROR_BAD_LENGTH       = 11,   /* Receiver SGL too short */
	FI_EFA_REMOTE_ERROR_BAD_STATUS       = 12,   /* Unexpected status returned by responder */
	FI_EFA_LOCAL_ERROR_UNRESP_REMOTE     = 13,   /* Unresponsive remote - detected locally */
	/*
	 * EFA provider specific error codes begin here.
	 */
	FI_EFA_ERR_OTHER                     = 4096, /* Unknown/other error */
	FI_EFA_ERR_DEPRECATED_PKT_TYPE       = 4097, /* Deprecated packet type (e.g. RTS, CONNACK) */
	FI_EFA_ERR_INVALID_PKT_TYPE          = 4098, /* Invalid packet type */
	FI_EFA_ERR_UNKNOWN_PKT_TYPE          = 4099, /* Unknown packet type */
	FI_EFA_ERR_PKT_POST                  = 4100, /* Packet post error */
	FI_EFA_ERR_PKT_SEND                  = 4101, /* Packet send error */
	FI_EFA_ERR_PKT_PROC_MSGRTM           = 4102, /* Error processing non-tagged RTM */
	FI_EFA_ERR_PKT_PROC_TAGRTM           = 4103, /* Error processing tagged RTM */
	FI_EFA_ERR_PKT_ALREADY_PROCESSED     = 4104, /* Packet already processed */
	FI_EFA_ERR_OOM                       = 4105, /* Out of memory */
	FI_EFA_ERR_MR_DEREG                  = 4106, /* MR close/deregistration error */
	FI_EFA_ERR_RX_ENTRY_COPY             = 4107, /* Error copying RX entry */
	FI_EFA_ERR_RX_ENTRIES_EXHAUSTED      = 4108, /* RX entries exhausted */
	FI_EFA_ERR_TX_ENTRIES_EXHAUSTED      = 4109, /* TX entries exhausted */
	FI_EFA_ERR_AV_INSERT                 = 4110, /* Error inserting into address vector */
	FI_EFA_ERR_RMA_ADDR                  = 4111, /* RMA address verification error */
	FI_EFA_ERR_INTERNAL_RX_BUF_POST      = 4112, /* Error posting internal RX buffer */
	FI_EFA_ERR_PEER_HANDSHAKE            = 4113, /* Error posting handshake to peer */
	FI_EFA_ERR_WR_POST_SEND              = 4114, /* Error posting work request(s) to send queue */
	FI_EFA_ERR_RTM_MISMATCH              = 4115, /* RTM size mismatch */
	FI_EFA_ERR_READ_POST                 = 4116, /* Error posting read request */ 
	FI_EFA_ERR_RDMA_READ_POST            = 4117, /* Error posting RDMA read request */
	FI_EFA_ERR_INVALID_DATATYPE          = 4118, /* Invalid datatype encountered */
	FI_EFA_ERR_WRITE_SEND_COMP           = 4119, /* Error writing SEND completion */
	FI_EFA_ERR_WRITE_RECV_COMP           = 4120, /* Error writing RECV completion */
	FI_EFA_ERR_DGRAM_CQ_READ             = 4121, /* Error reading from CQ */
	FI_EFA_ERR_SHM_INTERNAL_ERROR        = 4122, /* SHM internal error */
	FI_EFA_ERR_WRITE_SHM_CQ_ENTRY        = 4123, /* Error writing CQ entry for SHM op */
	FI_EFA_ERRNO_MAX                             /* Dummy errno for upper bound */
};

const char *efa_strerror(enum efa_errno);

#endif
