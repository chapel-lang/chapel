# Preface

```
NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE
NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE
NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE

  This file documents the "Memory Kinds" feature of GASNet-EX, intended
  only for use by developers with a specific interest in this feature.
  Other client developers should limit themselves to the interfaces and
  behaviors given in docs/GASNet-EX.txt and the GASNet-1 specification.

  While it is intended that features and capabilities described here will
  make their way into the GASNet-EX specification, the APIs in this file
  and their implementation are only a prototype. All aspects of the APIs
  and capabilities first introduced in this file are subject to
  non-trivial changes before the prototype stage ends.

NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE
NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE
NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE
```

# Introduction

This document provides a detailed status of the Memory Kinds feature
implementation and is updated as that status changes.

This document makes references to an external document, which is available on
request from gasnet-staff@lbl.gov:  

  + GASNet-EX API Proposal: Memory Kinds, Revision 2020.11.0

For brevity, this will be referenced as simply "the API Proposal".

# General Usage

By default, the `configure` script does not enable support for
any non-host memory kinds.  Use of new configure option `--enable-memory-kinds`
enables probes for the necessary headers and libraries for all available device
"kinds" (presently only "CUDA_UVA") and enables the prototype implementation of
memory kinds if such support is found.  This is the recommended mechanism to
enable memory kinds support, since it will enable additional kinds as they are
added.  For more detailed control for a given kind (such as "cuda-uva") the
following take precedence over `--(en|dis)able-memory-kinds`:

  + `--disable-kind-[name]` disables probing for support for the named kind.
  + `--enable-kind-[name]` probes for support for the named kind, with failure
    of the probe being a fatal `configure` error.
  + `--enable-kind-[name]=probe` probes for support for the named kind, with
    failure of the probe being non-fatal (the same behavior requested for all
    kinds by using `--enable-memory-kinds`).

On our main development platforms, the logic in `configure` is sufficient to
locate the required headers and libraries with no additional options.  However,
the following options (and environment equivalents) are available to guide the
probe if needed:  

  + `--with-cuda-home=...` or `CUDA_HOME`
  + `--with-cuda-cflags=...` or `CUDA_CFLAGS`
  + `--with-cuda-libs=...` or `CUDA_LIBS`
  + `--with-cuda-ldflags=...` or `CUDA_LDFLAGS`

Generally, it is sufficient to provide the installation prefix of the CUDA
toolkit using either `--with-cuda-home=...` or `CUDA_HOME`, since the others
all have sensible defaults once the installation prefix is known.

Clients can use the preprocessor identifier `GASNET_HAVE_MK_CLASS_CUDA_UVA`
(defined to `1` or undefined) to determine if support for the CUDA_UVA memory
kind was detected at configure time.

The preprocessor identifier `GASNET_HAVE_MK_CLASS_MULTIPLE` is more general,
providing the client with an indication if configure detected support for *any*
memory kinds other than host memory.

# Supported Configurations

All current memory kinds implementation work is limited to devices with the
CUDA Device API and Unified Virtual Addressing (UVA).  This should include
all modern NVIDIA GPUs and CUDA Toolkit versions.

Support is further limited to ibv-conduit on Linux and only when using Mellanox
InfiniBand hardware and drivers with support for "GPUDirect RDMA" (GDR).  In
some cases additional optional software, such as "nvidia_peer_memory" must be
installed.  Please consult Mellanox documentation for assistance determining
what driver software is needed for your specific hardware and Linux
distribution.  The Open MPI and MVAPICH projects also have some documentation
regarding deployment of GPUDirect RDMA for their respective MPI implementations.

Furthermore, only `GASNET_SEGMENT_FAST` segment mode is supported.  This is the
default segment mode, but can be specified explicitly at configure time using
the `--enable-segment-fast` option.  To be clear: `--enable-segment-large` and
`--enable-segment-everything` configurations of ibv-conduit do not support
the memory kinds work in the current implementation.

To the best of our knowledge, Mellanox currently disclaims support for GPUDirect
RDMA on aarch64 (aka ARM64 or ARMv8) and NVIDIA does not support UVA on ILP32
platforms.  Therefore, this work currently supports only x86-64 and ppc64le.

For any configurations that do not meet all of the configure-testable
requirements outlined above, the memory kinds support in the current prototype
implementation will be disabled (or `configure` will fail if it was passed
`--with-kind-cuda-uva`).  Specifically, `GASNET_HAVE_MK_CLASS_CUDA_UVA` will be
undefined and attempts to create device segments will fail at runtime.  Future
releases are expected to eventually include a "reference implementation" that
will allow creation of device segments on a wider range of platforms and
transparently stage transfers through host memory bounce buffers, but that is
not yet present.

If support is enabled at configure time, then the implementation will attempt
to determine at runtime if GDR support is present or not.  If it is not, then
`gex_MK_Create()` will fail with an appropriate message.

## Limits on GPU segment size

Modern NVIDIA GPUs with GPUDirect utilize a "Base Address Register" mechanism
to map the device memory into the PCIe address space.  The amount of memory
which can be mapped by GASNet-EX as a GPU segment is limited by the `BAR1`
capability of your GPU, which can also be severely limited by the motherboard
and/or BIOS.

To query the BAR1 capability of your GPU, run `nvidia-smi -q` and look for the
"Total" value in the "BAR1 Memory Usage" section for an (optimistic) maximum on
the amount of memory which can be mapped.  This limit is per GPU across all
process and runtimes on a given node.  Thus a portion is consumed by each
GASNet-EX segment created on a given node, as well as by other uses of
GPUDirect RDMA such as an MPI implementation.  Typically a few tens of MB are
also reserved by the driver itself.

## GDR and Multi-rail

Though our test and development systems have multi-rail InfiniBand networks,
there are currently unresolved issues with respect to use of multiple rails
within a given process.  Consequentially, support is currently limited to
single-rail configurations, which can be achieved ether at configure time by
using the option `--disable-ibv-multirail`, or at runtime by setting the
environment variable `GASNET_IBV_PORTS` to name a single valid port.  Both
mechanisms are documented in `ibv-conduit/README`.

For the most up-to-date information on this issue see
[bug 4148](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4148)

Additionally, the BAR1 usage (described earlier in this document) has been
observed to be *per-HCA* and thus use of multiple rails may limit the size of
GPU segments.

## Loopback

The current implementation does not handle RMA operations between combinations
of host and GPU memory in the same processes (loopback), though this will be
supported in the future.

It should be noted that this temporary limitation precludes use of GASNet for
transfers which could alternatively be performed using `cudaMemcpy()`, or
`cuMemcpy{DtoH,HtoD,DtoD}()` (possibly with some CUDA calls to enable
peer-to-peer access).  Therefore, it is recommended practice (and will
*continue* to be so when this limitation is removed) that clients bypass
GASNet-EX to perform such transfers.

For the most up-to-date information on this issue see
[bug 4149](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4149)

## GDR and PSHM

Currently the implementation is sufficient (when using supported hardware,
drivers and libraries) to perform RMA operations between combinations of host
and GPU memory in which the two involved endpoints are in distinct "nbrhds".

There is a temporary limitation (in addition to the no-loopback limitation,
above) which prohibits intra-nbrhd RMA operations.  In other words, there is no
support for RMA operations in which one or both endpoints has a GPU memory
segment and the two processes are in the same shared memory domain (aka "nbrhd"
in GASNet-EX documentation).

Currently, RMA transfers involving GPU memory between processes in the same
compute node are supported only when PSHM is "inactive" (meaning either
`--disable-pshm` at configure time, or `GASNET_SUPERNODE_MAXSIZE=1` in ones
environment at runtime).

For the most up-to-date information on this issue see
[bug 4148](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4148)

## Premature local completion of GDR Puts from device memory

In addition to the multi-path issues described above (under "GDR and
Multi-rail" and "GDR and PSHM" sub-headings), Put operations with their source
in device memory have been observed to signal local completion prior to actual
transfer of the data (as can be demonstrated by writing data to the source
after sync and observing it arrive in the destination buffer).

This is believed to be an issue with how ibv and GDR interact, and we are
hopeful that a workaround can be implemented in a future release.

For the most up-to-date information on this issue see
[bug 4150](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4150)

## GDR and small Gets into device memory

As documented
[here](https://github.com/linux-rdma/rdma-core/blob/master/providers/mlx5/man/mlx5dv_create_qp.3.md)
current versions of libibverbs for "mlx5" generation HCAs may default to
performing small RDMA Gets into an unused space in the work request structure
to minimize the number of PCI bus crossings required.  Such Get operations are
eventually completed by a `memcpy()` to the original destination when the
completion queue entry is reaped.  This `memcpy()` fails (with a `SIGSEGV`)
when the destination is device memory.

As noted in Mellanox's documentation, setting `MLX5_SCATTER_TO_CQE=0` in the
environment disables this undesired behavior.  We hope to be able to provide a
better solution (automatic and specific to device memory Gets) in a future
release.

For the most up-to-date information on this issue see
[bug 4151](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4151)

## CUDA Multi-Process Service (MPS)

Testing conducted to date is insufficient to establish whether the current
implementation is compatible with the use of CUDA MPS.  Reports (positive or
negative) regarding such compatibility are welcome.

# Tested Configurations

We have yet to establish the minimum required versions of hardware, drivers or
libraries.  However, the following two platforms are our primary development
systems and represent "known good" combinations (modulo those caveats listed
elsewhere in this document, such as with regards to multi-rail):

Summit:  

  + ppc64le (IBM POWER9)
  + CUDA 10.1.243
  + Mellanox ConnectX-5 HCAs
  + NVIDIA Volta-class GPUs

Dirac:  

  + x86_64 (Intel Nahalem)
  + CUDA 11.1
  + Mellanox ConnectX-5 HCAs
  + NVIDIA Maxwell-class GPUs

Eventual minimum requirements may be lower than those of either platform, or
possibly higher.

# Implementation Status Summary

Currently the implementation is sufficient (when hardware, software and
configuration constraints are met) to use the pseudo-code below to perform RMA
operations between combinations of host and GPU memory (subject to the
previously noted temporary prohibitions on loopback and intra-nbrhd transfers).

Please note that all error checking has been elided from this example.  Proper
checking of return codes, if any, is especially important when using this
prototype.

```
  // Bootstrap and establish host memory segment for the primordial endpoints
  gex_Client_Init(&myClient, &myEP, &myTM, "MK example", &argc, &argv, 0)
  gex_Segment_Attach(...);

  // Create memory kind object for CUDA device 0
  gex_MK_t dev0Kind;
  gex_MK_Create_args_t args;
  args.gex_flags = 0;
  args.gex_class = GEX_MK_CLASS_CUDA_UVA;
  args.gex_args.gex_class_cuda_uva.gex_CUdevice = 0;
  gex_MK_Create(&dev0_kind, myClient, args, 0);

  // Ask GASNet to allocate a 1GB CUDA UVA segment
  gex_Segment_t dev0Segment;
  gex_Segment_Create(&dev0Segment, myClient, NULL, 1024*1024*1024, dev0Kind, 0);

  // Create an RMA-only endpoint, bind dev0Segment and publish RMA credentials
  gex_EP_t dev0EP;
  gex_EP_Create(&dev0EP, myClient, GEX_EP_CAPABILITY_RMA, 0);
  gex_EP_BindSegment(dev0EP, dev0Segment, 0);
  gex_EP_PublishBoundSegment(myTM, &dev0EP, 1, 0);

  // Note assumptions made in remainder of this example
  assert( gex_EP_QueryIndex(myEP) == 0 );
  assert( gex_EP_QueryIndex(dev0EP) == 1 );

  // Query device addresses needed for RMA
  void *loc_dev0, *rem_dev0;
  loc_dev0 = gex_Segment_QueryAddr(dev0Segment);
  gex_Event_t ev = gex_EP_QueryBoundSegmentNB(gex_TM_Pair(myEP,1), peer_rank, &rem_dev0, NULL, NULL, 0);
  gex_Event_Wait(ev);
  
  // [ do something that places data in GPU memory ]

  // Perform a blocking 4MB GPU-to-GPU Get
  gex_RMA_GetBlocking(gex_TM_Pair(dev0EP,1), loc_dev0, peer_rank, rem_dev0, 4*1024*1024, 0);
```

# Implementation Status by GASNet-EX API

This section describes the known limitations of each of the APIs introduced
recently in order to support memory kinds.  Due to interaction among
APIs, it is impossible to completely avoid forward references.

## Additions:

The preprocessor identifier `GASNET_HAVE_MK_CLASS_MULTIPLE` is defined to `1` if
support has been compiled in for any memory kinds other than host memory.

## Renames:

Some types, constants and functions have been renamed relative to their first
appearances in the API Proposal (when it was known as "GASNet API Proposal:
Multi-EP. Revision 2020.6.1"):

  + `gex_Segment_EP_Bind()` is replaced by `gex_EP_SegmentBind()`
  + `gex_MemKind_Create()` is replaced by `gex_MK_Create()`
  + `gex_MemKind_Destroy()` is replaced by `gex_MK_Destroy()`
  + `gex_MemKind_t` is replaced by `gex_MK_t`
    - With the constant `GEX_MEMKIND_HOST` replaced by `GEX_MK_HOST`
  + `gex_MemKind_Class_t` becomes `gex_MK_Class_t`
    - With `GEX_MEMKIND_CLASS_` shortened `GEX_MK_CLASS_` in the naming of the
      enum values
  + `gex_MemKind_Create_args_t` to `gex_MK_Create_args_t`
    - With `gex_mk_` shortened to `gex_` in naming of struct and union members

The current (2020.11.0) revision of the API Proposal uses the names above, and
the remainder of this section will utilize the new names exclusively.

## `gex_Segment_Attach()`

The `gex_Segment_Attach()` call remains the only supported means by which to
create and bind a segment to the primordial endpoint.  This is true despite the
recent introduction of APIs which, when used in the proper sequence, would
appear to provide a suitable replacement.

While this is technically a limitation of the alternative APIs, it is
documented here for clarity.

## `gex_Segment_Create()`

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists (see `gex_EP_Create()` for the current scope of
multi-EP support).

On ibv-conduit, specifically, the implementation of this API is believed to be
complete with respect to the API Proposal.  In particular, it is capable of
creating segments of both client-allocated and GASNet-allocated memory, using
either the defined `kind` value `GEX_MK_HOST` or a kind created using
`gex_MK_Create()` with a class of `GEX_MK_CLASS_CUDA_UVA`.

Notably lacking from both specification and implementation is a means to request
or demand allocation of memory suitable for intra-nbrhd cross-mapping via PSHM.

## `gex_Segment_Destroy()`

Not currently implemented.

## `gex_EP_Create()`

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists, as described in the following paragraphs.

Current builds of GASNet-EX will define a preprocessor macro
`GASNET_MAXEPS` which advertises the optimistic maximum number of endpoints per
process, inclusive of the primordial endpoint created by `gex_Client_Init()`.
Any call to `gex_EP_Create()` which would exceed this limit will fail with
a return of GASNET_ERR_RESOURCE.

Currently, only ibv-conduit in FAST segment mode has a value of `GASNET_MAXEPS`
larger than 1 (it is currently 33).  Additionally, ibv-conduit only supports the
`GEX_EP_CAPABILITY_RMA` capability for non-primordial endpoints.

The `GEX_FLAG_HINT_ACCEL_*` values are currently defined, but ignored.

## `gex_EP_BindSegment()`

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists (see `gex_EP_Create()` for the current scope of
multi-EP support).

## `gex_EP_PublishBoundSegment()`

This API does not appear in the API Proposal, nor in related documents which
preceded it.  Complete semantics are documented in `docs/GASNet-EX.txt`.

This call is currently necessary as the only means to actively distribute the
RMA credentials required by some conduits (ibv among them).  While this task is
performed in `gex_Segment_Attach()` for primordial endpoints, use of this API is
required prior to use of `gex_RMA_*()` APIs using non-primordial endpoints.  It
is hoped that this call can become optional in the future.

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists (see `gex_EP_Create()` for the current scope of
multi-EP support).

## `gex_TM_Pair()`

This API is believed to be fully implemented in all conduits and accepted by
all APIs required to do so by the API Proposal (notably the `gex_RMA_*()`,
`gex_AM_*()` and `gex_VIS_*() API families).

Since multi-EP support is currently exclusive to ibv-conduit in FAST segment
mode, the use in other conduits is effectively limited to aliasing of the
primordial team.

## `gex_TM_Create()`

At this point in time, there is no support for non-primordial endpoints as
members of teams, and more specifically they are not accepted in the `args`
passed to `gex_TM_Create()`.  This means all RMA communication involving
non-primordial endpoints must currently utilize a `gex_TM_t` returned by
`gex_TM_pair()`.

## `gex_TM_Dup()`

Not implemented.

## `gex_MK_Create()`

This API is implemented as described in the API Proposal (with some renames
relative to their first appearance, as detailed earlier in this document), This
includes the conditional definition (defined to `1` or undefined) of
`GASNET_HAVE_MK_CLASS_CUDA_UVA`, which is currently defined only when the
necessary headers and libs were located at configure time *and* one is using
ibv-conduit in FAST segment mode.  In all other circumstances
`GASNET_HAVE_MK_CLASS_CUDA_UVA` will be undefined.

While `GASNET_HAVE_MK_CLASS_CUDA_UVA` has only a conditional definition, the
enum value `GEX_MK_CLASS_CUDA_UVA` is defined unconditionally in `gasnet-mk.h`.
Any calls to `gex_MK_Create()` specifying this class when *not* supported will
return `GASNET_ERR_BAD_ARG`, as documented in the API Proposal.

## `gex_MK_Destroy()`

This API is implemented, but is only legal for a kind not currently in use by
any segment.  Given the lack of a `gex_Segment_Destroy()` implementation, this
API is therefore only useful for a kind which has been created but never used.
