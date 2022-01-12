---
layout: page
title: fi_rxd(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_rxd \- The RxD (RDM over DGRAM) Utility Provider

# OVERVIEW

The RxD provider is a utility provider that supports RDM endpoints
emulated over a base DGRAM provider.

# SUPPORTED FEATURES

The RxD provider currently supports *FI_MSG* capabilities.

*Endpoint types*
: The provider supports only endpoint type *FI_EP_RDM*.

*Endpoint capabilities* : The following data transfer interface is
supported: *fi_msg*.

*Modes*
: The provider does not require the use of any mode bits but supports
  core DGRAM providers that require FI_CONTEXT and FI_MSG_PREFIX.

*Progress*
: The RxD provider only supports *FI_PROGRESS_MANUAL*.

# LIMITATIONS

The RxD provider has hard-coded maximums for supported queue sizes and
data transfers. Some of these limits are set based on the selected
base DGRAM provider.

No support for multi-recv.

No support for counters.

The RxD provider is still under development and is not extensively
tested.

# RUNTIME PARAMETERS

The *rxd* provider checks for the following environment variables:

*FI_OFI_RXD_SPIN_COUNT*
: Number of times to read the core provider's CQ for a segment completion
  before trying to progress sends. Default is 1000.

*FI_OFI_RXD_RETRY*
: Toggles retrying of packets and assumes reliability of individual packets
  and will reassemble all received packets. Retrying is turned on by default.

*FI_OFI_RXD_MAX_PEERS*
: Maximum number of peers the provider should prepare to track. Default: 1024

*FI_OFI_RXD_MAX_UNACKED*
: Maximum number of packets (per peer) to send at a time. Default: 128

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
