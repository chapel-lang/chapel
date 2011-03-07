/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_vis_strided.c,v $
 *     $Date: 2009/05/12 02:16:38 $
 * $Revision: 1.27 $
 * Description: GASNet Strided implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_extended_refvis.c
#endif

/*---------------------------------------------------------------------------------*/
/* ***  Strided *** */
/*---------------------------------------------------------------------------------*/
/* helper macros */
/* increment the values in init[] by incval chunks, 
   using provided count[], contiglevel and limit
   when contiglevel=i, chunks are assumed to have size count[i]*count[i-1]*...*count[0]
*/
#define GASNETE_STRIDED_VECTOR_INC(init, incval, count, contiglevel, limit) do { \
    size_t const _contiglevel = (contiglevel);                                   \
    size_t const _dimlim = (limit) - _contiglevel;                               \
    size_t const * const _count = (count);                                       \
    size_t * const _init = (init);                                               \
    size_t _dim;                                                                 \
    _init[0] += (incval);                                                        \
    for ( _dim = 0; _dim < _dimlim; _dim++) {                                    \
      size_t const _thismax = _count[_dim+_contiglevel+1];                       \
      if (_init[_dim] < _thismax) break;                                         \
      else {                                                                     \
        size_t const _carries = _init[_dim] / _thismax;                          \
        gasneti_assert(_dim != _dimlim-1); /* indicates an overflow */           \
        _init[_dim] -= _carries * _thismax;                                      \
        _init[_dim+1] += _carries;                                               \
      }                                                                          \
    }                                                                            \
  } while(0)

/* The GASNETE_STRIDED_HELPER(limit,contiglevel) macro expands to code based on 
     the template below (shown for the case of limit - contiglevel == 3, 
     eg 4-D area contiguous only in the smallest dimension).
   If (limit - contiglevel) > GASNETE_LOOPING_DIMS, then we use the generalized 
     striding code shown in the default case of GASNETE_STRIDED_HELPER.
   Parameters: 
     * if caller scope contains the declaration: 
         GASNETE_STRIDED_HELPER_DECLARE_NODST;
       then in all cases the dst pointer is not calculated, and pdst is always NULL
     * if caller scope contains the declaration: 
         GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(numchunks, init, addr_already_offset, update_addr_init);
       then the traversal will iterate over a total of numchunks contiguous chunks, 
       beginning at chunk coordinate indicated by init[0...(limit - contiglevel - 1)]
       if addr_already_offset is nonzero, the code assumes srcaddr/dstaddr already reference the first chunk,
       otherwise, the srcaddr/dstaddr values are offset based on init to reach the first chunk
       iff update_addr_init is nonzero, then srcaddr/dstaddr/init are updated on exit to point to the next unused chunk
       
    uint8_t * psrc = srcaddr;
    uint8_t * pdst = dstaddr;
    size_t _chunkcnt = numchunks;

    if (HAVE_PARTIAL && !srcdst_already_offset) {
      size_t _dim;
      for (_dim = contiglevel; _dim < limit; _dim++) {
        psrc += srcstrides[_dim] * init[_dim-contiglevel];
        pdst += dststrides[_dim] * init[_dim-contiglevel];
      }
    }

    size_t const _count0 = count[contiglevel+1];
    size_t _i0 = (HAVE_PARTIAL ? _count0 - init[0] : 0);
    size_t const _srcbump0 = srcstrides[contiglevel];
    size_t const _dstbump0 = dststrides[contiglevel];

    size_t const _count1 = count[contiglevel+1+1];
    size_t _i1 = (HAVE_PARTIAL ? _count1 - init[1] : 0);
    size_t const _srcbump1 = srcstrides[contiglevel+1] - _count0*srcstrides[contiglevel+1-1];
    size_t const _dstbump1 = dststrides[contiglevel+1] - _count0*dststrides[contiglevel+1-1];

    size_t const _count2 = count[contiglevel+2+1];
    size_t _i2 = (HAVE_PARTIAL ? _count2 - init[2] : 0);
    size_t const _srcbump2 = srcstrides[contiglevel+2] - _count1*srcstrides[contiglevel+2-1];
    size_t const _dstbump2 = dststrides[contiglevel+2] - _count1*dststrides[contiglevel+2-1];

    if (HAVE_PARTIAL) goto body;

    for (_i2 = _count2; _i2; _i2--) { 

    for (_i1 = _count1; _i1; _i1--) {

    for (_i0 = _count0; _i0; _i0--) {
      body:
      GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst);
      psrc += _srcbump0;
      pdst += _dstbump0;
      if_pf (HAVE_PARTIAL && --_chunkcnt == 0) goto done;
    }

      psrc += _srcbump1;
      pdst += _dstbump1;
    }

      psrc += _srcbump2;
      pdst += _dstbump2;
    }
    done: ;
    if (HAVE_PARTIAL && update_addr_init) {
      if (!_i0) ; // loop nest terminated 
      else if (!--_i0) { _i0 = _count0; 

        psrc += _srcbump1; 
        pdst += _dstbump1; 
        if (!--_i1) { _i1 = _count1; 

          psrc += _srcbump2; 
          pdst += _dstbump2; 
          if (!--_i2) { _i2 = _count2; 

          }
        }
      }
      srcaddr = psrc;
      dstaddr = pdst;

      init[0] = _count0 - _i0;
      init[1] = _count1 - _i1;
      init[2] = _count2 - _i2;
    }
*/

#define GASNETE_STRIDED_HELPER_DECLARE_NODST \
       size_t dststrides[1];                 \
       void * dstaddr = NULL;                \
       static int8_t _gasnete_strided_helper_nodst = (int8_t)sizeof(_gasnete_strided_helper_nodst)

static int32_t _gasnete_strided_helper_nodst = (int32_t)sizeof(_gasnete_strided_helper_nodst);
#define GASNETE_STRIDED_HELPER_HAVEDST (sizeof(_gasnete_strided_helper_nodst) == 4)

#define GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(numchunks, init, addr_already_offset, update_addr_init) \
       size_t * const _gasnete_strided_init = (init);                                                  \
       size_t _gasnete_strided_chunkcnt = (numchunks);                                                 \
       int const _gasnete_strided_addr_already_offset = (addr_already_offset);                         \
       int const _gasnete_strided_update_addr_init = (update_addr_init);                               \
       static int8_t _gasnete_strided_helper_havepartial = (int8_t)sizeof(_gasnete_strided_helper_havepartial)

static int32_t * const _gasnete_strided_init = 
   (sizeof(_gasnete_strided_init)?NULL:(void*)&_gasnete_strided_init); /* NULL:NULL triggers gcc -O1 bug on sysx */
static int32_t _gasnete_strided_chunkcnt = (int32_t)sizeof(_gasnete_strided_chunkcnt);
static int32_t const _gasnete_strided_addr_already_offset = (int32_t)sizeof(_gasnete_strided_addr_already_offset);
static int32_t const _gasnete_strided_update_addr_init = (int32_t)sizeof(_gasnete_strided_update_addr_init);
static int32_t const _gasnete_strided_helper_havepartial = (int32_t)sizeof(_gasnete_strided_helper_havepartial);
#define GASNETE_STRIDED_HELPER_HAVEPARTIAL (sizeof(_gasnete_strided_helper_havepartial) == 1)

#define _GASNETE_STRIDED_LABELHLP2(idx,name,line) _GASNETE_STRIDED_LABEL_##name##_##idx##_##line
#define _GASNETE_STRIDED_LABELHLP(idx,name,line)  _GASNETE_STRIDED_LABELHLP2(idx,name,line)
#define _GASNETE_STRIDED_LABEL(idx,name) _GASNETE_STRIDED_LABELHLP(idx,name,__LINE__)

#define GASNETE_STRIDED_HELPER_SETUP_BASE()                                                  \
    size_t const _count0 = count[contiglevel+1];                                             \
    size_t const _srcbump0 = srcstrides[contiglevel];                                        \
    size_t const _dstbump0 = (GASNETE_STRIDED_HELPER_HAVEDST ? dststrides[contiglevel] : 0); \
    size_t _i0 = (GASNETE_STRIDED_HELPER_HAVEPARTIAL ? _count0 - _gasnete_strided_init[0] : 0); 

#define GASNETE_STRIDED_HELPER_SETUP_INT(curr, lower)                                                               \
    size_t const _count##curr = count[contiglevel+curr+1];                                                          \
    size_t const _srcbump##curr = srcstrides[contiglevel+curr] - _count##lower*srcstrides[contiglevel+curr-1];      \
    size_t const _dstbump##curr = (GASNETE_STRIDED_HELPER_HAVEDST ?                                                 \
                                  dststrides[contiglevel+curr] - _count##lower*dststrides[contiglevel+curr-1] : 0); \
    size_t _i##curr = (GASNETE_STRIDED_HELPER_HAVEPARTIAL ? _count##curr - _gasnete_strided_init[curr] : 0);          


#define GASNETE_STRIDED_HELPER_LOOPHEAD_BASE()
#define GASNETE_STRIDED_HELPER_LOOPHEAD_INT(curr,junk) \
    for (_i##curr = _count##curr; _i##curr; _i##curr--) { 

#define GASNETE_STRIDED_HELPER_LOOPTAIL_BASE()
#define GASNETE_STRIDED_HELPER_LOOPTAIL_INT(curr,junk)            \
      psrc += _srcbump##curr;                                     \
      if (GASNETE_STRIDED_HELPER_HAVEDST) pdst += _dstbump##curr; \
      else gasneti_assert(pdst == NULL);                          \
    }

#define GASNETE_STRIDED_HELPER_CLEANUPHEAD_BASE()
#define GASNETE_STRIDED_HELPER_CLEANUPHEAD_INT(curr,junk)         \
      psrc += _srcbump##curr;                                     \
      if (GASNETE_STRIDED_HELPER_HAVEDST) pdst += _dstbump##curr; \
      if (!--_i##curr) { _i##curr = _count##curr;

#define GASNETE_STRIDED_HELPER_CLEANUPTAIL_BASE()
#define GASNETE_STRIDED_HELPER_CLEANUPTAIL_INT(curr,junk) }

#define GASNETE_STRIDED_HELPER_CLEANUPINIT_BASE()
#define GASNETE_STRIDED_HELPER_CLEANUPINIT_INT(curr,junk) \
  _gasnete_strided_init[curr] = _count##curr - _i##curr;

#define GASNETE_STRIDED_HELPER_CASE_BASE() 
#define GASNETE_STRIDED_HELPER_CASE_INT(junk,curr) case curr+1: {               \
    GASNETE_METAMACRO_ASC##curr(GASNETE_STRIDED_HELPER_SETUP)                   \
    if (GASNETE_STRIDED_HELPER_HAVEPARTIAL)                                     \
      goto _GASNETE_STRIDED_LABEL(curr,BODY);                                   \
    GASNETE_METAMACRO_DESC##curr(GASNETE_STRIDED_HELPER_LOOPHEAD)               \
    for (_i0 = _count0; _i0; _i0--) {                                           \
      _GASNETE_STRIDED_LABEL(curr,BODY): ;                                      \
      GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst);                               \
      psrc += _srcbump0;                                                        \
      if (GASNETE_STRIDED_HELPER_HAVEDST) pdst += _dstbump0;                    \
      if_pf (GASNETE_STRIDED_HELPER_HAVEPARTIAL &&                              \
          --_gasnete_strided_chunkcnt == 0)                                     \
        goto _GASNETE_STRIDED_LABEL(curr,DONE);                                 \
    }                                                                           \
    GASNETE_METAMACRO_ASC##curr(GASNETE_STRIDED_HELPER_LOOPTAIL)                \
    _GASNETE_STRIDED_LABEL(curr,DONE): ;                                        \
    if (GASNETE_STRIDED_HELPER_HAVEPARTIAL &&                                   \
        _gasnete_strided_update_addr_init) {                                    \
      if (!_i0) ; /* loop nest terminated */                                    \
      else if (!--_i0) { _i0 = _count0;                                         \
        GASNETE_METAMACRO_ASC##curr(GASNETE_STRIDED_HELPER_CLEANUPHEAD)         \
        GASNETE_METAMACRO_ASC##curr(GASNETE_STRIDED_HELPER_CLEANUPTAIL)         \
      }                                                                         \
      srcaddr = psrc;                                                           \
      if (GASNETE_STRIDED_HELPER_HAVEDST) dstaddr = pdst;                       \
      else gasneti_assert(pdst == NULL);                                        \
      _gasnete_strided_init[0] = _count0 - _i0;                                 \
      GASNETE_METAMACRO_ASC##curr(GASNETE_STRIDED_HELPER_CLEANUPINIT)           \
    }                                                                           \
  } break;

#if GASNET_DEBUG
  /* assert the generalized looping code is functioning properly */
  #define GASNETE_CHECK_PTR(ploc, addr, strides, idx, dim) do { \
      int i;                                                    \
      uint8_t *ptest = (addr);                                  \
      for (i=0; i < dim; i++) {                                 \
        ptest += (idx)[i]*(strides)[i-1];                       \
      }                                                         \
      gasneti_assert(ptest == ploc);                            \
    } while (0)
#else
  #define GASNETE_CHECK_PTR(ploc, addr, strides, idx, dim) 
#endif

#if GASNETE_LOOPING_DIMS > GASNETE_METAMACRO_DEPTH_MAX
#error GASNETE_LOOPING_DIMS must be <= GASNETE_METAMACRO_DEPTH_MAX
#endif

#define GASNETE_STRIDED_HELPER(limit,contiglevel) do {                 \
  /* general setup code */                                             \
  uint8_t *psrc = srcaddr;                                             \
  uint8_t *pdst = dstaddr;                                             \
  gasneti_assert((limit) > (contiglevel));                             \
  if (GASNETE_STRIDED_HELPER_HAVEPARTIAL &&                            \
      !_gasnete_strided_addr_already_offset) {                         \
    size_t _dim;                                                       \
    for (_dim = contiglevel; _dim < limit; _dim++) {                   \
      psrc += srcstrides[_dim] *                                       \
              _gasnete_strided_init[_dim-contiglevel];                 \
      if (GASNETE_STRIDED_HELPER_HAVEDST)                              \
        pdst += dststrides[_dim] *                                     \
                _gasnete_strided_init[_dim-contiglevel];               \
    }                                                                  \
  }                                                                    \
  switch ((limit) - (contiglevel)) {                                   \
    _CONCAT(GASNETE_METAMACRO_ASC,                                     \
            GASNETE_LOOPING_DIMS)(GASNETE_STRIDED_HELPER_CASE)         \
    default: { /* arbitrary dimensions > GASNETE_LOOPING_DIMS */       \
      size_t const dim = (limit) - (contiglevel);                      \
      size_t const * const _count = count + contiglevel + 1;           \
      size_t const * const _srcstrides = srcstrides + contiglevel + 1; \
      size_t const * const _dststrides =                               \
      (GASNETE_STRIDED_HELPER_HAVEDST?dststrides + contiglevel + 1:0); \
      ssize_t curdim = 0; /* must be signed */                         \
      /* Psrc,dst}ptr_start save the address of the first element */   \
      /* in the current row at each dimension */                       \
      uint8_t *_srcptr_start[GASNETE_DIRECT_DIMS];                     \
      uint8_t ** const srcptr_start = (dim <= GASNETE_DIRECT_DIMS ?    \
         _srcptr_start : gasneti_malloc(sizeof(uint8_t *)*dim));       \
      uint8_t *_dstptr_start[GASNETE_DIRECT_DIMS];                     \
      uint8_t ** const dstptr_start = ((dim <= GASNETE_DIRECT_DIMS ||  \
                                    !GASNETE_STRIDED_HELPER_HAVEDST) ? \
         _dstptr_start : gasneti_malloc(sizeof(uint8_t *)*dim));       \
      size_t _idx[GASNETE_DIRECT_DIMS];                                \
      size_t * const idx = (dim <= GASNETE_DIRECT_DIMS ?               \
         _idx : gasneti_malloc(sizeof(size_t)*dim));                   \
      uint8_t *psrc_base = psrc; /* hold true base of strided area */  \
      uint8_t *pdst_base = pdst;                                       \
      if (GASNETE_STRIDED_HELPER_HAVEPARTIAL) {                        \
        for (curdim = 0; curdim < dim; curdim++) {                     \
          size_t thisval = _gasnete_strided_init[curdim];              \
          gasneti_assert(thisval < _count[curdim]);                    \
          idx[curdim] = thisval;                                       \
          psrc_base -= thisval*_srcstrides[curdim-1];                  \
          srcptr_start[curdim] = psrc_base;                            \
          if (GASNETE_STRIDED_HELPER_HAVEDST) {                        \
            pdst_base -= thisval*_dststrides[curdim-1];                \
            dstptr_start[curdim] = pdst_base;                          \
          }                                                            \
        }                                                              \
      } else {                                                         \
        for (curdim = 0; curdim < dim; curdim++) {                     \
          idx[curdim] = 0;                                             \
          srcptr_start[curdim] = psrc;                                 \
          if (GASNETE_STRIDED_HELPER_HAVEDST)                          \
            dstptr_start[curdim] = pdst;                               \
        }                                                              \
      }                                                                \
      while (1) {                                                      \
        GASNETE_CHECK_PTR(psrc, psrc_base, _srcstrides, idx, dim);     \
        if (GASNETE_STRIDED_HELPER_HAVEDST)                            \
          GASNETE_CHECK_PTR(pdst, pdst_base, _dststrides, idx, dim);   \
        else gasneti_assert(pdst == NULL);                             \
        GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst);                    \
        for (curdim = 0; curdim < dim; curdim++) {                     \
          if (idx[curdim] < _count[curdim]-1) {                        \
            idx[curdim]++; /* advance to next row in this dim */       \
            psrc += _srcstrides[curdim-1];                             \
            if (GASNETE_STRIDED_HELPER_HAVEDST)                        \
              pdst += _dststrides[curdim-1];                           \
            break;                                                     \
          } else { /* row complete at this dim, prop to higher dim */  \
            idx[curdim] = 0;                                           \
            psrc = srcptr_start[curdim];                               \
            if (GASNETE_STRIDED_HELPER_HAVEDST)                        \
              pdst = dstptr_start[curdim];                             \
          }                                                            \
        }                                                              \
        if_pf ((GASNETE_STRIDED_HELPER_HAVEPARTIAL &&                  \
                --_gasnete_strided_chunkcnt == 0) ||                   \
               curdim == dim) break; /* traversal complete */          \
        for (curdim--; curdim >= 0; curdim--) {                        \
          srcptr_start[curdim] = psrc; /* save updated row starts */   \
          if (GASNETE_STRIDED_HELPER_HAVEDST)                          \
            dstptr_start[curdim] = pdst;                               \
        }                                                              \
      }                                                                \
      if (GASNETE_STRIDED_HELPER_HAVEPARTIAL &&                        \
          _gasnete_strided_update_addr_init) {                         \
        if (curdim == dim) { /* end of traversal */                    \
          psrc += _srcstrides[dim-2];                                  \
          if (GASNETE_STRIDED_HELPER_HAVEDST)                          \
            pdst += _dststrides[dim-2];                                \
        }                                                              \
        srcaddr = psrc;                                                \
        if (GASNETE_STRIDED_HELPER_HAVEDST) dstaddr = pdst;            \
        for (curdim = 0; curdim < dim; curdim++) {                     \
          gasneti_assert(idx[curdim] < _count[curdim]);                \
          _gasnete_strided_init[curdim] = idx[curdim];                 \
        }                                                              \
      }                                                                \
      if (dim > GASNETE_DIRECT_DIMS) {                                 \
        gasneti_free(idx);                                             \
        gasneti_free(srcptr_start);                                    \
        if (GASNETE_STRIDED_HELPER_HAVEDST)                            \
          gasneti_free(dstptr_start);                                  \
      }                                                                \
    } /* default */                                                    \
  } /* switch */                                                       \
} while (0)

/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts of the dualcontiguity size */
gasnet_handle_t gasnete_puts_ref_indiv(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                  gasnet_node_t dstnode,
                                   void *dstaddr, const size_t dststrides[],
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  const int islocal = (dstnode == gasneti_mynode);
  size_t const contiglevel = stats->dualcontiguity;
  GASNETI_TRACE_EVENT(C, PUTS_REF_INDIV);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  GASNETE_START_NBIREGION(synctype, islocal);

  if (contiglevel == stridelevels) { /* fully contiguous at both ends */
    GASNETE_PUT_INDIV(islocal, dstnode, dstaddr, srcaddr, stats->totalsz);
  } else {
    size_t const limit = stridelevels - stats->nulldims;
    size_t const contigsz = MIN(stats->srccontigsz, stats->dstcontigsz);

    #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  \
      GASNETE_PUT_INDIV(islocal, dstnode, pdst, psrc, contigsz)
    GASNETE_STRIDED_HELPER(limit,contiglevel);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
  }
  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}
/* reference version that uses individual gets of the dualcontiguity size */
gasnet_handle_t gasnete_gets_ref_indiv(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                   void *dstaddr, const size_t dststrides[],
                                   gasnet_node_t srcnode, 
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  const int islocal = (srcnode == gasneti_mynode);
  size_t const contiglevel = stats->dualcontiguity;
  GASNETI_TRACE_EVENT(C, GETS_REF_INDIV);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  GASNETE_START_NBIREGION(synctype, islocal);

  if (contiglevel == stridelevels) { /* fully contiguous at both ends */
    GASNETE_GET_INDIV(islocal, dstaddr, srcnode, srcaddr, stats->totalsz);
  } else {
    size_t const limit = stridelevels - stats->nulldims;
    size_t const contigsz = MIN(stats->srccontigsz, stats->dstcontigsz);

    #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  \
      GASNETE_GET_INDIV(islocal, pdst, srcnode, psrc, contigsz)
    GASNETE_STRIDED_HELPER(limit,contiglevel);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
  }
  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}
/*---------------------------------------------------------------------------------*/
/* strided full packing */

#define _GASNETE_STRIDED_PACKALL() {                                                   \
  uint8_t *ploc = buf;                                                                 \
  size_t const contiglevel = gasnete_strided_contiguity(strides, count, stridelevels); \
  size_t const limit = stridelevels - gasnete_strided_nulldims(count, stridelevels);   \
  size_t const contigsz = (contiglevel == 0 ? count[0] :                               \
                           count[contiglevel]*strides[contiglevel-1]);                 \
  /* macro interface */                                                                \
  void * srcaddr = addr;                                                               \
  size_t const * const srcstrides = strides;                                           \
  GASNETE_STRIDED_HELPER_DECLARE_NODST;                                                \
  GASNETE_STRIDED_HELPER(limit,contiglevel);                                           \
}

#define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
  GASNETE_FAST_UNALIGNED_MEMCPY(ploc, psrc, contigsz);   \
  ploc += contigsz;                                      \
} while (0)
void gasnete_strided_pack_all(void *addr, const size_t strides[],
                              const size_t count[], size_t stridelevels, 
                              void *buf) _GASNETE_STRIDED_PACKALL()
#undef GASNETE_STRIDED_HELPER_LOOPBODY

#define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
  GASNETE_FAST_UNALIGNED_MEMCPY(psrc, ploc, contigsz);   \
  ploc += contigsz;                                      \
} while (0)
void gasnete_strided_unpack_all(void *addr, const size_t strides[],
                                const size_t count[], size_t stridelevels, 
                                void *buf) _GASNETE_STRIDED_PACKALL()
#undef GASNETE_STRIDED_HELPER_LOOPBODY

/*---------------------------------------------------------------------------------*/
/* strided partial packing */
#define _GASNETE_STRIDED_PACKPARTIAL_INNER(_contiglevel,_limit) {                                    \
  size_t const contiglevel = (_contiglevel);                                                         \
  size_t const limit = (_limit);                                                                     \
  size_t const contigsz = (contiglevel == 0 ? count[0] : count[contiglevel]*strides[contiglevel-1]); \
  uint8_t *ploc = buf;                                                                               \
  /* macro interface */                                                                              \
  void *srcaddr = *addr;                                                                             \
  size_t const * const srcstrides = strides;                                                         \
  GASNETE_STRIDED_HELPER_DECLARE_NODST;                                                              \
  GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(numchunks,init,addr_already_offset,update_addr_init);       \
  GASNETE_STRIDED_HELPER(limit,contiglevel);                                                         \
  if (update_addr_init) *addr = srcaddr;                                                             \
  return ploc;                                                                                       \
}
/* if addr_already_offset is nonzero, the code assumes srcaddr/dstaddr already reference the first chunk,
    otherwise, the srcaddr/dstaddr values are offset based on init to reach the first chunk
   iff update_addr_init is nonzero, then srcaddr/dstaddr/init are updated on exit to point to the next unused chunk
   foldedstrided variants operate on a "folded" strided metadata - one where the nulldims have been 
    removed, and all contiguous trailing dimensions have been folded into count[0] 
 */
#define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
  GASNETE_FAST_UNALIGNED_MEMCPY(ploc, psrc, contigsz);   \
  ploc += contigsz;                                      \
} while (0)
void *gasnete_strided_pack_partial(void **addr, const size_t strides[],
                              const size_t count[], size_t __contiglevel, size_t __limit, 
                              size_t numchunks, size_t init[], 
                              int addr_already_offset, int update_addr_init,
                              void *buf) _GASNETE_STRIDED_PACKPARTIAL_INNER(__contiglevel, __limit)
void *gasnete_foldedstrided_pack_partial(void **addr, const size_t strides[],
                              const size_t count[], size_t stridelevels, 
                              size_t numchunks, size_t init[], 
                              int addr_already_offset, int update_addr_init,
                              void *buf) {
  gasneti_assert(gasnete_strided_contiguity(strides, count, stridelevels) == 0);
  gasneti_assert(gasnete_strided_nulldims(count, stridelevels) == 0);
  _GASNETE_STRIDED_PACKPARTIAL_INNER(0, stridelevels)
}
#undef GASNETE_STRIDED_HELPER_LOOPBODY

#define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
  GASNETE_FAST_UNALIGNED_MEMCPY(psrc, ploc, contigsz);   \
  ploc += contigsz;                                      \
} while (0)
void *gasnete_strided_unpack_partial(void **addr, const size_t strides[],
                              const size_t count[], size_t __contiglevel, size_t __limit, 
                              size_t numchunks, size_t init[], 
                              int addr_already_offset, int update_addr_init,
                              void *buf) _GASNETE_STRIDED_PACKPARTIAL_INNER(__contiglevel, __limit)
void *gasnete_foldedstrided_unpack_partial(void **addr, const size_t strides[],
                              const size_t count[], size_t stridelevels, 
                              size_t numchunks, size_t init[], 
                              int addr_already_offset, int update_addr_init,
                              void *buf) {
  gasneti_assert(gasnete_strided_contiguity(strides, count, stridelevels) == 0);
  gasneti_assert(gasnete_strided_nulldims(count, stridelevels) == 0);
  _GASNETE_STRIDED_PACKPARTIAL_INNER(0,stridelevels)
}
#undef GASNETE_STRIDED_HELPER_LOOPBODY

/*---------------------------------------------------------------------------------*/
/* convert strided metadata to addrlist metadata for the equivalent operation */
static void gasnete_convert_strided_to_addrlist(void * * srclist, void * * dstlist, 
                                    gasnete_strided_stats_t const *stats,
                                    void *_dstaddr, const size_t _dststrides[],
                                    void *_srcaddr, const size_t _srcstrides[],
                                    const size_t count[], size_t stridelevels) {
  size_t const contiglevel = stats->dualcontiguity;
  size_t const limit = stridelevels - stats->nulldims;
  size_t const srccontigsz = stats->srccontigsz;
  size_t const dstcontigsz = stats->dstcontigsz;

  gasneti_assert(srclist != NULL && dstlist != NULL && stats != NULL);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(limit > contiglevel);

  if (srccontigsz == dstcontigsz) {
    void * * srcpos = srclist; void * * dstpos = dstlist;
    void *srcaddr = _srcaddr; const size_t * const srcstrides = _srcstrides; 
    void *dstaddr = _dstaddr; const size_t * const dststrides = _dststrides; 
    #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
      *(srcpos++) = psrc;                                    \
      *(dstpos++) = pdst;                                    \
    } while(0)
    GASNETE_STRIDED_HELPER(limit,contiglevel);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    gasneti_assert(srcpos == srclist+stats->srcsegments);
    gasneti_assert(dstpos == dstlist+stats->dstsegments);
  } else {
    size_t _looplim; 
    void * * srcpos; void * * dstpos;
    void *srcaddr; const size_t * __srcstrides; 
    void *dstaddr; const size_t * __dststrides; 
    if (srccontigsz < dstcontigsz) {
      _looplim = dstcontigsz / srccontigsz;
      gasneti_assert(_looplim*srccontigsz == dstcontigsz);
      srcpos = srclist; dstpos = dstlist;
      srcaddr = _srcaddr; dstaddr = _dstaddr;
      __srcstrides = _srcstrides; __dststrides = _dststrides;
    } else { /* dstcontigsz < srccontigsz : swap metadata to allow unified loop nest below */
      _looplim = srccontigsz / dstcontigsz;
      gasneti_assert(_looplim*dstcontigsz == srccontigsz);
      srcpos = dstlist; dstpos = srclist;
      srcaddr = _dstaddr; dstaddr = _srcaddr;
      __srcstrides = _dststrides; __dststrides = _srcstrides;
    }
    { size_t const looplim = _looplim;
      size_t loopcnt = 1;
      const size_t * const srcstrides = __srcstrides; 
      const size_t * const dststrides = __dststrides; 
      #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
        *(srcpos++) = psrc;                                    \
        if (--loopcnt == 0) {                                  \
          *(dstpos++) = pdst;                                  \
          loopcnt = looplim;                                   \
        }                                                      \
      } while(0)
      GASNETE_STRIDED_HELPER(limit,contiglevel);
      #undef GASNETE_STRIDED_HELPER_LOOPBODY
    }
    if (srccontigsz < dstcontigsz) {
      gasneti_assert(srcpos == srclist+stats->srcsegments);
      gasneti_assert(dstpos == dstlist+stats->dstsegments);
    } else {
      gasneti_assert(dstpos == srclist+stats->srcsegments);
      gasneti_assert(srcpos == dstlist+stats->dstsegments);
    }
  }
}
/*---------------------------------------------------------------------------------*/
/* convert strided metadata to memvec metadata for the equivalent operation */
static void gasnete_convert_strided_to_memvec(gasnet_memvec_t *srclist, gasnet_memvec_t *dstlist, 
                                    gasnete_strided_stats_t const *stats,
                                    void *_dstaddr, const size_t _dststrides[],
                                    void *_srcaddr, const size_t _srcstrides[],
                                    const size_t count[], size_t stridelevels) {
  size_t const contiglevel = stats->dualcontiguity;
  size_t const limit = stridelevels - stats->nulldims;
  size_t const srccontigsz = stats->srccontigsz;
  size_t const dstcontigsz = stats->dstcontigsz;

  gasneti_assert(srclist != NULL && dstlist != NULL && stats != NULL);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(limit > contiglevel);

  if (srccontigsz == dstcontigsz) {
    gasnet_memvec_t *srcpos = srclist; gasnet_memvec_t *dstpos = dstlist;
    void *srcaddr = _srcaddr; const size_t * const srcstrides = _srcstrides; 
    void *dstaddr = _dstaddr; const size_t * const dststrides = _dststrides; 
    #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
      srcpos->addr = psrc;                                   \
      srcpos->len = srccontigsz;                             \
      srcpos++;                                              \
      dstpos->addr = pdst;                                   \
      dstpos->len = dstcontigsz;                             \
      dstpos++;                                              \
    } while(0)
    GASNETE_STRIDED_HELPER(limit,contiglevel);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    gasneti_assert(srcpos == srclist+stats->srcsegments);
    gasneti_assert(dstpos == dstlist+stats->dstsegments);
  } else {
    size_t _looplim; 
    gasnet_memvec_t *srcpos; gasnet_memvec_t *dstpos;
    size_t _srccontigsz; size_t _dstcontigsz;
    void *srcaddr; const size_t * __srcstrides; 
    void *dstaddr; const size_t * __dststrides; 
    if (srccontigsz < dstcontigsz) {
      _looplim = dstcontigsz / srccontigsz;
      gasneti_assert(_looplim*srccontigsz == dstcontigsz);
      srcpos = srclist; dstpos = dstlist;
      srcaddr = _srcaddr; dstaddr = _dstaddr;
      _srccontigsz = srccontigsz; _dstcontigsz = dstcontigsz;
      __srcstrides = _srcstrides; __dststrides = _dststrides;
    } else { /* dstcontigsz < srccontigsz : swap metadata to allow unified loop nest below */
      _looplim = srccontigsz / dstcontigsz;
      gasneti_assert(_looplim*dstcontigsz == srccontigsz);
      srcpos = dstlist; dstpos = srclist;
      srcaddr = _dstaddr; dstaddr = _srcaddr;
      _srccontigsz = dstcontigsz; _dstcontigsz = srccontigsz;
      __srcstrides = _dststrides; __dststrides = _srcstrides;
    }
    { size_t const looplim = _looplim;
      size_t loopcnt = 1;
      const size_t * const srcstrides = __srcstrides; 
      const size_t * const dststrides = __dststrides; 
      #define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
        srcpos->addr = psrc;                                   \
        srcpos->len = _srccontigsz;                            \
        srcpos++;                                              \
        if (--loopcnt == 0) {                                  \
          dstpos->addr = pdst;                                 \
          dstpos->len = _dstcontigsz;                          \
          dstpos++;                                            \
          loopcnt = looplim;                                   \
        }                                                      \
      } while(0)
      GASNETE_STRIDED_HELPER(limit,contiglevel);
      #undef GASNETE_STRIDED_HELPER_LOOPBODY
    }
    if (srccontigsz < dstcontigsz) {
      gasneti_assert(srcpos == srclist+stats->srcsegments);
      gasneti_assert(dstpos == dstlist+stats->dstsegments);
    } else {
      gasneti_assert(dstpos == srclist+stats->srcsegments);
      gasneti_assert(srcpos == dstlist+stats->dstsegments);
    }
  }
}
/*---------------------------------------------------------------------------------*/
/* simple gather put, remotely contiguous */
#ifndef GASNETE_PUTS_GATHER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_puts_gather(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                    gasnet_node_t dstnode,
                                    void *dstaddr, const size_t dststrides[],
                                    void *srcaddr, const size_t srcstrides[],
                                    const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = stats->totalsz;
  gasneti_assert(stats->dstcontiguity == stridelevels && stats->srccontiguity < stridelevels); /* only supports gather put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
  GASNETI_TRACE_EVENT(C, PUTS_GATHER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_strided_pack_all(srcaddr, srcstrides, count, stridelevels, packedbuf);
    visop->type = GASNETI_VIS_CAT_PUTS_GATHER;
    visop->handle = gasnete_put_nb_bulk(dstnode, dstaddr, packedbuf, nbytes GASNETE_THREAD_PASS);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0);
  }
}
  #define GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_remotecontig &&                                                                                 \
        (stats)->dstcontiguity == stridelevels && (stats)->srccontiguity < stridelevels)                                \
      return gasnete_puts_gather(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETS_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_gets_scatter(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                     void *dstaddr, const size_t dststrides[],
                                     gasnet_node_t srcnode, 
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = stats->totalsz;
  gasneti_assert(stats->srccontiguity == stridelevels && stats->dstcontiguity < stridelevels); /* only supports scatter get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
  GASNETI_TRACE_EVENT(C, GETS_SCATTER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+(2*stridelevels+1)*sizeof(size_t)+nbytes);
    size_t * const savedstrides = (size_t *)(visop + 1);
    size_t * const savedcount = savedstrides + stridelevels;
    void * const packedbuf = (void *)(savedcount + stridelevels + 1);
    memcpy(savedstrides, dststrides, stridelevels*sizeof(size_t));
    memcpy(savedcount, count, (stridelevels+1)*sizeof(size_t));
    visop->type = GASNETI_VIS_CAT_GETS_SCATTER;
    visop->addr = dstaddr;
    visop->len = stridelevels;
    visop->handle = gasnete_get_nb_bulk(packedbuf, srcnode, srcaddr, nbytes GASNETE_THREAD_PASS);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1);
  }
}
  #define GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_remotecontig &&                                                                                  \
        (stats)->srccontiguity == stridelevels && (stats)->dstcontiguity < stridelevels)                                 \
      return gasnete_gets_scatter(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTS_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
#define GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(stridelevels) (gasnet_AMMaxMedium() - (3*(stridelevels) + 1)*sizeof(size_t))
gasnet_handle_t gasnete_puts_AMPipeline(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                  gasnet_node_t dstnode,
                                   void *dstaddr, const size_t dststrides[],
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasneti_assert(stats->dstsegments > 1); /* supports scatter put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, PUTS_AMPIPELINE);
  GASNETE_START_NBIREGION(synctype, 0);

  { size_t * const init = gasneti_malloc(stridelevels*sizeof(size_t) + gasnet_AMMaxMedium());
    size_t * const packetbase = init + stridelevels;
    size_t * const packetinit = packetbase;
    size_t * const packetcount = packetinit + stridelevels;
    size_t * const packetstrides = packetcount + stridelevels + 1;
    size_t * const packedbuf = packetstrides + stridelevels;
    size_t const maxpayload = GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(stridelevels);
    size_t const packetoverhead = gasnet_AMMaxMedium() - maxpayload;
    size_t const chunksz = stats->dualcontigsz;
    size_t const totalchunks = MAX(stats->srcsegments,stats->dstsegments);
    size_t const chunksperpacket = maxpayload / chunksz;
    size_t const packetcnt = (totalchunks + chunksperpacket - 1)/chunksperpacket;
    size_t remaining = totalchunks;
    gasneti_iop_t *iop = gasneti_iop_register(packetcnt,0 GASNETE_THREAD_PASS);
    gasneti_assert(chunksz*totalchunks == stats->totalsz);
    gasneti_assert(chunksperpacket >= 1);
    memset(init, 0, stridelevels*sizeof(size_t)); /* init[] = [0..0] */
    memcpy(packetcount, count, (stridelevels+1)*sizeof(size_t));
    memcpy(packetstrides, dststrides, stridelevels*sizeof(size_t));
    while (remaining) {
      size_t const packetchunks = MIN(chunksperpacket, remaining);
      size_t * const adjinit = init+stats->dualcontiguity;
      uint8_t *end;
      size_t nbytes;
      remaining -= packetchunks;
      memcpy(packetinit, init, stridelevels*sizeof(size_t));
      if (stats->srccontiguity < stridelevels) { /* gather data payload from source into packet */
        end = gasnete_strided_pack_partial(&srcaddr, srcstrides, count, 
                                     stats->dualcontiguity, stridelevels - stats->nulldims, 
                                     packetchunks, adjinit, 
                                     1, remaining, packedbuf);
        nbytes = end - (uint8_t *)packetbase;
        gasneti_assert((end - (uint8_t *)packedbuf) == packetchunks * chunksz);
        gasneti_assert((end - (uint8_t *)packedbuf) <= GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(stridelevels));
        gasneti_assert((end - (uint8_t *)packedbuf) + packetoverhead == nbytes);
        #if GASNET_DEBUG
          if (remaining) {
            size_t * const tmp = gasneti_malloc(stridelevels*sizeof(size_t));
            memcpy(tmp, packetinit, stridelevels*sizeof(size_t));
            GASNETE_STRIDED_VECTOR_INC(tmp, packetchunks*chunksz/count[0], count, 0, stridelevels);
            gasneti_assert(!memcmp(tmp, init, stridelevels*sizeof(size_t)));
            gasneti_free(tmp);
          }
        #endif
      } else { /* source is contiguous */
        nbytes = packetchunks*chunksz;
        memcpy(packedbuf, srcaddr, nbytes);
        srcaddr = ((uint8_t *)srcaddr) + nbytes;
        if (remaining) GASNETE_STRIDED_VECTOR_INC(init, nbytes/count[0], count, 0, stridelevels);
        nbytes += packetoverhead;
      }
      /* fill packet with remote metadata */
      GASNETI_SAFE(
        MEDIUM_REQ(5,7,(dstnode, gasneti_handleridx(gasnete_puts_AMPipeline_reqh),
                      packetbase, nbytes,
                      PACK(iop), PACK(dstaddr), stridelevels, stats->dualcontiguity, packetchunks)));
    }
    gasneti_free(init);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, 0);
  }
}
  #define GASNETE_PUTS_AMPIPELINE_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_ampipe &&                                                                                           \
        (stats)->dstsegments > 1 &&                                                                                         \
        (stats)->dualcontigsz <= gasnete_vis_maxchunk &&                                                                    \
        (stats)->dualcontigsz <= GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(stridelevels))                                          \
      return gasnete_puts_AMPipeline(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTS_AMPIPELINE_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_puts_AMPipeline_reqh_inner)
void gasnete_puts_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *iop, void *dstaddr, 
  gasnet_handlerarg_t stridelevels, gasnet_handlerarg_t contiglevel, 
  gasnet_handlerarg_t packetchunks) {
  size_t * const packetinit = addr;
  size_t * const packetcount = packetinit + stridelevels;
  size_t * const packetstrides = packetcount + stridelevels + 1;
  size_t * const packedbuf = packetstrides + stridelevels;
  size_t const limit = stridelevels - gasnete_strided_nulldims(packetcount, stridelevels);
  GASNETI_UNUSED_UNLESS_DEBUG /* but still need side-effects */
  uint8_t * const end = gasnete_strided_unpack_partial(&dstaddr, packetstrides, packetcount, contiglevel, limit,
                                                       packetchunks, packetinit+contiglevel, 0, 0, packedbuf);
  gasneti_assert(end - (uint8_t *)addr == nbytes);
  gasneti_sync_writes();
  /* TODO: coalesce acknowledgements - need a per-srcnode, per-op seqnum & packetcnt */
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph),
                  PACK(iop))));
}
MEDIUM_HANDLER(gasnete_puts_AMPipeline_reqh,5,7, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1),      a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3), a4,a5,a6));
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter get */
#ifndef GASNETE_GETS_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
#define GASNETE_GETS_AMPIPELINE_MAXPAYLOAD(stridelevels) (gasnet_AMMaxMedium())
gasnet_handle_t gasnete_gets_AMPipeline(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                   void *dstaddr, const size_t dststrides[],
                                   gasnet_node_t srcnode, 
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasneti_assert(stats->srcsegments > 1); /* supports gather get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, GETS_AMPIPELINE);

  { size_t const chunksz = stats->dualcontigsz;
    size_t const adjchunksz = stats->dualcontigsz/count[0];
    size_t const totalchunks = MAX(stats->srcsegments,stats->dstsegments);
    size_t const chunksperpacket = gasnet_AMMaxMedium() / chunksz;
    size_t const packetcnt = (totalchunks + chunksperpacket - 1)/chunksperpacket;
    size_t const packetnbytes = (3*stridelevels+1)*sizeof(size_t);
    size_t packetidx;

    gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t) +
                                                   (2*stridelevels + 1)*sizeof(size_t) + /* tablecount, tablestrides */
                                                   packetcnt*stridelevels*sizeof(size_t) + /* tableinit */
                                                   packetnbytes); /* packet metadata */
    size_t * const tablebase = (size_t *)(visop + 1);
    size_t * const tablecount = tablebase;
    size_t * const tablestrides = tablecount + stridelevels + 1;
    size_t * tableinit = tablestrides + stridelevels;
    size_t * const packetbase = tableinit + packetcnt*stridelevels;
    size_t * const packetinit = packetbase;
    size_t * const packetcount = packetinit + stridelevels;
    size_t * const packetstrides = packetcount + stridelevels + 1;
    size_t remaining = totalchunks;
    gasneti_eop_t *eop;

    gasneti_assert(chunksz*totalchunks == stats->totalsz);
    gasneti_assert(chunksperpacket >= 1);

    GASNETE_VISOP_SETUP(visop, synctype, 1);
    visop->addr = dstaddr;
    visop->count = stridelevels;
    #if GASNET_DEBUG
      visop->type = GASNETI_VIS_CAT_GETS_AMPIPELINE;
    #endif
    gasneti_assert(packetcnt <= GASNETI_ATOMIC_MAX);
    gasneti_assert(packetcnt == (gasnet_handlerarg_t)packetcnt);
    gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);

    memcpy(tablecount, count, (stridelevels+1)*sizeof(size_t)); /* TODO: merge with packetcount? */
    memcpy(packetcount, count, (stridelevels+1)*sizeof(size_t)); 
    memcpy(tablestrides, dststrides, stridelevels*sizeof(size_t));
    memcpy(packetstrides, srcstrides, stridelevels*sizeof(size_t));
    memset(tableinit, 0, stridelevels*sizeof(size_t)); /* init[] = [0..0] */
    eop = visop->eop; /* visop may disappear once the last AM is launched */

    for (packetidx = 0; packetidx < packetcnt; packetidx++) {
      size_t const packetchunks = MIN(chunksperpacket, remaining);
      size_t * const nexttableinit = tableinit + stridelevels;
      size_t const adjnbytes = packetchunks*adjchunksz;
      remaining -= packetchunks;
      memcpy(packetinit, tableinit, stridelevels*sizeof(size_t));
      GASNETI_SAFE(
        MEDIUM_REQ(6,8,(srcnode, gasneti_handleridx(gasnete_gets_AMPipeline_reqh),
                      packetbase, packetnbytes,
                      PACK(visop), PACK(srcaddr), stridelevels, stats->dualcontiguity, packetchunks, packetidx)));

      if (remaining) {
        memcpy(nexttableinit, tableinit, stridelevels*sizeof(size_t));
        GASNETE_STRIDED_VECTOR_INC(nexttableinit, adjnbytes, count, 0, stridelevels);
      }
      tableinit = nexttableinit;
    }
    gasneti_assert(remaining == 0);
    gasneti_assert(tableinit == packetbase);
    GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
  }
}
  #define GASNETE_GETS_AMPIPELINE_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_ampipe &&                                                                                           \
        (stats)->srcsegments > 1 &&                                                                                         \
        (stats)->dualcontigsz <= gasnete_vis_maxchunk &&                                                                    \
        (stats)->dualcontigsz <= gasnet_AMMaxMedium())                                                                      \
      return gasnete_gets_AMPipeline(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETS_AMPIPELINE_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_gets_AMPipeline_reqh_inner)
void gasnete_gets_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, void *srcaddr, 
  gasnet_handlerarg_t stridelevels, gasnet_handlerarg_t contiglevel, 
  gasnet_handlerarg_t packetchunks, gasnet_handlerarg_t packetidx) {

  size_t * const packetinit = addr;
  size_t * const packetcount = packetinit + stridelevels;
  size_t * const packetstrides = packetcount + stridelevels + 1;
  size_t const limit = stridelevels - gasnete_strided_nulldims(packetcount, stridelevels);

  gasneti_assert((uint8_t *)(packetstrides+stridelevels) - (uint8_t *)addr == nbytes);
  gasneti_assert(gasnete_strided_contiguity(packetstrides, packetcount, stridelevels) >= contiglevel);
  gasneti_assert(contiglevel < limit);
  #if GASNET_DEBUG
  { size_t i;
    size_t chunksz = packetcount[0];
    for (i = 0; i < stridelevels; i++) {
      gasneti_assert(packetinit[i] < packetcount[i+1]);
      if (i < contiglevel) chunksz *= packetcount[i+1];
    }
    gasneti_assert(packetchunks * chunksz <= gasnet_AMMaxMedium());
  }
  #endif
  { /* gather data payload from source into packet */
    uint8_t * const packedbuf = gasneti_malloc(gasnet_AMMaxMedium());
    uint8_t * const end = gasnete_strided_pack_partial(&srcaddr, packetstrides, packetcount, 
                               contiglevel, limit, 
                               packetchunks, packetinit+contiglevel, 
                               0, 0, packedbuf);
    size_t nbytes = end - (uint8_t *)packedbuf;

    GASNETI_SAFE(
      MEDIUM_REP(4,5,(token, gasneti_handleridx(gasnete_gets_AMPipeline_reph),
                    packedbuf, nbytes,
                    PACK(_visop),packetidx,contiglevel,packetchunks)));
    gasneti_free(packedbuf);
  }
}
MEDIUM_HANDLER(gasnete_gets_AMPipeline_reqh,6,8, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1),      a2,a3,a4,a5),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3), a4,a5,a6,a7));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_gets_AMPipeline_reph_inner)
void gasnete_gets_AMPipeline_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, gasnet_handlerarg_t packetidx,
  gasnet_handlerarg_t contiglevel, gasnet_handlerarg_t packetchunks) {
  gasneti_vis_op_t * const visop = _visop;
  void *dstaddr = visop->addr;
  size_t const stridelevels = visop->count;
  size_t * const tablebase = (size_t *)(visop + 1);
  size_t * const tablecount = tablebase;
  size_t * const tablestrides = tablecount + stridelevels + 1;
  size_t * const tableinit = tablestrides + stridelevels + stridelevels * packetidx;
  size_t const limit = stridelevels - gasnete_strided_nulldims(tablecount, stridelevels);

  gasneti_assert(visop->type == GASNETI_VIS_CAT_GETS_AMPIPELINE);

  { GASNETI_UNUSED_UNLESS_DEBUG /* but still need side-effects */
    uint8_t * const end = gasnete_strided_unpack_partial(&dstaddr, tablestrides, tablecount, contiglevel, limit,
                                                       packetchunks, tableinit+contiglevel, 0, 0, addr);
    gasneti_assert(end - (uint8_t *)addr == nbytes);
  }
  
  if (gasneti_weakatomic_decrement_and_test(&(visop->packetcnt), 
                                            GASNETI_ATOMIC_WMB_PRE|GASNETI_ATOMIC_WEAK_FENCE)) {
    /* last response packet completes operation and cleans up */
    GASNETE_VISOP_SIGNAL(visop, 1);
    gasneti_free(visop); /* free visop, saved metadata and send buffer */
  }
}
MEDIUM_HANDLER(gasnete_gets_AMPipeline_reph,4,5, 
              (token,addr,nbytes, UNPACK(a0),      a1,a2,a3),
              (token,addr,nbytes, UNPACK2(a0, a1), a2,a3,a4));
#endif
/*---------------------------------------------------------------------------------*/
/* reference version that uses vector interface */
gasnet_handle_t gasnete_puts_ref_vector(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                  gasnet_node_t dstnode,
                                   void *dstaddr, const size_t dststrides[],
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTS_REF_VECTOR);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(GASNETE_PUTV_ALLOWS_VOLATILE_METADATA);

  if (stats->dualcontiguity == stridelevels) { /* fully contiguous at both ends */
    const int islocal = (dstnode == gasneti_mynode);
    gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
    GASNETE_START_NBIREGION(synctype, islocal);
      GASNETE_PUT_INDIV(islocal, dstnode, dstaddr, srcaddr, stats->totalsz);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
  } else {
    gasnet_handle_t retval;
    gasnet_memvec_t * const srclist = gasneti_malloc(sizeof(gasnet_memvec_t)*stats->srcsegments);
    gasnet_memvec_t * const dstlist = gasneti_malloc(sizeof(gasnet_memvec_t)*stats->dstsegments);

    gasnete_convert_strided_to_memvec(srclist, dstlist, stats, 
      dstaddr, dststrides, srcaddr, srcstrides, count, stridelevels);

    retval = gasnete_putv(synctype, dstnode, 
                          stats->dstsegments, dstlist, 
                          stats->srcsegments, srclist GASNETE_THREAD_PASS);
    gasneti_free(srclist);
    gasneti_free(dstlist);
    return retval; 
  }
}
/* reference version that uses vector interface */
gasnet_handle_t gasnete_gets_ref_vector(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                   void *dstaddr, const size_t dststrides[],
                                   gasnet_node_t srcnode, 
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETS_REF_VECTOR);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(GASNETE_GETV_ALLOWS_VOLATILE_METADATA);

  if (stats->dualcontiguity == stridelevels) { /* fully contiguous at both ends */
    const int islocal = (srcnode == gasneti_mynode);
    gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
    GASNETE_START_NBIREGION(synctype, islocal);
      GASNETE_GET_INDIV(islocal, dstaddr, srcnode, srcaddr, stats->totalsz);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
  } else {
    gasnet_handle_t retval;
    gasnet_memvec_t * const srclist = gasneti_malloc(sizeof(gasnet_memvec_t)*stats->srcsegments);
    gasnet_memvec_t * const dstlist = gasneti_malloc(sizeof(gasnet_memvec_t)*stats->dstsegments);

    gasnete_convert_strided_to_memvec(srclist, dstlist, stats, 
      dstaddr, dststrides, srcaddr, srcstrides, count, stridelevels);

    retval = gasnete_getv(synctype, 
                          stats->dstsegments, dstlist, 
                          srcnode,
                          stats->srcsegments, srclist GASNETE_THREAD_PASS);
    gasneti_free(srclist);
    gasneti_free(dstlist);
    return retval; 
  }
}
/*---------------------------------------------------------------------------------*/
/* reference version that uses indexed interface */
gasnet_handle_t gasnete_puts_ref_indexed(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                  gasnet_node_t dstnode,
                                   void *dstaddr, const size_t dststrides[],
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTS_REF_INDEXED);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(GASNETE_PUTI_ALLOWS_VOLATILE_METADATA);

  if (stats->dualcontiguity == stridelevels) { /* fully contiguous at both ends */
    const int islocal = (dstnode == gasneti_mynode);
    gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
    GASNETE_START_NBIREGION(synctype, islocal);
      GASNETE_PUT_INDIV(islocal, dstnode, dstaddr, srcaddr, stats->totalsz);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
  } else {
    gasnet_handle_t retval;
    void * * const srclist = gasneti_malloc(sizeof(void *)*stats->srcsegments);
    void * * const dstlist = gasneti_malloc(sizeof(void *)*stats->dstsegments);

    gasnete_convert_strided_to_addrlist(srclist, dstlist, stats, 
      dstaddr, dststrides, srcaddr, srcstrides, count, stridelevels);

    retval = gasnete_puti(synctype, dstnode, 
                          stats->dstsegments, dstlist, stats->dstcontigsz,
                          stats->srcsegments, srclist, stats->srccontigsz GASNETE_THREAD_PASS);
    gasneti_free(srclist);
    gasneti_free(dstlist);
    return retval; 
  }
}
/* reference version that uses indexed interface */
gasnet_handle_t gasnete_gets_ref_indexed(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                   void *dstaddr, const size_t dststrides[],
                                   gasnet_node_t srcnode, 
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETS_REF_INDEXED);
  gasneti_assert(!gasnete_strided_empty(count, stridelevels));
  gasneti_assert(GASNETE_GETI_ALLOWS_VOLATILE_METADATA);

  if (stats->dualcontiguity == stridelevels) { /* fully contiguous at both ends */
    const int islocal = (srcnode == gasneti_mynode);
    gasneti_assert(stats->totalsz == (size_t)stats->totalsz); /* check for size_t truncation */
    GASNETE_START_NBIREGION(synctype, islocal);
      GASNETE_GET_INDIV(islocal, dstaddr, srcnode, srcaddr, stats->totalsz);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
  } else {
    gasnet_handle_t retval;
    void * * const srclist = gasneti_malloc(sizeof(void *)*stats->srcsegments);
    void * * const dstlist = gasneti_malloc(sizeof(void *)*stats->dstsegments);

    gasnete_convert_strided_to_addrlist(srclist, dstlist, stats, 
      dstaddr, dststrides, srcaddr, srcstrides, count, stridelevels);

    retval = gasnete_geti(synctype,
                          stats->dstsegments, dstlist, stats->dstcontigsz,
                          srcnode,
                          stats->srcsegments, srclist, stats->srccontigsz GASNETE_THREAD_PASS);
    gasneti_free(srclist);
    gasneti_free(dstlist);
    return retval; 
  }
}
/*---------------------------------------------------------------------------------*/
/* top-level gasnet_puts_* entry point */
#ifndef GASNETE_PUTS_OVERRIDE
extern gasnet_handle_t gasnete_puts(gasnete_synctype_t synctype,
                                  gasnet_node_t dstnode,
                                   void *dstaddr, const size_t dststrides[],
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_strided_stats_t stats;
  gasneti_assert(gasnete_vis_isinit);
  gasnete_strided_stats(&stats, dststrides, srcstrides, count, stridelevels);

  /* catch silly degenerate cases */
  if_pf (stats.totalsz == 0) /* empty */
    return GASNET_INVALID_HANDLE;
  if_pf (dstnode == gasneti_mynode || /* purely local */ 
         stats.dualcontiguity == stridelevels) {/* fully contiguous */
    return gasnete_puts_ref_indiv(&stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_PUTS_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_PUTS_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) do {                         \
        switch (rand() % 4) {                                                                                                                     \
          case 0:                                                                                                                                 \
            GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);                        \
          case 1:                                                                                                                                 \
            GASNETE_PUTS_AMPIPELINE_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);                    \
          case 2:                                                                                                                                 \
            return gasnete_puts_ref_indiv(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);   \
          case 3:                                                                                                                                 \
            return gasnete_puts_ref_vector(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);  \
          case 4:                                                                                                                                 \
            return gasnete_puts_ref_indexed(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS); \
        } } while (0)
    #else
      #define GASNETE_PUTS_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels)       \
        GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);     \
        GASNETE_PUTS_AMPIPELINE_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels); \
        return gasnete_puts_ref_indiv(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
    #endif
  #endif
  GASNETE_PUTS_SELECTOR(&stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  gasneti_fatalerror("failure in GASNETE_PUTS_SELECTOR - should never reach here");
  return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */
}
#endif
/* top-level gasnet_gets_* entry point */
#ifndef GASNETE_GETS_OVERRIDE
extern gasnet_handle_t gasnete_gets(gasnete_synctype_t synctype,
                                   void *dstaddr, const size_t dststrides[],
                                   gasnet_node_t srcnode, 
                                   void *srcaddr, const size_t srcstrides[],
                                   const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_strided_stats_t stats;
  gasneti_assert(gasnete_vis_isinit);
  gasnete_strided_stats(&stats, dststrides, srcstrides, count, stridelevels);
  /* catch silly degenerate cases */
  if_pf (stats.totalsz == 0) /* empty */
    return GASNET_INVALID_HANDLE;
  if_pf (srcnode == gasneti_mynode || /* purely local */ 
         stats.dualcontiguity == stridelevels) {/* fully contiguous */
    return gasnete_gets_ref_indiv(&stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_GETS_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_GETS_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) do {                         \
        switch (rand() % 4) {                                                                                                                     \
          case 0:                                                                                                                                 \
            GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels);                       \
          case 1:                                                                                                                                 \
            GASNETE_GETS_AMPIPELINE_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels);                    \
          case 2:                                                                                                                                 \
            return gasnete_gets_ref_indiv(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);   \
          case 3:                                                                                                                                 \
            return gasnete_gets_ref_vector(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);  \
          case 4:                                                                                                                                 \
            return gasnete_gets_ref_indexed(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS); \
        } } while (0)
    #else 
      #define GASNETE_GETS_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels)       \
        GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels);    \
        GASNETE_GETS_AMPIPELINE_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels); \
        return gasnete_gets_ref_indiv(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS)
    #endif
  #endif
  GASNETE_GETS_SELECTOR(&stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels);
  gasneti_fatalerror("failure in GASNETE_GETS_SELECTOR - should never reach here");
  return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */
}
#endif
/*---------------------------------------------------------------------------------*/
