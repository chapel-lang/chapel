---
layout: page
title: fi_getinfo(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_getinfo, fi_freeinfo \- Obtain / free fabric interface information

fi_allocinfo, fi_dupinfo \- Allocate / duplicate an fi_info structure

# SYNOPSIS

```c
#include <rdma/fabric.h>

int fi_getinfo(int version, const char *node, const char *service,
        uint64_t flags, const struct fi_info *hints, struct fi_info **info);

void fi_freeinfo(struct fi_info *info);

struct fi_info *fi_allocinfo(void);

struct fi_info *fi_dupinfo(const struct fi_info *info);
```

# ARGUMENTS

*version*
: Interface version requested by application.

*node*
: Optional, name or fabric address to resolve.

*service*
: Optional, service name or port number of address.

*flags*
: Operation flags for the fi_getinfo call.

*hints*
: Reference to an fi_info structure that specifies criteria for
  selecting the returned fabric information.

*info*
: A pointer to a linked list of fi_info structures containing response
  information.

# DESCRIPTION

fi_getinfo returns information about available fabric services for reaching
specified node or service, subject to any provided hints.  Callers
may specify NULL for node, service, and hints in order to retrieve
information about what providers are available and their optimal usage
models.  If no matching fabric information is available, info will
be set to NULL and the call will return -FI_ENODATA.

Based on the input hints, node, and service parameters, a list of
fabric domains and endpoints will be returned.  Each fi_info structure
will describe an endpoint that meets the application's specified
communication criteria.  Each endpoint will be associated with a
domain.  Applications can restrict the number of returned endpoints by
including additional criteria in their search hints.  Relaxing or
eliminating input hints will increase the number and type of endpoints
that are available.  Providers that return multiple endpoints to a
single fi_getinfo call should return the endpoints that are highest
performing first.  Providers may indicate that an endpoint and domain can
support additional capabilities than those requested by the user only
if such support will not adversely affect application performance or security.

The version parameter is used by the application to request the
desired version of the interfaces.  The version determines the format
of all data structures used by any of the fabric interfaces.
Applications should use the FI_VERSION(major, minor) macro to indicate
the version, with hard-coded integer values for the major and minor
values.  The FI_MAJOR_VERSION and FI_MINOR_VERSION enum values defined
in fabric.h specify the latest version of the installed library.
However, it is recommended that the integer values for
FI_MAJOR_VERSION and FI_MINOR_VERSION be used, rather than referencing
the enum types in order to ensure compatibility with future versions
of the library.  This protects against the application being built
from source against a newer version of the library that introduces new
fields to data structures, which would not be initialized by the
application.

Node, service, or hints may be provided, with any combination
being supported.  If node is provided, fi_getinfo will attempt to
resolve the fabric address to the given node.  If node is not given,
fi_getinfo will attempt to resolve the fabric addressing information
based on the provided hints.  Node is commonly used to provide a network
address (such as an IP address) or hostname.  Service is usually
associated with a transport address (such as a TCP port number).  Node
and service parameters may be mapped by providers to native fabric
addresses.  Applications may also pass in an FI_ADDR_STR formatted
address (see format details below) as the node parameter.  In such cases,
the service parameter must be NULL.

The hints parameter, if provided, may be used to limit the resulting
output as indicated below.  As a general rule, specifying a non-zero
value for input hints indicates that a provider must support the
requested value or fail the operation with -FI_ENODATA.  With the
exception of mode bits, hints that are set to zero are treated as
a wildcard.  A zeroed hint value results in providers either returning
a default value or a value that works best for their implementation.
Mode bits that are set to zero indicate the application does not support
any modes.

The caller must call fi_freeinfo to release fi_info structures returned
by this call.

The fi_allocinfo call will allocate and zero an fi_info structure
and all related substructures.  The fi_dupinfo will duplicate
a single fi_info structure and all the substructures within it.


# FI_INFO

```c
struct fi_info {
	struct fi_info        *next;
	uint64_t              caps;
	uint64_t              mode;
	uint32_t              addr_format;
	size_t                src_addrlen;
	size_t                dest_addrlen;
	void                  *src_addr;
	void                  *dest_addr;
	fid_t                 handle;
	struct fi_tx_attr     *tx_attr;
	struct fi_rx_attr     *rx_attr;
	struct fi_ep_attr     *ep_attr;
	struct fi_domain_attr *domain_attr;
	struct fi_fabric_attr *fabric_attr;
	struct fid_nic        *nic;
};
```

*next*
: Pointer to the next fi_info structure in the list.  Will be NULL
  if no more structures exist.

*caps - fabric interface capabilities*
: If specified, indicates the desired capabilities of the fabric
  interfaces.  Supported capabilities are listed in the _Capabilities_
  section below.

*mode*
: Operational modes supported by the application.  See the _Mode_
  section below.

*addr_format - address format*
: If specified, indicates the format of addresses referenced by the
  fabric interfaces and data structures.  Supported formats are listed
  in the _Addressing formats_ section below.

*src_addrlen - source address length*
: Indicates the length of the source address.  This value must be > 0
  if *src_addr* is non-NULL.  This field will be ignored in hints if
  FI_SOURCE flag is set, or *src_addr* is NULL.

*dest_addrlen - destination address length*
: Indicates the length of the destination address.  This value must be > 0
  if *dest_addr* is non-NULL.  This field will be ignored in hints
  unless the node and service parameters are NULL or FI_SOURCE flag is
  set, or if *dst_addr* is NULL.

*src_addr - source address*
: If specified, indicates the source address.  This field will be
  ignored in hints if FI_SOURCE flag is set.  On output a provider shall
  return an address that corresponds to the indicated fabric, domain,
  node, and/or service fields.  The format of the address is indicated
  by the returned *addr_format* field.  Note that any returned address
  is only used when opening a local endpoint.  The address is not
  guaranteed to be usable by a peer process.

*dest_addr - destination address*
: If specified, indicates the destination address.  This field will be
  ignored in hints unless the node and service parameters are NULL or
  FI_SOURCE flag is set.  If FI_SOURCE is not specified, on output a
  provider shall return an address the corresponds to the indicated
  node and/or service fields, relative to the fabric and domain.  Note
  that any returned address is only usable locally.

*handle - provider context handle*
: The use of this field is operation specific. If hints->handle is set to struct
  fid_pep, the hints->handle will be copied to info->handle on output from
  fi_getinfo.  Other values of hints->handle will be handled in a provider
  specific manner.  The fi_info::handle field is also used by fi_endpoint()
  and fi_reject() calls when processing connection requests or to inherit
  another endpoint's attributes.  See [`fi_eq`(3)](fi_eq.3.html),
  [`fi_reject`(3)](fi_reject.3.html), and
  [`fi_endpoint`(3)](fi_endpoint.3.html).  The info->handle field will be
  ignored by fi_dupinfo and fi_freeinfo.

*tx_attr - transmit context attributes*
: Optionally supplied transmit context attributes.  Transmit context
  attributes may be specified and returned as part of fi_getinfo.
  When provided as hints, requested values of struct fi_tx_ctx_attr
  should be set.  On output, the actual transmit context attributes
  that can be provided will be returned.  Output values will be
  greater than or equal to the requested input values.

*rx_attr - receive context attributes*
: Optionally supplied receive context attributes.  Receive context
  attributes may be specified and returned as part of fi_getinfo.
  When provided as hints, requested values of struct fi_rx_ctx_attr
  should be set.  On output, the actual receive context attributes
  that can be provided will be returned.  Output values will be
  greater than or or equal to the requested input values.

*ep_attr - endpoint attributes*
: Optionally supplied endpoint attributes.  Endpoint attributes may be
  specified and returned as part of fi_getinfo.  When provided as
  hints, requested values of struct fi_ep_attr should be set.  On
  output, the actual endpoint attributes that can be provided will be
  returned.  Output values will be greater than or equal to requested
  input values.  See [`fi_endpoint`(3)](fi_endpoint.3.html) for details.

*domain_attr - domain attributes*
: Optionally supplied domain attributes.  Domain attributes may be
  specified and returned as part of fi_getinfo.  When provided as
  hints, requested values of struct fi_domain_attr should be set.  On
  output, the actual domain attributes that can be provided will be
  returned.  Output values will be greater than or equal to requested
  input values.  See [`fi_domain`(3)](fi_domain.3.html) for details.

*fabric_attr - fabric attributes*
: Optionally supplied fabric attributes.  Fabric attributes may be
  specified and returned as part of fi_getinfo.  When provided as
  hints, requested values of struct fi_fabric_attr should be set.  On
  output, the actual fabric attributes that can be provided will be
  returned.  See [`fi_fabric`(3)](fi_fabric.3.html) for details.

*nic - network interface details*
: Optional attributes related to the hardware NIC associated with
  the specified fabric, domain, and endpoint data.  This field is
  only valid for providers where the corresponding attributes are
  closely associated with a hardware NIC.  See
  [`fi_nic`(3)](fi_nic.3.html) for details.

# CAPABILITIES

Interface capabilities are obtained by OR-ing the following flags
together.  If capabilities in the hint parameter are set to 0, the
underlying provider will return the set of capabilities which are
supported.  Otherwise, providers will return data matching the
specified set of capabilities.  Providers may indicate support for
additional capabilities beyond those requested when the use of
expanded capabilities will not adversely affect performance or expose
the application to communication beyond that which was requested.
Applications may use this feature to request a minimal set of
requirements, then check the returned capabilities to enable
additional optimizations.

*FI_ATOMIC*
: Specifies that the endpoint supports some set of atomic operations.
  Endpoints supporting this capability support operations defined by
  struct fi_ops_atomic.  In the absence of any relevant flags,
  FI_ATOMIC implies the ability to initiate and be the target of
  remote atomic reads and writes.  Applications can use the FI_READ,
  FI_WRITE, FI_REMOTE_READ, and FI_REMOTE_WRITE flags to restrict the
  types of atomic operations supported by an endpoint.

*FI_COLLECTIVE*
: Requests support for collective operations.  Endpoints that support
  this capability support the collective operations defined in
  [`fi_collective`(3)](fi_collective.3.html).

*FI_DIRECTED_RECV*
: Requests that the communication endpoint use the source address of
  an incoming message when matching it with a receive buffer.  If this
  capability is not set, then the src_addr parameter for msg and tagged
  receive operations is ignored.

*FI_FENCE*
: Indicates that the endpoint support the FI_FENCE flag on data
  transfer operations.  Support requires tracking that all previous
  transmit requests to a specified remote endpoint complete prior
  to initiating the fenced operation.  Fenced operations are often
  used to enforce ordering between operations that are not otherwise
  guaranteed by the underlying provider or protocol.

*FI_HMEM*
: Specifies that the endpoint should support transfers to and from
  device memory.

*FI_LOCAL_COMM*
: Indicates that the endpoint support host local communication.  This
  flag may be used in conjunction with FI_REMOTE_COMM to indicate that
  local and remote communication are required.  If neither FI_LOCAL_COMM
  or FI_REMOTE_COMM are specified, then the provider will indicate
  support for the configuration that minimally affects performance.
  Providers that set FI_LOCAL_COMM but not FI_REMOTE_COMM, for example
  a shared memory provider, may only be used to communication between
  processes on the same system.

*FI_MSG*
: Specifies that an endpoint should support sending and receiving
  messages or datagrams.  Message capabilities imply support for send
  and/or receive queues.  Endpoints supporting this capability support
  operations defined by struct fi_ops_msg.

  The caps may be used to specify or restrict the type of messaging
  operations that are supported.  In the absence of any relevant
  flags, FI_MSG implies the ability to send and receive messages.
  Applications can use the FI_SEND and FI_RECV flags to optimize an
  endpoint as send-only or receive-only.

*FI_MULTICAST*
: Indicates that the endpoint support multicast data transfers.  This
  capability must be paired with FI_MSG.  Applications can use FI_SEND
  and FI_RECV to optimize multicast as send-only or receive-only.

*FI_MULTI_RECV*
: Specifies that the endpoint must support the FI_MULTI_RECV flag when
  posting receive buffers.

*FI_NAMED_RX_CTX*
: Requests that endpoints which support multiple receive contexts
  allow an initiator to target (or name) a specific receive context as
  part of a data transfer operation.

*FI_READ*
: Indicates that the user requires an endpoint capable of initiating
  reads against remote memory regions.  This flag requires that FI_RMA
  and/or FI_ATOMIC be set.

*FI_RECV*
: Indicates that the user requires an endpoint capable of receiving
  message data transfers.  Message transfers include base message
  operations as well as tagged message functionality.

*FI_REMOTE_COMM*
: Indicates that the endpoint support communication with endpoints
  located at remote nodes (across the fabric).  See FI_LOCAL_COMM for
  additional details.  Providers that set FI_REMOTE_COMM but not
  FI_LOCAL_COMM, for example NICs that lack loopback support, cannot
  be used to communicate with processes on the same system.

*FI_REMOTE_READ*
: Indicates that the user requires an endpoint capable of receiving
  read memory operations from remote endpoints.  This flag requires
  that FI_RMA and/or FI_ATOMIC be set.

*FI_REMOTE_WRITE*
: Indicates that the user requires an endpoint capable of receiving
  write memory operations from remote endpoints.  This flag requires
  that FI_RMA and/or FI_ATOMIC be set.

*FI_RMA*
: Specifies that the endpoint should support RMA read and write
  operations.  Endpoints supporting this capability support operations
  defined by struct fi_ops_rma.  In the absence of any relevant flags,
  FI_RMA implies the ability to initiate and be the target of remote
  memory reads and writes.  Applications can use the FI_READ,
  FI_WRITE, FI_REMOTE_READ, and FI_REMOTE_WRITE flags to restrict the
  types of RMA operations supported by an endpoint.

*FI_RMA_EVENT*
: Requests that an endpoint support the generation of completion events
  when it is the target of an RMA and/or atomic operation.  This
  flag requires that FI_REMOTE_READ and/or FI_REMOTE_WRITE be enabled on
  the endpoint.

*FI_RMA_PMEM*
: Indicates that the provider is 'persistent memory aware' and supports
  RMA operations to and from persistent memory.  Persistent memory aware
  providers must support registration of memory that is backed by non-
  volatile memory, RMA transfers to/from persistent memory, and enhanced
  completion semantics.  This flag requires that FI_RMA be set.
  This capability is experimental.

*FI_SEND*
: Indicates that the user requires an endpoint capable of sending
  message data transfers.  Message transfers include base message
  operations as well as tagged message functionality.

*FI_SHARED_AV*
: Requests or indicates support for address vectors which may be shared
  among multiple processes.

*FI_SOURCE*
: Requests that the endpoint return source addressing data as part of
  its completion data.  This capability only applies to connectionless
  endpoints.  Note that returning source address information may
  require that the provider perform address translation and/or look-up
  based on data available in the underlying protocol in order to
  provide the requested data, which may adversely affect performance.
  The performance impact may be greater for address vectors of type
  FI_AV_TABLE.

*FI_SOURCE_ERR*
: Must be paired with FI_SOURCE.  When specified, this requests that
  raw source addressing data be returned as part of completion data
  for any address that has not been inserted into the local address
  vector.  Use of this capability may require the provider to
  validate incoming source address data against addresses stored in
  the local address vector, which may adversely affect performance.

*FI_TAGGED*
: Specifies that the endpoint should handle tagged message transfers.
  Tagged message transfers associate a user-specified key or tag with
  each message that is used for matching purposes at the remote side.
  Endpoints supporting this capability support operations defined by
  struct fi_ops_tagged.  In the absence of any relevant flags,
  FI_TAGGED implies the ability to send and receive tagged messages.
  Applications can use the FI_SEND and FI_RECV flags to optimize an
  endpoint as send-only or receive-only.

*FI_TRIGGER*
: Indicates that the endpoint should support triggered operations.
  Endpoints support this capability must meet the usage model as
  described by fi_trigger.3.

*FI_VARIABLE_MSG*

: Requests that the provider must notify a receiver when a variable
  length message is ready to be received prior to attempting to place
  the data.  Such notification will include the size of the message and
  any associated message tag (for FI_TAGGED).  See 'Variable Length
  Messages' in fi_msg.3 for full details.  Variable length messages
  are any messages larger than an endpoint configurable size.  This
  flag requires that FI_MSG and/or FI_TAGGED be set.

*FI_WRITE*
: Indicates that the user requires an endpoint capable of initiating
  writes against remote memory regions.  This flag requires that FI_RMA
  and/or FI_ATOMIC be set.

Capabilities may be grouped into three general categories: primary,
secondary, and primary modifiers.  Primary capabilities must explicitly
be requested by an application, and a provider must enable support for
only those primary capabilities which were selected.  Primary modifiers
are used to limit a primary capability, such as restricting an endpoint
to being send-only.  If no modifiers are specified for an applicable
capability, all relevant modifiers are assumed.  See above definitions
for details.

Secondary capabilities may optionally be requested by an application.
If requested, a provider must support the capability or fail the
fi_getinfo request (FI_ENODATA).  A provider
may optionally report non-selected secondary capabilities if doing so
would not compromise performance or security.

Primary capabilities: FI_MSG, FI_RMA, FI_TAGGED, FI_ATOMIC, FI_MULTICAST,
FI_NAMED_RX_CTX, FI_DIRECTED_RECV, FI_VARIABLE_MSG, FI_HMEM, FI_COLLECTIVE

Primary modifiers: FI_READ, FI_WRITE, FI_RECV, FI_SEND,
FI_REMOTE_READ, FI_REMOTE_WRITE

Secondary capabilities: FI_MULTI_RECV, FI_SOURCE, FI_RMA_EVENT, FI_SHARED_AV,
FI_TRIGGER, FI_FENCE, FI_LOCAL_COMM, FI_REMOTE_COMM, FI_SOURCE_ERR, FI_RMA_PMEM.

# MODE

The operational mode bits are used to convey requirements that an
application must adhere to when using the fabric interfaces.  Modes
specify optimal ways of accessing the reported endpoint or domain.
Applications that are designed to support a specific mode of operation
may see improved performance when that mode is desired by the
provider.  It is recommended that providers support applications that
disable any provider preferred modes.

On input to fi_getinfo, applications set the mode bits that they
support.  On output, providers will clear mode bits that are not
necessary to achieve high-performance.  Mode bits that remain set
indicate application requirements for using the fabric interfaces
created using the returned fi_info.  The set of modes are listed
below.  If a NULL hints structure is provided, then the provider's
supported set of modes will be returned in the info structure(s).

*FI_ASYNC_IOV*
: Applications can reference multiple data buffers as part of a single
  operation through the use of IO vectors (SGEs).  Typically,
  the contents of an IO vector are copied by the provider into an
  internal buffer area, or directly to the underlying hardware.
  However, when a large number of IOV entries are supported,
  IOV buffering may have a negative impact on performance and memory
  consumption.  The FI_ASYNC_IOV mode indicates that the application
  must provide the buffering needed for the IO vectors.  When set,
  an application must not modify an IO vector of length > 1, including any
  related memory descriptor array, until the associated
  operation has completed.

*FI_BUFFERED_RECV*
: The buffered receive mode bit indicates that the provider owns the
  data buffer(s) that are accessed by the networking layer for received
  messages.  Typically, this implies that data must be copied from the
  provider buffer into the application buffer.  Applications that can
  handle message processing from network allocated data buffers can set
  this mode bit to avoid copies.  For full details on application
  requirements to support this mode, see the 'Buffered Receives' section
  in [`fi_msg`(3)](fi_msg.3.html).  This mode bit applies to FI_MSG and
  FI_TAGGED receive operations.

*FI_CONTEXT*
: Specifies that the provider requires that applications use struct
  fi_context as their per operation context parameter for operations
  that generated full completions.  This structure
  should be treated as opaque to the application.  For performance
  reasons, this structure must be allocated by the user, but may be
  used by the fabric provider to track the operation.  Typically,
  users embed struct fi_context within their own context structure.
  The struct fi_context must remain valid until the corresponding
  operation completes or is successfully canceled.  As such,
  fi_context should NOT be allocated on the stack.  Doing so is likely
  to result in stack corruption that will be difficult to debug.
  Users should not update or interpret the fields in this structure,
  or reuse it until the original operation has completed.  If an
  operation does not generate a completion (i.e. the endpoint was
  configured with FI_SELECTIVE_COMPLETION and the operation was not
  initiated with the FI_COMPLETION flag) then the context parameter is
  ignored by the fabric provider.  The structure is specified in
  rdma/fabric.h.

*FI_CONTEXT2*
: This bit is similar to FI_CONTEXT, but doubles the provider's
  requirement on the size of the per context structure.  When set,
  this specifies that the provider requires that applications use
  struct fi_context2 as their per operation context parameter.
  Or, optionally, an application can provide an array of two
  fi_context structures (e.g. struct fi_context[2]) instead.
  The requirements for using struct fi_context2 are identical as
  defined for FI_CONTEXT above.

*FI_LOCAL_MR*
: The provider is optimized around having applications register memory
  for locally accessed data buffers.  Data buffers used in send and
  receive operations and as the source buffer for RMA and atomic
  operations must be registered by the application for access domains
  opened with this capability.  This flag is defined for compatibility
  and is ignored if the application version is 1.5 or later and the
  domain mr_mode is set to anything other than FI_MR_BASIC or FI_MR_SCALABLE.
  See the domain attribute mr_mode [`fi_domain`(3)](fi_domain.3.html)
  and [`fi_mr`(3)](fi_mr.3.html).

*FI_MSG_PREFIX*
: Message prefix mode indicates that an application will provide
  buffer space in front of all message send and receive buffers for
  use by the provider.  Typically, the provider uses this space to
  implement a protocol, with the protocol headers being written into
  the prefix area.  The contents of the prefix space should be treated
  as opaque.  The use of FI_MSG_PREFIX may improve application
  performance over certain providers by reducing the number of IO
  vectors referenced by underlying hardware and eliminating provider
  buffer allocation.

  FI_MSG_PREFIX only applies to send and receive operations, including
  tagged sends and receives.  RMA and atomics do not require the
  application to provide prefix buffers.  Prefix buffer space must be
  provided with all sends and receives, regardless of the size of the
  transfer or other transfer options.  The ownership of prefix buffers
  is treated the same as the corresponding message buffers, but the
  size of the prefix buffer is not counted toward any message limits,
  including inject.

  Applications that support prefix mode must supply buffer space
  before their own message data.  The size of space that must be
  provided is specified by the msg_prefix_size endpoint attribute.
  Providers are required to define a msg_prefix_size that is a
  multiple of 8 bytes.  Additionally, applications may receive
  provider generated packets that do not contain application data.
  Such received messages will indicate a transfer size of that is
  equal to or smaller than msg_prefix_size.

  The buffer pointer given to all send and receive operations must point
  to the start of the prefix region of the buffer (as opposed to the
  payload).  For scatter-gather send/recv operations, the prefix buffer
  must be a contiguous region, though it may or may not be directly
  adjacent to the payload portion of the buffer.

*FI_NOTIFY_FLAGS_ONLY*
: This bit indicates that general completion flags may not be set by
  the provider, and are not needed by the application.  If specified,
  completion flags which simply report the type of operation that
  completed (e.g. send or receive) may not be set.  However,
  completion flags that are used for remote notifications will still
  be set when applicable.  See [`fi_cq`(3)](fi_cq.3.html) for details on
  which completion flags are valid when this mode bit is enabled.

*FI_RESTRICTED_COMP*
: This bit indicates that the application will only share completion queues
  and counters among endpoints, transmit contexts, and receive contexts that
  have the same set of capability flags.

*FI_RX_CQ_DATA*
: This mode bit only applies to data transfers that set FI_REMOTE_CQ_DATA.
  When set, a data transfer that carries remote CQ data will consume a
  receive buffer at the target.  This is true even for operations that would
  normally not consume posted receive buffers, such as RMA write operations.

# ADDRESSING FORMATS

Multiple fabric interfaces take as input either a source or
destination address parameter.  This includes struct fi_info (src_addr
and dest_addr), CM calls (getname, getpeer, connect, join, and leave),
and AV calls (insert, lookup, and straddr).  The fi_info addr_format
field indicates the expected address format for these operations.

A provider may support one or more of the following addressing
formats.  In some cases, a selected addressing format may need to be
translated or mapped into an address which is native to the
fabric.  See [`fi_av`(3)](fi_av.3.html).

*FI_ADDR_BGQ*
: Address is an IBM proprietary format that is used with their Blue Gene Q
  systems.

*FI_ADDR_EFA*
: Address is an Amazon Elastic Fabric Adapter (EFA) proprietary format.

*FI_ADDR_GNI*
: Address is a Cray proprietary format that is used with their GNI
  protocol.

*FI_ADDR_PSMX*
: Address is an Intel proprietary format used with their Performance Scaled
  Messaging protocol.

*FI_ADDR_PSMX2*
: Address is an Intel proprietary format used with their Performance Scaled
  Messaging protocol version 2.

*FI_ADDR_PSMX3*
: Address is an Intel proprietary format used with their Performance Scaled
  Messaging protocol version 3.

*FI_ADDR_STR*
: Address is a formatted character string.  The length and content of
  the string is address and/or provider specific, but in general follows
  a URI model:

```
address_format[://[node][:[service][/[field3]...][?[key=value][&k2=v2]...]]]
```

  Examples:
  - fi_sockaddr://10.31.6.12:7471
  - fi_sockaddr_in6://[fe80::6:12]:7471
  - fi_sockaddr://10.31.6.12:7471?qos=3

  Since the string formatted address does not contain any provider
  information, the prov_name field of the fabric attribute structure should
  be used to filter by provider if necessary.

*FI_FORMAT_UNSPEC*
: FI_FORMAT_UNSPEC indicates that a provider specific address format
  should be selected.  Provider specific addresses may be protocol
  specific or a vendor proprietary format.  Applications that select
  FI_FORMAT_UNSPEC should be prepared to treat returned addressing
  data as opaque.  FI_FORMAT_UNSPEC targets apps which make use of an
  out of band address exchange.  Applications which use FI_FORMAT_UNSPEC
  may use fi_getname() to obtain a provider specific address assigned
  to an allocated endpoint.

*FI_SOCKADDR*
: Address is of type sockaddr.  The specific socket address format
  will be determined at run time by interfaces examining the sa_family
  field.

*FI_SOCKADDR_IB*
: Address is of type sockaddr_ib (defined in Linux kernel source)

*FI_SOCKADDR_IN*
: Address is of type sockaddr_in (IPv4).

*FI_SOCKADDR_IN6*
: Address is of type sockaddr_in6 (IPv6).

*FI_ADDR_PSMX*
: Address is an Intel proprietary format that is used with their PSMX
  (extended performance scaled messaging) protocol.

# FLAGS

The operation of the fi_getinfo call may be controlled through the use of
input flags.  Valid flags include the following.

*FI_NUMERICHOST*
: Indicates that the node parameter is a numeric string representation
  of a fabric address, such as a dotted decimal IP address.  Use of
  this flag will suppress any lengthy name resolution protocol.

*FI_PROV_ATTR_ONLY*
: Indicates that the caller is only querying for what providers are
  potentially available.  All providers will return exactly one
  fi_info struct, regardless of whether that provider is usable on the
  current platform or not.  The returned fi_info struct will contain
  default values for all members, with the exception of fabric_attr.
  The fabric_attr member will have the prov_name and prov_version
  values filled in.

*FI_SOURCE*
: Indicates that the node and service parameters specify the local
  source address to associate with an endpoint.  If specified, either
  the node and/or service parameter must be non-NULL.  This flag is
  often used with passive endpoints.

# RETURN VALUE

fi_getinfo() returns 0 on success. On error, fi_getinfo() returns a
negative value corresponding to fabric errno. Fabric errno values are
defined in `rdma/fi_errno.h`.

fi_allocinfo() returns a pointer to a new fi_info structure on
success, or NULL on error.  fi_dupinfo() duplicates a single fi_info
structure and all the substructures within it, returning a pointer to
the new fi_info structure on success, or NULL on error.
Both calls require that the returned fi_info structure be freed
via fi_freeinfo().

# ERRORS

*FI_EBADFLAGS*
: The specified endpoint or domain capability or operation flags are
  invalid.

*FI_ENODATA*
: Indicates that no providers could be found which support the requested
  fabric information.

*FI_ENOMEM*
: Indicates that there was insufficient memory to complete the operation.

# NOTES

If hints are provided, the operation will be controlled by the values
that are supplied in the various fields (see section on _fi_info_).
Applications that require specific communication interfaces, domains,
capabilities or other requirements, can specify them using fields in
_hints_.  Libfabric returns a linked list in *info* that points to a
list of matching interfaces.  *info* is set to NULL if there are no
communication interfaces or none match the input hints.

If node is provided, fi_getinfo will attempt to resolve the fabric
address to the given node.  If node is not provided, fi_getinfo will
attempt to resolve the fabric addressing information based on the
provided hints.  The caller must call fi_freeinfo to release fi_info
structures returned by fi_getinfo.

If neither node, service or hints are provided, then fi_getinfo simply
returns the list all available communication interfaces.

Multiple threads may call
`fi_getinfo` simultaneously, without any requirement for serialization.

# SEE ALSO

[`fi_open`(3)](fi_open.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_nic`(3)](fi_nic.3.html)
