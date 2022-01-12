---
layout: page
title: fi_mrail(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_mrail \- The Multi-Rail Utility Provider

# OVERVIEW

The mrail provider (ofi_mrail) is an utility provider that layers over an underlying
provider to enable the use of multiple network ports (rails). This increases
the total available bandwidth of an underlying provider. The current status of
mrail provider is experimental - not all libfabric features are supported and
performance is not guaranteed.

# REQUIREMENTS

## Requirements for underlying provider

mrail provider requires the underlying provider to support the following
capabilities / modes:

  * Buffered receive (FI_BUFFERED_RECV)

  * FI_SOURCE

  * FI_AV_TABLE

## Requirements for applications

Applications need to:
  * Support FI_MR_RAW MR mode bit to make use of FI_RMA capability.
  * Set FI_OFI_MRAIL_ADDR env variable (see RUNTIME PARAMETERS section below).

# SUPPORTED FEATURES

*Endpoint types*
: The provider supports only *FI_EP_RDM*.

*Endpoint capabilities*
: The following data transfer interface is supported: *FI_MSG*, *FI_TAGGED*, *FI_RMA*.

# LIMITATIONS

: Limitations of the underlying provider may show up as that of mrail provider.
: mrail provider doesn't allow pass-through of any mode bits to the underlying
  provider.

## Unsupported features

The following are the major libfabric features that are not supported. Any other
feature not listed in "Supported features" can be assumed as unsupported.

  * FI_ATOMIC

  * Scalable endpoints

  * Shared contexts

  * FABRIC_DIRECT

  * Multicast

  * Triggered operations

# FUNCTIONALITY OVERVIEW

For messages (FI_MSG, FI_TAGGED), the provider uses different policies to send messages
over one or more rails based on message size (See *FI_OFI_MRIAL_CONFIG* in the RUNTIME
PARAMETERS section). Ordering is guaranteed through the use of sequence numbers.

For RMA, the data is striped equally across all rails.

# RUNTIME PARAMETERS

The ofi_mrail provider checks for the following environment variables.

*FI_OFI_MRAIL_ADDR*
: Comma delimited list of individual rail addresses. Each address can be an address in
  FI_ADDR_STR format, a host name, an IP address, or a netdev interface name.

*FI_OFI_MRAIL_ADDR_STRC*
: Deprecated. Replaced by *FI_OFI_MRAIL_ADDR*.

*FI_OFI_MRAIL_CONFIG*
: Comma separated list of `<max_size>:<policy>` pairs, sorted in ascending order of
 `<max_size>`. Each pair indicated the rail sharing policy to be used for messages
  up to the size `<max_size>` and not covered by all previous pairs. The value of
  `<policy>` can be *fixed* (a fixed rail is used), *round-robin* (one rail per
  message, selected in round-robin fashion), or *striping* (striping across all the
  rails). The default configuration is `16384:fixed,ULONG_MAX:striping`. The value
  ULONG_MAX can be input as -1.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
