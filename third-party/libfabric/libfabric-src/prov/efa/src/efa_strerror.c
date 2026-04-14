/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_errno.h"

#define IO_COMP_STATUS_MESSAGES(code, suffix, ...)	[EFA_IO_COMP_STATUS_##suffix] = #__VA_ARGS__,
#define PROV_ERRNO_MESSAGES(code, suffix, ...)		[FI_EFA_ERR_##suffix - EFA_PROV_ERRNO_START] = #__VA_ARGS__,

static const char *efa_io_comp_strerror(enum efa_errno status)
{
	static const char *io_comp_status_str[] = {
		EFA_IO_COMP_STATUSES(IO_COMP_STATUS_MESSAGES)
	};

	return EFA_ERRNO_IS_IO_COMP_STATUS(status)
		? io_comp_status_str[status]
		: "Unknown error";
}

static const char *efa_prov_strerror(enum efa_errno err)
{
	static const char *prov_errno_str[] = {
		EFA_PROV_ERRNOS(PROV_ERRNO_MESSAGES)
	};

	return EFA_ERRNO_IS_PROV(err)
		? prov_errno_str[err - EFA_PROV_ERRNO_START]
		: "Unknown error";
}

/**
 * @brief Convert an EFA error code into a short, printable string
 *
 * Given a non-negative EFA-specific error code, this function returns a pointer
 * to a null-terminated string that corresponds to it; suitable for
 * interpolation in logging messages.
 *
 * @param[in]	err    An EFA-specific error code
 * @return	Null-terminated string with static storage duration (caller does
 *		not free).
 */
const char *efa_strerror(enum efa_errno err)
{
	return EFA_ERRNO_IS_PROV(err)
		? efa_prov_strerror(err)
		: efa_io_comp_strerror(err);
}

/**
 * @brief Report a long-form help message to the user
 *
 * Given a non-negative EFA-specific error code, this function prints a
 * long-form help message. These help messages are provided as a best effort
 * attempt at debugging known issues, especially for `EFA_IO_COMP_STATUS_*`
 * errors from RDMA Core.
 *
 * @param[in]	err    An EFA-specific error code
 *
 * @todo As this function grows, we should consider reading these long-form
 * messages from disk, instead of baking the strings in.
 */
void efa_show_help(enum efa_errno err) {
	char *help = "";
	switch (err) {
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_DEST_QPN:
		help = "This error is detected remotely; "
		"typically encountered when the peer process is no longer present";
		break;
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_UNREACH_REMOTE:
		help = "This error is detected locally. "
		"The peer is not reachable by the EFA device. "
		"This typically indicates one or more misconfigured "
		"EC2 instances; most often due to incorrect inbound/outbound "
		"security group rules and/or instances placed in different "
		"subnets. Refer to the public AWS documentation for EFA for "
		"up-to-date configuration requirements. This error can also be "
		"encountered when a peer process is no longer present.";
		break;
	case FI_EFA_ERR_ESTABLISHED_RECV_UNRESP:
		help = "This error is detected locally. "
		"The connection was previously established via handshake, "
		"which indicates the error is likely due to a hardware failure "
		"on the remote peer, or the peer process no longer being present.";
		break;
	case FI_EFA_ERR_UNESTABLISHED_RECV_UNRESP:
		help = "This error is detected locally. "
		"The peer is reachable by the EFA device but libfabric failed "
		"to complete a handshake, which indicates the error is likely "
		"due to the peer process no longer being present.";
		break;
	case FI_EFA_ERR_INVALID_PKT_TYPE_ZCPY_RX:
		help = "This error is detected locally. "
		"Please consider matching the local and remote libfabric versions, or turning off "
		"the zero-copy recv feature by setting FI_EFA_USE_ZCPY_RX=0 in the environment";
		break;
	default:
		return;
	}
	EFA_WARN(FI_LOG_CQ, "%s\n", help);
}
