/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_help.h $
 * Description: GASNet Extended API Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_HELP_H
#define _GASNET_EXTENDED_HELP_H

#include <gasnet_help.h>

/* gasnete_islocal() is used by put/get fns to decide whether shared memory on 
   a given rank is "local". By default this is based on comparing the jobrank to
   the local one, but clients can override this to remove the check overhead
   by defining either GASNETE_PUTGET_ALWAYSLOCAL or GASNETE_PUTGET_ALWAYSREMOTE
 */
// TODO-EX: REMOVE THE GASNETE_PUTGET_ALWAYS* DEFINES ENTIRELY
#if GASNET_CONDUIT_SMP
  #if GASNET_PSHM // smp w/pshm: the PSHM support handles smp loopback
    #define gasnete_islocal(e_tm,rank) (gasneti_check_tm_rank(e_tm,rank),0)
  #else           // smp nopshm: single-process loopback handled in header
    #define gasnete_islocal(e_tm,rank) (gasneti_assert(gasneti_e_tm_rank_to_jobrank(e_tm,rank) == 0),1)
  #endif
#elif defined(GASNETE_PUTGET_ALWAYSLOCAL)  // always local
  #define gasnete_islocal(e_tm,rank) (gasneti_assert(gasneti_e_tm_rank_to_jobrank(e_tm,rank) == gasneti_mynode),1)
#elif defined(GASNETE_PUTGET_ALWAYSREMOTE) // always remote
  #define gasnete_islocal(e_tm,rank) (gasneti_assert(gasneti_e_tm_rank_to_jobrank(e_tm,rank) != gasneti_mynode),0)
#else // general case
  #define gasnete_islocal(e_tm,rank) (gasneti_e_tm_rank_to_jobrank(e_tm,rank) == gasneti_mynode)
#endif

/* ------------------------------------------------------------------------------------ */
// TODO-EX: Eliminate GASNETE_FAST_ALIGNED_MEMCPY (which no longer has any internal
// callers) and redirect the tools interface elsewhere.  This should include removal
// of GASNETI_BUG1389_WORKAROUND and many corresponding bits (configure option, config
// string, interaction w/ GASNETI_MAY_ALIAS, etc.)

/* bug 1389: need to prevent bad optimizations on GASNETE_FAST_ALIGNED_MEMCPY due to
   ansi-aliasing rules added in C99 that foolishly outlaw type-punning. 
   Exploit a union of all possible base types of the given size as a loophole in the rules.
   Other options include forcing gasneti_compiler_fence before&after the type-punning,
   globally disabling ansi aliasing using compiler flags, or redundantly copying the 
   first byte of the value as a (char *) (last is not guaranteed to work)

   See also instances of GASNETI_BUG1389_WORKAROUND, which enable a very conservative
   approach using memcpy() and compiler fences.  This is to allow us to determine if
   a given failure may be related to a recurrance of bug 1389.
 */
typedef union {
  uint8_t _gasnete_anytype_u8; /* might be a compiler builtin type */
  #if (SIZEOF__BOOL == 1) && defined(GASNETI_COMPILER_IS_CC)
    _Bool _gasnete_anytype_b;
  #endif
  #if SIZEOF_CHAR == 1
    char _gasnete_anytype_c;
  #endif
  #if SIZEOF_SHORT == 1
    short _gasnete_anytype_s;
  #endif
} GASNETI_MAY_ALIAS gasnete_anytype8_t;

#ifndef INTTYPES_16BIT_MISSING
typedef union {
  uint16_t _gasnete_anytype_u16; /* might be a compiler builtin type */
  gasnete_anytype8_t _gasnete_anytype_at8; /* necessary for structs of two 8-bit types */
  #if (SIZEOF__BOOL == 2) && defined(GASNETI_COMPILER_IS_CC)
    _Bool _gasnete_anytype_b;
  #endif
  #if SIZEOF_SHORT == 2
    short _gasnete_anytype_s;
  #endif
  #if SIZEOF_INT == 2
    int _gasnete_anytype_i;
  #endif
} GASNETI_MAY_ALIAS gasnete_anytype16_t;
#endif

typedef union {
  uint32_t _gasnete_anytype_u32; /* might be a compiler builtin type */
  #ifndef INTTYPES_16BIT_MISSING
    gasnete_anytype16_t _gasnete_anytype_at16; /* necessary for structs of two 16-bit types */
  #endif
  #if (SIZEOF__BOOL == 4) && defined(GASNETI_COMPILER_IS_CC)
    _Bool _gasnete_anytype_b;
  #endif
  #if SIZEOF_SHORT == 4
    short _gasnete_anytype_s;
  #endif
  #if SIZEOF_INT == 4
    int _gasnete_anytype_i;
  #endif
  #if SIZEOF_LONG == 4
    long _gasnete_anytype_l;
  #endif
  #if SIZEOF_FLOAT == 4
    float _gasnete_anytype_f;
  #endif
  #if SIZEOF_VOID_P == 4
    void *_gasnete_anytype_p;
    intptr_t _gasnete_anytype_ip; /* might be a compiler builtin type */
  #endif
} GASNETI_MAY_ALIAS gasnete_anytype32_t;

typedef union {
  uint64_t _gasnete_anytype_u64; /* might be a compiler builtin type */
  gasnete_anytype32_t _gasnete_anytype_at32; /* necessary for structs of two 32-bit types */
  #if (SIZEOF__BOOL == 8) && defined(GASNETI_COMPILER_IS_CC)
    _Bool _gasnete_anytype_b;
  #endif
  #if SIZEOF_INT == 8
    int _gasnete_anytype_i;
  #endif
  #if SIZEOF_LONG == 8
    long _gasnete_anytype_l;
  #endif
  #if SIZEOF_LONG_LONG == 8
    long long _gasnete_anytype_ll;
  #endif
  #if SIZEOF_DOUBLE == 8
    double _gasnete_anytype_d;
  #endif
  #if SIZEOF_LONG_DOUBLE == 8
    long double _gasnete_anytype_ld;
  #endif
  #if (SIZEOF_FLOAT__COMPLEX == 8) && defined(GASNETI_COMPILER_IS_CC)
    float _Complex _gasnete_anytype_fc;
  #endif
  #if SIZEOF_VOID_P == 8
    void *_gasnete_anytype_p;
    intptr_t _gasnete_anytype_ip; /* might be a compiler builtin type */
  #endif
} GASNETI_MAY_ALIAS gasnete_anytype64_t;

#if INTTYPES_16BIT_MISSING  /* deal with Cray's annoying lack of 16-bit types on some platforms */
  #define GASNETE_OMIT_WHEN_MISSING_16BIT(code) 
#else
  #define GASNETE_OMIT_WHEN_MISSING_16BIT(code) code
#endif

#if PLATFORM_ARCH_CRAYT3E /* T3E ridiculously sets the sizes of all unions above to 8 bytes */
  #define gasnete_anytype8_t  uint8_t
  #define gasnete_anytype32_t uint32_t
  #define gasnete_anytype64_t uint64_t
  #define GASNETE_ANYTYPE_LVAL(ptr,bits) (*((gasnete_anytype##bits##_t *)(ptr)))
#else
  #define GASNETE_ANYTYPE_LVAL(ptr,bits) (((gasnete_anytype##bits##_t *)(ptr))->_gasnete_anytype_u##bits)
#endif

/*  undefined results if the regions are overlapping */
#ifdef GASNETI_BUG1389_WORKAROUND
  #define GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes) do { \
    gasneti_compiler_fence();                                 \
    memcpy(dest, src, nbytes);                                \
    gasneti_compiler_fence();                                 \
  } while(0)
#else
#define GASNETE_FAST_ALIGNED_MEMCPY(dest, src, nbytes) do { \
  const void * const _fam_src = (src);                      \
  void * const _fam_dst = (dest);                           \
  size_t const _fam_nbytes = (nbytes);                      \
  switch(_fam_nbytes) {                                     \
    case 0:                                                 \
      break;                                                \
    case 1:  *((gasnete_anytype8_t *)_fam_dst) =            \
             *((gasnete_anytype8_t *)_fam_src);             \
      break;                                                \
  GASNETE_OMIT_WHEN_MISSING_16BIT(                          \
    case 2:  *((gasnete_anytype16_t *)_fam_dst) =           \
             *((gasnete_anytype16_t *)_fam_src);            \
      break;                                                \
  )                                                         \
    case 4:  *((gasnete_anytype32_t *)_fam_dst) =           \
             *((gasnete_anytype32_t *)_fam_src);            \
      break;                                                \
    case 8:  *((gasnete_anytype64_t *)_fam_dst) =           \
             *((gasnete_anytype64_t *)_fam_src);            \
      break;                                                \
    default:                                                \
      memcpy(_fam_dst, _fam_src, _fam_nbytes);              \
  }                                                         \
} while(0)
#endif /* GASNETI_BUG1389_WORKAROUND */

// TODO-EX: remove these if/when all uses are updated
#define GASNETE_FAST_UNALIGNED_MEMCPY(d,s,n)       GASNETI_MEMCPY(d,s,n)

/* given the address of a gex_RMA_Value_t object and the number of
   significant bytes, return the byte address where significant bytes begin */
#ifdef WORDS_BIGENDIAN
  #define GASNETE_STARTOFBITS(regvalptr,nbytes) \
    (((uint8_t*)(regvalptr)) + ((sizeof(gex_RMA_Value_t)-nbytes)))
#else /* little-endian */
  #define GASNETE_STARTOFBITS(regvalptr,nbytes) (regvalptr)
#endif

/* The value written to the target address is a direct byte copy of the 
   8*nbytes low-order bits of value, written with the endianness appropriate 
   for an nbytes integral value on the current architecture
   */
#ifdef GASNETI_BUG1389_WORKAROUND
  #define GASNETE_VALUE_ASSIGN(dest, value, nbytes) do {            \
    gasneti_compiler_fence();                                       \
    memcpy((dest), GASNETE_STARTOFBITS(&(value),nbytes), nbytes);   \
    gasneti_compiler_fence();                                       \
  } while(0)
#else
#define GASNETE_VALUE_ASSIGN(dest, value, nbytes) do {              \
  switch (nbytes) {                                                 \
    case 0:                                                         \
      break;                                                        \
    case 1: GASNETE_ANYTYPE_LVAL(dest,8) = (uint8_t)(value);        \
      break;                                                        \
  GASNETE_OMIT_WHEN_MISSING_16BIT(                                  \
    case 2: GASNETE_ANYTYPE_LVAL(dest,16) = (uint16_t)(value);      \
      break;                                                        \
  )                                                                 \
    case 4: GASNETE_ANYTYPE_LVAL(dest,32) = (uint32_t)(value);      \
      break;                                                        \
    case 8: GASNETE_ANYTYPE_LVAL(dest,64) = (uint64_t)(value);      \
      break;                                                        \
    default:  /* no such native nbytes integral type */             \
      memcpy((dest), GASNETE_STARTOFBITS(&(value),nbytes), nbytes); \
  }                                                                 \
  } while (0)
#endif /* GASNETI_BUG1389_WORKAROUND */

/* interpret *src as a ptr to an nbytes type,
   and return the value as a gex_RMA_Value_t */
#ifdef GASNETI_BUG1389_WORKAROUND
  #define GASNETE_VALUE_RETURN(src, nbytes) do {               \
    gex_RMA_Value_t _result = 0;                               \
    gasneti_compiler_fence();                                  \
    memcpy(GASNETE_STARTOFBITS(&_result,nbytes), src, nbytes); \
    return _result;                                            \
  } while(0)
#else
#define GASNETE_VALUE_RETURN(src, nbytes) do {                               \
    size_t __gvr_nbytes = (nbytes);                                          \
    gasneti_assert(__gvr_nbytes > 0);                                        \
    gasneti_assert_uint(__gvr_nbytes ,<=, sizeof(gex_RMA_Value_t));          \
    switch (__gvr_nbytes) {                                                  \
      case 1: return (gex_RMA_Value_t)GASNETE_ANYTYPE_LVAL(src,8);   \
    GASNETE_OMIT_WHEN_MISSING_16BIT(                                         \
      case 2: return (gex_RMA_Value_t)GASNETE_ANYTYPE_LVAL(src,16);  \
    )                                                                        \
      case 4: return (gex_RMA_Value_t)GASNETE_ANYTYPE_LVAL(src,32);  \
      case 8: return (gex_RMA_Value_t)GASNETE_ANYTYPE_LVAL(src,64);  \
      default: { /* no such native nbytes integral type */                   \
          gex_RMA_Value_t _result = 0;                                       \
          memcpy(GASNETE_STARTOFBITS(&_result,nbytes), src, __gvr_nbytes);   \
          return _result;                                                    \
      }                                                                      \
    }                                                                        \
  } while (0)
#endif /* GASNETI_BUG1389_WORKAROUND */


/* gasnete_loopback{get,put}_memsync() go after a get or put is done with both source
 * and destination on the local node.  This is only done if GASNet was configured
 * for the stricter memory consistency model.
 * The put_memsync belongs after the memory copy to ensure that writes are committed in
 * program order.
 * The get_memsync belongs after the memory copy to ensure that if the value(s) read
 * is used to predicate any subsequent reads, that the reads are done in program order.
 * Note that because gasnet_gets may read multiple words, it's possible that the 
 * values fetched in a multi-word get may reflect concurrent strict writes by other CPU's 
 * in a way that appears to violate program order, eg:
 *  CPU0: gex_RMA_PutBlockingVal(myteam,mynode,&A[0],someval,1,0);
 *        gex_RMA_PutBlockingVal(myteam,mynode,&A[1],someval,1,0);
 *  CPU1: gex_RMA_GetBlocking(myteam,dest,mynode,&A[0],someval,2,0) ; // may see updated A[1] but not A[0]
 * but there doesn't seem to be much we can do about that (adding another rmb before the
 * get does not solve the problem, because the two puts may globally complete in the middle
 * of the get's execution, after copying A[0] but before copying A[1]). It's a fundamental
 * result of the fact that multi-word gasnet put/gets are not performed atomically 
 * (and for performance reasons, cannot be).
 */
#ifdef GASNETI_MEMSYNC_ON_LOOPBACK
  #define gasnete_loopbackput_memsync() gasneti_local_wmb()
  #define gasnete_loopbackget_memsync() gasneti_local_rmb()
#else
  #define gasnete_loopbackput_memsync() 
  #define gasnete_loopbackget_memsync()
#endif


/* helper macros */
#define GASNETI_CHECKZEROSZ_GET(variety,tm,dest,rank,src,nbytes) do { \
    if_pf (nbytes == 0) {                                             \
      GASNETI_TRACE_GET_LOCAL(variety,tm,dest,rank,src,nbytes);       \
      return 0;                                                       \
    } } while(0)
#define GASNETI_CHECKZEROSZ_PUT(variety,tm,rank,dest,src,nbytes) do { \
    if_pf (nbytes == 0) {                                             \
      GASNETI_TRACE_PUT_LOCAL(variety,tm,rank,dest,src,nbytes);       \
      return 0;                                                       \
    } } while(0)
#define GASNETI_CHECKZEROSZ_NAMED(tracecall,nbytes) do {           \
    if_pf (nbytes == 0) {                                          \
      tracecall;                                                   \
      return 0;                                                    \
    } } while(0)
#if GASNET_PSHM
  #define GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes) do {                   \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      GASNETI_MEMCPY(dest, gasneti_pshm_addr2local(tm,rank,src), nbytes);       \
      gasnete_loopbackget_memsync();                                            \
      return 0;                                                                 \
    }} while(0)
  #define GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes) do {                   \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      GASNETI_MEMCPY(gasneti_pshm_addr2local(tm,rank,dest), src, nbytes);       \
      gasnete_loopbackput_memsync();                                            \
      gasneti_leaf_finish(lc_opt);                                              \
      return 0;                                                                 \
    }} while(0)
  #define GASNETI_CHECKPSHM_PUT_NOLC(tm,rank,dest,src,nbytes) do {              \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      GASNETI_MEMCPY(gasneti_pshm_addr2local(tm,rank,dest), src, nbytes);       \
      gasnete_loopbackput_memsync();                                            \
      return 0;                                                                 \
    }} while(0)
  #define GASNETI_CHECKPSHM_GETVAL(tm,rank,src,nbytes) do {                     \
    size_t const _nbytes = (nbytes);                                            \
    gasneti_assume(_nbytes > 0); /* bug 3793 */                                 \
    gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));                         \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      GASNETE_VALUE_RETURN(gasneti_pshm_addr2local(tm,rank,src), _nbytes);       \
    }} while(0)
  #define GASNETI_CHECKPSHM_PUTVAL(tm,rank,dest,value,nbytes) do {              \
    size_t const _nbytes = (nbytes);                                            \
    gasneti_assume(_nbytes > 0); /* bug 3793 */                                 \
    gasneti_assume(_nbytes <= sizeof(gex_RMA_Value_t));                         \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      GASNETE_VALUE_ASSIGN(gasneti_pshm_addr2local(tm,rank,dest), value, _nbytes); \
      gasnete_loopbackput_memsync();                                            \
      return 0;                                                                 \
    }} while(0)
  #define GASNETI_CHECKPSHM_MEMSET(tm,rank,dest,val,nbytes) do {                \
    if (gasneti_pshm_in_supernode(tm,rank)) {                                   \
      memset(gasneti_pshm_addr2local(tm,rank,dest), val, nbytes);               \
      gasnete_loopbackput_memsync();                                            \
      return 0;                                                                 \
    }} while(0)
#else
  #define GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes)      ((void)0)
  #define GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes)      ((void)0)
  #define GASNETI_CHECKPSHM_PUT_NOLC(tm,rank,dest,src,nbytes) ((void)0)
  #define GASNETI_CHECKPSHM_GETVAL(tm,rank,src,nbytes)        ((void)0)
  #define GASNETI_CHECKPSHM_PUTVAL(tm,rank,dest,value,nbytes) ((void)0)
  #define GASNETI_CHECKPSHM_MEMSET(tm,rank,dest,val,nbytes)   ((void)0)
#endif

#if GASNET_DEBUG
  #define GASNETI_CHECK_PUT_LCOPT(lc_opt, isnbi) do {                                    \
    gex_Event_t *_gcpl_lc_opt = (lc_opt);                                                \
    /* GEX_EVENT_{NOW,DEFER} always permitted */                                         \
    if_pf (_gcpl_lc_opt == NULL)                                                         \
      gasneti_fatalerror("gex_RMA_Put*(lc_opt=NULL) is invalid");                        \
    else if_pf (isnbi && gasneti_leaf_is_pointer(_gcpl_lc_opt))                          \
      gasneti_fatalerror("gex_RMA_PutNBI(lc_opt=<pointer>) is invalid (NB only)");       \
    else if_pf (!isnbi && _gcpl_lc_opt == GEX_EVENT_GROUP)                               \
      gasneti_fatalerror("gex_RMA_PutNB(lc_opt=GEX_EVENT_GROUP) is invalid (NBI only)"); \
  } while (0)
#else
  #define GASNETI_CHECK_PUT_LCOPT(lc_opt, isnbi) do { } while (0)
#endif

// GASNETI_NBRHD_* convenience macros (same semantics w/ and w/o PSHM)
//    LOCAL(e_tm,rank)                   -> non-zero iff the indicated rank is in caller's neighborhood
//    LOCAL_ADDR(e_tm,rank,addr)         -> address in caller's address space if the indicated rank is
//                                        in caller's neighborhood, and undefined otherwise.
//                                        input addr must be non-NULL
//    LOCAL_ADDR_OR_NULL(e_tm,rank,addr) -> address in caller's address space if the indicated rank is
//                                        in caller's neighborhood, and NULL otherwise.
//                                        input addr must be non-NULL
// Equivalents for callers using jobrank:
//    JOBRANK_IS_LOCAL(jobrank)
//    JOBRANK_LOCAL_ADDR(jobrank,addr)
//    JOBRANK_LOCAL_ADDR_OR_NULL(jobrank,addr)
// TODO-EX:
//   + LOCAL_ADDR might be made smarter?
//
#if GASNET_PSHM
  #define _GASNETI_NBRHD_LOCAL(e_tm,rank) gasneti_pshm_in_supernode(e_tm,rank)
  #define _GASNETI_NBRHD_LOCAL_ADDR(e_tm,rank,addr) gasneti_pshm_addr2local(e_tm,rank,addr)
  #define _GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank) gasneti_pshm_jobrank_in_supernode(jobrank)
  #define _GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,addr) gasneti_pshm_jobrank_addr2local(jobrank,addr)
#else
  #if GASNET_CONDUIT_SMP
    #define _GASNETI_NBRHD_LOCAL(e_tm,rank)             (1)
    #define _GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)    (1)
  #else 
    #define _GASNETI_NBRHD_LOCAL(e_tm,rank)             (gasneti_e_tm_rank_to_jobrank(e_tm,rank) == gasneti_mynode)
    #define _GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)    ((jobrank) == gasneti_mynode)
  #endif
  #define _GASNETI_NBRHD_LOCAL_ADDR(e_tm,rank,addr)        (addr)
  #define _GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,addr)  (addr)
#endif

GASNETI_INLINE(gasneti_nbrhd_jobrank_local_addr_or_null) GASNETI_PURE
void *gasneti_nbrhd_jobrank_local_addr_or_null(gex_Rank_t _jobrank, void *_addr) {
  return _GASNETI_NBRHD_JOBRANK_IS_LOCAL(_jobrank)
             ? _GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(_jobrank,_addr)
             : NULL;
}
GASNETI_PUREP(gasneti_nbrhd_jobrank_local_addr_or_null)

GASNETI_INLINE(gasneti_nbrhd_local_addr_or_null) GASNETI_PURE
void *gasneti_nbrhd_local_addr_or_null(gex_TM_t _e_tm, gex_Rank_t _rank, void *_addr) {
  gex_Rank_t _jobrank = gasneti_e_tm_rank_to_jobrank(_e_tm,_rank);
  return gasneti_nbrhd_jobrank_local_addr_or_null(_jobrank, _addr);
}
GASNETI_PUREP(gasneti_nbrhd_local_addr_or_null)

#define GASNETI_NBRHD_LOCAL(e_tm,rank) \
        (gasneti_assert((rank) < gex_TM_QuerySize(e_tm)), \
         _GASNETI_NBRHD_LOCAL(e_tm,rank))
#define GASNETI_NBRHD_LOCAL_ADDR(e_tm,rank,addr)\
        (gasneti_assert((rank) < gex_TM_QuerySize(e_tm)), gasneti_assert(addr), \
         _GASNETI_NBRHD_LOCAL_ADDR(e_tm,rank,addr))
#define GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(e_tm,rank,addr) \
        (gasneti_assert((rank) < gex_TM_QuerySize(e_tm)), gasneti_assert(addr), \
         gasneti_nbrhd_local_addr_or_null(e_tm,rank,addr))

#define GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)\
        (gasneti_assert((jobrank) < gasneti_nodes), \
         _GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank))
#define GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,addr)\
        (gasneti_assert((jobrank) < gasneti_nodes), gasneti_assert(addr), \
         _GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,addr))
#define GASNETI_NBRHD_JOBRANK_LOCAL_ADDR_OR_NULL(jobrank,addr) \
        (gasneti_assert((jobrank) < gasneti_nodes), gasneti_assert(addr), \
         gasneti_nbrhd_jobrank_local_addr_or_null(jobrank,addr))

/* ------------------------------------------------------------------------------------ */

#ifdef GASNETE_HAVE_EXTENDED_HELP_EXTRA_H
  #include <gasnet_extended_help_extra.h>
#endif

#endif
