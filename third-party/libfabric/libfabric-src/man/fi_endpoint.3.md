---
layout: page
title: fi_endpoint(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_endpoint \- Fabric endpoint operations

fi_endpoint / fi_scalable_ep / fi_passive_ep / fi_close
:   Allocate or close an endpoint.

fi_ep_bind
:   Associate an endpoint with hardware resources, such as event queues,
    completion queues, counters, address vectors, or shared transmit/receive
    contexts.

fi_scalable_ep_bind
:   Associate a scalable endpoint with an address vector

fi_pep_bind
:   Associate a passive endpoint with an event queue

fi_enable
:   Transitions an active endpoint into an enabled state.

fi_cancel
:   Cancel a pending asynchronous data transfer

fi_ep_alias
:   Create an alias to the endpoint

fi_control
:   Control endpoint operation.

fi_getopt / fi_setopt
:   Get or set endpoint options.

fi_rx_context / fi_tx_context / fi_srx_context  / fi_stx_context
:   Open a transmit or receive context.

fi_tc_dscp_set / fi_tc_dscp_get
:   Convert between a DSCP value and a network traffic class

fi_rx_size_left / fi_tx_size_left (DEPRECATED)
:   Query the lower bound on how many RX/TX operations may be posted without
    an operation returning -FI_EAGAIN.  This functions have been deprecated
    and will be removed in a future version of the library.

# SYNOPSIS

```c
#include <rdma/fabric.h>

#include <rdma/fi_endpoint.h>

int fi_endpoint(struct fid_domain *domain, struct fi_info *info,
    struct fid_ep **ep, void *context);

int fi_scalable_ep(struct fid_domain *domain, struct fi_info *info,
    struct fid_ep **sep, void *context);

int fi_passive_ep(struct fi_fabric *fabric, struct fi_info *info,
    struct fid_pep **pep, void *context);

int fi_tx_context(struct fid_ep *sep, int index,
    struct fi_tx_attr *attr, struct fid_ep **tx_ep,
    void *context);

int fi_rx_context(struct fid_ep *sep, int index,
    struct fi_rx_attr *attr, struct fid_ep **rx_ep,
    void *context);

int fi_stx_context(struct fid_domain *domain,
    struct fi_tx_attr *attr, struct fid_stx **stx,
    void *context);

int fi_srx_context(struct fid_domain *domain,
    struct fi_rx_attr *attr, struct fid_ep **rx_ep,
    void *context);

int fi_close(struct fid *ep);

int fi_ep_bind(struct fid_ep *ep, struct fid *fid, uint64_t flags);

int fi_scalable_ep_bind(struct fid_ep *sep, struct fid *fid, uint64_t flags);

int fi_pep_bind(struct fid_pep *pep, struct fid *fid, uint64_t flags);

int fi_enable(struct fid_ep *ep);

int fi_cancel(struct fid_ep *ep, void *context);

int fi_ep_alias(struct fid_ep *ep, struct fid_ep **alias_ep, uint64_t flags);

int fi_control(struct fid *ep, int command, void *arg);

int fi_getopt(struct fid *ep, int level, int optname,
    void *optval, size_t *optlen);

int fi_setopt(struct fid *ep, int level, int optname,
    const void *optval, size_t optlen);

uint32_t fi_tc_dscp_set(uint8_t dscp);

uint8_t fi_tc_dscp_get(uint32_t tclass);

DEPRECATED ssize_t fi_rx_size_left(struct fid_ep *ep);

DEPRECATED ssize_t fi_tx_size_left(struct fid_ep *ep);
```

# ARGUMENTS

*fid*
: On creation, specifies a fabric or access domain.  On bind,
  identifies the event queue, completion queue, counter, or address vector to
  bind to the endpoint. In other cases, it's a fabric identifier of an
  associated resource.

*info*
: Details about the fabric interface endpoint to be opened, obtained
  from fi_getinfo.

*ep*
: A fabric endpoint.

*sep*
: A scalable fabric endpoint.

*pep*
: A passive fabric endpoint.

*context*
: Context associated with the endpoint or asynchronous operation.

*index*
: Index to retrieve a specific transmit/receive context.

*attr*
: Transmit or receive context attributes.

*flags*
: Additional flags to apply to the operation.

*command*
: Command of control operation to perform on endpoint.

*arg*
: Optional control argument.

*level*
: Protocol level at which the desired option resides.

*optname*
: The protocol option to read or set.

*optval*
: The option value that was read or to set.

*optlen*
: The size of the optval buffer.

# DESCRIPTION

Endpoints are transport level communication portals.  There are two
types of endpoints: active and passive.  Passive endpoints belong to a
fabric domain and are most often used to listen for incoming connection
requests.  However, a passive endpoint may be used to reserve a fabric address
that can be granted to an active endpoint.  Active endpoints belong to access
domains and can perform data transfers.

Active endpoints may be connection-oriented or connectionless, and may
provide data reliability.  The data transfer interfaces -- messages (fi_msg),
tagged messages (fi_tagged), RMA (fi_rma), and atomics (fi_atomic) --
are associated with active endpoints.  In basic configurations, an
active endpoint has transmit and receive queues.  In general, operations
that generate traffic on the fabric are posted to the transmit queue.
This includes all RMA and atomic operations, along with sent messages and
sent tagged messages.  Operations that post buffers for receiving incoming
data are submitted to the receive queue.

Active endpoints are created in the disabled state.  They must
transition into an enabled state before accepting data transfer
operations, including posting of receive buffers.  The fi_enable call
is used to transition an active endpoint into an enabled state.  The
fi_connect and fi_accept calls will also transition an endpoint into
the enabled state, if it is not already active.

In order to transition an endpoint into an enabled state, it must be
bound to one or more fabric resources.  An endpoint that will generate
asynchronous completions, either through data transfer operations or
communication establishment events, must be bound to the appropriate
completion queues or event queues, respectively, before being enabled.
Additionally, endpoints that use manual progress must be associated
with relevant completion queues or event queues in order to drive
progress.  For endpoints that are only used as the target of RMA or
atomic operations, this means binding the endpoint to a completion
queue associated with receive processing.  Connectionless endpoints must
be bound to an address vector.

Once an endpoint has been activated, it may be associated with an address
vector.  Receive buffers may be posted to it and
calls may be made to connection establishment routines.
Connectionless endpoints may also perform data transfers.

The behavior of an endpoint may be adjusted by setting its control
data and protocol options.  This allows the underlying provider to
redirect function calls to implementations optimized to meet the
desired application behavior.

If an endpoint experiences a critical error, it will transition back
into a disabled state.  Critical errors are reported through the
event queue associated with the EP.  In certain cases, a disabled endpoint may
be re-enabled.  The ability to transition back into an enabled
state is provider specific and depends on the type of error that
the endpoint experienced.  When an endpoint is disabled as a result
of a critical error, all pending operations are discarded.

## fi_endpoint / fi_passive_ep / fi_scalable_ep

fi_endpoint allocates a new active endpoint.  fi_passive_ep allocates a
new passive endpoint.  fi_scalable_ep allocates a scalable endpoint.
The properties and behavior of the endpoint are defined based on the
provided struct fi_info.  See fi_getinfo for additional details on
fi_info.  fi_info flags that control the operation of an endpoint are
defined below. See section SCALABLE ENDPOINTS.

If an active endpoint is allocated in order to accept a connection request,
the fi_info parameter must be the same as the fi_info structure provided with
the connection request (FI_CONNREQ) event.

An active endpoint may acquire the properties of a passive endpoint by setting
the fi_info handle field to the passive endpoint fabric descriptor.  This is
useful for applications that need to reserve the fabric address of an
endpoint prior to knowing if the endpoint will be used on the active or passive
side of a connection.  For example, this feature is useful for simulating
socket semantics.  Once an active endpoint acquires the properties of a passive
endpoint, the passive endpoint is no longer bound to any fabric resources and
must no longer be used. The user is expected to close the passive endpoint
after opening the active endpoint in order to free up any lingering resources
that had been used.

## fi_close

Closes an endpoint and release all resources associated with it.

When closing a scalable endpoint, there must be no opened transmit contexts, or
receive contexts associated with the scalable endpoint.  If resources are still
associated with the scalable endpoint when attempting to close, the call will
return -FI_EBUSY.

Outstanding operations posted to the endpoint when fi_close is
called will be discarded.  Discarded operations will silently be dropped,
with no completions reported.  Additionally, a provider may discard previously
completed operations from the associated completion queue(s).  The
behavior to discard completed operations is provider specific.

## fi_ep_bind

fi_ep_bind is used to associate an endpoint with other allocated
resources, such as completion queues, counters, address vectors,
event queues, shared contexts, and memory regions.  The type of objects that
must be bound with an endpoint depend on the endpoint type and its
configuration.

Passive endpoints must be bound with an EQ that supports connection
management events.  Connectionless endpoints must be bound to a
single address vector.  If an endpoint is using a shared transmit
and/or receive context, the shared contexts must be bound to the endpoint.
CQs, counters, AV, and shared contexts must be bound to endpoints
before they are enabled either explicitly or implicitly.

An endpoint must be bound with CQs capable of reporting completions for any
asynchronous operation initiated on the endpoint.  For example, if the
endpoint supports any outbound transfers (sends, RMA, atomics, etc.), then
it must be bound to a completion queue that can report transmit completions.
This is true even if the endpoint is configured to suppress successful
completions, in order that operations that complete in error may be reported
to the user.

An active endpoint may direct asynchronous completions to different
CQs, based on the type of operation.  This is specified using
fi_ep_bind flags.  The following flags may be OR'ed together when
binding an endpoint to a completion domain CQ.

*FI_RECV*
: Directs the notification of inbound data transfers to the specified
  completion queue.  This includes received messages.  This binding
  automatically includes FI_REMOTE_WRITE, if applicable to the
  endpoint.

*FI_SELECTIVE_COMPLETION*
: By default, data transfer operations write CQ completion entries
  into the associated completion queue after they have successfully completed.
  Applications can use this bind flag to selectively enable when
  completions are generated.  If FI_SELECTIVE_COMPLETION is specified,
  data transfer operations will not generate CQ entries for _successful_
  completions unless FI_COMPLETION is set as an operational flag for the
  given operation.  Operations that fail asynchronously will still generate
  completions, even if a completion is not requested.  FI_SELECTIVE_COMPLETION
  must be OR'ed with FI_TRANSMIT and/or FI_RECV flags.

  When FI_SELECTIVE_COMPLETION is set, the user must determine when a
  request that does NOT have FI_COMPLETION set has completed indirectly,
  usually based on the completion of a subsequent operation or by using
  completion counters.  Use of this flag may improve performance by allowing
  the provider to avoid writing a CQ completion entry for every operation.

  See Notes section below for additional information on how this flag
  interacts with the FI_CONTEXT and FI_CONTEXT2 mode bits.

*FI_TRANSMIT*
: Directs the completion of outbound data transfer requests to the
  specified completion queue.  This includes send message, RMA, and
  atomic operations.

An endpoint may optionally be bound to a completion counter.  Associating
an endpoint with a counter is in addition to binding the EP with a CQ.  When
binding an endpoint to a counter, the following flags may be specified.

*FI_READ*
: Increments the specified counter whenever an RMA read, atomic fetch,
  or atomic compare operation initiated from the endpoint has completed
  successfully or in error.

*FI_RECV*
: Increments the specified counter whenever a message is
  received over the endpoint.  Received messages include both tagged
  and normal message operations.

*FI_REMOTE_READ*
: Increments the specified counter whenever an RMA read, atomic fetch, or
  atomic compare operation is initiated from a remote endpoint that
  targets the given endpoint.  Use of this flag requires that the
  endpoint be created using FI_RMA_EVENT.

*FI_REMOTE_WRITE*
: Increments the specified counter whenever an RMA write or base
  atomic operation is initiated from a remote endpoint that targets
  the given endpoint.  Use of this flag requires that the
  endpoint be created using FI_RMA_EVENT.

*FI_SEND*
: Increments the specified counter whenever a message transfer initiated
  over the endpoint has completed successfully or in error.  Sent messages
  include both tagged and normal message operations.

*FI_WRITE*
: Increments the specified counter whenever an RMA write or base atomic
  operation initiated from the endpoint has completed successfully or in error.

An endpoint may only be bound to a single CQ or counter for a given
type of operation.  For example, a EP may not bind to two counters
both using FI_WRITE.  Furthermore, providers may limit CQ and counter
bindings to endpoints of the same endpoint type (DGRAM, MSG, RDM, etc.).

## fi_scalable_ep_bind

fi_scalable_ep_bind is used to associate a scalable endpoint with an
address vector. See section on SCALABLE ENDPOINTS.  A scalable
endpoint has a single transport level address and can support multiple
transmit and receive contexts. The transmit and receive contexts share
the transport-level address. Address vectors that are bound to
scalable endpoints are implicitly bound to any transmit or receive
contexts created using the scalable endpoint.

## fi_enable

This call transitions the endpoint into an enabled state.  An endpoint
must be enabled before it may be used to perform data transfers.
Enabling an endpoint typically results in hardware resources being
assigned to it.  Endpoints making use of completion queues, counters,
event queues, and/or address vectors must be bound to them before being
enabled.

Calling connect or accept on an endpoint will implicitly enable an
endpoint if it has not already been enabled.

fi_enable may also be used to re-enable an endpoint that has been
disabled as a result of experiencing a critical error.  Applications
should check the return value from fi_enable to see if a disabled
endpoint has successfully be re-enabled.

## fi_cancel

fi_cancel attempts to cancel an outstanding asynchronous operation.
Canceling an operation causes the fabric provider to search for the
operation and, if it is still pending, complete it as having been
canceled. An error queue entry will be available in the
associated error queue with error code FI_ECANCELED. On the other hand,
if the operation completed before the call to fi_cancel, then the
completion status of that operation will be available in the associated
completion queue.  No specific entry related to fi_cancel itself will be posted.

Cancel uses the context parameter associated with an operation to identify
the request to cancel.  Operations posted without a valid context parameter --
either no context parameter is specified or the context value was ignored
by the provider -- cannot be canceled.  If multiple outstanding operations
match the context parameter, only one will be canceled. In this case, the
operation which is canceled is provider specific.
The cancel operation is asynchronous, but will complete within a bounded
period of time.

## fi_ep_alias

This call creates an alias to the specified endpoint.  Conceptually,
an endpoint alias provides an alternate software path from the
application to the underlying provider hardware.  An alias EP differs
from its parent endpoint only by its default data transfer flags.  For
example, an alias EP may be configured to use a different completion
mode.  By default, an alias EP inherits the same data transfer flags
as the parent endpoint.  An application can use fi_control to modify
the alias EP operational flags.

When allocating an alias, an application may configure either the transmit
or receive operational flags.  This avoids needing a separate call to
fi_control to set those flags.  The flags passed to fi_ep_alias must
include FI_TRANSMIT or FI_RECV (not both) with other operational flags OR'ed
in.  This will override the transmit or receive flags,
respectively, for operations posted through the alias endpoint.
All allocated aliases must be closed for the underlying endpoint to be
released.

## fi_control

The control operation is used to adjust the default behavior of an
endpoint.  It allows the underlying provider to redirect function
calls to implementations optimized to meet the desired application
behavior.  As a result, calls to fi_ep_control must be serialized
against all other calls to an endpoint.

The base operation of an endpoint is selected during creation using
struct fi_info.  The following control commands and arguments may be
assigned to an endpoint.

**FI_BACKLOG - int *value**
: This option only applies to passive endpoints.  It is used to set the
  connection request backlog for listening endpoints.

**FI_GETOPSFLAG -- uint64_t *flags**
: Used to retrieve the current value of flags associated with the data
  transfer operations initiated on the endpoint. The control argument must
  include FI_TRANSMIT or FI_RECV (not both) flags to indicate the type of
  data transfer flags to be returned.
  See below for a list of control flags.

**FI_GETWAIT -- void \*\***
: This command allows the user to retrieve the file descriptor associated
  with a socket endpoint.  The fi_control arg parameter should be an address
  where a pointer to the returned file descriptor will be written.  See fi_eq.3
  for addition details using fi_control with FI_GETWAIT.  The file descriptor
  may be used for notification that the endpoint is ready to send or receive
  data.

**FI_SETOPSFLAG -- uint64_t *flags**
: Used to change the data transfer operation flags associated with an
  endpoint. The control argument must include FI_TRANSMIT or FI_RECV (not both)
  to indicate the type of data transfer that the flags should apply to, with other
  flags OR'ed in. The given flags will override the previous transmit and receive
  attributes that were set when the endpoint was created.
  Valid control flags are defined below.

## fi_getopt / fi_setopt

Endpoint protocol operations may be retrieved using fi_getopt or set
using fi_setopt.  Applications specify the level that a desired option
exists, identify the option, and provide input/output buffers to get
or set the option.  fi_setopt provides an application a way to adjust
low-level protocol and implementation specific details of an endpoint.

The following option levels and option names and parameters are defined.

*FI_OPT_ENDPOINT*

- *FI_OPT_BUFFERED_LIMIT - size_t*
: Defines the maximum size of a buffered message that will be reported
  to users as part of a receive completion when the FI_BUFFERED_RECV mode
  is enabled on an endpoint.

  fi_getopt() will return the currently configured threshold, or the
  provider's default threshold if one has not be set by the application.
  fi_setopt() allows an application to configure the threshold.  If the
  provider cannot support the requested threshold, it will fail the
  fi_setopt() call with FI_EMSGSIZE.  Calling fi_setopt() with the
  threshold set to SIZE_MAX will set the threshold to the maximum
  supported by the provider.  fi_getopt() can then be used to retrieve
  the set size.

  In most cases, the sending and receiving endpoints must be
  configured to use the same threshold value, and the threshold must be
  set prior to enabling the endpoint.

- *FI_OPT_BUFFERED_MIN - size_t*
: Defines the minimum size of a buffered message that will be reported.
  Applications would set this to a size that's big enough to decide whether
  to discard or claim a buffered receive or when to claim a buffered receive
  on getting a buffered receive completion. The value is typically used by a
  provider when sending a rendezvous protocol request where it would send
  at least FI_OPT_BUFFERED_MIN bytes of application data along with it. A smaller
  sized rendezvous protocol message usually results in better latency for the
  overall transfer of a large message.

- *FI_OPT_CM_DATA_SIZE - size_t*
: Defines the size of available space in CM messages for user-defined
  data.  This value limits the amount of data that applications can exchange
  between peer endpoints using the fi_connect, fi_accept, and fi_reject
  operations.  The size returned is dependent upon the properties of the
  endpoint, except in the case of passive endpoints, in which the size reflects
  the maximum size of the data that may be present as part of a connection
  request event. This option is read only.

- *FI_OPT_MIN_MULTI_RECV - size_t*
: Defines the minimum receive buffer space available when the receive
  buffer is released by the provider (see FI_MULTI_RECV).  Modifying this
  value is only guaranteed to set the minimum buffer space needed on
  receives posted after the value has been changed.  It is recommended
  that applications that want to override the default MIN_MULTI_RECV
  value set this option before enabling the corresponding endpoint.

## fi_tc_dscp_set

This call converts a DSCP defined value into a libfabric traffic class value.
It should be used when assigning a DSCP value when setting the tclass field
in either domain or endpoint attributes

## fi_tc_dscp_get

This call returns the DSCP value associated with the tclass field for the
domain or endpoint attributes.

## fi_rx_size_left (DEPRECATED)

This function has been deprecated and will be removed in a future version
of the library.  It may not be supported by all providers.

The fi_rx_size_left call returns a lower bound on the number of receive
operations that may be posted to the given endpoint without that operation
returning -FI_EAGAIN.  Depending on the specific details of the subsequently
posted receive operations (e.g., number of iov entries, which receive function
is called, etc.), it may be possible to post more receive operations than
originally indicated by fi_rx_size_left.

## fi_tx_size_left (DEPRECATED)

This function has been deprecated and will be removed in a future version
of the library.  It may not be supported by all providers.

The fi_tx_size_left call returns a lower bound on the number of transmit
operations that may be posted to the given endpoint without that operation
returning -FI_EAGAIN.  Depending on the specific details of the subsequently
posted transmit operations (e.g., number of iov entries, which transmit
function is called, etc.), it may be possible to post more transmit operations
than originally indicated by fi_tx_size_left.

# ENDPOINT ATTRIBUTES

The fi_ep_attr structure defines the set of attributes associated with
an endpoint.  Endpoint attributes may be further refined using the transmit
and receive context attributes as shown below.

{% highlight c %}
struct fi_ep_attr {
	enum fi_ep_type type;
	uint32_t        protocol;
	uint32_t        protocol_version;
	size_t          max_msg_size;
	size_t          msg_prefix_size;
	size_t          max_order_raw_size;
	size_t          max_order_war_size;
	size_t          max_order_waw_size;
	uint64_t        mem_tag_format;
	size_t          tx_ctx_cnt;
	size_t          rx_ctx_cnt;
	size_t          auth_key_size;
	uint8_t         *auth_key;
};
{% endhighlight %}

## type - Endpoint Type

If specified, indicates the type of fabric interface communication
desired.  Supported types are:

*FI_EP_DGRAM*
: Supports a connectionless, unreliable datagram communication.
  Message boundaries are maintained, but the maximum message size may
  be limited to the fabric MTU.  Flow control is not guaranteed.

*FI_EP_MSG*
: Provides a reliable, connection-oriented data transfer service with
  flow control that maintains message boundaries.

*FI_EP_RDM*
: Reliable datagram message.  Provides a reliable, connectionless data
  transfer service with flow control that maintains message
  boundaries.

*FI_EP_SOCK_DGRAM*
: A connectionless, unreliable datagram endpoint with UDP socket-like
  semantics.  FI_EP_SOCK_DGRAM is most useful for applications designed
  around using UDP sockets.  See the SOCKET ENDPOINT section for additional
  details and restrictions that apply to datagram socket endpoints.

*FI_EP_SOCK_STREAM*
: Data streaming endpoint with TCP socket-like semantics.  Provides
  a reliable, connection-oriented data transfer service that does
  not maintain message boundaries.  FI_EP_SOCK_STREAM is most useful for
  applications designed around using TCP sockets.  See the SOCKET
  ENDPOINT section for additional details and restrictions that apply
  to stream endpoints.

*FI_EP_UNSPEC*
: The type of endpoint is not specified.  This is usually provided as
  input, with other attributes of the endpoint or the provider
  selecting the type.

## Protocol

Specifies the low-level end to end protocol employed by the provider.
A matching protocol must be used by communicating endpoints to ensure
interoperability.  The following protocol values are defined.
Provider specific protocols are also allowed.  Provider specific
protocols will be indicated by having the upper bit of the
protocol value set to one.

*FI_PROTO_GNI*
: Protocol runs over Cray GNI low-level interface.

*FI_PROTO_IB_RDM*
: Reliable-datagram protocol implemented over InfiniBand reliable-connected
  queue pairs.

*FI_PROTO_IB_UD*
: The protocol runs over Infiniband unreliable datagram queue pairs.

*FI_PROTO_IWARP*
: The protocol runs over the Internet wide area RDMA protocol transport.

*FI_PROTO_IWARP_RDM*
: Reliable-datagram protocol implemented over iWarp reliable-connected
  queue pairs.

*FI_PROTO_NETWORKDIRECT*
: Protocol runs over Microsoft NetworkDirect service provider interface.
  This adds reliable-datagram semantics over the NetworkDirect connection-
  oriented endpoint semantics.

*FI_PROTO_PSMX*
: The protocol is based on an Intel proprietary protocol known as PSM,
  performance scaled messaging.  PSMX is an extended version of the
  PSM protocol to support the libfabric interfaces.

*FI_PROTO_PSMX2*
: The protocol is based on an Intel proprietary protocol known as PSM2,
  performance scaled messaging version 2.  PSMX2 is an extended version of the
  PSM2 protocol to support the libfabric interfaces.

*FI_PROTO_PSMX3*
: The protocol is Intel's protocol known as PSM3, performance scaled
  messaging version 3.  PSMX3 is implemented over RoCEv2 and verbs.

*FI_PROTO_RDMA_CM_IB_RC*
: The protocol runs over Infiniband reliable-connected queue pairs,
  using the RDMA CM protocol for connection establishment.

*FI_PROTO_RXD*
: Reliable-datagram protocol implemented over datagram endpoints.  RXD is
  a libfabric utility component that adds RDM endpoint semantics over
  DGRAM endpoint semantics.

*FI_PROTO_RXM*
: Reliable-datagram protocol implemented over message endpoints.  RXM is
  a libfabric utility component that adds RDM endpoint semantics over
  MSG endpoint semantics.

*FI_PROTO_SOCK_TCP*
: The protocol is layered over TCP packets.

*FI_PROTO_UDP*
: The protocol sends and receives UDP datagrams.  For example, an
  endpoint using *FI_PROTO_UDP* will be able to communicate with a
  remote peer that is using Berkeley *SOCK_DGRAM* sockets using
  *IPPROTO_UDP*.

*FI_PROTO_UNSPEC*
: The protocol is not specified.  This is usually provided as input,
  with other attributes of the socket or the provider selecting the
  actual protocol.

## protocol_version - Protocol Version

Identifies which version of the protocol is employed by the provider.
The protocol version allows providers to extend an existing protocol,
by adding support for additional features or functionality for example,
in a backward compatible manner.  Providers that support different versions
of the same protocol should inter-operate, but only when using the
capabilities defined for the lesser version.

## max_msg_size - Max Message Size

Defines the maximum size for an application data transfer as a single
operation.

## msg_prefix_size - Message Prefix Size

Specifies the size of any required message prefix buffer space.  This
field will be 0 unless the FI_MSG_PREFIX mode is enabled.  If
msg_prefix_size is > 0 the specified value will be a multiple of
8-bytes.

## Max RMA Ordered Size

The maximum ordered size specifies the delivery order of transport
data into target memory for RMA and atomic operations.  Data ordering
is separate, but dependent on message ordering (defined below).  Data
ordering is unspecified where message order is not defined.

Data ordering refers to the access of the same target memory by subsequent
operations.  When back to back RMA read or write operations access the
same registered memory location, data ordering indicates whether the
second operation reads or writes the target memory after the first
operation has completed.  For example, will an RMA read that follows
an RMA write read back the data that was written?  Similarly, will an
RMA write that follows an RMA read update the target buffer after the
read has transferred the original data?  Data ordering answers these
questions, even in the presence of errors, such as the need to resend
data because of lost or corrupted network traffic.

RMA ordering applies between two operations, and not within a single
data transfer.  Therefore, ordering is defined
per byte-addressable memory location.  I.e. ordering specifies
whether location X is accessed by the second operation after the first
operation.  Nothing is implied about the completion of the first
operation before the second operation is initiated.  For example, if
the first operation updates locations X and Y, but the second operation
only accesses location X, there are no guarantees defined relative to
location Y and the second operation.

In order to support large data transfers being broken into multiple packets
and sent using multiple paths through the fabric, data ordering may be
limited to transfers of a specific size or less.  Providers specify when
data ordering is maintained through the following values.  Note that even
if data ordering is not maintained, message ordering may be.

*max_order_raw_size*
: Read after write size.  If set, an RMA or atomic read operation
  issued after an RMA or atomic write operation, both of which are
  smaller than the size, will be ordered. Where the target memory
  locations overlap, the RMA or atomic read operation will see the
  results of the previous RMA or atomic write.

*max_order_war_size*
: Write after read size.  If set, an RMA or atomic write operation
  issued after an RMA or atomic read operation, both of which are
  smaller than the size, will be ordered.  The RMA or atomic read
  operation will see the initial value of the target memory location
  before a subsequent RMA or atomic write updates the value.

*max_order_waw_size*
: Write after write size.  If set, an RMA or atomic write operation
  issued after an RMA or atomic write operation, both of which are
  smaller than the size, will be ordered.  The target memory location
  will reflect the results of the second RMA or atomic write.

An order size value of 0 indicates that ordering is not guaranteed.
A value of -1 guarantees ordering for any data size.

## mem_tag_format - Memory Tag Format

The memory tag format is a bit array used to convey the number of
tagged bits supported by a provider.  Additionally, it may be used to
divide the bit array into separate fields.  The mem_tag_format
optionally begins with a series of bits set to 0, to signify bits
which are ignored by the provider.  Following the initial prefix of
ignored bits, the array will consist of alternating groups of bits set
to all 1's or all 0's.  Each group of bits corresponds to a tagged
field.  The implication of defining a tagged field is that when a mask
is applied to the tagged bit array, all bits belonging to a single
field will either be set to 1 or 0, collectively.

For example, a mem_tag_format of 0x30FF indicates support for 14
tagged bits, separated into 3 fields.  The first field consists of
2-bits, the second field 4-bits, and the final field 8-bits.  Valid
masks for such a tagged field would be a bitwise OR'ing of zero or
more of the following values: 0x3000, 0x0F00, and 0x00FF. The provider
may not validate the mask provided by the application for performance
reasons.

By identifying fields within a tag, a provider may be able to optimize
their search routines.  An application which requests tag fields must
provide tag masks that either set all mask bits corresponding to a
field to all 0 or all 1.  When negotiating tag fields, an application
can request a specific number of fields of a given size.  A provider
must return a tag format that supports the requested number of fields,
with each field being at least the size requested, or fail the
request.  A provider may increase the size of the fields. When reporting
completions (see FI_CQ_FORMAT_TAGGED), it is not guaranteed that the
provider would clear out any unsupported tag bits in the tag field of
the completion entry.

It is recommended that field sizes be ordered from smallest to
largest.  A generic, unstructured tag and mask can be achieved by
requesting a bit array consisting of alternating 1's and 0's.

## tx_ctx_cnt - Transmit Context Count

Number of transmit contexts to associate with the endpoint.  If not
specified (0), 1 context will be assigned if the endpoint supports
outbound transfers.  Transmit contexts are independent transmit queues
that may be separately configured.  Each transmit context may be bound
to a separate CQ, and no ordering is defined between contexts.
Additionally, no synchronization is needed when accessing contexts in
parallel.

If the count is set to the value FI_SHARED_CONTEXT, the endpoint will
be configured to use a shared transmit context, if supported by the
provider.  Providers that do not support shared transmit contexts will
fail the request.

See the scalable endpoint and shared contexts sections for additional
details.

## rx_ctx_cnt - Receive Context Count

Number of receive contexts to associate with the endpoint.  If not
specified, 1 context will be assigned if the endpoint supports inbound
transfers.  Receive contexts are independent processing queues that
may be separately configured.  Each receive context may be bound to a
separate CQ, and no ordering is defined between contexts.
Additionally, no synchronization is needed when accessing contexts in
parallel.

If the count is set to the value FI_SHARED_CONTEXT, the endpoint will
be configured to use a shared receive context, if supported by the
provider.  Providers that do not support shared receive contexts will
fail the request.

See the scalable endpoint and shared contexts sections for additional
details.

## auth_key_size - Authorization Key Length

The length of the authorization key in bytes.  This field will be 0 if
authorization keys are not available or used.  This field is ignored
unless the fabric is opened with API version 1.5 or greater.

## auth_key - Authorization Key

If supported by the fabric, an authorization key (a.k.a. job
key) to associate with the endpoint.  An authorization key is used
to limit communication between endpoints.  Only peer endpoints that are
programmed to use the same authorization key may communicate.
Authorization keys are often used to implement job keys, to ensure
that processes running in different jobs do not accidentally
cross traffic.  The domain authorization key will be used if auth_key_size
is set to 0.  This field is ignored unless the fabric is opened with API
version 1.5 or greater.

# TRANSMIT CONTEXT ATTRIBUTES

Attributes specific to the transmit capabilities of an endpoint are
specified using struct fi_tx_attr.

{% highlight c %}
struct fi_tx_attr {
	uint64_t  caps;
	uint64_t  mode;
	uint64_t  op_flags;
	uint64_t  msg_order;
	uint64_t  comp_order;
	size_t    inject_size;
	size_t    size;
	size_t    iov_limit;
	size_t    rma_iov_limit;
	uint32_t  tclass;
};
{% endhighlight %}

## caps - Capabilities

The requested capabilities of the context.  The capabilities must be
a subset of those requested of the associated endpoint.  See the
CAPABILITIES section of fi_getinfo(3) for capability details.  If
the caps field is 0 on input to fi_getinfo(3), the applicable
capability bits from the fi_info structure will be used.

The following capabilities apply to the transmit attributes: FI_MSG,
FI_RMA, FI_TAGGED, FI_ATOMIC, FI_READ, FI_WRITE, FI_SEND, FI_HMEM,
FI_TRIGGER, FI_FENCE, FI_MULTICAST, FI_RMA_PMEM, FI_NAMED_RX_CTX,
and FI_COLLECTIVE.

Many applications will be able to ignore this field and rely solely
on the fi_info::caps field.  Use of this field provides fine grained
control over the transmit capabilities associated with an endpoint.
It is useful when handling scalable endpoints, with multiple transmit
contexts, for example, and allows configuring a specific transmit
context with fewer capabilities than that supported by the endpoint
or other transmit contexts.

## mode

The operational mode bits of the context.  The mode bits will be a
subset of those associated with the endpoint.  See the MODE section
of fi_getinfo(3) for details.  A mode value of 0 will be ignored on
input to fi_getinfo(3), with the mode value of the fi_info structure
used instead.  On return from fi_getinfo(3), the mode will be set
only to those constraints specific to transmit operations.

## op_flags - Default transmit operation flags

Flags that control the operation of operations submitted against the
context.  Applicable flags are listed in the Operation Flags
section.

## msg_order - Message Ordering

Message ordering refers to the order in which transport layer headers
(as viewed by the application) are identified and processed.  Relaxed message
order enables data transfers to be sent and received out of order, which may
improve performance by utilizing multiple paths through the fabric
from the initiating endpoint to a target endpoint.  Message order
applies only between a single source and destination endpoint pair.
Ordering between different target endpoints is not defined.

Message order is determined using a set of ordering bits.  Each set
bit indicates that ordering is maintained between data transfers of
the specified type.  Message order is defined for [read | write |
send] operations submitted by an application after [read | write |
send] operations.

Message ordering only applies to the end to end transmission of transport
headers.  Message ordering is necessary, but does not guarantee, the order in
which message data is sent or received by the transport layer.  Message
ordering requires matching ordering semantics on the receiving side of a data
transfer operation in order to guarantee that ordering is met.

*FI_ORDER_ATOMIC_RAR*
: Atomic read after read.  If set, atomic fetch operations are
  transmitted in the order submitted relative to other
  atomic fetch operations.  If not set, atomic fetches
  may be transmitted out of order from their submission.

*FI_ORDER_ATOMIC_RAW*
: Atomic read after write.  If set, atomic fetch operations are
  transmitted in the order submitted relative to atomic update
  operations.  If not set, atomic fetches may be transmitted ahead
  of atomic updates.

*FI_ORDER_ATOMIC_WAR*
: RMA write after read.  If set, atomic update operations are
  transmitted in the order submitted relative to atomic fetch
  operations.  If not set, atomic updates may be transmitted
  ahead of atomic fetches.

*FI_ORDER_ATOMIC_WAW*
: RMA write after write.  If set, atomic update operations are
  transmitted in the order submitted relative to other atomic
  update operations.  If not atomic updates may be
  transmitted out of order from their submission.

*FI_ORDER_NONE*
: No ordering is specified.  This value may be used as input in order
  to obtain the default message order supported by the provider. FI_ORDER_NONE
  is an alias for the value 0.

*FI_ORDER_RAR*
: Read after read.  If set, RMA and atomic read operations are
  transmitted in the order submitted relative to other
  RMA and atomic read operations.  If not set, RMA and atomic reads
  may be transmitted out of order from their submission.

*FI_ORDER_RAS*
: Read after send.  If set, RMA and atomic read operations are
  transmitted in the order submitted relative to message send
  operations, including tagged sends.  If not set, RMA and atomic
  reads may be transmitted ahead of sends.

*FI_ORDER_RAW*
: Read after write.  If set, RMA and atomic read operations are
  transmitted in the order submitted relative to RMA and atomic write
  operations.  If not set, RMA and atomic reads may be transmitted ahead
  of RMA and atomic writes.

*FI_ORDER_RMA_RAR*
: RMA read after read.  If set, RMA read operations are
  transmitted in the order submitted relative to other
  RMA read operations.  If not set, RMA reads
  may be transmitted out of order from their submission.

*FI_ORDER_RMA_RAW*
: RMA read after write.  If set, RMA read operations are
  transmitted in the order submitted relative to RMA write
  operations.  If not set, RMA reads may be transmitted ahead
  of RMA writes.

*FI_ORDER_RMA_WAR*
: RMA write after read.  If set, RMA write operations are
  transmitted in the order submitted relative to RMA read
  operations.  If not set, RMA writes may be transmitted
  ahead of RMA reads.

*FI_ORDER_RMA_WAW*
: RMA write after write.  If set, RMA write operations are
  transmitted in the order submitted relative to other RMA
  write operations.  If not set, RMA writes may be
  transmitted out of order from their submission.

*FI_ORDER_SAR*
: Send after read.  If set, message send operations, including tagged
  sends, are transmitted in order submitted relative to RMA and atomic
  read operations.  If not set, message sends may be transmitted ahead
  of RMA and atomic reads.

*FI_ORDER_SAS*
: Send after send.  If set, message send operations, including tagged
  sends, are transmitted in the order submitted relative to other
  message send.  If not set, message sends may be transmitted out of
  order from their submission.

*FI_ORDER_SAW*
: Send after write.  If set, message send operations, including tagged
  sends, are transmitted in order submitted relative to RMA and atomic
  write operations.  If not set, message sends may be transmitted ahead
  of RMA and atomic writes.

*FI_ORDER_WAR*
: Write after read.  If set, RMA and atomic write operations are
  transmitted in the order submitted relative to RMA and atomic read
  operations.  If not set, RMA and atomic writes may be transmitted
  ahead of RMA and atomic reads.

*FI_ORDER_WAS*
: Write after send.  If set, RMA and atomic write operations are
  transmitted in the order submitted relative to message send
  operations, including tagged sends.  If not set, RMA and atomic
  writes may be transmitted ahead of sends.

*FI_ORDER_WAW*
: Write after write.  If set, RMA and atomic write operations are
  transmitted in the order submitted relative to other RMA and atomic
  write operations.  If not set, RMA and atomic writes may be
  transmitted out of order from their submission.

## comp_order - Completion Ordering

Completion ordering refers to the order in which completed requests are
written into the completion queue.  Completion ordering is similar to
message order.  Relaxed completion order may enable faster reporting of
completed transfers, allow acknowledgments to be sent over different
fabric paths, and support more sophisticated retry mechanisms.
This can result in lower-latency completions, particularly when
using connectionless endpoints.  Strict completion ordering may require
that providers queue completed operations or limit available optimizations.

For transmit requests, completion ordering depends on the endpoint
communication type.  For unreliable communication, completion ordering
applies to all data transfer requests submitted to an endpoint.
For reliable communication, completion ordering only applies to requests
that target a single destination endpoint.  Completion ordering of
requests that target different endpoints over a reliable transport
is not defined.

Applications should specify the completion ordering that they support
or require.  Providers should return the completion order that they
actually provide, with the constraint that the returned ordering is
stricter than that specified by the application.  Supported completion
order values are:

*FI_ORDER_NONE*
: No ordering is defined for completed operations.  Requests submitted
  to the transmit context may complete in any order.

*FI_ORDER_STRICT*
: Requests complete in the order in which they are submitted to the
  transmit context.

## inject_size

The requested inject operation size (see the FI_INJECT flag) that
the context will support.  This is the maximum size data transfer that
can be associated with an inject operation (such as fi_inject) or may
be used with the FI_INJECT data transfer flag.

## size

The size of the transmit context.  The mapping of the size value to resources
is provider specific, but it is directly related to the number of command
entries allocated for the endpoint.  A smaller size value consumes fewer
hardware and software resources, while a larger size allows queuing more
transmit requests.

While the size attribute guides the size of underlying endpoint transmit
queue, there is not necessarily a one-to-one mapping between a transmit
operation and a queue entry.  A single transmit operation may consume
multiple queue entries; for example, one per scatter-gather entry.
Additionally, the size field is intended to guide the allocation of the
endpoint's transmit context.  Specifically, for connectionless endpoints,
there may be lower-level queues use to track communication on a per peer basis.
The sizes of any lower-level queues may only be significantly smaller than
the endpoint's transmit size, in order to reduce resource utilization.

## iov_limit

This is the maximum number of IO vectors (scatter-gather elements)
that a single posted operation may reference.

## rma_iov_limit

This is the maximum number of RMA IO vectors (scatter-gather elements)
that an RMA or atomic operation may reference.  The rma_iov_limit
corresponds to the rma_iov_count values in RMA and atomic operations.
See struct fi_msg_rma and struct fi_msg_atomic in fi_rma.3 and
fi_atomic.3, for additional details.  This limit applies to both the
number of RMA IO vectors that may be specified when initiating an
operation from the local endpoint, as well as the maximum number of
IO vectors that may be carried in a single request from a remote endpoint.

## Traffic Class (tclass)

Traffic classes can be a differentiated services
code point (DSCP) value, one of the following defined labels, or a
provider-specific definition.  If tclass is unset or set to FI_TC_UNSPEC,
the endpoint will use the default traffic class associated with the
domain.

*FI_TC_BEST_EFFORT*
: This is the default in the absence of any other local or fabric configuration.
  This class carries the traffic for a number of applications executing
  concurrently over the same network infrastructure. Even though it is shared,
  network capacity and resource allocation are distributed fairly across the
  applications.

*FI_TC_BULK_DATA*
: This class is intended for large data transfers associated with I/O and
  is present to separate sustained I/O transfers from other application
  inter-process communications.

*FI_TC_DEDICATED_ACCESS*
: This class operates at the highest priority, except the management class.
  It carries a high bandwidth allocation, minimum latency targets, and the
  highest scheduling and arbitration priority.

*FI_TC_LOW_LATENCY*
: This class supports low latency, low jitter data patterns typically caused by
  transactional data exchanges, barrier synchronizations, and collective
  operations that are typical of HPC applications. This class often requires
  maximum tolerable latencies that data transfers must achieve for correct or
  performance operations.  Fulfillment of such requests in this class will
  typically require accompanying bandwidth and message size limitations so
  as not to consume excessive bandwidth at high priority.

*FI_TC_NETWORK_CTRL*
: This class is intended for traffic directly related to fabric (network)
  management, which is critical to the correct operation of the network.
  Its use is typically restricted to privileged network management applications.

*FI_TC_SCAVENGER*
: This class is used for data that is desired but does not have strict delivery
  requirements, such as in-band network or application level monitoring data.
  Use of this class indicates that the traffic is considered lower priority
  and should not interfere with higher priority workflows.

*fi_tc_dscp_set / fi_tc_dscp_get*
: DSCP values are supported via the DSCP get and set functions.  The
  definitions for DSCP values are outside the scope of libfabric.  See
  the fi_tc_dscp_set and fi_tc_dscp_get function definitions for details
  on their use.

# RECEIVE CONTEXT ATTRIBUTES

Attributes specific to the receive capabilities of an endpoint are
specified using struct fi_rx_attr.

{% highlight c %}
struct fi_rx_attr {
	uint64_t  caps;
	uint64_t  mode;
	uint64_t  op_flags;
	uint64_t  msg_order;
	uint64_t  comp_order;
	size_t    total_buffered_recv;
	size_t    size;
	size_t    iov_limit;
};
{% endhighlight %}

## caps - Capabilities

The requested capabilities of the context.  The capabilities must be
a subset of those requested of the associated endpoint.  See the
CAPABILITIES section if fi_getinfo(3) for capability details.  If
the caps field is 0 on input to fi_getinfo(3), the applicable
capability bits from the fi_info structure will be used.

The following capabilities apply to the receive attributes: FI_MSG,
FI_RMA, FI_TAGGED, FI_ATOMIC, FI_REMOTE_READ, FI_REMOTE_WRITE, FI_RECV,
FI_HMEM, FI_TRIGGER, FI_RMA_PMEM, FI_DIRECTED_RECV, FI_VARIABLE_MSG,
FI_MULTI_RECV, FI_SOURCE, FI_RMA_EVENT, FI_SOURCE_ERR, and
FI_COLLECTIVE.

Many applications will be able to ignore this field and rely solely
on the fi_info::caps field.  Use of this field provides fine grained
control over the receive capabilities associated with an endpoint.
It is useful when handling scalable endpoints, with multiple receive
contexts, for example, and allows configuring a specific receive
context with fewer capabilities than that supported by the endpoint
or other receive contexts.

## mode

The operational mode bits of the context.  The mode bits will be a
subset of those associated with the endpoint.  See the MODE section
of fi_getinfo(3) for details.  A mode value of 0 will be ignored on
input to fi_getinfo(3), with the mode value of the fi_info structure
used instead.  On return from fi_getinfo(3), the mode will be set
only to those constraints specific to receive operations.

## op_flags - Default receive operation flags

Flags that control the operation of operations submitted against the
context.  Applicable flags are listed in the Operation Flags
section.

## msg_order - Message Ordering

For a description of message ordering, see the msg_order field in
the _Transmit Context Attribute_ section.  Receive context message
ordering defines the order in which received transport message headers
are processed when received by an endpoint.  When ordering is set, it
indicates that message headers will be processed in order, based on
how the transmit side has identified the messages. Typically, this means
that messages will be handled in order based on a message level sequence
number.

The following ordering flags, as defined for transmit ordering, also
apply to the processing of received operations: FI_ORDER_NONE,
FI_ORDER_RAR, FI_ORDER_RAW, FI_ORDER_RAS, FI_ORDER_WAR, FI_ORDER_WAW,
FI_ORDER_WAS, FI_ORDER_SAR, FI_ORDER_SAW, FI_ORDER_SAS, FI_ORDER_RMA_RAR,
FI_ORDER_RMA_RAW, FI_ORDER_RMA_WAR, FI_ORDER_RMA_WAW, FI_ORDER_ATOMIC_RAR,
FI_ORDER_ATOMIC_RAW, FI_ORDER_ATOMIC_WAR, and FI_ORDER_ATOMIC_WAW.

## comp_order - Completion Ordering

For a description of completion ordering, see the comp_order field in
the _Transmit Context Attribute_ section.

*FI_ORDER_DATA*
: When set, this bit indicates that received data is written into memory
  in order.  Data ordering applies to memory accessed as part of a single
  operation and between operations if message ordering is guaranteed.

*FI_ORDER_NONE*
: No ordering is defined for completed operations.  Receive operations may
  complete in any order, regardless of their submission order.

*FI_ORDER_STRICT*
: Receive operations complete in the order in which they are processed by
  the receive context, based on the receive side msg_order attribute.

## total_buffered_recv

This field is supported for backwards compatibility purposes.
It is a hint to the provider of the total available space
that may be needed to buffer messages that are received for which there
is no matching receive operation.  The provider may adjust or ignore
this value.  The allocation of internal network buffering among received
message is provider specific.  For instance, a provider may limit the size
of messages which can be buffered or the amount of buffering allocated to
a single message.

If receive side buffering is disabled (total_buffered_recv = 0)
and a message is received by an endpoint, then the behavior is dependent on
whether resource management has been enabled (FI_RM_ENABLED has be set or not).
See the Resource Management section of fi_domain.3 for further clarification.
It is recommended that applications enable resource management if they
anticipate receiving unexpected messages, rather than modifying this value.

## size

The size of the receive context.  The mapping of the size value to resources
is provider specific, but it is directly related to the number of command
entries allocated for the endpoint.  A smaller size value consumes fewer
hardware and software resources, while a larger size allows queuing more
transmit requests.

While the size attribute guides the size of underlying endpoint receive
queue, there is not necessarily a one-to-one mapping between a receive
operation and a queue entry.  A single receive operation may consume
multiple queue entries; for example, one per scatter-gather entry.
Additionally, the size field is intended to guide the allocation of the
endpoint's receive context.  Specifically, for connectionless endpoints,
there may be lower-level queues use to track communication on a per peer basis.
The sizes of any lower-level queues may only be significantly smaller than
the endpoint's receive size, in order to reduce resource utilization.

## iov_limit

This is the maximum number of IO vectors (scatter-gather elements)
that a single posted operating may reference.

# SCALABLE ENDPOINTS

A scalable endpoint is a communication portal that supports multiple
transmit and receive contexts.  Scalable endpoints are loosely modeled
after the networking concept of transmit/receive side scaling, also
known as multi-queue.  Support for scalable endpoints is domain
specific.  Scalable endpoints may improve the performance of
multi-threaded and parallel applications, by allowing threads to
access independent transmit and receive queues.  A scalable endpoint
has a single transport level address, which can reduce the memory
requirements needed to store remote addressing data, versus using
standard endpoints. Scalable endpoints cannot be used directly for
communication operations, and require the application to explicitly
create transmit and receive contexts as described below.

## fi_tx_context

Transmit contexts are independent transmit queues.  Ordering and
synchronization between contexts are not defined.  Conceptually a
transmit context behaves similar to a send-only endpoint.  A transmit
context may be configured with fewer capabilities than the base
endpoint and with different attributes (such as ordering requirements
and inject size) than other contexts associated with the same scalable
endpoint.  Each transmit context has its own completion queue.  The
number of transmit contexts associated with an endpoint is specified
during endpoint creation.

The fi_tx_context call is used to retrieve a specific context,
identified by an index (see above for details on transmit context
attributes).  Providers may dynamically allocate contexts when
fi_tx_context is called, or may statically create all contexts when
fi_endpoint is invoked.  By default, a transmit context inherits the
properties of its associated endpoint.  However, applications may
request context specific attributes through the attr parameter.
Support for per transmit context attributes is provider specific and
not guaranteed.  Providers will return the actual attributes assigned
to the context through the attr parameter, if provided.

## fi_rx_context

Receive contexts are independent receive queues for receiving incoming
data.  Ordering and synchronization between contexts are not
guaranteed.  Conceptually a receive context behaves similar to a
receive-only endpoint.  A receive context may be configured with fewer
capabilities than the base endpoint and with different attributes
(such as ordering requirements and inject size) than other contexts
associated with the same scalable endpoint.  Each receive context has
its own completion queue.  The number of receive contexts associated
with an endpoint is specified during endpoint creation.

Receive contexts are often associated with steering flows, that
specify which incoming packets targeting a scalable endpoint to
process.  However, receive contexts may be targeted directly by the
initiator, if supported by the underlying protocol.  Such contexts are
referred to as 'named'.  Support for named contexts must be indicated
by setting the caps FI_NAMED_RX_CTX capability when the corresponding
endpoint is created.  Support for named receive contexts is
coordinated with address vectors.  See fi_av(3) and fi_rx_addr(3).

The fi_rx_context call is used to retrieve a specific context,
identified by an index (see above for details on receive context
attributes).  Providers may dynamically allocate contexts when
fi_rx_context is called, or may statically create all contexts when
fi_endpoint is invoked.  By default, a receive context inherits the
properties of its associated endpoint.  However, applications may
request context specific attributes through the attr parameter.
Support for per receive context attributes is provider specific and
not guaranteed.  Providers will return the actual attributes assigned
to the context through the attr parameter, if provided.

# SHARED CONTEXTS

Shared contexts are transmit and receive contexts explicitly shared
among one or more endpoints.  A shareable context allows an application
to use a single dedicated provider resource among multiple transport
addressable endpoints.  This can greatly reduce the resources needed
to manage communication over multiple endpoints by multiplexing
transmit and/or receive processing, with the potential cost of
serializing access across multiple endpoints.  Support for shareable
contexts is domain specific.

Conceptually, shareable transmit contexts are transmit queues that may be
accessed by many endpoints.  The use of a shared transmit context is
mostly opaque to an application.  Applications must allocate and bind
shared transmit contexts to endpoints, but operations are posted
directly to the endpoint.  Shared transmit contexts are not associated
with completion queues or counters.  Completed operations are posted
to the CQs bound to the endpoint.  An endpoint may only
be associated with a single shared transmit context.

Unlike shared transmit contexts, applications interact directly with
shared receive contexts.  Users post receive buffers directly to a
shared receive context, with the buffers usable by any endpoint bound
to the shared receive context.  Shared receive contexts are not
associated with completion queues or counters.  Completed receive
operations are posted to the CQs bound to the endpoint.  An endpoint
may only be associated with a single receive context, and all
connectionless endpoints associated with a shared receive context must
also share the same address vector.

Endpoints associated with a shared transmit context may use dedicated
receive contexts, and vice-versa.  Or an endpoint may use shared
transmit and receive contexts.  And there is no requirement that the
same group of endpoints sharing a context of one type also share the
context of an alternate type.  Furthermore, an endpoint may use a
shared context of one type, but a scalable set of contexts of the
alternate type.

## fi_stx_context

This call is used to open a shareable transmit context (see above for
details on the transmit context attributes).  Endpoints associated
with a shared transmit context must use a subset of the transmit
context's attributes.  Note that this is the reverse of the
requirement for transmit contexts for scalable endpoints.

## fi_srx_context

This allocates a shareable receive context (see above for details on
the receive context attributes).  Endpoints associated with a shared
receive context must use a subset of the receive context's attributes.
Note that this is the reverse of the requirement for receive contexts
for scalable endpoints.

# SOCKET ENDPOINTS

The following feature and description should be considered experimental.
Until the experimental tag is removed, the interfaces, semantics, and data
structures associated with socket endpoints may change between library
versions.

This section applies to endpoints of type FI_EP_SOCK_STREAM and
FI_EP_SOCK_DGRAM, commonly referred to as socket endpoints.

Socket endpoints are defined with semantics that allow them to more
easily be adopted by developers familiar with the UNIX socket API, or
by middleware that exposes the socket API, while still taking advantage
of high-performance hardware features.

The key difference between socket endpoints and other active endpoints
are socket endpoints use synchronous data transfers.  Buffers passed
into send and receive operations revert to the control of the application
upon returning from the function call.  As a result, no data transfer
completions are reported to the application, and socket endpoints are not
associated with completion queues or counters.

Socket endpoints support a subset of message operations: fi_send,
fi_sendv, fi_sendmsg, fi_recv, fi_recvv, fi_recvmsg, and fi_inject.
Because data transfers are synchronous, the return value from send and receive
operations indicate the number of bytes transferred on success, or a negative
value on error, including -FI_EAGAIN if the endpoint cannot send or
receive any data because of full or empty queues, respectively.

Socket endpoints are associated with event queues and address vectors, and
process connection management events asynchronously, similar to other endpoints.
Unlike UNIX sockets, socket endpoint must still be declared as either active
or passive.

Socket endpoints behave like non-blocking sockets.  In order to support
select and poll semantics, active socket endpoints are associated with a
file descriptor that is signaled whenever the endpoint is ready to send
and/or receive data.  The file descriptor may be retrieved using fi_control.

# OPERATION FLAGS

Operation flags are obtained by OR-ing the following flags together.
Operation flags define the default flags applied to an endpoint's data
transfer operations, where a flags parameter is not available.  Data
transfer operations that take flags as input override the op_flags
value of transmit or receive context attributes of an endpoint.

*FI_COMMIT_COMPLETE*
: Indicates that a completion should not be generated (locally or at the
  peer) until the result of an operation have been made persistent.
  See [`fi_cq`(3)](fi_cq.3.html) for additional details on completion
  semantics.

*FI_COMPLETION*
: Indicates that a completion queue entry should be written for data
  transfer operations. This flag only applies to operations issued on an
  endpoint that was bound to a completion queue with the
  FI_SELECTIVE_COMPLETION flag set, otherwise, it is ignored.  See the
  fi_ep_bind section above for more detail.

*FI_DELIVERY_COMPLETE*
: Indicates that a completion should be generated when the operation has been
  processed by the destination endpoint(s).  See [`fi_cq`(3)](fi_cq.3.html)
  for additional details on completion semantics.

*FI_INJECT*
: Indicates that all outbound data buffers should be returned to the
  user's control immediately after a data transfer call returns, even
  if the operation is handled asynchronously.  This may require that
  the provider copy the data into a local buffer and transfer out of
  that buffer.  A provider can limit the total amount of send data
  that may be buffered and/or the size of a single send that can use
  this flag. This limit is indicated using inject_size (see inject_size
  above).

*FI_INJECT_COMPLETE*
: Indicates that a completion should be generated when the
  source buffer(s) may be reused.  See [`fi_cq`(3)](fi_cq.3.html) for
  additional details on completion semantics.

*FI_MULTICAST*
: Indicates that data transfers will target multicast addresses by default.
  Any fi_addr_t passed into a data transfer operation will be treated as a
  multicast address.

*FI_MULTI_RECV*
: Applies to posted receive operations.  This flag allows the user to
  post a single buffer that will receive multiple incoming messages.
  Received messages will be packed into the receive buffer until the
  buffer has been consumed.  Use of this flag may cause a single
  posted receive operation to generate multiple completions as
  messages are placed into the buffer.  The placement of received data
  into the buffer may be subjected to provider specific alignment
  restrictions.  The buffer will be released by the provider when the
  available buffer space falls below the specified minimum (see
  FI_OPT_MIN_MULTI_RECV).

*FI_TRANSMIT_COMPLETE*
: Indicates that a completion should be generated when the transmit
  operation has completed relative to the local provider.  See
  [`fi_cq`(3)](fi_cq.3.html) for additional details on completion semantics.

# NOTES

Users should call fi_close to release all resources allocated to the
fabric endpoint.

Endpoints allocated with the FI_CONTEXT or FI_CONTEXT2 mode bits set must
typically provide struct fi_context(2) as their per operation context parameter.
(See fi_getinfo.3 for details.)  However, when FI_SELECTIVE_COMPLETION is
enabled to suppress CQ completion entries, and an operation is initiated
without the FI_COMPLETION flag set, then the context parameter is ignored.
An application does not need to pass in a valid struct fi_context(2) into
such data transfers.

Operations that complete in error that are not associated with valid
operational context will use the endpoint context in any error
reporting structures.

Although applications typically associate individual completions with
either completion queues or counters, an endpoint can be attached to
both a counter and completion queue.  When combined with using
selective completions, this allows an application to use counters to
track successful completions, with a CQ used to report errors.
Operations that complete with an error increment the error counter
and generate a CQ completion event.

As mentioned in fi_getinfo(3), the ep_attr structure can be used to
query providers that support various endpoint attributes. fi_getinfo
can return provider info structures that can support the minimal set
of requirements (such that the application maintains correctness).
However, it can also return provider info structures that exceed
application requirements. As an example, consider an application
requesting msg_order as FI_ORDER_NONE. The resulting output from
fi_getinfo may have all the ordering bits set. The application can reset
the ordering bits it does not require before creating the endpoint.
The provider is free to implement a stricter ordering than is
required by the application.

# RETURN VALUES

Returns 0 on success.  On error, a negative value corresponding to
fabric errno is returned.  For fi_cancel, a return value of 0
indicates that the cancel request was submitted for processing.

Fabric errno values are defined in `rdma/fi_errno.h`.

# ERRORS

*-FI_EDOMAIN*
: A resource domain was not bound to the endpoint or an attempt was
  made to bind multiple domains.

*-FI_ENOCQ*
: The endpoint has not been configured with necessary event queue.

*-FI_EOPBADSTATE*
: The endpoint's state does not permit the requested operation.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_cq`(3)](fi_cq.3.html)
[`fi_msg`(3)](fi_msg.3.html),
[`fi_tagged`(3)](fi_tagged.3.html),
[`fi_rma`(3)](fi_rma.3.html)
