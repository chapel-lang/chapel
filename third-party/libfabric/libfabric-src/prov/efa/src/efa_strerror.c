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

#include "efa_errno.h"

#define UNKNOWN_ERR_STR     "Unknown error"
#define FI_EFA_ERRNO_OFFSET FI_EFA_ERR_OTHER

static const char *efa_io_comp_status_str(enum efa_errno status)
{
	static const char *status_str[] = {
		[FI_EFA_OK]                            = "Success",
		[FI_EFA_FLUSHED]                       = "Flushed during queue pair destroy",
		[FI_EFA_LOCAL_ERROR_QP_INTERNAL_ERROR] = "Internal queue pair error",
		[FI_EFA_LOCAL_ERROR_INVALID_OP_TYPE]   = "Invalid operation type",
		[FI_EFA_LOCAL_ERROR_INVALID_AH]        = "Invalid address handle",
		[FI_EFA_LOCAL_ERROR_INVALID_LKEY]      = "Invalid local key (LKEY)",
		[FI_EFA_LOCAL_ERROR_BAD_LENGTH]        = "Message too long",
		[FI_EFA_REMOTE_ERROR_BAD_ADDRESS]      = "Invalid address",
		[FI_EFA_REMOTE_ERROR_ABORT]            = "Receiver connection aborted",
		[FI_EFA_REMOTE_ERROR_BAD_DEST_QPN]     = "Invalid receiver queue pair number (QPN)",
		[FI_EFA_REMOTE_ERROR_RNR]              = "Receiver not ready",
		[FI_EFA_REMOTE_ERROR_BAD_LENGTH]       = "Receiver scatter-gather list (SGL) too short",
		[FI_EFA_REMOTE_ERROR_BAD_STATUS]       = "Unexpected status received from remote",
		[FI_EFA_LOCAL_ERROR_UNRESP_REMOTE]     = "Unresponsive receiver",
	};

	return (status < FI_EFA_OK || status > FI_EFA_LOCAL_ERROR_UNRESP_REMOTE)
		? UNKNOWN_ERR_STR
		: status_str[status];
}

static const char *efa_errno_str(enum efa_errno err)
{
	static const char *errno_str[] = {
		[FI_EFA_ERR_OTHER                 - FI_EFA_ERRNO_OFFSET] = UNKNOWN_ERR_STR,
		[FI_EFA_ERR_DEPRECATED_PKT_TYPE   - FI_EFA_ERRNO_OFFSET] = "Deprecated packet type encountered",
		[FI_EFA_ERR_INVALID_PKT_TYPE      - FI_EFA_ERRNO_OFFSET] = "Invalid packet type encountered",
		[FI_EFA_ERR_UNKNOWN_PKT_TYPE      - FI_EFA_ERRNO_OFFSET] = "Unknown packet type encountered",
		[FI_EFA_ERR_PKT_POST              - FI_EFA_ERRNO_OFFSET] = "Failure to post packet",
		[FI_EFA_ERR_PKT_SEND              - FI_EFA_ERRNO_OFFSET] = "Failure to send packet",
		[FI_EFA_ERR_PKT_PROC_MSGRTM       - FI_EFA_ERRNO_OFFSET] = "Error processing non-tagged RTM",
		[FI_EFA_ERR_PKT_PROC_TAGRTM       - FI_EFA_ERRNO_OFFSET] = "Error processing tagged RTM",
		[FI_EFA_ERR_PKT_ALREADY_PROCESSED - FI_EFA_ERRNO_OFFSET] = "Packet already processed",
		[FI_EFA_ERR_OOM                   - FI_EFA_ERRNO_OFFSET] = "Out of memory",
		[FI_EFA_ERR_MR_DEREG              - FI_EFA_ERRNO_OFFSET] = "MR deregistration error",
		[FI_EFA_ERR_RX_ENTRY_COPY         - FI_EFA_ERRNO_OFFSET] = "RX entry copy error",
		[FI_EFA_ERR_RX_ENTRIES_EXHAUSTED  - FI_EFA_ERRNO_OFFSET] = "RX entries exhausted",
		[FI_EFA_ERR_TX_ENTRIES_EXHAUSTED  - FI_EFA_ERRNO_OFFSET] = "TX entries exhausted",
		[FI_EFA_ERR_AV_INSERT             - FI_EFA_ERRNO_OFFSET] = "Failure inserting address into address vector",
		[FI_EFA_ERR_RMA_ADDR              - FI_EFA_ERRNO_OFFSET] = "RMA address verification failed",
		[FI_EFA_ERR_INTERNAL_RX_BUF_POST  - FI_EFA_ERRNO_OFFSET] = "Failure to post internal receive buffers",
		[FI_EFA_ERR_PEER_HANDSHAKE        - FI_EFA_ERRNO_OFFSET] = "Failure to post handshake to peer",
		[FI_EFA_ERR_WR_POST_SEND          - FI_EFA_ERRNO_OFFSET] = "Failure to post work request(s) to send queue",
		[FI_EFA_ERR_RTM_MISMATCH          - FI_EFA_ERRNO_OFFSET] = "RTM size mismatch",
		[FI_EFA_ERR_READ_POST             - FI_EFA_ERRNO_OFFSET] = "Error posting read request",
		[FI_EFA_ERR_RDMA_READ_POST        - FI_EFA_ERRNO_OFFSET] = "Error posting RDMA read request",
		[FI_EFA_ERR_INVALID_DATATYPE      - FI_EFA_ERRNO_OFFSET] = "Invalid datatype encountered",
		[FI_EFA_ERR_WRITE_SEND_COMP       - FI_EFA_ERRNO_OFFSET] = "Failure to write send completion",
		[FI_EFA_ERR_WRITE_RECV_COMP       - FI_EFA_ERRNO_OFFSET] = "Failure to write receive completion",
		[FI_EFA_ERR_DGRAM_CQ_READ         - FI_EFA_ERRNO_OFFSET] = "Error reading from DGRAM CQ",
		[FI_EFA_ERR_SHM_INTERNAL_ERROR    - FI_EFA_ERRNO_OFFSET] = "SHM internal error",
		[FI_EFA_ERR_WRITE_SHM_CQ_ENTRY    - FI_EFA_ERRNO_OFFSET] = "Failure to write CQ entry for SHM operation",
	};

	return (err < FI_EFA_ERRNO_OFFSET || err >= FI_EFA_ERRNO_MAX)
		? UNKNOWN_ERR_STR
		: errno_str[err - FI_EFA_ERRNO_OFFSET];
}

/**
 * @brief   Convert EFA error into a printable string
 * @details Given a non-negative EFA-specific error code, this function returns
 *          a pointer to a static string that corresponds to it. This text is
 *          suitable for printing in debug/warning/error messages.
 *
 * @param [in] err An EFA-specific error code
 * @return     Pointer to a string corresponding to err
 */
const char *efa_strerror(enum efa_errno err)
{
	return err >= FI_EFA_ERRNO_OFFSET
		? efa_errno_str(err)
		: efa_io_comp_status_str(err);
}
