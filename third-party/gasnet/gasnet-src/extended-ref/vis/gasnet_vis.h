/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_vis.h $
 * Description: GASNet Extended API Vector, Indexed & Strided declarations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_VIS_H
#define _GASNET_VIS_H

#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_IN_GASNET_INTERNAL_H)
  #error Internal GASNet code should not directly include gasnet_vis.h, just gasnet_internal.h
#endif

#include <gasnetex.h>

GASNETI_BEGIN_EXTERNC
GASNETI_BEGIN_NOWARN

/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_memveclist_totalsz)
uintptr_t gasnete_memveclist_totalsz(size_t _count, gex_Memvec_t const *_list) {
  uintptr_t _retval = 0;
  for (size_t _i = 0; _i < _count; _i++) {
    _retval += _list[_i].gex_len;
  }
  return _retval;
}

GASNETI_INLINE(gasnete_memveclist_stats)
gasneti_memveclist_stats_t gasnete_memveclist_stats(size_t _count, gex_Memvec_t const *_list) {
  gasneti_memveclist_stats_t _retval;
  size_t _minsz = (size_t)-1, _maxsz = 0;
  uintptr_t _totalsz = 0;
  char *_minaddr = (char *)(intptr_t)(uintptr_t)-1;
  char *_maxaddr = (char *)0;
  for (size_t _i = 0; _i < _count; _i++) {
    size_t const _len = _list[_i].gex_len;
    char * const _addr = (char *)_list[_i].gex_addr;
    char * const _end = _addr + _len - 1;
    if (_len > 0) {
      if (_len < _minsz) _minsz = _len;
      if (_len > _maxsz) _maxsz = _len;
      if (_addr < _minaddr) _minaddr = _addr;
      if (_end > _maxaddr) _maxaddr = _end;
      _totalsz += _len;
    }
  }
  _retval._minsz = _minsz;
  _retval._maxsz = _maxsz;
  _retval._minaddr = _minaddr;
  _retval._maxaddr = _maxaddr;
  _retval._totalsz = _totalsz;
  gasneti_assert_uint(_totalsz ,==, gasnete_memveclist_totalsz(_count, _list));
  return _retval;
}
/*---------------------------------------------------------------------------------*/

GASNETI_INLINE(gasnete_addrlist_stats)
gasneti_addrlist_stats_t gasnete_addrlist_stats(size_t _count, void * const *_list, size_t _len) {
  gasneti_addrlist_stats_t _retval;
  char *_minaddr = (char *)(intptr_t)(uintptr_t)-1;
  char *_maxaddr = (char *)0;
#if PLATFORM_COMPILER_GNU && PLATFORM_COMPILER_VERSION_EQ(4,5,1)
  ssize_t _i; /* size_t triggers an ICE exclusive to gcc-4.5.1, but this gets a warning instead */
#else
  size_t _i;
#endif
  for (_i = 0; _i < _count; _i++) {
    char * const _addr = (char *)_list[_i];
    char * const _end = _addr + _len - 1;
    if (_addr < _minaddr) _minaddr = _addr;
    if (_end > _maxaddr) _maxaddr = _end;
  }
  _retval._minaddr = _minaddr;
  _retval._maxaddr = _maxaddr;
  return _retval;
}

/*---------------------------------------------------------------------------------*/
#if GASNETE_OLD_STRIDED
#error GASNETE_OLD_STRIDED is no longer supported in this release.
#endif

/* returns non-zero iff the specified strided region is empty */
GASNETI_INLINE(gasnete_strided_empty)
int gasnete_strided_empty(size_t _elemsz, size_t const *_count, size_t _stridelevels) {
  if_pf (!_elemsz) return 1;
  for (size_t _i = 0; _i < _stridelevels; _i++) {
    if_pf (_count[_i] == 0) return 1;
  }
  return 0;
}

/* returns the length of the bounding box containing all the data, and optionally the adjusted baseptr */
GASNETI_INLINE(gasnete_strided_bounds)
size_t gasnete_strided_bounds(ptrdiff_t const *_strides, size_t _elemsz, size_t const *_count, size_t _stridelevels, const void **_baseptr) {
  uintptr_t _lo = 0;
  uintptr_t _hi = 0;
  for (size_t _d = 0; _d < _stridelevels; _d++) {
     ptrdiff_t const _stride = _strides[_d];
     size_t const _cnt = _count[_d];
     if (_stride >= 0) _hi += _stride * (_cnt - 1);
     else              _lo += _stride * (_cnt - 1);
  }
  if (_baseptr) *_baseptr = (uint8_t*)*_baseptr + _lo;
  return _hi - _lo + _elemsz; // adjust for length of last element
}

/* returns the total bytes of data in the transfer */
GASNETI_INLINE(gasnete_strided_datasize)
size_t gasnete_strided_datasize(size_t _elemsz, size_t const *_count, size_t _stridelevels) {
  size_t _sz = _elemsz;
  for (size_t _i = 0; _i < _stridelevels; _i++) {
    size_t const _cnt = _count[_i];
    if_pf (_sz == 0) return 0;
    _sz *= _cnt;
  }
  return _sz;
}

#if GASNET_NDEBUG
  #define gasnete_boundscheck_memveclist(tm, node, count, list)                             ((void)0)
  #define gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist)           ((void)0)
  #define gasnete_boundscheck_addrlist(tm, node, count, list, len)                          ((void)0)
  #define gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen)               ((void)0)
  #define gasnete_check_stridesNT(dststrides, srcstrides, count, stridelevels)              ((void)0)
#else
  #define gasnete_boundscheck_memveclist(tm, rank, count, list) do { \
    gex_TM_t __tm = (tm);                                            \
    gex_Rank_t __node = (rank);                                      \
    size_t __count = (count);                                        \
    gex_Memvec_t const * const __list = (list);                      \
    for (size_t _i=0; _i < __count; _i++) {                          \
      if (__list[_i].gex_len > 0)                                    \
        gasneti_boundscheck(__tm, __node,                            \
                            __list[_i].gex_addr, __list[_i].gex_len);\
    }                                                                \
  } while (0)

  #define gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist) do {         \
    gasneti_memveclist_stats_t _dststats = gasnete_memveclist_stats((dstcount), (dstlist));    \
    gasneti_memveclist_stats_t _srcstats = gasnete_memveclist_stats((srccount), (srclist));    \
    if_pf (_dststats._totalsz != _srcstats._totalsz) {                                         \
      char * _dstlist_str =                                                                    \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(dstcount));         \
      char * _srclist_str =                                                                    \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(srccount));         \
      gasneti_format_memveclist(_dstlist_str, (dstcount), (dstlist));                          \
      gasneti_format_memveclist(_srclist_str, (srccount), (srclist));                          \
      gasneti_fatalerror("Source and destination memvec lists disagree on total size at %s:\n" \
                         "  srclist: %s\n"                                                     \
                         "  dstlist: %s\n",                                                    \
                         gasneti_current_loc, _dstlist_str, _srclist_str);                     \
      /* gasneti_extern_free(_dstlist_str); -- dead code */                                    \
      /* gasneti_extern_free(_srclist_str); -- dead code */                                    \
    }                                                                                          \
    if_pf (_dststats._totalsz != 0 &&                                                          \
      ((uintptr_t)_dststats._minaddr) + _dststats._totalsz - 1 > ((uintptr_t)_dststats._maxaddr)) { \
      char * _dstlist_str =                                                                    \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(dstcount));         \
      gasneti_format_memveclist(_dstlist_str, (dstcount), (dstlist));                          \
      gasneti_fatalerror("Destination memvec list has overlapping elements at %s:\n"           \
                         "  dstlist: %s\n"                                                     \
                         "(note this test is currently conservative "                          \
                         "and may fail to detect some illegal cases)",                         \
                         gasneti_current_loc, _dstlist_str);                                   \
      /* gasneti_extern_free(_dstlist_str); -- dead code */                                    \
    }                                                                                          \
  } while (0)

  #define gasnete_boundscheck_addrlist(tm, rank, count, list, len) do { \
    gex_TM_t __tm = (tm);                                           \
    gex_Rank_t __node = (rank);                                     \
    size_t __count = (count);                                       \
    void * const * const __list = (list);                           \
    size_t __len = (len);                                           \
    if_pt (__len > 0) {                                             \
      for (size_t __i=0; __i < __count; __i++) {                    \
        gasneti_boundscheck(__tm, __node, __list[__i], __len);      \
      }                                                             \
    }                                                               \
  } while (0)

  #define gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen) do {        \
    if_pf ((dstcount)*(dstlen) != (srccount)*(srclen)) {                                  \
      gasneti_fatalerror("Total data size mismatch at: %s\n"                              \
       "dstcount(%" PRIuSZ ")*dstlen(%" PRIuSZ ") != srccount(%" PRIuSZ ")*srclen(%" PRIuSZ ")", \
                         gasneti_current_loc,                                             \
                         dstcount, dstlen, srccount, srclen);                             \
    }                                                                                     \
  } while (0)

  // g2ex check for non-transpositional strides on LEGACY-format metadata
  #define gasnete_check_stridesNT(dststrides, srcstrides, count, stridelevels) do {                     \
    const size_t * const __dststrides = (dststrides);                                                   \
    const size_t * const __srcstrides = (srcstrides);                                                   \
    const size_t * const __count = (count);                                                             \
    const size_t __stridelevels = (stridelevels);                                                       \
    gasneti_assert(__count);                                                                            \
    if_pt (!gasnete_strided_empty(__count[0], __count+1, __stridelevels)) {                             \
      gasneti_assert(__dststrides); gasneti_assert(__srcstrides);                                       \
      if_pf (__stridelevels > 0 && __dststrides[0] < __count[0])                                        \
          gasneti_fatalerror("dststrides[0](%" PRIuSZ ") < count[0](%" PRIuSZ ") at: %s",               \
                        __dststrides[0],__count[0], gasneti_current_loc);                               \
      if_pf (__stridelevels > 0 && __srcstrides[0] < __count[0])                                        \
          gasneti_fatalerror("srcstrides[0](%" PRIuSZ ") < count[0](%" PRIuSZ ") at: %s",               \
                        __srcstrides[0],__count[0], gasneti_current_loc);                               \
      for (size_t _i = 1; _i < __stridelevels; _i++) {                                                  \
        if_pf (__dststrides[_i] < (__count[_i] * __dststrides[_i-1]))                                   \
          gasneti_fatalerror("dststrides[%" PRIuSZ "](%" PRIuSZ ") < "                                  \
                  "(count[%" PRIuSZ "](%" PRIuSZ ") * dststrides[%" PRIuSZ "](%" PRIuSZ ")) at: %s",    \
                     _i,__dststrides[_i], _i,__count[_i], _i-1,__dststrides[_i-1], gasneti_current_loc); \
        if_pf (__srcstrides[_i] < (__count[_i] * __srcstrides[_i-1]))                                   \
          gasneti_fatalerror("srcstrides[%" PRIuSZ "](%" PRIuSZ ") < "                                  \
                  "(count[%" PRIuSZ "](%" PRIuSZ ") * srcstrides[%" PRIuSZ "](%" PRIuSZ ")) at: %s",    \
                     _i,__srcstrides[_i], _i,__count[_i], _i-1,__srcstrides[_i-1], gasneti_current_loc); \
      }                                                                                                 \
    }                                                                                                   \
  } while (0)

#endif

typedef enum _gasnete_synctype_t {
  gasnete_synctype_b,
  gasnete_synctype_nb,
  gasnete_synctype_nbi
} gasnete_synctype_t;

/*---------------------------------------------------------------------------------*/
// Peer completion support

#ifndef GEX_VIS_MAX_PEERCOMPLETION
#define GEX_VIS_MAX_PEERCOMPLETION 127
#endif

extern void gasnete_VIS_SetPeerCompletionHandler(gex_AM_Index_t _handler,
        const void *_source_addr, size_t _nbytes, gex_Flags_t _flags GASNETI_THREAD_FARG);
#define gex_VIS_SetPeerCompletionHandler(handler, source_addr, nbytes, flags) \
    gasnete_VIS_SetPeerCompletionHandler(handler, source_addr, nbytes, flags GASNETI_THREAD_GET)

// For this release, VIS PC is implemented as a functionally correct proof-of-concept 
// that uses initiator-chaining in the public header for expedience.
// TODO-EX: Replace this prototype with pipelined PC
// This is a really gross macro hack, but it's strictly temporary.
GASNETI_INLINE(_gasnete_vis_havepc)
int _gasnete_vis_havepc(const void * const _ti) {
  gasneti_assert(_ti);
  gasnete_vis_pcinfo_t const * const _vis_ti = ((gasnete_vis_pcinfo_t const * const *)_ti)[2];
  return _vis_ti && _vis_ti->_handler;
}
// TODO-EX: comment out GASNETI_MYTHREAD_GET_OR_LOOKUP defn once we remove this sole use
#define GASNETE_VIS_HAVEPC() _gasnete_vis_havepc(GASNETI_MYTHREAD_GET_OR_LOOKUP)
#define _GASNETE_VIS_PCWRAP(tm,rank,flags,fnbase,syncmode,opargs) (                      \
    GASNETE_VIS_HAVEPC() ?                                                               \
      gasnete_VIS_pcwrap##syncmode(tm,rank,flags,(fnbase##NB opargs) GASNETI_THREAD_GET) \
    : fnbase##syncmode opargs                                                            \
)
#define _GASNETE_VIS_PCWRAP_ARGS gex_TM_t _tm, gex_Rank_t _rank, gex_Flags_t _flags, gex_Event_t _evt GASNETI_THREAD_FARG
extern int         gasnete_VIS_pcwrapBlocking(_GASNETE_VIS_PCWRAP_ARGS);
extern int         gasnete_VIS_pcwrapNBI     (_GASNETE_VIS_PCWRAP_ARGS);
extern gex_Event_t gasnete_VIS_pcwrapNB      (_GASNETE_VIS_PCWRAP_ARGS) GASNETI_WARN_UNUSED_RESULT;

/*---------------------------------------------------------------------------------*/
// Degenerate contiguous RMA support

#if GASNETE_PUTGET_ALWAYSREMOTE
  // TODO-EX: This disgusting hack is brought to you by the revolting GASNETE_PUTGET_ALWAYSREMOTE option
  // that breaks loopback operation of the contiguous RMA APIs. Remove that option and this hack!
  GASNETI_INLINE(_gasnete_vis_handle_local_put)
  int _gasnete_vis_handle_local_put(gex_TM_t _tm, gex_Rank_t _rank, void *_dstaddr, void *_srcaddr, size_t _nbytes) {
    void *_local_dstaddr = GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(_tm, _rank, _dstaddr);
    if (_local_dstaddr) {
      GASNETI_MEMCPY_SAFE_EMPTY(_local_dstaddr, _srcaddr, _nbytes);
      return 1;
    } else return 0;
  }
  GASNETI_INLINE(_gasnete_vis_handle_local_get)
  int _gasnete_vis_handle_local_get(gex_TM_t _tm, gex_Rank_t _rank, void *_dstaddr, void *_srcaddr, size_t _nbytes) {
    void *_local_srcaddr = GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(_tm, _rank, _srcaddr);
    if (_local_srcaddr) {
      GASNETI_MEMCPY_SAFE_EMPTY(_dstaddr, _local_srcaddr, _nbytes);
      return 1;
    } else return 0;
  }
#else
  #define _gasnete_vis_handle_local_put(tm,rank,dstaddr,srcaddr,nbytes) 0
  #define _gasnete_vis_handle_local_get(tm,rank,dstaddr,srcaddr,nbytes) 0
#endif

// Implement GEX_FLAG_ENABLE_LEAF_LC for calls to contiguous RMA
// _lc_dummy is a stack temporary used to request handle-based LC via later QueryLeaf
#define _GASNETE_LCOPT_FLAGS_NB  ((_flags & GEX_FLAG_ENABLE_LEAF_LC) ? &_lc_dummy : GEX_EVENT_DEFER), _flags
#define _GASNETE_LCOPT_FLAGS_NBI ((_flags & GEX_FLAG_ENABLE_LEAF_LC) ? GEX_EVENT_GROUP : GEX_EVENT_DEFER), _flags

#define gasnete_vis_degen_PutBlocking(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_put(_tm,_dstrank,dstaddr,srcaddr,len) ? 0 : \
   _gex_RMA_PutBlocking(_tm,_dstrank,dstaddr,srcaddr,len,_flags GASNETI_THREAD_PASS))
#define gasnete_vis_degen_PutNB(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_put(_tm,_dstrank,dstaddr,srcaddr,len) ? 0L : \
   _gex_RMA_PutNB(_tm,_dstrank,dstaddr,srcaddr,len,_GASNETE_LCOPT_FLAGS_NB GASNETI_THREAD_PASS))
#define gasnete_vis_degen_PutNBI(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_put(_tm,_dstrank,dstaddr,srcaddr,len) ? 0 : \
   _gex_RMA_PutNBI(_tm,_dstrank,dstaddr,srcaddr,len,_GASNETE_LCOPT_FLAGS_NBI GASNETI_THREAD_PASS))

#define gasnete_vis_degen_GetBlocking(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_get(_tm,_srcrank,dstaddr,srcaddr,len) ? 0 : \
   _gex_RMA_GetBlocking(_tm,dstaddr,_srcrank,srcaddr,len,_flags GASNETI_THREAD_PASS))
#define gasnete_vis_degen_GetNB(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_get(_tm,_srcrank,dstaddr,srcaddr,len) ? 0L : \
   _gex_RMA_GetNB(_tm,dstaddr,_srcrank,srcaddr,len,_flags GASNETI_THREAD_PASS))
#define gasnete_vis_degen_GetNBI(dstaddr, srcaddr, len) \
  (_gasnete_vis_handle_local_get(_tm,_srcrank,dstaddr,srcaddr,len) ? 0 : \
   _gex_RMA_GetNBI(_tm,dstaddr,_srcrank,srcaddr,len,_flags GASNETI_THREAD_PASS))
/*---------------------------------------------------------------------------------*/
/* Vector */
#ifndef gasnete_putv
  extern gex_Event_t gasnete_putv(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif
#ifndef gasnete_getv
  extern gex_Event_t gasnete_getv(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        gex_Rank_t _srcrank,
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif

#define _GASNETE_VECTOR_COMMON_GET(degencontigop) \
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); \
  gasneti_assert(!GASNETE_VIS_HAVEPC());               \
  gasnete_boundscheck_memveclist(_tm, _srcrank, _srccount, _srclist); \
  _GASNETE_VECTOR_COMMON(degencontigop, GETV_DEGENERATE)
#define _GASNETE_VECTOR_COMMON_PUT(degencontigop) \
  gex_Event_t _lc_dummy;                          \
  gasnete_boundscheck_memveclist(_tm, _dstrank, _dstcount, _dstlist); \
  _GASNETE_VECTOR_COMMON(degencontigop, PUTV_DEGENERATE)
#define _GASNETE_VECTOR_COMMON(degencontigop,degentoken)                   \
  gasnete_memveclist_checksizematch(_dstcount, _dstlist, _srccount, _srclist); \
  if_pf (_dstcount == 0 || _srccount == 0) {  /* no-op */                  \
    GASNETI_TRACE_EVENT(C, degentoken);                                    \
    return 0;                                                              \
  }                                                                        \
  gasneti_assert(_dstlist); gasneti_assert(_srclist);                      \
  if_pf (_dstcount + _srccount == 2) {                                     \
    gasneti_assert(_dstcount == 1 && _srccount == 1);                      \
    GASNETI_TRACE_EVENT(C, degentoken);                                    \
    return degencontigop;                                                  \
  } 

GASNETI_INLINE(_gex_VIS_VectorPutBlocking)
int _gex_VIS_VectorPutBlocking(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTV(PUTV_BULK,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist);
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); // forbidden for Blocking put
  _GASNETE_VECTOR_COMMON_PUT(gasnete_vis_degen_PutBlocking(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return (int)(intptr_t)gasnete_putv(gasnete_synctype_b,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorPutBlocking(tm,dstrank,dstcount,dstlist,srccount,srclist,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_VectorPut,Blocking,(tm,dstrank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_VectorGetBlocking)
int _gex_VIS_VectorGetBlocking(
        gex_TM_t _tm,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        gex_Rank_t _srcrank,
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETV(GETV_BULK,_tm,_srcrank,_dstcount,_dstlist,_srccount,_srclist);
  _GASNETE_VECTOR_COMMON_GET(gasnete_vis_degen_GetBlocking(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return (int)(intptr_t)gasnete_getv(gasnete_synctype_b,_tm,_dstcount,_dstlist,_srcrank,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorGetBlocking(tm,dstcount,dstlist,srcrank,srccount,srclist,flags) \
       _gex_VIS_VectorGetBlocking(tm,dstcount,dstlist,srcrank,srccount,srclist,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_VectorPutNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_VectorPutNB(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTV(PUTV_NB_BULK,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist);
  _GASNETE_VECTOR_COMMON_PUT(gasnete_vis_degen_PutNB(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return gasnete_putv(gasnete_synctype_nb,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorPutNB(tm,dstrank,dstcount,dstlist,srccount,srclist,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_VectorPut,NB,(tm,dstrank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_VectorGetNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_VectorGetNB(
        gex_TM_t _tm,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        gex_Rank_t _srcrank,
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETV(GETV_NB_BULK,_tm,_srcrank,_dstcount,_dstlist,_srccount,_srclist);
  _GASNETE_VECTOR_COMMON_GET(gasnete_vis_degen_GetNB(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return gasnete_getv(gasnete_synctype_nb,_tm,_dstcount,_dstlist,_srcrank,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorGetNB(tm,dstcount,dstlist,srcrank,srccount,srclist,flags) \
       _gex_VIS_VectorGetNB(tm,dstcount,dstlist,srcrank,srccount,srclist,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_VectorPutNBI)
int _gex_VIS_VectorPutNBI(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTV(PUTV_NBI_BULK,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist);
  _GASNETE_VECTOR_COMMON_PUT(gasnete_vis_degen_PutNBI(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return (int)(intptr_t)gasnete_putv(gasnete_synctype_nbi,_tm,_dstrank,_dstcount,_dstlist,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorPutNBI(tm,dstrank,dstcount,dstlist,srccount,srclist,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_VectorPut,NBI,(tm,dstrank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_VectorGetNBI)
int _gex_VIS_VectorGetNBI(
        gex_TM_t _tm,
        size_t _dstcount, gex_Memvec_t const _dstlist[],
        gex_Rank_t _srcrank,
        size_t _srccount, gex_Memvec_t const _srclist[],
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETV(GETV_NBI_BULK,_tm,_srcrank,_dstcount,_dstlist,_srccount,_srclist);
  _GASNETE_VECTOR_COMMON_GET(gasnete_vis_degen_GetNBI(_dstlist[0].gex_addr,_srclist[0].gex_addr,_dstlist[0].gex_len));
  return (int)(intptr_t)gasnete_getv(gasnete_synctype_nbi,_tm,_dstcount,_dstlist,_srcrank,_srccount,_srclist,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_VectorGetNBI(tm,dstcount,dstlist,srcrank,srccount,srclist,flags) \
       _gex_VIS_VectorGetNBI(tm,dstcount,dstlist,srcrank,srccount,srclist,flags GASNETI_THREAD_GET)

#undef _GASNETE_VECTOR_COMMON
#undef _GASNETE_VECTOR_COMMON_PUT
#undef _GASNETE_VECTOR_COMMON_GET

/*---------------------------------------------------------------------------------*/
/* Indexed */
#ifndef gasnete_puti
  extern gex_Event_t gasnete_puti(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif
#ifndef gasnete_geti
  extern gex_Event_t gasnete_geti(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        gex_Rank_t _srcrank,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif

#define _GASNETE_INDEXED_COMMON_GET(degencontigop) \
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); \
  gasneti_assert(!GASNETE_VIS_HAVEPC());               \
  gasnete_boundscheck_addrlist(_tm, _srcrank, _srccount, _srclist, _srclen); \
  _GASNETE_INDEXED_COMMON(degencontigop, GETI_DEGENERATE)
#define _GASNETE_INDEXED_COMMON_PUT(degencontigop) \
  gex_Event_t _lc_dummy;                           \
  gasnete_boundscheck_addrlist(_tm, _dstrank, _dstcount, _dstlist, _dstlen); \
  _GASNETE_INDEXED_COMMON(degencontigop, PUTI_DEGENERATE)
#define _GASNETE_INDEXED_COMMON(degencontigop,degentoken)                  \
  gasnete_addrlist_checksizematch(_dstcount, _dstlen, _srccount, _srclen); \
  if_pf (_dstcount*_dstlen == 0) {  /* no-op */                            \
    gasneti_assert_uint(_srccount*_srclen ,==, 0);                         \
    GASNETI_TRACE_EVENT(C, degentoken);                                    \
    return 0;                                                              \
  } else gasneti_assert_uint(_srccount*_srclen ,>, 0);                     \
  gasneti_assert(_dstlist); gasneti_assert(_srclist);                      \
  if_pf (_dstcount + _srccount == 2) {                                     \
    gasneti_assert(_dstcount == 1 && _srccount == 1);                      \
    GASNETI_TRACE_EVENT(C, degentoken);                                    \
    return degencontigop;                                                  \
  } 

GASNETI_INLINE(_gex_VIS_IndexedPutBlocking)
int _gex_VIS_IndexedPutBlocking(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTI(PUTI_BULK,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); // forbidden for Blocking put
  _GASNETE_INDEXED_COMMON_PUT(gasnete_vis_degen_PutBlocking(_dstlist[0],_srclist[0],_dstlen));
  return (int)(intptr_t)gasnete_puti(gasnete_synctype_b,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedPutBlocking(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_IndexedPut,Blocking,(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_IndexedGetBlocking)
int _gex_VIS_IndexedGetBlocking(
        gex_TM_t _tm,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        gex_Rank_t _srcrank,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETI(GETI_BULK,_tm,_srcrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  _GASNETE_INDEXED_COMMON_GET(gasnete_vis_degen_GetBlocking(_dstlist[0],_srclist[0],_dstlen));
  return (int)(intptr_t)gasnete_geti(gasnete_synctype_b,_tm,_dstcount,_dstlist,_dstlen,_srcrank,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedGetBlocking(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags) \
       _gex_VIS_IndexedGetBlocking(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_IndexedPutNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_IndexedPutNB(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTI(PUTI_NB_BULK,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  _GASNETE_INDEXED_COMMON_PUT(gasnete_vis_degen_PutNB(_dstlist[0],_srclist[0],_dstlen));
  return gasnete_puti(gasnete_synctype_nb,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedPutNB(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_IndexedPut,NB,(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_IndexedGetNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_IndexedGetNB(
        gex_TM_t _tm,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        gex_Rank_t _srcrank,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETI(GETI_NB_BULK,_tm,_srcrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  _GASNETE_INDEXED_COMMON_GET(gasnete_vis_degen_GetNB(_dstlist[0],_srclist[0],_dstlen));
  return gasnete_geti(gasnete_synctype_nb,_tm,_dstcount,_dstlist,_dstlen,_srcrank,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedGetNB(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags) \
       _gex_VIS_IndexedGetNB(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_IndexedPutNBI)
int _gex_VIS_IndexedPutNBI(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTI(PUTI_NBI_BULK,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  _GASNETE_INDEXED_COMMON_PUT(gasnete_vis_degen_PutNBI(_dstlist[0],_srclist[0],_dstlen));
  return (int)(intptr_t)gasnete_puti(gasnete_synctype_nbi,_tm,_dstrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedPutNBI(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_IndexedPut,NBI,(tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_IndexedGetNBI)
int _gex_VIS_IndexedGetNBI(
        gex_TM_t _tm,
        size_t _dstcount, void * const _dstlist[], size_t _dstlen,
        gex_Rank_t _srcrank,
        size_t _srccount, void * const _srclist[], size_t _srclen,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETI(GETI_NBI_BULK,_tm,_srcrank,_dstcount,_dstlist,_dstlen,_srccount,_srclist,_srclen);
  _GASNETE_INDEXED_COMMON_GET(gasnete_vis_degen_GetNBI(_dstlist[0],_srclist[0],_dstlen));
  return (int)(intptr_t)gasnete_geti(gasnete_synctype_nbi,_tm,_dstcount,_dstlist,_dstlen,_srcrank,_srccount,_srclist,_srclen,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_IndexedGetNBI(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags) \
       _gex_VIS_IndexedGetNBI(tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,flags GASNETI_THREAD_GET)

#undef _GASNETE_INDEXED_COMMON
#undef _GASNETE_INDEXED_COMMON_PUT
#undef _GASNETE_INDEXED_COMMON_GET

/*---------------------------------------------------------------------------------*/
/* Strided */

#ifndef gasnete_puts
  extern gex_Event_t gasnete_puts(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG);

#endif
#ifndef gasnete_gets
  extern gex_Event_t gasnete_gets(
        gasnete_synctype_t _synctype,
        gex_TM_t _tm,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif

#define _GASNETE_STRIDED_COMMON_GET(degencontigop)  \
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); \
  gasneti_assert(!GASNETE_VIS_HAVEPC());               \
  _GASNETE_STRIDED_COMMON(degencontigop, GETS_DEGENERATE)
#define _GASNETE_STRIDED_COMMON_PUT(degencontigop)  \
  gex_Event_t _lc_dummy;                            \
  _GASNETE_STRIDED_COMMON(degencontigop, PUTS_DEGENERATE)
#define _GASNETE_STRIDED_COMMON(degencontigop,degentoken)  \
  if_pf (_elemsz == 0) {                                   \
    GASNETI_TRACE_EVENT(C, degentoken);                    \
    return 0;                                              \
  } else if_pf (_stridelevels == 0) {                      \
    GASNETI_TRACE_EVENT(C, degentoken);                    \
    return degencontigop;                                  \
  } else { /* check array validity */                      \
    gasneti_assert(_dststrides);                           \
    gasneti_assert(_srcstrides);                           \
    gasneti_assert(_count);                                \
  }

GASNETI_INLINE(_gex_VIS_StridedPutBlocking)
int _gex_VIS_StridedPutBlocking(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTS(PUTS_BULK,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  gasneti_assert(!(_flags & GEX_FLAG_ENABLE_LEAF_LC)); // forbidden for Blocking put
  _GASNETE_STRIDED_COMMON_PUT(gasnete_vis_degen_PutBlocking(_dstaddr,_srcaddr,_elemsz));
  return (int)(intptr_t)gasnete_puts(gasnete_synctype_b,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedPutBlocking(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_StridedPut,Blocking,(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_StridedGetBlocking)
int _gex_VIS_StridedGetBlocking(
        gex_TM_t _tm,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETS(GETS_BULK,_tm,_srcrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  _GASNETE_STRIDED_COMMON_GET(gasnete_vis_degen_GetBlocking(_dstaddr,_srcaddr,_elemsz));
  return (int)(intptr_t)gasnete_gets(gasnete_synctype_b,_tm,_dstaddr,_dststrides,_srcrank,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedGetBlocking(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
       _gex_VIS_StridedGetBlocking(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_StridedPutNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_StridedPutNB(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTS(PUTS_NB_BULK,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  _GASNETE_STRIDED_COMMON_PUT(gasnete_vis_degen_PutNB(_dstaddr,_srcaddr,_elemsz));
  return gasnete_puts(gasnete_synctype_nb,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedPutNB(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_StridedPut,NB,(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_StridedGetNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_VIS_StridedGetNB(
        gex_TM_t _tm,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETS(GETS_NB_BULK,_tm,_srcrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  _GASNETE_STRIDED_COMMON_GET(gasnete_vis_degen_GetNB(_dstaddr,_srcaddr,_elemsz));
  return gasnete_gets(gasnete_synctype_nb,_tm,_dstaddr,_dststrides,_srcrank,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedGetNB(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
       _gex_VIS_StridedGetNB(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_VIS_StridedPutNBI)
int _gex_VIS_StridedPutNBI(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PUTS(PUTS_NBI_BULK,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  _GASNETE_STRIDED_COMMON_PUT(gasnete_vis_degen_PutNBI(_dstaddr,_srcaddr,_elemsz));
  return (int)(intptr_t)gasnete_puts(gasnete_synctype_nbi,_tm,_dstrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedPutNBI(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
    _GASNETE_VIS_PCWRAP(tm,dstrank,flags, \
       _gex_VIS_StridedPut,NBI,(tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET))

GASNETI_INLINE(_gex_VIS_StridedGetNBI)
int _gex_VIS_StridedGetNBI(
        gex_TM_t _tm,
        void *_dstaddr, const ptrdiff_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const ptrdiff_t _srcstrides[],
        size_t _elemsz, const size_t _count[], size_t _stridelevels,
        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_GETS(GETS_NBI_BULK,_tm,_srcrank,_dstaddr,_dststrides,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels);
  _GASNETE_STRIDED_COMMON_GET(gasnete_vis_degen_GetNBI(_dstaddr,_srcaddr,_elemsz));
  return (int)(intptr_t)gasnete_gets(gasnete_synctype_nbi,_tm,_dstaddr,_dststrides,_srcrank,_srcaddr,_srcstrides,_elemsz,_count,_stridelevels,_flags GASNETI_THREAD_PASS);
}
#define gex_VIS_StridedGetNBI(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags) \
       _gex_VIS_StridedGetNBI(tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,elemsz,count,stridelevels,flags GASNETI_THREAD_GET)

#undef _GASNETE_STRIDED_COMMON
#undef _GASNETE_STRIDED_COMMON_GET
#undef _GASNETE_STRIDED_COMMON_PUT

/*---------------------------------------------------------------------------------*/
// g2ex Strided wrappers
// These translate the Strided metadata from legacy to EX format

#define _GASNETE_G2EX_STRIDED_COMMON() \
  gasneti_assert_uint(sizeof(size_t) ,==, sizeof(ptrdiff_t)); \
  gasnete_check_stridesNT(_dststrides, _srcstrides, _count, _stridelevels)

GASNETI_INLINE(_gasnet_puts_bulk)
void _gasnet_puts_bulk(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const size_t _dststrides[],
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  _gex_VIS_StridedPutBlocking(_tm,_dstrank,_dstaddr,(ptrdiff_t*)_dststrides,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}
GASNETI_INLINE(_gasnet_gets_bulk)
void _gasnet_gets_bulk(
        gex_TM_t _tm,
        void *_dstaddr, const size_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  _gex_VIS_StridedGetBlocking(_tm,_dstaddr,(ptrdiff_t*)_dststrides,_srcrank,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}
GASNETI_INLINE(_gasnet_puts_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gasnet_puts_nb_bulk(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const size_t _dststrides[],
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  return _gex_VIS_StridedPutNB(_tm,_dstrank,_dstaddr,(ptrdiff_t*)_dststrides,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}
GASNETI_INLINE(_gasnet_gets_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gasnet_gets_nb_bulk(
        gex_TM_t _tm,
        void *_dstaddr, const size_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  return _gex_VIS_StridedGetNB(_tm,_dstaddr,(ptrdiff_t*)_dststrides,_srcrank,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}
GASNETI_INLINE(_gasnet_puts_nbi_bulk)
void _gasnet_puts_nbi_bulk(
        gex_TM_t _tm, gex_Rank_t _dstrank,
        void *_dstaddr, const size_t _dststrides[],
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  _gex_VIS_StridedPutNBI(_tm,_dstrank,_dstaddr,(ptrdiff_t*)_dststrides,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}
GASNETI_INLINE(_gasnet_gets_nbi_bulk)
void _gasnet_gets_nbi_bulk(
        gex_TM_t _tm,
        void *_dstaddr, const size_t _dststrides[],
        gex_Rank_t _srcrank,
        void *_srcaddr, const size_t _srcstrides[],
        const size_t _count[], size_t _stridelevels
        GASNETI_THREAD_FARG) {
  _GASNETE_G2EX_STRIDED_COMMON();
  _gex_VIS_StridedGetNBI(_tm,_dstaddr,(ptrdiff_t*)_dststrides,_srcrank,_srcaddr,(ptrdiff_t*)_srcstrides,_count[0],_count+1,_stridelevels,0 GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

#undef gasnete_vis_degen_PutBlocking
#undef gasnete_vis_degen_PutNBI
#undef gasnete_vis_degen_GetBlocking
#undef gasnete_vis_degen_GetNB
#undef gasnete_vis_degen_GetNBI

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#endif
