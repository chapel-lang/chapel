---
layout: page
title: fi_tcp(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_tcp \- The msg sockets Fabric Provider

# OVERVIEW

The tcp provider can be used on any system that supports TCP sockets. The
provider is not intended to provide performance improvements over regular
TCP sockets, but rather to allow developers to write, test,and debug
application code even on platforms that do not have high-performance
fabric hardware.

# SUPPORTED FEATURES

The following features are supported

*Endpoint types*
: *FI_EP_MSG* is the only supported endpoint type. Reliable
  datagram endpoint over TCP sockets can be achieved by layering RxM over
  tcp provider.

: *FI_EP_RDM* is supported by layering ofi_rxm provider on top of the
  tcp provider.

*Endpoint capabilities*
: The tcp provider currently supports *FI_MSG*, *FI_RMA*

*Progress*
: Currently tcp provider supports only *FI_PROGRESS_MANUAL*

*Shared Rx Context*
: The tcp provider supports shared receive context

*Multi recv buffers*
: The tcp provider supports multi recv buffers

# RUNTIME PARAMETERS

The tcp provider check for the following enviroment variables -

*FI_TCP_IFACE*
: A specific can be requested with this variable

*FI_TCP_PORT_LOW_RANGE/FI_TCP_PORT_HIGH_RANGE*
: These variables are used to set the range of ports to be used by the
  tcp provider for its passive endpoint creation. This is useful where
  only a range of ports are allowed by firewall for tcp connections.

*FI_TCP_TX_SIZE*
: Default tx context size (default: 256)

*FI_TCP_RX_SIZE*
: Default rx context size (default: 256)

# LIMITATIONS

The tcp provider is implemented over TCP sockets to emulate libfabric API.
Hence the performance may be lower than what an application might see
implementing to sockets directly, depending on the types of data transfers
the application is trying to achieve.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
