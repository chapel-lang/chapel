# GASNet-1 Backwards-Compatibility in GASNet-EX

## Introduction

The GASNet-EX library is the result of a major specification upgrade and
implementation renovation effort, building upon 15 years of lessons learned
with the earlier GASNet-1 interface and implementation. GASNet-EX is the next
generation of the GASNet-1 communication system, continuing our commitment to
provide portable, high-performance, production-quality, open-source software.
GASNet-EX interfaces are focused on the emerging needs of exascale
supercomputing, providing communication services to a variety of programming
models on current and future HPC architectures.

For an introduction to GASNet-EX, please see:

* Bonachea D, Hargrove P.     
  "GASNet-EX: A High-Performance, Portable Communication Library for Exascale",     
  Proceedings of Languages and Compilers for Parallel Computing (LCPC'18), Oct 2018.     
  https://doi.org/10.25344/S4QP4W    

For more details on the GASNet-EX API see: [GASNet-EX.txt](GASNet-EX.txt)

## Using GASNet-1 APIs over GASNet-EX

We highly encourage all client authors to update their codes from GASNet-1
interfaces to GASNet-EX replacements, in order to reap benefits in performance,
scalability and functionality relative to what's available through the GASNet-1
API. However in the interests of backwards-compatibility and smoothing
transitional efforts, the GASNet-EX implementation also includes a complete
implementation of the GASNet-1 API. 

Legacy client codes may continue to `#include <gasnet.h>` and use the GASNet-1
APIs provided by that header, as specified in the latest GASNet-1 spec:

* Bonachea D, Hargrove P., "GASNet Specification, v1.8.1",     
  Lawrence Berkeley National Laboratory Tech Report (LBNL-2001064). Aug 2017.     
  https://doi.org/10.2172/1398512 

### Compliance Status

Early versions of GASNet-EX (starting in release 2017.6.0) were mostly
compliant with the GASNet-1 API, implementing all GASNet-1 calls in common
usage.

Starting in release 2019.3.0, the **current GASNet-EX implementation is fully
compliant with the GASNet-1 specification**.

The GASNet-EX implementation of GASNet-1 APIs is functionally correct, but
may exhibit minor differences in performance characteristics relative to the
GASNet-1 implementation. In many cases, the GASNet-EX implementation is
noticably more efficient (by virtue of improvements made to shared internal
code). In a few rare cases the opposite may be true. One notable example
of the latter is `gasnet_AMRequestLongAsync*()` now always blocks for local
completion of the source payload before returning from injection; clients
wishing flexible control over local completion behavior should call
`gex_AM_RequestLong*()` instead.

### Behavioral Differences

Although GASNet-EX provides a compliant implementation of the GASNet-1
specification, there are some minor behavioral differences between the legacy
GASNet-1 implementation and the current GASNet-EX-based backwards-compatibility
layer. These differences are unlikely to be relevant for most clients, and
mostly arise in unspecified or underspecified areas of the GASNet-1
specification. They are documented below for completeness:

* The GASNet-1 spec is a bit vague about the behavior of RMA operations or
  AMLong operations where the source and destination memory regions overlap.
  GASNet-EX considers this a usage error that yields undefined behavior, and in
  some cases the implementation may diagnose such errors as fatal.

* There is similarly some vagueness in the behavior of RMA operations or
  AMLong operations where any portion of the remote address range falls outside
  the registered GASNet segment.  GASNet-EX considers this a usage error that
  yields undefined behavior, and in some cases the implementation may diagnose
  such errors as fatal.

* GASNet-1 provided "non-bulk" RMA put/get operations, which required the payloads
  be "properly aligned for accessing objects of size nbytes". This requirement
  has been relaxed in the GASNet-EX implementation which no longer requires
  any particular payload alignment for RMA put/get.  As such,
  `gasnet_get{,_nb,_nbi}()` and *blocking* `gasnet_put()` are now semantically
  identical to their `_bulk` variants.  Non-blocking put calls still differ in
  the local completion semantics of the source buffer for `_bulk` vs "non-bulk"
  variants.

* The `nbytes` argument to `AM_{Request,Reply}Long()` calls has always been
  permitted to be zero, specifying an empty payload transfer. GASNet-1 stated
  that in this case the destination address passed to the AMLong handler at the
  target was unspecified. GASNet-EX strengthens this to guarantee the
  `dest_addr` value provided at the initiator is always delivered to the
  corresponding target-side handler argument, even in the case of an empty
  payload transfer.

* The `minheapoffset` argument to `gasnet_attach()` is now ignored.

* The GASNet-1 spec only requires implementations to support 65535 "in-flight"
  non-blocking operations per process, and the GASNet-1 implementation imposed
  a per-thread limit of ~64K explicit handles. In the GASNet-EX implementation
  the number of in-flight ops is now limited only by available heap memory.

* GASNet-1 specifies `gasnet_handle_t` values are thread-specific, but GASNet-EX
  implements `gasnet_handle_t` as an alias for `gex_Event_t`, values of which
  are thus thread-independent.

* The GASNet-1 spec allowed creation of a zero-length RMA segment, but this was
  not a useful capability in practice, with no known clients relying upon this
  corner-case. GASNet-EX prohibits zero-length memory segments to reduce
  pointless corner cases. Calls to the legacy `gasnet_attach()` call requesting
  a zero-length segment are now silently rounded up to one page, maintaining
  compatibility at the cost of wasting a small amount of memory for clients
  theoretically using this corner case.

* The Vector/Index/Strided (VIS) non-contiguous RMA library calls in the
  GASNet-1 implementation `gasnet_{put,get}{v,i,s}*()` were prototypes and not
  a normative part of the GASNet-1 specification. Nevertheless, the GASNet-EX
  implementation provides full backwards-compatibility with those prototype
  interfaces, declared in `<gasnet_vis.h>`. 

* The collectives library calls (excluding barrier) in the GASNet-1
  implementation (`gasnet_coll_*`) were prototypes and not a normative part of
  the GASNet-1 specification. GASNet-EX includes a fundamental redesign of
  these interfaces, specified in [GASNet-EX.txt](GASNet-EX.txt). The GASNet-1
  prototype variants of these calls are not supported.

## Mixing GASNet-EX and GASNet-1 in a Transitional Client

GASNet-EX supports incremental migration from GASNet-1 APIs to GASNet-EX APIs,
allowing calls to both APIs within a transitional client.  

Such clients are subject to the following notes and restrictions:

* Clients must initialize the library using exactly one of `gasnet_init()`
  (GASNet-1) or `gex_Client_Init()` (GASNet-EX). The former automatically
  activates backwards-compatibility with legacy GASNet-1 behaviors. The latter
  can be passed the flag `GEX_FLAG_USES_GASNET1` to activate the same
  behaviors.

* Clients who call `gasnet_init()` have selected the GASNet-1 init sequence, and
  are required to call `gasnet_attach()` before making any other GASNet calls,
  aside from those documented in the GASNet-1 specification as permitted in the
  init/attach interval.

* Clients who init using `gasnet_init()` may obtain the objects necessary for
  invoking GASNet-EX calls using `gasnet_QueryGexObjects()`. 
  See [GASNet-EX.txt](GASNet-EX.txt) for details.

* GASNet-EX collectives calls and the GASNet-1 barrier call are prohibited from
  operating concurrently. See [GASNet-EX.txt](GASNet-EX.txt) for details.

* See [GASNet-EX.txt](GASNet-EX.txt) for complete details on how GASNet-EX
  types and functions differ from the GASNet-1 entities they replace.

