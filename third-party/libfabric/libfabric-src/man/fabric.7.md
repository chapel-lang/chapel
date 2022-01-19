---
layout: page
title: fabric(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fabric \- Fabric Interface Library

# SYNOPSIS

```c
#include <rdma/fabric.h>
```

Libfabric is a high-performance fabric software library designed to
provide low-latency interfaces to fabric hardware.

# OVERVIEW

Libfabric provides 'process direct I/O' to application software communicating
across fabric software and hardware.  Process direct I/O, historically
referred to as RDMA, allows an application to directly access network
resources without operating system interventions.  Data transfers can
occur directly to and from application memory.

There are two components to the libfabric software:

*Fabric Providers*
: Conceptually, a fabric provider may be viewed as a local hardware
  NIC driver, though a provider is not limited by this definition.
  The first component of libfabric is a general purpose framework that
  is capable of handling different types of fabric hardware.  All
  fabric hardware devices and their software drivers are required to
  support this framework.  Devices and the drivers that plug into the
  libfabric framework are referred to as fabric providers, or simply
  providers.  Provider details may be found in
  [`fi_provider`(7)](fi_provider.7.html).

*Fabric Interfaces*
: The second component is a set of communication operations.
  Libfabric defines several sets of communication functions that
  providers can support.  It is not required that providers implement
  all the interfaces that are defined; however, providers clearly
  indicate which interfaces they do support.

# FABRIC INTERFACES

The fabric interfaces are designed such that they are cohesive and not
simply a union of disjoint interfaces.  The interfaces are logically
divided into two groups: control interfaces and communication
operations. The control interfaces are a common set of operations that
provide access to local communication resources, such as address
vectors and event queues.  The communication operations expose
particular models of communication and fabric functionality, such as
message queues, remote memory access, and atomic operations.
Communication operations are associated with fabric endpoints.

Applications will typically use the control interfaces to discover
local capabilities and allocate necessary resources.  They will then
allocate and configure a communication endpoint to send and receive
data, or perform other types of data transfers, with remote endpoints.

# CONTROL INTERFACES

The control interfaces APIs provide applications access to network
resources.  This involves listing all the interfaces available,
obtaining the capabilities of the interfaces and opening a provider.

*fi_getinfo - Fabric Information*
: The fi_getinfo call is the base call used to discover and request
  fabric services offered by the system.  Applications can use this
  call to indicate the type of communication that they desire.  The
  results from fi_getinfo, fi_info, are used to reserve and configure
  fabric resources.

  fi_getinfo returns a list of fi_info structures.  Each structure
  references a single fabric provider, indicating the interfaces that
  the provider supports, along with a named set of resources.  A
  fabric provider may include multiple fi_info structures in the
  returned list.

*fi_fabric - Fabric Domain*
: A fabric domain represents a collection of hardware and software
  resources that access a single physical or virtual network.  All
  network ports on a system that can communicate with each other
  through the fabric belong to the same fabric domain.  A fabric
  domain shares network addresses and can span multiple providers.
  libfabric supports systems connected to multiple fabrics.

*fi_domain - Access Domains*
: An access domain represents a single logical connection into a
  fabric.  It may map to a single physical or virtual NIC or a port.
  An access domain defines the boundary across which fabric resources
  may be associated.  Each access domain belongs to a single fabric
  domain.

*fi_endpoint - Fabric Endpoint*
: A fabric endpoint is a communication portal.  An endpoint may be
  either active or passive.  Passive endpoints are used to listen for
  connection requests.  Active endpoints can perform data transfers.
  Endpoints are configured with specific communication capabilities
  and data transfer interfaces.

*fi_eq - Event Queue*
: Event queues, are used to collect and report the completion of
  asynchronous operations and events.  Event queues report events
  that are not directly associated with data transfer operations.

*fi_cq - Completion Queue*
: Completion queues are high-performance event queues used to report
  the completion of data transfer operations.

*fi_cntr - Event Counters*
: Event counters are used to report the number of completed
  asynchronous operations.  Event counters are considered
  light-weight, in that a completion simply increments a counter,
  rather than placing an entry into an event queue.

*fi_mr - Memory Region*
: Memory regions describe application local memory buffers.  In order
  for fabric resources to access application memory, the application
  must first grant permission to the fabric provider by constructing a
  memory region.  Memory regions are required for specific types of
  data transfer operations, such as RMA transfers (see below).

*fi_av - Address Vector*
: Address vectors are used to map higher level addresses, such as IP
  addresses, which may be more natural for an application to use, into
  fabric specific addresses.  The use of address vectors allows
  providers to reduce the amount of memory required to maintain large
  address look-up tables, and eliminate expensive address resolution
  and look-up methods during data transfer operations.

# DATA TRANSFER INTERFACES

Fabric endpoints are associated with multiple data transfer
interfaces.  Each interface set is designed to support a specific
style of communication, with an endpoint allowing the different
interfaces to be used in conjunction.  The following data transfer
interfaces are defined by libfabric.

*fi_msg - Message Queue*
: Message queues expose a simple, message-based FIFO queue interface
  to the application.  Message data transfers allow applications to
  send and receive data with message boundaries being maintained.

*fi_tagged - Tagged Message Queues*
: Tagged message lists expose send/receive data transfer operations
  built on the concept of tagged messaging.  The tagged message queue
  is conceptually similar to standard message queues, but with the
  addition of 64-bit tags for each message.  Sent messages are matched
  with receive buffers that are tagged with a similar value.

*fi_rma - Remote Memory Access*
: RMA transfers are one-sided operations that read or write data
  directly to a remote memory region.  Other than defining the
  appropriate memory region, RMA operations do not require interaction
  at the target side for the data transfer to complete.

*fi_atomic - Atomic*
: Atomic operations can perform one of several operations on a remote
  memory region.  Atomic operations include well-known functionality,
  such as atomic-add and compare-and-swap, plus several other
  pre-defined calls.  Unlike other data transfer interfaces, atomic
  operations are aware of the data formatting at the target memory
  region.

# LOGGING INTERFACE

Logging can be controlled using the FI_LOG_LEVEL, FI_LOG_PROV, and
FI_LOG_SUBSYS environment variables.

*FI_LOG_LEVEL*
: FI_LOG_LEVEL controls the amount of logging data that is output.  The
  following log levels are defined.

- *Warn*
: Warn is the least verbose setting and is intended for reporting errors
  or warnings.

- *Trace*
: Trace is more verbose and is meant to include non-detailed output helpful to
  tracing program execution.

- *Info*
: Info is high traffic and meant for detailed output.

- *Debug*
: Debug is high traffic and is likely to impact application performance.
  Debug output is only available if the library has been compiled with
  debugging enabled.

*FI_LOG_PROV*
: The FI_LOG_PROV environment variable enables or disables logging from
  specific providers. Providers can be enabled by listing them in a comma
  separated fashion. If the list begins with the '^' symbol, then the list will
  be negated. By default all providers are enabled.

  Example: To enable logging from the psm and sockets provider:
	FI_LOG_PROV="psm,sockets"

  Example: To enable logging from providers other than psm:
	FI_LOG_PROV="^psm"

*FI_LOG_SUBSYS*
: The FI_LOG_SUBSYS environment variable enables or disables logging at the
  subsystem level.  The syntax for enabling or disabling subsystems is similar to
  that used for FI_LOG_PROV.  The following subsystems are defined.

- *core*
: Provides output related to the core framework and its management of providers.

- *fabric*
: Provides output specific to interactions associated with the fabric object.

- *domain*
: Provides output specific to interactions associated with the domain object.

- *ep_ctrl*
: Provides output specific to endpoint non-data transfer operations,
  such as CM operations.

- *ep_data*
: Provides output specific to endpoint data transfer operations.

- *av*
: Provides output specific to address vector operations.

- *cq*
: Provides output specific to completion queue operations.

- *eq*
: Provides output specific to event queue operations.

- *mr*
: Provides output specific to memory registration.

# PROVIDER INSTALLATION AND SELECTION

The libfabric build scripts will install all providers that are supported
by the installation system.  Providers that are missing build prerequisites
will be disabled.  Installed providers will dynamically check for necessary
hardware on library initialization and respond appropriately to application
queries.

Users can enable or disable available providers through build configuration
options.  See 'configure --help' for details.  In general, a specific provider
can be controlled using the configure option '--enable-<provider_name>'.  For
example, '--enable-udp' (or '--enable-udp=yes') will add the udp provider to the
build.  To disable the provider, '--enable-udp=no' can be used.

Providers can also be enable or disabled at run time using the FI_PROVIDER
environment variable.  The FI_PROVIDER variable is set to a comma separated
list of providers to include.  If the list begins with the '^' symbol, then
the list will be negated.

  Example: To enable the udp and tcp providers only, set:
	FI_PROVIDER="udp,tcp"

The fi_info utility, which is included as part of the libfabric package, can
be used to retrieve information about which providers are available in the
system.  Additionally, it can retrieve a list of all environment variables
that may be used to configure libfabric and each provider.  See
[`fi_info`(1)](fi_info.1.html) for more details.

# ENVIRONMENT VARIABLE CONTROLS

Core features of libfabric and its providers may be configured by an
administrator through the use of environment variables.  Man pages
will usually describe the most commonly accessed variables, such as those
mentioned above.  However, libfabric defines interfaces for publishing
and obtaining environment variables.  These are targeted for providers,
but allow applications and users to obtain the full list of variables
that may be set, along with a brief description of their use.

A full list of variables available may be obtained by running the fi_info
application, with the -e or --env command line option.

# NOTES

Because libfabric is designed to provide applications direct access to
fabric hardware, there are limits on how libfabric resources may be used
in conjunction with system calls.  These limitations are notable for
developers who may be familiar programming to the sockets interface.
Although limits are provider specific, the following restrictions
apply to many providers and should be adhered to by applications desiring
portability across providers.

*fork*
: Fabric resources are not guaranteed to be available by child processes.
  This includes objects, such as endpoints and completion queues, as well
  as application controlled data buffers which have been assigned to the
  network.  For example, data buffers that have been registered with a
  fabric domain may not be available in a child process because of copy
  on write restrictions.

# ABI CHANGES

libfabric releases maintain compatibility with older releases, so that
compiled applications can continue to work as-is, and previously written
applications will compile against newer versions of the library without
needing source code changes.  The changes below describe ABI updates
that have occurred and which libfabric release corresponds to the
changes.

Note that because most functions called by applications actually call
static inline functions, which in turn reference function pointers in
order to call directly into providers, libfabric only exports a handful
of functions directly.  ABI changes are limited to those functions,
most notably the fi_getinfo call and its returned attribute structures.

The ABI version is independent from the libfabric release version.

## ABI 1.0

The initial libfabric release (1.0.0) also corresponds to ABI version 1.0.
The 1.0 ABI was unchanged for libfabric major.minor versions 1.0, 1.1, 1.2,
1.3, and 1.4.

## ABI 1.1

A number of external data structures were appended starting with libfabric
version 1.5.  These changes included adding the fields to the following
data structures.  The 1.1 ABI was exported by libfabric versions 1.5 and
1.6.

*fi_fabric_attr*
: Added api_version

*fi_domain_attr*
: Added cntr_cnt, mr_iov_limit, caps, mode, auth_key, auth_key_size,
  max_err_data, and mr_cnt fields.  The mr_mode field was also changed
  from an enum to an integer flag field.

*fi_ep_attr*
: Added auth_key_size and auth_key fields.

## ABI 1.2

The 1.2 ABI version was exported by libfabric versions 1.7 and 1.8, and
expanded the following structure.

*fi_info*
: The fi_info structure was expanded to reference a new fabric object,
  fid_nic.  When available, the fid_nic references a new set of attributes
  related to network hardware details.

## ABI 1.3

The 1.3 ABI version was exported by libfabric versions 1.9, 1.10, and
1.11.  Added new fields to the following attributes:

*fi_domain_attr*
: Added tclass

*fi_tx_attr*
: Added tclass

## ABI 1.4

The 1.4 ABI version was exported by libfabric 1.12.  Added fi_tostr_r, a
thread-safe (re-entrant) version of fi_tostr.

## ABI 1.5

ABI version starting with libfabric 1.13.  Added new fi_open API
call.

# SEE ALSO

[`fi_info`(1)](fi_info.1.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_av`(3)](fi_av.3.html),
[`fi_eq`(3)](fi_eq.3.html),
[`fi_cq`(3)](fi_cq.3.html),
[`fi_cntr`(3)](fi_cntr.3.html),
[`fi_mr`(3)](fi_mr.3.html)
