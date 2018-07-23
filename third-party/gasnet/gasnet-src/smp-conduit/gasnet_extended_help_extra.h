/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_extended_help_extra.h $
 * Description: GASNet Extended smp-specific Header
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_HELP_EXTRA_H
#define _GASNET_EXTENDED_HELP_EXTRA_H

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  =========================================================
 */

GASNETI_INLINE(gasnete_get_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_get_nb_bulk(void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(UNALIGNED,H,dest,node,src,nbytes);
  gasneti_unreachable();
  return GASNET_INVALID_HANDLE;
}
#define gasnete_get_nb_bulk gasnete_get_nb_bulk

GASNETI_INLINE(gasnete_put_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_put_nb(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(ALIGNED,H,node,dest,src,nbytes);
  gasneti_unreachable();
  return GASNET_INVALID_HANDLE;
}
#define gasnete_put_nb gasnete_put_nb

GASNETI_INLINE(gasnete_put_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_put_nb_bulk(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H,node,dest,src,nbytes);
  gasneti_unreachable();
  return GASNET_INVALID_HANDLE;
}
#define gasnete_put_nb_bulk gasnete_put_nb_bulk

GASNETI_INLINE(gasnete_memset_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_memset_nb(gasnet_node_t node, void *dest, int val, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_MEMSET(H,node,dest,val,nbytes);
  gasneti_unreachable();
  return GASNET_INVALID_HANDLE;
}
#define gasnete_memset_nb gasnete_memset_nb

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

GASNETI_INLINE(gasnete_syncnb_one)
int gasnete_syncnb_one(gasnet_handle_t handle)
{
  gasneti_assert(handle == GASNET_INVALID_HANDLE);
  gasneti_sync_reads();
  return GASNET_OK;
}
#define gasnete_try_syncnb  gasnete_syncnb_one
#define gasnete_wait_syncnb gasnete_syncnb_one

GASNETI_INLINE(gasnete_syncnb_array)
int gasnete_syncnb_array(gasnet_handle_t *phandle, size_t numhandles)
{
#if GASNET_DEBUG
  for (size_t i=0; i<numhandles; ++i)
    gasneti_assert(phandle[i] == GASNET_INVALID_HANDLE);
#endif
  gasneti_sync_reads();
  return GASNET_OK;
}
#define gasnete_try_syncnb_some  gasnete_syncnb_array
#define gasnete_try_syncnb_all   gasnete_syncnb_array
#define gasnete_wait_syncnb_some gasnete_syncnb_array
#define gasnete_wait_syncnb_all  gasnete_syncnb_array

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
 */
   
GASNETI_INLINE(gasnete_get_nbi_bulk)
void gasnete_get_nbi_bulk(void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(UNALIGNED,V,dest,node,src,nbytes);
  gasneti_unreachable();
}
#define gasnete_get_nbi_bulk gasnete_get_nbi_bulk

GASNETI_INLINE(gasnete_put_nbi)
void gasnete_put_nbi(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(ALIGNED,V,node,dest,src,nbytes);
  gasneti_unreachable();
}
#define gasnete_put_nbi gasnete_put_nbi

GASNETI_INLINE(gasnete_put_nbi_bulk)
void gasnete_put_nbi_bulk(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V,node,dest,src,nbytes);
  gasneti_unreachable();
}
#define gasnete_put_nbi_bulk gasnete_put_nbi_bulk

GASNETI_INLINE(gasnete_memset_nbi)
void gasnete_memset_nbi(gasnet_node_t node, void *dest, int val, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_MEMSET(V,node,dest,val,nbytes);
  gasneti_unreachable();
}
#define gasnete_memset_nbi gasnete_memset_nbi

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/
GASNETI_INLINE(gasnete_syncnbi)
int gasnete_syncnbi(GASNETI_THREAD_FARG_ALONE)
{
  gasneti_sync_reads();
  return GASNET_OK;
}
#define gasnete_try_syncnbi_all   gasnete_syncnbi
#define gasnete_try_syncnbi_gets  gasnete_syncnbi
#define gasnete_try_syncnbi_puts  gasnete_syncnbi
#define gasnete_wait_syncnbi_all  gasnete_syncnbi
#define gasnete_wait_syncnbi_gets gasnete_syncnbi
#define gasnete_wait_syncnbi_puts gasnete_syncnbi

GASNETI_INLINE(gasnete_begin_nbi_accessregion)
void gasnete_begin_nbi_accessregion(int allowrecursion GASNETI_THREAD_FARG)
{ /* empty */ }
#define gasnete_begin_nbi_accessregion gasnete_begin_nbi_accessregion

GASNETI_INLINE(gasnete_end_nbi_accessregion) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_end_nbi_accessregion(GASNETI_THREAD_FARG_ALONE)
{ return GASNET_INVALID_HANDLE; }
#define gasnete_end_nbi_accessregion gasnete_end_nbi_accessregion

/* ------------------------------------------------------------------------------------ */
/*
  Value Put
  =========
*/

GASNETI_INLINE(gasnete_put_val)
void gasnete_put_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUTVAL(V,node,dest,value,nbytes);
  gasneti_unreachable();
}
#define gasnete_put_val gasnete_put_val

GASNETI_INLINE(gasnete_put_nb_val) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t gasnete_put_nb_val(gasnet_node_t node, void *dest, gasnet_register_value_t value, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUTVAL(H,node,dest,value,nbytes);
  gasneti_unreachable();
  return GASNET_INVALID_HANDLE;
}
#define gasnete_put_nb_val gasnete_put_nb_val

/* nbi is trivially identical to blocking */
#define gasnete_put_nbi_val gasnete_put_val

/* ------------------------------------------------------------------------------------ */
/*
  Blocking Value Get
  ==================
*/

GASNETI_INLINE(gasnete_get_val)
gasnet_register_value_t gasnete_get_val(gasnet_node_t node, void *src, size_t nbytes GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GETVAL(node,src,nbytes);
  gasneti_unreachable();
  return 0;
}
#define gasnete_get_val gasnete_get_val

/*
  Blocking Get and Put
  ====================
*/

// Get trivially identical to synced nb (need gasneti_sync_reads())
#define gasnete_get(dest,rank,src,nbytes_and_TI) \
        gasnete_syncnb_one(gasnete_get_nb_bulk(dest,rank,src,nbytes_and_TI))

// Put trivially identical to nbi (w/o any need to sync_reads)
#define gasnete_put(rank,dest,src,nbytes_and_TI) \
        gasnete_put_nbi_bulk(rank,dest,src,nbytes_and_TI)

#endif
