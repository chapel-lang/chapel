---
layout: page
title: fi_netdir(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_netdir \- The Network Direct Fabric Provider

# OVERVIEW

The Network Direct provider enables applications using OFI to be run over
any verbs hardware (Infiniband, iWarp and etc). It uses the Microsoft Network
Direct SPI for network transport and provides a translation of OFI calls to
appropriate Network Direct API calls.
The Network Direct providers allows to OFI-based applications utilize
zero-copy data transfers between applications, kernel-bypass I/O generation and
one-sided data transfer operations on Microsoft Windows OS.
An application is able to use OFI with Network Direct provider enabled on
Windows OS to expose the capabilities of the networking devices if the hardware
vendors of the devices implemented the Network Direct service provider interface
(SPI) for their hardware.

# SUPPORTED FEATURES

The Network Direct provider support the following features defined for the
libfabric API:

*Endpoint types*
: The provider support the FI_EP_MSG endpoint types.

*Memory registration modes*
: The provider implements the *FI_MR_BASIC* memory registration mode.

*Data transfer operations*
: The following data transfer interfaces are supported for the following
  endpoint types: *FI_MSG*, *FI_RMA*.  See DATA TRANSFER OPERATIONS below
  for more details.

*Modes*
: The Network Direct provider requires applications to support
  the following modes:
  * FI_LOCAL_MR for all applications.

*Addressing Formats*
: Supported addressing formats include FI_SOCKADDR, FI_SOCKADDR_IN, FI_SOCKADDR_IN6

*Progress*
: The Network Direct provider supports FI_PROGRESS_AUTO: Asynchronous operations
  make forward progress automatically.

*Operation flags*
: The provider supports FI_INJECT, FI_COMPLETION, FI_TRANSMIT_COMPLETE,
  FI_INJECT_COMPLETE, FI_DELIVERY_COMPLETE, FI_SELECTIVE_COMPLETION

*Completion ordering*
: RX/TX contexts: FI_ORDER_STRICT

*Other supported features*
: Multiple input/output vector (IOV) is supported for FI_RMA read/write and
  FI_MSG receive/transmit operations.

# LIMITATIONS

*Memory Regions*
: Only FI_MR_BASIC mode is supported. Adding regions via s/g list is
  supported only up to a s/g list size of 1. No support for binding memory
  regions to a counter.

*Wait objects*
: Wait object and wait sets are not supported.

*Resource Management*
: Application has to make sure CQs are not overrun as this cannot be detected
  by the provider.

*Unsupported Endpoint types*
: FI_EP_DGRAM, FI_EP_RDM

*Other unsupported features*
: Scalable endpoints, FABRIC_DIRECT

*Unsupported features specific to MSG endpoints*
: FI_SOURCE, FI_TAGGED, FI_CLAIM, fi_ep_alias, shared TX context, operations.

# RUNTIME PARAMETERS

The Network Direct provider checks for the following environment variables.

### Variables specific to RDM endpoints

*FI_NETDIR_INLINETHR*
: The size of the (default: 8 Kbyte):
  * Transmitted data that can be inlined
  * Preposted data for the unexpected receive queue

*FI_NETDIR_PREPOSTCNT*
: The number of pre-registered buffers between the endpoints that are not require
  internal ACK messages, must be a power of 2 (default: 8).

*FI_NETDIR_PREPOSTBUFCNT*
: The number of preposted arrays of buffers, must be a power of 2 (default: 1).

### Environment variables notes
The fi_info utility would give the up-to-date information on environment variables:
fi_info -p netdir -e

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_open_ops`(3)](fi_open_ops.3.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
[`fi_atomic`(3)](fi_atomic.3.html)
