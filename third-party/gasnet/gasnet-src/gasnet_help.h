/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_help.h $
 * Description: GASNet Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_HELP_H
#define _GASNET_HELP_H

typedef struct { 
  uint64_t allocated_bytes;   /* num bytes ever allocated */
  uint64_t freed_bytes;       /* num bytes ever freed */
  uint64_t live_bytes;        /* num bytes currently allocated */
  uint64_t live_bytes_max;    /* max num bytes live at any given time */
  uint64_t allocated_objects; /* num objects ever allocated */
  uint64_t freed_objects;     /* num objects ever freed */
  uint64_t live_objects;      /* num objects currently allocated */
  uint64_t live_objects_max;  /* max num objects live at any given time */
  uint64_t overhead_bytes;    /* num bytes consumed by allocator overhead (lower bound) */
} gasneti_heapstats_t;

#if GASNET_DEBUGMALLOC
  /* _curloc is passed to debug mallocator as "file:line",
     or the special constant "SRCPOS" to retrieve the info from gasnet_srclines 
     To enable use of srcpos for a compilation unit, client should: 
       #undef GASNETT_MALLOC_USE_SRCPOS
       #define GASNETT_MALLOC_USE_SRCPOS 1 
  */
  #ifndef GASNETT_MALLOC_USE_SRCPOS
  #define GASNETT_MALLOC_USE_SRCPOS 0 /* off by default */
  #endif
  #define GASNETI_CURLOCFARG , const char *_curloc
  #define GASNETI_CURLOCAARG , (GASNETT_MALLOC_USE_SRCPOS ? \
                               "SRCPOS" :                   \
                                __FILE__ ":" _STRINGIFY(__LINE__))
  #define GASNETI_CURLOCPARG , _curloc
  extern size_t _gasneti_memcheck(void *_ptr, const char *_curloc, int _checktype);
  extern void _gasneti_memcheck_one(const char *_curloc);
  extern void _gasneti_memcheck_all(const char *_curloc);
  #define gasneti_memcheck(ptr)  (gasneti_assert(ptr != NULL), \
         (void)_gasneti_memcheck(ptr, __FILE__ ":" _STRINGIFY(__LINE__), 0)) 
  #define gasneti_memcheck_one() _gasneti_memcheck_one(__FILE__ ":" _STRINGIFY(__LINE__))
  #define gasneti_memcheck_all() _gasneti_memcheck_all(__FILE__ ":" _STRINGIFY(__LINE__))
  extern int gasneti_getheapstats(gasneti_heapstats_t *_pstat);
  extern void gasneti_heapinfo_dump(const char *_filename, int _show_live_objects);
#else
  #define GASNETI_CURLOCFARG 
  #define GASNETI_CURLOCAARG 
  #define GASNETI_CURLOCPARG 
  #define gasneti_memcheck(ptr)   ((void)0)
  #define gasneti_memcheck_one()  ((void)0)
  #define gasneti_memcheck_all()  ((void)0)
  #define gasneti_getheapstats(pstat) (memset(pstat, 0, sizeof(gasneti_heapstats_t)),1)
  #define gasneti_heapinfo_dump(f,s) ((void)0)
#endif

/* extern versions of gasnet malloc fns for use in public headers */
extern void *_gasneti_extern_malloc(size_t _sz GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_malloc)
extern void *_gasneti_extern_realloc(void *_ptr, size_t _sz GASNETI_CURLOCFARG);
extern void *_gasneti_extern_calloc(size_t _n, size_t _s GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_calloc)
extern void _gasneti_extern_free(void *_ptr GASNETI_CURLOCFARG);
extern void _gasneti_extern_leak(void *_ptr GASNETI_CURLOCFARG);
extern char *_gasneti_extern_strdup(const char *_s GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_strdup)
extern char *_gasneti_extern_strndup(const char *_s, size_t _n GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_strndup)

#define gasneti_extern_malloc(sz)      _gasneti_extern_malloc((sz) GASNETI_CURLOCAARG)
#define gasneti_extern_realloc(ptr,sz) _gasneti_extern_realloc((ptr), (sz) GASNETI_CURLOCAARG)
#define gasneti_extern_calloc(N,S)     _gasneti_extern_calloc((N),(S) GASNETI_CURLOCAARG)
#define gasneti_extern_free(ptr)       _gasneti_extern_free((ptr) GASNETI_CURLOCAARG)
#define gasneti_extern_leak(ptr)       _gasneti_extern_leak((ptr) GASNETI_CURLOCAARG)
#define gasneti_extern_strdup(s)       _gasneti_extern_strdup((s) GASNETI_CURLOCAARG)
#define gasneti_extern_strndup(s,n)    _gasneti_extern_strndup((s),(n) GASNETI_CURLOCAARG)

/* aligned malloc - allocated size bytes with given power-of-2 alignment
   may only be freed using gasneti_free_aligned */
#if HAVE_POSIX_MEMALIGN && !GASNET_DEBUGMALLOC
  // when debug mallocator is disabled, posix_memalign is friendlier
  // for use with heap analysis tools like valgrind
  #define GASNETI_USE_POSIX_MEMALIGN 1
#endif
GASNETI_INLINE(_gasneti_malloc_aligned) GASNETI_MALLOC
void * _gasneti_malloc_aligned(size_t _alignment, size_t _size GASNETI_CURLOCFARG) {
  gasneti_assert(GASNETI_POWEROFTWO(_alignment));
  gasneti_assert(_alignment <= GASNET_PAGESIZE);
#if GASNETI_USE_POSIX_MEMALIGN
  if_pf(_alignment < sizeof(void*)) _alignment = sizeof(void*);
  void *_result = NULL; // init to avoid -Wmaybe-uninitialized warnings
  int _return_code = posix_memalign(&_result, _alignment, _size);
  gasneti_assert_zeroret(_return_code);
#else
  size_t _alloc_size = _size + sizeof(void *) + _alignment;
  void *_base = _gasneti_extern_malloc(_alloc_size GASNETI_CURLOCPARG);
  void **_result = (void **)GASNETI_ALIGNUP((uintptr_t)_base + sizeof(void *), _alignment);
  *(_result - 1) = _base; /* hidden base ptr for free() */
  gasneti_assert_ptr((void *)(_result - 1) ,>=, _base);
  gasneti_assert_ptr(((uint8_t *)_result + _size) ,<=, ((uint8_t *)_base + _alloc_size));
#endif
  gasneti_assume(_result);
  gasneti_assert_ptr(_result ,==, (void **)GASNETI_ALIGNUP(_result, _alignment));
  return (void *)_result;
}
GASNETI_MALLOCP(_gasneti_malloc_aligned)
#define gasneti_malloc_aligned(align,sz) _gasneti_malloc_aligned((align), (sz) GASNETI_CURLOCAARG)

GASNETI_INLINE(_gasneti_free_aligned)
void _gasneti_free_aligned(void *_ptr GASNETI_CURLOCFARG) {
  gasneti_assert(_ptr);
#if GASNETI_USE_POSIX_MEMALIGN
  free(_ptr);
#else
  void *_base = *((void **)_ptr - 1);
  gasneti_assert(_base);
  _gasneti_extern_free(_base GASNETI_CURLOCPARG);
#endif
}
#define gasneti_free_aligned(ptr) _gasneti_free_aligned((ptr) GASNETI_CURLOCAARG)

GASNETI_INLINE(_gasneti_leak_aligned)
void _gasneti_leak_aligned(void *_ptr GASNETI_CURLOCFARG) {
  gasneti_assert(_ptr);
#if !GASNETI_USE_POSIX_MEMALIGN
  void *_base = *((void **)_ptr - 1);
  gasneti_assert(_base);
  _gasneti_extern_leak(_base GASNETI_CURLOCPARG);
#endif
}
#define gasneti_leak_aligned(ptr) _gasneti_leak_aligned((ptr) GASNETI_CURLOCAARG)

extern const char *gasnet_max_segsize_str; // client-overrideable max segment size
extern uint64_t gasnet_max_segsize; // DEPRECATED: client-overrideable max segment size

#if GASNET_DEBUG
  extern void gasneti_checkinit(void);
  extern void gasneti_checkattach(void);
  #define GASNETI_CHECKINIT()    gasneti_checkinit()
  #define GASNETI_CHECKATTACH()  gasneti_checkattach()
#else
  #define GASNETI_CHECKINIT()    ((void)0)
  #define GASNETI_CHECKATTACH()  ((void)0)
#endif

extern gex_Rank_t gasneti_mynode;
#define gex_System_QueryJobRank() (GASNETI_CHECKINIT(), (gex_Rank_t)gasneti_mynode)

extern gex_Rank_t gasneti_nodes;
#define gex_System_QueryJobSize() (GASNETI_CHECKINIT(), (gex_Rank_t)gasneti_nodes)

/* ------------------------------------------------------------------------------------ */
extern int gasneti_VerboseErrors;
#define gex_System_GetVerboseErrors() ((int)gasneti_VerboseErrors)
GASNETI_INLINE(gex_System_SetVerboseErrors)
void gex_System_SetVerboseErrors(int _enable) {
  gasneti_assert(_enable == 1 || _enable == 0);
  gasneti_VerboseErrors = _enable;
}

/* ------------------------------------------------------------------------------------ */
#if GASNETI_TM0_ALIGN
// We can detect TM0 by its better alignment than other tm's
GASNETI_INLINE(gasneti_is_tm0)
int gasneti_is_tm0(gasneti_TM_t _i_tm) {
  gasneti_static_assert(GASNETI_POWEROFTWO(GASNETI_TM0_ALIGN));
  gasneti_static_assert(GASNETI_TM0_ALIGN > 1);
  return (!((uintptr_t)(_i_tm) & (GASNETI_TM0_ALIGN-1)));
}
#else
// Cannot use alignment to distinguish TM0
extern gasneti_TM_t gasneti_thing_that_goes_thunk_in_the_dark;
#define gasneti_is_tm0(_i_tm) ((_i_tm) == gasneti_thing_that_goes_thunk_in_the_dark)
#endif

// "TM-pair"
//
// We have one external handle type `gex_TM_t`, but two corresponding internal
// types: `gasneti_TM_t` and `gasneti_TM_Pair_t`.  A `gasneti_TM_t` can be
// either an actual pointer to a `struct gasneti_team_member_internal_s` (with
// fields describing the TM) OR it can be just a 32-bit inline representation
// of a TM-pair (with fields accessed via `gasneti_tm_pair_{loc,rem}_idx()`).
//
// Use of the query `gasneti_e_tm_is_pair()` reports whether a `gex_TM_t`, once
// imported, will have the TM-pair representation.
// Use of the query `gasneti_i_tm_is_pair()` reports whether a `gasneti_TM_t`
// has the TM-pair representation.
//
// Code not needing to access any fields may pass `gex_TM_t` or `gasneti_TM_t`
// values transparently, though the latter is preferred so that the debug check
// for the "MAGIC" signature upon import occurs early in the call stack.  Such
// code does not require any explicit handling of TM-pairs.
//
// For code which does access fields, there are three approaches illustrated
// by the following examples in which
//   + `e_tm` is a `gex_TM_t`
//   + `i_tm` is a `gasneti_TM_t` without certainty of contents
//   + `my_tm` is a `gasneti_TM_t` certain to point to a structure
//   + `my_pair` is a `gasneti_TM_Pair_t` certain to hold a 32-bit inline pair
//
// 1.  "import excluding pair" - for code paths where TM-pair is prohibited:
//     my_tm = gasneti_import_tm_nonpair(e_tm);
//
// 2.  "check then import"
//     if (gasneti_e_tm_is_pair(e_tm))
//       my_pair = gasneti_import_tm_pair(e_tm);
//     else
//       my_tm = gasneti_import_tm(e_tm);
//
// 3.  "import then check before use"
//     i_tm = gasneti_import_tm(e_tm)
//     [...]
//     if (gasneti_i_tm_is_pair(i_tm))
//       my_pair = gasneti_i_tm_to_pair(i_tm);
//     else
//       my_tm = i_tm;
//
// The four type-conversion functions used in the examples above:
//   + gasneti_TM_t gasneti_import_tm(gex_TM_t);
//     Does not check if the argument is a TM-pair
//   + gasneti_TM_t gasneti_import_tm_nonpair(gex_TM_t);
//     Asserts that the argument is NOT a TM-pair
//   + gasneti_TM_Pair_t gasneti_import_tm_pair(gex_TM_t);
//     Asserts that the argument is a TM-pair
//   + gasneti_TM_Pair_t gasneti_i_tm_to_pair(gasneti_TM_t);
//     Asserts that the argument is a TM-pair
//
// -OR-
//
// At least the following internal functions operate an `e_tm` or `i_tm`, with
// transparent support for both encodings and may be sufficient to keep much
// code independent of TM-pairness:
//   + gasneti_[ei]_tm_rank_to_jobrank()
//   + gasneti_[ei]_tm_rank_to_ep_index()
//   + gasneti_[ei]_tm_rank_to_location()
//   + gasneti_[ei]_tm_jobrank_to_rank()
//   + gasneti_[ei]_tm_size()
//     Returns gex_System_QueryJobSize() for TM-pair, suitable for range checking a rank
//     Generalize/replace the following idioms which do not accept a TM-pair:
//        `gex_TM_QuerySize(e_tm)`
//        `gasneti_import_tm(e_tm)->_size`
//        `i_tm->_size`
//   + gasneti_[ei]_tm_to_i_ep()
//     Generalize/replace the following idioms which do not accept a TM-pair:
//        `gex_TM_QueryEP(e_tm)`
//        `gasneti_import_tm(e_tm)->_ep`
//        `i_tm->_ep`
//   + gasneti_[ei]_tm_to_ep_index()
//     More efficient replacement for `gasneti_[ei]_tm_to_i_ep()->_index`,
//     replacing multiple alternatives which do not accept a TM-pair
//   + gasneti_boundscheck()
//   + gasneti_boundscheck_allowoutseg()
//   + gasneti_formattm()
//   + gasneti_pshm_local_rank()
//   + gasneti_pshm_in_supernode()
//   + gasneti_pshm_addr2local()
//   + GASNETI_NBRHD_LOCAL()
//   + GASNETI_NBRHD_LOCAL_ADDR()
//   + GASNETI_NBRHD_LOCAL_ADDR_OR_NULL()

#if GASNET_DEBUG
  GASNETI_INLINE(gasneti_assertvalid_tm_pair)
  void gasneti_assertvalid_tm_pair(gasneti_TM_Pair_t _tm_pair) {
    // TODO: check that client index (currently always zero) is in range
    gasneti_assert_uint((_tm_pair & 0xff) ,==, 1);
  }
#else
  #define gasneti_assertvalid_tm_pair(_tm_pair) ((void)0)
#endif

// Detect a TM generated by gex_TM_Pair
GASNETI_INLINE(gasneti_i_tm_is_pair)
int gasneti_i_tm_is_pair(gasneti_TM_t _i_tm)
{
  gasneti_TM_Pair_t _tm_pair = (gasneti_TM_Pair_t) _i_tm;
  int _result = (_tm_pair & 1);
  if (_result) gasneti_assertvalid_tm_pair(_tm_pair);
  return _result;
}
#define gasneti_e_tm_is_pair(_e_tm) gasneti_i_tm_is_pair(gasneti_import_tm(_e_tm))

// Assertion-checking conversions from {gex,gasneti}_TM_t to gasneti_TM_Pair_t
GASNETI_INLINE(gasneti_i_tm_to_pair)
gasneti_TM_Pair_t gasneti_i_tm_to_pair(gasneti_TM_t _i_tm)
{
  gasneti_TM_Pair_t _tm_pair = (gasneti_TM_Pair_t) _i_tm;
  gasneti_assertvalid_tm_pair(_tm_pair);
  return _tm_pair;
}
#define gasneti_e_tm_to_pair(_e_tm) gasneti_i_tm_to_pair(gasneti_import_tm(_e_tm))

// Extract EP indices from a known TM_Pair
GASNETI_INLINE(gasneti_tm_pair_loc_idx)
gex_EP_Index_t gasneti_tm_pair_loc_idx(gasneti_TM_Pair_t _tm_pair)
{
  gasneti_assertvalid_tm_pair(_tm_pair);
  return (_tm_pair >> GASNETI_TM_PAIR_LOC_IDX_SHIFT) & GASNETI_TM_PAIR_IDX_MASK;
}
GASNETI_INLINE(gasneti_tm_pair_rem_idx)
gex_EP_Index_t gasneti_tm_pair_rem_idx(gasneti_TM_Pair_t _tm_pair)
{
  gasneti_assertvalid_tm_pair(_tm_pair);
  return (_tm_pair >> GASNETI_TM_PAIR_REM_IDX_SHIFT) & GASNETI_TM_PAIR_IDX_MASK;
}


// Given (tm,rank) return the jobrank or ep_location
extern GASNETI_PURE gex_Rank_t        gasneti_tm_fwd_rank(gasneti_TM_t _tm, gex_Rank_t _rank);
GASNETI_PUREP(gasneti_tm_fwd_rank)
extern GASNETI_PURE gex_EP_Location_t gasneti_tm_fwd_location(gasneti_TM_t _tm, gex_Rank_t _rank, gex_Flags_t _flags);
GASNETI_PUREP(gasneti_tm_fwd_location)

// Given (tm,jobrank) return the rank of jobrank in tm, or GEX_RANK_INVALID
extern gex_Rank_t gasneti_tm_rev_rank(gasneti_TM_t _tm, gex_Rank_t _jobrank);

// Helpers which deal correctly/transparently with TM-pair
GASNETI_INLINE(gasneti_e_tm_size)
gex_Rank_t gasneti_e_tm_size(gex_TM_t _e_tm) {
  gasneti_assert(_e_tm);
  return gasneti_e_tm_is_pair(_e_tm) ? gex_System_QueryJobSize() : gex_TM_QuerySize(_e_tm);
}
GASNETI_INLINE(gasneti_i_tm_size)
gex_Rank_t gasneti_i_tm_size(gasneti_TM_t _i_tm) {
  gasneti_assert(_i_tm);
  return gasneti_i_tm_is_pair(_i_tm) ? gex_System_QueryJobSize() : _i_tm->_size;
}

GASNETI_INLINE(gasneti_i_tm_to_ep_index)
gex_Rank_t gasneti_i_tm_to_ep_index(gasneti_TM_t _i_tm) {
  gasneti_assert(_i_tm);
  if (gasneti_is_tm0(_i_tm)) {
    return 0; // fast path
  } else if (gasneti_i_tm_is_pair(_i_tm)) {
    return gasneti_tm_pair_loc_idx(gasneti_i_tm_to_pair(_i_tm));
  } else {
    return _i_tm->_ep->_index;
  }
}
#define gasneti_e_tm_to_ep_index(_e_tm) gasneti_i_tm_to_ep_index(gasneti_import_tm(_e_tm))

#if GASNET_DEBUG
  #define gasneti_check_jobrank(jobrank) \
    gasneti_assert_uint(jobrank ,<, gex_System_QueryJobSize());
  GASNETI_INLINE(gasneti_check_e_tm_rank)
  void gasneti_check_e_tm_rank(gex_TM_t _e_tm, gex_Rank_t _rank) {
    gasneti_assert(_e_tm);
    gasneti_assert_uint(_rank ,<, gasneti_e_tm_size(_e_tm));
  }
  GASNETI_INLINE(gasneti_check_i_tm_rank)
  void gasneti_check_i_tm_rank(gasneti_TM_t _i_tm, gex_Rank_t _rank) {
    gasneti_assert(_i_tm);
    gasneti_assert_uint(_rank ,<, gasneti_i_tm_size(_i_tm));
  }
#else
  #define gasneti_check_jobrank(jobrank) ((void)0)
  #define gasneti_check_i_tm_rank(tm,rank) ((void)0)
  #define gasneti_check_e_tm_rank(tm,rank) ((void)0)
#endif

// TODO-EX: remove when a runtime branch on tm->_rank_map is necessary
#define GASNETI_ALLOW_SPARSE_TEAMREP 0

GASNETI_INLINE(gasneti_i_tm_rank_to_jobrank)
gex_Rank_t gasneti_i_tm_rank_to_jobrank(gasneti_TM_t _i_tm, gex_Rank_t _rank) {
  gasneti_check_i_tm_rank(_i_tm, _rank);
  if (gasneti_is_tm0(_i_tm) || gasneti_i_tm_is_pair(_i_tm)) return _rank;
  if (!GASNETI_ALLOW_SPARSE_TEAMREP || _i_tm->_rank_map) {
    gasneti_assert(_i_tm->_rank_map);
    return _i_tm->_rank_map[_rank];
  }
  return gasneti_tm_fwd_rank(_i_tm, _rank);
}
#define gasneti_e_tm_rank_to_jobrank(e_tm,rank) \
        gasneti_i_tm_rank_to_jobrank(gasneti_import_tm(e_tm),rank)

GASNETI_INLINE(gasneti_i_tm_rank_to_ep_index)
gex_Rank_t gasneti_i_tm_rank_to_ep_index(gasneti_TM_t _i_tm, gex_Rank_t _rank) {
  gasneti_check_i_tm_rank(_i_tm, _rank);
  gex_EP_Index_t _result;
  if (gasneti_is_tm0(_i_tm)) {
    _result = 0;
  } else if (gasneti_i_tm_is_pair(_i_tm)) {
    _result = gasneti_tm_pair_rem_idx(gasneti_i_tm_to_pair(_i_tm));
  } else if (!GASNETI_ALLOW_SPARSE_TEAMREP || _i_tm->_rank_map) {
    // NULL _index_map indicates all members of TM are primordial EPs (idx==0)
    _result = _i_tm->_index_map ? _i_tm->_index_map[_rank] : 0;
  } else {
    gex_EP_Location_t _loc = gasneti_tm_fwd_location(_i_tm, _rank, 0);
    _result = _loc.gex_ep_index;
  }
  gasneti_assert(_result < GASNET_MAXEPS);
  return _result;
}
#define gasneti_e_tm_rank_to_ep_index(e_tm,rank) \
        gasneti_i_tm_rank_to_ep_index(gasneti_import_tm(e_tm),rank)

GASNETI_INLINE(gasneti_i_tm_rank_to_location)
gex_EP_Location_t gasneti_i_tm_rank_to_location(gasneti_TM_t _i_tm, gex_Rank_t _rank, gex_Flags_t _flags) {
  gasneti_check_i_tm_rank(_i_tm, _rank);
  gex_EP_Location_t _result;
  if (gasneti_is_tm0(_i_tm)) {
    _result.gex_rank = _rank;
    _result.gex_ep_index = 0;
  } else if (gasneti_i_tm_is_pair(_i_tm)) {
    _result.gex_rank = _rank;
    _result.gex_ep_index = gasneti_tm_pair_rem_idx(gasneti_i_tm_to_pair(_i_tm));
  } else if (!GASNETI_ALLOW_SPARSE_TEAMREP || _i_tm->_rank_map) {
    gasneti_assert(_i_tm->_rank_map);
    _result.gex_rank = _i_tm->_rank_map[_rank];
    // NULL _index_map indicates all members of TM are primordial EPs (idx==0)
    _result.gex_ep_index = _i_tm->_index_map ? _i_tm->_index_map[_rank] : 0;
  } else {
    _result = gasneti_tm_fwd_location(_i_tm, _rank, _flags);
  }
  return _result;
}
#define gasneti_e_tm_rank_to_location(e_tm,rank,flags) \
        gasneti_i_tm_rank_to_location(gasneti_import_tm(e_tm),rank,flags)

GASNETI_INLINE(gasneti_i_tm_jobrank_to_rank)
gex_Rank_t gasneti_i_tm_jobrank_to_rank(gasneti_TM_t _i_tm, gex_Rank_t _jobrank) {
  gasneti_assert(_i_tm);
  gasneti_assert_uint(_jobrank ,<, gex_System_QueryJobSize());
  if (gasneti_is_tm0(_i_tm) || gasneti_i_tm_is_pair(_i_tm)) return _jobrank;
  return gasneti_tm_rev_rank(_i_tm, _jobrank);
}
#define gasneti_e_tm_jobrank_to_rank(e_tm,jobrank) \
        gasneti_i_tm_jobrank_to_rank(gasneti_import_tm(e_tm),jobrank)

extern gasnet_seginfo_t *gasneti_seginfo;
extern gasnet_seginfo_t *gasneti_seginfo_aux;
extern gasnet_seginfo_t *gasneti_seginfo_tbl[GASNET_MAXEPS];

// TODO: work towards dropping non-scalable seginfo tables
GASNETI_INLINE(gasneti_client_seginfo)
const gasnet_seginfo_t *gasneti_client_seginfo(gex_TM_t _e_tm, gex_Rank_t _rank) {
  gex_EP_Location_t _loc = gasneti_e_tm_rank_to_location(_e_tm, _rank, 0);
  gex_Rank_t _jobrank = _loc.gex_rank;
  gex_EP_Index_t _idx = _loc.gex_ep_index;
  gasnet_seginfo_t *_si_array = gasneti_seginfo_tbl[_idx];
  gasneti_assert(_si_array);
  return _si_array + _jobrank;
}
GASNETI_INLINE(gasneti_aux_seginfo)
const gasnet_seginfo_t *gasneti_aux_seginfo(gex_Rank_t _jobrank) {
  return gasneti_seginfo_aux + _jobrank;
}

GASNETI_INLINE(_gasneti_in_seginfo_t)
int _gasneti_in_seginfo_t(const void *_ptr, size_t _nbytes, const gasnet_seginfo_t *_seginfo) {
  gasneti_assert(_nbytes); // precondition to avoid "fence post" error at top of segment
  uintptr_t _uptr = (uintptr_t)_ptr;
  uintptr_t _segbase = (uintptr_t)_seginfo->addr;
  return (_uptr >= _segbase && (_uptr + _nbytes) <= (_segbase + _seginfo->size));
}

GASNETI_INLINE(_gasneti_in_segment_t)
int _gasneti_in_segment_t(const void *_ptr, size_t _nbytes, const gex_Segment_t _segment) {
  gasneti_assert(_nbytes); // precondition to avoid "fence post" error at top of segment
  uintptr_t _uptr = (uintptr_t)_ptr;
  gasneti_Segment_t _i_seg = gasneti_import_segment(_segment);
  return (_uptr >= (uintptr_t)(_i_seg->_addr) && (_uptr + _nbytes) <= (uintptr_t)_i_seg->_ub);
}

#if GASNET_SEGMENT_EVERYTHING
  // Do not remove calls to gasneti_inseg_helper() or reduce it to a macro.
  // The function call ensures ptr and nbytes evaluated exactly once for possible side-effects.
  GASNETI_INLINE(gasneti_inseg_helper)
  void gasneti_inseg_helper(const void *_ptr, size_t _nbytes) {
    gasneti_assert(_nbytes);
    gasneti_assert(_ptr);
  }
  #define gasneti_in_clientsegment(e_tm,rank,ptr,nbytes) \
          (gasneti_inseg_helper(ptr,nbytes),gasneti_check_e_tm_rank(e_tm,rank), 1)
  #define gasneti_in_auxsegment(jobrank,ptr,nbytes) \
          (gasneti_inseg_helper(ptr,nbytes),gasneti_check_jobrank(jobrank), 1)
  #define gasneti_in_fullsegment(e_tm,rank,ptr,nbytes) \
          (gasneti_inseg_helper(ptr,nbytes), gasneti_check_e_tm_rank(e_tm,rank), 1)
#else
  #define gasneti_in_clientsegment(e_tm,rank,ptr,nbytes) \
          _gasneti_in_seginfo_t(ptr,nbytes,gasneti_client_seginfo(e_tm,rank))
  #define gasneti_in_auxsegment(jobrank,ptr,nbytes) \
          _gasneti_in_seginfo_t(ptr,nbytes,gasneti_aux_seginfo(jobrank))
  GASNETI_INLINE(gasneti_in_fullsegment)
  int gasneti_in_fullsegment(gex_TM_t _e_tm, gex_Rank_t _rank, const void *_ptr, size_t _nbytes) {
    return gasneti_in_clientsegment(_e_tm, _rank, _ptr, _nbytes) ||
           gasneti_in_auxsegment(gasneti_e_tm_rank_to_jobrank(_e_tm, _rank), _ptr, _nbytes);
  }
#endif

// Local-only in-segment checks, taking a gasneti_EP_t to name the local endpoint
#if GASNET_SEGMENT_EVERYTHING
  #define gasneti_in_local_auxsegment(ep,ptr,nbytes)       1
  #define gasneti_in_local_clientsegment(ep,ptr,nbytes)    1
  #define gasneti_in_local_fullsegment(ep,ptr,nbytes)      1
#else
  GASNETI_INLINE(gasneti_in_local_auxsegment)
  int gasneti_in_local_auxsegment(gasneti_EP_t _ep, void *_ptr, size_t _nbytes) {
    // TODO-EX: needs update for multi-{EP,Segment}
    return _gasneti_in_seginfo_t(_ptr, _nbytes, &gasneti_seginfo_aux[gasneti_mynode]);
  }
  GASNETI_INLINE(gasneti_in_local_clientsegment)
  int gasneti_in_local_clientsegment(gasneti_EP_t _ep, void *_ptr, size_t _nbytes) {
    gex_Segment_t _segment = gasneti_export_segment(_ep->_segment);
    return _segment && _gasneti_in_segment_t(_ptr, _nbytes, _segment);
  }
  GASNETI_INLINE(gasneti_in_local_fullsegment)
  int gasneti_in_local_fullsegment(gasneti_EP_t _ep, void *_ptr, size_t _nbytes) {
    return gasneti_in_local_clientsegment(_ep, _ptr, _nbytes) ||
           gasneti_in_local_auxsegment(_ep, _ptr, _nbytes);
  }
#endif

#ifdef _INCLUDED_GASNET_INTERNAL_H
  /* default for GASNet implementation is to check against union of client and aux segments */
  #define gasneti_in_segment gasneti_in_fullsegment
  #define gasneti_in_local_segment gasneti_in_local_fullsegment
#else
  /* default for client is to check against just the client seg */
  #define gasneti_in_segment gasneti_in_clientsegment
  #define gasneti_in_local_segment gasneti_in_local_clientsegment
#endif

#ifdef GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET
  /* in-segment check for internal put/gets that may exploit outofseg support */
  #define gasneti_in_segment_allowoutseg(e_tm,rank,ptr,nbytes) \
          (gasneti_check_e_tm_rank(e_tm,rank), 1)
#else
  #define gasneti_in_segment_allowoutseg  gasneti_in_segment
#endif

#define _gasneti_boundscheck(e_tm,rank,ptr,nbytes,segtest) do {         \
    gex_TM_t _gex_bc_tm = (e_tm);                                              \
    gasneti_assert(_gex_bc_tm);                                                \
    gex_Rank_t _gex_bc_rank = (rank);                                          \
    gex_Rank_t _gex_bc_size = gasneti_e_tm_size(_gex_bc_tm);                   \
    const void *_gex_bc_ptr = (const void *)(ptr);                             \
    size_t _gex_bc_nbytes = (size_t)(nbytes);                                  \
    gasneti_assert(_gex_bc_nbytes); /* avoids "fence post" error */            \
    if_pf (_gex_bc_rank >= _gex_bc_size)                                       \
      gasneti_fatalerror("Rank out of range (%lu >= %lu)",                     \
              (unsigned long)_gex_bc_rank, (unsigned long)(_gex_bc_size));     \
    if_pf (_gex_bc_ptr == NULL ||                                              \
           !segtest(_gex_bc_tm,_gex_bc_rank,_gex_bc_ptr,_gex_bc_nbytes)) {     \
      const gasnet_seginfo_t *_gex_bc_client_seg =                             \
                       gasneti_client_seginfo(_gex_bc_tm,_gex_bc_rank);        \
      gex_Rank_t _gex_bc_jobrank = gasneti_e_tm_rank_to_jobrank(_gex_bc_tm, _gex_bc_rank); \
      const gasnet_seginfo_t *_gex_bc_aux_seg =                                \
                                         gasneti_aux_seginfo(_gex_bc_jobrank); \
      gasneti_fatalerror("Remote address out of range (" GASNETI_TMRANKFMT     \
         " ptr=" GASNETI_LADDRFMT" nbytes=%" PRIuPTR ")"                       \
         "\n  clientsegment=(" GASNETI_LADDRFMT"..." GASNETI_LADDRFMT")"       \
         "\n     auxsegment=(" GASNETI_LADDRFMT"..." GASNETI_LADDRFMT")",      \
         GASNETI_TMRANKSTR(_gex_bc_tm,_gex_bc_rank),                           \
         GASNETI_LADDRSTR(_gex_bc_ptr),                                        \
         (uintptr_t)_gex_bc_nbytes,                                            \
         GASNETI_LADDRSTR(_gex_bc_client_seg->addr),                           \
         GASNETI_LADDRSTR((uintptr_t)_gex_bc_client_seg->addr +                \
                                     _gex_bc_client_seg->size),                \
         GASNETI_LADDRSTR(_gex_bc_aux_seg->addr),                              \
         GASNETI_LADDRSTR((uintptr_t)_gex_bc_aux_seg->addr +                   \
                                     _gex_bc_aux_seg->size)                    \
         );                                                                    \
    }                                                                          \
  } while(0)


// gasneti_boundscheck() and gasneti_boundscheck_allowoutseg()
#if GASNET_NDEBUG
  #define gasneti_boundscheck(e_tm,rank,ptr,nbytes) ((void)0)
  #define gasneti_boundscheck_allowoutseg(e_tm,rank,ptr,nbytes) ((void)0)
#else
  #define gasneti_boundscheck(e_tm,rank,ptr,nbytes) \
         _gasneti_boundscheck(e_tm,rank,ptr,nbytes,gasneti_in_segment)
  #define gasneti_boundscheck_allowoutseg(e_tm,rank,ptr,nbytes) \
         _gasneti_boundscheck(e_tm,rank,ptr,nbytes,gasneti_in_segment_allowoutseg)
#endif

/* make a GASNet core API call - if it fails, print error message and abort */
#ifndef GASNETI_SAFE
#define GASNETI_SAFE(fncall) do {                                            \
   int _retcode = (fncall);                                                  \
   if_pf (_retcode != (int)GASNET_OK) {                                      \
     gasneti_fatalerror("\nGASNet encountered an error: %s(%i)\n"            \
        "  while calling: %s",                                               \
        gasnet_ErrorName(_retcode), _retcode, #fncall);                      \
   }                                                                         \
 } while (0)
#endif

/* ------------------------------------------------------------------------------------ */
/* tools for dealing with gex_Event_t* pre-defined values */
// TODO-EX: move to gasnet_event_internal.h
#ifndef _GEX_EVENT_T
GASNETI_INLINE(gasneti_leaf_is_pointer) GASNETI_PURE
int gasneti_leaf_is_pointer(const gex_Event_t *_opt_val) {
  gasneti_assert(_opt_val != NULL);
  return ((uintptr_t)(_opt_val) >= (uintptr_t)4);
}
GASNETI_PUREP(gasneti_leaf_is_pointer)

GASNETI_INLINE(gasneti_leaf_finish)
void gasneti_leaf_finish(gex_Event_t *_opt_val) {
  if (gasneti_leaf_is_pointer(_opt_val)) *_opt_val = GEX_EVENT_INVALID;
}
#endif

/* ------------------------------------------------------------------------------------ */
/* semi-portable spinlocks using gasneti_atomic_t
   This useful primitive is not available on all platforms and it therefore reserved 
   for internal use only.

   On platforms where implemented, the following are roughly equivalent to the
   corresponding pthread_mutex_* calls:
     GASNETI_SPINLOCK_INITIALIZER
     gasneti_spinlock_{init,destroy,lock,unlock,trylock}
   The functions return 0 on success to match the corresponding pthread_mutex functions.

   There is no gasneti_spinlock_t, these functions operate on gasneti_atomic_t.
   
   Unlike the pthread_mutex, the use of spinlocks have no fairness guarantees.  For
   instance, it would be perfectly legal for a race to always grant the lock to the CPU
   which "owns" the associated memory.  Therefore, spinlocks must be used with care.
   Also unlike pthread_mutex, it is safe to unlock one from signal context.  Though
   trying to acquire a spinlock in signal context is legal, it is dangerous.

   Like the pthread_mutex, these operations perform an RMB() when acquiring the
   lock and a WMB() when releasing it.  So, no additional memory barriers are
   required when using these locks to protect one's data structures.

   GASNETI_HAVE_SPINLOCK will be defined to 1 on platforms supporting this primitive.
 */
#if 0
  /* TODO Some platforms may have cheaper implementations than atomic-CAS. */
  /* eg. some platforms (SPARC?) can support spinlock using test-and-set */
#elif defined(GASNETI_ATOMICOPS_NOT_SIGNALSAFE)
  /* We don't implement this case due to lack of signal safety */
#elif defined(GASNETI_HAVE_ATOMIC_CAS)
  #if GASNET_DEBUG
    #define GASNETI_SPINLOCK_LOCKED	0xa5a5
    #define GASNETI_SPINLOCK_UNLOCKED	0xaa55
    #define GASNETI_SPINLOCK_DESTROYED	0xDEAD
    GASNETI_INLINE(gasneti_spinlock_is_valid)
    int gasneti_spinlock_is_valid(gasneti_atomic_t *_plock) {
      uint32_t _tmp = gasneti_atomic_read(_plock, GASNETI_ATOMIC_RMB_PRE);
      if_pf (_tmp == GASNETI_SPINLOCK_DESTROYED)
        gasneti_fatalerror("Detected use of destroyed spinlock");
      if_pf (!((_tmp == GASNETI_SPINLOCK_LOCKED) || (_tmp == GASNETI_SPINLOCK_UNLOCKED)))
        gasneti_fatalerror("Detected use of uninitialized or corrupted spinlock");
      return 1;
    }
    GASNETI_INLINE(gasneti_spinlock_is_locked)
    int gasneti_spinlock_is_locked(gasneti_atomic_t *_plock) {
      uint32_t _tmp = gasneti_atomic_read(_plock, GASNETI_ATOMIC_RMB_PRE);
      return (_tmp == GASNETI_SPINLOCK_LOCKED);
    }
  #else
    #define GASNETI_SPINLOCK_LOCKED	1
    #define GASNETI_SPINLOCK_UNLOCKED	0
    #define gasneti_spinlock_is_valid(plock) 1
  #endif
  #define GASNETI_SPINLOCK_INITIALIZER gasneti_atomic_init(GASNETI_SPINLOCK_UNLOCKED)
  #define gasneti_spinlock_init(plock) \
      gasneti_atomic_set((plock), GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_WMB_POST)
  #define gasneti_spinlock_destroy(plock) \
      gasneti_assert(gasneti_atomic_compare_and_swap(plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_DESTROYED, GASNETI_ATOMIC_WMB_POST))
  #define gasneti_spinlock_lock(plock) do {                                     \
      gasneti_waituntil(                                                        \
	!gasneti_spinlock_is_valid(plock) ||                                    \
        gasneti_atomic_compare_and_swap(plock,                                  \
          GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_LOCKED, 0)                \
      ); /* Acquire: the rmb() is in the gasneti_waituntil() */                 \
      gasneti_assert(gasneti_spinlock_is_locked(plock));                        \
  } while (0)
  GASNETI_INLINE(gasneti_spinlock_unlock)
  int gasneti_spinlock_unlock(gasneti_atomic_t *_plock) {
      #if GASNET_DEBUG
        /* Using CAS for release is more costly, but adds validation */
        gasneti_assert(gasneti_atomic_compare_and_swap(_plock, GASNETI_SPINLOCK_LOCKED, GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_REL));
      #else
        gasneti_atomic_set(_plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_REL);
      #endif
      return 0;
  }
  /* return 0/EBUSY on success/failure to match pthreads */
  GASNETI_INLINE(gasneti_spinlock_trylock) GASNETI_WARN_UNUSED_RESULT
  int gasneti_spinlock_trylock(gasneti_atomic_t *_plock) {
      gasneti_assert(gasneti_spinlock_is_valid(_plock));
      if ((GASNETI_SPINLOCK_UNLOCKED == gasneti_atomic_read(_plock, 0)) &&
          gasneti_atomic_compare_and_swap(_plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_LOCKED, GASNETI_ATOMIC_ACQ_IF_TRUE)) {
	  gasneti_assert(gasneti_spinlock_is_locked(_plock));
	  return 0;
      } else {
	  return EBUSY;
      }
  }
  #define GASNETI_HAVE_SPINLOCK 1
#else
  /* Here we use a binary semaphore */
  #define GASNETI_SPINLOCK_UNLOCKED	1
  #define GASNETI_SPINLOCK_DESTROYED	2
  #if GASNET_DEBUG
    GASNETI_INLINE(gasneti_spinlock_is_valid)
    int gasneti_spinlock_is_valid(gasneti_atomic_t *_plock) {
      uint32_t _tmp = gasneti_atomic_read(_plock, GASNETI_ATOMIC_RMB_PRE);
      if_pf (_tmp == GASNETI_SPINLOCK_DESTROYED)
        gasneti_fatalerror("Detected use of destroyed spinlock");
      return 1;
    }
    GASNETI_INLINE(gasneti_spinlock_is_locked)
    int gasneti_spinlock_is_locked(gasneti_atomic_t *_plock) {
      gasneti_atomic_val_t _tmp = gasneti_atomic_read(_plock, GASNETI_ATOMIC_RMB_PRE);
      return (_tmp != GASNETI_SPINLOCK_UNLOCKED);
    }
  #else
    #define gasneti_spinlock_is_valid(plock) 1
  #endif
  #define GASNETI_SPINLOCK_INITIALIZER gasneti_atomic_init(GASNETI_SPINLOCK_UNLOCKED)
  #define gasneti_spinlock_init(plock) \
      gasneti_atomic_set((plock), GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_WMB_POST)
  #define gasneti_spinlock_destroy(plock) do {                                          \
      gasneti_assert(!gasneti_spinlock_is_locked(plock));                               \
      gasneti_atomic_set((plock), GASNETI_SPINLOCK_DESTROYED, GASNETI_ATOMIC_WMB_POST); \
  } while (0)
  GASNETI_INLINE(_gasneti_spinlock_try) GASNETI_WARN_UNUSED_RESULT
  int _gasneti_spinlock_try(gasneti_atomic_t *_plock) {
    gasneti_assert(gasneti_spinlock_is_valid(_plock));
    return (gasneti_atomic_read(_plock, 0) == GASNETI_SPINLOCK_UNLOCKED) &&
           gasneti_atomic_decrement_and_test(_plock, GASNETI_ATOMIC_ACQ_IF_TRUE);
  }
  /* Ick: forward reference to GASNETI_WAITHOOK only works because this is a macro */
  #define gasneti_spinlock_lock(plock) do { \
    while (!_gasneti_spinlock_try(plock)) { \
      GASNETI_WAITHOOK();                   \
    }                                       \
  } while (0)
  GASNETI_INLINE(gasneti_spinlock_unlock)
  int gasneti_spinlock_unlock(gasneti_atomic_t *_plock) {
    gasneti_assert(gasneti_spinlock_is_locked(_plock));
    gasneti_atomic_set(_plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_REL);
    return 0;
  }
  /* return 0/EBUSY on success/failure to match pthreads */
  GASNETI_INLINE(gasneti_spinlock_trylock) GASNETI_WARN_UNUSED_RESULT
  int gasneti_spinlock_trylock(gasneti_atomic_t *_plock) {
    return _gasneti_spinlock_try(_plock) ? 0 : EBUSY;
  }
  #define GASNETI_HAVE_SPINLOCK 1
#endif

/* ------------------------------------------------------------------------------------ */
/* public threadinfo support */

#if GASNETI_CLIENT_THREADS
  #if defined GASNETI_THREADINFO_OPT_CONFIGURE
    /* take user-specified value, if any*/
    #undef GASNETI_THREADINFO_OPT
    #define GASNETI_THREADINFO_OPT GASNETI_THREADINFO_OPT_CONFIGURE
  #elif defined GASNETI_THREADINFO_OPT
    /* keep conduit-specified value */
  #elif defined GASNETI_HAVE_TLS_SUPPORT
    /* Default to OFF on some common ABIs with good TLS support */
    #if (PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC) && \
        (PLATFORM_OS_DARWIN || PLATFORM_OS_SOLARIS || PLATFORM_OS_LINUX || PLATFORM_OS_CNL)
      #define GASNETI_THREADINFO_OPT    0
    #elif PLATFORM_ARCH_POWERPC && \
          PLATFORM_OS_LINUX
      #define GASNETI_THREADINFO_OPT    0
    #endif
  #endif
  #ifndef GASNETI_THREADINFO_OPT
    /* Default to ON if not set anywhere above */
    #define GASNETI_THREADINFO_OPT      1
  #endif
  #ifndef GASNETI_LAZY_BEGINFUNCTION
  #define GASNETI_LAZY_BEGINFUNCTION  1
  #endif
#endif

#if GASNETI_THREADINFO_OPT
  /* Here we use a clever trick - GASNET_GET_THREADINFO() uses the sizeof(_gasneti_threadinfo_available)
      to determine whether gasneti_threadinfo_cache was bound a value posted by GASNET_POST_THREADINFO()
      of if it bound to the globally declared dummy variables. 
     Even a very stupid C optimizer should constant-fold away the unused calls to gasneti_get_threadinfo() 
      and discard the unused variables
     We need 2 separate variables to ensure correct name-binding semantics for GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
   */
  #if PLATFORM_COMPILER_PGI_CXX
    // Add a redundant value use to avoid a 550 set-but-not-used warning 
    #define _GASNETI_THREAD_POSTED (sizeof(_gasneti_threadinfo_available) > 1 \
                                    && !_gasneti_threadinfo_available)
  #else
    #define _GASNETI_THREAD_POSTED (sizeof(_gasneti_threadinfo_available) > 1)
  #endif
  static gasnet_threadinfo_t _gasneti_threadinfo_cache = 0;
  static uint8_t             _gasneti_threadinfo_available = 
    sizeof(_gasneti_threadinfo_cache) + sizeof(_gasneti_threadinfo_available);
    /* silly little trick to prevent unused variable warning on gcc -Wall */

  // tmp variable below solves scoping problems on cache for expressions like:
  //   GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
  // where the cache from an enclosing scope is consulted by that GET
  #define GASNET_POST_THREADINFO(info)                      \
    gasnet_threadinfo_t const _gasneti_threadinfo_tmp = (info); \
    gasnet_threadinfo_t _gasneti_threadinfo_cache = _gasneti_threadinfo_tmp; \
    uint32_t _gasneti_threadinfo_available = 0
    /* if you get an unused variable warning on _gasneti_threadinfo_available, 
       it means you POST'ed in a function which made no GASNet calls that needed it
       So, PLEASE don't add __unused__ annotations here. */

  #if GASNETI_LAZY_BEGINFUNCTION
    // bug 3498: Ensure a sequence point after the assignment to _gasneti_threadinfo_cache
    GASNETI_INLINE(gasneti_lazy_get_threadinfo) GASNETI_WARN_UNUSED_RESULT
    gasnet_threadinfo_t gasneti_lazy_get_threadinfo(gasnet_threadinfo_t * const _p_ti, 
                                                    gasnet_threadinfo_t _ti_val) {
      return (*_p_ti = _ti_val);
    }
    // memoized implementation of GET POSTED, where BEGIN_FUNCTION delays lookup to first use
    #define _GASNETI_GET_THREADINFO_POSTED()                           \
      ( GASNETT_PREDICT_TRUE(_gasneti_threadinfo_cache) ? _gasneti_threadinfo_cache :   \
        gasneti_lazy_get_threadinfo(&_gasneti_threadinfo_cache,(gasnet_threadinfo_t)_gasneti_mythread_slow()) \
      ) 
  #else
    // straightforward implementation of GET POSTED
    #define _GASNETI_GET_THREADINFO_POSTED() ( _gasneti_threadinfo_cache )
  #endif

  // GASNET_GET_THREADINFO: the main cached-lookup workhorse
  // note both the branches textually below are always statically resolved
  #define GASNET_GET_THREADINFO()                                      \
    ( _GASNETI_THREAD_POSTED ? /* have POST or BEGIN_FUNCTION ? */     \
      _GASNETI_GET_THREADINFO_POSTED() /* use it */ :                  \
      ( _GASNETI_THREAD_FARG_AVAILABLE ? /* in FARG context? use. */   \
         (gasnet_threadinfo_t)(uintptr_t)_GASNETI_THREAD_FARG_NAME :   \
         (gasnet_threadinfo_t)_gasneti_mythread_slow() /* lookup */ )  \
    )     

  /* the gasnet_threadinfo_t pointer points to a thread data-structure owned by
     the extended API, whose first element is a pointer reserved
     for use by the core API (initialized to NULL)
   */

  #if GASNETI_LAZY_BEGINFUNCTION
    /* postpone thread discovery to first use */
    #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(0)
  #else
    #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
  #endif

#else
  #if GASNET_DEBUG
    #define GASNET_POST_THREADINFO(info)   \
      static void *_gasneti_threadinfo_dummy = /* diagnose duplicate POST in a scope */ \
                  (void *)&_gasneti_threadinfo_dummy;
  #else
    #define GASNET_POST_THREADINFO(info) ((void)0)
  #endif
  #define GASNET_GET_THREADINFO() (NULL)
  #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
#endif

/* ------------------------------------------------------------------------------------ */
/* thread-id optimization support */

#if GASNETI_THREADINFO_OPT
  #if GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS
    #define _GASNETI_THREAD_FARG_RTYPE  gasnet_threadinfo_t 
  #else
    #define _GASNETI_THREAD_FARG_RTYPE  void *
  #endif
  #define _GASNETI_THREAD_FARG_TYPE    _GASNETI_THREAD_FARG_RTYPE const GASNETI_RESTRICT
  #define _GASNETI_THREAD_FARG_NAME    _gasneti_threadinfo_farg
  static uint8_t _GASNETI_THREAD_FARG_NAME = sizeof(_GASNETI_THREAD_FARG_NAME);
  #define _GASNETI_THREAD_FARG_AVAILABLE  (sizeof(_GASNETI_THREAD_FARG_NAME) > 1)
  // -----------------------------------------------------------------------------------------
  // *** Propagating info into GASNETI_THREAD_FARG function context ***
  
  // GASNETI_THREAD_FARG(_ALONE): use to declare the threadinfo hidden arg as part of a function declaration
  #define GASNETI_THREAD_FARG         , GASNETI_THREAD_FARG_ALONE
  #define GASNETI_THREAD_FARG_ALONE   _GASNETI_THREAD_FARG_TYPE _GASNETI_THREAD_FARG_NAME

  // GASNETI_THREAD_GET(_ALONE): retrieve the threadinfo (when GASNETI_THREADINFO_OPT) from one of:
  //     a prior GASNET_POST_THREADINFO, an FARG to the enclosing function, or dynamic lookup
  // and pass as the hidden argument to a function declared using GASNETI_THREAD_FARG(_ALONE)
  #define GASNETI_THREAD_GET          , GASNETI_THREAD_GET_ALONE
  #define GASNETI_THREAD_GET_ALONE    GASNET_GET_THREADINFO()

  // -----------------------------------------------------------------------------------------
  // *** Inside FARG/POST'd context ***
  //
  //   The macros in this section should ONLY be used by code inside an FARG/POST'd context, ie:
  //    1. inside functions declared using GASNETI_THREAD_FARG*.
  //    2. within the lexical scope of a GASNET_BEGIN_FUNCTION() or GASNET_POST_THREADINFO()
  //   Otherwise, they generate a compiler diagnostic in GASNETI_THREADINFO_OPT mode.
  //   the error looks something like: "error: size of array is negative"
  //   This is helpful in contexts where we wish to statically ensure lack of dynamic lookup.
 
  // GASNETI_THREAD_PASS(_ALONE): propagate the threadinfo to a callee declared with GASNETI_THREAD_FARG*
  //   this only differs from GASNETI_THREAD_GET* in that it statically requires FARG/POST'd context (and no lookup)
  #define GASNETI_THREAD_PASS         , GASNETI_THREAD_PASS_ALONE
  #define GASNETI_THREAD_PASS_ALONE ( gasneti_static_assert(_GASNETI_THREAD_FARG_AVAILABLE||_GASNETI_THREAD_POSTED), \
                                     ( _GASNETI_THREAD_POSTED ?                                      \
                                      (_GASNETI_THREAD_FARG_RTYPE)_GASNETI_GET_THREADINFO_POSTED() : \
                                      (_GASNETI_THREAD_FARG_RTYPE)(uintptr_t)_GASNETI_THREAD_FARG_NAME ) )

  // GASNETI_MYTHREAD: retrieve the value of the threadinfo as a (gasneti_threaddata_t *),
  //                   suitable for access to threaddata fields from internal code
  #define GASNETI_MYTHREAD  (  gasneti_static_assert(_GASNETI_THREAD_FARG_AVAILABLE||_GASNETI_THREAD_POSTED), \
                             (_GASNETI_THREAD_POSTED ?                                           \
                              (struct _gasneti_threaddata_t *)_GASNETI_GET_THREADINFO_POSTED() : \
                              (struct _gasneti_threaddata_t *)(uintptr_t)_GASNETI_THREAD_FARG_NAME ) )

  // -----------------------------------------------------------------------------------------
  // *** Declaring GASNETI_THREAD_FARG context : NO LONGER SUPPORTED ***
  
  // Former macros:
  //   GASNETI_THREAD_LOOKUP has been superceded by GASNET_BEGIN_FUNCTION
  //   GASNETI_THREAD_POST() has been superceded by GASNET_POST_THREADINFO()
  // GASNETI_THREAD_GET* now automatically pulls from these sources or FARG before lookup.

  // -----------------------------------------------------------------------------------------
  // *** Other ***
  
  // GASNETI_MYTHREAD_GET_OR_LOOKUP: force retrieve my (gasneti_threaddata_t *) from one of:
  //     a prior GASNET_POST_THREADINFO, an FARG to the enclosing function, or dynamic lookup
  //  This is essentially GASNETI_MYTHREAD without requiring FARG/POST'd context (allows lookup)
  //  Only valid known use is macros that expand threaddata field access directly into an
  //  "unknown" context, such as in client code or certain cases of internal code with callers
  //  in multiple conduits and/or subsystems.
  //  This is NOT suitable for internal code in which the macro definition and its callers fall
  //  within a single conduit or subsystem.  Such cases should instead establish FARG/POST'd
  //  context and use GASNETI_MYTHREAD.
  #define GASNETI_MYTHREAD_GET_OR_LOOKUP ((struct _gasneti_threaddata_t *)GASNET_GET_THREADINFO())

#else
  #define GASNETI_THREAD_FARG         
  #define GASNETI_THREAD_FARG_ALONE   void
  #define GASNETI_THREAD_GET         
  #define GASNETI_THREAD_GET_ALONE   
  #define GASNETI_THREAD_PASS         
  #define GASNETI_THREAD_PASS_ALONE   
  #define GASNETI_MYTHREAD            (_gasneti_mythread_slow())
  #define GASNETI_MYTHREAD_GET_OR_LOOKUP GASNETI_MYTHREAD
#endif

// Misc thread-id related goop
// GASNETI_THREAD_SWALLOW: Utility to discard an FARG passed to a 0-arg function-like macro
//   TODO-EX: move and rename this essentially unrelated utility macro
#define GASNETI_THREAD_SWALLOW(x)

/* ------------------------------------------------------------------------------------ */
/* GASNETI_MAX_THREADS: cannot exceed the size representable in gasnete_threadidx_t, 
   but some conduits or configures may set it to less */
#if GASNET_SEQ /* only one client thread by definition */
  #undef GASNETI_MAX_THREADS
  #ifdef GASNETE_CONDUIT_THREADS_USING_TD
    #define GASNETI_MAX_THREADS (1 + GASNETE_CONDUIT_THREADS_USING_TD)
  #else
    #define GASNETI_MAX_THREADS 1
  #endif
  #define GASNETI_MAX_THREADS_REASON "GASNET_SEQ mode only supports single-threaded operation."
#elif defined(GASNETI_MAX_THREADS) /* conduit-imposed limit */
  #if defined(GASNETI_MAX_THREADS_CONFIGURE) && GASNETI_MAX_THREADS_CONFIGURE < GASNETI_MAX_THREADS
    #undef  GASNETI_MAX_THREADS /* limit lowered by configure */
    #define GASNETI_MAX_THREADS GASNETI_MAX_THREADS_CONFIGURE
  #else
    #define GASNETI_MAX_THREADS_REASON "This limit is imposed by " GASNET_EXTENDED_NAME_STR " conduit."
  #endif
#else /* default */
  #if GASNETI_MAX_THREADS_CONFIGURE
    #define GASNETI_MAX_THREADS GASNETI_MAX_THREADS_CONFIGURE
  #else /* default */
    #define GASNETI_MAX_THREADS 256
  #endif
#endif
#ifndef GASNETI_MAX_THREADS_REASON
  #define GASNETI_MAX_THREADS_REASON "To raise this limit, configure GASNet using --with-max-pthreads-per-node=N."
#endif

#ifdef _GASNETE_THREADIDX_T
   /* conduit override */
  #ifndef SIZEOF_GASNETE_THREADIDX_T
    #error "Must define both _GASNETE_THREADIDX_T and SIZEOF_GASNETE_THREADIDX_T, or neither"
  #endif
  #ifndef GASNETE_INVALID_THREADIDX
    #error "Must define both _GASNETE_THREADIDX_T and GASNETE_INVALID_THREADIDX, or neither"
  #endif
#elif GASNETI_MAX_THREADS < 65536
  typedef uint16_t gasnete_threadidx_t;
  #define SIZEOF_GASNETE_THREADIDX_T 2
  #define GASNETE_INVALID_THREADIDX ((gasnete_threadidx_t)-1)
#elif GASNETI_MAX_THREADS < 4294967296
  typedef uint32_t gasnete_threadidx_t;
  #define SIZEOF_GASNETE_THREADIDX_T 4
  #define GASNETE_INVALID_THREADIDX ((gasnete_threadidx_t)-1)
#else
  typedef uint64_t gasnete_threadidx_t;
  #define SIZEOF_GASNETE_THREADIDX_T 8
  #define GASNETE_INVALID_THREADIDX ((gasnete_threadidx_t)-1)
#endif
/* returns the runtime size of the thread table (always <= GASNETI_MAX_THREADS) */
extern uint64_t gasneti_max_threads(void);
// same as above, except reduced by conduit-internal threads, if any
#if GASNET_SEQ
  #define gex_System_QueryMaxThreads() ((uint64_t)1)
#elif GASNETE_CONDUIT_THREADS_USING_TD
  GASNETI_INLINE(gex_System_QueryMaxThreads)
  uint64_t gex_System_QueryMaxThreads(void) {
    // This is conservative.
    // A conduit may spawn _up to_ GASNETE_CONDUIT_THREADS_USING_TD, but could spawn fewer.
    return gasneti_max_threads() - GASNETE_CONDUIT_THREADS_USING_TD;
  }
#else
  #define gex_System_QueryMaxThreads() gasneti_max_threads()
#endif
extern void gasneti_fatal_threadoverflow(const char *_subsystem);

#ifndef _GASNETI_MYTHREAD_SLOW
  struct _gasneti_threaddata_t;
  #if GASNETI_MAX_THREADS <= 256
    extern struct _gasneti_threaddata_t *gasnete_threadtable[GASNETI_MAX_THREADS];
  #else
    extern struct _gasneti_threaddata_t **gasnete_threadtable;
  #endif
  #if GASNETI_MAX_THREADS > 1
    #if GASNETI_COMPILER_IS_CC
      #if GASNET_STATS
        // this call breaks a dependency cycle with trace.h 
        GASNETI_INLINE(gasneti_record_dynamic_threadlookup)
        void gasneti_record_dynamic_threadlookup(void);
        #define GASNETI_RECORD_DYNAMIC_THREADLOOKUP gasneti_record_dynamic_threadlookup
      #endif
      GASNETI_THREADKEY_DECLARE(gasnete_threaddata);
      extern void * gasnete_new_threaddata(void);
      GASNETI_INLINE(_gasneti_mythread_slow) GASNETI_CONST
      struct _gasneti_threaddata_t *_gasneti_mythread_slow(void) {
        void *_threaddata = gasneti_threadkey_get(gasnete_threaddata);
        #ifdef GASNETI_RECORD_DYNAMIC_THREADLOOKUP
          GASNETI_RECORD_DYNAMIC_THREADLOOKUP(); 
        #endif
        if_pf (!_threaddata) { /* first time we've seen this thread - need to set it up */
          // NOTE: DON'T use _gasnete_mythread_slow_slow to initially populate TLS, because it's annotated const
          // so the optimizer won't understand it modifies the TLS "global" directly accessed above
          _threaddata = gasnete_new_threaddata();
        }
        gasneti_memcheck(_threaddata);
        return _threaddata;
      }
      GASNETI_CONSTP(_gasneti_mythread_slow)
    #else // !GASNETI_COMPILER_IS_CC
      // threadkey-get currently incurs a fncall on !CC anyhow, so nothing to save here
      extern struct _gasneti_threaddata_t *_gasnete_mythread_slow_slow(void) GASNETI_CONST;
      GASNETI_CONSTP(_gasnete_mythread_slow_slow)
      #define _gasneti_mythread_slow() _gasnete_mythread_slow_slow()
    #endif
  #else
    #define _gasneti_mythread_slow() (gasnete_threadtable[0])
  #endif
#endif

/* register a cleanup function to run when the calling thread exits 
   not guaranteed to run during process exits (gasnet_exit), but should
   run for dynamic thread exits when the process is continuing.
   Cleanups will run in reverse order of registration
 */
extern void gasnete_register_threadcleanup(void (*_cleanupfn)(void *), void *_context);

typedef struct _gasnete_thread_cleanup {
    struct _gasnete_thread_cleanup *_next;
    void (*_cleanupfn)(void *);
    void *_context;
} gasnete_thread_cleanup_t; /* thread exit cleanup function LIFO */


/* high-water mark on highest thread index allocated thus far */
extern int gasnete_maxthreadidx;
#define gasnete_assert_valid_threadid(threadidx) do {   \
    int _thid = (threadidx);                            \
    gasneti_assert_uint(_thid ,<=, gasnete_maxthreadidx); \
    gasneti_assert(gasnete_threadtable[_thid] != NULL); \
    gasneti_memcheck(gasnete_threadtable[_thid]);       \
} while (0)

// ------------------------------------------------------------------------------------
// Checks for communication calls in invalid contexts
//
// TODO: should be expanded to check handler and HSL contexts as well (not just NPAM)

#if GASNET_DEBUG
  extern void gasneti_check_inject(int _for_reply GASNETI_THREAD_FARG);
  #define GASNETI_CHECK_INJECT()        gasneti_check_inject(0 GASNETI_THREAD_GET)
  #define GASNETI_CHECK_INJECT_REPLY()  gasneti_check_inject(1 GASNETI_THREAD_GET)
  extern void gasneti_check_inject_reset(GASNETI_THREAD_FARG_ALONE);
  #define GASNETI_CHECK_INJECT_RESET()  gasneti_check_inject_reset(GASNETI_THREAD_GET_ALONE)
#else
  #define GASNETI_CHECK_INJECT()        ((void)0)
  #define GASNETI_CHECK_INJECT_REPLY()  ((void)0)
  #define GASNETI_CHECK_INJECT_RESET()  ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNet progressfn support
 * progressfns are internal functions that are called "periodically" by a conduit to 
 *  allow internal GASNet modules to make progress. 
 * Each progressfn is associated with a named subsystem (one-to-one mapping)
 *  subsystem names should be prefixed by gasneti_pf_ to prevent macro name capture
 * GASNETI_PROGRESSFNS_ENABLE/GASNETI_PROGRESSFNS_DISABLE are used by the conduit
 *  to provide a hint when a particular subsystem's progressfns want to be serviced
 * Each progressfn has either a BOOLEAN hint or COUNTED hint flavor
 *  COUNTED flavor: ENABLE/DISABLE manipulate an atomic reference count, initially zero 
 *    and hinting that calls are requested whenever the count > 0
 *  BOOLEAN flavor: ENABLE/DISABLE is a simple (non-atomic) flag, and the conduit
 *    is responsible for arbitrating any races between different threads when  
 *    setting/clearing that flag under GASNETI_THREADS
 * progressfns are called from a non-AM context, but they should never perform collective ops
 *  or execute any code that might block (aside from aquiring hsls in an AM-safe manner)
 * they may be called concurrently (if GASNETI_THREADS), and must be prepared to
 *  receive calls even when the hint indicates the given subsytem does not need service
 *  additionally, progressfns that make gasnet calls must be prepared to recieve 
 *  reentrant calls (ie without infinite recursion or deadlock)
 */
typedef void (*gasneti_progressfn_t)(void);

/* currently the list of progressfns is compile-time constant for dispatch performance 
 * reasons (a static dispatch is about 3x faster than a dynamic one on modern CPUs)
 * in the future it may be expanded with a dynamic function registration facility
 * PROGRESSFNS_LIST entries should look like:
   FN(token subsysname, flavor [COUNTED|BOOLEAN], gasneti_progressfn_t progressfn)
 */

/* conduit-specific core plug-in */
#ifndef GASNETC_PROGRESSFNS_LIST
#define GASNETC_PROGRESSFNS_LIST(FN)
#endif

#ifndef GASNETE_PROGRESSFNS_LIST
  extern gasneti_progressfn_t gasnete_barrier_pf;
  #define GASNETE_BARRIER_PROGRESSFN(FN) \
    FN(gasneti_pf_barrier, BOOLEAN, gasnete_barrier_pf) 

  /* conduit-specific extended plug-in */
  #ifndef GASNETE_PROGRESSFN_EXTRA
  #define GASNETE_PROGRESSFN_EXTRA(FN) 
  #endif

  #define GASNETE_PROGRESSFNS_LIST(FN) \
    GASNETE_PROGRESSFN_EXTRA(FN) \
    GASNETE_BARRIER_PROGRESSFN(FN)     
#endif

#if GASNET_DEBUG
  extern gasneti_progressfn_t gasneti_debug_progressfn_bool;
  extern gasneti_progressfn_t gasneti_debug_progressfn_counted;
  #define GASNETI_DEBUG_PROGRESSFNS(FN) \
      FN(gasneti_pf_debug_boolean, BOOLEAN, gasneti_debug_progressfn_bool) \
      FN(gasneti_pf_debug_counted, COUNTED, gasneti_debug_progressfn_counted) 
#else
  #define GASNETI_DEBUG_PROGRESSFNS(FN)
#endif

#define GASNETI_PROGRESSFNS_LIST(FN) \
  GASNETI_DEBUG_PROGRESSFNS(FN)      \
  GASNETI_COLL_PROGRESSFNS(FN)       \
  GASNETI_VIS_PROGRESSFNS(FN)        \
  GASNETE_PROGRESSFNS_LIST(FN)       \
  GASNETC_PROGRESSFNS_LIST(FN) 

/* default to one atomic counter per subsystem, because atomic_read
   is many times faster than a do-nothing function call 
*/
#ifndef GASNETI_PROGRESSFNS_ENABLE
  #define _GASNETI_PROGRESSFNS_DEFAULT
  #define _GASNETI_PROGRESSFNS_FLAG(subsysname,flavor) \
          _gasneti_progressfn_enabled_##subsysname##_##flavor

  #define _GASNETI_PROGRESSFNS_TYPE_BOOLEAN volatile int
  #define _GASNETI_PROGRESSFNS_TYPE_COUNTED gasneti_weakatomic_t
  #define _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_TYPE_##flavor

  #define _GASNETI_PROGRESSFNS_INIT_BOOLEAN = 0
  #define _GASNETI_PROGRESSFNS_INIT_COUNTED = gasneti_weakatomic_init(0)
  #define _GASNETI_PROGRESSFNS_INIT(flavor) _GASNETI_PROGRESSFNS_INIT_##flavor

  #define _GASNETI_PROGRESSFNS_DECLARE_FLAGS(subsysname, flavor, progressfn) \
    extern _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_FLAG(subsysname, flavor);
  #define _GASNETI_PROGRESSFNS_DEFINE_FLAGS(subsysname, flavor, progressfn)         \
    _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_FLAG(subsysname, flavor) \
                                      _GASNETI_PROGRESSFNS_INIT(flavor);
  GASNETI_PROGRESSFNS_LIST(_GASNETI_PROGRESSFNS_DECLARE_FLAGS) /* forward declaration */

  #define _GASNETI_PROGRESSFNS_ENABLE_BOOLEAN(subsysname) \
    (_GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN) = 1)
  #define _GASNETI_PROGRESSFNS_DISABLE_BOOLEAN(subsysname) \
    (_GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN) = 0)
  #define _GASNETI_PROGRESSFNS_ENABLE_COUNTED(subsysname) do {                                   \
    gasneti_weakatomic_increment(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0);                \
    gasneti_assert(gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0) > 0); \
  } while (0)
  #define _GASNETI_PROGRESSFNS_DISABLE_COUNTED(subsysname) do {                                  \
    gasneti_assert(gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0) > 0); \
    gasneti_weakatomic_decrement(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0);                \
  } while (0)
  #define GASNETI_PROGRESSFNS_ENABLE(subsysname,flavor) \
         _GASNETI_PROGRESSFNS_ENABLE_##flavor(subsysname)
  #define GASNETI_PROGRESSFNS_DISABLE(subsysname,flavor) \
         _GASNETI_PROGRESSFNS_DISABLE_##flavor(subsysname)

  #define _GASNETI_PROGRESSFNS_ISENABLED_BOOLEAN(subsysname) \
    _GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN)
  #define _GASNETI_PROGRESSFNS_ISENABLED_COUNTED(subsysname) \
    gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0)
  #define _GASNETI_PROGRESSFNS_RUN_IFENABLED(subsysname, flavor, progressfn) \
    (_GASNETI_PROGRESSFNS_ISENABLED_##flavor(subsysname) ? progressfn() : ((void)0)) ,
  #define GASNETI_PROGRESSFNS_RUN()                        \
   ( GASNETI_PROGRESSFNS_LIST(_GASNETI_PROGRESSFNS_RUN_IFENABLED) ((void)0) ) 
#endif

/* ------------------------------------------------------------------------------------ */
#ifdef GASNETI_GASNETI_AMPOLL
  GASNETI_GASNETI_AMPOLL
#else
  /*
   gasnet_AMPoll() - public poll function called by the client, throttled and traced 
                     should not be called from within GASNet (so we only trace directly user-initiated calls)
   gasneti_AMPoll() - called internally by GASNet, provides throttling (if enabled), progress functions, but no tracing
   gasnetc_AMPoll() - conduit AM dispatcher, should only be called from gasneti_AMPoll()
   */
  #ifdef GASNETI_GASNETC_AMPOLL
    GASNETI_GASNETC_AMPOLL
  #else
    extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE);
  #endif

  #if GASNETI_THROTTLE_FEATURE_ENABLED /* enabled by configure */ \
      && GASNETC_USING_SUSPEND_RESUME /* implemented by the conduit */ \
      && (GASNET_PAR || GASNETI_CONDUIT_THREADS) /* appropriate threading mode */
    #define GASNETI_THROTTLE_POLLERS 1
  #else
    #define GASNETI_THROTTLE_POLLERS 0
  #endif

  /* threads who need a network lock in order to send a message make 
     matched calls to gasneti_suspend/resume_spinpollers to help them 
     get the lock. They should not AMPoll while this suspend is in effect.
     The following debugging assertions detect violations of these rules.
  */ 
  #if GASNET_DEBUG
    GASNETI_THREADKEY_DECLARE(gasneti_throttledebug_key);

    #define gasneti_AMPoll_spinpollers_check()          \
      /* assert this thread hasn't already suspended */ \
      gasneti_assert((int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key) == 0)
    #define gasneti_suspend_spinpollers_check() do {                                        \
      int _mythrottlecnt = (int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key); \
      /* assert this thread hasn't already suspended */                                     \
      gasneti_assert_int(_mythrottlecnt ,==, 0);                                            \
      gasneti_threadkey_set(gasneti_throttledebug_key, (void *)(intptr_t)1);                \
    } while(0)
    #define gasneti_resume_spinpollers_check() do {                                         \
      int _mythrottlecnt = (int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key); \
      /* assert this thread previously suspended */                                         \
      gasneti_assert_int(_mythrottlecnt ,==, 1);                                            \
      gasneti_threadkey_set(gasneti_throttledebug_key, (void *)(intptr_t)0);                \
    } while(0)
  #else
    #define gasneti_AMPoll_spinpollers_check()  ((void)0)
    #define gasneti_suspend_spinpollers_check() ((void)0)
    #define gasneti_resume_spinpollers_check()  ((void)0)
  #endif

  #if !GASNETI_THROTTLE_POLLERS 
    GASNETI_INLINE(_gasneti_AMPoll)
    int _gasneti_AMPoll(GASNETI_THREAD_FARG_ALONE) {
       int _retval;
       gasneti_AMPoll_spinpollers_check();
       gasneti_memcheck_one();
       _retval = gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE);
       GASNETI_PROGRESSFNS_RUN();
       return _retval;
    }
    #define gasneti_suspend_spinpollers() gasneti_suspend_spinpollers_check()
    #define gasneti_resume_spinpollers()  gasneti_resume_spinpollers_check()
  #else
    /* AMPoll with throttling, to reduce lock contention in the network:
       poll if and only if no other thread appears to already be spin-polling,
       and no thread is attempting to use the network for sending 
       Design goals (in rough order of importance):
        - when one or more threads need to send, all spin-pollers should get 
          out of the way (but continue checking their completion condition)
        - only one thread should be spin-polling at a time, to prevent 
          lock contention and cache thrashing between spin-pollers
        - manage AMPoll calls internal to GASNet (including those from polluntil) 
          and explicit client AMPoll calls
       Lowest priority design goals, which have not been achieved:
        - allow concurrent handler execution - if the spin poller recvs an AM, 
          it should release another spin poller before invoking the handler
          [lock reacquisition becomes a bottleneck]
        - the single spin-poller should not need to pay locking overheads in the loop
          [a limitation of the infrastructure: no way to differentiate "single"]
    */
    extern gasneti_atomic_t gasneti_throttle_haveusefulwork;
    extern gasneti_mutex_t gasneti_throttle_spinpoller;

    #define gasneti_suspend_spinpollers() do {                      \
        gasneti_suspend_spinpollers_check();                        \
        gasneti_atomic_increment(&gasneti_throttle_haveusefulwork,0); \
    } while (0)
    #define gasneti_resume_spinpollers() do {                       \
        gasneti_resume_spinpollers_check();                         \
        gasneti_atomic_decrement(&gasneti_throttle_haveusefulwork,0); \
    } while (0)

    /* and finally, the throttled poll implementation */
    GASNETI_INLINE(_gasneti_AMPoll)
    int _gasneti_AMPoll(GASNETI_THREAD_FARG_ALONE) {
       int _retval = GASNET_OK;
       gasneti_AMPoll_spinpollers_check();
       gasneti_memcheck_one();
       /* if another thread is spin-polling then skip both the poll and progress fns: */
       if_pt (!gasneti_mutex_trylock(&gasneti_throttle_spinpoller)) {
          /* if another thread is sending then skip the poll: */
          if_pt (!gasneti_atomic_read(&gasneti_throttle_haveusefulwork,0))
             _retval = gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE);
          gasneti_mutex_unlock(&gasneti_throttle_spinpoller);
          GASNETI_PROGRESSFNS_RUN();
       }
       return _retval;
    }
  #endif
  #define gasneti_AMPoll() _gasneti_AMPoll(GASNETI_THREAD_GET_ALONE)
#endif

// Should poll when GEX_FLAG_IMMEDIATE bit set? (undefined or 1)
#if defined(GASNETC_IMMEDIATE_AMPOLLS) && (GASNETC_IMMEDIATE_AMPOLLS != 1)
  #error GASNETC_IMMEDIATE_AMPOLLS must be 1 or undefined
#endif

// Convenience test
#if GASNETC_IMMEDIATE_AMPOLLS
  #define GASNETC_IMMEDIATE_WOULD_POLL(flag) 1
#else
  #define GASNETC_IMMEDIATE_WOULD_POLL(flag) (!((flag)&GEX_FLAG_IMMEDIATE))
#endif

// Convenience conditional poll
#define GASNETC_IMMEDIATE_MAYBE_POLL(flag) \
    do { if (GASNETC_IMMEDIATE_WOULD_POLL(flag)) gasneti_AMPoll(); } while (0)

/* Blocking functions
 * Note the _rmb at the end loop of each is required to ensure that subsequent
 * reads will not observe values that were prefeteched or are otherwise out
 * of date.
 */
extern int gasneti_wait_mode; /* current waitmode hint */
#define GASNETI_WAITHOOK() do {                                       \
    if (gasneti_wait_mode != GASNET_WAIT_SPIN) gasneti_sched_yield(); \
    /* prevent optimizer from hoisting the condition check out of */  \
    /* the enclosing spin loop - this is our way of telling the */    \
    /* optimizer "the whole world could change here" */               \
    gasneti_compiler_fence();                                         \
    gasneti_spinloop_hint();                                          \
  } while (0)

/* busy-waits, with no implicit polling (cnd should include an embedded poll)
   differs from GASNET_BLOCKUNTIL because it may be waiting for an event
     caused by the receipt of a non-AM message
 */
#ifndef gasneti_waitwhile
  #define gasneti_waitwhile(cnd) do { \
    while (cnd) GASNETI_WAITHOOK();   \
    gasneti_local_rmb();              \
  } while (0)
#endif
#define gasneti_waituntil(cnd) gasneti_waitwhile(!(cnd)) 

/* busy-wait, with implicit polling */
/* Note no poll if the condition is already satisfied */
#ifndef gasneti_pollwhile
  #define gasneti_pollwhile(cnd) do { \
    if (cnd) {                        \
      gasneti_AMPoll();               \
      while (cnd) {                   \
        GASNETI_WAITHOOK();           \
        gasneti_AMPoll();             \
      }                               \
    }                                 \
    gasneti_local_rmb();              \
  } while (0)
#endif
#define gasneti_polluntil(cnd) gasneti_pollwhile(!(cnd)) 

/* ------------------------------------------------------------------------------------ */

/* high-performance timer library */
#include <gasnet_timer.h>

/* tracing utilities */
#include <gasnet_trace.h>

/* ------------------------------------------------------------------------------------ */
/* default implementations of various conduit functions (may be overridden in some conduits) */

#ifndef _GASNET_AMPOLL
#define _GASNET_AMPOLL
  /* GASNet client calls gasnet_AMPoll(), which throttles and traces */
  GASNETI_INLINE(_gasnet_AMPoll)
  int _gasnet_AMPoll(GASNETI_THREAD_FARG_ALONE) {
    GASNETI_TRACE_EVENT(X, AMPOLL);
    GASNETI_CHECK_INJECT();
    return _gasneti_AMPoll(GASNETI_THREAD_PASS_ALONE);
  }
  #define gasnet_AMPoll() _gasnet_AMPoll(GASNETI_THREAD_GET_ALONE)
#endif

#ifndef _GASNET_GETENV
#define _GASNET_GETENV
  GASNETI_INLINE(gasnet_getenv)
  char *gasnet_getenv(const char *_s) {
    GASNETI_CHECKINIT();
    return gasneti_getenv(_s);
  }
#endif

#ifndef _GASNET_WAITMODE
#define _GASNET_WAITMODE
  #define GASNET_WAIT_SPIN      0 /* contend aggressively for CPU resources while waiting (spin) */
  #define GASNET_WAIT_BLOCK     1 /* yield CPU resources immediately while waiting (block) */
  #define GASNET_WAIT_SPINBLOCK 2 /* spin for an implementation-dependent period, then block */
  extern int gasneti_set_waitmode(int _wait_mode);
  #define gasnet_set_waitmode(wait_mode) gasneti_set_waitmode(wait_mode)
#endif

#ifndef _GASNET_GETMAXSEGMENTSIZE
#define _GASNET_GETMAXSEGMENTSIZE
#define _GASNET_GETMAXSEGMENTSIZE_DEFAULT
    extern uintptr_t gasneti_MaxLocalSegmentSize;
    extern uintptr_t gasneti_MaxGlobalSegmentSize;
  #if GASNET_SEGMENT_EVERYTHING
    #define gasnet_getMaxLocalSegmentSize()   ((uintptr_t)-1)
    #define gasnet_getMaxGlobalSegmentSize()  ((uintptr_t)-1)
  #else
    #define gasnet_getMaxLocalSegmentSize() \
            (GASNETI_CHECKINIT(), (uintptr_t)gasneti_MaxLocalSegmentSize)
    #define gasnet_getMaxGlobalSegmentSize() \
            (GASNETI_CHECKINIT(), (uintptr_t)gasneti_MaxGlobalSegmentSize)
  #endif
#endif

#ifndef _GASNET_GETSEGMENTINFO
#define _GASNET_GETSEGMENTINFO
  extern int gasneti_getSegmentInfo(gasnet_seginfo_t *_seginfo_table, int _numentries);
  #define gasnet_getSegmentInfo(seginfo_table, numentries) \
          gasneti_getSegmentInfo(seginfo_table, numentries)
#endif

#ifndef _GASNET_GETNODEINFO
#define _GASNET_GETNODEINFO
  extern int gasneti_getNodeInfo(gasnet_nodeinfo_t *_nodeinfo_table, int _numentries);
  #define gasnet_getNodeInfo(nodeinfo_table, numentries) \
          gasneti_getNodeInfo(nodeinfo_table, numentries)
#endif
extern gasnet_nodeinfo_t *gasneti_nodeinfo;

#ifdef GASNETI_RECORD_DYNAMIC_THREADLOOKUP
  GASNETI_INLINE(gasneti_record_dynamic_threadlookup)
  void gasneti_record_dynamic_threadlookup(void) {
    GASNETI_STAT_EVENT(C, DYNAMIC_THREADLOOKUP);
  }
#endif

/* ------------------------------------------------------------------------------------ */
/* Bits for conduits which want/need to override pthread_create() */
#if defined(PTHREAD_MUTEX_INITIALIZER) /* only if pthread.h available */ && !GASNET_SEQ
  /* gasneti_pthread_create() available on all non-SEQ builds w/ pthreads */
  typedef int (gasneti_pthread_create_fn_t)(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
  extern int gasneti_pthread_create(gasneti_pthread_create_fn_t *_create_fn, pthread_t *_thread, const pthread_attr_t *_attr, void *(*_start_routine)(void *), void *_arg);

  #if defined(GASNETC_PTHREAD_CREATE_OVERRIDE)
    /* Capture existing defn, which could be another library's override */
    static int gasneti_pthread_create_system(pthread_t *_thread, const pthread_attr_t *_attr, void *(*_start_routine)(void *), void *_arg) {
      return pthread_create(_thread,_attr,_start_routine,_arg);
    }
    /* Install our override */
    #undef pthread_create
    #define pthread_create(thr, attr, fn, arg) \
        gasneti_pthread_create(&gasneti_pthread_create_system, (thr), (attr), (fn), (arg))
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
// Memory Kinds

// The following GASNET_HAVE_MK_CLASS_* identifiers are either `1` or unset

#define GASNET_HAVE_MK_CLASS_HOST 1 // For consistency - always available

#if GASNET_HAVE_MK_CLASS_CUDA_UVA
  #undef GASNET_HAVE_MK_CLASS_CUDA_UVA
  #define GASNET_HAVE_MK_CLASS_CUDA_UVA 1
  #define GASNETI_MK_CLASS_CUDA_UVA_CONFIG mk_class_cuda_uva
#else
  #undef GASNET_HAVE_MK_CLASS_CUDA_UVA
  #define GASNETI_MK_CLASS_CUDA_UVA_CONFIG nomk_class_cuda_uva
#endif

#if GASNET_HAVE_MK_CLASS_CUDA_UVA // || GASNET_HAVE_MK_CLASS_[FOO]
  #define GASNET_HAVE_MK_CLASS_MULTIPLE 1
#endif

#if GASNET_HAVE_MK_CLASS_MULTIPLE
  GASNETI_INLINE(gasneti_i_segment_kind_is_host)
  int gasneti_i_segment_kind_is_host(gasneti_Segment_t _segment) {
    // Either NULL (such as for no bound segment, which is just fine for
    // out-of-segment or in-aux-seg local addrs) OR the kind is GEX_MK_HOST.
    return !_segment || (_segment->_kind == GEX_MK_HOST);
  }
  #define gasneti_e_segment_kind_is_host(segment) \
          gasneti_i_segment_kind_is_host(gasneti_import_segment(segment))
#else
  #define gasneti_i_segment_kind_is_host(segment) 1
  #define gasneti_e_segment_kind_is_host(segment) 1
#endif

/* ------------------------------------------------------------------------------------ */
/* PSHM support */
#if GASNET_PSHM

/* Max number of processes supported per node */
#ifndef GASNETI_PSHM_MAX_NODES
  #ifdef GASNETI_CONFIG_PSHM_MAX_NODES
    #define GASNETI_PSHM_MAX_NODES GASNETI_CONFIG_PSHM_MAX_NODES
  #else
    #define GASNETI_PSHM_MAX_NODES 255
  #endif
#endif

#if GASNETI_PSHM_MAX_NODES < 256
  typedef uint8_t gasneti_pshm_rank_t;
#elif GASNETI_PSHM_MAX_NODES < 65536
  typedef uint16_t gasneti_pshm_rank_t;
#else
  #error "GASNETI_PSHM_MAX_NODES too large"
#endif

extern gasneti_pshm_rank_t gasneti_pshm_nodes;  /* # nodes in my supernode */
extern gasneti_pshm_rank_t gasneti_pshm_mynode; /* my 0-based rank in supernode */
extern gex_Rank_t gasneti_pshm_firstnode;    /* lowest node # in supernode */

/* vector of first node within each supernode */
extern gex_Rank_t *gasneti_pshm_firsts;

/* Non-NULL only when supernode members are non-contiguous */
extern gasneti_pshm_rank_t *gasneti_pshm_rankmap;

/* Returns "local rank" if given node is in the callers supernode.
 * Otherwise returns an "impossible" value >= gasneti_pshm_nodes.
 */
GASNETI_INLINE(gasneti_pshm_jobrank_to_local_rank) GASNETI_PURE
unsigned int gasneti_pshm_jobrank_to_local_rank(gex_Rank_t _jobrank) {
#if GASNET_CONDUIT_SMP
  return _jobrank;
#else
  if_pt (gasneti_pshm_rankmap == NULL) {
    /* NOTE: gex_Rank_t is an unsigned type, so in the case of
     * (_jobrank < gasneti_pshm_firstnode), the subtraction will wrap to
     * a "large" value.
     */
    return (_jobrank - gasneti_pshm_firstnode);
  } else {
    return gasneti_pshm_rankmap[_jobrank];
  }
#endif
}
GASNETI_PUREP(gasneti_pshm_jobrank_to_local_rank)

/* Returns 1 if given node is in the caller's supernode, or 0 if it's not.
 * NOTE: result is false before vnet initialization.
 */
GASNETI_INLINE(gasneti_pshm_jobrank_in_supernode) GASNETI_PURE
int gasneti_pshm_jobrank_in_supernode(gex_Rank_t _jobrank) {
#if GASNET_CONDUIT_SMP
  return 1;
#else
  return (gasneti_pshm_jobrank_to_local_rank(_jobrank) < gasneti_pshm_nodes);
#endif
}
GASNETI_PUREP(gasneti_pshm_jobrank_in_supernode)

/* Returns local version of remote in-supernode address.
 */
// TODO-EX: This is probably the wrong interface for at least 2 reasons:
// + Relies on dense array of nodeinfo even though only supernode-local are non-zero
// + Was designed for single segment and even auxseg is currently a hack
GASNETI_INLINE(gasneti_pshm_jobrank_addr2local) GASNETI_PURE
void *gasneti_pshm_jobrank_addr2local(gex_Rank_t _jobrank, const void *_addr) {
#if 1 // TODO-EX: this is a hack!
  // Properties of unsigned subtraction make the following oblivous to order of client vs aux segment
  if_pf (((uintptr_t)_addr - (uintptr_t)gasneti_seginfo[_jobrank].addr) >= gasneti_seginfo[_jobrank].size)
    return (void*)((uintptr_t)_addr + (uintptr_t)gasneti_nodeinfo[_jobrank].auxoffset);
#endif
  return  (void*)((uintptr_t)_addr
                   + (uintptr_t)gasneti_nodeinfo[_jobrank].offset);
} 
GASNETI_PUREP(gasneti_pshm_jobrank_addr2local)

// Helper for what follows
// Returns a jobrank or GEX_RANK_INVALID depending on whether the local and
// remote endpoints named by (tm,rank) are both "eligible" for PSHM, exclusive
// of the check on the jobrank being in-nbrhd.  The eligibility criteria are:
//   1. Remote endpoint must be primordial (have EP index 0)
//   2. Local endpoint must be host memory
// However, checking these efficiently is not as simple as it sounds.
extern gasneti_Segment_t gasneti_tm_pair_to_segment(gasneti_TM_Pair_t _tm_pair);
GASNETI_INLINE(gasneti_pshm_jobrank_if_eligible) GASNETI_PURE
gex_Rank_t gasneti_pshm_jobrank_if_eligible(gex_TM_t _e_tm, gex_Rank_t _rank) {
  gasneti_TM_t _i_tm = gasneti_import_tm(_e_tm);
  if (gasneti_is_tm0(_i_tm)) {
    // fast path for TM0
    return _rank;
  }
  gex_EP_Location_t _loc = gasneti_i_tm_rank_to_location(_i_tm, _rank, 0);
  if (_loc.gex_ep_index) {
    // not eligible due to non-primordial remote EP
    return GEX_RANK_INVALID;
  }
  // If we've made it this far, the (tm,rank) is eligible only and only if
  // the local ep is host memory (which can take some work to determine).
  gex_Rank_t _jobrank = _loc.gex_rank;
#if !GASNET_HAVE_MK_CLASS_MULTIPLE
  return _jobrank; // Trivial host memory when no device kinds are supported
#else
  gasneti_Segment_t _segment;
  if (! gasneti_i_tm_is_pair(_i_tm)) {
    // Full TM object - can check objects directly
    gasneti_EP_t _ep = _i_tm->_ep;
    if (_ep->_index == 0) return _jobrank; // EP index 0 is primordial
    _segment = _i_tm->_ep->_segment;
  } else {
    gasneti_TM_Pair_t _tm_pair = gasneti_i_tm_to_pair(_i_tm);
    gex_EP_Index_t _idx = gasneti_tm_pair_loc_idx(_tm_pair);
    if (_idx == 0) return _jobrank; // EP index 0 is primordial
    _segment = gasneti_tm_pair_to_segment(_tm_pair);
  }
  return gasneti_i_segment_kind_is_host(_segment) ? _jobrank : GEX_RANK_INVALID;
#endif
}

// Same as the three functions above, but taking (tm,rank) in place of jobrank
// All are TM-pair aware, and the first two are multi-EP aware

GASNETI_INLINE(gasneti_pshm_local_rank) GASNETI_PURE
unsigned int gasneti_pshm_local_rank(gex_TM_t _e_tm, gex_Rank_t _rank) {
  gex_Rank_t _jobrank = gasneti_pshm_jobrank_if_eligible(_e_tm, _rank);
  return (_jobrank == GEX_RANK_INVALID)
         ? (unsigned int)(-1)
         : gasneti_pshm_jobrank_to_local_rank(_jobrank);
}
GASNETI_PUREP(gasneti_pshm_local_rank)

GASNETI_INLINE(gasneti_pshm_in_supernode) GASNETI_PURE
int gasneti_pshm_in_supernode(gex_TM_t _e_tm, gex_Rank_t _rank) {
  gex_Rank_t _jobrank = gasneti_pshm_jobrank_if_eligible(_e_tm, _rank);
  return (_jobrank != GEX_RANK_INVALID) && gasneti_pshm_jobrank_in_supernode(_jobrank);
}
GASNETI_PUREP(gasneti_pshm_in_supernode)

GASNETI_INLINE(gasneti_pshm_addr2local) GASNETI_PURE
void *gasneti_pshm_addr2local(gex_TM_t _e_tm, gex_Rank_t _rank, const void *_addr) {
  gex_Rank_t _jobrank = gasneti_e_tm_rank_to_jobrank(_e_tm,_rank);
  return gasneti_pshm_jobrank_addr2local(_jobrank, _addr);
} 
GASNETI_PUREP(gasneti_pshm_addr2local)
#endif /* GASNET_PSHM */

/* ------------------------------------------------------------------------------------ */
// Wrappers for memcpy()

/* TODO-EX: these should replace the alignment-aware versions */
// + GASNETI_MEMCPY
//     Arguments must match POSIX constraints:
//       Zero value of nbytes is permitted
//       Both pointers must be valid (even for !nbytes)
//       If nbytes non-zero src and dst ranges must not overlap
//     This is the least costly option and should be used whenever the
//     call site can be guaranteed to meet these requirements.
// + GASNETI_MEMCPY_SAFE_EMPTY
//     Omits memcpy() (thus ignoring pointers entirely) IFF !nbytes
// + GASNETI_MEMCPY_SAFE_IDENTICAL
//     Omits memcpy() IFF (src == dst)
// + GASNETI_MEMCPY_SAFE
//     Omits memcpy() IFF (!nbytes || src == dst)
//     This is the most costly option.
//     Use any of the versions above when possible.
#define GASNETI_MEMCPY(dst,src,nbytes) do {                \
    static uint8_t _fm_dummy;                              \
    void       *_fm_d = (dst);                             \
    void const *_fm_s = (src);                             \
    size_t      _fm_n = (nbytes);                          \
    gasneti_assume(_fm_s && _fm_d);                        \
    gasneti_assert((_fm_dummy += *(volatile uint8_t*)_fm_s, 1)); \
    gasneti_assert((_fm_dummy += *(volatile uint8_t*)_fm_d, 1)); \
    gasneti_assume(!_fm_n || ((uintptr_t)_fm_s >= (uintptr_t)_fm_d+_fm_n)   \
                          || ((uintptr_t)_fm_d >= (uintptr_t)_fm_s+_fm_n)); \
    (void) memcpy(_fm_d, _fm_s, _fm_n);                    \
  } while (0)
#define GASNETI_MEMCPY_SAFE_IDENTICAL(dst,src,nbytes) do { \
    void       *_fmc_d = (dst);                            \
    void const *_fmc_s = (src);                            \
    if_pt (_fmc_d != _fmc_s)                               \
        GASNETI_MEMCPY(_fmc_d, _fmc_s, (nbytes));          \
  } while (0)
#define GASNETI_MEMCPY_SAFE_EMPTY(dst,src,nbytes) do {     \
    size_t _fmse_n = (nbytes);                             \
    if_pt (_fmse_n)                                        \
        GASNETI_MEMCPY((dst), (src), _fmse_n);             \
  } while (0)
#define GASNETI_MEMCPY_SAFE(dst,src,nbytes) do {           \
    void       *_fms_d = (dst);                            \
    void const *_fms_s = (src);                            \
    size_t      _fms_n = (nbytes);                         \
    if_pt (_fms_n && _fms_d != _fms_s)                     \
        GASNETI_MEMCPY(_fms_d, _fms_s, _fms_n);            \
  } while (0)

/* ------------------------------------------------------------------------------------ */
/*
  Variable-Argument Active Message Request/Reply Functions
  ========================================================
*/

/* The following relies on __VA_ARGS__ support that was introduced in C99 and C++11.
 * We do not require client code to use either for compiling GASNet headers,
 * and this convenience interface is disabled when the necessary support is
 * lacking. Auto-detection can be overridden via -DGASNETI_FORCE_VA_ARG=0/1
 * Consequently, GASNet public headers should NOT assume this API exists.
 */
#ifdef GASNETI_FORCE_VA_ARG /* manual override */
  #define GASNETI_USING_VA_ARG GASNETI_FORCE_VA_ARG
#else
  #define GASNETI_USING_VA_ARG GASNETI_COMPILER_IS_CC || \
          ( GASNETI_COMPILER_IS_CXX && GASNETI_CXX_HAS_VA_ARGS ) || \
          __STDC_VERSION__ >= 199901L || \
          __cplusplus >= 201103L
#endif
#if GASNETI_USING_VA_ARG
/*
   These macros implement six AM Request/Reply without any explicit
   argument count.  It uses C99 variable argument pre-processor macros to
   count the number of arguments after 'flags' and construct a coresponding
   gex_AM_{Request,Reply}{Short,Medium,Long}{0..MaxArgs}() call.  This
   does *not* depend on the varargs-based implementation which is contained
   in gasnet_ammacros.h.

   Since there is no portable way to omit a comma appearing before the
   expansion of an empty __VA_ARGS__, we include the 'flags' argument
   within the "...".  So, the argument-counting macros may appear "off by
   one" relative to the canonical example(s).
 */

#define GASNETI_AMNUMARGS(...) GASNETI_AMNUMARGS_(__VA_ARGS__,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0)
#define GASNETI_AMNUMARGS_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,N,...) N
#define GASNETI_AMVA(_stem,...) _CONCAT(gex_AM_##_stem,GASNETI_AMNUMARGS(__VA_ARGS__))

#define                gex_AM_RequestShort(tm,rank,hidx,...) \
        GASNETI_AMVA(RequestShort,__VA_ARGS__)(tm,rank,hidx,__VA_ARGS__)

#define                gex_AM_RequestMedium(tm,rank,hidx,src_addr,nbytes,lc_opt,...) \
        GASNETI_AMVA(RequestMedium,__VA_ARGS__)(tm,rank,hidx,src_addr,nbytes,lc_opt,__VA_ARGS__)

#define                gex_AM_RequestLong(tm,rank,hidx,src_addr,nbytes,dst_addr,lc_opt,...) \
        GASNETI_AMVA(RequestLong,__VA_ARGS__)(tm,rank,hidx,src_addr,nbytes,dst_addr,lc_opt,__VA_ARGS__)

#define                gex_AM_ReplyShort(token,hidx,...) \
        GASNETI_AMVA(ReplyShort,__VA_ARGS__)(token,hidx,__VA_ARGS__)

#define                gex_AM_ReplyMedium(token,hidx,src_addr,nbytes,lc_opt,...) \
        GASNETI_AMVA(ReplyMedium,__VA_ARGS__)(token,hidx,src_addr,nbytes,lc_opt,__VA_ARGS__)

#define                gex_AM_ReplyLong(token,hidx,src_addr,nbytes,dst_addr,lc_opt,...) \
        GASNETI_AMVA(ReplyLong,__VA_ARGS__)(token,hidx,src_addr,nbytes,dst_addr,lc_opt,__VA_ARGS__)

/* Similarly provide argument-counting convenience wrappers for gex_AM_Commit{Request,Reply}{Medium,Long}, 
 * with the same pre-requisites as above (compiler must support C99 __VA_ARGS__).
 *
 * Similarly to above, the implementation includes the last non-variadic argument (nbytes or dest_addr)
 * within the __VA_ARGS__, for the same reasons.
 */
#define gex_AM_CommitRequestMedium(sd,hidx,...) \
        GASNETI_AMVA(CommitRequestMedium,__VA_ARGS__)(sd,hidx,__VA_ARGS__)
#define gex_AM_CommitReplyMedium(sd,hidx,...) \
        GASNETI_AMVA(CommitReplyMedium,__VA_ARGS__)(sd,hidx,__VA_ARGS__)
#define gex_AM_CommitRequestLong(sd,hidx,nbytes,...) \
        GASNETI_AMVA(CommitRequestLong,__VA_ARGS__)(sd,hidx,nbytes,__VA_ARGS__)
#define gex_AM_CommitReplyLong(sd,hidx,nbytes,...) \
        GASNETI_AMVA(CommitReplyLong,__VA_ARGS__)(sd,hidx,nbytes,__VA_ARGS__)

#endif
/* ------------------------------------------------------------------------------------ */
// Internal helpers for data types (like ctypes)

// Masks (disjoint pairs):

#define _GEX_DT_4BYTE  (GEX_DT_I32 | GEX_DT_U32 | GEX_DT_FLT)
#define _GEX_DT_8BYTE  (GEX_DT_I64 | GEX_DT_U64 | GEX_DT_DBL)

#define _GEX_DT_INT (GEX_DT_I32 | GEX_DT_U32 | GEX_DT_I64 | GEX_DT_U64)
#define _GEX_DT_FP  (GEX_DT_FLT | GEX_DT_DBL)

#define _GEX_DT_SIGNED    (GEX_DT_I32 | GEX_DT_I64)
#define _GEX_DT_UNSIGNED  (GEX_DT_U32 | GEX_DT_U64)

// Union of all:

#define _GEX_DT_VALID  (_GEX_DT_INT | _GEX_DT_FP | GEX_DT_USER)

// Predicates:

// Is the argument a *single* valid data type?
GASNETI_INLINE(gasneti_dt_valid) GASNETI_PURE
int gasneti_dt_valid(gex_DT_t _dt) {
  return (((_dt) & _GEX_DT_VALID) && GASNETI_POWEROFTWO(_dt));
}
GASNETI_INLINE(gasneti_dt_valid_atomic) GASNETI_PURE
int gasneti_dt_valid_atomic(gex_DT_t _dt) {
  return gasneti_dt_valid(_dt) && (_dt != GEX_DT_USER);
}
#define gasneti_dt_valid_reduce gasneti_dt_valid

// Is the argument a (possibly empty) mask consisting of only valid data types?
#define gasneti_dt_valid_mask(dts) (!((dts) & ~_GEX_DT_VALID))

#define gasneti_dt_int(dt)    ((dt) & _GEX_DT_INT)
#define gasneti_dt_fp(dt)     ((dt) & _GEX_DT_FP)

#define gasneti_dt_signed(dt)   ((dt) & _GEX_DT_SIGNED)
#define gasneti_dt_unsigned(dt) ((dt) & _GEX_DT_UNSIGNED)

#define gasneti_dt_4byte(dt) ((dt) & _GEX_DT_4BYTE)
#define gasneti_dt_8byte(dt) ((dt) & _GEX_DT_8BYTE)

// Query:

// What is the size of the type?
// TODO: might be made cheaper by encoding size into the GEX_DT_* constants
GASNETI_INLINE(gasneti_dt_size) GASNETI_PURE
size_t gasneti_dt_size(gex_DT_t _dt) {
  gasneti_assert(!gasneti_dt_4byte(_dt) ^ !gasneti_dt_8byte(_dt));
  return (size_t) (gasneti_dt_4byte(_dt) ? 4 : 8);
}

/* ------------------------------------------------------------------------------------ */
// Internal helpers for gex_OP_t (like ctypes)

// Masks (disjoint):
#define _GEX_OP_ARITH_BINARY \
        (GEX_OP_ADD|GEX_OP_MULT|GEX_OP_MIN|GEX_OP_MAX)
#define _GEX_OP_NC_ARITH_BINARY \
         GEX_OP_SUB
#define _GEX_OP_ARITH_UNARY \
        (GEX_OP_INC|GEX_OP_DEC)
#define _GEX_OP_BITWISE \
        (GEX_OP_AND|GEX_OP_OR|GEX_OP_XOR)
#define _GEX_OP_FETCH_ARITH_BINARY \
        (GEX_OP_FADD|GEX_OP_FMULT|GEX_OP_FMIN|GEX_OP_FMAX)
#define _GEX_OP_FETCH_NC_ARITH_BINARY \
         GEX_OP_FSUB
#define _GEX_OP_FETCH_ARITH_UNARY \
        (GEX_OP_FINC|GEX_OP_FDEC)
#define _GEX_OP_FETCH_BITWISE \
        (GEX_OP_FAND|GEX_OP_FOR|GEX_OP_FXOR)
#define _GEX_OP_ACCESSOR \
        (GEX_OP_SET|GEX_OP_CAS)
#define _GEX_OP_FETCH_ACCESSOR \
        (GEX_OP_GET|GEX_OP_SWAP|GEX_OP_FCAS)
#define _GEX_OP_USER_REDUCE \
        (GEX_OP_USER|GEX_OP_USER_NC)

// Masks for various properties:

#define _GEX_OP_REDUCE \
        (_GEX_OP_ARITH_BINARY|_GEX_OP_BITWISE|_GEX_OP_USER_REDUCE)
#define _GEX_OP_ATOMIC \
        (_GEX_OP_ARITH_BINARY | _GEX_OP_FETCH_ARITH_BINARY | \
         _GEX_OP_NC_ARITH_BINARY | _GEX_OP_FETCH_NC_ARITH_BINARY | \
         _GEX_OP_ARITH_UNARY  | _GEX_OP_FETCH_ARITH_UNARY  | \
         _GEX_OP_BITWISE      | _GEX_OP_FETCH_BITWISE      | \
         _GEX_OP_ACCESSOR     | _GEX_OP_FETCH_ACCESSOR)
#define _GEX_OP_VALID \
         (_GEX_OP_REDUCE | _GEX_OP_ATOMIC)

#define _GEX_OP_INT \
        _GEX_OP_VALID
#define _GEX_OP_FP  \
        (_GEX_OP_ARITH_BINARY | _GEX_OP_FETCH_ARITH_BINARY | \
         _GEX_OP_NC_ARITH_BINARY | _GEX_OP_FETCH_NC_ARITH_BINARY | \
         _GEX_OP_ARITH_UNARY  | _GEX_OP_FETCH_ARITH_UNARY  | \
         _GEX_OP_ACCESSOR     | _GEX_OP_FETCH_ACCESSOR | \
         _GEX_OP_USER_REDUCE)

#define _GEX_OP_FETCH \
        (_GEX_OP_FETCH_ARITH_BINARY | \
         _GEX_OP_FETCH_NC_ARITH_BINARY | \
         _GEX_OP_FETCH_ARITH_UNARY  | \
         _GEX_OP_FETCH_BITWISE      | \
         _GEX_OP_FETCH_ACCESSOR)

#define _GEX_OP_0ARG \
        (GEX_OP_GET | \
         _GEX_OP_ARITH_UNARY | _GEX_OP_FETCH_ARITH_UNARY)
#define _GEX_OP_1ARG \
        (GEX_OP_SET | GEX_OP_SWAP |\
         _GEX_OP_ARITH_BINARY | _GEX_OP_FETCH_ARITH_BINARY | \
         _GEX_OP_NC_ARITH_BINARY | _GEX_OP_FETCH_NC_ARITH_BINARY | \
         _GEX_OP_BITWISE      | _GEX_OP_FETCH_BITWISE)
#define _GEX_OP_2ARG \
        (GEX_OP_FCAS | GEX_OP_CAS)


// Predicates for use with single-bit arguments:
#define gasneti_op_reduce(op) ((op) & _GEX_OP_REDUCE)
#define gasneti_op_atomic(op) ((op) & _GEX_OP_ATOMIC)
#define gasneti_op_int(op)    ((op) & _GEX_OP_INT)
#define gasneti_op_fp(op)     ((op) & _GEX_OP_FP)
#define gasneti_op_fetch(op)  ((op) & _GEX_OP_FETCH)
#define gasneti_op_0arg(op)   ((op) & _GEX_OP_0ARG)
#define gasneti_op_1arg(op)   ((op) & _GEX_OP_1ARG)
#define gasneti_op_2arg(op)   ((op) & _GEX_OP_2ARG)

// Is the argument a *single* valid operation?
GASNETI_INLINE(gasneti_op_valid) GASNETI_PURE
int gasneti_op_valid(gex_OP_t _op) {
  return (((_op) & _GEX_OP_VALID) && GASNETI_POWEROFTWO(_op));
}
GASNETI_INLINE(gasneti_op_valid_atomic) GASNETI_PURE
int gasneti_op_valid_atomic(gex_OP_t _op) {
  return gasneti_op_valid(_op) && gasneti_op_atomic(_op);
}
GASNETI_INLINE(gasneti_op_valid_reduce) GASNETI_PURE
int gasneti_op_valid_reduce(gex_OP_t _op) {
  return gasneti_op_valid(_op) && gasneti_op_reduce(_op);
}

// Predicates on masks:
// All bits (if any) must satisfy the predicate.
// As defined, however, these are trivially true on empty sets.
#define gasneti_op_valid_mask(ops)  (!((ops) & ~_GEX_OP_VALID))
#define gasneti_op_reduce_mask(ops) (!((ops) & ~_GEX_OP_REDUCE))
#define gasneti_op_atomic_mask(ops) (!((ops) & ~_GEX_OP_ATOMIC))
#define gasneti_op_int_mask(ops)    (!((ops) & ~_GEX_OP_INT))
#define gasneti_op_fp_mask(ops)     (!((ops) & ~_GEX_OP_FP))

/* ------------------------------------------------------------------------------------ */

#endif
