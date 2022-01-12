---
layout: page
title: fi_sockets(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_sockets \- The Sockets Fabric Provider

# OVERVIEW

The sockets provider is being deprecated in favor of the tcp, udp, and
utility providers.  Further work on the sockets provider will be minimal.
Most applications should instead use the tcp provider instead.

The sockets provider is a general purpose provider that can be used on any
system that supports TCP sockets.  The provider is not intended to provide
performance improvements over regular TCP sockets, but rather to allow
developers to write, test, and debug application code even on platforms
that do not have high-performance fabric hardware.  The sockets provider
supports all libfabric provider requirements and interfaces.

# SUPPORTED FEATURES

The sockets provider supports all the features defined for the libfabric API. 
Key features include:

*Endpoint types*
: The provider supports all endpoint types: *FI_EP_MSG*, *FI_EP_RDM*,
  and *FI_EP_DGRAM*.

*Endpoint capabilities*
: The following data transfer interface is supported for a all endpoint
  types: *fi_msg*.  Additionally, these interfaces are supported
  for reliable endpoints (*FI_EP_MSG* and *FI_EP_RDM*): *fi_tagged*,
  *fi_atomic*, and *fi_rma*.

*Modes*
: The sockets provider supports all operational modes including
  *FI_CONTEXT* and *FI_MSG_PREFIX*.

*Progress*
: Sockets provider supports both *FI_PROGRESS_AUTO* and *FI_PROGRESS_MANUAL*,
  with a default set to auto.  When progress is set to auto, a background
  thread runs to ensure that progress is made for asynchronous requests.

# LIMITATIONS

Sockets provider attempts to emulate the entire API set, including all
defined options. In order to support development on a wide range of
systems, it is implemented over TCP sockets. As a result, the
performance numbers are lower compared to other providers implemented
over high-speed fabric, and lower than what an application might see
implementing to sockets directly.

Does not support FI_ADDR_STR address format.

# RUNTIME PARAMETERS

The sockets provider checks for the following environment variables -

*FI_SOCKETS_PE_WAITTIME*
: An integer value that specifies how many milliseconds to spin while waiting for progress in *FI_PROGRESS_AUTO* mode.

*FI_SOCKETS_CONN_TIMEOUT*
: An integer value that specifies how many milliseconds to wait for one connection establishment.

*FI_SOCKETS_MAX_CONN_RETRY*
: An integer value that specifies the number of socket connection retries before reporting as failure.

*FI_SOCKETS_DEF_CONN_MAP_SZ*
: An integer to specify the default connection map size. 

*FI_SOCKETS_DEF_AV_SZ*
: An integer to specify the default address vector size.

*FI_SOCKETS_DEF_CQ_SZ*
: An integer to specify the default completion queue size.

*FI_SOCKETS_DEF_EQ_SZ*
: An integer to specify the default event queue size.

*FI_SOCKETS_DGRAM_DROP_RATE*
: An integer value to specify the drop rate of dgram frame when endpoint is *FI_EP_DGRAM*. This is for debugging purpose only.

*FI_SOCKETS_PE_AFFINITY*
: If specified, progress thread is bound to the indicated range(s) of Linux virtual processor ID(s). This option is currently not supported on OS X. The usage is - id_start[-id_end[:stride]][,].

*FI_SOCKETS_KEEPALIVE_ENABLE*
: A boolean to enable the keepalive support.

*FI_SOCKETS_KEEPALIVE_TIME*
: An integer to specify the idle time in seconds before sending the first keepalive probe. Only relevant if *FI_SOCKETS_KEEPALIVE_ENABLE* is enabled.

*FI_SOCKETS_KEEPALIVE_INTVL*
: An integer to specify the time in seconds between individual keepalive probes. Only relevant if *FI_SOCKETS_KEEPALIVE_ENABLE* is enabled.

*FI_SOCKETS_KEEPALIVE_PROBES*
: An integer to specify the maximum number of keepalive probes sent before dropping the connection. Only relevant if *FI_SOCKETS_KEEPALIVE_ENABLE* is enabled.

*FI_SOCKETS_IFACE*
: The prefix or the name of the network interface (default: any)

# LARGE SCALE JOBS

For large scale runs one can use these environment variables to set the default parameters e.g. size of the address vector(AV), completion queue (CQ), connection map etc. that satisfies the requirement of the particular benchmark. The recommended parameters for large scale runs are *FI_SOCKETS_MAX_CONN_RETRY*, *FI_SOCKETS_DEF_CONN_MAP_SZ*, *FI_SOCKETS_DEF_AV_SZ*, *FI_SOCKETS_DEF_CQ_SZ*, *FI_SOCKETS_DEF_EQ_SZ*.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
