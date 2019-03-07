/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_extended_help_extra.h $
 * Description: GASNet Extended smp-specific Header
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_HELP_EXTRA_H
#define _GASNET_EXTENDED_HELP_EXTRA_H

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  =========================================================
 */

GASNETI_INLINE(gasnete_get_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t gasnete_get_nb(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  gasneti_unreachable();
  return GEX_EVENT_INVALID;
}
#define gasnete_get_nb gasnete_get_nb

GASNETI_INLINE(gasnete_put_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t gasnete_put_nb(
                     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
  gasneti_unreachable();
  return GEX_EVENT_INVALID;
}
#define gasnete_put_nb gasnete_put_nb

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
 */
   
GASNETI_INLINE(gasnete_get_nbi)
int gasnete_get_nbi (gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  gasneti_unreachable();
  return 0;
}
#define gasnete_get_nbi gasnete_get_nbi

GASNETI_INLINE(gasnete_put_nbi)
int gasnete_put_nbi (gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
  gasneti_unreachable();
  return 0;
}
#define gasnete_put_nbi gasnete_put_nbi


/* ------------------------------------------------------------------------------------ */
/*
  Value Put
  =========
*/

GASNETI_INLINE(gasnete_put_val)
int gasnete_put_val(
                gex_TM_t tm,
                gex_Rank_t rank, void *dest,
                gex_RMA_Value_t value,
                size_t nbytes, gex_Flags_t flags
                GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUTVAL(tm,rank,dest,value,nbytes);
  gasneti_unreachable();
  return 0;
}
#define gasnete_put_val gasnete_put_val

GASNETI_INLINE(gasnete_put_nb_val) GASNETI_WARN_UNUSED_RESULT
gex_Event_t gasnete_put_nb_val(
                gex_TM_t tm,
                gex_Rank_t rank, void *dest,
                gex_RMA_Value_t value,
                size_t nbytes, gex_Flags_t flags
                GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUTVAL(tm,rank,dest,value,nbytes);
  gasneti_unreachable();
  return GEX_EVENT_INVALID;
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
gex_RMA_Value_t gasnete_get_val(
                gex_TM_t tm,
                gex_Rank_t rank, void *src,
                size_t nbytes, gex_Flags_t flags
                GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GETVAL(tm,rank,src,nbytes);
  gasneti_unreachable();
  return 0;
}
#define gasnete_get_val gasnete_get_val

/*
  Blocking Get and Put
  ====================
*/

// Get trivially identical to nb plus gasneti_sync_reads())
GASNETI_INLINE(gasnete_get) GASNETI_WARN_UNUSED_RESULT
int gasnete_get(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gex_Event_t e = gasnete_get_nb(tm,dest,rank,src,nbytes,flags GASNETI_THREAD_PASS);
  gasneti_sync_reads();
  return 0;
}
#define gasnete_get gasnete_get

// Put identical to nbi (w/o any need to sync_reads) except for lack of lc_opt argument
#define gasnete_put(tm,rank,dest,src,nbytes,flags_and_TI) \
        gasnete_put_nbi(tm,rank,dest,src,nbytes,GEX_EVENT_NOW,flags_and_TI)

#endif
