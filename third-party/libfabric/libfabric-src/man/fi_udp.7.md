---
layout: page
title: fi_udp(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_udp \- The UDP Fabric Provider

# OVERVIEW

The UDP provider is a basic provider that can be used on any
system that supports UDP sockets.  The provider is not intended to provide
performance improvements over regular TCP sockets, but rather to allow
application and provider developers to write, test, and debug their code.
The UDP provider forms the foundation of a utility provider that enables
the implementation of libfabric features over any hardware.

# SUPPORTED FEATURES

The UDP provider supports a minimal set of features useful for sending and
receiving datagram messages over an unreliable endpoint.

*Endpoint types*
: The provider supports only endpoint type *FI_EP_DGRAM*.

*Endpoint capabilities*
: The following data transfer interface is supported: *fi_msg*.  The
  provider supports standard unicast datagram transfers, as well as
  multicast operations.

*Modes*
: The provider does not require the use of any mode bits.

*Progress*
: The UDP provider supports both *FI_PROGRESS_AUTO* and *FI_PROGRESS_MANUAL*,
  with a default set to auto.  However, receive side data buffers are not
  modified outside of completion processing routines.

# LIMITATIONS

The UDP provider has hard-coded maximums for supported queue sizes and data
transfers.  These values are reflected in the related fabric attribute
structures

EPs must be bound to both RX and TX CQs.

No support for selective completions or multi-recv.

No support for counters.

# RUNTIME PARAMETERS

No runtime parameters are currently defined.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
