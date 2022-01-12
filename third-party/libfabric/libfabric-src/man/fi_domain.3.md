---
layout: page
title: fi_domain(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_domain \- Open a fabric access domain

# SYNOPSIS

```c
#include <rdma/fabric.h>

#include <rdma/fi_domain.h>

int fi_domain(struct fid_fabric *fabric, struct fi_info *info,
    struct fid_domain **domain, void *context);

int fi_close(struct fid *domain);

int fi_domain_bind(struct fid_domain *domain, struct fid *eq,
    uint64_t flags);

int fi_open_ops(struct fid *domain, const char *name, uint64_t flags,
    void **ops, void *context);

int fi_set_ops(struct fid *domain, const char *name, uint64_t flags,
    void *ops, void *context);
```

# ARGUMENTS

*fabric*
: Fabric domain

*info*
: Fabric information, including domain capabilities and attributes.

*domain*
: An opened access domain.

*context*
: User specified context associated with the domain.  This context is
  returned as part of any asynchronous event associated with the
  domain.

*eq*
: Event queue for asynchronous operations initiated on the domain.

*name*
: Name associated with an interface.

*ops*
: Fabric interface operations.

# DESCRIPTION

An access domain typically refers to a physical or virtual NIC or
hardware port; however, a domain may span across multiple hardware
components for fail-over or data striping purposes.  A domain defines
the boundary for associating different resources together.  Fabric
resources belonging to the same domain may share resources.

## fi_domain

Opens a fabric access domain, also referred to as a resource domain.
Fabric domains are identified by a name.  The properties of the opened
domain are specified using the info parameter.

## fi_open_ops

fi_open_ops is used to open provider specific interfaces.  Provider
interfaces may be used to access low-level resources and operations
that are specific to the opened resource domain.  The details of
domain interfaces are outside the scope of this documentation.

## fi_set_ops

fi_set_ops assigns callbacks that a provider should invoke in place
of performing selected tasks. This allows users to modify or control
a provider's default behavior. Conceptually, it allows the user to
hook specific functions used by a provider and replace it with their
own.

The operations being modified are identified using a well-known
character string, passed as the name parameter. The format of the
ops parameter is dependent upon the name value. The ops parameter will
reference a structure containing the callbacks and other fields needed
by the provider to invoke the user's functions.

If a provider accepts the override, it will return FI_SUCCESS. If the
override is unknown or not supported, the provider will return
-FI_ENOSYS. Overrides should be set prior to allocating resources on
the domain.

The following fi_set_ops operations and corresponding callback
structures are defined.

**FI_SET_OPS_HMEM_OVERRIDE -- Heterogeneous Memory Overrides**

HMEM override allows users to override HMEM related operations a
provider may perform. Currently, the scope of the HMEM override
is to allow a user to define the memory movement functions a provider
should use when accessing a user buffer. The user-defined memory
movement functions need to account for all the different HMEM iface
types a provider may encounter.

All objects allocated against a domain will inherit this override.

The following is the HMEM override operation name and structure.

```c
#define FI_SET_OPS_HMEM_OVERRIDE "hmem_override_ops"

struct fi_hmem_override_ops {
    size_t	size;

    ssize_t (*copy_from_hmem_iov)(void *dest, size_t size,
        enum fi_hmem_iface iface, uint64_t device, const struct iovec *hmem_iov,
        size_t hmem_iov_count, uint64_t hmem_iov_offset);

    ssize_t (*copy_to_hmem_iov)(enum fi_hmem_iface iface, uint64_t device,
	const struct iovec *hmem_iov, size_t hmem_iov_count,
        uint64_t hmem_iov_offset, const void *src, size_t size);
};
```

All fields in struct fi_hmem_override_ops must be set (non-null) to a
valid value.

*size*
: This should be set to the sizeof(struct fi_hmem_override_ops). The
size field is used for forward and backward compatibility purposes.

*copy_from_hmem_iov*
: Copy data from the device/hmem to host memory. This function should
return a negative fi_errno on error, or the number of bytes copied on
success.

*copy_to_hmem_iov*
: Copy data from host memory to the device/hmem. This function should
return a negative fi_errno on error, or the number of bytes copied on
success.

## fi_domain_bind

Associates an event queue with the domain.  An event queue bound to a
domain will be the default EQ associated with asynchronous control
events that occur on the domain or active endpoints allocated on a
domain.  This includes CM events.  Endpoints may direct their control
events to alternate EQs by binding directly with the EQ.

Binding an event queue to a domain with the FI_REG_MR flag indicates
that the provider should perform all memory registration operations
asynchronously, with the completion reported through the event queue.
If an event queue is not bound to the domain with the FI_REG_MR flag,
then memory registration requests complete synchronously.

See [`fi_av_bind`(3)](fi_av_bind.3.html),
[`fi_ep_bind`(3)](fi_ep_bind.3.html),
[`fi_mr_bind`(3)](fi_mr_bind.3.html),
[`fi_pep_bind`(3)](fi_pep_bind.3.html), and
[`fi_scalable_ep_bind`(3)](fi_scalable_ep_bind.3.html) for more
information.

## fi_close

The fi_close call is used to release all resources associated with a domain or
interface.  All objects associated with the opened domain must be released
prior to calling fi_close, otherwise the call will return -FI_EBUSY.

# DOMAIN ATTRIBUTES

The `fi_domain_attr` structure defines the set of attributes associated
with a domain.

```c
struct fi_domain_attr {
	struct fid_domain     *domain;
	char                  *name;
	enum fi_threading     threading;
	enum fi_progress      control_progress;
	enum fi_progress      data_progress;
	enum fi_resource_mgmt resource_mgmt;
	enum fi_av_type       av_type;
	int                   mr_mode;
	size_t                mr_key_size;
	size_t                cq_data_size;
	size_t                cq_cnt;
	size_t                ep_cnt;
	size_t                tx_ctx_cnt;
	size_t                rx_ctx_cnt;
	size_t                max_ep_tx_ctx;
	size_t                max_ep_rx_ctx;
	size_t                max_ep_stx_ctx;
	size_t                max_ep_srx_ctx;
	size_t                cntr_cnt;
	size_t                mr_iov_limit;
	uint64_t              caps;
	uint64_t              mode;
	uint8_t               *auth_key;
	size_t                auth_key_size;
	size_t                max_err_data;
	size_t                mr_cnt;
	uint32_t              tclass;
};
```

## domain

On input to fi_getinfo, a user may set this to an opened domain
instance to restrict output to the given domain.  On output from
fi_getinfo, if no domain was specified, but the user has an opened
instance of the named domain, this will reference the first opened
instance.  If no instance has been opened, this field will be NULL.

The domain instance returned by fi_getinfo should only be considered
valid if the application does not close any domain instances from
another thread while fi_getinfo is being processed.

## Name

The name of the access domain.

## Multi-threading Support (threading)

The threading model specifies the level of serialization required of
an application when using the libfabric data transfer interfaces.
Control interfaces are always considered thread safe, and may be
accessed by multiple threads.  Applications which can guarantee
serialization in their access of provider allocated resources and
interfaces enables a provider to eliminate lower-level locks.

*FI_THREAD_COMPLETION*
: The completion threading model is intended for providers that make use
  of manual progress.  Applications must serialize access to all objects
  that are associated through the use of having a shared completion
  structure.  This includes endpoint, transmit context, receive context,
  completion queue, counter, wait set, and poll set objects.

  For example, threads must serialize access to an endpoint and its
  bound completion queue(s) and/or counters.  Access to endpoints that
  share the same completion queue must also be serialized.

  The use of FI_THREAD_COMPLETION can increase parallelism over
  FI_THREAD_SAFE, but requires the use of isolated resources.

*FI_THREAD_DOMAIN*
: A domain serialization model requires applications to serialize
  access to all objects belonging to a domain.

*FI_THREAD_ENDPOINT*
: The endpoint threading model is similar to FI_THREAD_FID, but with
  the added restriction that serialization is required when accessing
  the same endpoint, even if multiple transmit and receive contexts are
  used.  Conceptually, FI_THREAD_ENDPOINT maps well to providers that
  implement fabric services in hardware but use a single command
  queue to access different data flows.

*FI_THREAD_FID*
: A fabric descriptor (FID) serialization model requires applications
  to serialize access to individual fabric resources associated with
  data transfer operations and completions.  Multiple threads must
  be serialized when accessing the same endpoint, transmit context,
  receive context, completion queue, counter, wait set, or poll set.
  Serialization is required only by threads accessing the same object.

  For example, one thread may be initiating a data transfer on an
  endpoint, while another thread reads from a completion queue
  associated with the endpoint.

  Serialization to endpoint access is only required when accessing
  the same endpoint data flow.  Multiple threads may initiate transfers
  on different transmit contexts of the same endpoint without serializing,
  and no serialization is required between the submission of data
  transmit requests and data receive operations.

  In general, FI_THREAD_FID allows the provider to be implemented
  without needing internal locking when handling data transfers.
  Conceptually, FI_THREAD_FID maps well to providers that implement
  fabric services in hardware and provide separate command queues to
  different data flows.

*FI_THREAD_SAFE*
: A thread safe serialization model allows a multi-threaded
  application to access any allocated resources through any interface
  without restriction.  All providers are required to support
  FI_THREAD_SAFE.

*FI_THREAD_UNSPEC*
: This value indicates that no threading model has been defined.  It
  may be used on input hints to the fi_getinfo call.  When specified,
  providers will return a threading model that allows for the greatest
  level of parallelism.

## Progress Models (control_progress / data_progress)

Progress is the ability of the underlying implementation to complete
processing of an asynchronous request.  In many cases, the processing
of an asynchronous request requires the use of the host processor.
For example, a received message may need to be matched with the
correct buffer, or a timed out request may need to be retransmitted.
For performance reasons, it may be undesirable for the provider to
allocate a thread for this purpose, which will compete with the
application threads.

Control progress indicates the method that the provider uses to make
progress on asynchronous control operations.  Control operations are
functions which do not directly involve the transfer of application
data between endpoints.  They include address vector, memory
registration, and connection management routines.

Data progress indicates the method that the provider uses to make
progress on data transfer operations.  This includes message queue,
RMA, tagged messaging, and atomic operations, along with their
completion processing.

Progress frequently requires action being taken at both the transmitting
and receiving sides of an operation.  This is often a requirement for
reliable transfers, as a result of retry and acknowledgement processing.

To balance between performance and ease of use, two progress models
are defined.

*FI_PROGRESS_AUTO*
: This progress model indicates that the provider will make forward
  progress on an asynchronous operation without further intervention
  by the application.  When FI_PROGRESS_AUTO is provided as output to
  fi_getinfo in the absence of any progress hints, it often indicates
  that the desired functionality is implemented by the provider
  hardware or is a standard service of the operating system.

  All providers are required to support FI_PROGRESS_AUTO.  However, if
  a provider does not natively support automatic progress, forcing the
  use of FI_PROGRESS_AUTO may result in threads being allocated below
  the fabric interfaces.

*FI_PROGRESS_MANUAL*
: This progress model indicates that the provider requires the use of
  an application thread to complete an asynchronous request.  When
  manual progress is set, the provider will attempt to advance an
  asynchronous operation forward when the application attempts to
  wait on or read an event queue, completion queue, or counter
  where the completed operation will be reported.  Progress also
  occurs when the application processes a poll or wait set that
  has been associated with the event or completion queue.

  Only wait operations defined by the fabric interface will result in
  an operation progressing.  Operating system or external wait
  functions, such as select, poll, or pthread routines, cannot.

  Manual progress requirements not only apply to endpoints that initiate
  transmit operations, but also to endpoints that may be the target of
  such operations.  This holds true even if the target endpoint will not
  generate completion events for the operations.  For example, an endpoint
  that acts purely as the target of RMA or atomic operations that uses
  manual progress may still need application assistance to process
  received operations.

*FI_PROGRESS_UNSPEC*
: This value indicates that no progress model has been defined.  It
  may be used on input hints to the fi_getinfo call.

## Resource Management (resource_mgmt)

Resource management (RM) is provider and protocol support to protect
against overrunning local and remote resources.  This includes
local and remote transmit contexts, receive contexts, completion
queues, and source and target data buffers.

When enabled, applications are given some level of protection against
overrunning provider queues and local and remote data buffers.  Such
support may be built directly into the hardware and/or network
protocol, but may also require that checks be enabled in the provider
software.  By disabling resource management, an application assumes
all responsibility for preventing queue and buffer overruns, but doing
so may allow a provider to eliminate internal synchronization calls,
such as atomic variables or locks.

It should be noted that even if resource management is disabled, the
provider implementation and protocol may still provide some level of
protection against overruns.  However, such protection is not guaranteed.
The following values for resource management are defined.

*FI_RM_DISABLED*
: The provider is free to select an implementation and protocol that does
  not protect against resource overruns.  The application is responsible
  for resource protection.

*FI_RM_ENABLED*
: Resource management is enabled for this provider domain.

*FI_RM_UNSPEC*
: This value indicates that no resource management model has been defined.
  It may be used on input hints to the fi_getinfo call.

The behavior of the various resource management options depends on whether
the endpoint is reliable or unreliable, as well as provider and protocol
specific implementation details, as shown in the following table.  The
table assumes that all peers enable or disable RM the same.

| Resource | DGRAM EP-no RM | DGRAM EP-with RM | RDM/MSG EP-no RM | RDM/MSG EP-with RM |
|:--------:|:-------------------:|:-------------------:|:------------------:|:-----------------:|
| Tx Ctx         | undefined error  | EAGAIN           | undefined error   | EAGAIN             |
| Rx Ctx         | undefined error  | EAGAIN           | undefined error   | EAGAIN             |
| Tx CQ          | undefined error  | EAGAIN           | undefined error   | EAGAIN             |
| Rx CQ          | undefined error  | EAGAIN           | undefined error   | EAGAIN             |
| Target EP      | dropped          | dropped          | transmit error    | retried            |
| No Rx Buffer   | dropped          | dropped          | transmit error    | retried            |
| Rx Buf Overrun | truncate or drop | truncate or drop | truncate or error | truncate or error  |
| Unmatched RMA  | not applicable   | not applicable   | transmit error    | transmit error     |
| RMA Overrun    | not applicable   | not applicable   | transmit error    | transmit error     |

The resource column indicates the resource being accessed by a data
transfer operation.

*Tx Ctx / Rx Ctx*
: Refers to the transmit/receive contexts when a data transfer operation
  is submitted.  When RM is enabled, attempting to submit a request will fail if
  the context is full.  If RM is disabled, an undefined error (provider specific)
  will occur.  Such errors should be considered fatal to the context,
  and applications must take steps to avoid queue overruns.

*Tx CQ / Rx CQ*
: Refers to the completion queue associated with the Tx or Rx context when
  a local operation completes.  When RM is disabled, applications must take
  care to ensure that completion queues do not get overrun.  When an overrun
  occurs, an undefined, but fatal, error will occur affecting all endpoints
  associated with the CQ.  Overruns can be avoided by sizing the CQs
  appropriately or by deferring the posting of a data transfer operation unless
  CQ space is available to store its completion.  When RM is enabled, providers
  may use different mechanisms to prevent CQ overruns.  This includes
  failing (returning -FI_EAGAIN) the posting of operations that could
  result in CQ overruns, or internally retrying requests (which will be hidden
  from the application).  See notes at the end of this section regarding
  CQ resource management restrictions.

*Target EP / No Rx Buffer*
: Target EP refers to resources associated with the endpoint that is the target
  of a transmit operation.  This includes the target endpoint's receive queue,
  posted receive buffers (no Rx buffers), the receive side completion queue,
  and other related packet processing queues.  The defined behavior is that
  seen by the initiator of a request.  For FI_EP_DGRAM endpoints, if the target EP
  queues are unable to accept incoming messages, received messages will
  be dropped.  For reliable endpoints, if RM is disabled, the transmit
  operation will complete in error.  If RM is enabled, the provider will
  internally retry the operation.

*Rx Buffer Overrun*
: This refers to buffers posted to receive incoming tagged or untagged messages,
  with the behavior defined from the viewpoint of the sender.  The behavior
  for handling received messages that are larger than the buffers provided by
  the application is provider specific.  Providers may either truncate the
  message and report a successful completion, or fail the operation.  For
  datagram endpoints, failed sends will result in the message being dropped.
  For reliable endpoints, send operations may complete successfully,
  yet be truncated at the receive side.  This can occur when the target side
  buffers received data until an application buffer is made available.
  The completion status may also be dependent upon the completion model selected
  byt the application (e.g. FI_DELIVERY_COMPLETE versus FI_TRANSMIT_COMPLETE).

*Unmatched RMA / RMA Overrun*
: Unmatched RMA and RMA overruns deal with the processing of RMA and
  atomic operations.  Unlike send operations, RMA operations that attempt
  to access a memory address that is either not registered for such
  operations, or attempt to access outside of the target memory region
  will fail, resulting in a transmit error.

When a resource management error occurs on an endpoint, the endpoint is
transitioned into a disabled state.  Any operations which have not
already completed will fail and be discarded.  For connectionless endpoints,
the endpoint must be re-enabled before it will accept new data transfer
operations.  For connected endpoints, the connection is torn down and
must be re-established.

There is one notable restriction on the protections offered by resource
management.  This occurs when resource management is enabled on an
endpoint that has been bound to completion queue(s) using the
FI_SELECTIVE_COMPLETION flag.  Operations posted to such an endpoint
may specify that a successful completion should not generate a entry
on the corresponding completion queue.  (I.e. the operation leaves the
FI_COMPLETION flag unset).  In such situations, the provider is not
required to reserve an entry in the completion queue to handle the
case where the operation fails and does generate a CQ entry, which
would effectively require tracking the operation to completion.
Applications concerned with avoiding CQ overruns in the occurrence
of errors must ensure that there is sufficient space in the CQ to
report failed operations.  This can typically be achieved by sizing
the CQ to at least the same size as the endpoint queue(s) that are
bound to it.

## AV Type (av_type)

Specifies the type of address vectors that are usable with this domain.
For additional details on AV type, see [`fi_av`(3)](fi_av.3.html).
The following values may be specified.

*FI_AV_MAP*
: Only address vectors of type AV map are requested or supported.

*FI_AV_TABLE*
: Only address vectors of type AV index are requested or supported.

*FI_AV_UNSPEC*
: Any address vector format is requested and supported.

Address vectors are only used by connectionless endpoints.  Applications
that require the use of a specific type of address vector should set the
domain attribute av_type to the necessary value when calling fi_getinfo.
The value FI_AV_UNSPEC may be used to indicate that the provider can support
either address vector format.  In this case, a provider may return
FI_AV_UNSPEC to indicate that either format is supportable, or may return
another AV type to indicate the optimal AV type supported by this domain.

## Memory Registration Mode (mr_mode)

Defines memory registration specific mode bits used with this domain.
Full details on MR mode options are available in [`fi_mr`(3)](fi_mr.3.html).
The following values may be specified.

*FI_MR_ALLOCATED*
: Indicates that memory registration occurs on allocated data buffers, and
  physical pages must back all virtual addresses being registered.

*FI_MR_ENDPOINT*
: Memory registration occurs at the endpoint level, rather than domain.

*FI_MR_LOCAL*
: The provider is optimized around having applications register memory
  for locally accessed data buffers.  Data buffers used in send and
  receive operations and as the source buffer for RMA and atomic
  operations must be registered by the application for access domains
  opened with this capability.

*FI_MR_MMU_NOTIFY*
: Indicates that the application is responsible for notifying the provider
  when the page tables referencing a registered memory region may have been
  updated.

*FI_MR_PROV_KEY*
: Memory registration keys are selected and returned by the provider.

*FI_MR_RAW*
: The provider requires additional setup as part of their memory registration
  process.  This mode is required by providers that use a memory key
  that is larger than 64-bits.

*FI_MR_RMA_EVENT*
: Indicates that the memory regions associated with completion counters
  must be explicitly enabled after being bound to any counter.

*FI_MR_UNSPEC*
: Defined for compatibility -- library versions 1.4 and earlier.  Setting
  mr_mode to 0 indicates that FI_MR_BASIC or FI_MR_SCALABLE are requested
  and supported.

*FI_MR_VIRT_ADDR*
: Registered memory regions are referenced by peers using the virtual address
  of the registered memory region, rather than a 0-based offset.

*FI_MR_BASIC*
: Defined for compatibility -- library versions 1.4 and earlier.  Only
  basic memory registration operations are requested or supported.
  This mode is equivalent to the FI_MR_VIRT_ADDR, FI_MR_ALLOCATED, and
  FI_MR_PROV_KEY flags being set in later library versions.  This flag
  may not be used in conjunction with other mr_mode bits.

*FI_MR_SCALABLE*
: Defined for compatibility -- library versions 1.4 and earlier.
  Only scalable memory registration operations
  are requested or supported.  Scalable registration uses offset based
  addressing, with application selectable memory keys.  For library versions
  1.5 and later, this is the default if no mr_mode bits are set.  This
  flag may not be used in conjunction with other mr_mode bits.

Buffers used in data transfer operations may require notifying the provider
of their use before a data transfer can occur.  The mr_mode field indicates
the type of memory registration that is required, and when registration is
necessary.  Applications that require the use of a specific registration mode
should set the domain attribute mr_mode to the necessary value when calling
fi_getinfo.  The value FI_MR_UNSPEC may be used to indicate support for any
registration mode.

## MR Key Size (mr_key_size)

Size of the memory region remote access key, in bytes.  Applications
that request their own MR key must select a value within the range
specified by this value.  Key sizes larger than 8 bytes require using the
FI_RAW_KEY mode bit.

## CQ Data Size (cq_data_size)

Applications may include a small message with a data transfer that
is placed directly into a remote completion queue as part of a completion
event.  This is referred to as remote CQ data (sometimes referred to
as immediate data).  This field indicates the number of bytes that
the provider supports for remote CQ data.  If supported (non-zero
value is returned), the minimum size of remote CQ data must be
at least 4-bytes.

## Completion Queue Count (cq_cnt)

The optimal number of completion queues supported by the domain, relative
to any specified or default CQ attributes.  The cq_cnt value may be a
fixed value of the maximum number of CQs supported by the
underlying hardware, or may be a dynamic value, based on the default
attributes of an allocated CQ, such as the CQ size and data format.

## Endpoint Count (ep_cnt)

The total number of endpoints supported by the domain, relative to any
specified or default endpoint attributes.  The ep_cnt value may be a
fixed value of the maximum number of endpoints supported by the
underlying hardware, or may be a dynamic value, based on the default
attributes of an allocated endpoint, such as the endpoint capabilities
and size.  The endpoint count is the number of addressable endpoints
supported by the provider. Providers return capability limits based on
configured hardware maximum capabilities. Providers cannot predict all
possible system limitations without posteriori knowledge acquired during
runtime that will further limit these hardware maximums (e.g. application
memory consumption, FD usage, etc.).

## Transmit Context Count (tx_ctx_cnt)

The number of outbound command queues optimally supported by the
provider.  For a low-level provider, this represents the number of
command queues to the hardware and/or the number of parallel transmit
engines effectively supported by the hardware and caches.
Applications which allocate more transmit contexts than this value
will end up sharing underlying resources.  By default, there is a
single transmit context associated with each endpoint, but in an
advanced usage model, an endpoint may be configured with multiple
transmit contexts.

## Receive Context Count (rx_ctx_cnt)

The number of inbound processing queues optimally supported by the
provider.  For a low-level provider, this represents the number
hardware queues that can be effectively utilized for processing
incoming packets.  Applications which allocate more receive contexts
than this value will end up sharing underlying resources.  By default,
a single receive context is associated with each endpoint, but in an
advanced usage model, an endpoint may be configured with multiple
receive contexts.

## Maximum Endpoint Transmit Context (max_ep_tx_ctx)

The maximum number of transmit contexts that may be associated with an
endpoint.

## Maximum Endpoint Receive Context (max_ep_rx_ctx)

The maximum number of receive contexts that may be associated with an
endpoint.

## Maximum Sharing of Transmit Context (max_ep_stx_ctx)

The maximum number of endpoints that may be associated with a
shared transmit context.

## Maximum Sharing of Receive Context (max_ep_srx_ctx)

The maximum number of endpoints that may be associated with a
shared receive context.

## Counter Count (cntr_cnt)

The optimal number of completion counters supported by the domain.
The cq_cnt value may be a fixed value of the maximum number of counters
supported by the underlying hardware, or may be a dynamic value, based on
the default attributes of the domain.

## MR IOV Limit (mr_iov_limit)

This is the maximum number of IO vectors (scatter-gather elements)
that a single memory registration operation may reference.

## Capabilities (caps)

Domain level capabilities.  Domain capabilities indicate domain
level features that are supported by the provider.

*FI_LOCAL_COMM*
: At a conceptual level, this field indicates that the underlying device
  supports loopback communication.  More specifically, this field
  indicates that an endpoint may communicate with other endpoints that
  are allocated from the same underlying named domain.  If this field
  is not set, an application may need to use an alternate domain or
  mechanism (e.g. shared memory) to communicate with peers that execute
  on the same node.

*FI_REMOTE_COMM*
: This field indicates that the underlying provider supports communication
  with nodes that are reachable over the network.  If this field is not set,
  then the provider only supports communication between processes that
  execute on the same node -- a shared memory provider, for example.

*FI_SHARED_AV*
: Indicates that the domain supports the ability to share address
  vectors among multiple processes using the named address vector
  feature.

See [`fi_getinfo`(3)](fi_getinfo.3.html) for a discussion on primary versus
secondary capabilities.  All domain capabilities are considered secondary
capabilities.

## mode

The operational mode bit related to using the domain.

*FI_RESTRICTED_COMP*
: This bit indicates that the domain limits completion queues and counters
  to only be used with endpoints, transmit contexts, and receive contexts that
  have the same set of capability flags.

## Default authorization key (auth_key)

The default authorization key to associate with endpoint and memory
registrations created within the domain. This field is ignored unless the
fabric is opened with API version 1.5 or greater.

## Default authorization key length (auth_key_size)

The length in bytes of the default authorization key for the domain. If set to 0,
then no authorization key will be associated with endpoints and memory
registrations created within the domain unless specified in the endpoint or
memory registration attributes. This field is ignored unless the fabric is
opened with API version 1.5 or greater.

## Max Error Data Size (max_err_data)
: The maximum amount of error data, in bytes, that may be returned as part of
  a completion or event queue error.  This value corresponds to the
  err_data_size field in struct fi_cq_err_entry and struct fi_eq_err_entry.

## Memory Regions Count (mr_cnt)

The optimal number of memory regions supported by the domain, or endpoint if
the mr_mode FI_MR_ENDPOINT bit has been set.  The mr_cnt
value may be a fixed value of the maximum number of MRs supported by the
underlying hardware, or may be a dynamic value, based on the default
attributes of the domain, such as the supported memory registration modes.
Applications can set the mr_cnt on input to fi_getinfo, in order to
indicate their memory registration requirements.  Doing so may allow the
provider to optimize any memory registration cache or lookup tables.

## Traffic Class (tclass)

This specifies the default traffic class that will be associated any endpoints
created within the domain.  See [`fi_endpoint`(3)](fi_endpoint.3.html
for additional information.

# RETURN VALUE

Returns 0 on success. On error, a negative value corresponding to fabric
errno is returned. Fabric errno values are defined in
`rdma/fi_errno.h`.

# NOTES

Users should call fi_close to release all resources allocated to the
fabric domain.

The following fabric resources are associated with domains:
active endpoints, memory regions, completion event queues, and address
vectors.

Domain attributes reflect the limitations and capabilities of the
underlying hardware and/or software provider.  They do not reflect
system limitations, such as the number of physical pages that an
application may pin or number of file descriptors that the
application may open.  As a result, the reported maximums may not be
achievable, even on a lightly loaded systems, without an
administrator configuring system resources appropriately for the
installed provider(s).

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_av`(3)](fi_av.3.html),
[`fi_ep`(3)](fi_ep.3.html),
[`fi_eq`(3)](fi_eq.3.html),
[`fi_mr`(3)](fi_mr.3.html)
