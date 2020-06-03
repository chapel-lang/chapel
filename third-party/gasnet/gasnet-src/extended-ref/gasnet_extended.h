/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended.h $
 * Description: GASNet Extended API Header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_H
#define _GASNET_EXTENDED_H

#include <string.h>

#include <gasnet_extended_help.h>

/*  TODO: add debug code to enforce restrictions on SEQ and PARSYNC config */
/*        (only one thread calls, HSL's only locked by that thread - how to check without pthread_getspecific()?) */
/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* passes back a pointer to a handler table containing the handlers of
    the extended API, which the core should register on its behalf
    (the table is terminated with an entry where fnptr == NULL)
   all handlers will have an index in range 100-199 // <== TODO-EX: update range here
   may be called before gasnete_init()
*/
extern gex_AM_Entry_t const *gasnete_get_handlertable(void);

/* Initialize the Extended API:
   must be called by the core API at the end of gasnet_attach() before calls to extended API
     (this function may make calls to the core functions)
*/
extern void gasnete_init(void);

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  ==========================================================
*/

#ifndef gasnete_put_nb
  extern gex_Event_t gasnete_put_nb(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        /*const*/ void *_src,  // TODO-EX: un-comment const
                        size_t _nbytes, gex_Event_t *_lc_opt,
                        gex_Flags_t _flags GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

#ifndef gasnete_get_nb
  extern gex_Event_t gasnete_get_nb(
                        gex_TM_t _tm, void *_dest,
                        gex_Rank_t _rank, void *_src,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

GASNETI_INLINE(_gex_RMA_GetNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_RMA_GetNB(
                        gex_TM_t _tm, void *_dest,
                        gex_Rank_t _rank, void *_src,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NB,_tm,_dest,_rank,_src,_nbytes);
  gasneti_boundscheck(_tm, _rank, _src, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_GET_LOCAL(NB,_tm,_dest,_rank,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackget_memsync();
    return GEX_EVENT_INVALID;
  } else {
    GASNETI_TRACE_GET(NB,_tm,_dest,_rank,_src,_nbytes);
    return gasnete_get_nb(_tm, _dest, _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_GetNB(tm,dest,rank,src,nbytes,flags) \
       _gex_RMA_GetNB(tm,dest,rank,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_RMA_PutNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_RMA_PutNB(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        /*const*/ void *_src,  // TODO-EX: un-comment const
                        size_t _nbytes, gex_Event_t *_lc_opt,
                        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_CHECK_PUT_LCOPT(_lc_opt, 0);
  GASNETI_CHECKZEROSZ_PUT(NB,_tm,_rank,_dest,_src,_nbytes);
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_LOCAL(NB,_tm,_rank,_dest,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackput_memsync();
    gasneti_leaf_finish(_lc_opt);
    return GEX_EVENT_INVALID;
  } else {
    GASNETI_TRACE_PUT(NB,_tm,_rank,_dest,_src,_nbytes);
    return gasnete_put_nb(_tm, _rank, _dest, _src, _nbytes, _lc_opt, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_PutNB(tm,rank,dest,src,nbytes,lc_opt,flags) \
       _gex_RMA_PutNB(tm,rank,dest,src,nbytes,lc_opt,flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-event non-blocking operations:
  ===========================================================
*/

#ifndef gasnete_test
extern int gasnete_test(gex_Event_t _event GASNETI_THREAD_FARG);
#endif
#ifndef gasnete_test_some
extern int gasnete_test_some(gex_Event_t *_pevent, size_t _numevents GASNETI_THREAD_FARG);
#endif
#ifndef gasnete_test_all
extern int gasnete_test_all (gex_Event_t *_pevent, size_t _numevents GASNETI_THREAD_FARG);
#endif


GASNETI_INLINE(_gex_Event_Test) GASNETI_WARN_UNUSED_RESULT
int  _gex_Event_Test(gex_Event_t _event GASNETI_THREAD_FARG) {
  int _result = GASNET_OK;
  if_pt (_event != GEX_EVENT_INVALID)
    _result = gasnete_test(_event GASNETI_THREAD_PASS);
  GASNETI_TRACE_TRYSYNC(TEST_SYNCNB,_result);
  return _result;
}
#define gex_Event_Test(pevent) \
       _gex_Event_Test(pevent GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_Event_TestSome)
int _gex_Event_TestSome(gex_Event_t *_pevent, size_t _numevents, gex_Flags_t _flags GASNETI_THREAD_FARG) {
  int _result = gasnete_test_some(_pevent,_numevents GASNETI_THREAD_PASS);
  GASNETI_TRACE_TRYSYNC(TEST_SYNCNB_SOME,_result);
  return _result;
}
#define gex_Event_TestSome(pevent, numevents, flags) \
       _gex_Event_TestSome(pevent, numevents, flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_Event_TestAll)
int _gex_Event_TestAll(gex_Event_t *_pevent, size_t _numevents, gex_Flags_t _flags GASNETI_THREAD_FARG) {
  int _result = gasnete_test_all(_pevent,_numevents GASNETI_THREAD_PASS);
  GASNETI_TRACE_TRYSYNC(TEST_SYNCNB_ALL,_result);
  return _result;
}
#define gex_Event_TestAll(pevent, numevents, flags) \
       _gex_Event_TestAll(pevent, numevents, flags GASNETI_THREAD_GET)


#ifndef gasnete_wait
  GASNETI_INLINE(gasnete_wait)
  void gasnete_wait(gex_Event_t _event GASNETI_THREAD_FARG) {
    if_pt (_event != GEX_EVENT_INVALID) {
      gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */
      gasneti_pollwhile(gasnete_test(_event GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
    }
  }
#endif

GASNETI_INLINE(_gex_Event_Wait)
void _gex_Event_Wait(gex_Event_t _event GASNETI_THREAD_FARG) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait(_event GASNETI_THREAD_PASS);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB);
}
#define gex_Event_Wait(event) \
       _gex_Event_Wait(event GASNETI_THREAD_GET)

#ifndef gasnete_wait_some // TODO-EX: a non-inline function could allow some optimizations
  GASNETI_INLINE(gasnete_wait_some)
  void gasnete_wait_some(gex_Event_t *_pevent, size_t _numevents GASNETI_THREAD_FARG) {
    gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */
    gasneti_pollwhile(gasnete_test_some(_pevent, _numevents GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
  }
#endif

GASNETI_INLINE(_gex_Event_WaitSome)
void _gex_Event_WaitSome(gex_Event_t *_pevent, size_t _numevents, gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait_some(_pevent, _numevents GASNETI_THREAD_PASS);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB_SOME);
}
#define gex_Event_WaitSome(pevent, numevents, flags) \
       _gex_Event_WaitSome(pevent, numevents, flags GASNETI_THREAD_GET)

#ifndef gasnete_wait_all // TODO-EX: a non-inline function could allow some optimizations
  GASNETI_INLINE(gasnete_wait_all)
  void gasnete_wait_all(gex_Event_t *_pevent, size_t _numevents GASNETI_THREAD_FARG) {
    gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */
    gasneti_pollwhile(gasnete_test_all(_pevent, _numevents GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
  }
#endif

GASNETI_INLINE(_gex_Event_WaitAll)
void _gex_Event_WaitAll(gex_Event_t *_pevent, size_t _numevents, gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait_all(_pevent, _numevents GASNETI_THREAD_PASS);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB_ALL);
}
#define gex_Event_WaitAll(pevent, numevents, flags) \
       _gex_Event_WaitAll(pevent, numevents, flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
*/

#ifndef gasnete_get_nbi
extern int gasnete_get_nbi  (gex_TM_t _tm, void *_dest,
                             gex_Rank_t _rank, void *_src,
                             size_t _nbytes, gex_Flags_t _flags
                             GASNETI_THREAD_FARG);
#endif

#ifndef gasnete_put_nbi
extern int gasnete_put_nbi  (gex_TM_t _tm,
                             gex_Rank_t _rank, void *_dest,
                             /*const*/ void *_src,  // TODO-EX: un-comment const
                             size_t _nbytes, gex_Event_t *_lc_opt,
                             gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif

GASNETI_INLINE(_gex_RMA_GetNBI)
int _gex_RMA_GetNBI  (gex_TM_t _tm, void *_dest,
                        gex_Rank_t _rank, void *_src,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NBI,_tm,_dest,_rank,_src,_nbytes);
  gasneti_boundscheck(_tm, _rank, _src, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_GET_LOCAL(NBI,_tm,_dest,_rank,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackget_memsync();
    return 0;
  } else {
    GASNETI_TRACE_GET(NBI,_tm,_dest,_rank,_src,_nbytes);
    return gasnete_get_nbi(_tm, _dest, _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_GetNBI(tm,dest,rank,src,nbytes,flags) \
       _gex_RMA_GetNBI(tm,dest,rank,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_RMA_PutNBI)
int _gex_RMA_PutNBI  (gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        /*const*/ void *_src,  // TODO-EX: un-comment const
                        size_t _nbytes, gex_Event_t *_lc_opt,
                        gex_Flags_t _flags GASNETI_THREAD_FARG) {
  GASNETI_CHECK_PUT_LCOPT(_lc_opt, 1);
  GASNETI_CHECKZEROSZ_PUT(NBI,_tm,_rank,_dest,_src,_nbytes);
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_LOCAL(NBI,_tm,_rank,_dest,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackput_memsync();
    return 0;
  } else {
    GASNETI_TRACE_PUT(NBI,_tm,_rank,_dest,_src,_nbytes);
    return gasnete_put_nbi(_tm, _rank, _dest, _src, _nbytes, _lc_opt, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_PutNBI(tm,rank,dest,src,nbytes,lc_opt,flags) \
       _gex_RMA_PutNBI(tm,rank,dest,src,nbytes,lc_opt,flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-event non-blocking operations:
  ===========================================================
*/

// The internal event categories
#define GASNETI_EC_PUT (1U << 0)
#define GASNETI_EC_ALC (1U << 1)
#define GASNETI_EC_GET (1U << 2)
#define GASNETI_EC_RMW (1U << 3)

typedef unsigned int gex_EC_t;

#ifndef GEX_EC_PUT
  #define GEX_EC_PUT  ((gex_EC_t)(GASNETI_EC_PUT|GASNETI_EC_ALC))
#endif
#ifndef GEX_EC_GET
  #define GEX_EC_GET  ((gex_EC_t)GASNETI_EC_GET)
#endif
#ifndef GEX_EC_AM
  #define GEX_EC_AM   ((gex_EC_t)GASNETI_EC_ALC)
#endif
#ifndef GEX_EC_LC
  #define GEX_EC_LC   ((gex_EC_t)GASNETI_EC_ALC)
#endif
#ifndef GEX_EC_RMW
  #define GEX_EC_RMW  ((gex_EC_t)GASNETI_EC_RMW)
#endif
#ifndef GEX_EC_ALL
  #define GEX_EC_ALL  (~(gex_EC_t)0)
#endif

#ifndef gasnete_test_syncnbi_mask
  extern int  gasnete_test_syncnbi_mask(gex_EC_t _mask, gex_Flags_t _flags GASNETI_THREAD_FARG);
#endif

GASNETI_INLINE(_gex_NBI_Test) GASNETI_WARN_UNUSED_RESULT
int _gex_NBI_Test(gex_EC_t _mask, gex_Flags_t _flags GASNETI_THREAD_FARG) {
  int _retval = gasnete_test_syncnbi_mask(_mask, _flags GASNETI_THREAD_PASS);
  GASNETI_TRACE_TRYSYNC(TEST_SYNCNBI,_retval);
  return _retval;
}
#define gex_NBI_Test(mask, flags)   \
       _gex_NBI_Test(mask, flags GASNETI_THREAD_GET)

#ifndef gasnete_wait_syncnbi_mask
  #define gasnete_wait_syncnbi_mask(mask, flagsTI) \
    gasneti_pollwhile(gasnete_test_syncnbi_mask(mask, flagsTI) == GASNET_ERR_NOT_READY)
#endif

#define gex_NBI_Wait(mask, flags) do {                                                  \
  GASNETI_TRACE_WAITSYNC_BEGIN();                                                                \
  gasneti_AMPoll(); /* ensure at least one poll */                                               \
  gasnete_wait_syncnbi_mask(mask, flags GASNETI_THREAD_GET);                                     \
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNBI);                                                      \
  } while (0)

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
#ifndef gasnete_begin_nbi_accessregion
extern void gasnete_begin_nbi_accessregion(gex_Flags_t _flags, int _allowrecursion GASNETI_THREAD_FARG);
#endif
#ifndef gasnete_end_nbi_accessregion
extern gex_Event_t gasnete_end_nbi_accessregion(gex_Flags_t _flags GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

#define gex_NBI_BeginAccessRegion(flags) gasnete_begin_nbi_accessregion(flags,0 GASNETI_THREAD_GET)
#define gex_NBI_EndAccessRegion(flags)   gasnete_end_nbi_accessregion(flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Query to get leaf event from its root
  =====================================
*/
#ifndef gasnete_Event_QueryLeaf
extern gex_Event_t gasnete_Event_QueryLeaf(gex_Event_t _root, gex_EC_t _event_id);
#endif

#define gex_Event_QueryLeaf(root, event_id) gasnete_Event_QueryLeaf(root, event_id)

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/


#if GASNETI_DIRECT_BLOCKING_GET
  extern int gasnete_get  (gex_TM_t _tm,
                           void *_dest,
                           gex_Rank_t _rank, void *_src,
                           size_t _nbytes, gex_Flags_t _flags
                           GASNETI_THREAD_FARG);
#elif !defined(gasnete_get)
  GASNETI_INLINE(gasnete_get)
  int gasnete_get (gex_TM_t _tm,
                    void *_dest,
                    gex_Rank_t _rank, void *_src,
                    size_t _nbytes, gex_Flags_t _flags
                    GASNETI_THREAD_FARG)
  {
    gex_Event_t h = gasnete_get_nb(_tm, _dest, _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
    if (h == GEX_EVENT_NO_OP) return 1;
    else gasnete_wait(h GASNETI_THREAD_PASS);
    return 0;
  }
#endif

#if GASNETI_DIRECT_BLOCKING_PUT
  extern int gasnete_put  (gex_TM_t _tm,
                           gex_Rank_t _rank, void* _dest,
                           /*const*/ void *_src, // TODO-EX: uncomment const
                           size_t _nbytes, gex_Flags_t _flags
                           GASNETI_THREAD_FARG);
#elif !defined(gasnete_put)
  GASNETI_INLINE(gasnete_put)
  int gasnete_put  (gex_TM_t _tm,
                    gex_Rank_t _rank, void* _dest,
                    /*const*/ void *_src, // TODO-EX: uncomment const
                    size_t _nbytes, gex_Flags_t _flags
                    GASNETI_THREAD_FARG)
  {
    gex_Event_t h = gasnete_put_nb(_tm, _rank, _dest, _src, _nbytes, GEX_EVENT_DEFER, _flags GASNETI_THREAD_PASS);
    if (h == GEX_EVENT_NO_OP) return 1;
    else gasnete_wait(h GASNETI_THREAD_PASS);
    return 0;
  }
#endif

GASNETI_INLINE(_gex_RMA_GetBlocking)
int _gex_RMA_GetBlocking  (gex_TM_t _tm, void *_dest,
                    gex_Rank_t _rank, void *_src,
                    size_t _nbytes, gex_Flags_t _flags
                    GASNETI_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_NAMED(GASNETI_TRACE_GET_NAMED(GET_LOCAL,LOCAL,_tm,_dest,_rank,_src,_nbytes),_nbytes);
  gasneti_boundscheck(_tm, _rank, _src, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_GET_NAMED(GET_LOCAL,LOCAL,_tm,_dest,_rank,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackget_memsync();
    return 0;
  } else {
    GASNETI_TRACE_GET_NAMED(GET,NONLOCAL,_tm,_dest,_rank,_src,_nbytes);
    return gasnete_get(_tm, _dest, _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_GetBlocking(tm,dest,rank,src,nbytes,flags) \
       _gex_RMA_GetBlocking(tm,dest,rank,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_INLINE(_gex_RMA_PutBlocking)
int _gex_RMA_PutBlocking  (gex_TM_t _tm,
                    gex_Rank_t _rank, void *_dest,
                    /*const*/ void *_src,  // TODO-EX: un-comment const
                    size_t _nbytes, gex_Flags_t _flags
                    GASNETI_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_NAMED(GASNETI_TRACE_PUT_NAMED(PUT_LOCAL,LOCAL,_tm,_rank,_dest,_src,_nbytes),_nbytes);
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_NAMED(PUT_LOCAL,LOCAL,_tm,_rank,_dest,_src,_nbytes);
    GASNETI_MEMCPY(_dest, _src, _nbytes);
    gasnete_loopbackput_memsync();
    return 0;
  } else {
    GASNETI_TRACE_PUT_NAMED(PUT,NONLOCAL,_tm,_rank,_dest,_src,_nbytes);
    return gasnete_put(_tm, _rank, _dest, _src, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_PutBlocking(tm,rank,dest,src,nbytes,flags) \
       _gex_RMA_PutBlocking(tm,rank,dest,src,nbytes,flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Value Put
  =========
*/

#if GASNETI_DIRECT_PUT_VAL
  extern int gasnete_put_val(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG);
#elif !defined(gasnete_put_val)
  GASNETI_INLINE(gasnete_put_val)
  int gasnete_put_val( gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG)
  {
    gasneti_assume(_nbytes > 0); gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));
    gex_RMA_Value_t _src = _value;
    return gasnete_put(_tm, _rank, _dest, GASNETE_STARTOFBITS(&_src,_nbytes),
                       _nbytes, _flags GASNETI_THREAD_PASS);
  }
#endif

GASNETI_INLINE(_gex_RMA_PutVal)
int _gex_RMA_PutVal(  gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG)
{
  gasneti_assume(_nbytes > 0);
  gasneti_assert_uint(_nbytes ,<=, sizeof(gex_RMA_Value_t));
  gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_LOCAL(VAL,_tm,_rank,_dest,&_value,_nbytes);
    GASNETE_VALUE_ASSIGN(_dest, _value, _nbytes);
    gasnete_loopbackput_memsync();
    return 0;
  } else {
    GASNETI_TRACE_PUT(VAL,_tm,_rank,_dest,GASNETE_STARTOFBITS(&_value,_nbytes),_nbytes);
    return gasnete_put_val(_tm, _rank, _dest, _value, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_PutBlockingVal(tm,rank,dest,value,nbytes,flags) \
       _gex_RMA_PutVal(tm,rank,dest,value,nbytes,flags GASNETI_THREAD_GET)

#if GASNETI_DIRECT_PUT_NB_VAL && !defined(gasnete_put_nb_val)
  extern gex_Event_t gasnete_put_nb_val(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

GASNETI_INLINE(_gex_RMA_PutNBVal) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_RMA_PutNBVal (
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG)
{
  gasneti_assume(_nbytes > 0);
  gasneti_assert_uint(_nbytes ,<=, sizeof(gex_RMA_Value_t));
  gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_LOCAL(NB_VAL,_tm,_rank,_dest,&_value,_nbytes);
    GASNETE_VALUE_ASSIGN(_dest, _value, _nbytes);
    gasnete_loopbackput_memsync();
    return GEX_EVENT_INVALID;
  } else {
    GASNETI_TRACE_PUT(NB_VAL,_tm,_rank,_dest,GASNETE_STARTOFBITS(&_value,_nbytes),_nbytes);
    #if GASNETI_DIRECT_PUT_NB_VAL || defined(gasnete_put_nb_val)
      return gasnete_put_nb_val(_tm, _rank, _dest, _value, _nbytes, _flags GASNETI_THREAD_PASS);
    #else
      { gex_RMA_Value_t _src = _value;
        return gasnete_put_nb(_tm, _rank, _dest, GASNETE_STARTOFBITS(&_src,_nbytes),
                              _nbytes, GEX_EVENT_NOW, _flags GASNETI_THREAD_PASS);
      }
    #endif
  }
}
#define gex_RMA_PutNBVal(tm,rank,dest,value,nbytes,flags) \
       _gex_RMA_PutNBVal(tm,rank,dest,value,nbytes,flags GASNETI_THREAD_GET)

#if GASNETI_DIRECT_PUT_NBI_VAL
  extern int gasnete_put_nbi_val(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG);
#elif !defined(gasnete_put_nbi_val)
  GASNETI_INLINE(gasnete_put_nbi_val)
  int gasnete_put_nbi_val(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG)
  {
    gasneti_assume(_nbytes > 0); gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));
    gex_RMA_Value_t _src = _value;
    return gasnete_put_nbi(_tm, _rank, _dest, GASNETE_STARTOFBITS(&_src,_nbytes),
                           _nbytes, GEX_EVENT_NOW, _flags GASNETI_THREAD_PASS);
  }
#endif

GASNETI_INLINE(_gex_RMA_PutNBIVal)
int _gex_RMA_PutNBIVal(
                        gex_TM_t _tm,
                        gex_Rank_t _rank, void *_dest,
                        gex_RMA_Value_t _value,
                        size_t _nbytes, gex_Flags_t _flags
                        GASNETI_THREAD_FARG)
{
  gasneti_assume(_nbytes > 0);
  gasneti_assert_uint(_nbytes ,<=, sizeof(gex_RMA_Value_t));
  gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));
  gasneti_boundscheck(_tm, _rank, _dest, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_PUT_LOCAL(NBI_VAL,_tm,_rank,_dest,&_value,_nbytes);
    GASNETE_VALUE_ASSIGN(_dest, _value, _nbytes);
    gasnete_loopbackput_memsync();
    return 0;
  } else {
    GASNETI_TRACE_PUT(NBI_VAL,_tm,_rank,_dest,GASNETE_STARTOFBITS(&_value,_nbytes),_nbytes);
    return gasnete_put_nbi_val(_tm, _rank, _dest, _value, _nbytes, _flags GASNETI_THREAD_PASS);
  }
}
#define gex_RMA_PutNBIVal(tm,rank,dest,value,nbytes,flags) \
       _gex_RMA_PutNBIVal(tm,rank,dest,value,nbytes,flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Blocking Value Get
  ==================
*/

#if PLATFORM_COMPILER_SUN_C
  #pragma error_messages(off, E_END_OF_LOOP_CODE_NOT_REACHED)
#endif

#if !defined(gasnete_get_val) && GASNETI_DIRECT_GET_VAL
  extern gex_RMA_Value_t gasnete_get_val (
                  gex_TM_t _tm,
                  gex_Rank_t _rank, void *_src,
                  size_t _nbytes, gex_Flags_t _flags
                  GASNETI_THREAD_FARG);
#endif

GASNETI_INLINE(_gex_RMA_GetBlockingVal) GASNETI_WARN_UNUSED_RESULT
gex_RMA_Value_t _gex_RMA_GetBlockingVal (
                gex_TM_t _tm,
                gex_Rank_t _rank, void *_src,
                size_t _nbytes, gex_Flags_t _flags
                GASNETI_THREAD_FARG)
{
  gasneti_boundscheck(_tm, _rank, _src, _nbytes);
  if (gasnete_islocal(_tm,_rank)) {
    GASNETI_TRACE_GET_LOCAL(VAL,_tm,NULL,_rank,_src,_nbytes);
    GASNETE_VALUE_RETURN(_src, _nbytes);
  } else {
    GASNETI_TRACE_GET(VAL,_tm,NULL,_rank,_src,_nbytes);
    #if GASNETI_DIRECT_GET_VAL || defined(gasnete_get_val)
      return gasnete_get_val(_tm, _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
    #else
      { gex_RMA_Value_t _val = 0;
        gasnete_get(_tm, GASNETE_STARTOFBITS(&_val,_nbytes), _rank, _src, _nbytes, _flags GASNETI_THREAD_PASS);
        return _val;
      }
    #endif
  }
}
#define gex_RMA_GetBlockingVal(tm,rank,src,nbytes,flags) \
       _gex_RMA_GetBlockingVal(tm,rank,src,nbytes,flags GASNETI_THREAD_GET)

#if PLATFORM_COMPILER_SUN_C
  #pragma error_messages(default, E_END_OF_LOOP_CODE_NOT_REACHED)
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/

#if GASNETI_STATS_OR_TRACE
extern gasneti_tick_t gasnete_barrier_notifytime;
#endif

extern void gasnet_barrier_notify(int _id, int _flags);
extern int gasnet_barrier_wait(int _id, int _flags);
extern int gasnet_barrier_try(int _id, int _flags);
extern int gasnet_barrier(int _id, int _flags);
extern int gasnet_barrier_result(int *_id);

#ifndef gasnete_tm_barrier
  extern void
  gasnete_tm_barrier(gex_TM_t e_tm, gex_Flags_t flags GASNETI_THREAD_FARG);
#endif
GASNETI_INLINE(_gex_Coll_BarrierBlocking)
void _gex_Coll_BarrierBlocking(gex_TM_t _tm, gex_Flags_t _flags GASNETI_THREAD_FARG)
{
  GASNETI_TRACE_BARRIER1(_tm,_flags);
  gasnete_tm_barrier(_tm, _flags GASNETI_THREAD_PASS);
}
#define gex_Coll_BarrierBlocking(tm,flags) \
       _gex_Coll_BarrierBlocking(tm,flags GASNETI_THREAD_GET)

#ifndef gasnete_tm_barrier_nb
  extern gex_Event_t
  gasnete_tm_barrier_nb(gex_TM_t e_tm, gex_Flags_t flags GASNETI_THREAD_FARG);
#endif
GASNETI_INLINE(_gex_Coll_BarrierNB) GASNETI_WARN_UNUSED_RESULT
gex_Event_t _gex_Coll_BarrierNB(gex_TM_t _tm, gex_Flags_t _flags GASNETI_THREAD_FARG)
{
  GASNETI_TRACE_BARRIER2(_tm,_flags);
  return gasnete_tm_barrier_nb(_tm, _flags GASNETI_THREAD_PASS);
}
#define gex_Coll_BarrierNB(tm,flags) \
       _gex_Coll_BarrierNB(tm,flags GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Teams:
  ======
*/

extern size_t gasneti_TM_Split(gex_TM_t *_new_tm_p, gex_TM_t _parent_tm,
                               int _color, int _key,
                               void *_addr, size_t _len, gex_Flags_t _flags
                               GASNETI_THREAD_FARG);
#define gex_TM_Split(n,p,c,k,a,l,f) gasneti_TM_Split(n,p,c,k,a,l,f GASNETI_THREAD_GET)

// extern gex_Rank_t gex_TM_TranslateRankToJobrank(gex_TM_t tm, gex_Rank_t rank);
#define gex_TM_TranslateRankToJobrank(tm,rank) \
        gasneti_e_tm_rank_to_jobrank(tm,rank)

// extern gex_Rank_t gex_TM_TranslateJobrankToRank(gex_TM_t tm, gex_Rank_t jobrank);
#define gex_TM_TranslateJobrankToRank(tm,jobrank) \
        gasneti_e_tm_jobrank_to_rank(tm,jobrank)
/* ------------------------------------------------------------------------------------ */

// TODO-EX: remove these checks for conduits using legacy internal APIs
#if GASNETI_DIRECT_GET
  #error "out-of-date #define of GASNETI_DIRECT_GET"
#endif
#if GASNETI_DIRECT_GET_BULK
  #error "out-of-date #define of GASNETI_DIRECT_GET_BULK"
#elif defined(gasnete_get_bulk)
  #error "out-of-date #define of gasnete_get_bulk"
#endif
#if GASNETI_DIRECT_PUT
  #error "out-of-date #define of GASNETI_DIRECT_PUT"
#endif
#if GASNETI_DIRECT_PUT_BULK
  #error "out-of-date #define of GASNETI_DIRECT_PUT_BULK"
#elif defined(gasnete_put_bulk)
  #error "out-of-date #define of gasnete_put_bulk"
#endif
#if GASNETI_DIRECT_GET_NBI
  #error "out-of-date #define of GASNETI_DIRECT_GET_NBI"
#endif
#if GASNETI_DIRECT_GET_NB
  #error "out-of-date #define of GASNETI_DIRECT_GET_NB"
#endif
#if GASNETI_DIRECT_MEMSET
  #error "out-of-date #define of GASNETI_DIRECT_MEMSET"
#endif

#endif
