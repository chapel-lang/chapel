/*   $Source: /var/local/cvs/gasnet/gasnet_help.h,v $
 *     $Date: 2009/04/30 09:14:00 $
 * $Revision: 1.105 $
 * Description: GASNet Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_HELP_H
#define _GASNET_HELP_H

GASNETI_BEGIN_EXTERNC

typedef struct { 
  uint64_t allocated_bytes;   /* num bytes ever allocated */
  uint64_t freed_bytes;       /* num bytes ever freed */
  uint64_t live_bytes;        /* num bytes currently allocated */
  uint64_t live_bytes_max;    /* max num bytes live at any given time */
  uint64_t allocated_objects; /* num objects ever allocated */
  uint64_t freed_objects;     /* num objects ever freed */
  uint64_t live_objects;      /* num objects currently allocated */
  uint64_t live_objects_max;  /* max num objects live at any given time */
  uint64_t overhead_bytes;    /* num bytes consumed by allocator overhead (lower bound) */
} gasneti_heapstats_t;

#if GASNET_DEBUG
  /* curloc is passed to debug mallocator as "file:line",
     or the special constant "SRCPOS" to retrieve the info from gasnet_srclines 
     To enable use of srcpos for a compilation unit, client should: 
       #undef GASNETT_MALLOC_USE_SRCPOS
       #define GASNETT_MALLOC_USE_SRCPOS 1 
  */
  #ifndef GASNETT_MALLOC_USE_SRCPOS
  #define GASNETT_MALLOC_USE_SRCPOS 0 /* off by default */
  #endif
  #define GASNETI_CURLOCFARG , const char *curloc
  #define GASNETI_CURLOCAARG , (GASNETT_MALLOC_USE_SRCPOS ? \
                               "SRCPOS" :                   \
                                __FILE__ ":" _STRINGIFY(__LINE__))
  #define GASNETI_CURLOCPARG , curloc
  extern size_t _gasneti_memcheck(void *ptr, const char *curloc, int checktype);
  extern void _gasneti_memcheck_one(const char *curloc);
  extern void _gasneti_memcheck_all(const char *curloc);
  #define gasneti_memcheck(ptr)  (gasneti_assert(ptr != NULL), \
         (void)_gasneti_memcheck(ptr, __FILE__ ":" _STRINGIFY(__LINE__), 0)) 
  #define gasneti_memcheck_one() _gasneti_memcheck_one(__FILE__ ":" _STRINGIFY(__LINE__))
  #define gasneti_memcheck_all() _gasneti_memcheck_all(__FILE__ ":" _STRINGIFY(__LINE__))
  extern int gasneti_getheapstats(gasneti_heapstats_t *pstat);
#else
  #define GASNETI_CURLOCFARG 
  #define GASNETI_CURLOCAARG 
  #define GASNETI_CURLOCPARG 
  #define gasneti_memcheck(ptr)   ((void)0)
  #define gasneti_memcheck_one()  ((void)0)
  #define gasneti_memcheck_all()  ((void)0)
  #define gasneti_getheapstats(pstat) (memset(pstat, 0, sizeof(gasneti_heapstats_t)),1)
#endif

/* extern versions of gasnet malloc fns for use in public headers */
extern void *_gasneti_extern_malloc(size_t sz GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_malloc)
extern void *_gasneti_extern_realloc(void *ptr, size_t sz GASNETI_CURLOCFARG);
extern void *_gasneti_extern_calloc(size_t N, size_t S GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_calloc)
extern void _gasneti_extern_free(void *ptr GASNETI_CURLOCFARG);
extern char *_gasneti_extern_strdup(const char *s GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_strdup)
extern char *_gasneti_extern_strndup(const char *s, size_t n GASNETI_CURLOCFARG) GASNETI_MALLOC;
GASNETI_MALLOCP(_gasneti_extern_strndup)

#define gasneti_extern_malloc(sz)      _gasneti_extern_malloc((sz) GASNETI_CURLOCAARG)
#define gasneti_extern_realloc(ptr,sz) _gasneti_extern_realloc((ptr), (sz) GASNETI_CURLOCAARG)
#define gasneti_extern_calloc(N,S)     _gasneti_extern_calloc((N),(S) GASNETI_CURLOCAARG)
#define gasneti_extern_free(ptr)       _gasneti_extern_free((ptr) GASNETI_CURLOCAARG)
#define gasneti_extern_strdup(s)       _gasneti_extern_strdup((s) GASNETI_CURLOCAARG)
#define gasneti_extern_strndup(s,n)    _gasneti_extern_strndup((s),(n) GASNETI_CURLOCAARG)

/* aligned malloc - allocated size bytes with given power-of-2 alignment
   may only be freed using gasneti_free_aligned */
GASNETI_INLINE(_gasneti_malloc_aligned) GASNETI_MALLOC
void * _gasneti_malloc_aligned(size_t alignment, size_t size GASNETI_CURLOCFARG) {
  size_t alloc_size = size + sizeof(void *) + alignment;
  void *base = _gasneti_extern_malloc(alloc_size GASNETI_CURLOCPARG);
  void **result = (void **)GASNETI_ALIGNUP((uintptr_t)base + sizeof(void *), alignment);
  *(result - 1) = base; /* hidden base ptr for free() */
  gasneti_assert(GASNETI_POWEROFTWO(alignment));
  gasneti_assert(result == (void **)GASNETI_ALIGNUP(result, alignment));
  gasneti_assert((void *)(result - 1) >= base);
  gasneti_assert(((uint8_t *)result + size) <= ((uint8_t *)base + alloc_size));
  return (void *)result;
}
GASNETI_MALLOCP(_gasneti_malloc_aligned)
#define gasneti_malloc_aligned(align,sz) _gasneti_malloc_aligned((align), (sz) GASNETI_CURLOCAARG)

GASNETI_INLINE(_gasneti_free_aligned)
void _gasneti_free_aligned(void *ptr GASNETI_CURLOCFARG) {
  void *base;
  gasneti_assert(ptr);
  base = *((void **)ptr - 1);
  gasneti_assert(base);
  _gasneti_extern_free(base GASNETI_CURLOCPARG);
}
#define gasneti_free_aligned(ptr) _gasneti_free_aligned((ptr) GASNETI_CURLOCAARG)

extern uint64_t gasnet_max_segsize; /* client-overrideable max segment size */
#if GASNET_SEGMENT_EVERYTHING
  #define gasneti_in_clientsegment(node,ptr,nbytes) (gasneti_assert((node) < gasneti_nodes), 1)
  #define gasneti_in_fullsegment(node,ptr,nbytes)   (gasneti_assert((node) < gasneti_nodes), 1)
#else
  #define gasneti_in_clientsegment(node,ptr,nbytes) \
    (gasneti_assert((node) < gasneti_nodes),        \
     ((ptr) >= gasneti_seginfo_client[node].addr && \
      (void *)(((uintptr_t)(ptr))+(nbytes)) <= gasneti_seginfo_client_ub[node]))
  #define gasneti_in_fullsegment(node,ptr,nbytes) \
    (gasneti_assert((node) < gasneti_nodes),      \
     ((ptr) >= gasneti_seginfo[node].addr &&      \
      (void *)(((uintptr_t)(ptr))+(nbytes)) <= gasneti_seginfo_ub[node]))
#endif

#ifdef _INCLUDED_GASNET_INTERNAL_H
  /* default for GASNet implementation is to check against entire seg */
  #define gasneti_in_segment gasneti_in_fullsegment
#else
  /* default for client is to check against just the client seg */
  #define gasneti_in_segment gasneti_in_clientsegment
#endif

#ifdef GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET
  /* in-segment check for internal put/gets that may exploit outofseg support */
  #define gasneti_in_segment_allowoutseg(node,ptr,nbytes) \
          (gasneti_assert((node) < gasneti_nodes), 1)
#else
  #define gasneti_in_segment_allowoutseg  gasneti_in_segment
#endif

#define _gasneti_boundscheck(node,ptr,nbytes,nodetest,segtest) do {            \
    gasnet_node_t _node = (node);                                              \
    const void *_ptr = (const void *)(ptr);                                    \
    size_t _nbytes = (size_t)(nbytes);                                         \
    if_pf (!nodetest(_node))                                                   \
      gasneti_fatalerror("Node index out of range (%lu >= %lu) at %s",         \
                         (unsigned long)_node, (unsigned long)gasneti_nodes,   \
                         gasneti_current_loc);                                 \
    if_pf (_ptr == NULL || !segtest(_node,_ptr,_nbytes))                       \
      gasneti_fatalerror("Remote address out of range "                        \
         "(node=%lu ptr="GASNETI_LADDRFMT" nbytes=%lu) at %s"                  \
         "\n  clientsegment=("GASNETI_LADDRFMT"..."GASNETI_LADDRFMT")"         \
         "\n    fullsegment=("GASNETI_LADDRFMT"..."GASNETI_LADDRFMT")",        \
         (unsigned long)_node, GASNETI_LADDRSTR(_ptr), (unsigned long)_nbytes, \
         gasneti_current_loc,                                                  \
         GASNETI_LADDRSTR(gasneti_seginfo_client[_node].addr),                 \
         GASNETI_LADDRSTR(gasneti_seginfo_client_ub[_node]),                   \
         GASNETI_LADDRSTR(gasneti_seginfo[_node].addr),                        \
         GASNETI_LADDRSTR(gasneti_seginfo_ub[_node])                           \
         );                                                                    \
  } while(0)

/* in-segment queries for the sole purpose of generating bounds checking errors 
   allow overrides for clients that allow bending the rules (shmem)
 */
#ifndef gasneti_in_segment_bc
#define gasneti_in_segment_bc gasneti_in_segment
#endif
#ifndef gasneti_in_segment_allowoutofseg_bc
#define gasneti_in_segment_allowoutofseg_bc gasneti_in_segment_allowoutseg
#endif
#ifndef gasneti_in_nodes_bc
#define gasneti_in_nodes_bc(node) (node < gasneti_nodes)
#endif

#if GASNET_NDEBUG
  #define gasneti_boundscheck(node,ptr,nbytes) 
  #define gasneti_boundscheck_allowoutseg(node,ptr,nbytes)
#else
  #define gasneti_boundscheck(node,ptr,nbytes) \
         _gasneti_boundscheck(node,ptr,nbytes,gasneti_in_nodes_bc,gasneti_in_segment_bc)
  #define gasneti_boundscheck_allowoutseg(node,ptr,nbytes) \
         _gasneti_boundscheck(node,ptr,nbytes,gasneti_in_nodes_bc,gasneti_in_segment_allowoutofseg_bc)
#endif

/* make a GASNet core API call - if it fails, print error message and abort */
#ifndef GASNETI_SAFE
#define GASNETI_SAFE(fncall) do {                                            \
   int _retcode = (fncall);                                                  \
   if_pf (_retcode != (int)GASNET_OK) {                                      \
     gasneti_fatalerror("\nGASNet encountered an error: %s(%i)\n"            \
        "  while calling: %s\n"                                              \
        "  at %s",                                                           \
        gasnet_ErrorName(_retcode), _retcode, #fncall, gasneti_current_loc); \
   }                                                                         \
 } while (0)
#endif

#if GASNET_DEBUG
  extern void gasneti_checkinit(void);
  extern void gasneti_checkattach(void);
  #define GASNETI_CHECKINIT()    gasneti_checkinit()
  #define GASNETI_CHECKATTACH()  gasneti_checkattach()
#else
  #define GASNETI_CHECKINIT()    ((void)0)
  #define GASNETI_CHECKATTACH()  ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
/* semi-portable spinlocks using gasneti_atomic_t
   This useful primitive is not available on all platforms and it therefore reserved 
   for internal use only.

   On platforms where implemented, the following are roughly equivalent to the
   corresponding pthread_mutex_* calls:
     GASNETI_SPINLOCK_INITIALIZER
     gasneti_spinlock_{init,destroy,lock,unlock,trylock}
   The functions return 0 on success to match the corresponding pthread_mutex functions.

   There is no gasneti_spinlock_t, these functions operate on gasneti_atomic_t.
   
   Unlike the pthread_mutex, the use of spinlocks have no fairness guarantees.  For
   instance, it would be perfectly legal for a race to always grant the lock to the CPU
   which "owns" the associated memory.  Therefore, spinlocks must be used with care.
   Also unlike pthread_mutex, it is safe to unlock one from signal context.  Though
   trying to acquire a spinlock in signal context is legal, it is dangerous.

   Like the pthread_mutex, these operations perform an RMB() when acquiring the
   lock and a WMB() when releasing it.  So, no additional memory barriers are
   required when using these locks to protect one's data structures.

   GASNETI_HAVE_SPINLOCK will be defined to 1 on platforms supporting this primitive.
 */
#if 0
  /* TODO Some platforms may have cheaper implementations than atomic-CAS. */
  /* eg. some platforms (SPARC?) can support spinlock using test-and-set */
#elif defined(GASNETI_ATOMICOPS_NOT_SIGNALSAFE)
  /* We don't implement this case due to lack of signal safety */
#elif defined(GASNETI_HAVE_ATOMIC_CAS)
  #if GASNET_DEBUG
    #define GASNETI_SPINLOCK_LOCKED	0xa5a5
    #define GASNETI_SPINLOCK_UNLOCKED	0xaa55
    #define GASNETI_SPINLOCK_DESTROYED	0xDEAD
    GASNETI_INLINE(gasneti_spinlock_is_valid)
    int gasneti_spinlock_is_valid(gasneti_atomic_t *plock) {
      uint32_t tmp = gasneti_atomic_read(plock, GASNETI_ATOMIC_RMB_PRE);
      if_pf (tmp == GASNETI_SPINLOCK_DESTROYED)
        gasneti_fatalerror("Detected use of destroyed spinlock");
      if_pf (!((tmp == GASNETI_SPINLOCK_LOCKED) || (tmp == GASNETI_SPINLOCK_UNLOCKED)))
        gasneti_fatalerror("Detected use of uninitialized or corrupted spinlock");
      return 1;
    }
    GASNETI_INLINE(gasneti_spinlock_is_locked)
    int gasneti_spinlock_is_locked(gasneti_atomic_t *plock) {
      uint32_t tmp = gasneti_atomic_read(plock, GASNETI_ATOMIC_RMB_PRE);
      return (tmp == GASNETI_SPINLOCK_LOCKED);
    }
  #else
    #define GASNETI_SPINLOCK_LOCKED	1
    #define GASNETI_SPINLOCK_UNLOCKED	0
    #define gasneti_spinlock_is_valid(plock) 1
  #endif
  #define GASNETI_SPINLOCK_INITIALIZER gasneti_atomic_init(GASNETI_SPINLOCK_UNLOCKED)
  #define gasneti_spinlock_init(plock) \
      gasneti_atomic_set((plock), GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_WMB_POST)
  #define gasneti_spinlock_destroy(plock) \
      gasneti_assert(gasneti_atomic_compare_and_swap(plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_DESTROYED, GASNETI_ATOMIC_WMB_POST))
  #define gasneti_spinlock_lock(plock) do {                                     \
      gasneti_waituntil(                                                        \
	!gasneti_spinlock_is_valid(plock) ||                                    \
        gasneti_atomic_compare_and_swap(plock,                                  \
          GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_LOCKED, 0)                \
      ); /* Acquire: the rmb() is in the gasneti_waituntil() */                 \
      gasneti_assert(gasneti_spinlock_is_locked(plock));                        \
  } while (0)
  GASNETI_INLINE(gasneti_spinlock_unlock)
  int gasneti_spinlock_unlock(gasneti_atomic_t *plock) {
      #if GASNET_DEBUG
        /* Using CAS for release is more costly, but adds validation */
        gasneti_assert(gasneti_atomic_compare_and_swap(plock, GASNETI_SPINLOCK_LOCKED, GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_REL));
      #else
        gasneti_atomic_set(plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_ATOMIC_REL);
      #endif
      return 0;
  }
  /* return 0/EBUSY on success/failure to match pthreads */
  GASNETI_INLINE(gasneti_spinlock_trylock) GASNETI_WARN_UNUSED_RESULT
  int gasneti_spinlock_trylock(gasneti_atomic_t *plock) {
      gasneti_assert(gasneti_spinlock_is_valid(plock));
      if (gasneti_atomic_compare_and_swap(plock, GASNETI_SPINLOCK_UNLOCKED, GASNETI_SPINLOCK_LOCKED, GASNETI_ATOMIC_ACQ_IF_TRUE)) {
	  gasneti_assert(gasneti_spinlock_is_locked(plock));
	  return 0;
      } else {
	  return EBUSY;
      }
  }
  #define GASNETI_HAVE_SPINLOCK 1
#endif

/* ------------------------------------------------------------------------------------ */
/* public threadinfo support */

#if GASNETI_CLIENT_THREADS
  #ifndef GASNETI_THREADINFO_OPT
  #define GASNETI_THREADINFO_OPT      1
  #endif
  #ifndef GASNETI_LAZY_BEGINFUNCTION
  #define GASNETI_LAZY_BEGINFUNCTION  1
  #endif
#endif

#if GASNETI_THREADINFO_OPT
  /* Here we use a clever trick - GASNET_GET_THREADINFO() uses the sizeof(gasneti_threadinfo_available)
      to determine whether gasneti_threadinfo_cache was bound a value posted by GASNET_POST_THREADINFO()
      of if it bound to the globally declared dummy variables. 
     Even a very stupid C optimizer should constant-fold away the unused calls to gasneti_get_threadinfo() 
      and discard the unused variables
     We need 2 separate variables to ensure correct name-binding semantics for GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
   */
  static uint8_t gasnete_threadinfo_cache = 0;
  static uint8_t gasnete_threadinfo_available = 
    sizeof(gasnete_threadinfo_cache) + sizeof(gasnete_threadinfo_available);
    /* silly little trick to prevent unused variable warning on gcc -Wall */

  #define GASNET_POST_THREADINFO(info)                     \
    gasnet_threadinfo_t gasnete_threadinfo_cache = (info); \
    uint32_t gasnete_threadinfo_available = 0
    /* if you get an unused variable warning on gasnete_threadinfo_available, 
       it means you POST'ed in a function which made no GASNet calls that needed it
       So, PLEASE don't add GASNETI_UNUSED annotations here. */

  #if GASNETI_LAZY_BEGINFUNCTION
    #define GASNET_GET_THREADINFO()                              \
      ( (sizeof(gasnete_threadinfo_available) == 1) ?            \
        (gasnet_threadinfo_t)gasnete_mythread() :                \
        ( (uintptr_t)gasnete_threadinfo_cache == 0 ?             \
          ((*(gasnet_threadinfo_t *)&gasnete_threadinfo_cache) = \
            (gasnet_threadinfo_t)gasnete_mythread()) :           \
          (gasnet_threadinfo_t)(uintptr_t)gasnete_threadinfo_cache) )
  #else
    #define GASNET_GET_THREADINFO()                   \
      ( (sizeof(gasnete_threadinfo_available) == 1) ? \
        (gasnet_threadinfo_t)gasnete_mythread() :     \
        (gasnet_threadinfo_t)(uintptr_t)gasnete_threadinfo_cache )
  #endif

  /* the gasnet_threadinfo_t pointer points to a thread data-structure owned by
     the extended API, whose first element is a pointer reserved
     for use by the core API (initialized to NULL)
   */

  #if GASNETI_LAZY_BEGINFUNCTION
    /* postpone thread discovery to first use */
    #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(0)
  #else
    #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
  #endif

#else
  #define GASNET_POST_THREADINFO(info)   \
    static uint8_t gasnete_dummy = sizeof(gasnete_dummy) /* prevent a parse error */
  #define GASNET_GET_THREADINFO() (NULL)
  #define GASNET_BEGIN_FUNCTION() GASNET_POST_THREADINFO(GASNET_GET_THREADINFO())
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNet progressfn support
 * progressfns are internal functions that are called "periodically" by a conduit to 
 *  allow internal GASNet modules to make progress. 
 * Each progressfn is associated with a named subsystem (one-to-one mapping)
 *  subsystem names should be prefixed by gasneti_pf_ to prevent macro name capture
 * GASNETI_PROGRESSFNS_ENABLE/GASNETI_PROGRESSFNS_DISABLE are used by the conduit
 *  to provide a hint when a particular subsystem's progressfns want to be serviced
 * Each progressfn has either a BOOLEAN hint or COUNTED hint flavor
 *  COUNTED flavor: ENABLE/DISABLE manipulate an atomic reference count, initially zero 
 *    and hinting that calls are requested whenever the count > 0
 *  BOOLEAN flavor: ENABLE/DISABLE is a simple (non-atomic) flag, and the conduit
 *    is responsible for arbitrating any races between different threads when  
 *    setting/clearing that flag under GASNETI_THREADS
 * progressfns are called from a non-AM context, but they should never perform collective ops
 *  or execute any code that might block (aside from aquiring hsls in an AM-safe manner)
 * they may be called concurrently (if GASNETI_THREADS), and must be prepared to
 *  receive calls even when the hint indicates the given subsytem does not need service
 *  additionally, progressfns that make gasnet calls must be prepared to recieve 
 *  reentrant calls (ie without infinite recursion or deadlock)
 */
typedef void (*gasneti_progressfn_t)(void);

/* currently the list of progressfns is compile-time constant for dispatch performance 
 * reasons (a static dispatch is about 3x faster than a dynamic one on modern CPUs)
 * in the future it may be expanded with a dynamic function registration facility
 * PROGRESSFNS_LIST entries should look like:
   FN(token subsysname, flavor [COUNTED|BOOLEAN], gasneti_progressfn_t progressfn)
 */

/* conduit-specific core plug-in */
#ifndef GASNETC_PROGRESSFNS_LIST
#define GASNETC_PROGRESSFNS_LIST(FN)
#endif

#ifndef GASNETE_PROGRESSFNS_LIST
  extern gasneti_progressfn_t gasnete_barrier_pf;
  #define GASNETE_BARRIER_PROGRESSFN(FN) \
    FN(gasneti_pf_barrier, BOOLEAN, gasnete_barrier_pf) 

  /* conduit-specific extended plug-in */
  #ifndef GASNETE_PROGRESSFN_EXTRA
  #define GASNETE_PROGRESSFN_EXTRA(FN) 
  #endif

  #define GASNETE_PROGRESSFNS_LIST(FN) \
    GASNETE_PROGRESSFN_EXTRA(FN) \
    GASNETE_BARRIER_PROGRESSFN(FN)     
#endif

#if GASNET_DEBUG
  extern gasneti_progressfn_t gasneti_debug_progressfn_bool;
  extern gasneti_progressfn_t gasneti_debug_progressfn_counted;
  #define GASNETI_DEBUG_PROGRESSFNS(FN) \
      FN(gasneti_pf_debug_boolean, BOOLEAN, gasneti_debug_progressfn_bool) \
      FN(gasneti_pf_debug_counted, COUNTED, gasneti_debug_progressfn_counted) 
#else
  #define GASNETI_DEBUG_PROGRESSFNS(FN)
#endif

#define GASNETI_PROGRESSFNS_LIST(FN) \
  GASNETI_DEBUG_PROGRESSFNS(FN)      \
  GASNETI_COLL_PROGRESSFNS(FN)       \
  GASNETI_VIS_PROGRESSFNS(FN)        \
  GASNETE_PROGRESSFNS_LIST(FN)       \
  GASNETC_PROGRESSFNS_LIST(FN) 

/* default to one atomic counter per subsystem, because atomic_read
   is many times faster than a do-nothing function call 
*/
#ifndef GASNETI_PROGRESSFNS_ENABLE
  #define _GASNETI_PROGRESSFNS_DEFAULT
  #define _GASNETI_PROGRESSFNS_FLAG(subsysname,flavor) \
          _gasneti_progressfn_enabled_##subsysname##_##flavor

  #define _GASNETI_PROGRESSFNS_TYPE_BOOLEAN volatile int
  #define _GASNETI_PROGRESSFNS_TYPE_COUNTED gasneti_weakatomic_t
  #define _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_TYPE_##flavor

  #define _GASNETI_PROGRESSFNS_INIT_BOOLEAN = 0
  #define _GASNETI_PROGRESSFNS_INIT_COUNTED = gasneti_weakatomic_init(0)
  #define _GASNETI_PROGRESSFNS_INIT(flavor) _GASNETI_PROGRESSFNS_INIT_##flavor

  #define _GASNETI_PROGRESSFNS_DECLARE_FLAGS(subsysname, flavor, progressfn) \
    extern _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_FLAG(subsysname, flavor);
  #define _GASNETI_PROGRESSFNS_DEFINE_FLAGS(subsysname, flavor, progressfn)         \
    _GASNETI_PROGRESSFNS_TYPE(flavor) _GASNETI_PROGRESSFNS_FLAG(subsysname, flavor) \
                                      _GASNETI_PROGRESSFNS_INIT(flavor);
  GASNETI_PROGRESSFNS_LIST(_GASNETI_PROGRESSFNS_DECLARE_FLAGS) /* forward declaration */

  #define _GASNETI_PROGRESSFNS_ENABLE_BOOLEAN(subsysname) \
    (_GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN) = 1)
  #define _GASNETI_PROGRESSFNS_DISABLE_BOOLEAN(subsysname) \
    (_GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN) = 0)
  #define _GASNETI_PROGRESSFNS_ENABLE_COUNTED(subsysname) do {                                   \
    gasneti_weakatomic_increment(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0);                \
    gasneti_assert(gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0) > 0); \
  } while (0)
  #define _GASNETI_PROGRESSFNS_DISABLE_COUNTED(subsysname) do {                                  \
    gasneti_assert(gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0) > 0); \
    gasneti_weakatomic_decrement(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0);                \
  } while (0)
  #define GASNETI_PROGRESSFNS_ENABLE(subsysname,flavor) \
         _GASNETI_PROGRESSFNS_ENABLE_##flavor(subsysname)
  #define GASNETI_PROGRESSFNS_DISABLE(subsysname,flavor) \
         _GASNETI_PROGRESSFNS_DISABLE_##flavor(subsysname)

  #define _GASNETI_PROGRESSFNS_ISENABLED_BOOLEAN(subsysname) \
    _GASNETI_PROGRESSFNS_FLAG(subsysname,BOOLEAN)
  #define _GASNETI_PROGRESSFNS_ISENABLED_COUNTED(subsysname) \
    gasneti_weakatomic_read(&_GASNETI_PROGRESSFNS_FLAG(subsysname,COUNTED),0)
  #define _GASNETI_PROGRESSFNS_RUN_IFENABLED(subsysname, flavor, progressfn) \
    (_GASNETI_PROGRESSFNS_ISENABLED_##flavor(subsysname) ? progressfn() : ((void)0)) ,
  #define GASNETI_PROGRESSFNS_RUN()                        \
   ( GASNETI_PROGRESSFNS_LIST(_GASNETI_PROGRESSFNS_RUN_IFENABLED) ((void)0) ) 
#endif

/* ------------------------------------------------------------------------------------ */
#ifndef GASNETI_GASNETI_AMPOLL
  /*
   gasnet_AMPoll() - public poll function called by the client, throttled and traced 
                     should not be called from within GASNet (so we only trace directly user-initiated calls)
   gasneti_AMPoll() - called internally by GASNet, provides throttling (if enabled), progress functions, but no tracing
   gasnetc_AMPoll() - conduit AM dispatcher, should only be called from gasneti_AMPoll()
   */
  #ifndef GASNETI_GASNETC_AMPOLL
    extern int gasnetc_AMPoll(void);
  #endif

  #if GASNETI_THROTTLE_FEATURE_ENABLED && (GASNET_PAR || GASNETI_CONDUIT_THREADS)
    #define GASNETI_THROTTLE_POLLERS 1
  #else
    #define GASNETI_THROTTLE_POLLERS 0
  #endif

  /* threads who need a network lock in order to send a message make 
     matched calls to gasneti_suspend/resume_spinpollers to help them 
     get the lock. They should not AMPoll while this suspend is in effect.
     The following debugging assertions detect violations of these rules.
  */ 
  #if GASNET_DEBUG
    GASNETI_THREADKEY_DECLARE(gasneti_throttledebug_key);

    #define gasneti_AMPoll_spinpollers_check()          \
      /* assert this thread hasn't already suspended */ \
      gasneti_assert((int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key) == 0)
    #define gasneti_suspend_spinpollers_check() do {                                        \
      int _mythrottlecnt = (int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key); \
      /* assert this thread hasn't already suspended */                                     \
      gasneti_assert(_mythrottlecnt == 0);                                                  \
      gasneti_threadkey_set(gasneti_throttledebug_key, (void *)(intptr_t)1);                \
    } while(0)
    #define gasneti_resume_spinpollers_check() do {                                         \
      int _mythrottlecnt = (int)(intptr_t)gasneti_threadkey_get(gasneti_throttledebug_key); \
      /* assert this thread previously suspended */                                         \
      gasneti_assert(_mythrottlecnt == 1);                                                  \
      gasneti_threadkey_set(gasneti_throttledebug_key, (void *)(intptr_t)0);                \
    } while(0)
  #else
    #define gasneti_AMPoll_spinpollers_check()  ((void)0)
    #define gasneti_suspend_spinpollers_check() ((void)0)
    #define gasneti_resume_spinpollers_check()  ((void)0)
  #endif

  #if !GASNETI_THROTTLE_POLLERS 
    GASNETI_INLINE(gasneti_AMPoll)
    int gasneti_AMPoll(void) {
       int retval;
       gasneti_AMPoll_spinpollers_check();
       gasneti_memcheck_one();
       retval = gasnetc_AMPoll();
       GASNETI_PROGRESSFNS_RUN();
       return retval;
    }
    #define gasneti_suspend_spinpollers() gasneti_suspend_spinpollers_check()
    #define gasneti_resume_spinpollers()  gasneti_resume_spinpollers_check()
  #else
    /* AMPoll with throttling, to reduce lock contention in the network:
       poll if and only if no other thread appears to already be spin-polling,
       and no thread is attempting to use the network for sending 
       Design goals (in rough order of importance):
        - when one or more threads need to send, all spin-pollers should get 
          out of the way (but continue checking their completion condition)
        - only one thread should be spin-polling at a time, to prevent 
          lock contention and cache thrashing between spin-pollers
        - manage AMPoll calls internal to GASNet (including those from polluntil) 
          and explicit client AMPoll calls
        - allow concurrent handler execution - if the spin poller recvs an AM, 
          it should release another spin poller before invoking the handler
        - the single spin-poller should not need to pay locking overheads in the loop
    */
    extern gasneti_atomic_t gasneti_throttle_haveusefulwork;
    extern gasneti_mutex_t gasneti_throttle_spinpoller;

    #define gasneti_suspend_spinpollers() do {                      \
        gasneti_suspend_spinpollers_check();                        \
        gasneti_atomic_increment(&gasneti_throttle_haveusefulwork,0); \
    } while (0)
    #define gasneti_resume_spinpollers() do {                       \
        gasneti_resume_spinpollers_check();                         \
        gasneti_atomic_decrement(&gasneti_throttle_haveusefulwork,0); \
    } while (0)

    /* and finally, the throttled poll implementation */
    GASNETI_INLINE(gasneti_AMPoll)
    int gasneti_AMPoll() {
       int retval;
       gasneti_AMPoll_spinpollers_check();
       gasneti_memcheck_one();
       if (gasneti_atomic_read(&gasneti_throttle_haveusefulwork,0) > 0) 
         return GASNET_OK; /* another thread sending - skip the poll */
       if (gasneti_mutex_trylock(&gasneti_throttle_spinpoller) != 0)
         return GASNET_OK; /* another thread spin-polling - skip the poll */
       retval = gasnetc_AMPoll();
       gasneti_mutex_unlock(&gasneti_throttle_spinpoller);
       GASNETI_PROGRESSFNS_RUN();
       return retval;
    }
  #endif
#endif
  
/* Blocking functions
 * Note the _rmb at the end loop of each is required to ensure that subsequent
 * reads will not observe values that were prefeteched or are otherwise out
 * of date.
 */
extern int gasneti_wait_mode; /* current waitmode hint */
#define GASNETI_WAITHOOK() do {                                       \
    if (gasneti_wait_mode != GASNET_WAIT_SPIN) gasneti_sched_yield(); \
    /* prevent optimizer from hoisting the condition check out of */  \
    /* the enclosing spin loop - this is our way of telling the */    \
    /* optimizer "the whole world could change here" */               \
    gasneti_compiler_fence();                                         \
    gasneti_spinloop_hint();                                          \
  } while (0)

/* busy-waits, with no implicit polling (cnd should include an embedded poll)
   differs from GASNET_BLOCKUNTIL because it may be waiting for an event
     caused by the receipt of a non-AM message
 */
#ifndef gasneti_waitwhile
  #define gasneti_waitwhile(cnd) do { \
    while (cnd) GASNETI_WAITHOOK();   \
    gasneti_local_rmb();              \
  } while (0)
#endif
#define gasneti_waituntil(cnd) gasneti_waitwhile(!(cnd)) 

/* busy-wait, with implicit polling */
/* Note no poll if the condition is already satisfied */
#ifndef gasneti_pollwhile
  #define gasneti_pollwhile(cnd) do { \
    if (cnd) {                        \
      gasneti_AMPoll();               \
      while (cnd) {                   \
        GASNETI_WAITHOOK();           \
        gasneti_AMPoll();             \
      }                               \
    }                                 \
    gasneti_local_rmb();              \
  } while (0)
#endif
#define gasneti_polluntil(cnd) gasneti_pollwhile(!(cnd)) 

/* ------------------------------------------------------------------------------------ */

/* high-performance timer library */
#include <gasnet_timer.h>

/* tracing utilities */
#include <gasnet_trace.h>

/* ------------------------------------------------------------------------------------ */
/* default implementations of various conduit functions (may be overridden in some conduits) */

#ifndef _GASNET_AMPOLL
#define _GASNET_AMPOLL
  /* GASNet client calls gasnet_AMPoll(), which throttles and traces */
  GASNETI_INLINE(gasnet_AMPoll)
  int gasnet_AMPoll(void) {
    GASNETI_TRACE_EVENT(I, AMPOLL);
    return gasneti_AMPoll();
  }
#endif

#ifndef _GASNET_GETENV
#define _GASNET_GETENV
  GASNETI_INLINE(gasnet_getenv)
  char *gasnet_getenv(const char *s) {
    GASNETI_CHECKINIT();
    return gasneti_getenv(s);
  }
#endif

#ifndef _GASNET_WAITMODE
#define _GASNET_WAITMODE
  #define GASNET_WAIT_SPIN      0 /* contend aggressively for CPU resources while waiting (spin) */
  #define GASNET_WAIT_BLOCK     1 /* yield CPU resources immediately while waiting (block) */
  #define GASNET_WAIT_SPINBLOCK 2 /* spin for an implementation-dependent period, then block */
  extern int gasneti_set_waitmode(int wait_mode);
  #define gasnet_set_waitmode(wait_mode) gasneti_set_waitmode(wait_mode)
#endif

#ifndef _GASNET_MYNODE
#define _GASNET_MYNODE
#define _GASNET_MYNODE_DEFAULT
  extern gasnet_node_t gasneti_mynode;
  #define gasnet_mynode() (GASNETI_CHECKINIT(), (gasnet_node_t)gasneti_mynode)
#endif

#ifndef _GASNET_NODES
#define _GASNET_NODES
#define _GASNET_NODES_DEFAULT
  extern gasnet_node_t gasneti_nodes;
  #define gasnet_nodes() (GASNETI_CHECKINIT(), (gasnet_node_t)gasneti_nodes)
#endif

#ifndef _GASNET_GETMAXSEGMENTSIZE
#define _GASNET_GETMAXSEGMENTSIZE
#define _GASNET_GETMAXSEGMENTSIZE_DEFAULT
  #if GASNET_SEGMENT_EVERYTHING
    #define gasnet_getMaxLocalSegmentSize()   ((uintptr_t)-1)
    #define gasnet_getMaxGlobalSegmentSize()  ((uintptr_t)-1)
  #else
    extern uintptr_t gasneti_MaxLocalSegmentSize;
    extern uintptr_t gasneti_MaxGlobalSegmentSize;
    #define gasnet_getMaxLocalSegmentSize() \
            (GASNETI_CHECKINIT(), (uintptr_t)gasneti_MaxLocalSegmentSize)
    #define gasnet_getMaxGlobalSegmentSize() \
            (GASNETI_CHECKINIT(), (uintptr_t)gasneti_MaxGlobalSegmentSize)
  #endif
#endif

#ifndef _GASNET_GETSEGMENTINFO
#define _GASNET_GETSEGMENTINFO
  extern int gasneti_getSegmentInfo(gasnet_seginfo_t *seginfo_table, int numentries);
  #define gasnet_getSegmentInfo(seginfo_table, numentries) \
          gasneti_getSegmentInfo(seginfo_table, numentries)
#endif

#ifndef _GASNETI_SEGINFO
#define _GASNETI_SEGINFO
#define _GASNETI_SEGINFO_DEFAULT
  extern gasnet_seginfo_t *gasneti_seginfo;
  extern gasnet_seginfo_t *gasneti_seginfo_client;
  extern void **gasneti_seginfo_ub;
  extern void **gasneti_seginfo_client_ub;
#endif

/* ------------------------------------------------------------------------------------ */
/* Bits for conduits which want/need to override pthread_create() */
#if defined(PTHREAD_MUTEX_INITIALIZER) /* only if pthread.h available */ && !GASNET_SEQ
  /* gasneti_pthread_create() available on all non-SEQ builds w/ pthreads */
  typedef int (gasneti_pthread_create_fn_t)(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
  extern int gasneti_pthread_create(gasneti_pthread_create_fn_t *create_fn, pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

  #if defined(GASNETC_PTHREAD_CREATE_OVERRIDE)
    /* Capture existing defn, which could be another library's override */
    static int gasneti_pthread_create_system(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
      return pthread_create(thread,attr,start_routine,arg);
    }
    /* Install our override */
    #undef pthread_create
    #define pthread_create(thr, attr, fn, arg) \
        gasneti_pthread_create(&gasneti_pthread_create_system, (thr), (attr), (fn), (arg))
  #endif
#endif
/* ------------------------------------------------------------------------------------ */

GASNETI_END_EXTERNC

#endif
