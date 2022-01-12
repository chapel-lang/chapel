---
layout: page
title: fi_rxm(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_rxm \- The RxM (RDM over MSG) Utility Provider

# OVERVIEW

The RxM provider (ofi_rxm) is an utility provider that supports FI_EP_RDM type
endpoint emulated over FI_EP_MSG type endpoint(s) of an underlying core provider.
FI_EP_RDM endpoints have a reliable datagram interface and RxM emulates this by
hiding the connection management of underlying FI_EP_MSG endpoints from the user.
Additionally, RxM can hide memory registration requirement from a core provider
like verbs if the apps don't support it.

# REQUIREMENTS

## Requirements for core provider

RxM provider requires the core provider to support the following features:

  * MSG endpoints (FI_EP_MSG)

  * RMA read/write (FI_RMA) - Used for implementing rendezvous protocol for
    large messages.

  * FI_OPT_CM_DATA_SIZE of at least 24 bytes.

## Requirements for applications

Since RxM emulates RDM endpoints by hiding connection management and connections
are established only on-demand (when app tries to send data), the first several
data transfer calls would return EAGAIN. Applications should be aware of this and
retry until the operation succeeds.

If an application has chosen manual progress for data progress, it should also
read the CQ so that the connection establishment progresses. Not doing so would
result in a stall. See also the ERRORS section in fi_msg(3).

# SUPPORTED FEATURES

The RxM provider currently supports *FI_MSG*, *FI_TAGGED*, *FI_RMA* and *FI_ATOMIC* capabilities.

*Endpoint types*
: The provider supports only *FI_EP_RDM*.

*Endpoint capabilities*
: The following data transfer interface is supported: *FI_MSG*, *FI_TAGGED*, *FI_RMA*, *FI_ATOMIC*.

*Progress*
: The RxM provider supports both *FI_PROGRESS_MANUAL* and *FI_PROGRESS_AUTO*.
  Manual progress in general has better connection scale-up and lower CPU utilization
  since there's no separate auto-progress thread.

*Addressing Formats*
: FI_SOCKADDR, FI_SOCKADDR_IN

*Memory Region*
: FI_MR_VIRT_ADDR, FI_MR_ALLOCATED, FI_MR_PROV_KEY MR mode bits would be
  required from the app in case the core provider requires it.

# LIMITATIONS

When using RxM provider, some limitations from the underlying MSG provider could also show
up. Please refer to the corresponding MSG provider man pages to find about those limitations.

## Unsupported features

RxM provider does not support the following features:

  * op_flags: FI_FENCE.

  * Scalable endpoints

  * Shared contexts

  * FABRIC_DIRECT

  * FI_MR_SCALABLE

  * Authorization keys

  * Application error data buffers

  * Multicast

  * FI_SYNC_ERR

  * Reporting unknown source addr data as part of completions

  * Triggered operations

## Progress limitations

When sending large messages, an app doing an sread or waiting on the CQ file descriptor
may not get a completion when reading the CQ after being woken up from the wait.
The app has to do sread or wait on the file descriptor again. This is needed
because RxM uses a rendezvous protocol for large message sends. An app would get
woken up from waiting on CQ fd when rendezvous protocol request completes but it
would have to wait again to get an ACK from the receiver indicating completion of
large message transfer by remote RMA read.

## FI_ATOMIC limitations

The FI_ATOMIC capability will only be listed in the fi_info if the fi_info
hints parameter specifies FI_ATOMIC. If FI_ATOMIC is requested, message order
FI_ORDER_RAR, FI_ORDER_RAW, FI_ORDER_WAR, FI_ORDER_WAW, FI_ORDER_SAR, and
FI_ORDER_SAW can not be supported.

## Miscellaneous limitations
 * RxM protocol peers should have same endian-ness otherwise connections won't
   successfully complete. This enables better performance at run-time as byte
   order translations are avoided.

# RUNTIME PARAMETERS

The ofi_rxm provider checks for the following environment variables.

*FI_OFI_RXM_BUFFER_SIZE*
: Defines the transmit buffer size / inject size. Messages of size less than this
  would be transmitted via an eager protocol and those above would be transmitted
  via a rendezvous or SAR (Segmentation And Reassembly) protocol. Transmit data
  would be copied up to this size (default: ~16k).

*FI_OFI_RXM_COMP_PER_PROGRESS*
: Defines the maximum number of MSG provider CQ entries (default: 1) that would
  be read per progress (RxM CQ read).

*FI_OFI_RXM_ENABLE_DYN_RBUF*
: Enables support for dynamic receive buffering, if available by the message
  endpoint provider.  This feature allows direct placement of received
  message data into application buffers, bypassing RxM bounce buffers.
  This feature targets providers that provide internal network buffering,
  such as the tcp provider.  (default: false)

*FI_OFI_RXM_SAR_LIMIT*
: Set this environment variable to control the RxM SAR (Segmentation And Reassembly)
  protocol. Messages of size greater than this (default: 128 Kb) would be transmitted
  via rendezvous protocol.

*FI_OFI_RXM_USE_SRX*
: Set this to 1 to use shared receive context from MSG provider, or 0 to
  disable using shared receive context. Shared receive contexts reduce overall
  memory usage, but may increase in message latency.  If not set, verbs will
  not use shared receive contexts by default, but the tcp provider will.

*FI_OFI_RXM_TX_SIZE*
: Defines default TX context size (default: 1024)

*FI_OFI_RXM_RX_SIZE*
: Defines default RX context size (default: 1024)

*FI_OFI_RXM_MSG_TX_SIZE*
: Defines FI_EP_MSG TX size that would be requested (default: 128).

*FI_OFI_RXM_MSG_RX_SIZE*
: Defines FI_EP_MSG RX size that would be requested (default: 128).

*FI_UNIVERSE_SIZE*
: Defines the expected number of ranks / peers an endpoint would communicate
with (default: 256).

*FI_OFI_RXM_CM_PROGRESS_INTERVAL*
: Defines the duration of time in microseconds between calls to RxM CM progression
  functions when using manual progress. Higher values may provide less noise for
  calls to fi_cq read functions, but may increase connection setup time (default: 10000)

*FI_OFI_RXM_CQ_EQ_FAIRNESS*
: Defines the maximum number of message provider CQ entries that can be
  consecutively read across progress calls without checking to see if the
  CM progress interval has been reached (default: 128)

# Tuning

## Bandwidth

To optimize for bandwidth, ensure you use higher values than default for
FI_OFI_RXM_TX_SIZE, FI_OFI_RXM_RX_SIZE, FI_OFI_RXM_MSG_TX_SIZE, FI_OFI_RXM_MSG_RX_SIZE
subject to memory limits of the system and the tx and rx sizes supported by the
MSG provider.

FI_OFI_RXM_SAR_LIMIT is another knob that can be experimented with to optimze for
bandwidth.

## Memory

To conserve memory, ensure FI_UNIVERSE_SIZE set to what is required. Similarly
check that FI_OFI_RXM_TX_SIZE, FI_OFI_RXM_RX_SIZE, FI_OFI_RXM_MSG_TX_SIZE and
FI_OFI_RXM_MSG_RX_SIZE env variables are set to only required values.

# NOTES

The data transfer API may return -FI_EAGAIN during on-demand connection setup
of the core provider FI_MSG_EP. See [`fi_msg`(3)](fi_msg.3.html) for a detailed
description of handling FI_EAGAIN.

# Troubleshooting / Known issues

If an RxM endpoint is expected to communicate with more peers than the default
value of FI_UNIVERSE_SIZE (256) CQ overruns can happen. To avoid this set a
higher value for FI_UNIVERSE_SIZE. CQ overrun can make a MSG endpoint unusable.

At higher # of ranks, there may be connection errors due to a node running out
of memory. The workaround is to use shared receive contexts for the MSG provider
(FI_OFI_RXM_USE_SRX=1) or reduce eager message size (FI_OFI_RXM_BUFFER_SIZE) and
MSG provider TX/RX queue sizes (FI_OFI_RXM_MSG_TX_SIZE / FI_OFI_RXM_MSG_RX_SIZE).

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
