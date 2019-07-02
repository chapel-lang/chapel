/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_strided.c $
 * Description: GASNet Strided implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_refvis.c
#endif

/*---------------------------------------------------------------------------------*/
/* *** GASNet-EX Strided Implementation *** */
/*---------------------------------------------------------------------------------*

  High-level Design of the GEX Strided Implementation, v2.0:
  
  0. Trace user inputs and assert argument validity (trace/debug only)
  1. Handle trivial degeneracy in header
     a. elemsz == 0 : empty
     b. stridelevels == 0 : dual linear contiguity (_gex_RMA_Put/Get)
  -- Library boundary --
  2. Perform stride optimization/normalization: 3/4-pass, sl starts at stridelevels and shrinks
     PASS 1: Copy/convert user metadata to internal format, remove null dimensions, and
             perform stride inversion to make peer strides non-negative: O(sl)
     PASS 2(opt): Sort strides by peer stride: O(1) if already sorted, O(sl^2) otherwise
     PASS 3: Fold trailing duallcontig dimensions into elemsz: O(sl)
     PASS 4: Fold together trivial inner dimensions: O(sl)
     Trace optimized metadata, if it changed (trace only)
  3. Perform bounds check (debug only)
  4. Handle emergent degeneracy
     a. count[i] == 0 : empty
     b. stridelevels == 0 : dual linear contiguity (_gex_RMA_Put/Get)
  5. Handle NBRHD locality
     a. hoisted address translation and memcpy loop
  6. Perform stride analysis: 1-pass
     a. Compute contiguity parameters for each side O(sl)
  7. Select non-trivial algorithm (logic allows conduit override)
     - dual linear contiguity > MAX_CHUNK => indiv put/get (gasnete_{put,get})
     - peer linear contiguity > MAX_CHUNK => remote contig (gasnete_{put,get})
     - (off by default) Ref Indexed
     - (off by default) Ref Vector 
     - if metadata + MIN_CHUNKS*elemsz fits in MaxMedium => AM Pipeline 
     - otherwise, indiv put/get
  8. Perform non-trivial algorithm

 *---------------------------------------------------------------------------------*/

/* Clang can be picky */
#if PLATFORM_COMPILER_CLANG && PLATFORM_COMPILER_VERSION_GE(2,8,0)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconstant-logical-operand"
#endif

/* helper macros */
/* increment the values in init[0..(stridelevels-1)] by incval chunks, 
   using provided count[0..(stridelevels-1)] dimensional extents.
   incval uses the same units as count[0] (ie elements, not bytes), 
   and carries are propagated in ascending order through the dimensions of init
*/
#define GASNETE_STRIDED_VECTOR_INC(init, incval, countiter, stridelevels) do { \
    size_t const _stridelevels = (stridelevels);                           \
    size_t * const _init = (init);                                         \
    _init[0] += (incval);                                                  \
    for (size_t _dim = 0; _dim < _stridelevels; _dim++) {                  \
      size_t const _thisinit = _init[_dim];                                \
      size_t const _thismax = countiter(_dim);                             \
      if (_thisinit < _thismax) break;                                     \
      else {                                                               \
        gasneti_assert_uint(_dim ,!=, _stridelevels-1); /* overflow */     \
        /* TODO-EX: possibly use ldiv() here? */                           \
        size_t const _carries = _thisinit / _thismax;                      \
        _init[_dim] = _thisinit % _thismax;                                \
        _init[_dim+1] += _carries;                                         \
      }                                                                    \
    }                                                                      \
  } while(0)

// return the total size for an SMD holding up to stridelevels smd + scratch space
#define SMD_SZ(stridelevels) \
  gasneti_offsetof(gasneti_vis_smd_t, dim[stridelevels]) + \
  3*stridelevels*MAX(sizeof(ptrdiff_t),sizeof(size_t)) // TODO-EX: this may need to grow

// given an smd(stridelevels), return the pointer to the scratch space
// note the position of scratch depends on stridelevels (and thus may be invalidated if that changes)
#define SMD_SCRATCH(smd,stridelevels) \
  ((void*)&((smd)->dim[stridelevels]))

// Striding metadata iterators
#define _SITER_ARRAY_HELPER(idx)                 [(idx)]
#define SITER_ARRAY(parray)                      (parray)_SITER_ARRAY_HELPER

#define _SITER_SDIM_COUNT_HELPER(idx)            [(idx)].count
#define SITER_SDIM_COUNT(pdim)                   (pdim)_SITER_SDIM_COUNT_HELPER

#define _SITER_SDIM_STRIDE_HELPER_SMD_SELF(idx)  [(idx)].stride[SMD_SELF]
#define _SITER_SDIM_STRIDE_HELPER_SMD_PEER(idx)  [(idx)].stride[SMD_PEER]
#define SITER_SDIM_STRIDE(pdim, selfpeer)        (pdim)_SITER_SDIM_STRIDE_HELPER_##selfpeer

#define _SITER_NOOP_HELPER(idx) 
#define SITER_NOOP()                             (0)_SITER_NOOP_HELPER

/* 
   GASNETE_1STRIDED_HELPER(size_t stridelevels, countiter, void *p1base, stride1iter) 
   GASNETE_2STRIDED_HELPER(size_t stridelevels, countiter, void *p1base, stride1iter, 
                                                           void *p2base, stride2iter) 
    expands to code based on the template below (shown for the case of stridelevels == 3), 

   Iter arguments should be one of the following iterator macros. 
   Arguments may be expressions, eg to offset by contiglevel
     SITER_ARRAY({size_t,ptrdiff_t} *parray) 
       iterates over parray[0..(stridelevels-1)]
     SITER_SDIM_COUNT(gasneti_vis_smd_dim_t *pdim)
       iterates over pdim[0..(stridelevels-1)].count
     SITER_SDIM_STRIDE(gasneti_vis_smd_dim_t *pdim, SMD_{SELF,PEER})
       iterates over pdim[0..(stridelevels-1)].stride[SMD_SELF|SMD_PEER]

   If stridelevels > GASNETE_LOOPING_DIMS, then we use the generalized 
     striding code shown in the default case of GASNETE_STRIDED_HELPER.
   Parameters: 
     * if caller scope contains the declaration: 
         GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(size_t numchunks, size_t *init, 
                                                int addr_already_offset, int update_addr_init);
       then the traversal will iterate over a total of numchunks contiguous chunks, 
       beginning at chunk coordinate indicated by init[0..(stridelevels-1)]
       if addr_already_offset is nonzero, the code assumes p1base/p2base already reference the first chunk,
       otherwise, the pointer values are advanced based on init to reach the first chunk
       iff update_addr_init is nonzero and there are chunks remaining, 
       then p1base/p2base/init are updated on exit to point to the next unused chunk
       
    uint8_t * _p1 = p1base;
    uint8_t * _p2 = p2base; // only for 2STRIDED, otherwise always NULL
    size_t _chunkcnt = numchunks;

    if (HAVE_PARTIAL && !addr_already_offset) {
      for (size_t _dim = 0; _dim < stridelevels; _dim++) {
        _p1 += stride1iter(_dim) * (ptrdiff_t)init[_dim];
        _p2 += stride2iter(_dim) * (ptrdiff_t)init[_dim];
      }
    }

    size_t const _count0 = countiter(0);
    ptrdiff_t const _p1bump0 = stride1iter(0);
    ptrdiff_t const _p2bump0 = stride2iter(0);
    size_t _i0 = (HAVE_PARTIAL ? _count0 - init[0] : _count0);

    size_t const _count1 = countiter(1);
    ptrdiff_t const _p1bump1 = stride1iter(1) - (ptrdiff_t)_count0 * stride1iter(0);
    ptrdiff_t const _p2bump1 = stride2iter(1) - (ptrdiff_t)_count0 * stride2iter(0);
    size_t _i1 = (HAVE_PARTIAL ? _count1 - init[1] : _count1);

    size_t const _count2 = countiter(2);
    ptrdiff_t const _p1bump2 = stride1iter(2) - (ptrdiff_t)_count1 * stride1iter(1);
    ptrdiff_t const _p2bump2 = stride2iter(2) - (ptrdiff_t)_count1 * stride2iter(1);
    size_t _i2 = (HAVE_PARTIAL ? _count2 - init[2] : _count2);

    goto body;

    for (_i2 = _count2; _i2; _i2--) { 

    for (_i1 = _count1; _i1; _i1--) {

    for (_i0 = _count0; _i0; _i0--) {
      body:
      GASNETE_STRIDED_HELPER_LOOPBODY(_p1,_p2);
      _p1 += _p1bump0;
      _p2 += _p2bump0;
      if_pf (HAVE_PARTIAL && --_chunkcnt == 0) goto done;
    }

      _p1 += _p1bump1;
      _p2 += _p2bump1;
    }

      _p1 += _p1bump2;
      _p2 += _p2bump2;
    }
    done: ;
    if (HAVE_PARTIAL && update_addr_init) {
      if (!_i0) ; // loop nest terminated 
      else if (!--_i0) { _i0 = _count0; 

        _p1 += _p1bump1; 
        _p2 += _p2bump1; 
        if (!--_i1) { _i1 = _count1; 

          _p1 += _p1bump2; 
          _p2 += _p2bump2; 
          if (!--_i2) { _i2 = _count2; 

          }
        }
      }
      p1base = _p1;
      p2base = _p2;

      init[0] = _count0 - _i0;
      init[1] = _count1 - _i1;
      init[2] = _count2 - _i2;
    }
*/

#define _STRIDED_HELPER_DECLARE_NO2 \
 static int8_t _strided_helper_no2 = (int8_t)sizeof(_strided_helper_no2)
static int32_t _strided_helper_no2 = (int32_t)sizeof(_strided_helper_no2);
#define _STRIDED_HELPER_HAVE2 (sizeof(_strided_helper_no2) == 4)

#define GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(numchunks, init, addr_already_offset, update_addr_init) \
       size_t * const _strided_init = (init);                                                  \
       size_t _strided_chunkcnt = (numchunks);                                                 \
       int const _strided_addr_already_offset = (addr_already_offset);                         \
       int const _strided_update_addr_init = (update_addr_init);                               \
       static int8_t _strided_helper_havepartial = (int8_t)sizeof(_strided_helper_havepartial)

static int32_t * const _strided_init = 
   (sizeof(_strided_init)?NULL:(void*)&_strided_init); /* NULL:NULL triggers gcc -O1 bug on sysx */
static int32_t _strided_chunkcnt = (int32_t)sizeof(_strided_chunkcnt);
static int32_t const _strided_addr_already_offset = (int32_t)sizeof(_strided_addr_already_offset);
static int32_t const _strided_update_addr_init = (int32_t)sizeof(_strided_update_addr_init);
static int32_t const _strided_helper_havepartial = (int32_t)sizeof(_strided_helper_havepartial);
#define _STRIDED_HELPER_HAVEPARTIAL (sizeof(_strided_helper_havepartial) == 1)

#define _STRIDED_LABELHLP2(idx,name,line) _STRIDED_LABEL_##name##_##idx##_##line
#define _STRIDED_LABELHLP(idx,name,line)  _STRIDED_LABELHLP2(idx,name,line)
#define _STRIDED_LABEL(idx,name) _STRIDED_LABELHLP(idx,name,__LINE__)

#define _STRIDED_HELPER_SETUP_BASE(junk1,junk2,junk3, countiter, stride1iter, stride2iter) \
    size_t const _count0 = countiter(0);                                     \
    ptrdiff_t const _p1bump0 = stride1iter(0);                               \
    ptrdiff_t const _p2bump0 = (_STRIDED_HELPER_HAVE2 ? stride2iter(0) : 0); \
    size_t _i0 = _count0;                                                    \
    if (_STRIDED_HELPER_HAVEPARTIAL) {                                       \
      gasneti_assert_uint(_strided_init[0] ,<, _count0);                     \
      _i0 -= _strided_init[0];                                               \
    }

#define _STRIDED_HELPER_SETUP_INT(lower, curr, junk, countiter, stride1iter, stride2iter)  \
    size_t const _count##curr = countiter(curr);                                     \
    ptrdiff_t const _p1bump##curr = stride1iter(curr) -                              \
                                    ((ptrdiff_t)_count##lower) * stride1iter(lower); \
    ptrdiff_t const _p2bump##curr = (_STRIDED_HELPER_HAVE2 ?                         \
          stride2iter(curr) - ((ptrdiff_t)_count##lower) * stride2iter(lower) : 0);  \
    size_t _i##curr = _count##curr;                                                  \
    if (_STRIDED_HELPER_HAVEPARTIAL) {                                               \
      gasneti_assert_uint(_strided_init[curr] ,<, _count##curr);                     \
      _i##curr -= _strided_init[curr];                                               \
    }

#define _STRIDED_HELPER_LOOPHEAD(junk1,curr,junk2) \
    for (_i##curr = _count##curr; _i##curr; _i##curr--) { 

#define _STRIDED_HELPER_LOOPTAIL(junk1,curr,junk2)     \
      _p1 += _p1bump##curr;                            \
      if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump##curr; \
      else gasneti_assert(_p2 == NULL);                \
    }

#define _STRIDED_HELPER_CLEANUPHEAD(junk1,curr,junk2)  \
      _p1 += _p1bump##curr;                            \
      if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump##curr; \
      if (!--_i##curr) { _i##curr = _count##curr;

#define _STRIDED_HELPER_CLEANUPTAIL(junk1,curr,junk2) }

#define _STRIDED_HELPER_CLEANUPINIT(junk1,curr,junk2) \
  _strided_init[curr] = _count##curr - _i##curr;   \
  gasneti_assert_uint(_strided_init[curr] ,<, _count##curr);

#define _STRIDED_HELPER_CASE(curr,junk1,junk2, countiter, stride1iter, stride2iter) \
  case curr+1: {                                                    \
    GASNETI_META3_ASC##curr(_STRIDED_HELPER_SETUP_BASE,             \
                            _STRIDED_HELPER_SETUP_INT,              \
                            countiter, stride1iter, stride2iter)    \
    goto _STRIDED_LABEL(curr,BODY);                                 \
    GASNETI_META_DES##curr(GASNETI_META_EMPTY,_STRIDED_HELPER_LOOPHEAD) \
    for (_i0 = _count0; _i0; _i0--) {                               \
      _STRIDED_LABEL(curr,BODY): ;                                  \
      GASNETE_STRIDED_HELPER_LOOPBODY(_p1,_p2);                     \
      _p1 += _p1bump0;                                              \
      if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump0;                   \
      if_pf (_STRIDED_HELPER_HAVEPARTIAL &&                         \
             --_strided_chunkcnt == 0)                              \
        goto _STRIDED_LABEL(curr,DONE);                             \
    }                                                               \
    GASNETI_META_ASC##curr(GASNETI_META_EMPTY,_STRIDED_HELPER_LOOPTAIL) \
    _STRIDED_LABEL(curr,DONE): ;                                    \
    if (_STRIDED_HELPER_HAVEPARTIAL && _strided_update_addr_init) { \
      if (!_i0) ; /* loop nest terminated */                        \
      else if (!--_i0) { _i0 = _count0;                             \
        GASNETI_META_ASC##curr(GASNETI_META_EMPTY,_STRIDED_HELPER_CLEANUPHEAD) \
        GASNETI_META_ASC##curr(GASNETI_META_EMPTY,_STRIDED_HELPER_CLEANUPTAIL) \
      }                                                             \
      *_pp1base = _p1;                                              \
      if (_STRIDED_HELPER_HAVE2) *_pp2base = _p2;                   \
      else gasneti_assert(_p2 == NULL);                             \
      _strided_init[0] = _count0 - _i0;                             \
      GASNETI_META_ASC##curr(GASNETI_META_EMPTY,_STRIDED_HELPER_CLEANUPINIT) \
    }                                                               \
  } break;

#if GASNETE_LOOPING_DIMS > GASNETI_META_MAX
#error GASNETE_LOOPING_DIMS must be <= GASNETI_META_MAX
#endif

#if GASNET_DEBUG
#define GASNETE_IS_DEBUG 1
#else
#define GASNETE_IS_DEBUG 0
#endif

#define GASNETE_1STRIDED_HELPER(stridelevels, countiter, p1base, stride1iter) do { \
    _STRIDED_HELPER_DECLARE_NO2;                                                   \
    void *_dummy2 = NULL;                                                          \
    GASNETE_2STRIDED_HELPER(stridelevels, countiter, p1base, stride1iter,          \
                            _dummy2, SITER_NOOP());                                \
  } while (0)

#define GASNETE_SMD_STRIDED_HELPER(smd) do {                                            \
    gasneti_vis_smd_t * const _smd = (smd);                                             \
    gasneti_vis_smd_dim_t const * const _sdim = _smd->dim;                              \
    GASNETE_2STRIDED_HELPER(_smd->stridelevels,   SITER_SDIM_COUNT(_sdim),              \
                            _smd->addr[SMD_SELF], SITER_SDIM_STRIDE(_sdim, SMD_SELF),   \
                            _smd->addr[SMD_PEER], SITER_SDIM_STRIDE(_sdim, SMD_PEER));  \
  } while (0)

#define GASNETE_2STRIDED_HELPER(stridelevels, countiter, p1base, stride1iter, p2base, stride2iter) do { \
  void ** const _pp1base = &(p1base);                                  \
  void ** const _pp2base = &(p2base);                                  \
  /* general setup code */                                             \
  uint8_t *_p1 = *_pp1base;                                            \
  uint8_t *_p2 = *_pp2base;                                            \
  size_t const _stridelevels = (stridelevels);                         \
  gasneti_assert(_stridelevels > 0); /* should never use for degen */  \
  if (_STRIDED_HELPER_HAVEPARTIAL && !_strided_addr_already_offset) {  \
    for (size_t _dim = 0; _dim < _stridelevels; _dim++) {              \
      _p1 += stride1iter(_dim) * (ptrdiff_t)_strided_init[_dim];       \
      if (_STRIDED_HELPER_HAVE2)                                       \
        _p2 += stride2iter(_dim) * (ptrdiff_t)_strided_init[_dim];     \
    }                                                                  \
  }                                                                    \
  switch (_stridelevels) {                                             \
    _CONCAT(GASNETI_META3_ASC, GASNETE_LOOPING_DIMS)(                  \
      GASNETI_META3_EMPTY, _STRIDED_HELPER_CASE,                       \
      countiter, stride1iter, stride2iter)                             \
    default: { /* arbitrary dimensions > GASNETE_LOOPING_DIMS */       \
      size_t    __idx[GASNETE_DIRECT_DIMS];                            \
      ptrdiff_t __p1bump[GASNETE_DIRECT_DIMS];                         \
      ptrdiff_t __p2bump[GASNETE_DIRECT_DIMS];                         \
      size_t * const _idx =                                            \
                   (_stridelevels <= GASNETE_DIRECT_DIMS ? __idx :     \
                    gasneti_malloc(_stridelevels*sizeof(size_t)));     \
      ptrdiff_t * const _p1bump =                                      \
                   (_stridelevels <= GASNETE_DIRECT_DIMS ? __p1bump :  \
                    gasneti_malloc(_stridelevels*sizeof(ptrdiff_t)));  \
      ptrdiff_t * const _p2bump = ((!_STRIDED_HELPER_HAVE2 ||          \
                    _stridelevels <= GASNETE_DIRECT_DIMS) ? __p2bump : \
                    gasneti_malloc(_stridelevels*sizeof(ptrdiff_t)));  \
      _idx[0] = countiter(0);                                          \
      _p1bump[0] = stride1iter(0);                                     \
      _p2bump[0] = (_STRIDED_HELPER_HAVE2 ? stride2iter(0) : 0);       \
      for (size_t _d = 1; _d < _stridelevels; _d++) {                  \
        _idx[_d] = countiter(_d);                                      \
        _p1bump[_d] = stride1iter(_d) -                                \
                 ((ptrdiff_t)_idx[_d-1]) * stride1iter(_d-1);          \
        if (_STRIDED_HELPER_HAVE2)                                     \
          _p2bump[_d] = stride2iter(_d) -                              \
                 ((ptrdiff_t)_idx[_d-1]) * stride2iter(_d-1);          \
      }                                                                \
      if (_STRIDED_HELPER_HAVEPARTIAL) {                               \
        for (size_t _d = 0; _d < _stridelevels; _d++) {                \
          gasneti_assert_uint(_strided_init[_d] ,<, _idx[_d]);         \
          _idx[_d] -= _strided_init[_d];                               \
        }                                                              \
      }                                                                \
      uint8_t const *_p1_truebase = *_pp1base;                         \
      uint8_t const *_p2_truebase = *_pp2base;                         \
      if (GASNETE_IS_DEBUG &&                                           \
        _STRIDED_HELPER_HAVEPARTIAL && _strided_addr_already_offset) { \
        for (size_t _d = 0; _d < _stridelevels; _d++) {                \
         _p1_truebase -= (ptrdiff_t)_strided_init[_d] * stride1iter(_d); \
         if (_STRIDED_HELPER_HAVE2)                                    \
          _p2_truebase -= (ptrdiff_t)_strided_init[_d] * stride2iter(_d); \
        }                                                              \
      }                                                                \
      while (1) { /* main iteration loop */                            \
       _STRIDED_LABEL(general,BODY): ;                                 \
        GASNETE_CHECK_PTR(_p1, _p1_truebase, stride1iter, countiter,   \
                          _idx, 1, _stridelevels);                     \
        if (_STRIDED_HELPER_HAVE2)                                     \
          GASNETE_CHECK_PTR(_p2, _p2_truebase, stride2iter, countiter, \
                            _idx, 1, _stridelevels);                   \
        else gasneti_assert(_p2 == NULL);                              \
        GASNETE_STRIDED_HELPER_LOOPBODY(_p1,_p2);                      \
        _p1 += _p1bump[0];                                             \
        if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump[0];                  \
        if_pf (_STRIDED_HELPER_HAVEPARTIAL &&                          \
               --_strided_chunkcnt == 0) break;                        \
        if (--_idx[0] == 0) { /* end 0-level body */                   \
          for (size_t _d=1; _d < _stridelevels; _d++) {                \
            _p1 += _p1bump[_d];                                        \
            if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump[_d];             \
            if (--_idx[_d]) { /* begin _d-level body */                \
              for (size_t _e=_d-1; ; _e--) { /* reset lower idx */     \
                _idx[_e] = countiter(_e);                              \
                if (!_e) goto _STRIDED_LABEL(general,BODY);            \
              } gasneti_unreachable();                                 \
            }                                                          \
          }                                                            \
          for (size_t _d=0; _d < _stridelevels; _d++)                  \
            gasneti_assert_uint(_idx[_d] ,==, 0);                      \
          break; /* all _idx[] zero, iteration complete */             \
        }                                                              \
      }                                                                \
      /* loop cleanup code */                                          \
      if (_STRIDED_HELPER_HAVEPARTIAL && _strided_update_addr_init) {  \
        if (_idx[0]) { /* early termination */                         \
          for (size_t _d=0; _d < _stridelevels; _d++) {                \
            if (--_idx[_d] == 0) { _idx[_d] = countiter(_d);           \
              if (_d+1 < _stridelevels) {                              \
                _p1 += _p1bump[_d+1];                                  \
                if (_STRIDED_HELPER_HAVE2) _p2 += _p2bump[_d+1];       \
              }                                                        \
            } else break;                                              \
          }                                                            \
          *_pp1base = _p1;                                             \
          if (_STRIDED_HELPER_HAVE2) *_pp2base = _p2;                  \
          else gasneti_assert(_p2 == NULL);                            \
          for (size_t _d = 0; _d < _stridelevels; _d++) {              \
            _strided_init[_d] = countiter(_d) - _idx[_d];              \
            gasneti_assert_uint(_strided_init[_d] ,<, countiter(_d));  \
          }                                                            \
        }                                                              \
      }                                                                \
      if (_stridelevels > GASNETE_DIRECT_DIMS) {                       \
        gasneti_free(_idx);                                            \
        gasneti_free(_p1bump);                                         \
        if (_STRIDED_HELPER_HAVE2) gasneti_free(_p2bump);              \
      }                                                                \
    } /* default */                                                    \
  } /* switch */                                                       \
} while (0)

#if GASNET_DEBUG
  /* assert the generalized looping code is functioning properly */
  #define GASNETE_CHECK_PTR(ploc, truebase, strideiter, countiter, idx, invertidx, stridelevels) do { \
      uint8_t const *_ptest = (truebase);                       \
      for (size_t _d=0; _d < (stridelevels); _d++) {            \
        size_t _thisidx = (idx)[_d];                            \
        if (invertidx) _thisidx = countiter(_d) - _thisidx;     \
        gasneti_assert_uint(_thisidx ,<, countiter(_d));        \
        _ptest += _thisidx * strideiter(_d);                    \
      }                                                         \
      gasneti_assert_ptr(_ptest ,==, (ploc));                   \
    } while (0)
#else
  #define GASNETE_CHECK_PTR(ploc, truebase, strideiter, countiter, idx, invertidx, stridelevels) ((void)0)
#endif

/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts of the dualcontiguity size */
gex_Event_t gasnete_puts_ref_indiv(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTS_REF_INDIV);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
  gex_Event_t * const lc_opt = (flags & GEX_FLAG_ENABLE_LEAF_LC) ? GEX_EVENT_GROUP : GEX_EVENT_DEFER;
  GASNETE_START_NBIREGION(synctype);

    size_t const elemsz = smd->elemsz;
    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  \
      GASNETE_PUT_INDIV(tm, rank, p2, p1, elemsz, lc_opt)
    GASNETE_SMD_STRIDED_HELPER(smd);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY

  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}

/* reference version that uses individual gets of the dualcontiguity size */
gex_Event_t gasnete_gets_ref_indiv(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETS_REF_INDIV);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
  GASNETE_START_NBIREGION(synctype);

    size_t const elemsz = smd->elemsz;
    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  \
      GASNETE_GET_INDIV(tm, rank, p1, p2, elemsz)
    GASNETE_SMD_STRIDED_HELPER(smd);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY

  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}

// perform a loopback/PSHM memcpy of a strided area
void gasnete_strided_memcpy(void * dstbase, void * srcbase, 
                            size_t const stridelevels, size_t const elemsz,
                            gasneti_vis_smd_dim_t const * const sdim, int srcside) {
  gasneti_assert(elemsz > 0);
  gasneti_assert(stridelevels > 0);

  #define SITER_SRC_STRIDE(idx) sdim[idx].stride[srcside]
  #define SITER_DST_STRIDE(idx) sdim[idx].stride[!srcside]
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  \
    GASNETI_MEMCPY(p1, p2, elemsz)

    GASNETE_2STRIDED_HELPER(stridelevels, SITER_SDIM_COUNT(sdim),
                            dstbase, SITER_DST_STRIDE,
                            srcbase, SITER_SRC_STRIDE);
  #undef SITER_SRC_STRIDE
  #undef SITER_DST_STRIDE
  #undef GASNETE_STRIDED_HELPER_LOOPBODY
}

#if GASNETE_PARTIALPACK_TEST && GASNET_DEBUG
// Test code for partial packing
void gasnete_partialpack_memcpy(void * dstbase, void * srcbase, 
                            size_t const stridelevels, size_t const elemsz,
                            gasneti_vis_smd_dim_t const * const sdim, int srcside) {
  gasneti_assert(elemsz > 0);
  gasneti_assert(stridelevels > 0);

  #define SITER_SRC_STRIDE(idx) sdim[idx].stride[srcside]
  #define SITER_DST_STRIDE(idx) sdim[idx].stride[!srcside]
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  \
    do { GASNETI_MEMCPY(p1, p2, elemsz); invchunks++; } while (0)

  size_t total_chunks = 1;
  for (size_t d = 0; d < stridelevels; d++)
    total_chunks *= sdim[d].count;
  size_t *init = gasneti_calloc(stridelevels, sizeof(size_t));
  size_t *tmpv = gasneti_calloc(stridelevels, sizeof(size_t));
  int iter = 0;
  void *psrc = srcbase;
  void *pdst = dstbase;
  while (total_chunks) {
    size_t numchunks = (total_chunks + 1)/2;
    size_t invchunks = 0;
    int addr_already_offset = iter % 2;
    if (!addr_already_offset) { psrc = srcbase; pdst = dstbase; }
    GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(numchunks, init, addr_already_offset, 1);

    GASNETE_2STRIDED_HELPER(stridelevels, SITER_SDIM_COUNT(sdim),
                            pdst, SITER_DST_STRIDE,
                            psrc, SITER_SRC_STRIDE);

    gasneti_assert_uint(invchunks ,==, numchunks);
    if (total_chunks > numchunks) { // partial outputs only valid when there are trailing elements
      GASNETE_STRIDED_VECTOR_INC(tmpv, numchunks, SITER_SDIM_COUNT(sdim), stridelevels);
      GASNETE_CHECK_PTR(psrc, srcbase, SITER_SRC_STRIDE, SITER_SDIM_COUNT(sdim), tmpv, 0, stridelevels); 
      GASNETE_CHECK_PTR(pdst, dstbase, SITER_DST_STRIDE, SITER_SDIM_COUNT(sdim), tmpv, 0, stridelevels); 
      for (size_t d = 0; d < stridelevels; d++) gasneti_assert_uint(init[d] ,==, tmpv[d]);
    }
   
    total_chunks -= numchunks;
    iter++;
  }
  gasneti_free(init);
  gasneti_free(tmpv);
  #undef SITER_SRC_STRIDE
  #undef SITER_DST_STRIDE
  #undef GASNETE_STRIDED_HELPER_LOOPBODY
}
#define gasnete_strided_memcpy gasnete_partialpack_memcpy
#endif

#if _DISABLED_STUFF_
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
  GASNETI_MEMCPY(ploc, psrc, contigsz);                  \
  ploc += contigsz;                                      \
} while (0)
void gasnete_strided_pack_all(void *addr, const size_t strides[],
                              const size_t count[], size_t stridelevels, 
                              void *buf) _GASNETE_STRIDED_PACKALL()
#undef GASNETE_STRIDED_HELPER_LOOPBODY

#define GASNETE_STRIDED_HELPER_LOOPBODY(psrc,pdst)  do { \
  GASNETI_MEMCPY(psrc, ploc, contigsz);                  \
  ploc += contigsz;                                      \
} while (0)
void gasnete_strided_unpack_all(void *addr, const size_t strides[],
                                const size_t count[], size_t stridelevels, 
                                void *buf) _GASNETE_STRIDED_PACKALL()
#undef GASNETE_STRIDED_HELPER_LOOPBODY

/*---------------------------------------------------------------------------------*/
/* simple gather put, remotely contiguous */
#ifndef GASNETE_PUTS_GATHER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gex_Event_t gasnete_puts_gather(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                    gex_Rank_t dstnode,
                                    void *dstaddr, const size_t dststrides[],
                                    void *srcaddr, const size_t srcstrides[],
                                    const size_t count[], size_t stridelevels GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = stats->_totalsz;
  gasneti_assert(stats->_dstcontiguity == stridelevels && stats->_srccontiguity < stridelevels); /* only supports gather put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, PUTS_GATHER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_strided_pack_all(srcaddr, srcstrides, count, stridelevels, packedbuf);
    visop->type = GASNETI_VIS_CAT_PUTS_GATHER;
    visop->event = gasnete_put_nb(gasneti_THUNK_TM, dstnode, dstaddr, packedbuf, nbytes, GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0, (void)0);
  }
}
  #define GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_remotecontig &&                                                                                 \
        (stats)->_dstcontiguity == stridelevels && (stats)->_srccontiguity < stridelevels)                              \
      return gasnete_puts_gather(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTS_GATHER_SELECTOR(stats,synctype,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETS_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gex_Event_t gasnete_gets_scatter(gasnete_strided_stats_t const *stats, gasnete_synctype_t synctype,
                                     void *dstaddr, const size_t dststrides[],
                                     gex_Rank_t srcnode,
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = stats->_totalsz;
  gasneti_assert(stats->_srccontiguity == stridelevels && stats->_dstcontiguity < stridelevels); /* only supports scatter get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, GETS_SCATTER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+(2*stridelevels+1)*sizeof(size_t)+nbytes);
    size_t * const savedstrides = (size_t *)(visop + 1);
    size_t * const savedcount = savedstrides + stridelevels;
    void * const packedbuf = (void *)(savedcount + stridelevels + 1);
    GASNETI_MEMCPY(savedstrides, dststrides, stridelevels*sizeof(size_t));
    GASNETI_MEMCPY(savedcount, count, (stridelevels+1)*sizeof(size_t));
    visop->type = GASNETI_VIS_CAT_GETS_SCATTER;
    visop->addr = dstaddr;
    visop->len = stridelevels;
    visop->event = gasnete_get_nb(gasneti_THUNK_TM, packedbuf, srcnode, srcaddr, nbytes, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1, (void)0);
  }
}
  #define GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
    if (gasnete_vis_use_remotecontig &&                                                                                  \
        (stats)->_srccontiguity == stridelevels && (stats)->_dstcontiguity < stridelevels)                               \
      return gasnete_gets_scatter(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETS_SCATTER_SELECTOR(stats,synctype,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) ((void)0)
#endif
#endif
#endif // DISABLED
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#if GASNETE_USE_AMPIPELINE && !defined(GASNETE_PUTS_AMPIPELINE)
#define GASNETE_PUTS_AMPIPELINE 1
#define _GASNETE_PUTS_AMPIPELINE_MAXPACKET(tm,rank,stridelevels,args)                       \
          gex_AM_MaxRequestMedium((tm),(rank),                                              \
                                  (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),                \
                                  (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0), \
                                  args) 
#define GASNETE_PUTS_AMPIPELINE_MAXPACKET(tm,rank,stridelevels) \
       _GASNETE_PUTS_AMPIPELINE_MAXPACKET(tm,rank,stridelevels,HARGS(5,7))
#define GASNETE_PUTS_AMPIPELINE1_MAXPACKET(tm,rank,stridelevels) \
       _GASNETE_PUTS_AMPIPELINE_MAXPACKET( tm,rank,stridelevels,HARGS(4,6))
#define GASNETE_PUTS_AMPIPELINE_PACKETOVERHEAD(stridelevels)  ((stridelevels)*(2*sizeof(size_t)+sizeof(ptrdiff_t)))
#define GASNETE_PUTS_AMPIPELINE1_PACKETOVERHEAD(stridelevels) ((stridelevels)*(sizeof(size_t)+sizeof(ptrdiff_t)))
#define GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(tm,rank,stridelevels) \
  ((ptrdiff_t)GASNETE_PUTS_AMPIPELINE_MAXPACKET(tm,rank,stridelevels) - (ptrdiff_t)GASNETE_PUTS_AMPIPELINE_PACKETOVERHEAD(stridelevels))
GASNETI_NEVER_INLINE(gasnete_puts_AMPipeline,
gex_Event_t gasnete_puts_AMPipeline(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG)) {
  GASNETI_TRACE_EVENT(C, PUTS_AMPIPELINE);
  gasneti_assert(smd->have_stats);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  // temporary storage: (smd scratch for NPAM, otherwise malloc)
  //  init[stridelevels] | count[stridelevels] | peer_strides[stridelevels] 
  //
  // packet data:
  //   packet_init[stridelevels] - cloned from local init[] during packing
  //     - omitted for single-packet
  //   count[stridelevels] - copied from smd
  //   peer_strides[stridelevels] - copied from smd
  //   packed data, to fill up to MaxRequestMedium()
  //     packetchunks complete chunks of elemsz granularity
  //     no partial chunks
  //
  // AM Request args: (single-packet)
  //   &op_addr, dstaddr, (int)stridelevels, (int)elemsz
  // AM Request args: (multi-packet)
  //   &access_region_iop, dstaddr, (int)stridelevels, (int)elemsz, (int)packetchunks
  //
  // Each request handler unpacks into destination and sends a reply to markdone(1) the iop
  //
  // TODO-EX: consider growing chunksize to lcontig_sz[SMD_PEER]:
  // thus removing lcontig_dims[SMD_PEER] entries from the 2-3 metadata arrays on the wire
  // and increasing the granularity of the unpack operation at the peer.
  // The cost is coarser granularity in the chunking decreases the packet occupancy of the payload
  // Implementation would simply require shrinking all the stridelevels above (and growing elemsz),
  // except for local_init[], which is deliberately positioned to allow sending the suffix
  //
  gasneti_vis_smd_dim_t const * const sdim = smd->dim;
  const void * const dstaddr = smd->addr[SMD_PEER];
  size_t const stridelevels = smd->stridelevels;
  size_t const chunksz = smd->elemsz;
  size_t const totalchunks = smd->elemcnt;
  size_t const totalsz = smd->totalsz;
  gasneti_assert_uint(chunksz*totalchunks ,==, totalsz);

  size_t const maxpacket1 = GASNETE_PUTS_AMPIPELINE1_MAXPACKET(tm,rank,stridelevels);
  size_t const headersz1 = GASNETE_PUTS_AMPIPELINE1_PACKETOVERHEAD(stridelevels);
  size_t const maxpayload1 = maxpacket1 - headersz1;

  gasneti_assert_uint(headersz1 + chunksz ,<=, maxpacket1);

  if (totalsz <= maxpayload1) { // TODO-EX: could squeeze in more by trimming packet_init and packetchunks
    // *** simplified path: entire payload fits in a single MaxMedium packet
    size_t const packetsz = headersz1 + totalsz;
    gex_Event_t handle;
    void *op;
    GASNETE_START_ONEOP(op, handle, synctype, 0);

    #if GASNETE_VIS_NPAM
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, packetsz, packetsz, NULL, 0, HARGS(4,6));
      void * const packetbase = gex_AM_SrcDescAddr(sd);
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, packetsz);
    #else
      void   * const packetbase = gasnete_visbuf_malloc(packetsz);
    #endif
    size_t    * const packetcount = packetbase;
    ptrdiff_t * const packetstrides = (ptrdiff_t*)(packetcount + stridelevels);
    void      * const packedbuf = packetstrides + stridelevels;
    gasneti_assume(packedbuf == (uint8_t*)packetbase + headersz1);

    // setup the packet header
    for (size_t d = 0; d < stridelevels; d++) {
      packetcount[d] = sdim[d].count; 
      packetstrides[d] = sdim[d].stride[SMD_PEER]; 
    }

    void * srcaddr = smd->addr[SMD_SELF];
    if (smd->lcontig_dims[SMD_SELF] == stridelevels) { // source is contiguous
      GASNETI_MEMCPY(packedbuf, srcaddr, totalsz);
    } else { // gather data payload from source into packet
      uint8_t *pbuf = packedbuf;
      #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
        GASNETI_MEMCPY(pbuf, p1, chunksz); pbuf += chunksz; \
      } while (0)
        GASNETE_1STRIDED_HELPER(stridelevels, SITER_SDIM_COUNT(sdim),
                                     srcaddr, SITER_SDIM_STRIDE(sdim,SMD_SELF));
      #undef GASNETE_STRIDED_HELPER_LOOPBODY
      gasneti_assert_ptr(pbuf ,==, (uint8_t*)packetbase + packetsz);
    }

    // send packet
    gasneti_assert_uint(stridelevels ,==, (size_t)(uint32_t)(gex_AM_Arg_t)stridelevels);
    gasneti_assert_uint(chunksz ,==,      (size_t)(uint32_t)(gex_AM_Arg_t)chunksz);
    #define ARGS PACK(op), PACK(dstaddr), stridelevels, chunksz
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_puts_AMPipeline1_reqh),
                               packetbase, packetsz, GEX_EVENT_NOW, 0, ARGS);
      gasneti_free(packetbase);
    #else
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_puts_AMPipeline1_reqh), packetsz, ARGS);
    #endif   
    #undef ARGS

    GASNETE_RETURN_ONEOP(handle, synctype);
  }

  // *** general multi-packet path
  GASNETE_START_NBIREGION(synctype);
  size_t const maxpacket = GASNETE_PUTS_AMPIPELINE_MAXPACKET(tm,rank,stridelevels);
  size_t const headersz = GASNETE_PUTS_AMPIPELINE_PACKETOVERHEAD(stridelevels);
  size_t const maxpayload = maxpacket - headersz;
  gasneti_assert_uint(headersz + chunksz ,<=, maxpacket);

  #if GASNETE_VIS_NPAM
    void   * const header = SMD_SCRATCH(smd,stridelevels);
    size_t * const init = header;
    size_t * const packetinit = init;
  #else
    size_t * const init = gasnete_visbuf_malloc(stridelevels*sizeof(size_t) + maxpacket);
    void   * const packetbase = init + stridelevels;
    size_t * const packetinit = packetbase;
    void   * const packedbuf = (uint8_t*)packetbase + headersz;
  #endif
  size_t    * const packetcount = packetinit + stridelevels;
  ptrdiff_t * const packetstrides = (ptrdiff_t*)(packetcount + stridelevels);

  // setup the packet header
  memset(init,0,stridelevels*sizeof(size_t)); /* init[] = [0..0] */
  for (size_t d = 0; d < stridelevels; d++) {
    packetcount[d] = sdim[d].count; 
    packetstrides[d] = sdim[d].stride[SMD_PEER]; 
  }

  #if GASNETE_VIS_NPAM < 2
    size_t const chunksperpacket = maxpayload / chunksz;
    size_t const packetcnt = (totalchunks + chunksperpacket - 1)/chunksperpacket;

    gasneti_iop_t * const iop = gasneti_iop_register(packetcnt,0 GASNETI_THREAD_PASS);
    gasneti_assert(chunksperpacket >= 1);
  #endif

  // packetization loop
  void *srcaddr = smd->addr[SMD_SELF];
  size_t remaining = totalchunks;
  do {
    // obtain NPAM buffer, populate packet header and compute packetchunks
    #if GASNETE_VIS_NPAM < 2
      size_t const packetchunks = MIN(chunksperpacket, remaining);
    #endif
    #if GASNETE_VIS_NPAM == 0
      GASNETI_MEMCPY(packetinit, init, stridelevels*sizeof(size_t));
    #elif GASNETE_VIS_NPAM == 1
      #define min_length maxpacket
      #define max_length maxpacket
    #elif GASNETE_VIS_NPAM == 2
      #define min_length (headersz + chunksz)
      #define max_length (headersz + remaining*chunksz)
    #else
      #error unrecognized GASNETE_VIS_NPAM setting
    #endif
    #if GASNETE_VIS_NPAM
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, min_length, max_length, NULL, 0, HARGS(5,7));
      void * const packetbase = gex_AM_SrcDescAddr(sd);
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, min_length);
      GASNETI_MEMCPY(packetbase, header, headersz);
      void * const packedbuf = (uint8_t*)packetbase + headersz;
      #undef min_length
      #undef max_length
    #endif
    #if GASNETE_VIS_NPAM == 2
      size_t const packetchunks = ( gex_AM_SrcDescSize(sd) - headersz ) / chunksz;
      gasneti_iop_t * const iop = gasneti_iop_register(1,0 GASNETI_THREAD_PASS);
    #endif
    remaining -= packetchunks;

    // fill packet with data
    size_t packetsz;
    if (smd->lcontig_dims[SMD_SELF] == stridelevels) { // source is contiguous
      packetsz = packetchunks*chunksz;
      GASNETI_MEMCPY(packedbuf, srcaddr, packetsz);
      srcaddr = ((uint8_t *)srcaddr) + packetsz;
      packetsz += headersz;
      if (remaining) GASNETE_STRIDED_VECTOR_INC(init, packetchunks, SITER_SDIM_COUNT(sdim), stridelevels);
    } else { // gather data payload from source into packet
      uint8_t *pbuf = packedbuf;
      #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
        GASNETI_MEMCPY(pbuf, p1, chunksz); pbuf += chunksz; \
      } while (0)
        GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(packetchunks, init, 1, remaining);
        GASNETE_1STRIDED_HELPER(stridelevels, SITER_SDIM_COUNT(sdim),
                                     srcaddr, SITER_SDIM_STRIDE(sdim,SMD_SELF));
      #undef GASNETE_STRIDED_HELPER_LOOPBODY
      packetsz = pbuf - (uint8_t*)packetbase;
      gasneti_assert_uint(packetsz ,==, headersz + packetchunks * chunksz);
    }

    // send packet
    #if GASNETE_VIS_NPAM < 2
      gasneti_assert_uint(packetsz ,<=, maxpacket);
    #endif
    gasneti_assert_uint(stridelevels ,==, (size_t)(uint32_t)(gex_AM_Arg_t)stridelevels);
    gasneti_assert_uint(chunksz ,==,      (size_t)(uint32_t)(gex_AM_Arg_t)chunksz);
    gasneti_assert_uint(packetchunks ,==, (size_t)(uint32_t)(gex_AM_Arg_t)packetchunks);
    #define ARGS PACK(iop), PACK(dstaddr), stridelevels, chunksz, packetchunks
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_puts_AMPipeline_reqh),
                               packetbase, packetsz, GEX_EVENT_NOW, 0, ARGS);
    #else
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_puts_AMPipeline_reqh), packetsz, ARGS);
    #endif   
    #undef ARGS

  } while (remaining); // packetization loop

  #if !GASNETE_VIS_NPAM
    gasneti_free(init);
  #endif
  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_puts_AMPipeline1_reqh_inner)
void gasnete_puts_AMPipeline1_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *op, void *dstaddr, 
  gex_AM_Arg_t _stridelevels, gex_AM_Arg_t _chunksz) {
  // marshall inputs
  size_t const stridelevels = (uint32_t)_stridelevels;
  size_t const chunksz      = (uint32_t)_chunksz;
  size_t const    * const packetcount =   addr;
  ptrdiff_t const * const packetstrides = (ptrdiff_t*)(packetcount + stridelevels);
  void const      * const packedbuf =     packetstrides + stridelevels;

  // unpack data into destination
  uint8_t const * psrc = packedbuf;
  #if GASNET_DEBUG
    size_t packetchunks=1;
    for (size_t i=0; i < stridelevels; i++) packetchunks *= packetcount[i];
    gasneti_assert(packetchunks > 0);
    gasneti_assert_uint(psrc - (uint8_t*)addr + packetchunks * chunksz ,==, nbytes);
  #endif
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
     GASNETI_MEMCPY(p1, psrc, chunksz); psrc += chunksz; \
  } while (0)
    GASNETE_1STRIDED_HELPER(stridelevels, SITER_ARRAY(packetcount),
                                 dstaddr, SITER_ARRAY(packetstrides));
    gasneti_assert_uint(psrc - (uint8_t *)addr ,==, nbytes);
  #undef GASNETE_STRIDED_HELPER_LOOPBODY

  /* TODO: coalesce acknowledgements - need a per-srcnode, per-op seqnum & packetcnt */
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_puts_AMPipeline1_reph), 0, PACK(op));
}
MEDIUM_HANDLER(gasnete_puts_AMPipeline1_reqh,4,6, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1),      a2,a3),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3), a4,a5));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_puts_AMPipeline1_reph_inner)
void gasnete_puts_AMPipeline1_reph_inner(gex_Token_t token,
  void *_op) {
  if (gasneti_op_is_eop(_op)) {
    gasneti_eop_t * eop = _op; 
    gasneti_eop_markdone(eop);
  } else { 
    gasneti_iop_t * iop = _op; 
    gasneti_iop_markdone(iop, 1, 0);
  }
}
SHORT_HANDLER(gasnete_puts_AMPipeline1_reph,1,2,
              (token, UNPACK(a0)),
              (token, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_puts_AMPipeline_reqh_inner)
void gasnete_puts_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *iop, void *dstaddr, 
  gex_AM_Arg_t _stridelevels, gex_AM_Arg_t _chunksz, gex_AM_Arg_t _packetchunks) {
  // marshall inputs
  size_t const stridelevels = (uint32_t)_stridelevels;
  size_t const chunksz      = (uint32_t)_chunksz;
  size_t const packetchunks = (uint32_t)_packetchunks;
  size_t          * const packetinit =    addr;
  size_t const    * const packetcount =   packetinit + stridelevels;
  ptrdiff_t const * const packetstrides = (ptrdiff_t*)(packetcount + stridelevels);
  void const      * const packedbuf =     packetstrides + stridelevels;

  // unpack data into destination
  uint8_t const * psrc = packedbuf;
  gasneti_assert_uint(psrc - (uint8_t*)addr + packetchunks * chunksz ,==, nbytes);
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
     GASNETI_MEMCPY(p1, psrc, chunksz); psrc += chunksz; \
  } while (0)
    GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(packetchunks, packetinit, 0, 0);
    GASNETE_1STRIDED_HELPER(stridelevels, SITER_ARRAY(packetcount),
                                 dstaddr, SITER_ARRAY(packetstrides));
    gasneti_assert_uint(psrc - (uint8_t *)addr ,==, nbytes);
  #undef GASNETE_STRIDED_HELPER_LOOPBODY

  /* TODO: coalesce acknowledgements - need a per-srcnode, per-op seqnum & packetcnt */
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph), 0, PACK(iop));
}
MEDIUM_HANDLER(gasnete_puts_AMPipeline_reqh,5,7, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1),      a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3), a4,a5,a6));
#endif // GASNETE_PUTS_AMPIPELINE
#ifndef GASNETE_PUTS_AMPIPELINE_SELECTOR
  #if GASNETE_PUTS_AMPIPELINE
    #define GASNETE_PUTS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {   \
      if ((ptrdiff_t)smd->elemsz <= MIN((ptrdiff_t)gasnete_vis_put_maxchunk,           \
                                        GASNETE_PUTS_AMPIPELINE_MAXPAYLOAD(tm,rank,stridelevels)) \
         ) { gasneti_assert(gasnete_vis_use_ampipe);                                   \
      RETURN(gasnete_puts_AMPipeline(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
      } } while (0)
  #else
    #define GASNETE_PUTS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags) ((void)0)
  #endif
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter get */
#if GASNETE_USE_AMPIPELINE && !defined(GASNETE_GETS_AMPIPELINE)
#define GASNETE_GETS_AMPIPELINE 1
#define GASNETE_GETS_AMPIPELINE_REQUESTSZ(stridelevels)         ((stridelevels)*(sizeof(size_t)+sizeof(ptrdiff_t)))
#define GASNETE_GETS_AMPIPELINE_MAXREQUEST(tm,rank)             MIN(gex_AM_MaxRequestMedium((tm),(rank),GEX_EVENT_GROUP,0,HARGS(6,9)), \
                                                                    gex_AM_MaxRequestMedium((tm),(rank),GEX_EVENT_NOW,  0,HARGS(6,9)))

#define GASNETE_GETS_AMPIPELINE_MAXREPLY(tm,rank)               gex_AM_MaxReplyMedium((tm),(rank),                          \
                                                                 (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),                 \
                                                                 (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0) , \
                                                                 HARGS(2,3)) 
#define GASNETE_GETS_AMPIPELINE_REPLYOVERHEAD(stridelevels)     ((stridelevels)*sizeof(size_t))
#define GASNETE_GETS_AMPIPELINE_MAXPAYLOAD(tm,rank,stridelevels) \
       ((ptrdiff_t)GASNETE_GETS_AMPIPELINE_MAXREPLY(tm,rank) - (ptrdiff_t)GASNETE_GETS_AMPIPELINE_REPLYOVERHEAD(stridelevels))
GASNETI_NEVER_INLINE(gasnete_gets_AMPipeline,
gex_Event_t gasnete_gets_AMPipeline(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG)) {
  GASNETI_TRACE_EVENT(C, GETS_AMPIPELINE);
  gasneti_assert(smd->have_stats);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  // visop storage: (malloc)
  //   .count = stridelevels
  //   .len   = elemsz
  //   .addr  = dstaddr
  //   table self_strides[stridelevels] - copied from smd
  //   table count[stridelevels] - copied from smd
  //   Request packet data
  //
  // Request packet data:
  //   count[stridelevels] - copied from smd
  //   peer_strides[stridelevels] - copied from smd
  //
  // AM Request args:
  //   &visop, srcaddr, (uintptr_t)initchunk, (int)stridelevels, (int)elemsz, (int)packetchunks
  //
  // Packetization partitions data to fit replies into MaxReplyMedium():
  //   packetchunks complete chunks of elemsz granularity
  //   no partial chunks
  //
  // Each request handler calculates packet_init[] from initchunk,
  //   allocs MaxReplyMedium() temporary storage, 
  //   packs source data there, sends and then frees.
  //
  // AM Reply args:
  //   &visop, (int)packetchunks
  //   Payload is: packet_init[stridelevels] | packed data
  //
  // Reply handler retrieves dstaddr, elemsz, count[], self_strides[] from visop
  //   and uses it to unpack data into destination
  //   weakatomic dec-and-test on visop->packetcnt to VISOP_SIGNAL
  //
  // TODO-EX: consider growing chunksize to lcontig_sz[SMD_PEER]:
  // thus removing lcontig_dims[SMD_PEER] entries from the metadata arrays on the wire
  // and increasing the granularity of the pack operation at the peer.
  // The cost is coarser granularity in the chunking decreases the packet occupancy of the reply payload
  // Implementation would require shrinking all the stridelevels on the wire (and growing elemsz),
  // reply handler would need to copy packet_init[] to a temporary location (where?)

  size_t const stridelevels = smd->stridelevels;
  size_t const chunksz = smd->elemsz;
  size_t const totalchunks = smd->elemcnt;

  // setup visop, which is used to during AM reply processing
  size_t const visopsz = sizeof(gasneti_vis_op_t)
                         + stridelevels*sizeof(ptrdiff_t)  // table self_strides[]
                         + stridelevels*sizeof(size_t)     // table/packet count[]
                         + stridelevels*sizeof(ptrdiff_t); // packet peer_strides[]
  gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(visopsz); // TODO-EX: ideally use auxseg here to speed injection
  GASNETE_VISOP_SETUP(visop, synctype, 1);
  gasneti_eop_t *eop = visop->eop; // visop may disappear once the last AM is launched
  visop->count = stridelevels;
  visop->len = chunksz;
  visop->addr = smd->addr[SMD_SELF];
  #if GASNET_DEBUG
    visop->type = GASNETI_VIS_CAT_GETS_AMPIPELINE;
  #endif

  // setup the table and packet header
  void * const tablebase = visop + 1;
  ptrdiff_t * const self_strides = tablebase;
  size_t    * const count        = (size_t*)(self_strides + stridelevels);
  ptrdiff_t * const peer_strides = (ptrdiff_t*)(count + stridelevels);
  void      * const header = count;
  size_t const headersz = (uint8_t*)&peer_strides[stridelevels] - (uint8_t*)header;

  void * const srcaddr = smd->addr[SMD_PEER];
  for (size_t d = 0; d < stridelevels; d++) {
    count[d] = smd->dim[d].count; 
    peer_strides[d] = smd->dim[d].stride[SMD_PEER]; 
    self_strides[d] = smd->dim[d].stride[SMD_SELF]; 
  }

  gasneti_assert_uint(chunksz*totalchunks ,==, smd->totalsz);
  gasneti_assert_uint(headersz ,==, GASNETE_GETS_AMPIPELINE_REQUESTSZ(stridelevels));
  gasneti_assert_uint(headersz ,<=, GASNETE_GETS_AMPIPELINE_MAXREQUEST(tm,rank));
  ptrdiff_t const maxpayload = GASNETE_GETS_AMPIPELINE_MAXPAYLOAD(tm,rank,stridelevels);
  gasneti_assert_uint(maxpayload ,>=, chunksz);

  gasneti_assert_uint(stridelevels ,==, (size_t)(uint32_t)(gex_AM_Arg_t)stridelevels);
  gasneti_assert_uint(chunksz ,==,      (size_t)(uint32_t)(gex_AM_Arg_t)chunksz);
  if (smd->totalsz <= maxpayload) { // optimized single-packet case
    gasneti_weakatomic_set(&(visop->packetcnt), 1, GASNETI_ATOMIC_WMB_POST);

    gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_gets_AMPipeline_reqh),
                      header, headersz, GEX_EVENT_NOW, 0,
                      PACK(visop), PACK(srcaddr), PACK((uintptr_t)0), stridelevels, chunksz, totalchunks);
  } else {
    size_t const chunksperpacket = (size_t)maxpayload / chunksz;
    size_t const packetcnt = (totalchunks + chunksperpacket - 1)/chunksperpacket;
    gasneti_assert(chunksperpacket >= 1);
    gasneti_assert_uint(packetcnt ,<=, GASNETI_ATOMIC_MAX);

    gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);

    gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS); // AOP used for AM LC
    for (size_t initchunk = 0; initchunk < totalchunks; initchunk += chunksperpacket) {
      size_t const remaining = totalchunks - initchunk;
      size_t const packetchunks = MIN(chunksperpacket, remaining);
      gasneti_assert_uint(packetchunks ,==, (size_t)(uint32_t)(gex_AM_Arg_t)packetchunks);
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_gets_AMPipeline_reqh),
                      header, headersz, GEX_EVENT_GROUP, 0,
                      PACK(visop), PACK(srcaddr), PACK((uintptr_t)initchunk), stridelevels, chunksz, packetchunks);

    }
    gex_Event_t am_aop = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
    gasnete_wait(am_aop GASNETI_THREAD_PASS); // TODO-EX: could delay this until a progress function
  }
  GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
}
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_gets_AMPipeline_reqh_inner)
void gasnete_gets_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, void *srcaddr, void *_initchunk,
  gex_AM_Arg_t _stridelevels, gex_AM_Arg_t _chunksz, gex_AM_Arg_t _packetchunks) {
  // marshall inputs
  size_t const stridelevels = (uint32_t)_stridelevels;
  size_t const chunksz      = (uint32_t)_chunksz;
  size_t const packetchunks = (uint32_t)_packetchunks;
  size_t const initchunk = (size_t)(uintptr_t)_initchunk;
  size_t    * const packetcount = addr;
  ptrdiff_t * const packetstrides = (ptrdiff_t *)(packetcount + stridelevels);
  gasneti_assert_uint((uint8_t *)(packetstrides+stridelevels) - (uint8_t *)addr ,==, nbytes);

  // allocate outgoing buffer
  size_t const replysz = GASNETE_GETS_AMPIPELINE_REPLYOVERHEAD(stridelevels) +
                         packetchunks * chunksz;
  gasneti_assert(replysz > 0);
  #if GASNETE_VIS_NPAM == 0
    void * const replybase = gasnete_visbuf_malloc(replysz);
  #else
    gex_AM_SrcDesc_t sd = gex_AM_PrepareReplyMedium(token, NULL, replysz, replysz, NULL, 0, HARGS(2,3));
    void * const replybase = gex_AM_SrcDescAddr(sd);
    gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, replysz);
  #endif
  size_t  * const packetinit = replybase;
  uint8_t * const packedbuf = (uint8_t*)(packetinit + stridelevels);

  // compute starting position
  memset(packetinit, 0, stridelevels*sizeof(size_t));
  GASNETE_STRIDED_VECTOR_INC(packetinit, initchunk, SITER_ARRAY(packetcount), stridelevels);

  // gather data payload from source into packet
  uint8_t *pbuf = packedbuf;
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
     GASNETI_MEMCPY(pbuf, p1, chunksz); pbuf += chunksz; \
  } while (0)
      GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(packetchunks, packetinit, 0, 0);
      GASNETE_1STRIDED_HELPER(stridelevels, SITER_ARRAY(packetcount),
                                   srcaddr, SITER_ARRAY(packetstrides));
  #undef GASNETE_STRIDED_HELPER_LOOPBODY
  gasneti_assert_uint(pbuf - (uint8_t*)replybase ,==, replysz);

  // send packet
  #define ARGS PACK(_visop), packetchunks
  #if GASNETE_VIS_NPAM == 0
    gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_gets_AMPipeline_reph),
                             replybase, replysz, GEX_EVENT_NOW, 0, ARGS);
    gasneti_free(replybase);
  #else
    gex_AM_CommitReplyMedium(sd, gasneti_handleridx(gasnete_gets_AMPipeline_reph), replysz, ARGS);
  #endif   
  #undef ARGS
}
MEDIUM_HANDLER(gasnete_gets_AMPipeline_reqh,6,9, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1),      UNPACK(a2),     a3,a4,a5),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3), UNPACK2(a4,a5), a6,a7,a8));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_gets_AMPipeline_reph_inner)
void gasnete_gets_AMPipeline_reph_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, 
  gex_AM_Arg_t _packetchunks) {
  // marshall inputs
  gasneti_vis_op_t * const visop = _visop;
  gasneti_assert_uint(visop->type ,==, GASNETI_VIS_CAT_GETS_AMPIPELINE);
  size_t const packetchunks = (uint32_t)_packetchunks;
  void *       dstaddr =      visop->addr;
  size_t const stridelevels = visop->count;
  size_t const chunksz =      visop->len;
  void * const tablebase = visop + 1;
  ptrdiff_t * const self_strides = tablebase;
  size_t    * const count        = (size_t*)(self_strides + stridelevels);
  size_t    * const init =      addr;
  uint8_t   * const packedbuf = (uint8_t *)(init + stridelevels);

  // unpack data into destination
  uint8_t * psrc = packedbuf;
  gasneti_assert_uint(psrc - (uint8_t*)addr + packetchunks * chunksz ,==, nbytes);
  #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2) do { \
     GASNETI_MEMCPY(p1, psrc, chunksz); psrc += chunksz; \
  } while (0)
    GASNETE_STRIDED_HELPER_DECLARE_PARTIAL(packetchunks, init, 0, 0);
    GASNETE_1STRIDED_HELPER(stridelevels, SITER_ARRAY(count),
                                 dstaddr, SITER_ARRAY(self_strides));
    gasneti_assert_uint(psrc - (uint8_t *)addr ,==, nbytes);
  #undef GASNETE_STRIDED_HELPER_LOOPBODY

  if (gasneti_weakatomic_decrement_and_test(&(visop->packetcnt), 
                                            GASNETI_ATOMIC_WMB_PRE|GASNETI_ATOMIC_WEAK_FENCE)) {
    /* last response packet completes operation and cleans up */
    GASNETE_VISOP_SIGNAL(visop, 1);
    gasneti_free(visop); /* free visop, saved metadata and send buffer */
  }
}
MEDIUM_HANDLER(gasnete_gets_AMPipeline_reph,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
#endif // GASNETE_GETS_AMPIPELINE
#ifndef GASNETE_GETS_AMPIPELINE_SELECTOR
  #if GASNETE_GETS_AMPIPELINE
    #define GASNETE_GETS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {   \
      if ((ptrdiff_t)smd->elemsz <= MIN((ptrdiff_t)gasnete_vis_get_maxchunk,           \
                                        GASNETE_GETS_AMPIPELINE_MAXPAYLOAD(tm,rank,stridelevels)) && \
        GASNETT_PREDICT_TRUE(GASNETE_GETS_AMPIPELINE_REQUESTSZ(stridelevels) <= GASNETE_GETS_AMPIPELINE_MAXREQUEST(tm,rank)) \
         ) { gasneti_assert(gasnete_vis_use_ampipe);                                   \
      RETURN(gasnete_gets_AMPipeline(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
      } } while (0)
  #else
    #define GASNETE_GETS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags) ((void)0)
  #endif
#endif
/*---------------------------------------------------------------------------------*/
/* convert strided metadata to memvec metadata for the equivalent operation */
static void *gasnete_convert_strided_to_memvec(gasneti_vis_smd_t * const smd,
                                               gex_Memvec_t *memvec[2]) {
  gasneti_assert(smd && smd->have_stats);
  gasneti_assert(memvec);
  void *buf = gasnete_visbuf_malloc(sizeof(gex_Memvec_t)*(smd->lcontig_segments[0] + smd->lcontig_segments[1]));
  memvec[SMD_SELF] = buf;
  memvec[SMD_PEER] = memvec[SMD_SELF] + smd->lcontig_segments[SMD_SELF];

  size_t const selfcontigsz = smd->lcontig_sz[SMD_SELF];
  size_t const peercontigsz = smd->lcontig_sz[SMD_PEER];

  if (selfcontigsz == peercontigsz) {
    gex_Memvec_t * selfpos = memvec[SMD_SELF];
    gex_Memvec_t * peerpos = memvec[SMD_PEER];
    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  do { \
      selfpos->gex_len = selfcontigsz;                   \
      (selfpos++)->gex_addr = p1;                        \
      peerpos->gex_len = peercontigsz;                   \
      (peerpos++)->gex_addr = p2;                        \
    } while(0)
    GASNETE_SMD_STRIDED_HELPER(smd);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    gasneti_assert_ptr(selfpos ,==, memvec[SMD_SELF]+smd->lcontig_segments[SMD_SELF]);
    gasneti_assert_ptr(peerpos ,==, memvec[SMD_PEER]+smd->lcontig_segments[SMD_PEER]);
  } else {
    int const smallid = (selfcontigsz < peercontigsz ? SMD_SELF : SMD_PEER);
    size_t const smlsz = smd->lcontig_sz[smallid];
    size_t const bigsz = smd->lcontig_sz[!smallid];
    size_t const looplim = bigsz / smlsz;
    gasneti_assert_uint(looplim*smlsz ,==, bigsz);
    size_t loopcnt = 1;
    gex_Memvec_t * smlpos = memvec[smallid];
    gex_Memvec_t * bigpos = memvec[!smallid];
   
    #define SITER_SML_STRIDE(idx) smd->dim[idx].stride[smallid]         
    #define SITER_BIG_STRIDE(idx) smd->dim[idx].stride[!smallid]        

    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  do { \
        smlpos->gex_len = smlsz;                         \
        (smlpos++)->gex_addr = p1;                       \
        if (--loopcnt == 0) {                            \
          bigpos->gex_len = bigsz;                       \
          (bigpos++)->gex_addr = p2;                     \
          loopcnt = looplim;                             \
        }                                                \
      } while(0)

    GASNETE_2STRIDED_HELPER(smd->stridelevels,   SITER_SDIM_COUNT(smd->dim),
                            smd->addr[smallid],  SITER_SML_STRIDE,
                            smd->addr[!smallid], SITER_BIG_STRIDE);

    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    #undef SITER_SML_STRIDE
    #undef SITER_BIG_STRIDE
    gasneti_assert_ptr(smlpos ,==, memvec[smallid]+smd->lcontig_segments[smallid]);
    gasneti_assert_ptr(bigpos ,==, memvec[!smallid]+smd->lcontig_segments[!smallid]);
  }
  return buf;
}
/*---------------------------------------------------------------------------------*/
/* reference version that uses vector interface */
gex_Event_t gasnete_puts_ref_vector(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTS_REF_VECTOR);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  gasneti_assert(GASNETE_PUTV_ALLOWS_VOLATILE_METADATA);

  gex_Memvec_t *memvec[2];
  void * buf = gasnete_convert_strided_to_memvec(smd, memvec);

  gex_Event_t retval = gasnete_putv(synctype, tm, rank, 
                         smd->lcontig_segments[SMD_PEER], memvec[SMD_PEER],
                         smd->lcontig_segments[SMD_SELF], memvec[SMD_SELF],
                         flags GASNETI_THREAD_PASS);
  gasneti_free(buf);
  return retval; 
}
/* reference version that uses vector interface */
gex_Event_t gasnete_gets_ref_vector(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETS_REF_VECTOR);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  gasneti_assert(GASNETE_GETV_ALLOWS_VOLATILE_METADATA);

  gex_Memvec_t *memvec[2];
  void * buf = gasnete_convert_strided_to_memvec(smd, memvec);

  gex_Event_t retval = gasnete_getv(synctype, tm,
                         smd->lcontig_segments[SMD_SELF], memvec[SMD_SELF],
                         rank,
                         smd->lcontig_segments[SMD_PEER], memvec[SMD_PEER],
                         flags GASNETI_THREAD_PASS);
  gasneti_free(buf);
  return retval; 
}
/*---------------------------------------------------------------------------------*/
/* convert strided metadata to addrlist metadata for the equivalent operation */
static void *gasnete_convert_strided_to_addrlist(gasneti_vis_smd_t * const smd,
                                                 void **addrlist[2]) {
  gasneti_assert(smd && smd->have_stats);
  gasneti_assert(addrlist);
  void *buf = gasnete_visbuf_malloc(sizeof(void *)*(smd->lcontig_segments[0] + smd->lcontig_segments[1]));
  addrlist[SMD_SELF] = buf;
  addrlist[SMD_PEER] = addrlist[SMD_SELF] + smd->lcontig_segments[SMD_SELF];

  size_t const selfcontigsz = smd->lcontig_sz[SMD_SELF];
  size_t const peercontigsz = smd->lcontig_sz[SMD_PEER];

  if (selfcontigsz == peercontigsz) {
    void * * selfpos = addrlist[SMD_SELF];
    void * * peerpos = addrlist[SMD_PEER];
    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  do { \
      *(selfpos++) = p1;                                 \
      *(peerpos++) = p2;                                 \
    } while(0)
    GASNETE_SMD_STRIDED_HELPER(smd);
    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    gasneti_assert_ptr(selfpos ,==, addrlist[SMD_SELF]+smd->lcontig_segments[SMD_SELF]);
    gasneti_assert_ptr(peerpos ,==, addrlist[SMD_PEER]+smd->lcontig_segments[SMD_PEER]);
  } else {
    int const smallid = (selfcontigsz < peercontigsz ? SMD_SELF : SMD_PEER);
    size_t const looplim = smd->lcontig_sz[!smallid] / smd->lcontig_sz[smallid];
    gasneti_assert_uint(looplim*smd->lcontig_sz[smallid] ,==, smd->lcontig_sz[!smallid]);
    size_t loopcnt = 1;
    void * * smlpos = addrlist[smallid];
    void * * bigpos = addrlist[!smallid];
   
    #define SITER_SML_STRIDE(idx) smd->dim[idx].stride[smallid]         
    #define SITER_BIG_STRIDE(idx) smd->dim[idx].stride[!smallid]        

    #define GASNETE_STRIDED_HELPER_LOOPBODY(p1,p2)  do {   \
        *(smlpos++) = p1;                                  \
        if (--loopcnt == 0) {                              \
          *(bigpos++) = p2;                                \
          loopcnt = looplim;                               \
        }                                                  \
      } while(0)

    GASNETE_2STRIDED_HELPER(smd->stridelevels,   SITER_SDIM_COUNT(smd->dim),
                            smd->addr[smallid],  SITER_SML_STRIDE,
                            smd->addr[!smallid], SITER_BIG_STRIDE);

    #undef GASNETE_STRIDED_HELPER_LOOPBODY
    #undef SITER_SML_STRIDE
    #undef SITER_BIG_STRIDE
    gasneti_assert_ptr(smlpos ,==, addrlist[smallid]+smd->lcontig_segments[smallid]);
    gasneti_assert_ptr(bigpos ,==, addrlist[!smallid]+smd->lcontig_segments[!smallid]);
  }
  return buf;
}
/*---------------------------------------------------------------------------------*/
/* reference version that uses indexed interface */
gex_Event_t gasnete_puts_ref_indexed(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTS_REF_INDEXED);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  gasneti_assert(GASNETE_PUTI_ALLOWS_VOLATILE_METADATA);

  void **addrlist[2];
  void * buf = gasnete_convert_strided_to_addrlist(smd, addrlist);

  gex_Event_t retval = gasnete_puti(synctype, tm, rank, 
                         smd->lcontig_segments[SMD_PEER], addrlist[SMD_PEER], smd->lcontig_sz[SMD_PEER],
                         smd->lcontig_segments[SMD_SELF], addrlist[SMD_SELF], smd->lcontig_sz[SMD_SELF],
                         flags GASNETI_THREAD_PASS);
  gasneti_free(buf);
  return retval; 
}
/* reference version that uses indexed interface */
gex_Event_t gasnete_gets_ref_indexed(gasneti_vis_smd_t * const smd,
                                   gasnete_synctype_t const synctype, 
                                   gex_TM_t const tm, gex_Rank_t const rank, 
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETS_REF_INDEXED);
  gasneti_assert(smd->elemsz > 0);
  gasneti_assert(smd->stridelevels > 0);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));

  gasneti_assert(GASNETE_GETI_ALLOWS_VOLATILE_METADATA);

  void **addrlist[2];
  void * buf = gasnete_convert_strided_to_addrlist(smd, addrlist);

  gex_Event_t retval = gasnete_geti(synctype, tm, 
                         smd->lcontig_segments[SMD_SELF], addrlist[SMD_SELF], smd->lcontig_sz[SMD_SELF],
                         rank, 
                         smd->lcontig_segments[SMD_PEER], addrlist[SMD_PEER], smd->lcontig_sz[SMD_PEER],
                         flags GASNETI_THREAD_PASS);
  gasneti_free(buf);
  return retval; 
}

/*---------------------------------------------------------------------------------*/
// returns the size of the bounding box containing all the elements in memory, and optionally the baseptr
GASNETI_INLINE(gasnete_smd_querybounds)
size_t gasnete_smd_querybounds(gasneti_vis_smd_t const *smd, int rside, const void **baseptr) {
  uint8_t *lo = (uint8_t*)smd->addr[rside]; // low and high element addresses
  uint8_t *hi = lo;
  for (size_t d = 0; d < smd->stridelevels; d++) {
     ptrdiff_t const stride = smd->dim[d].stride[rside];
     size_t const count = smd->dim[d].count;     
     if (stride >= 0) hi += stride * (count - 1);
     else             lo += stride * (count - 1);
  }
  if (baseptr) *baseptr = lo;
  return hi - lo + smd->elemsz; // adjust for length of last element
}

// Allocate a strided op and perform metadata normalization
// returns 0 for degenerate empty operation
GASNETI_INLINE(gasnete_build_smd)
int gasnete_build_smd(
    gasneti_vis_smd_t * const smd,
    int isput, // for tracing
    void * const selfaddr, const ptrdiff_t selfstrides[],
    void * const peeraddr, const ptrdiff_t peerstrides[],
    const size_t elemsz, const size_t count[], const size_t stridelevels
  ) {
  gasneti_assert(elemsz > 0);
  gasneti_assert(stridelevels > 0);
  gasneti_assert(selfstrides && peerstrides && count);
  gasneti_assert(selfaddr && peeraddr);

  smd->elemsz = elemsz;
  smd->addr[SMD_SELF] = selfaddr;
  smd->addr[SMD_PEER] = peeraddr;

  // SMD normalization pass 1: 
  // ------------------------
  // populate SMD
  // handle empty count degeneracy
  // remove null dimensions
  // perform stride inversion to normalize peer stride
  size_t opt_stridelevels = stridelevels;
  gasneti_vis_smd_dim_t *outdim = &(smd->dim[0]);
  ptrdiff_t last_peerstride = -1;
  int is_sorted = 1;
  for (size_t d = 0; d < stridelevels; d++) {
    size_t const cnt = count[d];
    if_pf (cnt == 0) { // degenerate no-op
      return 0;
    } else if_pf (cnt == 1) { // null dimension
      GASNETI_TRACE_PRINTF(D,("STRIDED_OPT: Null dimension removed"));
      opt_stridelevels--; // remove it
    } else {
      outdim->count = cnt;
      ptrdiff_t selfstride = selfstrides[d];
      ptrdiff_t peerstride = peerstrides[d];
      if (peerstride < 0) { // invert stride
        GASNETI_TRACE_PRINTF(D,("STRIDED_OPT: Stride inversion"));
        smd->addr[SMD_SELF] = (uint8_t*)smd->addr[SMD_SELF] + selfstride * (cnt-1);
        smd->addr[SMD_PEER] = (uint8_t*)smd->addr[SMD_PEER] + peerstride * (cnt-1);
        peerstride = -peerstride;
        selfstride = -selfstride;
      }
      outdim->stride[SMD_SELF] = selfstride;
      outdim->stride[SMD_PEER] = peerstride;
      is_sorted &= (peerstride >= last_peerstride);
      last_peerstride = peerstride;
      outdim++;
    }
  }
  smd->stridelevels = opt_stridelevels;

  if_pf (opt_stridelevels == 0) goto out; // degenerate contiguity

  // SMD normalization pass 2
  // ------------------------
  // sort dimensions by ascending peer stride
  // use a simple N^2 in-place selection sort, expected to perform best for small stridelevels
  // TODO-EX: Replace with an NlogN library sort for larger stridelevels
  if (!is_sorted) {
    for (size_t i=0; i < opt_stridelevels-1; i++) {
      ptrdiff_t minstride = smd->dim[i].stride[SMD_PEER];
      size_t mindim = i;
      for (size_t j=i+1; j < opt_stridelevels; j++) { 
        ptrdiff_t const stride = smd->dim[j].stride[SMD_PEER];
        if (stride < minstride) { minstride = stride; mindim = j; }
      }
      if (mindim != i) { // swap
        gasneti_vis_smd_dim_t const tmp = smd->dim[i]; 
        smd->dim[i] = smd->dim[mindim];
        smd->dim[mindim] = tmp;
      }
    }
    GASNETI_TRACE_PRINTF(D,("STRIDED_OPT: Stride sort"));
  }

  // SMD normalization pass 3
  // ------------------------
  // Fold trailing dual-contiguous dimensions into elemsz
  {
    size_t opt_elemsz = elemsz;
    gasneti_vis_smd_dim_t *indim = &(smd->dim[0]);
    gasneti_vis_smd_dim_t *outdim = indim;
    size_t const in_stridelevels = opt_stridelevels;
    size_t d;
    for (d = 0; d < in_stridelevels; d++) {
      if ( indim->stride[SMD_SELF] == opt_elemsz &&
           indim->stride[SMD_PEER] == opt_elemsz) {
        GASNETI_TRACE_PRINTF(D,("STRIDED_OPT: Dualcontig dimension folded into elemsz"));
        opt_elemsz *= indim->count;
        opt_stridelevels--;
        indim++;
      } else break;
    }
    if (indim != outdim) {
      for ( ; d < in_stridelevels; d++) {
        *outdim++ = *indim++;
      }
      gasneti_assert_ptr(outdim ,==, &(smd->dim[opt_stridelevels]));
      smd->elemsz = opt_elemsz;
      smd->stridelevels = opt_stridelevels;
    } else gasneti_assert_uint(opt_stridelevels ,==, in_stridelevels);
  }

  if_pf (opt_stridelevels == 0) goto out; // degenerate contiguity

  // SMD normalization pass 4
  // ------------------------
  // Perform dimensional folding to combine trivial inner dimensions
  {
    gasneti_vis_smd_dim_t *lodim = &(smd->dim[0]);
    gasneti_vis_smd_dim_t *hidim = lodim + 1;
    size_t const in_stridelevels = opt_stridelevels;
    for (size_t d = in_stridelevels-1; d; d--) {
      size_t const locnt = lodim->count;
      if ( hidim->stride[SMD_SELF] == lodim->stride[SMD_SELF] * locnt &&
           hidim->stride[SMD_PEER] == lodim->stride[SMD_PEER] * locnt ) {
        GASNETI_TRACE_PRINTF(D,("STRIDED_OPT: Combined trivial inner dimension"));
        lodim->count = locnt * hidim->count;
        opt_stridelevels--;
      } else {
        lodim++;
        if (lodim != hidim) {
          *lodim = *hidim;
        }
      }
      hidim++;
    }
    gasneti_assert_ptr(lodim ,==, &(smd->dim[opt_stridelevels-1]));
    if (opt_stridelevels != in_stridelevels) smd->stridelevels = opt_stridelevels;
  }

out:
  #if GASNET_DEBUG || GASNET_TRACE  // trace and sanity check the resulting smd
  {
    gasneti_assert_uint(opt_stridelevels ,==, smd->stridelevels);
    size_t const opt_elemsz = smd->elemsz;
    ptrdiff_t *scratch = (ptrdiff_t *)&smd->dim[stridelevels];
    ptrdiff_t * const opt_strides[2] = { scratch, scratch + opt_stridelevels };
    size_t * const opt_count = (size_t*)(opt_strides[1] + opt_stridelevels);
    int change = (opt_elemsz != elemsz) || (opt_stridelevels != stridelevels) ||
                 (smd->addr[SMD_SELF] != selfaddr) ||
                 (smd->addr[SMD_PEER] != peeraddr);
    gasneti_vis_smd_dim_t const * dim = &(smd->dim[0]);
    for (size_t d = 0; d < opt_stridelevels; d++) {
      opt_strides[SMD_PEER][d] = dim->stride[SMD_PEER];
      opt_strides[SMD_SELF][d] = dim->stride[SMD_SELF];
      opt_count[d] = dim->count;
      change |= (opt_strides[SMD_PEER][d] != peerstrides[d]);
      change |= (opt_strides[SMD_SELF][d] != selfstrides[d]);
      change |= (opt_count[d] != count[d]);
      dim++;
    }
    #if GASNET_TRACE
    if (change && GASNETI_TRACE_ENABLED(D)) { // trace the optimized metadata
      char *str = gasneti_malloc(gasneti_format_putsgets_bufsz(opt_stridelevels));
      int srcid = (isput?SMD_SELF:SMD_PEER);
      gasneti_format_putsgets(str,NULL,NULL,(gex_Rank_t)-1,
                              smd->addr[!srcid],opt_strides[!srcid],
                              smd->addr[srcid],opt_strides[srcid],
                              opt_elemsz,opt_count,opt_stridelevels);
      GASNETI_TRACE_PRINTF(D,("%s: %s",(isput?"PUTS_OPT":"GETS_OPT"),str));
      gasneti_free(str);
    }
    #endif
    #if GASNET_DEBUG
    { // sanity check our strided metadata optimizations
      // data size
      size_t const opt_totalsz = gasnete_strided_datasize(opt_elemsz, opt_count, opt_stridelevels);
      size_t const totalsz = gasnete_strided_datasize(elemsz, count, stridelevels);
      gasneti_assert_uint(opt_totalsz ,==, totalsz);
      // self bounds
      void const *selfbase = selfaddr; 
      size_t const selflen = gasnete_strided_bounds(selfstrides, elemsz, count, stridelevels, &selfbase);
      void const *opt_selfbase; 
      size_t const opt_selflen = gasnete_smd_querybounds(smd, SMD_SELF, &opt_selfbase);
      gasneti_assert_uint(selflen ,==, opt_selflen);
      gasneti_assert_ptr(selfbase ,==, opt_selfbase);
      // peer bounds
      void const *peerbase = peeraddr; 
      size_t const peerlen = gasnete_strided_bounds(peerstrides, elemsz, count, stridelevels, &peerbase);
      void const *opt_peerbase; 
      size_t const opt_peerlen = gasnete_smd_querybounds(smd, SMD_PEER, &opt_peerbase);
      gasneti_assert_uint(peerlen ,==, opt_peerlen);
      gasneti_assert_ptr(peerbase ,==, opt_peerbase);

      smd->have_stats = 0;
    }
    #endif
  }
  #endif // DEBUG || TRACE

  return 1;
}
/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_analyze_smd)
void gasnete_analyze_smd(gasneti_vis_smd_t * const smd,
                         int isput) { // for tracing
  gasneti_assert(!smd->have_stats);
  gasneti_assert(smd->stridelevels > 0 && smd->elemsz > 0);
  size_t const stridelevels = smd->stridelevels;
  size_t const elemsz = smd->elemsz;

  size_t elemcnt = 1;
  size_t lcontigsz = elemsz;
  int brkSMD_SELF = 0, brkSMD_PEER = 0;

  gasneti_vis_smd_dim_t const * dim = smd->dim;
  for (size_t d = 0; d < stridelevels; d++) {
    size_t const cnt = dim->count;

    #define DIMANAL(side)                                     \
      if (brk##side) {                                        \
        smd->lcontig_segments[side] *= cnt;                   \
      } else {                                                \
        if (dim->stride[side] == (ptrdiff_t)lcontigsz) {      \
          /* linear contiguous dimension */                   \
        } else {                                              \
          brk##side = 1;                                      \
          smd->lcontig_dims[side] = d;                        \
          smd->lcontig_sz[side] = lcontigsz;                  \
          smd->lcontig_segments[side] = cnt;                  \
        }                                                     \
      }
    DIMANAL(SMD_SELF); DIMANAL(SMD_PEER);
    #undef DIMANAL

    elemcnt *= cnt; 
    lcontigsz *= cnt;
    dim++;
  }
  smd->totalsz = lcontigsz;
  smd->elemcnt = elemcnt;
  #define DIMTAIL(side)                                      \
    if (!brk##side) { /* fully lcontig */                    \
      smd->lcontig_dims[side] = stridelevels;                \
      smd->lcontig_sz[side] = lcontigsz;                     \
      smd->lcontig_segments[side] = 1;                       \
    }
  DIMTAIL(SMD_SELF); DIMTAIL(SMD_PEER);
  #undef DIMTAIL
  #if GASNET_DEBUG
    smd->have_stats = 1;
  #endif
  if (GASNETI_TRACE_ENABLED(D)) {
    int srcid = (isput ? SMD_SELF : SMD_PEER);
    GASNETI_TRACE_PRINTF(D,("%s: totalsz=%"PRIuSZ" elemcnt=%"PRIuSZ"\n"
                            "dst: contiguity=%"PRIuSZ" contigsz=%"PRIuSZ" contigsegments=%"PRIuSZ"\n"
                            "src: contiguity=%"PRIuSZ" contigsz=%"PRIuSZ" contigsegments=%"PRIuSZ,
                            (isput?"PUTS_OPT":"GETS_OPT"), smd->totalsz, smd->elemcnt,
                            smd->lcontig_dims[!srcid], smd->lcontig_sz[!srcid], smd->lcontig_segments[!srcid],
                            smd->lcontig_dims[srcid],  smd->lcontig_sz[srcid],  smd->lcontig_segments[srcid]
                       ));
  }
  gasneti_assert_uint(smd->totalsz ,==, smd->elemsz * smd->elemcnt);
  #define DIMASSERT(side)                                    \
    gasneti_assert_uint(smd->lcontig_dims[side] ,<=, stridelevels); \
    gasneti_assert_uint(smd->lcontig_segments[side] * smd->lcontig_sz[side] ,==, smd->totalsz);
  DIMASSERT(SMD_SELF); DIMASSERT(SMD_PEER);
  #undef DIMASSERT
  // normalization properties of the stride optimizer:
  gasneti_assert_uint(elemsz ,==, MIN(smd->lcontig_sz[SMD_SELF],smd->lcontig_sz[SMD_PEER]));
  gasneti_assert_uint(MIN(smd->lcontig_dims[SMD_SELF],smd->lcontig_dims[SMD_PEER]) ,==, 0);
}
/*---------------------------------------------------------------------------------*/
/* top-level gasnet_puts_* entry point */
#ifndef GASNETE_PUTS_OVERRIDE
extern gex_Event_t gasnete_puts(gasnete_synctype_t synctype,
                                   gex_TM_t tm, gex_Rank_t rank,
                                   void *dstaddr, const ptrdiff_t dststrides[],
                                   void *srcaddr, const ptrdiff_t srcstrides[],
                                   size_t elemsz, const size_t count[], size_t stridelevels,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(elemsz > 0); // this degenerate case handled in public header
  gasneti_assert(stridelevels > 0); // this degenerate case handled in public header
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  union {
    gasneti_vis_smd_t _smd;
    char _buf[SMD_SZ(GASNETE_DIRECT_DIMS)];
  } _smd_buf;
  gasneti_vis_smd_t * const smd = (GASNETT_PREDICT_TRUE(stridelevels <= GASNETE_DIRECT_DIMS) ? 
                                   &_smd_buf._smd : gasneti_malloc(SMD_SZ(stridelevels)));
  #define RETURN(expr) do {                                       \
    gex_Event_t _result = (expr);                                 \
    if_pf (stridelevels > GASNETE_DIRECT_DIMS) gasneti_free(smd); \
    return _result;                                               \
  } while (0)
  
  int ret = gasnete_build_smd(smd, 1, srcaddr, srcstrides, dstaddr, dststrides, 
                             elemsz, count, stridelevels);
  if_pf (!ret) { // degenerate count[i] == 0, for some i
    GASNETI_TRACE_EVENT(C, PUTS_DEGENERATE);
    RETURN(GEX_EVENT_INVALID);
  }

  #if GASNET_DEBUG // Bounds check - currently only handle SEG_BOUND
    if (!(flags & (GEX_FLAG_PEER_SEG_UNKNOWN|GEX_FLAG_PEER_SEG_SOME))) {
      const void *base; 
      size_t len = gasnete_smd_querybounds(smd, SMD_PEER, &base);
      gasneti_boundscheck(tm, rank, base, len);
    }
  #endif
  
  void *peeraddr;
  if_pf (smd->stridelevels == 0) {
    // folded to fully contiguous
    GASNETI_TRACE_EVENT(C, PUTS_DEGENERATE);
    gex_Event_t result;
    GASNETE_PUT_DEGEN(result, synctype, tm, rank, smd->addr[SMD_PEER], smd->addr[SMD_SELF], smd->elemsz, flags);
    RETURN(result);
  } else if ((peeraddr = GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(tm, rank, smd->addr[SMD_PEER]))) {
    // shared memory - use shared-memory bypass
    GASNETI_TRACE_EVENT(C, PUTS_NBRHD);
    gasnete_strided_memcpy(peeraddr, smd->addr[SMD_SELF], smd->stridelevels, smd->elemsz, smd->dim, SMD_SELF);
    RETURN(GEX_EVENT_INVALID);
  } 
  // select and dispatch a network algorithm
  #if GASNETE_USE_AMPIPELINE
    #define GASNETE_PUTS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {     \
      if (smd->elemsz > gasnete_vis_put_maxchunk)                                       \
        RETURN(gasnete_puts_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
    } while (0)
  #else
    #define GASNETE_PUTS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags) ((void)0)
  #endif

  /* select algorithm */
  #ifndef GASNETE_PUTS_SELECTOR
    #if GASNET_DEBUG && GASNETE_RANDOM_SELECTOR // for testing purposes only
      #define GASNETE_PUTS_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {                    \
        gasnete_analyze_smd(smd, 1);                                                           \
        switch (rand() % 5) {                                                                  \
          case 0:                                                                              \
            /*GASNETE_PUTS_GATHER_SELECTOR(RETURN,smd,synctype,tm,rank,flags);*/               \
          case 1:                                                                              \
            GASNETE_PUTS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags);               \
          case 2:                                                                              \
            RETURN(gasnete_puts_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));    \
          case 3:                                                                              \
            RETURN(gasnete_puts_ref_vector(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));   \
          case 4:                                                                              \
            RETURN(gasnete_puts_ref_indexed(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));  \
          default: gasneti_unreachable();                                                      \
        } } while (0)
    #else // normal algorithm selection
      #define GASNETE_PUTS_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {             \
        GASNETE_PUTS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags);             \
        gasnete_analyze_smd(smd, 1); /* not needed for ref-indiv */                     \
        /*GASNETE_PUTS_GATHER_SELECTOR(RETURN,smd,synctype,tm,rank,flags);*/            \
        GASNETE_PUTS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags);            \
        RETURN(gasnete_puts_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
      } while (0)
    #endif
  #endif
  GASNETE_PUTS_SELECTOR(RETURN,smd,synctype,tm,rank,flags);
  gasneti_unreachable();
  #undef RETURN
}
#endif
/* top-level gasnet_gets_* entry point */
#ifndef GASNETE_GETS_OVERRIDE
extern gex_Event_t gasnete_gets(gasnete_synctype_t synctype,
                                   gex_TM_t tm,
                                   void *dstaddr, const ptrdiff_t dststrides[],
                                   gex_Rank_t rank,
                                   void *srcaddr, const ptrdiff_t srcstrides[],
                                   size_t elemsz, const size_t count[], size_t stridelevels,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(elemsz > 0); // this degenerate case handled in public header
  gasneti_assert(stridelevels > 0); // this degenerate case handled in public header
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  union {
    gasneti_vis_smd_t _smd;
    char _buf[SMD_SZ(GASNETE_DIRECT_DIMS)];
  } _smd_buf;
  gasneti_vis_smd_t * const smd = (GASNETT_PREDICT_TRUE(stridelevels <= GASNETE_DIRECT_DIMS) ? 
                                   &_smd_buf._smd : gasneti_malloc(SMD_SZ(stridelevels)));
  #define RETURN(expr) do {                                       \
    gex_Event_t _result = (expr);                                 \
    if_pf (stridelevels > GASNETE_DIRECT_DIMS) gasneti_free(smd); \
    return _result;                                               \
  } while (0)
  
  int ret = gasnete_build_smd(smd, 0, dstaddr, dststrides, srcaddr, srcstrides, 
                             elemsz, count, stridelevels);
  if_pf (!ret) { // degenerate count[i] == 0, for some i
    GASNETI_TRACE_EVENT(C, PUTS_DEGENERATE);
    RETURN(GEX_EVENT_INVALID);
  }

  #if GASNET_DEBUG // Bounds check - currently only handle SEG_BOUND
    if (!(flags & (GEX_FLAG_PEER_SEG_UNKNOWN|GEX_FLAG_PEER_SEG_SOME))) {
      const void *base;
      size_t len = gasnete_smd_querybounds(smd, SMD_PEER, &base);
      gasneti_boundscheck(tm, rank, base, len);
    }
  #endif

  void *peeraddr;
  if_pf (smd->stridelevels == 0) {
    // folded to fully contiguous
    GASNETI_TRACE_EVENT(C, GETS_DEGENERATE);
    gex_Event_t result;
    GASNETE_GET_DEGEN(result, synctype, tm, smd->addr[SMD_SELF], rank, smd->addr[SMD_PEER], smd->elemsz, flags);
    RETURN(result);
  } else if ((peeraddr = GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(tm, rank, smd->addr[SMD_PEER]))) {
    // shared memory - use shared-memory bypass
    GASNETI_TRACE_EVENT(C, GETS_NBRHD);
    gasnete_strided_memcpy(smd->addr[SMD_SELF], peeraddr, smd->stridelevels, smd->elemsz, smd->dim, SMD_PEER);
    RETURN(GEX_EVENT_INVALID);
  }
  // select and dispatch a network algorithm
  #if GASNETE_USE_AMPIPELINE
    #define GASNETE_GETS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {     \
      if (smd->elemsz > gasnete_vis_get_maxchunk)                                       \
        RETURN(gasnete_gets_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
    } while (0)
  #else
    #define GASNETE_GETS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags) ((void)0)
  #endif

  /* select algorithm */
  #ifndef GASNETE_GETS_SELECTOR
    #if GASNET_DEBUG && GASNETE_RANDOM_SELECTOR // for testing purposes only
      #define GASNETE_GETS_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {                    \
        gasnete_analyze_smd(smd, 1);                                                           \
        switch (rand() % 5) {                                                                  \
          case 0:                                                                              \
            /*GASNETE_GETS_SCATTER_SELECTOR(RETURN,smd,synctype,tm,rank,flags);*/              \
          case 1:                                                                              \
            GASNETE_GETS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags);               \
          case 2:                                                                              \
            RETURN(gasnete_gets_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));    \
          case 3:                                                                              \
            RETURN(gasnete_gets_ref_vector(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));   \
          case 4:                                                                              \
            RETURN(gasnete_gets_ref_indexed(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS));  \
          default: gasneti_unreachable();                                                      \
        } } while (0)
    #else // normal algorithm selection
      #define GASNETE_GETS_SELECTOR(RETURN,smd,synctype,tm,rank,flags) do {             \
        GASNETE_GETS_REF_INDIV_SELECTOR(RETURN,smd,synctype,tm,rank,flags);             \
        gasnete_analyze_smd(smd, 1); /* not needed for ref-indiv */                     \
        /*GASNETE_GETS_SCATTER_SELECTOR(RETURN,smd,synctype,tm,rank,flags);*/           \
        GASNETE_GETS_AMPIPELINE_SELECTOR(RETURN,smd,synctype,tm,rank,flags);            \
        RETURN(gasnete_gets_ref_indiv(smd,synctype,tm,rank,flags GASNETI_THREAD_PASS)); \
      } while (0)
    #endif
  #endif
  GASNETE_GETS_SELECTOR(RETURN,smd,synctype,tm,rank,flags);
  gasneti_unreachable();
  #undef RETURN
}
#endif
/*---------------------------------------------------------------------------------*/
#if PLATFORM_COMPILER_CLANG && PLATFORM_COMPILER_VERSION_GE(2,8,0)
  #pragma clang diagnostic pop
#endif

