---
layout: page
title: fi_psm3(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_psm3 \- The PSM3 Fabric Provider

# OVERVIEW

The *psm3* provider implements a Performance Scaled Messaging
capability which supports Intel RoCEv2 capable NICs. PSM3 represents
an Ethernet and standard RoCEv2 enhancement of previous PSM
implementations.

# SUPPORTED FEATURES

The *psm3* provider supports a subset of all the features defined in the
libfabric API.

Endpoint types
: Supports non-connection based types *FI_DGRAM* and *FI_RDM*.

Endpoint capabilities
: Endpoints can support any combination of data transfer capabilities
  *FI_TAGGED*, *FI_MSG*, *FI_ATOMICS*, and *FI_RMA*. These capabilities
  can be further refined by *FI_SEND*, *FI_RECV*, *FI_READ*, *FI_WRITE*,
  *FI_REMOTE_READ*, and *FI_REMOTE_WRITE* to limit the direction of
  operations.

  *FI_MULTI_RECV* is supported for non-tagged message queue only.

  Scalable endpoints are supported if the underlying PSM3 library supports
  multiple endpoints. This condition must be satisfied both when the
  provider is built and when the provider is used. See the *Scalable
  endpoints* section for more information.

  Other supported capabilities include *FI_TRIGGER*, *FI_REMOTE_CQ_DATA*,
  *FI_RMA_EVENT*, *FI_SOURCE*, and *FI_SOURCE_ERR*. Furthermore,
  *FI_NAMED_RX_CTX* is supported when scalable endpoints are enabled.

Modes
: *FI_CONTEXT* is required for the *FI_TAGGED* and *FI_MSG*
  capabilities. That means, any request belonging to these two
  categories that generates a completion must pass as the operation
  context a valid pointer to type *struct fi_context*, and the space
  referenced by the pointer must remain untouched until the request
  has completed. If none of *FI_TAGGED* and *FI_MSG* is asked for,
  the *FI_CONTEXT* mode is not required.

Progress
: The *psm3* provider performs optimal with manual progress. By default, the
  application is expected to call *fi_cq_read* or *fi_cntr_read* function
  from time to time when no other libfabric function is called to ensure
  progress is made in a timely manner. The provider does support auto
  progress mode. However, the performance can be significantly impacted if
  the application purely depends on the provider to make auto progress.

Scalable endpoints
: Scalable endpoints support depends on the multi-EP feature of the *PSM3*
  library. If the *PSM3* library supports this feature, the availability is
  further controlled by an environment variable *PSM3_MULTI_EP*. The *psm3*
  provider automatically sets this variable to 1 if it is not set. The
  feature can be disabled explicitly by setting *PSM3_MULTI_EP* to 0.

  When creating a scalable endpoint, the exact number of contexts requested
  should be set in the "fi_info" structure passed to the *fi_scalable_ep*
  function. This number should be set in "fi_info->ep_attr->tx_ctx_cnt" or
  "fi_info->ep_attr->rx_ctx_cnt" or both, whichever greater is used. The
  *psm3* provider allocates all requested contexts upfront when the scalable
  endpoint is created. The same context is used for both Tx and Rx.

  For optimal performance, it is advised to avoid having multiple threads
  accessing the same context, either directly by posting send/recv/read/write
  request, or indirectly by polling associated completion queues or counters.

  Using the scalable endpoint as a whole in communication functions is not
  supported. Instead, individual tx context or rx context of the scalable
  endpoint should be used. Similarly, using the address of the scalable
  endpoint as the source address or destination address doesn't collectively
  address all the tx/rx contexts. It addresses only the first tx/rx context,
  instead.

# LIMITATIONS

The *psm3* provider doesn't support all the features defined in the
libfabric API. Here are some of the limitations not listed above:

Unsupported features
: These features are unsupported: connection management, passive endpoint,
  and shared receive context.

# RUNTIME PARAMETERS

The *psm3* provider checks for the following environment variables:

*FI_PSM3_UUID*
: PSM requires that each job has a unique ID (UUID). All the processes
  in the same job need to use the same UUID in order to be able to
  talk to each other. The PSM reference manual advises to keep UUID
  unique to each job. In practice, it generally works fine to reuse
  UUID as long as (1) no two jobs with the same UUID are running at
  the same time; and (2) previous jobs with the same UUID have exited
  normally. If running into "resource busy" or "connection failure"
  issues with unknown reason, it is advisable to manually set the UUID
  to a value different from the default.

  The default UUID is 00FF00FF-0000-0000-0000-00FF0F0F00FF.

  It is possible to create endpoints with UUID different from the one
  set here. To achieve that, set 'info->ep_attr->auth_key' to the uuid
  value and 'info->ep_attr->auth_key_size' to its size (16 bytes) when
  calling fi_endpoint() or fi_scalable_ep(). It is still true that an
  endpoint can only communicate with endpoints with the same UUID.

*FI_PSM3_NAME_SERVER*
: The *psm3* provider has a simple built-in name server that can be used
  to resolve an IP address or host name into a transport address needed
  by the *fi_av_insert* call. The main purpose of this name server is to
  allow simple client-server type applications (such as those in *fabtests*)
  to be written purely with libfabric, without using any out-of-band
  communication mechanism. For such applications, the server would run first
  to allow endpoints be created and registered with the name server, and
  then the client would call *fi_getinfo* with the *node* parameter set to
  the IP address or host name of the server. The resulting *fi_info*
  structure would have the transport address of the endpoint created by the
  server in the *dest_addr* field. Optionally the *service* parameter can
  be used in addition to *node*. Notice that the *service* number is
  interpreted by the provider and is not a TCP/IP port number.

  The name server is on by default. It can be turned off by setting the
  variable to 0. This may save a small amount of resource since a separate
  thread is created when the name server is on.

  The provider detects OpenMPI and MPICH runs and changes the default setting
  to off.

*FI_PSM3_TAGGED_RMA*
: The RMA functions are implemented on top of the PSM Active Message functions.
  The Active Message functions have limit on the size of data can be transferred
  in a single message. Large transfers can be divided into small chunks and
  be pipe-lined. However, the bandwidth is sub-optimal by doing this way.

  The *psm3* provider use PSM tag-matching message queue functions to achieve
  higher bandwidth for large size RMA. It takes advantage of the extra tag bits
  available in PSM3 to separate the RMA traffic from the regular tagged message
  queue.

  The option is on by default. To turn it off set the variable to 0.

*FI_PSM3_DELAY*
: Time (seconds) to sleep before closing PSM endpoints. This is a workaround
  for a bug in some versions of PSM library.

  The default setting is 0.

*FI_PSM3_TIMEOUT*
: Timeout (seconds) for gracefully closing PSM endpoints. A forced closing
  will be issued if timeout expires.

  The default setting is 5.

*FI_PSM3_CONN_TIMEOUT*
: Timeout (seconds) for establishing connection between two PSM endpoints.

  The default setting is 5.

*FI_PSM3_PROG_INTERVAL*
: When auto progress is enabled (asked via the hints to *fi_getinfo*),
  a progress thread is created to make progress calls from time to time.
  This option set the interval (microseconds) between progress calls.

  The default setting is 1 if affinity is set, or 1000 if not. See
  *FI_PSM3_PROG_AFFINITY*.

*FI_PSM3_PROG_AFFINITY*
: When set, specify the set of CPU cores to set the progress thread
  affinity to. The format is
  `<start>[:<end>[:<stride>]][,<start>[:<end>[:<stride>]]]*`,
  where each triplet `<start>:<end>:<stride>` defines a block of
  core_ids. Both `<start>` and `<end>` can be either the `core_id`
  (when >=0) or `core_id - num_cores` (when <0).

  By default affinity is not set.

*FI_PSM3_INJECT_SIZE*
: Maximum message size allowed for fi_inject and fi_tinject calls. This is
  an experimental feature to allow some applications to override default
  inject size limitation. When the inject size is larger than the default
  value, some inject calls might block.

  The default setting is 64.

*FI_PSM3_LOCK_LEVEL*
: When set, dictate the level of locking being used by the provider. Level
  2 means all locks are enabled. Level 1 disables some locks and is suitable
  for runs that limit the access to each PSM3 context to a single thread.
  Level 0 disables all locks and thus is only suitable for single threaded
  runs.

  To use level 0 or level 1, wait object and auto progress mode cannot be
  used because they introduce internal threads that may break the conditions
  needed for these levels.

  The default setting is 2.

*FI_PSM3_LAZY_CONN*
: There are two strategies on when to establish connections between the PSM3
  endpoints that OFI endpoints are built on top of. In eager connection mode,
  connections are established when addresses are inserted into the address
  vector. In lazy connection mode, connections are established when addresses
  are used the first time in communication. Eager connection mode has slightly
  lower critical path overhead but lazy connection mode scales better.

  This option controls how the two connection modes are used. When set to 1,
  lazy connection mode is always used. When set to 0, eager connection mode
  is used when required conditions are all met and lazy connection mode is
  used otherwise. The conditions for eager connection mode are: (1) multiple
  endpoint (and scalable endpoint) support is disabled by explicitly setting
  PSM3_MULTI_EP=0; and (2) the address vector type is FI_AV_MAP.

  The default setting is 0.

*FI_PSM3_DISCONNECT*
: The provider has a mechanism to automatically send disconnection notifications
  to all connected peers before the local endpoint is closed. As the response,
  the peers call *psm3_ep_disconnect* to clean up the connection state at their
  side. This allows the same PSM3 epid be used by different dynamically started
  processes (clients) to communicate with the same peer (server). This mechanism,
  however, introduce extra overhead to the finalization phase. For applications
  that never reuse epids within the same session such overhead is unnecessary.

  This option controls whether the automatic disconnection notification mechanism
  should be enabled. For client-server application mentioned above, the client
  side should set this option to 1, but the server should set it to 0.

  The default setting is 0.

*FI_PSM3_TAG_LAYOUT*
: Select how the 96-bit PSM3 tag bits are organized. Currently three choices are
  available: *tag60* means 32-4-60 partitioning for CQ data, internal protocol
  flags, and application tag. *tag64* means 4-28-64 partitioning for internal
  protocol flags, CQ data, and application tag. *auto* means to choose either
  *tag60* or *tag64* based on the hints passed to fi_getinfo -- *tag60* is used
  if remote CQ data support is requested explicitly, either by passing non-zero value
  via *hints->domain_attr->cq_data_size* or by including *FI_REMOTE_CQ_DATA* in
  *hints->caps*, otherwise *tag64* is used. If *tag64* is the result of automatic
  selection, *fi_getinfo* also returns a second instance of the provider with
  *tag60* layout.

  The default setting is *auto*.

  Notice that if the provider is compiled with macro *PSMX3_TAG_LAYOUT* defined
  to 1 (means *tag60*) or 2 (means *tag64*), the choice is fixed at compile time
  and this runtime option will be disabled.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_psm`(7)](fi_psm.7.html),
[`fi_psm2`(7)](fi_psm2.7.html),
