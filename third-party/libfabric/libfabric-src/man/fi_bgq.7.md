---
layout: page
title: fi_bgq(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_bgq \- The Blue Gene/Q Fabric Provider

# OVERVIEW

The bgq provider is a native implementation of the libfabric interfaces
that makes direct use of the unique hardware features such as the
Messaging Unit (MU), Base Address Table (BAT), and L2 Atomics.

The purpose of this provider is to demonstrate the scalability and
performance of libfabric, providing an "extreme scale"
development environment for applications and middleware using the
libfabric API, and to support a functional and performant version of
MPI3 on Blue Gene/Q via MPICH CH4.

# SUPPORTED FEATURES

The bgq provider supports most features defined for the libfabric API.
Key features include:

*Endpoint types*
: The Blue Gene/Q hardware is connectionless and reliable. Therefore, the
  bgq provider only supports the *FI_EP_RDM* endpoint type.

*Capabilities*
: Supported capabilities include *FI_MSG*, *FI_RMA*, *FI_TAGGED*,
  *FI_ATOMIC*, *FI_NAMED_RX_CTX*, *FI_READ*, *FI_WRITE*, *FI_SEND*, *FI_RECV*,
  *FI_REMOTE_READ*,  *FI_REMOTE_WRITE*, *FI_MULTI_RECV*, *FI_DIRECTED_RECV*,
  *FI_SOURCE* and *FI_FENCE*.

Notes on FI_DIRECTED_RECV capability:
The immediate data which is sent within the *senddata* call to support
FI_DIRECTED_RECV for BGQ must be exactly 4 bytes, which BGQ uses to
completely identify the source address to an exascale-level number of ranks
for tag matching on the recv and can be managed within the MU packet.
Therefore the domain attribute cq_data_size is set to 4 which is the OFI
standard minimum.

*Modes*
: The bgq provider requires *FI_CONTEXT* and *FI_ASYNC_IOV*

*Memory registration modes*
: Both FI_MR_SCALABLE and FI_MR_BASIC are supported, specified at configuration
  time with the "--with-bgq-mr" configure option.  The base address table
  utilized by FI_MR_SCALABLE for rdma transfers is completely software emulated,
  supporting FI_ATOMIC, FI_READ, FI_WRITE, FI_REMOTE_READ, and FI_REMOTE_WRITE
  capabilities.  With FI_MR_BASIC the FI_WRITE is completely hardware
  accelerated, the other rdma transfers are still software emulated but the
  use of a base address table is no longer required as the offset is now the
  virtual address of the memory from the application and the key is the delta
  from which the physical address can be computed if necessary.

*Additional features*
: Supported additional features include *FABRIC_DIRECT*, *scalable endpoints*,
  and *counters*.

*Progress*
: Both progress modes, *FI_PROGRESS_AUTO* and *FI_PROGRESS_MANUAL*, are
  supported. The progress mode may be specified via the "--with-bgq-progress"
  configure option.

*Address vector*
: Only the *FI_AV_MAP* address vector format is supported.

# UNSUPPORTED FEATURES

*Endpoint types*
: Unsupported endpoint types include *FI_EP_DGRAM* and *FI_EP_MSG*

*Capabilities*
: The bgq provider does not support the *FI_RMA_EVENT*, and
  *FI_TRIGGER* capabilities.

*Address vector*
: The bgq provider does not support the *FI_AV_TABLE* address vector format.
  Support for *FI_AV_TABLE* may be added in the future.

# LIMITATIONS

The bgq provider only supports *FABRIC_DIRECT*. The size of the fi_context
structure for *FI_CONTEXT* is too small to be useful. In the 'direct' mode the
bgq provider can re-define the struct fi_context to a larger size - currently
64 bytes which is the L1 cache size.

The fi_context structure for *FI_CONTEXT* must be aligned to 8 bytes. This requirement is because
the bgq provider will use MU network atomics to track completions and the memory
used with MU atomic operations must be aligned to 8 bytes. Unfortunately, the libfabric API
has no mechanism for applications to programmatically determine these alignment
requirements. Because unaligned MU atomics operations are a fatal error, the
bgq provider will assert on the alignment for "debug" builds (i.e., the '-DNDEBUG'
pre-processor flag is not specified).

The progress thread used for *FI_PROGRESS_AUTO* effectively limits the maximum
number of ranks-per-node to 32.  However for FI_PROGRESS_MANUAL the maximum is 64.

For FI_MR_SCALABLE mr mode the memory region key size (mr_key_size) is 2 *bytes*; Valid key values are
0..2^16-1.

It is invalid to register memory at the base virtual address "0" with a
length of "UINTPTR_MAX" (or equivalent). The Blue Gene/Q hardware operates on
37-bit physical addresses and all virtual addresses specified in the libfabric
API, such as the location of source/destination data and remote memory locations,
must be converted to a physical address before use. A 64-bit virtual address
space will not fit into a 37-bit physical address space.

fi_trecvmsg() fnd fi_recvmsg() unctions do not support non-contiguous receives
and the iovec count must be 1. The fi_trecvv() and fi_recvv() functions are
currently not supported.

# RUNTIME PARAMETERS

No runtime parameters are currently defined.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
