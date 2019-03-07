/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_fwd.h $
 * Description: GASNet Header for selected constants and simple types
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

// This header provides a subset of gasnetex.h, for convenience of clients who
// want a minimal header that is safe to include in their own client's code.
// It defines only preprocessor #define constants and typedefs of simple data types.
// All preprocessor identifiers are in the GEX_ or _GEX_ namespace.
// All typedefs are in the gex_ namespace.
// The _gex_ namespace is used for structure tags.
// In particular it does not contain any function declarations or definitions.
// Note that only identifiers in the gex_ or GEX_ namespace are guaranteed to
// exist as specified but their expansions shown here are subject to change.
// The only dependency is on the fixed-width integer types (i.e. <stdint.h> or
// <cstdint>), if necessary clients can #include other/portable_inttypes.h to
// provide them.

#ifndef _GEX_FWD_H
#define _GEX_FWD_H

#if defined(__cplusplus) && !defined(_IN_GASNETEX_H)
extern "C" {
#endif

//================================================
// gex_Rank_t
//================================================

typedef uint32_t gex_Rank_t;

#define GEX_RANK_INVALID (~(gex_Rank_t)0)

//================================================
// gex_Event_t
//================================================

struct _gex_event_s;
typedef struct _gex_event_s *gex_Event_t;

// Pre-defined values: output values
#define GEX_EVENT_INVALID      ((gex_Event_t)(uintptr_t)0)
#define GEX_EVENT_NO_OP        ((gex_Event_t)(uintptr_t)1)

// Pre-defined values: input pointers-to-event
#define GEX_EVENT_NOW    ((gex_Event_t*)(uintptr_t)1)
#define GEX_EVENT_DEFER  ((gex_Event_t*)(uintptr_t)2)
#define GEX_EVENT_GROUP  ((gex_Event_t*)(uintptr_t)3)

//================================================
// GEX_DT_*
// Data types for atomics and reductions
//================================================

typedef uint32_t gex_DT_t;

// Integer types:
#define GEX_DT_I32   (1 << _GEX_DT_I32)
#define GEX_DT_U32   (1 << _GEX_DT_U32)
#define GEX_DT_I64   (1 << _GEX_DT_I64)
#define GEX_DT_U64   (1 << _GEX_DT_U64)

// Floating-point types:
#define GEX_DT_FLT   (1 << _GEX_DT_FLT)
#define GEX_DT_DBL   (1 << _GEX_DT_DBL)

// User-defined type:
#define GEX_DT_USER  (1 << _GEX_DT_USER)

//================================================
// GEX_OP_*
// Operation codes for atomics and reductions
//================================================

typedef uint32_t gex_OP_t;

// Accessors
#define GEX_OP_SET   (1 << _GEX_OP_SET)
#define GEX_OP_GET   (1 << _GEX_OP_GET)
#define GEX_OP_SWAP  (1 << _GEX_OP_SWAP)
#define GEX_OP_FCAS  (1 << _GEX_OP_FCAS)
#define GEX_OP_CAS   (1 << _GEX_OP_CAS)

// Non-fetching bitwise operators
#define GEX_OP_AND   (1 << _GEX_OP_AND)
#define GEX_OP_OR    (1 << _GEX_OP_OR)
#define GEX_OP_XOR   (1 << _GEX_OP_XOR)

// Fetching bitwise operators
#define GEX_OP_FAND  (1 << _GEX_OP_FAND)
#define GEX_OP_FOR   (1 << _GEX_OP_FOR)
#define GEX_OP_FXOR  (1 << _GEX_OP_FXOR)

// Non-fetching arithmetic operators
#define GEX_OP_ADD   (1 << _GEX_OP_ADD)
#define GEX_OP_SUB   (1 << _GEX_OP_SUB)
#define GEX_OP_MULT  (1 << _GEX_OP_MULT)
#define GEX_OP_MIN   (1 << _GEX_OP_MIN)
#define GEX_OP_MAX   (1 << _GEX_OP_MAX)
#define GEX_OP_INC   (1 << _GEX_OP_INC)
#define GEX_OP_DEC   (1 << _GEX_OP_DEC)

// Fetching arithmetic operators
#define GEX_OP_FADD  (1 << _GEX_OP_FADD)
#define GEX_OP_FSUB  (1 << _GEX_OP_FSUB)
#define GEX_OP_FMULT (1 << _GEX_OP_FMULT)
#define GEX_OP_FMIN  (1 << _GEX_OP_FMIN)
#define GEX_OP_FMAX  (1 << _GEX_OP_FMAX)
#define GEX_OP_FINC  (1 << _GEX_OP_FINC)
#define GEX_OP_FDEC  (1 << _GEX_OP_FDEC)

// User-defined reduction operators:
#define GEX_OP_USER          (1 << _GEX_OP_USER)
#define GEX_OP_USER_NC       (1U << _GEX_OP_USER_NC)

// DEPRECATED alias
#define GEX_OP_CSWAP GEX_OP_FCAS

// Convert between fetching and non-fetching variants
// Undefined result if input is not in the appropriate range.
#define GEX_OP_TO_FETCHING(op)    ((op) << (_GEX_OP_FAND - _GEX_OP_AND))
#define GEX_OP_TO_NONFETCHING(op) ((op) >> (_GEX_OP_FAND - _GEX_OP_AND))

//================================================
// GEX_FLAG_*
// Flags used for numerous purposes
//================================================

typedef uint32_t gex_Flags_t;

#define GEX_FLAG_IMMEDIATE              (1U <<  0)

#define GEX_FLAG_SELF_SEG_UNKNOWN       (1U <<  1)
#define GEX_FLAG_SELF_SEG_SOME          (1U <<  2)
#define GEX_FLAG_SELF_SEG_BOUND         (1U <<  3)
#define GEX_FLAG_SELF_SEG_OFFSET        (1U <<  4)
#define GEX_FLAG_PEER_SEG_UNKNOWN       (1U <<  5)
#define GEX_FLAG_PEER_SEG_SOME          (1U <<  6)
#define GEX_FLAG_PEER_SEG_BOUND         (1U <<  7)
#define GEX_FLAG_PEER_SEG_OFFSET        (1U <<  8)

#define GEX_FLAG_AM_PREPARE_LEAST_CLIENT (1U <<  9)
#define GEX_FLAG_AM_PREPARE_LEAST_ALLOC  (1U << 10)

#define GEX_FLAG_AD_MY_RANK             (1U <<  9)
#define GEX_FLAG_AD_MY_NBRHD            (1U << 10)

#define GEX_FLAG_AD_ACQ                 (1U << 11)
#define GEX_FLAG_AD_REL                 (1U << 12)

#define GEX_FLAG_AD_FAVOR_MY_RANK       (1U <<  0)
#define GEX_FLAG_AD_FAVOR_MY_NBRHD      (1U <<  1)
#define GEX_FLAG_AD_FAVOR_REMOTE        (1U <<  2)

#define GEX_FLAG_AM_SHORT               (1U <<  0)
#define GEX_FLAG_AM_MEDIUM              (1U <<  1)
#define GEX_FLAG_AM_LONG                (1U <<  2)
#define GEX_FLAG_AM_MEDLONG             (GEX_FLAG_AM_MEDIUM|GEX_FLAG_AM_LONG)

#define GEX_FLAG_AM_REQUEST             (1U <<  3)
#define GEX_FLAG_AM_REPLY               (1U <<  4)
#define GEX_FLAG_AM_REQREP              (GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_REPLY)

#define GEX_FLAG_ENABLE_LEAF_LC         (1U <<  9)

#define GEX_FLAG_USES_GASNET1           (1U <<  0)

#define GEX_FLAG_TM_SCRATCH_SIZE_MIN          (1U <<  0)
#define GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED  (1U <<  1)

#define GEX_FLAG_RANK_IS_JOBRANK        (1U << 13)

//--------------------------------------------------------
// Private definitions
// Identifiers defined below are not for public use
//--------------------------------------------------------

#define _GEX_DT_I32   0
#define _GEX_DT_U32   1
#define _GEX_DT_I64   2
#define _GEX_DT_U64   3
#define _GEX_DT_FLT   4
#define _GEX_DT_DBL   5
#define _GEX_DT_USER  6

// NOTE:
// The order of OPs is key to making the TO_FETCHING and TO_NONFETCHING
// macros work.  Additionally, the non-fetching ops defined in UPC 1.3
// (plus a "shadow" for GET) must use only values less than 16.

// Non-fetching:
#define _GEX_OP_AND   0
#define _GEX_OP_OR    1
#define _GEX_OP_XOR   2
#define _GEX_OP_ADD   3
#define _GEX_OP_SUB   4
#define _GEX_OP_MULT  5
#define _GEX_OP_MIN   6
#define _GEX_OP_MAX   7
#define _GEX_OP_INC   8
#define _GEX_OP_DEC   9
#define _GEX_OP_SET   10  // This is non-fetching SWAP in UPC 1.3
#define _GEX_OP_CAS   11  // This is non-fetching CSWAP in UPC 1.3
// Bit 12 is reserved to correspond to GEX_OP_TO_NONFETCHING(GEX_OP_GET), which is invalid
// Bit 13 is reserved for _GEX_OP_LOGAND
// Bit 14 is reserved for _GEX_OP_LOGOR

// Fetching:
#define _GEX_OP_FAND  15
#define _GEX_OP_FOR   16
#define _GEX_OP_FXOR  17
#define _GEX_OP_FADD  18
#define _GEX_OP_FSUB  19
#define _GEX_OP_FMULT 20
#define _GEX_OP_FMIN  21
#define _GEX_OP_FMAX  22
#define _GEX_OP_FINC  23
#define _GEX_OP_FDEC  24
#define _GEX_OP_SWAP  25
#define _GEX_OP_FCAS  26
#define _GEX_OP_GET   27
// Bit 28 is reserved to correspond to GEX_OP_TO_FETCHING(GEX_OP_LOGAND), which is invalid
// Bit 29 is reserved to correspond to GEX_OP_TO_FETCHING(GEX_OP_LOGOR), which is invalid

// User-defined operations:
#define _GEX_OP_USER    30
#define _GEX_OP_USER_NC 31

#if defined(__cplusplus) && !defined(_IN_GASNETEX_H)
}  // ends extern "C"
#endif

#endif // _GEX_FWD_H
