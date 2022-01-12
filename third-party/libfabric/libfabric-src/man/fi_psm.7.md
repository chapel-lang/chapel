---
layout: page
title: fi_psm(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_psm \- The PSM Fabric Provider

# OVERVIEW

The *psm* provider runs over the PSM 1.x interface that is currently
supported by the Intel TrueScale Fabric. PSM provides tag-matching
message queue functions that are optimized for MPI implementations.
PSM also has limited Active Message support, which is not officially
published but is quite stable and well documented in the source code
(part of the OFED release). The *psm* provider makes use of both the
tag-matching message queue functions and the Active Message functions
to support a variety of libfabric data transfer APIs, including tagged
message queue, message queue, RMA, and atomic operations.

The *psm* provider can work with the psm2-compat library, which exposes
a PSM 1.x interface over the Intel Omni-Path Fabric.

# LIMITATIONS

The *psm* provider doesn't support all the features defined in the
libfabric API. Here are some of the limitations:

Endpoint types
: Only support non-connection based types *FI_DGRAM* and *FI_RDM*

Endpoint capabilities
: Endpoints can support any combination of data transfer capabilities
  *FI_TAGGED*, *FI_MSG*, *FI_ATOMICS*, and *FI_RMA*. These capabilities
  can be further refined by *FI_SEND*, *FI_RECV*, *FI_READ*, *FI_WRITE*,
  *FI_REMOTE_READ*, and *FI_REMOTE_WRITE* to limit the direction of
  operations. The limitation is that no two endpoints can have overlapping
  receive or RMA target capabilities in any of the above categories. For
  example it is fine to have two endpoints with *FI_TAGGED* | *FI_SEND*,
  one endpoint with *FI_TAGGED* | *FI_RECV*, one endpoint with *FI_MSG*,
  one endpoint with *FI_RMA* | *FI_ATOMICS*. But it is not allowed to
  have two endpoints with *FI_TAGGED*, or two endpoints with *FI_RMA*.

  *FI_MULTI_RECV* is supported for non-tagged message queue only.

  Other supported capabilities include *FI_TRIGGER*.

Modes
: *FI_CONTEXT* is required for the *FI_TAGGED* and *FI_MSG*
  capabilities. That means, any request belonging to these two
  categories that generates a completion must pass as the operation
  context a valid pointer to type *struct fi_context*, and the space
  referenced by the pointer must remain untouched until the request
  has completed. If none of *FI_TAGGED* and *FI_MSG* is asked for,
  the *FI_CONTEXT* mode is not required.

Progress
: The *psm* provider requires manual progress. The application is
  expected to call *fi_cq_read* or *fi_cntr_read* function from time
  to time when no other libfabric function is called to ensure
  progress is made in a timely manner. The provider does support
  auto progress mode. However, the performance can be significantly
  impacted if the application purely depends on the provider to
  make auto progress.

Unsupported features
: These features are unsupported: connection management,
  scalable endpoint, passive endpoint, shared receive context,
  send/inject with immediate data.

# RUNTIME PARAMETERS

The *psm* provider checks for the following environment variables:

*FI_PSM_UUID*
: PSM requires that each job has a unique ID (UUID). All the processes
  in the same job need to use the same UUID in order to be able to
  talk to each other. The PSM reference manual advises to keep UUID
  unique to each job. In practice, it generally works fine to reuse
  UUID as long as (1) no two jobs with the same UUID are running at
  the same time; and (2) previous jobs with the same UUID have exited
  normally. If running into "resource busy" or "connection failure"
  issues with unknown reason, it is advisable to manually set the UUID
  to a value different from the default.

  The default UUID is 0FFF0FFF-0000-0000-0000-0FFF0FFF0FFF.

*FI_PSM_NAME_SERVER*
: The *psm* provider has a simple built-in name server that can be used
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

*FI_PSM_TAGGED_RMA*
: The RMA functions are implemented on top of the PSM Active Message functions.
  The Active Message functions have limit on the size of data can be transferred
  in a single message. Large transfers can be divided into small chunks and
  be pipe-lined. However, the bandwidth is sub-optimal by doing this way.

  The *psm* provider use PSM tag-matching message queue functions to achieve
  higher bandwidth for large size RMA. For this purpose, a bit is reserved from
  the tag space to separate the RMA traffic from the regular tagged message queue.

  The option is on by default. To turn it off set the variable to 0.

*FI_PSM_AM_MSG*
: The *psm* provider implements the non-tagged message queue over the PSM
  tag-matching message queue. One tag bit is reserved for this purpose.
  Alternatively, the non-tagged message queue can be implemented over
  Active Message. This experimental feature has slightly larger latency.

  This option is off by default. To turn it on set the variable to 1.

*FI_PSM_DELAY*
: Time (seconds) to sleep before closing PSM endpoints. This is a workaround
  for a bug in some versions of PSM library.

  The default setting is 1.

*FI_PSM_TIMEOUT*
: Timeout (seconds) for gracefully closing PSM endpoints. A forced closing
  will be issued if timeout expires.

  The default setting is 5.

*FI_PSM_PROG_INTERVAL*
: When auto progress is enabled (asked via the hints to *fi_getinfo*),
  a progress thread is created to make progress calls from time to time.
  This option set the interval (microseconds) between progress calls.

  The default setting is 1 if affinity is set, or 1000 if not. See
  *FI_PSM_PROG_AFFINITY*.

*FI_PSM_PROG_AFFINITY*
: When set, specify the set of CPU cores to set the progress thread
  affinity to. The format is
  `<start>[:<end>[:<stride>]][,<start>[:<end>[:<stride>]]]*`,
  where each triplet `<start>:<end>:<stride>` defines a block of
  core_ids. Both `<start>` and `<end>` can be either the `core_id`
  (when >=0) or `core_id - num_cores` (when <0).

  By default affinity is not set.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_psm2`(7)](fi_psm2.7.html),
[`fi_psm3`(7)](fi_psm3.7.html),
