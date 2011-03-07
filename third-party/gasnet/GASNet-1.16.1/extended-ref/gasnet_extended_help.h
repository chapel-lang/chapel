/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_extended_help.h,v $
 *     $Date: 2009/10/25 23:24:44 $
 * $Revision: 1.52 $
 * Description: GASNet Extended API Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_HELP_H
#define _GASNET_EXTENDED_HELP_H

GASNETI_BEGIN_EXTERNC

#include <gasnet_help.h>

/* ------------------------------------------------------------------------------------ */
/* GASNETI_MAX_THREADS: cannot exceed the size representable in gasnete_threadidx_t, 
   but some conduits or configures may set it to less */
#if GASNET_SEQ
  #undef GASNETI_MAX_THREADS /* only one thread by definition */
  #define GASNETI_MAX_THREADS 1
  #define GASNETI_MAX_THREADS_REASON "GASNET_SEQ mode only supports single-threaded operation."
#elif defined(GASNETI_MAX_THREADS) /* conduit-imposed limit */
  #if defined(GASNETI_MAX_THREADS_CONFIGURE) && GASNETI_MAX_THREADS_CONFIGURE < GASNETI_MAX_THREADS
    #undef  GASNETI_MAX_THREADS /* limit lowered by configure */
    #define GASNETI_MAX_THREADS GASNETI_MAX_THREADS_CONFIGURE
  #else
    #define GASNETI_MAX_THREADS_REASON "This limit is imposed by "GASNET_EXTENDED_NAME_STR" conduit."
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
#elif GASNETI_MAX_THREADS <= 256
  typedef uint8_t gasnete_threadidx_t;
#elif GASNETI_MAX_THREADS <= 65536
  typedef uint16_t gasnete_threadidx_t;
#elif GASNETI_MAX_THREADS <= 4294967296
  typedef uint32_t gasnete_threadidx_t;
#else
  typedef uint64_t gasnete_threadidx_t;
#endif
/* returns the runtime size of the thread table (always <= GASNETI_MAX_THREADS) */
extern uint64_t gasneti_max_threads(void);
extern void gasneti_fatal_threadoverflow(const char *subsystem);

#ifndef _GASNETE_MYTHREAD
  struct _gasnete_threaddata_t;
  #if GASNETI_MAX_THREADS <= 256
    extern struct _gasnete_threaddata_t *gasnete_threadtable[GASNETI_MAX_THREADS];
  #else
    extern struct _gasnete_threaddata_t **gasnete_threadtable;
  #endif
  #if GASNETI_CLIENT_THREADS
    extern struct _gasnete_threaddata_t *gasnete_mythread(void) GASNETI_CONST;
    GASNETI_CONSTP(gasnete_mythread)
  #else
    #define gasnete_mythread() (gasnete_threadtable[0])
  #endif
#endif

/* register a cleanup function to run when the calling thread exits 
   not guaranteed to run during process exits (gasnet_exit), but should
   run for dynamic thread exits when the process is continuing.
   Cleanups will run in reverse order of registration
 */
extern void gasnete_register_threadcleanup(void (*cleanupfn)(void *), void *context);

/* free list of valget cells */
#ifdef GASNETE_VALGET_CUSTOM
#define GASNETE_VALGET_FIELDS
#else 
#define GASNETE_VALGET_FIELDS struct _gasnete_valget_op_t *valget_free;
#endif

typedef struct _gasnete_thread_cleanup {
    struct _gasnete_thread_cleanup *next;
    void (*cleanupfn)(void *);
    void *context;
} gasnete_thread_cleanup_t; /* thread exit cleanup function LIFO */


/* fields that should appear first in the threaddata struct for all conduits */
#define GASNETE_COMMON_THREADDATA_FIELDS                                      \
  void *gasnetc_threaddata;     /* ptr reserved for use by the core */        \
  void *gasnete_coll_threaddata;/* ptr reserved for use by the collectives */ \
  void *gasnete_vis_threaddata; /* ptr reserved for use by the VIS */         \
                                                                              \
  gasnete_threadidx_t threadidx;                                              \
                                                                              \
  gasnete_thread_cleanup_t *thread_cleanup; /* thread cleanup function LIFO */\
  int thread_cleanup_delay;                                                   \
                                                                              \
  GASNETE_VALGET_FIELDS

/* high-water mark on highest thread index allocated thus far */
extern int gasnete_maxthreadidx;
#define gasnete_assert_valid_threadid(threadidx) do {   \
    GASNETI_UNUSED_UNLESS_DEBUG                         \
    int _thid = (threadidx);                            \
    gasneti_assert(_thid <= gasnete_maxthreadidx);      \
    gasneti_assert(gasnete_threadtable[_thid] != NULL); \
    gasneti_memcheck(gasnete_threadtable[_thid]);       \
} while (0)

/* gasnete_islocal() is used by put/get fns to decide whether shared memory on 
   a given node is "local". By default this is based on comparing the nodeid to
   the local node id, but clients can override this to remove the check overhead
   by defining either GASNETE_PUTGET_ALWAYSLOCAL or GASNETE_PUTGET_ALWAYSREMOTE
 */
#if defined(GASNETE_PUTGET_ALWAYSLOCAL)
  #define gasnete_islocal(nodeid) (1) /* always local */
#elif defined(GASNETE_PUTGET_ALWAYSREMOTE)
  #define gasnete_islocal(nodeid) (0) /* always remote */
#else
  #define gasnete_islocal(nodeid) (nodeid == gasneti_mynode)
#endif

/* ------------------------------------------------------------------------------------ */
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
    float _gasnete_anytype_4;
  #endif
  #if SIZEOF_VOID_P == 4
    void *_gasnete_anytype_p;
    intptr_t _gasnete_anytype_ip; /* might be a compiler builtin type */
  #endif
} GASNETI_MAY_ALIAS gasnete_anytype32_t;

typedef union {
  uint64_t _gasnete_anytype_u64; /* might be a compiler builtin type */
  gasnete_anytype32_t _gasnete_anytype_at32; /* necessary for structs of two 32-bit types */
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
    double _gasnete_anytype_4;
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
  switch(nbytes) {                                          \
    case 0:                                                 \
      break;                                                \
    case 1:  *((gasnete_anytype8_t *)(dest)) =              \
             *((gasnete_anytype8_t *)(src));                \
      break;                                                \
  GASNETE_OMIT_WHEN_MISSING_16BIT(                          \
    case 2:  *((gasnete_anytype16_t *)(dest)) =             \
             *((gasnete_anytype16_t *)(src));               \
      break;                                                \
  )                                                         \
    case 4:  *((gasnete_anytype32_t *)(dest)) =             \
             *((gasnete_anytype32_t *)(src));               \
      break;                                                \
    case 8:  *((gasnete_anytype64_t *)(dest)) =             \
             *((gasnete_anytype64_t *)(src));               \
      break;                                                \
    default:                                                \
      memcpy(dest, src, nbytes);                            \
  }                                                         \
  } while(0)
#endif /* GASNETI_BUG1389_WORKAROUND */

#define GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes) memcpy(dest, src, nbytes)

/* Wrapper around GASNETE_FAST_UNALIGNED_MEMCPY which becomes a no-op if src == dst */
#define GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dest, src, nbytes) do { \
    void *_dest = (dest);                                           \
    const void *_src = (src);                                       \
    if_pt (_dest != _src)                                           \
        GASNETE_FAST_UNALIGNED_MEMCPY(_dest, _src, (nbytes));       \
  } while (0)

/* given the address of a gasnet_register_value_t object and the number of
   significant bytes, return the byte address where significant bytes begin */
#ifdef WORDS_BIGENDIAN
  #define GASNETE_STARTOFBITS(regvalptr,nbytes) \
    (((uint8_t*)(regvalptr)) + ((sizeof(gasnet_register_value_t)-nbytes)))
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
   and return the value as a gasnet_register_value_t */
#ifdef GASNETI_BUG1389_WORKAROUND
  #define GASNETE_VALUE_RETURN(src, nbytes) do {              \
    gasnet_register_value_t result = 0;                       \
    gasneti_compiler_fence();                                 \
    memcpy(GASNETE_STARTOFBITS(&result,nbytes), src, nbytes); \
    return result;                                            \
  } while(0)
#else
#define GASNETE_VALUE_RETURN(src, nbytes) do {                               \
    gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t)); \
    switch (nbytes) {                                                        \
      case 1: return (gasnet_register_value_t)GASNETE_ANYTYPE_LVAL(src,8);   \
    GASNETE_OMIT_WHEN_MISSING_16BIT(                                         \
      case 2: return (gasnet_register_value_t)GASNETE_ANYTYPE_LVAL(src,16);  \
    )                                                                        \
      case 4: return (gasnet_register_value_t)GASNETE_ANYTYPE_LVAL(src,32);  \
      case 8: return (gasnet_register_value_t)GASNETE_ANYTYPE_LVAL(src,64);  \
      default: { /* no such native nbytes integral type */                   \
          gasnet_register_value_t result = 0;                                \
          memcpy(GASNETE_STARTOFBITS(&result,nbytes), src, nbytes);          \
          return result;                                                     \
      }                                                                      \
    }                                                                        \
  } while (0)
#endif /* GASNETI_BUG1389_WORKAROUND */


#if GASNET_NDEBUG
  #define gasnete_aligncheck(ptr,nbytes)
#else
  #if 0
    #define gasnete_aligncheck(ptr,nbytes) do {               \
        if ((nbytes) <= 8 && (nbytes) % 2 == 0)               \
          gasneti_assert(((uintptr_t)(ptr)) % (nbytes) == 0); \
      } while (0)
  #else
    static uint8_t _gasnete_aligncheck[600];
    #define gasnete_aligncheck(ptr,nbytes) do {                                         \
        uint8_t *_gasnete_alignbuf =                                                    \
          (uint8_t *)(((uintptr_t)&(_gasnete_aligncheck[0x100])) & ~((uintptr_t)0xFF)); \
        uintptr_t offset = ((uintptr_t)(ptr)) & 0xFF;                                   \
        uint8_t *p = _gasnete_alignbuf + offset;                                        \
        gasneti_assert(p >= _gasnete_aligncheck &&                                      \
              (p + 8) < (_gasnete_aligncheck+sizeof(_gasnete_aligncheck)));             \
        /* NOTE: a runtime bus error in this code indicates the relevant pointer        \
            was not "properly aligned for accessing objects of size nbytes", as         \
            required by the GASNet spec for src/dest addresses in non-bulk puts/gets    \
         */                                                                             \
        switch (nbytes) {                                                               \
          case 1: *(uint8_t *)p = 0; break;                                             \
        GASNETE_OMIT_WHEN_MISSING_16BIT(                                                \
          case 2: *(uint16_t *)p = 0; break;                                            \
        )                                                                               \
          case 4: *(uint32_t *)p = 0; break;                                            \
          case 8: *(uint64_t *)p = 0; break;                                            \
        }                                                                               \
      } while (0)
  #endif
#endif


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
 *  CPU0: gasnet_put_val(mynode,&A[0],someval,1) ; gasnet_put_val(mynode,&A[1],someval,1); 
 *  CPU1: gasnet_get(dest,mynode,&A[0],someval,2) ; // may see updated A[1] but not A[0]
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

/* ------------------------------------------------------------------------------------ */
/* thread-id optimization support */
#ifdef GASNETI_THREADINFO_OPT
  #if GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS
    #define GASNETE_THREAD_FARG_ALONE   gasnet_threadinfo_t const GASNETI_RESTRICT _threadinfo
  #else
    #define GASNETE_THREAD_FARG_ALONE   void * const GASNETI_RESTRICT _threadinfo
  #endif
  #define GASNETE_THREAD_FARG         , GASNETE_THREAD_FARG_ALONE
  #define GASNETE_THREAD_GET_ALONE    GASNET_GET_THREADINFO()
  #define GASNETE_THREAD_GET          , GASNETE_THREAD_GET_ALONE
  #define GASNETE_THREAD_PASS_ALONE   (_threadinfo)
  #define GASNETE_THREAD_PASS         , GASNETE_THREAD_PASS_ALONE
  #define GASNETE_THREAD_LOOKUP       GASNETE_THREAD_FARG_ALONE = GASNETE_THREAD_GET_ALONE;
  #define GASNETE_THREAD_SWALLOW(x)
  #define GASNETE_TISTARTOFBITS(ptr,nbytes,ti) GASNETE_STARTOFBITS(ptr,nbytes)
  #define GASNETE_MYTHREAD            ((struct _gasnete_threaddata_t *)_threadinfo)
#else
  #define GASNETE_THREAD_FARG_ALONE   void
  #define GASNETE_THREAD_FARG         
  #define GASNETE_THREAD_GET_ALONE   
  #define GASNETE_THREAD_GET         
  #define GASNETE_THREAD_PASS_ALONE   
  #define GASNETE_THREAD_PASS         
  #define GASNETE_THREAD_LOOKUP
  #define GASNETE_THREAD_SWALLOW(x)
  #define GASNETE_TISTARTOFBITS       GASNETE_STARTOFBITS
  #define GASNETE_MYTHREAD            (gasnete_mythread())
#endif
/* ------------------------------------------------------------------------------------ */

#ifdef GASNETE_HAVE_EXTENDED_HELP_EXTRA_H
  #include <gasnet_extended_help_extra.h>
#endif

GASNETI_END_EXTERNC

#endif
