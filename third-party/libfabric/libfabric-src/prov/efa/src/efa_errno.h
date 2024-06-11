/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_ERRNO_H
#define EFA_ERRNO_H

#define EFA_IO_COMP_STATUS_START	0

/**
 * Status codes 0..4095 are reserved for RDMA Core (undocumented; possibly
 * arbitrary)
 */
#define EFA_IO_COMP_STATUS_RESERVED	4095

#define EFA_PROV_ERRNO_START		(EFA_IO_COMP_STATUS_RESERVED + 1)

#define EFA_ERRNO_IS_IO_COMP_STATUS(e)	(((e) >= EFA_IO_COMP_STATUS_START) && ((e) < EFA_IO_COMP_STATUS_MAX))
#define EFA_ERRNO_IS_PROV(e)		(((e) >= EFA_PROV_ERRNO_START) && ((e) < EFA_PROV_ERRNO_MAX))


/** @defgroup EfaErrnoXMacros EFA error-handling X macros
 * 
 * The macros described here are [X
 * macros](https://en.wikipedia.org/wiki/X_macro) or "higher-order" macros,
 * which are designed to take a function-like macro as the sole argument to
 * transform the error code information as needed. This reduces boilerplate code
 * and overall maintenance overhead by facilitating reliable in-order
 * enumeration for the various C language constructs used for the EFA provider's
 * error-handling logic.
 *
 * The list items are modeled as a `(code, suffix, ...)` triplet;
 * described below.
 *
 * @param[in] code   Numeric error code
 * @param[in] suffix Unique suffix for the resulting identifier
 * @param[in] ...    Everything else is considered part of the error message,
 *                   accessible via `__VA_ARGS__`
 *
 * @note The error message needs to be stringized (i.e. `#__VA_ARGS__`)
 *
 * To add a new error code, simply add an entry to the list.
 *
 * ```c
 * _(9999, MY_ERROR, Something went wrong!)
 * ```
 *
 * @{
 */

/**
 * @brief `EFA_IO_COMP_STATUS_*` code definitions
 *
 * These status codes correspond directly to the `efa_io_comp_status` codes
 * defined by the RDMA Core EFA provider.
 */
#define EFA_IO_COMP_STATUSES(_)									\
	_(0,	OK,				Success)					\
	_(1,	FLUSHED,			Flushed during queue pair destroy)		\
	_(2,	LOCAL_ERROR_QP_INTERNAL_ERROR,	Internal queue pair error)			\
	_(3,	LOCAL_ERROR_INVALID_OP_TYPE,	Invalid operation type)				\
	_(4,	LOCAL_ERROR_INVALID_AH,		Invalid address handle)				\
	_(5,	LOCAL_ERROR_INVALID_LKEY,	Invalid local key (LKEY))			\
	_(6,	LOCAL_ERROR_BAD_LENGTH,		Message too long)				\
	_(7,	REMOTE_ERROR_BAD_ADDRESS,	Destination ENI is down or does not run EFA)	\
	_(8,	REMOTE_ERROR_ABORT,		Receiver connection aborted)			\
	_(9,	REMOTE_ERROR_BAD_DEST_QPN,	Invalid receiver queue pair number (QPN))	\
	_(10,	REMOTE_ERROR_RNR,		Receiver not ready)				\
	_(11,	REMOTE_ERROR_BAD_LENGTH,	Receiver scatter-gather list (SGL) too short)	\
	_(12,	REMOTE_ERROR_BAD_STATUS,	Unexpected status received from remote)		\
	_(13,	LOCAL_ERROR_UNRESP_REMOTE,	Unresponsive receiver (connection never established or unknown))

/**
 * @brief EFA provider proprietary error codes
 */
#define EFA_PROV_ERRNOS(_)									\
	_(4096,	OTHER,				Unknown error)					\
	_(4097,	DEPRECATED_PKT_TYPE,		Deprecated packet type encountered)		\
	_(4098,	INVALID_PKT_TYPE,		Invalid packet type encountered)		\
	_(4099,	UNKNOWN_PKT_TYPE,		Unknown packet type encountered)		\
	_(4100,	PKT_POST,			Failure to post packet)				\
	_(4101,	PKT_SEND,			Failure to send packet)				\
	_(4102,	PKT_PROC_MSGRTM,		Error processing non-tagged RTM)		\
	_(4103,	PKT_PROC_TAGRTM,		Error processing tagged RTM)			\
	_(4104,	PKT_ALREADY_PROCESSED,		Packet already processed)			\
	_(4105,	OOM,				Out of memory)					\
	_(4106,	MR_DEREG,			MR deregistration error)			\
	_(4107,	RXE_COPY,			RX entry copy error)				\
	_(4108,	RXE_POOL_EXHAUSTED,		RX entries exhausted)				\
	_(4109,	TXE_POOL_EXHAUSTED,		TX entries exhausted)				\
	_(4110,	AV_INSERT,			Failure inserting address into address vector)	\
	_(4111,	RMA_ADDR,			RMA address verification failed)		\
	_(4112,	INTERNAL_RX_BUF_POST,		Failure to post internal receive buffers)	\
	_(4113,	PEER_HANDSHAKE,			Failure posting handshake to peer)		\
	_(4114,	WR_POST_SEND,			Failure to post work request(s) to send queue)	\
	_(4115,	RTM_MISMATCH,			RTM size mismatch)				\
	_(4116,	READ_POST,			Error posting read request)			\
	_(4117,	RDMA_READ_POST,			Error posting RDMA read request)		\
	_(4118,	INVALID_DATATYPE,		Invalid data type encountered)			\
	_(4119,	WRITE_SEND_COMP,		Failure to write send completion)		\
	_(4120,	WRITE_RECV_COMP,		Failure to write receive completion)		\
	_(4121,	DGRAM_CQ_READ,			Error reading from DGRAM CQ)			\
	_(4122,	SHM_INTERNAL_ERROR,		SHM internal error)				\
	_(4123,	WRITE_SHM_CQ_ENTRY,		Failure to write CQ entry for SHM operation)	\
	_(4124, ESTABLISHED_RECV_UNRESP,	Unresponsive receiver (connection previously established))

/** @} */

/* Doxygen's preprocessor will handle expansion(s) in the inline documentation */
#define EFA_IO_COMP_STATUS_ENUM(code, suffix, ...)	EFA_IO_COMP_STATUS_##suffix = code,	/**< (code) __VA_ARGS__ */
#define EFA_PROV_ERRNO_ENUM(code, suffix, ...)		FI_EFA_ERR_##suffix = code,		/**< (code) __VA_ARGS__ */

/**
 * The `EFA_IO_COMP_STATUS_*` codes correspond directly to the
 * `efa_io_comp_status` codes defined by the RDMA Core EFA provider. These are
 * included mainly for compatibility and ease-of-debugging. As a rule of thumb,
 * the EFA provider should only report these as a fallback; preferring
 * proprietary error codes instead.
 *
 * Proprietary error codes begin at 4096 to avoid conflicts with any current or
 * future RDMA Core error codes.
 *
 * @sa #EFA_IO_COMP_STATUSES
 * @sa #EFA_PROV_ERRNOS
 */
enum efa_errno {
	EFA_IO_COMP_STATUSES(EFA_IO_COMP_STATUS_ENUM)
	EFA_IO_COMP_STATUS_MAX, /**< Sentinel value for #EFA_IO_COMP_STATUSES */
	EFA_PROV_ERRNOS(EFA_PROV_ERRNO_ENUM)
	EFA_PROV_ERRNO_MAX, /**< Sentinel value for EFA provider error codes */
};

#undef EFA_IO_COMP_STATUS_ENUM
#undef EFA_PROV_ERRNO_ENUM

const char *efa_strerror(enum efa_errno);
void efa_show_help(enum efa_errno);

#endif
