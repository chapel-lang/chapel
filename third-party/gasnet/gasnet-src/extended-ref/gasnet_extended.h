/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended.h $
 * Description: GASNet Extended API Header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_H
#define _GASNET_EXTENDED_H

#include <string.h>

#include <gasnet_extended_help.h>
#include <gasnet_coll.h>

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
   all handlers will have an index in range 100-199 
   may be called before gasnete_init()
*/
extern gasnet_handlerentry_t const *gasnete_get_handlertable(void);

/* Initialize the Extended API:
   must be called by the core API at the end of gasnet_attach() before calls to extended API
     (this function may make calls to the core functions)
*/
extern void gasnete_init(void);

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* put_nb       source memory is safe to modify on return
   put_nb_bulk  source memory is NOT safe to modify on return
 */
#ifndef gasnete_put_nb_bulk
  extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

#ifndef gasnete_get_nb_bulk
  extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

#ifndef gasnete_memset_nb
  extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes   GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

#if GASNETI_DIRECT_GET_NB
  extern gasnet_handle_t gasnete_get_nb (void *dest, gasnet_node_t node, void *src,
                                         size_t nbytes GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#elif !defined(gasnete_get_nb)
  #define gasnete_get_nb gasnete_get_nb_bulk
#endif

GASNETI_INLINE(_gasnet_get_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_get_nb      (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NB,H);
  gasneti_boundscheck(node, src, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(NB,dest,node,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_GET(NB,dest,node,src,nbytes);
    return gasnete_get_nb(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get_nb(dest,node,src,nbytes) \
       _gasnet_get_nb(dest,node,src,nbytes GASNETE_THREAD_GET)

#if !defined(gasnete_put_nb)
  extern gasnet_handle_t gasnete_put_nb (gasnet_node_t node, void *dest, 
					 void *src, size_t nbytes 
					 GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif
GASNETI_INLINE(_gasnet_put_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_PUT(NB,H);
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NB,node,dest,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_PUT(NB,node,dest,src,nbytes);
    return gasnete_put_nb(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_nb(node,dest,src,nbytes) \
       _gasnet_put_nb(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_get_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NB_BULK,H);
  gasneti_boundscheck(node, src, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(NB_BULK,dest,node,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_GET(NB_BULK,dest,node,src,nbytes);
    return gasnete_get_nb_bulk(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get_nb_bulk(dest,node,src,nbytes) \
       _gasnet_get_nb_bulk(dest,node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_put_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_PUT(NB_BULK,H);
  gasneti_boundscheck(node, dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NB_BULK,node,dest,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_PUT(NB_BULK,node,dest,src,nbytes);
    return gasnete_put_nb_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_nb_bulk(node,dest,src,nbytes) \
       _gasnet_put_nb_bulk(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_memset_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t   _gasnet_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_MEMSET(NB,H);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_MEMSET_LOCAL(NB,node,dest,val,nbytes);
    memset(dest, val, nbytes);
    gasnete_loopbackput_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_MEMSET(NB,node,dest,val,nbytes);
    return gasnete_memset_nb(node, dest, val, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_memset_nb(node,dest,val,nbytes) \
       _gasnet_memset_nb(node,dest,val,nbytes GASNETE_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

#ifndef gasnete_try_syncnb
extern int gasnete_try_syncnb(gasnet_handle_t handle);
#endif
#ifndef gasnete_try_syncnb_some
extern int gasnete_try_syncnb_some(gasnet_handle_t *phandle, size_t numhandles);
#endif
#ifndef gasnete_try_syncnb_all
extern int gasnete_try_syncnb_all (gasnet_handle_t *phandle, size_t numhandles);
#endif

/* TODO: document the following "new" entry point: */
GASNETI_INLINE(gasnet_try_syncnb_nopoll) GASNETI_WARN_UNUSED_RESULT
int  gasnet_try_syncnb_nopoll(gasnet_handle_t handle) {
  int result = GASNET_OK;
  if_pt (handle != GASNET_INVALID_HANDLE)
    result = gasnete_try_syncnb(handle);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB_NP,result);
  return result;
}

GASNETI_INLINE(gasnet_try_syncnb) GASNETI_WARN_UNUSED_RESULT
int  gasnet_try_syncnb(gasnet_handle_t handle) {
  int result = GASNET_OK;
  if_pt (handle != GASNET_INVALID_HANDLE)  {
    gasneti_AMPoll();
    result = gasnete_try_syncnb(handle);
  }
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB,result);
  return result;
}

/* TODO: document the following "new" entry point: */
GASNETI_INLINE(gasnet_try_syncnb_some_nopoll)
int gasnet_try_syncnb_some_nopoll(gasnet_handle_t *phandle, size_t numhandles) {
  int result = gasnete_try_syncnb_some(phandle,numhandles);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB_SOME_NP,result);
  return result;
}

GASNETI_INLINE(gasnet_try_syncnb_some)
int gasnet_try_syncnb_some(gasnet_handle_t *phandle, size_t numhandles) {
  int result = (gasneti_AMPoll(), gasnete_try_syncnb_some(phandle,numhandles));
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB_SOME,result);
  return result;
}

/* TODO: document the following "new" entry point: */
GASNETI_INLINE(gasnet_try_syncnb_all_nopoll)
int gasnet_try_syncnb_all_nopoll(gasnet_handle_t *phandle, size_t numhandles) {
  int result = gasnete_try_syncnb_all(phandle,numhandles);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB_ALL_NP,result);
  return result;
}

GASNETI_INLINE(gasnet_try_syncnb_all)
int gasnet_try_syncnb_all(gasnet_handle_t *phandle, size_t numhandles) {
  int result = (gasneti_AMPoll(), gasnete_try_syncnb_all(phandle,numhandles));
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNB_ALL,result);
  return result;
}

#if GASNETI_DIRECT_WAIT_SYNCNB 
  extern void gasnete_wait_syncnb(gasnet_handle_t handle);
#elif !defined(gasnete_wait_syncnb)
  #define gasnete_wait_syncnb(handle) do {                                      \
      gasnet_handle_t _handle = (handle);                                       \
      if_pt (_handle != GASNET_INVALID_HANDLE) {                                \
        gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */        \
        gasneti_pollwhile(gasnete_try_syncnb(_handle) == GASNET_ERR_NOT_READY); \
      }                                                                         \
    } while(0)
#endif

GASNETI_INLINE(gasnet_wait_syncnb)
void gasnet_wait_syncnb(gasnet_handle_t handle) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait_syncnb(handle);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB);
}

#if GASNETI_DIRECT_WAIT_SYNCNB_SOME
  extern void gasnete_wait_syncnb_some(gasnet_handle_t *phandle, size_t numhandles);
#elif !defined(gasnete_wait_syncnb_some)
  #define gasnete_wait_syncnb_some(phandle, numhandles) do {                                   \
      gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */                         \
      gasneti_pollwhile(gasnete_try_syncnb_some(phandle, numhandles) == GASNET_ERR_NOT_READY); \
    } while(0)
#endif

GASNETI_INLINE(gasnet_wait_syncnb_some)
void gasnet_wait_syncnb_some(gasnet_handle_t *phandle, size_t numhandles) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait_syncnb_some(phandle, numhandles);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB_SOME);
}

#if GASNETI_DIRECT_WAIT_SYNCNB_ALL
  extern void gasnete_wait_syncnb_all(gasnet_handle_t *phandle, size_t numhandles);
#elif !defined(gasnete_wait_syncnb_all)
  #define gasnete_wait_syncnb_all(phandle, numhandles) do {                                   \
      gasneti_AMPoll(); /* Ensure at least one poll - TODO: remove? */                        \
      gasneti_pollwhile(gasnete_try_syncnb_all(phandle, numhandles) == GASNET_ERR_NOT_READY); \
    } while(0)
#endif

GASNETI_INLINE(gasnet_wait_syncnb_all)
void gasnet_wait_syncnb_all(gasnet_handle_t *phandle, size_t numhandles) {
  GASNETI_TRACE_WAITSYNC_BEGIN();
  gasnete_wait_syncnb_all(phandle, numhandles);
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB_ALL);
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
*/
/* put_nbi       source memory is safe to modify on return
   put_nbi_bulk  source memory is NOT safe to modify on return
 */

#ifndef gasnete_put_nbi
extern void gasnete_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif

#ifndef gasnete_put_nbi_bulk
extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif

#ifndef gasnete_get_nbi_bulk
extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif

#ifndef gasnete_memset_nbi
extern void gasnete_memset_nbi   (gasnet_node_t node, void *dest, int val,   size_t nbytes GASNETE_THREAD_FARG);
#endif

#if GASNETI_DIRECT_GET_NBI
  extern void gasnete_get_nbi (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_get_nbi)
  #define gasnete_get_nbi gasnete_get_nbi_bulk
#endif

GASNETI_INLINE(_gasnet_get_nbi)
void _gasnet_get_nbi      (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NBI,V);
  gasneti_boundscheck(node, src, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(NBI,dest,node,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
  } else {
    GASNETI_TRACE_GET(NBI,dest,node,src,nbytes);
    gasnete_get_nbi(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get_nbi(dest,node,src,nbytes) \
       _gasnet_get_nbi(dest,node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_put_nbi)
void _gasnet_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_PUT(NBI,V);
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NBI,node,dest,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT(NBI,node,dest,src,nbytes);
    gasnete_put_nbi(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_nbi(node,dest,src,nbytes) \
       _gasnet_put_nbi(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_get_nbi_bulk)
void _gasnet_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(NBI_BULK,V);
  gasneti_boundscheck(node, src, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(NBI_BULK,dest,node,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
  } else {
    GASNETI_TRACE_GET(NBI_BULK,dest,node,src,nbytes);
    gasnete_get_nbi_bulk(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get_nbi_bulk(dest,node,src,nbytes) \
       _gasnet_get_nbi_bulk(dest,node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_put_nbi_bulk)
void _gasnet_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_PUT(NBI_BULK,V);
  gasneti_boundscheck(node, dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NBI_BULK,node,dest,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT(NBI_BULK,node,dest,src,nbytes);
    gasnete_put_nbi_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_nbi_bulk(node,dest,src,nbytes) \
       _gasnet_put_nbi_bulk(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_memset_nbi)
void   _gasnet_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_MEMSET(NBI,V);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_MEMSET_LOCAL(NBI,node,dest,val,nbytes);
    memset(dest, val, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_MEMSET(NBI,node,dest,val,nbytes);
    gasnete_memset_nbi(node, dest, val, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_memset_nbi(node,dest,val,nbytes) \
       _gasnet_memset_nbi(node,dest,val,nbytes GASNETE_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

#ifndef gasnete_try_syncnbi_gets
  extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE);
#endif
#ifndef gasnete_try_syncnbi_puts
  extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE);
#endif

GASNETI_INLINE(_gasnet_try_syncnbi_gets) GASNETI_WARN_UNUSED_RESULT
int _gasnet_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  int retval;
  gasneti_AMPoll();
  retval = gasnete_try_syncnbi_gets(GASNETE_THREAD_PASS_ALONE);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNBI_GETS,retval);
  return retval;
}
#define gasnet_try_syncnbi_gets()   \
       _gasnet_try_syncnbi_gets(GASNETE_THREAD_GET_ALONE)

GASNETI_INLINE(_gasnet_try_syncnbi_puts) GASNETI_WARN_UNUSED_RESULT
int _gasnet_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
  int retval;
  gasneti_AMPoll();
  retval = gasnete_try_syncnbi_puts(GASNETE_THREAD_PASS_ALONE);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNBI_PUTS,retval);
  return retval;
}
#define gasnet_try_syncnbi_puts()   \
       _gasnet_try_syncnbi_puts(GASNETE_THREAD_GET_ALONE)

#if GASNETI_DIRECT_TRY_SYNCNBI_ALL
  extern int gasnete_try_syncnbi_all(GASNETE_THREAD_FARG_ALONE);
#elif !defined(gasnete_try_syncnbi_all)
  #define gasnete_try_syncnbi_all                                               \
   (gasnete_try_syncnbi_gets(GASNETE_THREAD_PASS_ALONE) == GASNET_OK ?          \
    gasnete_try_syncnbi_puts(GASNETE_THREAD_PASS_ALONE) : GASNET_ERR_NOT_READY) \
    GASNETE_THREAD_SWALLOW
#endif

GASNETI_INLINE(_gasnet_try_syncnbi_all) GASNETI_WARN_UNUSED_RESULT
int _gasnet_try_syncnbi_all(GASNETE_THREAD_FARG_ALONE) {
  int retval;
  gasneti_AMPoll();
  retval = gasnete_try_syncnbi_all(GASNETE_THREAD_PASS_ALONE);
  GASNETI_TRACE_TRYSYNC(TRY_SYNCNBI_ALL,retval);
  return retval;
}
#define gasnet_try_syncnbi_all()   \
       _gasnet_try_syncnbi_all(GASNETE_THREAD_GET_ALONE)

#if GASNETI_DIRECT_WAIT_SYNCNBI_GETS
  extern void gasnete_wait_syncnbi_gets(GASNETE_THREAD_FARG_ALONE);
#elif !defined(gasnete_wait_syncnbi_gets)
  #define gasnete_wait_syncnbi_gets \
    gasneti_pollwhile(gasnete_try_syncnbi_gets(GASNETE_THREAD_GET_ALONE) == GASNET_ERR_NOT_READY) \
    GASNETE_THREAD_SWALLOW
#endif

#define gasnet_wait_syncnbi_gets() do {                                                          \
  GASNETI_TRACE_WAITSYNC_BEGIN();                                                                \
  gasneti_AMPoll(); /* ensure at least one poll */                                                \
  gasnete_wait_syncnbi_gets(GASNETE_THREAD_GET_ALONE);                                           \
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNBI_GETS);                                                 \
  } while (0)

#if GASNETI_DIRECT_WAIT_SYNCNBI_PUTS
  extern void gasnete_wait_syncnbi_puts(GASNETE_THREAD_FARG_ALONE);
#elif !defined(gasnete_wait_syncnbi_puts)
  #define gasnete_wait_syncnbi_puts \
    gasneti_pollwhile(gasnete_try_syncnbi_puts(GASNETE_THREAD_GET_ALONE) == GASNET_ERR_NOT_READY) \
    GASNETE_THREAD_SWALLOW
#endif

#define gasnet_wait_syncnbi_puts() do {                                                          \
  GASNETI_TRACE_WAITSYNC_BEGIN();                                                                \
  gasneti_AMPoll(); /* ensure at least one poll */                                                \
  gasnete_wait_syncnbi_puts(GASNETE_THREAD_GET_ALONE);                                           \
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNBI_PUTS);                                                 \
  } while (0)

#if GASNETI_DIRECT_WAIT_SYNCNBI_ALL
  extern void gasnete_wait_syncnbi_all(GASNETE_THREAD_FARG_ALONE);
#elif !defined(gasnete_wait_syncnbi_all)
  #define gasnete_wait_syncnbi_all do {                                                     \
    gasneti_pollwhile(gasnete_try_syncnbi_gets(GASNETE_THREAD_GET_ALONE) == GASNET_ERR_NOT_READY); \
    gasneti_pollwhile(gasnete_try_syncnbi_puts(GASNETE_THREAD_GET_ALONE) == GASNET_ERR_NOT_READY); \
  } while (0) GASNETE_THREAD_SWALLOW
#endif

#define gasnet_wait_syncnbi_all() do {                                                           \
  GASNETI_TRACE_WAITSYNC_BEGIN();                                                                \
  gasneti_AMPoll(); /* ensure at least one poll */                                                \
  gasnete_wait_syncnbi_all(GASNETE_THREAD_GET_ALONE);                                            \
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNBI_ALL);                                                  \
  } while (0)
        
/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
#ifndef gasnete_begin_nbi_accessregion
extern void            gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG);
#endif
#ifndef gasnete_end_nbi_accessregion
extern gasnet_handle_t gasnete_end_nbi_accessregion(GASNETE_THREAD_FARG_ALONE) GASNETI_WARN_UNUSED_RESULT;
#endif

#define gasnet_begin_nbi_accessregion() gasnete_begin_nbi_accessregion(0 GASNETE_THREAD_GET)
#define gasnet_end_nbi_accessregion()   gasnete_end_nbi_accessregion(GASNETE_THREAD_GET_ALONE)

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/

#if GASNETI_DIRECT_GET
  extern void gasnete_get (void *dest, gasnet_node_t node, void *src,
                           size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_get)
  #define gasnete_get gasnete_get_bulk
#endif

#if GASNETI_DIRECT_GET_BULK
  extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
                                size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_get_bulk)
  #define gasnete_get_bulk(dest, node, src, nbytesTI) \
    gasnete_wait_syncnb(gasnete_get_nb_bulk(dest, node, src, nbytesTI))
#endif

#if GASNETI_DIRECT_PUT
  extern void gasnete_put (gasnet_node_t node, void* dest, void *src,
                           size_t nbytes GASNETE_THREAD_FARG);
  #define gasnete_putTI gasnete_put
#elif !defined(gasnete_put)
  /* only valid because this is blocking put */
  #define gasnete_put   gasnete_put_bulk
  #if GASNETI_THREADINFO_OPT
    #define gasnete_putTI(node,dest,src,nbytes,ti) gasnete_put_bulk(node,dest,src,nbytes GASNETE_THREAD_PASS)
  #else
    #define gasnete_putTI gasnete_put_bulk
  #endif
#endif

#if GASNETI_DIRECT_PUT_BULK
  extern void gasnete_put_bulk (gasnet_node_t node, void* dest, void *src,
                                size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_put_bulk)
  #define gasnete_put_bulk(node, dest, src, nbytesTI) \
    gasnete_wait_syncnb(gasnete_put_nb_bulk(node, dest, src, nbytesTI))
#endif

#if GASNETI_DIRECT_MEMSET
  extern void gasnete_memset(gasnet_node_t node, void *dest, int val,
			     size_t nbytes GASNETE_THREAD_FARG);
#else
  #define gasnete_memset(node, dest, val, nbytesTI) \
    gasnete_wait_syncnb(gasnete_memset_nb(node, dest, val, nbytesTI))
#endif

GASNETI_INLINE(_gasnet_get)
void _gasnet_get (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_NAMED(GASNETI_TRACE_GET_NAMED(GET_LOCAL,LOCAL,dest,node,src,nbytes),V);
  gasneti_boundscheck(node, src, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_NAMED(GET_LOCAL,LOCAL,dest,node,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
  } else {
    GASNETI_TRACE_GET_NAMED(GET,NONLOCAL,dest,node,src,nbytes);
    gasnete_get(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get(dest,node,src,nbytes) \
       _gasnet_get(dest,node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_get_bulk)
void _gasnet_get_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_GET(BULK,V);
  gasneti_boundscheck(node, src, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(BULK,dest,node,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackget_memsync();
  } else {
    GASNETI_TRACE_GET(BULK,dest,node,src,nbytes);
    gasnete_get_bulk(dest, node, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_get_bulk(dest,node,src,nbytes) \
       _gasnet_get_bulk(dest,node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_put)
void _gasnet_put (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_NAMED(GASNETI_TRACE_PUT_NAMED(PUT_LOCAL,LOCAL,node,dest,src,nbytes),V);
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(src, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_NAMED(PUT_LOCAL,LOCAL,node,dest,src,nbytes);
    GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT_NAMED(PUT,NONLOCAL,node,dest,src,nbytes);
    gasnete_put(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put(node,dest,src,nbytes) \
       _gasnet_put(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_put_bulk)
void _gasnet_put_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_PUT(BULK,V);
  gasneti_boundscheck(node, dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(BULK,node,dest,src,nbytes);
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT(BULK,node,dest,src,nbytes);
    gasnete_put_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_bulk(node,dest,src,nbytes) \
       _gasnet_put_bulk(node,dest,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_memset)
void  _gasnet_memset (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKZEROSZ_NAMED(GASNETI_TRACE_MEMSET_NAMED(MEMSET_LOCAL,LOCAL,node,dest,val,nbytes),V);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_MEMSET_NAMED(MEMSET_LOCAL,LOCAL,node,dest,val,nbytes);
    memset(dest, val, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_MEMSET_NAMED(MEMSET,NONLOCAL,node,dest,val,nbytes);
    gasnete_memset(node, dest, val, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_memset(node,dest,val,nbytes) \
       _gasnet_memset(node,dest,val,nbytes GASNETE_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Value Put
  =========
*/

#if GASNETI_DIRECT_PUT_VAL
  extern void gasnete_put_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_put_val)
  #define gasnete_put_val(node, dest, value, nbytesTI) do {                    \
    gasnet_register_value_t src = value;                                       \
    gasnete_putTI(node, dest, GASNETE_TISTARTOFBITS(&src,nbytesTI), nbytesTI); \
  } while (0)
#endif

GASNETI_INLINE(_gasnet_put_val)
void _gasnet_put_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG) {
  gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t));
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(VAL,node,dest,&value,nbytes);
    GASNETE_VALUE_ASSIGN(dest, value, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT(VAL,node,dest,GASNETE_STARTOFBITS(&value,nbytes),nbytes);
    gasnete_put_val(node, dest, value, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_val(node,dest,value,nbytes) \
       _gasnet_put_val(node,dest,value,nbytes GASNETE_THREAD_GET)

#if GASNETI_DIRECT_PUT_NB_VAL && !defined(gasnete_put_nb_val)
  extern gasnet_handle_t gasnete_put_nb_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif

GASNETI_INLINE(_gasnet_put_nb_val) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_put_nb_val (gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG) {
  gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t));
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NB_VAL,node,dest,&value,nbytes);
    GASNETE_VALUE_ASSIGN(dest, value, nbytes);
    gasnete_loopbackput_memsync();
    return GASNET_INVALID_HANDLE;
  } else {
    GASNETI_TRACE_PUT(NB_VAL,node,dest,GASNETE_STARTOFBITS(&value,nbytes),nbytes);
    #if GASNETI_DIRECT_PUT_NB_VAL || defined(gasnete_put_nb_val)
      return gasnete_put_nb_val(node, dest, value, nbytes GASNETE_THREAD_PASS);
    #else
      { gasnet_register_value_t src = value;
        return gasnete_put_nb(node, dest, GASNETE_STARTOFBITS(&src,nbytes), nbytes GASNETE_THREAD_PASS);
      }
    #endif
  }
}
#define gasnet_put_nb_val(node,dest,value,nbytes) \
       _gasnet_put_nb_val(node,dest,value,nbytes GASNETE_THREAD_GET)

#if GASNETI_DIRECT_PUT_NBI_VAL
  extern void gasnete_put_nbi_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG);
#elif !defined(gasnete_put_nbi_val)
  #define gasnete_put_nbi_val(node, dest, value, nbytesTI) do {                  \
    gasnet_register_value_t src = value;                                         \
    gasnete_put_nbi(node, dest, GASNETE_TISTARTOFBITS(&src,nbytesTI), nbytesTI); \
  } while (0)
#endif

GASNETI_INLINE(_gasnet_put_nbi_val)
void _gasnet_put_nbi_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETE_THREAD_FARG) {
  gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t));
  gasneti_boundscheck(node, dest, nbytes);
  gasnete_aligncheck(dest, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_PUT_LOCAL(NBI_VAL,node,dest,&value,nbytes);
    GASNETE_VALUE_ASSIGN(dest, value, nbytes);
    gasnete_loopbackput_memsync();
  } else {
    GASNETI_TRACE_PUT(NBI_VAL,node,dest,GASNETE_STARTOFBITS(&value,nbytes),nbytes);
    gasnete_put_nbi_val(node, dest, value, nbytes GASNETE_THREAD_PASS);
  }
}
#define gasnet_put_nbi_val(node,dest,value,nbytes) \
       _gasnet_put_nbi_val(node,dest,value,nbytes GASNETE_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
/*
  Non-Blocking Value Get (explicit-handle)
  ========================================
*/

#ifndef _GASNET_VALGET_HANDLE_T
struct _gasnete_valget_op_t;
typedef struct _gasnete_valget_op_t *gasnet_valget_handle_t;
#endif

extern gasnet_valget_handle_t gasnete_get_nb_val(gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#ifndef gasnete_wait_syncnb_valget
extern gasnet_register_value_t gasnete_wait_syncnb_valget(gasnet_valget_handle_t handle);
#endif

GASNETI_INLINE(_gasnet_get_nb_val) GASNETI_WARN_UNUSED_RESULT
gasnet_valget_handle_t _gasnet_get_nb_val (gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  if (gasnete_islocal(node)) GASNETI_TRACE_GET_LOCAL(NB_VAL,NULL,node,src,nbytes);
  else GASNETI_TRACE_GET(NB_VAL,NULL,node,src,nbytes);     
  return gasnete_get_nb_val(node,src,nbytes GASNETE_THREAD_PASS); 
}
#define gasnet_get_nb_val(node,src,nbytes) \
       _gasnet_get_nb_val(node,src,nbytes GASNETE_THREAD_GET)

GASNETI_INLINE(gasnet_wait_syncnb_valget) GASNETI_WARN_UNUSED_RESULT
gasnet_register_value_t gasnet_wait_syncnb_valget (gasnet_valget_handle_t handle) {
  gasnet_register_value_t val;
  GASNETI_TRACE_WAITSYNC_BEGIN();
  val = gasnete_wait_syncnb_valget(handle); 
  GASNETI_TRACE_WAITSYNC_END(WAIT_SYNCNB_VALGET);   
  return val;
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking Value Get
  ==================
*/

#if PLATFORM_COMPILER_SUN_C
  #pragma error_messages(off, E_END_OF_LOOP_CODE_NOT_REACHED)
#endif

#if !defined(gasnete_get_val) && GASNETI_DIRECT_GET_VAL
  extern gasnet_register_value_t gasnete_get_val (gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif

GASNETI_INLINE(_gasnet_get_val) GASNETI_WARN_UNUSED_RESULT
gasnet_register_value_t _gasnet_get_val (gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasneti_boundscheck(node, src, nbytes);
  gasnete_aligncheck(src, nbytes);
  if (gasnete_islocal(node)) {
    GASNETI_TRACE_GET_LOCAL(VAL,NULL,node,src,nbytes);
    GASNETE_VALUE_RETURN(src, nbytes);
  } else {
    GASNETI_TRACE_GET(VAL,NULL,node,src,nbytes);
    #if GASNETI_DIRECT_GET_VAL || defined(gasnete_get_val)
      return gasnete_get_val(node, src, nbytes GASNETE_THREAD_PASS);
    #else
      { gasnet_register_value_t val = 0;
        gasnete_get(GASNETE_STARTOFBITS(&val,nbytes), node, src, nbytes GASNETE_THREAD_PASS);
        return val;
      }
    #endif
  }
}
#define gasnet_get_val(node,src,nbytes) \
       _gasnet_get_val(node,src,nbytes GASNETE_THREAD_GET)

#if PLATFORM_COMPILER_SUN_C
  #pragma error_messages(default, E_END_OF_LOOP_CODE_NOT_REACHED)
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/


#ifndef GASNET_TEAM_ALL
extern gasnet_team_handle_t gasnete_coll_team_all;
#define GASNET_TEAM_ALL gasnete_coll_team_all
#endif

/*intialize the barriers for a given team*/
extern void gasnete_coll_barrier_init(gasnete_coll_team_t team, int barrier_type,
                                      gasnet_node_t *nodes, gasnet_node_t *supernodes);

/*initialize the barriers for GASNET_TEAM_ALL*/
extern void gasnete_barrier_init(void);


#if GASNETI_STATS_OR_TRACE
extern gasneti_tick_t gasnete_barrier_notifytime;
#endif

extern void gasnet_barrier_notify(int id, int flags);
extern int gasnet_barrier_wait(int id, int flags);
extern int gasnet_barrier_try(int id, int flags);
extern int gasnet_barrier(int id, int flags);
extern int gasnet_barrier_result(int *id);
/* ------------------------------------------------------------------------------------ */

#endif
