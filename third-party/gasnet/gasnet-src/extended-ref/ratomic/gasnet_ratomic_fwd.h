/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/ratomic/gasnet_ratomic_fwd.h $
 * Description: GASNet Remote Atomics API Header (forward decls)
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_RATOMIC_FWD_H
#define _GASNET_RATOMIC_FWD_H

// (###) Note to conduit authors:
// When providing conduit-specific implementations of Remote Atomics,
// one should clone this file to the conduit directory and make the
// changes indicated by comments starting with (###).


// If we are using this file, then there is no conduit-specific version.
// (###) Conduits cloning the file *must* remove (or comment-out) this define!
#if GASNETE_BUILD_AMRATOMIC
  #define GASNETE_RATOMIC_AMONLY 1
#endif

/* stats needed by the RAtomic reference implementation */
// (###) Conduits should merge this set of STATS counters with their own
// to allow the reference implementation to continue compiling along side
// the conduit-specific implementation(s).
//
// TODO-EX: find an alternative location to define the RAtomic reference
// STATS so that conduits may *append* to them (rather than replicate)
// when cloning this file.
#ifndef GASNETI_RATOMIC_STATS
  #define GASNETI_RATOMIC_STATS(CNT,VAL,TIME)    \
        /* Currently empty */
#endif

//
// Assert that all atomics implementations possible in the current build
// are unconditionally "tools safe" (for each type independently).  The
// term "tools safe" here means that concurrent access via GASNet-Tools
// will produce correct results.
//
// Each token GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE##dtcode should be 0 or 1:
//  0: Honor _real_ad->_tools_safe (testing each instance dynamically)
//  1: Ignore _real_ad->_tools_safe, assuming it is 1 (saves 1 branch)
// Consumed by the GASNETE_RATOMIC_DISP() macro.
//
// (###) Conduits cloning this file must update these to be accurate.
// For types which are never offloaded, the value '1' may be retained.
// For a type which may be offloaded this value '0' should be used
// *unless* the implementation is consistent with GASNet-Tools atomics.
// HOWEVER, that is almost never a safe determination to make, since
// Tools may use mutexes, etc.   TL;DR: use 0 for offloadable types.
//
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I32 1
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U32 1
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I64 1
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U64 1
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_FLT 1
#define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_DBL 1

#endif // _GASNET_RATOMIC_FWD_H
