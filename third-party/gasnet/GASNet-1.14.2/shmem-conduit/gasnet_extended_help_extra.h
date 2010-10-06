/*   $Source: /var/local/cvs/gasnet/shmem-conduit/gasnet_extended_help_extra.h,v $
 *     $Date: 2009/04/27 10:43:12 $
 * $Revision: 1.10 $
 * Description: GASNet Extended Shmem-specific Header 
 * Copyright 2005, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
#include <mpp/shmem.h>
#else
#include <shmem.h>
#endif

#include <string.h> /* memcpy */

#ifndef _GASNET_EXTENDED_HELP_EXTRA_H
#define _GASNET_EXTENDED_HELP_EXTRA_H

/*
 * Defining GASNETE_NBISYNC_ALWAYS_QUIET causes a quiet to be generated at
 * every nbi sync operation instead of only NBIs that contain puts.  This
 * allows puts to be be completed as 1-store instead of 2-stores.
 *
 * This is more important on X1 since we really want the nbi loop to
 * generate a vector load/store.
 */
#if PLATFORM_ARCH_CRAYX1
#define GASNETE_NBISYNC_ALWAYS_QUIET	1
#else
#define GASNETE_NBISYNC_ALWAYS_QUIET	0
#endif

#define GASNETE_OK  0 /* Always 0, same as GASNET_OK */

#if defined(CRAY_SHMEM)
  #include <intrinsics.h>
  #include <strings.h>
  #define GASNETE_PRAGMA_IVDEP	_Pragma("_CRI ivdep")

  extern uintptr_t gasnete_pe_bits_shift;
  extern uintptr_t gasnete_addr_bits_mask;

  /* Translation on X1 -- works in every GASNet configuration, but we can
   * ignore the translation on clients that already understand global pointers
   */
  #define GASNETE_TRANSLATE_X1(addr,pe)				    \
    (void *) ( (((uintptr_t) (addr)) & gasnete_addr_bits_mask) |    \
               (((uintptr_t) (pe)) << gasnete_pe_bits_shift))

  #define GASNETE_TRANSLATE_PTR GASNETE_TRANSLATE_X1

#elif defined(SGI_SHMEM)
  #define GASNETE_PRAGMA_IVDEP	  /* no ivdep is useful here */
  #define GASNETE_TRANSLATE_PTR(addr,pe) shmem_ptr(addr,pe)

#endif

/* The translation is free when using segment-large or segment-fast.  For
 * segment-everything, the translation is only free if the client can manage
 * global addresses itself (and asserts that it can).
 */
#if defined(GASNETE_GLOBAL_ADDRESS_CLIENT) || \
    defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  #define GASNETE_SHMPTR(addr,pe) (addr)
#else
  #define GASNETE_SHMPTR(addr,pe) GASNETE_TRANSLATE_PTR(addr,pe)
#endif
#define GASNETE_SHMPTR_AM(addr,pe) GASNETE_TRANSLATE_PTR(addr,pe)


#if PLATFORM_ARCH_CRAYX1
#define _GASNETE_CRAYX1_ONLY(x)  x
#define _GASNETE_BCOPY bcopy
#else
#define _GASNETE_CRAYX1_ONLY(x)
#define _GASNETE_BCOPY(s,d,n) memcpy((d),(s),(n))
#endif

#define _GASNETE_INLINE_VLOOP(dest,src,nbytes)      \
	do {					    \
	    unsigned long i, sz;		    \
	    sz = ((unsigned long)(nbytes))>>3;	    \
	    for (i=0; i<sz; i++) {		    \
		dest[i] = src[i];		    \
	    }					    \
	} while (0)

#define _GASNETE_DESTSRC_ALIGNED(dest,src,al)			    \
	    (!((((uintptr_t)dest)|((uintptr_t)src))&(al)))
	    
#define _gasnete_global_ldst(dest,src,nbytes)			    \
	do {							    \
	    if (!(nbytes&0x7) &&				    \
			_GASNETE_DESTSRC_ALIGNED(dest,src,0x7)) {   \
	      uint64_t *pDest = (uint64_t *)dest;		    \
	      uint64_t *pSrc = (uint64_t *)src;			    \
	      switch(nbytes) {					    \
		case 80:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];	pDest[5] = pSrc[5];	    \
		    pDest[6] = pSrc[6];	pDest[7] = pSrc[7];	    \
		    pDest[8] = pSrc[8];	pDest[9] = pSrc[9];	    \
		    break;					    \
		case 72:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];	pDest[5] = pSrc[5];	    \
		    pDest[6] = pSrc[6];	pDest[7] = pSrc[7];	    \
		    pDest[8] = pSrc[8];				    \
		    break;					    \
		case 64:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];	pDest[5] = pSrc[5];	    \
		    pDest[6] = pSrc[6];	pDest[7] = pSrc[7];	    \
		    break;					    \
		case 56:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];	pDest[5] = pSrc[5];	    \
		    pDest[6] = pSrc[6];				    \
		    break;					    \
		case 48:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];	pDest[5] = pSrc[5];	    \
		    break;					    \
		case 40:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    pDest[4] = pSrc[4];				    \
		    break;					    \
		case 32:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];	pDest[3] = pSrc[3];	    \
		    break;					    \
		case 24:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    pDest[2] = pSrc[2];				    \
		    break;					    \
		case 16:					    \
		    pDest[0] = pSrc[0];	pDest[1] = pSrc[1];	    \
		    break;					    \
		case 8:						    \
		    pDest[0] = pSrc[0];				    \
		    break;					    \
		default:					    \
		    _GASNETE_CRAYX1_ONLY(			    \
		    if (nbytes <= 256)				    \
			_GASNETE_INLINE_VLOOP(pDest,pSrc,nbytes);   \
		    else					    \
		    )						    \
			_GASNETE_BCOPY(src, dest, nbytes);	    \
		    break;					    \
	      }                                                     \
	    } else {						    \
	      switch(nbytes) {					    \
		case 4:						    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x3))	    \
			*((uint32_t *)dest) = *((uint32_t *)src);   \
		    else					    \
			memcpy(dest,src,nbytes);		    \
		    break;					    \
		case 2:						    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x1))	    \
			*((uint16_t *)dest) = *((uint16_t *)src);   \
		    else					    \
			memcpy(dest,src,nbytes);		    \
		    break;					    \
		case 1:						    \
		    *((uint8_t *)dest) = *((uint8_t *)src);	    \
		    break;					    \
		default:					    \
		    memcpy(dest, src, nbytes);			    \
		    break;					    \
	      }							    \
	    }							    \
	} while (0)

#if PLATFORM_ARCH_CRAYX1
  #define _gasnete_x1_global_ldst_bulk(dest,src,nbytes)		    \
	do {							    \
	    uint64_t *pDest = (uint64_t *)dest;			    \
	    uint64_t *pSrc = (uint64_t *)src;			    \
	    switch(nbytes) {					    \
		case 8:						    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x7))	    \
			*((uint64_t *)dest) = *((uint64_t *)src);   \
		    else					    \
			memcpy(dest,src,nbytes);		    \
		    break;					    \
		case 4:						    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x3))	    \
			*((uint32_t *)dest) = *((uint32_t *)src);   \
		    else					    \
			memcpy(dest,src,nbytes);		    \
		    break;					    \
		case 2:						    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x1))	    \
			*((uint16_t *)dest) = *((uint16_t *)src);   \
		    else					    \
			memcpy(dest,src,nbytes);		    \
		    break;					    \
		case 1:						    \
		    *((uint8_t *)dest) = *((uint8_t *)src);	    \
		    break;					    \
		default:					    \
		    if (_GASNETE_DESTSRC_ALIGNED(dest,src,0x7) &&   \
			nbytes <= 256 && !(nbytes&0x7))	            \
			_GASNETE_INLINE_VLOOP(pDest,pSrc,nbytes);   \
		    else					    \
			_GASNETE_BCOPY(src, dest, nbytes);	    \
		    break;					    \
	    }							    \
	} while (0)

  #define _gasnete_global_ldst_bulk _gasnete_x1_global_ldst_bulk
#else
  #define _gasnete_global_ldst_bulk _gasnete_global_ldst
#endif

#define gasnete_global_get(dest,src,nbytes,pe)			    \
	    _gasnete_global_ldst(dest,GASNETE_SHMPTR(src,pe),nbytes)
#define gasnete_global_get_bulk(dest,src,nbytes,pe)		    \
	    _gasnete_global_ldst_bulk(dest,GASNETE_SHMPTR(src,pe),nbytes)
#define gasnete_global_put(dest,src,nbytes,pe)			    \
	    _gasnete_global_ldst(GASNETE_SHMPTR(dest,pe),src,nbytes)
#define gasnete_global_put_bulk(dest,src,nbytes,pe)		    \
	    _gasnete_global_ldst_bulk(GASNETE_SHMPTR(dest,pe),src,nbytes)

/* 
 * Blocking 
 */
#ifdef GASNETE_GLOBAL_ADDRESS
  #define gasnete_put(pe,dest,src,nbytes)		    \
	  do { gasnete_global_put(dest,src,nbytes,pe); shmem_quiet(); } while (0)
  #define gasnete_put_bulk(pe,dest,src,nbytes)		    \
	  do { gasnete_global_put_bulk(dest,src,nbytes,pe); shmem_quiet(); } while (0)
  #define gasnete_get(dest,pe,src,nbytes)      gasnete_global_get(dest,src,nbytes,pe)
  #define gasnete_get_bulk(dest,pe,src,nbytes) gasnete_global_get_bulk(dest,src,nbytes,pe)
#else
  #define gasnete_put(pe,dest,src,nbytes)      shmem_putmem(dest,src,nbytes,pe)
  #define gasnete_put_bulk(pe,dest,src,nbytes) shmem_putmem(dest,src,nbytes,pe)
  #define gasnete_get(dest,pe,src,nbytes)      shmem_getmem(dest,src,nbytes,pe)
  #define gasnete_get_bulk(dest,pe,src,nbytes) shmem_getmem(dest,src,nbytes,pe)
#endif

#define gasnete_putTI gasnete_put

/*
 * NBI
 *
 * NBI Synchronization can be enabled in two different modes, based on macro
 * define GASNETE_NBISYNC_ALWAYS_QUIET.
 *
 * 1. Always quiet (nbi sync will always shmem_quiet).
 *    * Dis: Potential overkill if only gets were issued
 *    * Adv: Puts can be completed as one store, instead of two (one store for
 *           the actual data and another to save the current nbi state).
 * 2. Quiet on puts only
 *    Inverse of above.
 *
 */
extern int *	    gasnete_nbisync_cur;

#define GASNETE_SYNC_NONE   ((int *)-1)
#define GASNETE_SYNC_QUIET  ((int *)-2)

#if GASNETE_NBISYNC_ALWAYS_QUIET
  #define GASNETE_NBISYNC_HAS_PUT
#else
  #define GASNETE_NBISYNC_HAS_PUT (gasnete_nbisync_cur = GASNETE_SYNC_QUIET)
#endif

#ifdef GASNETE_GLOBAL_ADDRESS
  #define gasnete_put_nbi(pe,dest,src,nbytes)			\
	    do { gasnete_global_put(dest,src,nbytes,pe); GASNETE_NBISYNC_HAS_PUT; } while (0)
  #define gasnete_put_nbi_bulk(pe,dest,src,nbytes)		\
	    do { gasnete_global_put_bulk(dest,src,nbytes,pe); GASNETE_NBISYNC_HAS_PUT; } while (0)

  #define gasnete_get_nbi(dest,pe,src,nbytes)	   gasnete_global_get(dest,src,nbytes,pe)
  #define gasnete_get_nbi_bulk(dest,pe,src,nbytes) gasnete_global_get_bulk(dest,src,nbytes,pe)
#else
  #define gasnete_put_nbi(pe,dest,src,nbytes)			\
	    do { shmem_putmem(dest,src,nbytes,pe); GASNETE_NBISYNC_HAS_PUT; } while (0)
  #define gasnete_put_nbi_bulk					\
	    do { shmem_putmem(dest,src,nbytes,pe); GASNETE_NBISYNC_HAS_PUT; } while (0)

  #define gasnete_get_nbi(dest,pe,src,nbytes)	   shmem_getmem(dest,src,nbytes,pe)
  #define gasnete_get_nbi_bulk(dest,pe,src,nbytes) shmem_getmem(dest,src,nbytes,pe)
#endif

/*
 * NBI Synchronization.
 *
 * (Region-based synchronization begin/end remains a function call in
 * gasnet_extended.c).
 */


/* Gets are always 'blocking' in the gasnet sense since gets move from memory
 * to registers, so there is no added explicit synchronization required.
 */
#define gasnete_try_syncnbi_gets(t) GASNETE_OK

GASNETI_INLINE(_gasnete_try_syncnbi_puts)
int
_gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) 
{
  #if GASNETE_NBISYNC_ALWAYS_QUIET
    gasnete_nbisync_cur = GASNETE_SYNC_NONE;
    shmem_quiet();
  #else
    if (gasnete_nbisync_cur == GASNETE_SYNC_QUIET) {
	gasnete_nbisync_cur = GASNETE_SYNC_NONE;
	shmem_quiet();
    }
  #endif

    return GASNETE_OK;
}
#define gasnete_try_syncnbi_puts(t) _gasnete_try_syncnbi_puts(t)

/*
 * NB (Puts, Memsets and Gets)
 *
 * NB Synchronization has only two states.
 * 1. SYNC_QUIET: Returned for non-blocking puts, that require quiets
 * 2. SYNC_NONE: No sync required, for gets.
 */

GASNETI_INLINE(_gasnete_put_nb_bulk)
gasnet_handle_t 
_gasnete_put_nb_bulk(gasnet_node_t node, void *dest, void *src, size_t nbytes) 
{
  #ifdef GASNETE_GLOBAL_ADDRESS
    gasnete_global_put_bulk(dest,src,nbytes,node);
  #else
    shmem_putmem(dest,src,nbytes,node);
  #endif
    return GASNETE_SYNC_QUIET;
}
#define gasnete_put_nb_bulk(pe,dest,src,nbytes) _gasnete_put_nb_bulk(pe,dest,src,nbytes)

GASNETI_INLINE(_gasnete_get_nb_bulk)
gasnet_handle_t 
_gasnete_get_nb_bulk(void *dest, gasnet_node_t node, void *src, size_t nbytes)
{
  #ifdef GASNETE_GLOBAL_ADDRESS
    gasnete_global_get_bulk(dest,src,nbytes,node);
  #else
    shmem_getmem(dest,src,nbytes,node);
  #endif
    return GASNETE_SYNC_NONE;
}
#define gasnete_get_nb_bulk(dest,pe,src,nbytes) _gasnete_get_nb_bulk(dest,pe,src,nbytes)

/*
 * Memsets on global addresses.
 *
 * Non-blocking memsets are overkill on shmem-conduit.  In their AM form they
 * would require synchronization and 'nb' synchronization to AMPoll.  Since
 * GASNet requires syncnb_puts to also cover handles for memsets, this
 * would invariably complicate the put-nb type operations.  Therefore, the
 * decision here is to implement non-blocking memsets as blocking operations.
 *
 * AM-based non-blocking memset
 *    * Spin until the reply for the memset is received
 *
 * Global Address non-blocking memset
 *    * Run blocking memset
 *
 * All memset_nb return GASNET_SYNC_NONE (no sync required).
 */
#ifdef GASNETE_GLOBAL_ADDRESS
  GASNETI_INLINE(_gasnete_memset_nb)
  gasnet_handle_t 
  _gasnete_memset_nb(gasnet_node_t node, void *dest, int val, size_t nbytes)
  {
    memset(GASNETE_SHMPTR(dest,node), val, nbytes);
    return GASNETE_SYNC_NONE;
  }
  #define gasnete_memset_nb _gasnete_memset_nb
  #define gasnete_memset_nbi(node,dest,val,nbytes)	\
			    (void)_gasnete_memset_nb(node,dest,val,nbytes)

#else
  #error Shmem no global address support not implemented
  extern gasnet_handle_t
         gasnete_am_memset_nb(gasnet_node_t node, void *dest, int val, size_t nbytes);

  #define gasnete_memset_nb gasnete_am_memset_nb
  #define gasnete_memset_nbi(node,dest,src,val,nbytes)	\
			    (void)gasnete_am_memset_nb(node,dest,src,val,nbytes)
#endif


/*
 * Non-bulk are the same as bulk, except on X1
 */

#ifdef GASNETE_GLOBAL_ADDRESS
  GASNETI_INLINE(_gasnete_get_nb)
  gasnet_handle_t 
  _gasnete_get_nb(void *dest, gasnet_node_t node, void *src, size_t nbytes)
  {
    gasnete_global_get(dest,src,nbytes,node);
    return GASNETE_SYNC_NONE;
  }

  GASNETI_INLINE(_gasnete_put_nb)
  gasnet_handle_t 
  _gasnete_put_nb(gasnet_node_t node, void *dest, void *src, size_t nbytes)
  {
    gasnete_global_put(dest,src,nbytes,node);
    return GASNETE_SYNC_QUIET;
  }
  #define gasnete_get_nb	_gasnete_get_nb
  #define gasnete_put_nb	_gasnete_put_nb
#else
  #define gasnete_get_nb	gasnete_get_nb_bulk
  #define gasnete_put_nb	gasnete_put_nb_bulk
#endif

/*
 * NB Synchronization.
 *
 */

GASNETI_INLINE(_gasnete_try_syncnb)
int
_gasnete_try_syncnb(gasnet_handle_t handle)
{
    if (handle == GASNETE_SYNC_NONE || handle == GASNET_INVALID_HANDLE)
	return GASNETE_OK;

    gasneti_assert(handle == GASNETE_SYNC_QUIET);
    shmem_quiet();

    return GASNETE_OK;
}
#define gasnete_try_syncnb(handle)	_gasnete_try_syncnb(handle)

GASNETI_INLINE(_gasnete_try_syncnb_some)
int _gasnete_try_syncnb_some(gasnet_handle_t *phandle, size_t numhandles) {
    int success = 0;
    int empty = 1;
    int doquiet = 0;
    int	i;

    gasneti_assert(phandle != NULL);
    for (i = 0; i < numhandles; i++) {
      gasnet_handle_t handle = phandle[i];
      if (handle != GASNET_INVALID_HANDLE) {
	doquiet |= (handle == GASNETE_SYNC_QUIET);
	phandle[i] = GASNET_INVALID_HANDLE;
        empty = 0;
	success = 1;
      }
    }

    /* Encountered at least one put operation, that requires a quiet. */
    if (doquiet)
	shmem_quiet();

    if (success || empty) return GASNETE_OK;
    else return GASNET_ERR_NOT_READY;
}

#define gasnete_try_syncnb_some(h,num)	 _gasnete_try_syncnb_some(h,num)
#define gasnete_try_syncnb_all(h,num)	 _gasnete_try_syncnb_some(h,num)

/* 
 * Value gets and puts are more tricky
 *
 * They can't map directly to shmem functions as the gasnet interface too
 * general to map to the type-specific elemental shmem_g and shmem_p variants.
 *
 */
#define GASNETI_DIRECT_PUT_VAL     1
#define GASNETI_DIRECT_PUT_NB_VAL  1
#define GASNETI_DIRECT_PUT_NBI_VAL 1

/*
  Non-Blocking Value Get (explicit-handle)
  ========================================
*/

#if SIZEOF_LONG == 8
#define GASNET_SHMEM_GET_8  shmem_long_g
#define GASNET_SHMEM_PUT_8  shmem_long_p
#elif SIZEOF_DOUBLE == 8
#define GASNET_SHMEM_GET_8  shmem_double_g
#define GASNET_SHMEM_PUT_8  shmem_double_p
#endif

#if SIZEOF_LONG == 4
#define GASNET_SHMEM_GET_4  shmem_long_g
#define GASNET_SHMEM_PUT_4  shmem_long_p
#elif SIZEOF_INT == 4
#define GASNET_SHMEM_GET_4  shmem_int_g
#define GASNET_SHMEM_PUT_4  shmem_int_p
#elif SIZEOF_SHORT == 4
#define GASNET_SHMEM_GET_4  shmem_short_g
#define GASNET_SHMEM_PUT_4  shmem_short_p
#elif SIZEOF_FLOAT == 4
#define GASNET_SHMEM_GET_4  shmem_float_g
#define GASNET_SHMEM_PUT_4  shmem_float_p
#endif

#if SIZEOF_SHORT == 2
#define GASNET_SHMEM_GET_2  shmem_short_g
#define GASNET_SHMEM_PUT_2  shmem_short_p
#endif

#ifdef GASNETE_GLOBAL_ADDRESS
GASNETI_INLINE(_gasnete_get_nb_val)
gasnet_valget_handle_t 
_gasnete_get_nb_val(gasnet_node_t node, void *src, 
		   size_t nbytes) 
{
    switch (nbytes) {
	case 8:
	    return (gasnet_valget_handle_t) 
		    *((uint64_t *) GASNETE_SHMPTR(src,node));
	case 4:
	    return (gasnet_valget_handle_t) 
		    *((uint32_t *) GASNETE_SHMPTR(src,node));
	case 2:
	    return (gasnet_valget_handle_t) 
		    *((uint16_t *) GASNETE_SHMPTR(src,node));
	case 1:
	    return (gasnet_valget_handle_t) 
		    *((uint8_t *) GASNETE_SHMPTR(src,node));
	default:
	    /* This kills the vectorizer on X1 */
	    #ifdef GASNET_DEBUG
	    gasneti_fatalerror(
		"VIOLATION: Unsupported size %d in valget", 
		(int)nbytes);
	    #endif
	    return (gasnet_valget_handle_t) 0;
	    break;
    }
    return (gasnet_valget_handle_t) 0;
}
#define gasnete_get_nb_val _gasnete_get_nb_val

#else /* !GASNETE_GLOBAL_ADDRESS */

GASNETI_INLINE(gasnete_get_nb_val)
gasnet_valget_handle_t 
gasnete_get_nb_val(gasnet_node_t node, void *src, 
		   size_t nbytes) 
{
    switch (nbytes) {
	case 8:	
	    #ifdef GASNET_SHMEM_GET_8
		return (gasnet_valget_handle_t) GASNET_SHMEM_GET_8(src, node);
	    #else
	    {
		static uint64_t	temp64;
		shmem_getmem((void *) &temp64,src,8,node);
		return (gasnet_valget_handle_t) temp64;
	    }
	    #endif

	case 4: 
	    #ifdef GASNET_SHMEM_GET_4
		return (gasnet_valget_handle_t) GASNET_SHMEM_GET_4(src, node);
	    #else
	    {
		static uint32_t	temp32;
		shmem_getmem((void *) &temp32,src,4,node);
		return (gasnet_valget_handle_t) temp32;
	    }
	    #endif

	case 2: 
	    #ifdef GASNET_SHMEM_GET_2
		return (gasnet_valget_handle_t) GASNET_SHMEM_GET_2(src, node);
	    #else
	    {
		static uint16_t temp16;
		shmem_getmem((void *) &temp16,src,2,node);
		return (gasnet_valget_handle_t) temp16;
	    }
	    #endif
	case 1:
	    {
		uint8_t	val;
		val = *((uint8_t *) shmem_ptr(src,node));
		return (gasnet_valget_handle_t) val;
	    }
#if 0
		shmem_getmem((void *) &temp8,src,1,node);
		return (gasnet_valget_handle_t) temp8;
#endif

	case 0: return 0;
	default:
	    {
		static uint64_t	tempA;
		#if 0 && defined(GASNET_DEBUG)
		if (nbytes > sizeof(gasnet_register_value_t))
		      gasneti_fatalerror(
			"VIOLATION: Unsupported size %d in valget", nbytes);
		#endif
		shmem_getmem((void *) &tempA, src, nbytes, node);
		return (gasnet_valget_handle_t) tempA;
	    }
    }
}

#endif

#define gasnete_get_val	(gasnet_register_value_t) gasnete_get_nb_val

/* 
 * Since shmem only has blocking valgets, we use the value as the handle and
 * the resulting value.
 */
#define gasnete_wait_syncnb_valget(handle) ((gasnet_register_value_t)handle)

/* ------------------------------------------------------------------------------------ */
/*
  Non-Blocking and Blocking Value Put 
  ====================================
*/
#ifdef GASNETE_GLOBAL_ADDRESS
GASNETI_INLINE(gasnete_put_val_inner)
void 
gasnete_put_val_inner(gasnet_node_t node, void *dest, 
		      gasnet_register_value_t value, 
		      size_t nbytes)
{
    switch (nbytes) {
	case 8:
	    *((uint64_t *)GASNETE_SHMPTR(dest,node)) = (uint64_t)value;
	    return;
	case 4:
	    *((uint32_t *)GASNETE_SHMPTR(dest,node)) = (uint32_t)value;
	    return;
	case 2:
	    *((uint16_t *)GASNETE_SHMPTR(dest,node)) = (uint16_t)value;
	    return;
	case 1:
	    *((uint8_t *)GASNETE_SHMPTR(dest,node)) = (uint8_t)value;
	    return;
	default:
	    #if 0
	    gasneti_fatalerror(
		"VIOLATION: Unsupported size %d in valput", 
		nbytes);
	    #endif
	    break;
    }
    return;
}
#else
GASNETI_INLINE(gasnete_put_val_inner)
void 
gasnete_put_val_inner(gasnet_node_t node, void *dest, 
		      gasnet_register_value_t value, 
		      size_t nbytes)
{
    static char	val_put[8];

    switch (nbytes) {
    #ifdef GASNET_SHMEM_PUT_8
	case 8:	GASNET_SHMEM_PUT_8(dest, value, node); return;
    #endif
    #ifdef GASNET_SHMEM_PUT_4
	case 4: GASNET_SHMEM_PUT_4(dest, value, node); return;
    #endif
    #ifdef GASNET_SHMEM_PUT_2
	case 2: GASNET_SHMEM_PUT_2(dest, value, node); return;
    #endif
	case 0: return;
	default:
	    #if 0 && defined(GASNET_DEBUG)
	      if (nbytes > sizeof(gasnet_register_value_t))
		      gasneti_fatalerror(
			"VIOLATION: Unsupported size %d in valput", nbytes);
	    #endif
	    memcpy(val_put, &value, nbytes);
	    shmem_putmem(dest, val_put, nbytes, node);
	    return;
    }
}
#endif

GASNETI_INLINE(_gasnete_put_val)
void 
_gasnete_put_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, 
		size_t nbytes)
{
    gasnete_put_val_inner(node, dest, value, nbytes);
    /* No choice here -- we need remote completion, so big quiet hammer */
    shmem_quiet();
}
#define gasnete_put_val _gasnete_put_val

GASNETI_INLINE(_gasnete_put_nb_val)
gasnet_handle_t 
_gasnete_put_nb_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, 
		    size_t nbytes)
{
    gasnete_put_val_inner(node, dest, value, nbytes);
    return GASNETE_SYNC_QUIET;
}
#define gasnete_put_nb_val _gasnete_put_nb_val 

GASNETI_INLINE(_gasnete_put_nbi_val)
void 
_gasnete_put_nbi_val(gasnet_node_t node, void *dest, 
		    gasnet_register_value_t value, 
		    size_t nbytes)
{
    gasnete_put_val_inner(node, dest, value, nbytes);
    gasnete_nbisync_cur = GASNETE_SYNC_QUIET;
    return;
}
#define gasnete_put_nbi_val _gasnete_put_nbi_val 

#endif

