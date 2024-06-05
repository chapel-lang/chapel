# GASNet-EX Memory Kinds: Implementation Status

# Introduction

This document provides a detailed status of the Memory Kinds feature
implementation and is updated as that status changes.

This document makes references to an external document:

  + GASNet-EX API: Memory Kinds

which is available in the repository as <memory_kinds.pdf>
For brevity, this will be referenced as simply "the MK API".

The majority of this document will address only "CUDA_UVA" and "HIP" memory
kinds.  For information regarding other experimental memory kinds, see the
[Experimental Memory Kinds](#markdown-header-experimental-memory-kinds)
section.

# General Usage

By default, the `configure` script does not enable support for
any non-host memory kinds.  Use of new configure option `--enable-memory-kinds`
enables probes for the necessary headers and libraries for all available device
"kinds" (presently "CUDA_UVA", "HIP" and "ZE") and enables the prototype implementation of
memory kinds if such support is found.  This is the recommended mechanism to
enable memory kinds support, since it will enable additional kinds as they are
added.  For more detailed control for a given kind (such as "cuda-uva" or "hip") the
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

  + `--with-hip-home=...` or `HIP_HOME`
  + `--with-hip-cflags=...` or `HIP_CFLAGS`
  + `--with-hip-libs=...` or `HIP_LIBS`
  + `--with-hip-ldflags=...` or `HIP_LDFLAGS`

Generally, it is sufficient to provide the installation prefix of the device
library installation using the `--with-...-home` option or the corresponding
environment variable, since the remaining settings
all have sensible defaults once the installation prefix is known.

Clients can use the preprocessor identifier `GASNET_HAVE_MK_CLASS_CUDA_UVA`
(defined to `1` or undefined) to determine if support for the CUDA_UVA memory
kind was detected at configure time.

Clients can use the preprocessor identifier `GASNET_HAVE_MK_CLASS_HIP`
(defined to `1` or undefined) to determine if support for the HIP memory
kind was detected at configure time.

The preprocessor identifier `GASNET_HAVE_MK_CLASS_MULTIPLE` is more general,
providing the client with an indication if configure detected support for *any*
memory kinds other than host memory.  Again the value is either defined to `1`
or undefined.

# Supported Configurations

All current memory kinds implementation work is limited to two types of devices:

1. Devices with the CUDA Device API and Unified Virtual Addressing (UVA).
   This should include all modern NVIDIA GPUs and CUDA Toolkit versions.

2. Devices with the HIP API, which should include all AMD GPUs supported by AMD ROCm.
   Please consult the ROCm documentation for information on supported devices.

Support is further limited to the following conduit/network combinations, and
only when running drivers with support for PCIe peer-to-peer communication:  

  + ibv-conduit on Linux over NVIDIA/Mellanox InfiniBand hardware  
  + ucx-conduit on Linux over NVIDIA/Mellanox InfiniBand hardware  
  + ofi-conduit (`verbs` provider) on Linux over NVIDIA/Mellanox InfiniBand hardware  
  + ofi-conduit (`verbs` provider) on HPE Cray EX systems using Slingshot-10 NICs  
  + ofi-conduit (`cxi` provider) on HPE Cray EX systems using Slingshot-11 NICs  
  + ofi-conduit *may* work with other providers and/or networks, but the
    GASNet-EX maintainers lack access to test such configurations.  Reports of
    success or failure are encouraged!

In this document we will use "communication offload" in place
of vendor-specific terms for the relevant technology ("GPUDirect RDMA" for
NVIDIA or "ROCmRDMA" for AMD), except when a specific vendor's technology is
intended.  In some cases, additional optional software (such as GPU-specific
Linux kernel modules) is required.  Please consult your GPU vendor's
documentation for assistance determining what driver software is needed for
your specific hardware and Linux distribution.  The Open MPI and MVAPICH
projects also have some documentation regarding deploying of communication
offload for their respective MPI implementations.

With ibv-conduit, `GASNET_SEGMENT_FAST` segment mode is supported.  This is the
default segment mode, but can be specified explicitly at configure time using
the `--enable-segment-fast` option.  To be clear: `--enable-segment-large` and
`--enable-segment-everything` configurations of ibv-conduit do not support
the memory kinds work in the current implementation.

With ucx-conduit, `GASNET_SEGMENT_FAST` and `GASNET_SEGMENT_LARGE` segment
modes are supported.  Fast is the default segment mode, but can be specified
explicitly at configure time using the `--enable-segment-fast` option.  To be
clear: `--enable-segment-everything` configurations of ucx-conduit do not
support the memory kinds work in the current implementation.

With ofi-conduit, `GASNET_SEGMENT_FAST` and `GASNET_SEGMENT_LARGE` segment
modes are supported.  Fast is the default segment mode, but can be specified
explicitly at configure time using the `--enable-segment-fast` option.  To be
clear: `--enable-segment-everything` configurations of ofi-conduit do not
support the memory kinds work in the current implementation.

To the best of our knowledge communications offload support is not available on
current aarch64 (aka ARM64 or ARMv8) systems.  NVIDIA does not support UVA on
32-bit platforms, and AMD does not claim any 32-bit CPU support at all.
This work currently supports only x86-64 and ppc64le.

For configurations on an unsupported CPU architecture which otherwise appear to
to satisfy the configure-testable requirements above, a warning will be issued.
In the case that `--enable-kind-[kind]` was passed but a testable requirement
other than CPU architecture is not satisfied, `configure` will fail.  In other
cases lacking prerequisites, `GASNET_HAVE_MK_CLASS_[KIND]` will be
undefined and attempts to create device segments will fail at runtime.  Future
releases are expected to eventually include a "reference implementation" that
will allow creation of device segments on a wider range of platforms and
transparently stage transfers through host memory bounce buffers, but that is
not yet present.

If support is enabled at configure time, then the implementation will attempt
to determine at runtime if communication offload support is present or not.  If
it is not, then `gex_MK_Create()` will fail with an appropriate message.

## Limits on GPU segment size

Modern GPUs supporting communication offload utilize a "Base Address Register"
mechanism to map the device memory into the PCIe address space.  The amount of
memory which can be mapped by GASNet-EX as a GPU segment is limited by the
`BAR` capability of your GPU, which can also be severely limited by the
motherboard and/or BIOS.

To query the BAR capability of an NVIDIA GPU, run `nvidia-smi -q` and look for the
"Total" value in the "BAR1 Memory Usage" section for an (optimistic) maximum on
the amount of memory which can be mapped.

We are not currently aware of a programmatic means to query the BAR capability of
an AMD GPU.  However, the Linux kernel boot messages (sometimes available via
the `dmesg` utility or in `/var/log/boot`, or similar) may contain lines like
the following:
```
[  115.169185] amdgpu 0000:03:00.0: amdgpu: VRAM: 32752M 0x0000008000000000 - 0x00000087FEFFFFFF (32752M used)
[  115.169188] amdgpu 0000:03:00.0: amdgpu: GART: 512M 0x0000000000000000 - 0x000000001FFFFFFF
[  115.169190] amdgpu 0000:03:00.0: amdgpu: AGP: 267780096M 0x000000A000000000 - 0x0000FFFFFFFFFFFF
[  115.169201] [drm] Detected VRAM RAM=32752M, BAR=32768M
```
Which in this case shows slightly under 32GB of VRAM and a BAR size of a full
32GB.  In this instance, the VRAM size of `32752M` is the relevant size and,
just like the NVIDIA case, is an (optimistic) maximum limit on mappable memory.

For either GPU vendor, these limits are per GPU across all
process and runtimes on a given node.  Thus a portion is consumed by each
GASNet-EX segment created on a given node, as well as by other uses of
GPUDirect RDMA such as an MPI implementation.  Typically a few tens of MB are
also reserved by the driver itself, though driver overheads as large as 560MB
per process have been observed.

Additionally, the BAR usage is *per-HCA* and thus use of multiple rails may
limit the size of GPU segments.

## UCX Support for Memory Kinds

The support for CUDA and HIP/ROCm device memory has been *optionally* available
in UCX since the 1.6.0 release (which is also the oldest supported by
ucx-conduit).

The ucx-conduit has support for `GEX_MK_CLASS_CUDA_UVA` and `GEX_MK_CLASS_HIP`.
However, see "UCX and CUDA memory" under "Known Issues", below.

## OFI (aka libfabric) Support for Memory Kinds

Our testing has found version 1.12.0 of libfabric to be the oldest which
correctly supports CUDA device memory (modulo the known issues listed later).
For HIP/ROCm device memory the minimum is 1.12.1.  In both cases the support in
libfabric is optional and must be explicitly enabled when the library is built.

In general, absence of the appropriate device support in a build of libfabric
will go undetected at run time and device memory will be treated as host memory.
This can lead to random data corruption, SIGBUS or SIGSEGV errors, which might
be indistinguishable from some of the known issues described later.

To enable device memory support in a build of libfabric from sources, pass its
`configure` script one or both of `--with-cuda` or `--with-rocr` (yes "rocr"
not "rocm").  If the necessary CUDA or ROCm runtime libraries are not in the
default locations, then one may pass an optional argument such as
`--with-cuda=/opt/nvidia/cuda-11.2`.

To query an install of libfabric for CUDA support, the most reliable means
known to us is to run `fi_pingpong` on a compute node with debug tracing
enabled using the two commands which follow:
```sh
$ fi_pingpong &
$ env FI_LOG_LEVEL=debug fi_pingpong localhost |& grep ofi_hmem_init
```
This will yield one or more lines of the form
```
libfabric:[PID]:[TIMESTAMP]:core:core:ofi_hmem_init():222<info> Hmem iface FI_HMEM_[API] not supported
```
Where `[API]` may be `CUDA` or `ROCR`, among others.  These lines enumerate the
device APIs which are *not* supported in the libfabric build.  It is generally
safe to assume that absence of your target API from this output means support is
present in your build of libfabric (assuming it meets the minimum version
requirements, above).

The ofi-conduit has support for `GEX_MK_CLASS_CUDA_UVA` and `GEX_MK_CLASS_HIP`.
However, see multiple "OFI *" entries under "Known Issues", below.

# Known Issues

## Loopback

The current implementation does not handle RMA operations between combinations
of host and GPU memory in the same process (loopback), though this will be
supported in the future.

It should be noted that this temporary limitation precludes use of GASNet for
transfers which could alternatively be performed using `cudaMemcpy()`, or
`cuMemcpy{DtoH,HtoD,DtoD}()` (possibly with some CUDA calls to enable
peer-to-peer access), or by using the `hipMemcpy*()` equivalents.
Therefore, it is recommended practice (and will
*continue* to be so when this limitation is removed) that clients bypass
GASNet-EX to perform such transfers.

For the most up-to-date information on this issue see
[bug 4149](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4149)

## Small Gets into CUDA memory on supported InfiniBand hardware

As documented
[here](https://github.com/linux-rdma/rdma-core/blob/master/providers/mlx5/man/mlx5dv_create_qp.3.md)
current versions of libibverbs for "mlx5" generation HCAs may default to
performing small RMA Gets into an unused space in the work request structure
to minimize the number of PCI bus crossings required.  Such Get operations are
eventually completed by a `memcpy()` to the original destination when the
completion queue entry is reaped.  This `memcpy()` fails (with a `SIGSEGV`)
when the destination is device memory.

As noted in the vendor's documentation, setting `MLX5_SCATTER_TO_CQE=0` in the
environment disables this undesired behavior.  We hope to be able to provide a
better solution (automatic and specific to device memory Gets) in a future
release.

This issue has been seen to impact ibv-, ucx- and ofi-conduits running over
supported InfiniBand hardware, but not on *every* system.  Because this
work-around may increase the latency of all small RMA Gets, including those
into host memory, it is recommended that you set `MLX5_SCATTER_TO_CQE=0` only
if your system exhibits this issue.

For the most up-to-date information on this issue see
[bug 4151](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4151)

## UCX and CUDA memory

Use of GASNet-EX "CUDA_UVA" memory kinds with ucx-conduit may crash due to
incorrect default algorithm selection inside UCX for RMA transfers involving
device memory.  For sufficiently recent UCX versions, there are work-arounds
based on setting environment variables to modify the algorithm selection to be
correct for device memory at the expense of slowing of host memory RMA.

For the most up-to-date information on this issue see
[bug 4384](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4384)

## OFI and Unsupported Providers

Currently, ofi-conduit support for memory kinds is limited to the `verbs` and
`cxi` libfabric providers (when they are built with appropriate `FI_HMEM`
capability support).  If ofi-conduit is compiled with memory kinds enabled and
used with any other provider, the `GEX_MK_CLASS_*` defines will indicate support
for the relevant kinds but calls to `gex_MK_Create()` will return
`GASNET_ERR_RESOURCE`.

It is hoped that in the future builds of ofi-conduit for an unsupported provider
will not define `GEX_MK_CLASS_*`.

## OFI/InfiniBand and CUDA memory

When using ofi-conduit and the `verbs` provider over supported InfiniBand
hardware, RMA Puts with their source in CUDA device memory may crash with a
`SIGSEGV` due to incorrect algorithm selection inside libfabric.

This issue has been seen with a subset of applicable systems, with the majority
of those tested *not* reproducing the problem.  We have been unable to correlate
the problem with the versions of the HCA, firmware, driver or libibverbs.

If you are using an impacted system, this issue can be worked-around
by setting `FI_VERBS_INLINE_SIZE=0` in the environment.  Because this
setting disables a valuable performance optimization, it may increase
the latency of all small RMA Puts, including those from host memory,
and small Active Message.  Therefore, it is strongly recommended that
you set this variable *only* if your system exhibits this issue.

For the most up-to-date information on this second issue see
[bug 4494](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4494)

## OFI/Slingshot-10 and CUDA memory

Because the Slingshot-10 network uses an InfiniBand HCA and libfabric's
`verbs` provider, such systems are impacted by two of the known issues described
above.

1.  Small Gets into CUDA memory on supported InfiniBand hardware  
    The work around of setting `MLX5_SCATTER_TO_CQE=0` is effective.

2.  OFI/InfiniBand and CUDA memory  
    The work around of setting `FI_VERBS_INLINE_SIZE=0` is effective.

This is based on testing with HPE's `libfabric/1.11.0.x.y` environment module
(for multiple values of `x` and `y`).

# Tested Configurations

We have yet to establish the minimum required versions of hardware, drivers or
libraries.  However, the following platforms are our primary development
systems and represent "known good" combinations (modulo those caveats listed
elsewhere in this document):

Summit:  

  + ppc64le (IBM POWER9)
  + CUDA 10.1.243 and 11.3
  + Mellanox ConnectX-5 HCAs
  + NVIDIA Volta-class GPUs

Dirac:  

  + x86_64 (Intel Nehalem)
  + CUDA 11.1 through 11.6
  + Mellanox ConnectX-5 HCAs
  + NVIDIA Maxwell-class GPUs

AMD GPU systems:

  + x86_64 (multiple AMD CPU families)
  + ROCm 4.5
  + Mellanox ConnectX-5 and ConnectX-6 HCAs
  + AMD Instinct-class (MI50, MI100, MI250X) GPUs

HPE Cray EX systems with Slingshot-10:

  + SS10+Nvidia:
    - AMD Milan CPUs
    - Mellanox ConnectX-5 HCAs
    - Nvidia A100 GPUs
    - CUDA 11.6 and 11.7

  + SS10+AMD:
    - AMD Rome CPUs
    - Mellanox ConnectX-5 HCAs
    - AMD MI100 GPUs
    - ROCm 4.5

HPE Cray EX systems with Slingshot-11:

  + SS11+Nvidia:
    - AMD Milan CPUs
    - HPE Cassini NICs
    - Nvidia A100 GPUs
    - CUDA 11.7

  + SS11+AMD:
    - AMD Milan CPUs
    - HPE Cassini NICs
    - AMD MI250X GPUs
    - ROCm 5.1.0

Eventual minimum requirements may be lower than on the platforms listed above, or
possibly higher.

# Experimental Memory Kinds

## oneAPI Level Zero "ZE" Kind

Currently there is experimental support for the "oneAPI Level Zero" API of Intel
GPUs, which is known as the "ZE" kind.  There are known correctness issues,
the performance has yet to be characterized or tuned, and there are multiple
quality-of-implementation issues to be addressed before this support can be
recommended for use in production.

One notable aspect of the experimental status is that the kind-specific members
in `gex_MK_Create_args_t` are subject to change in future releases.

We advise contacting us at gasnet-staff@lbl.gov if you wish to use the ZE kind.

Support for the ZE kind is only present in ofi-conduit, and has only been tested
with the `cxi` provider for HPE Cray EX systems using Slingshot-11 NICs.  It has
*not* been tested with ofi-conduit using the 'verbs' libfabric provider for
Slingshot-10 or other networks using NVIDIA/Mellanox InfiniBand hardware.  
We encourage reports of success or failure with other libfabric providers.

There currently is no ZE memory kind support in ibv- or ucx-conduits.
We are seeking access to InfiniBand-connected clusters of nodes with Intel GPUs
to enable work on ibv- and ucx-conduits, and on the `verbs` provider for
ofi-conduit.  
Please contact us if you can provide such access.

Configure options and environment variables to enable and configure the ZE kind
are analogous to those for CUDA-UVA and HIP, and behave as described earlier
under [General Usage](#markdown-header-general-usage):

  + `--enable-kind-ze[=probe]`
  + `--with-ze-home=...` or `ZE_HOME`
  + `--with-ze-cflags=...` or `ZE_CFLAGS`
  + `--with-ze-libs=...` or `ZE_LIBS`
  + `--with-ze-ldflags=...` or `ZE_LDFLAGS`

Clients can use the preprocessor identifier `GASNET_HAVE_MK_CLASS_ZE`
(defined to `1` or undefined) to determine if support for the ZE memory
kind was detected at configure time.

### Minimum System Requirements for the ZE Kind

Testing of the ZE kind and Slingshot-11 networks has shown the vendor-provided
kernel with SUSE Linux Enterprise Server 15 SP4 (aka "SLES 15.4") to be usable.
However, when using the kernel provided with SLES 15.3, all calls to
`gex_EP_BindSegment()` with ZE device memory segments fail.  
We encourage reports of success or failure with other kernel versions.

### Known Issues with the ZE Memory Kind

As of the time of writing, testing has shown 32KiB to be the largest device
memory segment size which can successfully be used with the ZE kind and the
ofi-conduit `cxi` provider.  Larger sizes fail in `gex_EP_BindSegment()`,
with a verbose message indicating that the problem appears to be this known
issue.  We encourage reports of success with larger device segments.
For the most up-to-date information on this issue see
[bug 4679](https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4679)

# Implementation Status Summary

Currently the implementation is sufficient (when hardware, software and
configuration constraints are met) to use the pseudo-code below to perform RMA
operations between combinations of host and GPU memory (subject to any
previously noted temporary prohibitions, such as on loopback transfers).

Please note that all error checking has been elided from this example.  Proper
checking of return codes, if any, is especially important when using this
prototype.  Modifications for `GEX_MK_CLASS_HIP` are straight-forward, requiring
only changes to the initialization of `args`.

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

On those conduits and segment modes listed under "Supported Configurations",
the implementation of this API is believed to be
complete with respect to the MK API.  In particular, it is capable of
creating segments of both client-allocated and GASNet-allocated memory, using
either the defined `kind` value `GEX_MK_HOST` or a kind created using
`gex_MK_Create()` with a class of `GEX_MK_CLASS_CUDA_UVA` or `GEX_MK_CLASS_HIP`.

Notably lacking from both specification and implementation is a means to request
or demand allocation of memory suitable for intra-nbrhd cross-mapping via PSHM.

## `gex_Segment_Destroy()`

This API is fully implemented to the current proposed specification.

## `gex_EP_Create()`

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists, as described in the following paragraphs.

Current builds of GASNet-EX will define a preprocessor macro
`GASNET_MAXEPS` which advertises the optimistic maximum number of endpoints per
process, inclusive of the primordial endpoint created by `gex_Client_Init()`.
Any call to `gex_EP_Create()` which would exceed this limit will fail with
a return of GASNET_ERR_RESOURCE.

Currently, only those conduits and segment modes listed under "Supported
Configurations" have values of `GASNET_MAXEPS` larger than 1 (each currently
33).  Additionally, these conduits support only the `GEX_EP_CAPABILITY_RMA`
capability for non-primordial endpoints.

The `GEX_FLAG_HINT_ACCEL_*` values are currently defined, but ignored.

## `gex_EP_BindSegment()`

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists (see `gex_EP_Create()` for the current scope of
multi-EP support).

## `gex_EP_PublishBoundSegment()`

This API does not appear in the MK API, nor in related documents which
preceded it.  Complete semantics are documented in `docs/GASNet-EX.txt`.

This call is currently necessary as the only means to actively distribute the
RMA credentials required by some conduits (ibv, ucx and ofi among them).  While this task is
performed in `gex_Segment_Attach()` for primordial endpoints, use of this API is
required prior to use of `gex_RMA_*()` APIs using non-primordial endpoints.  It
is hoped that this call can become optional in the future.

This API, along with all types and constants required to specify its arguments,
are defined and will link in any conduit.  However, it is useful only when
multi-EP support exists (see `gex_EP_Create()` for the current scope of
multi-EP support).

## `gex_TM_Pair()`

This API is believed to be fully implemented in all conduits and accepted by
all APIs required to do so by the MK API (notably the `gex_RMA_*()`,
`gex_AM_*()` and `gex_VIS_*()` API families).

Since multi-EP support is currently exclusive to ibv, ucx and ofi conduits (and only in
some segment modes), the use in other conduits is effectively limited to aliasing of the
primordial team.

## `gex_TM_Create()`

At this point in time, there is no support for non-primordial endpoints as
members of teams, and more specifically they are not accepted in the `args`
passed to `gex_TM_Create()`.  This means all RMA communication involving
non-primordial endpoints must currently utilize a `gex_TM_t` returned by
`gex_TM_Pair()`.

## `gex_TM_Dup()`

Not implemented.

## `gex_MK_Create()`

This API is implemented as described in the MK API. This
includes the conditional definition (defined to `1` or undefined) of
`GASNET_HAVE_MK_CLASS_CUDA_UVA` and/or `GASNET_HAVE_MK_CLASS_HIP`, each of which
is defined only when the respective headers and libs were located at configure
time *and* one is using a conduit and segment mode listed under "Supported
Configurations".
Otherwise these feature macros will be undefined.

While these feature macros have only a conditional definition, the
enum values `GEX_MK_CLASS_CUDA_UVA` and `GEX_MK_CLASS_HIP` are both
defined unconditionally in `gasnet_mk.h`.
Any calls to `gex_MK_Create()` specifying a class when *not* supported will
return `GASNET_ERR_BAD_ARG`, as documented in the MK API.

## `gex_MK_Destroy()`

This API is implemented, but is currently only allowed for a `gex_MK_t` not
ever used to create a segment.
