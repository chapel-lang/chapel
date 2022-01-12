---
layout: page
title: fi_efa(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_efa \- The Amazon Elastic Fabric Adapter (EFA) Provider

# OVERVIEW

The EFA provider supports the Elastic Fabric Adapter (EFA) device on
Amazon EC2.  EFA provides reliable and unreliable datagram send/receive
with direct hardware access from userspace (OS bypass).

# SUPPORTED FEATURES

The following features are supported:

*Endpoint types*
: The provider supports endpoint type *FI_EP_DGRAM*, and *FI_EP_RDM* on a new
  Scalable (unordered) Reliable Datagram protocol (SRD). SRD provides support
  for reliable datagrams and more complete error handling than typically seen
  with other Reliable Datagram (RD) implementations. The EFA provider provides
  segmentation, reassembly of out-of-order packets to provide send-after-send
  ordering guarantees to applications via its *FI_EP_RDM* endpoint.

*RDM Endpoint capabilities*
: The following data transfer interfaces are supported via the *FI_EP_RDM*
  endpoint: *FI_MSG*, *FI_TAGGED*, and *FI_RMA*. *FI_SEND*, *FI_RECV*,
  *FI_DIRECTED_RECV*, *FI_MULTI_RECV*, and *FI_SOURCE* capabilities are supported.
  The endpoint provides send-after-send guarantees for data operations. The
  *FI_EP_RDM* endpoint does not have a maximum message size.

*DGRAM Endpoint capabilities*
: The DGRAM endpoint only supports *FI_MSG* capability with a maximum
  message size of the MTU of the underlying hardware (approximately 8 KiB).

*Address vectors*
: The provider supports *FI_AV_TABLE* and *FI_AV_MAP* address vector types.
  *FI_EVENT* is unsupported.

*Completion events*
: The provider supports *FI_CQ_FORMAT_CONTEXT*, *FI_CQ_FORMAT_MSG*, and
  *FI_CQ_FORMAT_DATA*. *FI_CQ_FORMAT_TAGGED* is supported on the RDM
  endpoint. Wait objects are not currently supported.

*Modes*
: The provider requires the use of *FI_MSG_PREFIX* when running over
  the DGRAM endpoint, and requires *FI_MR_LOCAL* for all memory
  registrations on the DGRAM endpoint.

*Memory registration modes*
: The RDM endpoint does not require memory registration for send and receive
  operations, i.e. it does not require *FI_MR_LOCAL*. Applications may specify
  *FI_MR_LOCAL* in the MR mode flags in order to use descriptors provided by the
  application. The *FI_EP_DGRAM* endpoint only supports *FI_MR_LOCAL*.

*Progress*
: The RDM endpoint supports both *FI_PROGRESS_AUTO* and *FI_PROGRESS_MANUAL*,
  with the default set to auto. However, receive side data buffers are not
  modified outside of completion processing routines. The DGRAM endpoint only
  supports *FI_PROGRESS_MANUAL*.

*Threading*
: The RDM endpoint supports *FI_THREAD_SAFE*, the DGRAM endpoint supports
  *FI_THREAD_DOMAIN*, i.e. the provider is not thread safe when using the DGRAM
  endpoint.

# LIMITATIONS

The DGRAM endpoint does not support *FI_ATOMIC* interfaces. For RMA operations,
completion events for RMA targets (*FI_RMA_EVENT*) is not supported. The DGRAM
endpoint does not fully protect against resource overruns, so resource
management is disabled for this endpoint (*FI_RM_DISABLED*).

No support for selective completions.

No support for counters for the DGRAM endpoint.

No support for inject.

# RUNTIME PARAMETERS

*FI_EFA_TX_SIZE*
: Maximum number of transmit operations before the provider returns -FI_EAGAIN.
  For only the RDM endpoint, this parameter will cause transmit operations to
  be queued when this value is set higher than the default and the transmit queue
  is full.

*FI_EFA_RX_SIZE*
: Maximum number of receive operations before the provider returns -FI_EAGAIN.

*FI_EFA_TX_IOV_LIMIT*
: Maximum number of IOVs for a transmit operation.

*FI_EFA_RX_IOV_LIMIT*
: Maximum number of IOVs for a receive operation.

# RUNTIME PARAMETERS SPECIFIC TO RDM ENDPOINT

These OFI runtime parameters apply only to the RDM endpoint.

*FI_EFA_RX_WINDOW_SIZE*
: Maximum number of MTU-sized messages that can be in flight from any
  single endpoint as part of long message data transfer.

*FI_EFA_TX_QUEUE_SIZE*
: Depth of transmit queue opened with the NIC. This may not be set to a value
  greater than what the NIC supports.

*FI_EFA_RECVWIN_SIZE*
: Size of out of order reorder buffer (in messages).  Messages
  received out of this window will result in an error.

*FI_EFA_CQ_SIZE*
: Size of any cq created, in number of entries.

*FI_EFA_MR_CACHE_ENABLE*
: Enables using the mr cache and in-line registration instead of a bounce
  buffer for iov's larger than max_memcpy_size. Defaults to true. When
  disabled, only uses a bounce buffer

*FI_EFA_MR_MAX_CACHED_COUNT*
: Sets the maximum number of memory registrations that can be cached at
  any time.

*FI_EFA_MR_MAX_CACHED_SIZE*
: Sets the maximum amount of memory that cached memory registrations can
  hold onto at any time.

*FI_EFA_MAX_MEMCPY_SIZE*
: Threshold size switch between using memory copy into a pre-registered
  bounce buffer and memory registration on the user buffer.

*FI_EFA_MTU_SIZE*
: Overrides the default MTU size of the device.

*FI_EFA_RX_COPY_UNEXP*
: Enables the use of a separate pool of bounce-buffers to copy unexpected
  messages out of the pre-posted receive buffers.

*FI_EFA_RX_COPY_OOO*
: Enables the use of a separate pool of bounce-buffers to copy out-of-order RTS
  packets out of the pre-posted receive buffers.

*FI_EFA_MAX_TIMEOUT*
: Maximum timeout (us) for backoff to a peer after a receiver not ready error.

*FI_EFA_TIMEOUT_INTERVAL*
: Time interval (us) for the base timeout to use for exponential backoff
  to a peer after a receiver not ready error.

*FI_EFA_ENABLE_SHM_TRANSFER*
: Enable SHM provider to provide the communication across all intra-node processes.
  SHM transfer will be disabled in the case where
  [`ptrace protection`](https://wiki.ubuntu.com/SecurityTeam/Roadmap/KernelHardening#ptrace_Protection)
  is turned on. You can turn it off to enable shm transfer.

*FI_EFA_SHM_AV_SIZE*
: Defines the maximum number of entries in SHM provider's address vector.

*FI_EFA_SHM_MAX_MEDIUM_SIZE*
: Defines the switch point between small/medium message and large message. The message
  larger than this switch point will be transferred with large message protocol.

*FI_EFA_INTER_MAX_MEDIUM_MESSAGE_SIZE*
: The maximum size for inter EFA messages to be sent by using medium message protocol. Messages which can fit in one packet will be sent as eager message. Messages whose sizes are smaller than this value will be sent using medium message protocol. Other messages will be sent using CTS based long message protocol.

*FI_EFA_FORK_SAFE*
: Enable fork() support. This may have a small performance impact and should only be set when required. Applications that require to register regions backed by huge pages and also require fork support are not supported.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
