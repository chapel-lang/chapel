# GASNet-EX Implementation-Defined Documentation

This file documents client-facing implementation-specific behaviors of the
current GASNet-EX implementation provided by Lawrence Berkeley National
Laboratory.

For details of the GASNet-EX API specification and/or detailed implementation
status information, see appropriate sibling documents.

## Preprocessor Identifiers

The following are preprocessor identifiers conditionally provided by this implementation.
These are not officially part of the GASNet-EX library specification, and all
represent optional implementation features.  In particular, lack of support for
these features would not prevent an alternative implementation from claiming
conformance.

 + `GASNET_[NET]_CONDUIT`   
   This is defined to `1`, with the name of the conduit in upper-case replacing
   the `[NET]` place-holder.  Example: `GASNET_SMP_CONDUIT`  
   Exactly one such macro will be defined by this implementation in any given
   include of gasnetex.h.
 + `GASNET_DEBUG` *OR* `GASNET_NDEBUG`   
   `GASNET_DEBUG` is defined to `1` if and only if the library was
   configured with debugging support enabled, and is #undef otherwise.  
   `GASNET_NDEBUG` is defined to `1` if and only if the library was
   configured *without* debugging support enabled, and is #undef otherwise.  
   Exactly one such macro will be defined by this implementation in any given
   include of gasnetex.h.
 + `GASNET_TRACE`   
   This identifier is defined to `1` if and only if the library was
   configured with tracing support enabled, and is #undef otherwise.
 + `GASNET_STATS`   
   This identifier is defined to `1` if and only if the library was
   configured with statistical collection support enabled, and is #undef
   otherwise.
 + `GASNET_SRCLINES`   
   This identifier is defined to `1` if and only if the library was
   configured with source lines support enabled, and is #undef otherwise.
 + `GASNET_PSHM`   
   This identifier is defined to `1` if and only if the library was
   configured with PSHM support enabled, and is #undef otherwise.

See top-level README documentation for more details about each of these optional features.


