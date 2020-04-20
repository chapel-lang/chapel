/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_internal.c $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_am.h>

#include <gasnet_tools.h>

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#if HAVE_MALLOC_H && !PLATFORM_OS_OPENBSD /* OpenBSD warns that malloc.h is obsolete */
  #include <malloc.h>
#endif

/* set to non-zero for verbose error reporting */
int gasneti_VerboseErrors = 1;

/* ------------------------------------------------------------------------------------ */
/* generic atomics support */
#if defined(GASNETI_BUILD_GENERIC_ATOMIC32) || defined(GASNETI_BUILD_GENERIC_ATOMIC64)
  #ifdef GASNETI_ATOMIC_LOCK_TBL_DEFNS
    #define _gasneti_atomic_lock_initializer	GEX_HSL_INITIALIZER
    #define _gasneti_atomic_lock_init(x)	gex_HSL_Init(x)
    #define _gasneti_atomic_lock_lock(x)	gex_HSL_Lock(x)
    #define _gasneti_atomic_lock_unlock(x)	gex_HSL_Unlock(x)
    #define _gasneti_atomic_lock_malloc		gasneti_malloc
    GASNETI_ATOMIC_LOCK_TBL_DEFNS(gasneti_hsl_atomic_, gex_HSL_)
    #undef _gasneti_atomic_lock_initializer
    #undef _gasneti_atomic_lock_init
    #undef _gasneti_atomic_lock_lock
    #undef _gasneti_atomic_lock_unlock
    #undef _gasneti_atomic_lock_malloc
  #endif
  #ifdef GASNETI_GENATOMIC32_DEFN
    GASNETI_GENATOMIC32_DEFN
  #endif
  #ifdef GASNETI_GENATOMIC64_DEFN
    GASNETI_GENATOMIC64_DEFN
  #endif
#endif

/* ------------------------------------------------------------------------------------ */

#if GASNETI_THROTTLE_POLLERS
  gasneti_atomic_t gasneti_throttle_haveusefulwork = gasneti_atomic_init(0);
  gasneti_mutex_t gasneti_throttle_spinpoller = GASNETI_MUTEX_INITIALIZER;
#endif
#if GASNET_DEBUG
  GASNETI_THREADKEY_DEFINE(gasneti_throttledebug_key);
#endif

#define GEX_VERSION_STR  _STRINGIFY(GEX_SPEC_VERSION_MAJOR) "."  _STRINGIFY(GEX_SPEC_VERSION_MINOR)
GASNETI_IDENT(gasneti_IdentString_EXAPIVersion, "$GASNetEXAPIVersion: " GEX_VERSION_STR " $");

#define GASNET_VERSION_STR  _STRINGIFY(GASNETI_SPEC_VERSION_MAJOR)
GASNETI_IDENT(gasneti_IdentString_APIVersion, "$GASNetAPIVersion: " GASNET_VERSION_STR " $");

#define GASNETI_THREAD_MODEL_STR _STRINGIFY(GASNETI_THREAD_MODEL)
GASNETI_IDENT(gasneti_IdentString_ThreadModel, "$GASNetThreadModel: GASNET_" GASNETI_THREAD_MODEL_STR " $");

#define GASNETI_SEGMENT_CONFIG_STR _STRINGIFY(GASNETI_SEGMENT_CONFIG)
GASNETI_IDENT(gasneti_IdentString_SegConfig, "$GASNetSegment: GASNET_SEGMENT_" GASNETI_SEGMENT_CONFIG_STR " $");

#ifdef GASNETI_BUG1389_WORKAROUND
  GASNETI_IDENT(gasneti_IdentString_ConservativeLocalCopy, "$GASNetConservativeLocalCopy: 1 $");
#endif

#if GASNETI_CLIENT_THREADS
  GASNETI_IDENT(gasneti_IdentString_ThreadInfoOpt, "$GASNetThreadInfoOpt: " _STRINGIFY(GASNETI_THREADINFO_OPT) " $");
#endif

/* embed a string with complete configuration info to support versioning checks */
GASNETI_IDENT(gasneti_IdentString_libraryConfig, "$GASNetConfig: (libgasnet.a) " GASNET_CONFIG_STRING " $");
/* the canonical conduit name */
GASNETI_IDENT(gasneti_IdentString_ConduitName, "$GASNetConduitName: " GASNET_CONDUIT_NAME_STR " $");

int gasneti_init_done = 0; /*  true after init */
int gasneti_attach_done = 0; /*  true after attach */
extern void gasneti_checkinit(void) {
  if (!gasneti_init_done)
    gasneti_fatalerror("Illegal call to GASNet before gasnet_init() initialization");
}
extern void gasneti_checkattach(void) {
   gasneti_checkinit();
   if (!gasneti_attach_done)
    gasneti_fatalerror("Illegal call to GASNet before gasnet_attach() initialization");
}

void (*gasnet_client_attach_hook)(void *, uintptr_t) = NULL;

int gasneti_wait_mode = GASNET_WAIT_SPIN;

int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MALLOC_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIOPT_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(HIDDEN_AM_CONCUR_,GASNET_HIDDEN_AM_CONCURRENCY_LEVEL)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CACHE_LINE_BYTES_,GASNETI_CACHE_LINE_BYTES)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME)) = 1;

/* global definitions of GASNet-wide internal variables
   not subject to override */
gex_Rank_t gasneti_mynode = (gex_Rank_t)-1;
gex_Rank_t gasneti_nodes = 0;

// a necessary evil - see the declaration in gasnet_help.h
gasneti_TM_t gasneti_thing_that_goes_thunk_in_the_dark = NULL;

/* Default global definitions of GASNet-wide internal variables
   if conduits override one of these, they must
   still provide variable or macro definitions for these tokens */
#if defined(_GASNET_GETMAXSEGMENTSIZE_DEFAULT)
  uintptr_t gasneti_MaxLocalSegmentSize = 0;
  uintptr_t gasneti_MaxGlobalSegmentSize = 0;
#endif

#ifdef _GASNETI_PROGRESSFNS_DEFAULT
  GASNETI_PROGRESSFNS_LIST(_GASNETI_PROGRESSFNS_DEFINE_FLAGS)
#endif

#if GASNET_DEBUG
  static void gasneti_disabled_progressfn(void) {
    gasneti_fatalerror("Called a disabled progress function");
  }
  gasneti_progressfn_t gasneti_debug_progressfn_bool = gasneti_disabled_progressfn;
  gasneti_progressfn_t gasneti_debug_progressfn_counted = gasneti_disabled_progressfn;
#endif

gasnet_seginfo_t *gasneti_seginfo = NULL;
gasnet_seginfo_t *gasneti_seginfo_aux = NULL;

/* ------------------------------------------------------------------------------------ */
/* conduit-independent sanity checks */
extern void gasneti_check_config_preinit(void) {
  gasneti_static_assert(sizeof(int8_t) == 1);
  gasneti_static_assert(sizeof(uint8_t) == 1);
  gasneti_static_assert(sizeof(gasnete_anytype8_t) == 1);
  #ifndef INTTYPES_16BIT_MISSING
    gasneti_static_assert(sizeof(int16_t) == 2);
    gasneti_static_assert(sizeof(uint16_t) == 2);
    gasneti_static_assert(sizeof(gasnete_anytype16_t) == 2);
  #endif
  gasneti_static_assert(sizeof(int32_t) == 4);
  gasneti_static_assert(sizeof(uint32_t) == 4);
  gasneti_static_assert(sizeof(gasnete_anytype32_t) == 4);
  gasneti_static_assert(sizeof(int64_t) == 8);
  gasneti_static_assert(sizeof(uint64_t) == 8);
  gasneti_static_assert(sizeof(gasnete_anytype64_t) == 8);

  gasneti_static_assert(sizeof(uintptr_t) >= sizeof(void *));

  #define CHECK_DT(id, type) do { \
      gasneti_assert_always(gasneti_dt_valid(id)); \
      gasneti_assert_always_uint(gasneti_dt_size(id) ,==, sizeof(type)); \
      gasneti_assert_always(!!gasneti_dt_int(id) == !gasneti_dt_fp(id)); \
    } while (0)
  #define CHECK_INT_DT(id, type, sign) do { \
      CHECK_DT(id, type); \
      gasneti_assert_always(gasneti_dt_int(id)); \
      gasneti_assert_always(gasneti_dt_##sign(id)); \
      gasneti_assert_always(!!gasneti_dt_signed(id) == !gasneti_dt_unsigned(id)); \
    } while (0)
  #define CHECK_FP_DT(id, type) do { \
      CHECK_DT(id, type); \
      gasneti_assert_always(gasneti_dt_fp(id)); \
    } while (0)

  CHECK_INT_DT(GEX_DT_I32,  int32_t,   signed);
  CHECK_INT_DT(GEX_DT_U32, uint32_t, unsigned);
  CHECK_INT_DT(GEX_DT_I64,  int64_t,   signed);
  CHECK_INT_DT(GEX_DT_U64, uint64_t, unsigned);

  CHECK_FP_DT(GEX_DT_FLT,  float);
  CHECK_FP_DT(GEX_DT_DBL, double);

  gasneti_assert_always(gasneti_dt_valid_reduce(GEX_DT_USER));
  gasneti_assert_always(!gasneti_dt_valid_atomic(GEX_DT_USER));
  gasneti_assert_always(!gasneti_dt_int(GEX_DT_USER));
  gasneti_assert_always(!gasneti_dt_fp(GEX_DT_USER));
  gasneti_assert_always(!gasneti_dt_signed(GEX_DT_USER));
  gasneti_assert_always(!gasneti_dt_unsigned(GEX_DT_USER));

  #undef CHECK_DT
  #undef CHECK_INT_DT
  #undef CHECK_FP_DT

  #define _CHECK_OP(id, pred1, pred2, pred3) do { \
      gasneti_assert_always(gasneti_op_atomic(id)); \
      gasneti_assert_always(gasneti_op_int(id)); \
      gasneti_assert_always(!!gasneti_op_0arg(id) + \
                            !!gasneti_op_1arg(id) + \
                            !!gasneti_op_2arg(id) == 1); \
      gasneti_assert_always(gasneti_op_##pred1(id)); \
      gasneti_assert_always(gasneti_op_##pred2(id)); \
      gasneti_assert_always(gasneti_op_##pred3(id)); \
    } while (0)
  #define CHECK_ARITH_OP(stem, reduce_pred, fp_pred) do { \
      gasneti_assert_always(!gasneti_op_fetch(GEX_OP_##stem)); \
      _CHECK_OP(GEX_OP_##stem, reduce_pred, fp_pred, valid); \
      gasneti_assert_always(gasneti_op_fetch(GEX_OP_F##stem)); \
      _CHECK_OP(GEX_OP_F##stem, not_reduce, fp_pred, valid); \
    } while (0)
  #define CHECK_ACCESSOR(stem, pred) do { \
      gasneti_assert_always(gasneti_op_valid(GEX_OP_##stem)); \
      _CHECK_OP(GEX_OP_##stem, fp, not_reduce, pred); \
    } while (0)
  #define CHECK_USER(stem) do { \
      gasneti_assert_always(gasneti_op_valid(GEX_OP_##stem)); \
      gasneti_assert_always(gasneti_op_valid_reduce(GEX_OP_##stem)); \
      gasneti_assert_always(!gasneti_op_valid_atomic(GEX_OP_##stem)); \
      gasneti_assert_always(gasneti_op_int(GEX_OP_##stem)); \
      gasneti_assert_always(gasneti_op_fp(GEX_OP_##stem)); \
    } while (0)

  #define gasneti_op_not_reduce !gasneti_op_reduce
  #define gasneti_op_not_fetch  !gasneti_op_fetch
  #define gasneti_op_not_fp     !gasneti_op_fp

  CHECK_ARITH_OP(AND,  reduce, not_fp);
  CHECK_ARITH_OP(OR,   reduce, not_fp);
  CHECK_ARITH_OP(XOR,  reduce, not_fp);
  CHECK_ARITH_OP(ADD,  reduce,     fp);
  CHECK_ARITH_OP(SUB,  not_reduce, fp);
  CHECK_ARITH_OP(MULT, reduce,     fp);
  CHECK_ARITH_OP(MIN,  reduce,     fp);
  CHECK_ARITH_OP(MAX,  reduce,     fp);
  CHECK_ARITH_OP(INC,  not_reduce, fp);
  CHECK_ARITH_OP(DEC,  not_reduce, fp);

  CHECK_ACCESSOR(SET,   not_fetch);
  CHECK_ACCESSOR(CAS,   not_fetch);
  CHECK_ACCESSOR(GET,   fetch);
  CHECK_ACCESSOR(SWAP,  fetch);
  CHECK_ACCESSOR(FCAS,  fetch);

  CHECK_USER(USER);
  CHECK_USER(USER_NC);

  #undef _CHECK_OP
  #undef CHECK_ARITH_OP
  #undef CHECK_ACCESSOR
  #undef CHECK_USER
  #undef gasneti_op_not_reduce
  #undef gasneti_op_not_fetch
  #undef gasneti_op_not_fp

  #if WORDS_BIGENDIAN
    #if PLATFORM_ARCH_LITTLE_ENDIAN
      #error endianness disagreement: PLATFORM_ARCH_LITTLE_ENDIAN and WORDS_BIGENDIAN are both set
    #endif
    gasneti_assert_always(!gasneti_isLittleEndian());
  #else
    #if PLATFORM_ARCH_BIG_ENDIAN
      #error endianness disagreement: PLATFORM_ARCH_BIG_ENDIAN and !WORDS_BIGENDIAN
    #endif
    gasneti_assert_always(gasneti_isLittleEndian());
  #endif

  /* check GASNET_PAGESIZE is a power of 2 and > 0 */
  gasneti_static_assert(GASNET_PAGESIZE > 0);
  gasneti_static_assert(GASNETI_POWEROFTWO(GASNET_PAGESIZE));

  gasneti_static_assert(SIZEOF_GEX_RMA_VALUE_T == sizeof(gex_RMA_Value_t));
  gasneti_static_assert(SIZEOF_GEX_RMA_VALUE_T >= sizeof(int));
  gasneti_static_assert(SIZEOF_GEX_RMA_VALUE_T >= sizeof(void *));

  #if    PLATFORM_ARCH_32 && !PLATFORM_ARCH_64
    gasneti_static_assert(sizeof(void*) == 4);
  #elif !PLATFORM_ARCH_32 &&  PLATFORM_ARCH_64
    gasneti_static_assert(sizeof(void*) == 8);
  #else
    #error must #define exactly one of PLATFORM_ARCH_32 or PLATFORM_ARCH_64
  #endif

  #if defined(GASNETI_UNI_BUILD)
    if (gasneti_cpu_count() > 1) 
      gasneti_fatalerror("GASNet was built in uniprocessor (non-SMP-safe) configuration, "
        "but executed on an SMP. Please re-run GASNet configure with --enable-smp-safe and rebuild");
  #endif

  { static int firstcall = 1;
    if (firstcall) { /* miscellaneous conduit-independent initializations */
      firstcall = 0;
      #if GASNET_DEBUG && GASNETI_THREADS
        gasneti_threadkey_init(gasneti_throttledebug_key);
      #endif
      gasneti_memcheck_all();
    }
  }
}

static void gasneti_check_portable_conduit(void);
static void gasneti_check_architecture(void);
int gasneti_malloc_munmap_disabled = 0;
extern void gasneti_check_config_postattach(void) {
  gasneti_check_config_preinit();

  /*  verify sanity of the core interface */
  gasneti_assert_always_uint(gex_AM_MaxArgs() ,>=, 2*MAX(sizeof(int),sizeof(void*)));      
  gasneti_assert_always_uint(gex_AM_LUBRequestMedium() ,>=, 512);
  gasneti_assert_always_uint(gex_AM_LUBReplyMedium() ,>=, 512);
  gasneti_assert_always_uint(gex_AM_LUBRequestLong() ,>=, 512);
  gasneti_assert_always_uint(gex_AM_LUBReplyLong() ,>=, 512);

  gasneti_assert_always_uint(gasneti_nodes ,>=, 1);
  gasneti_assert_always_uint(gasneti_mynode ,<, gasneti_nodes);
  { static int firstcall = 1;
    if (firstcall) { /* miscellaneous conduit-independent initializations */
      firstcall = 0;

      #ifndef GASNET_DISABLE_MUNMAP_DEFAULT
      #define GASNET_DISABLE_MUNMAP_DEFAULT 0
      #endif
      if (gasneti_getenv_yesno_withdefault("GASNET_DISABLE_MUNMAP",GASNET_DISABLE_MUNMAP_DEFAULT)) {
        #if HAVE_PTMALLOC                                        
          mallopt(M_TRIM_THRESHOLD, -1);
          mallopt(M_MMAP_MAX, 0);
          GASNETI_TRACE_PRINTF(I,("Setting mallopt M_TRIM_THRESHOLD=-1 and M_MMAP_MAX=0"));
          gasneti_malloc_munmap_disabled = 1;
        #else
          GASNETI_TRACE_PRINTF(I,("WARNING: GASNET_DISABLE_MUNMAP set on an unsupported platform"));
          if (gasneti_verboseenv()) 
            fprintf(stderr, "WARNING: GASNET_DISABLE_MUNMAP set on an unsupported platform\n");
        #endif
      }
      #if GASNET_NDEBUG
        gasneti_check_portable_conduit();
        gasneti_check_architecture();
      #endif
    }
  }
  gasneti_memcheck_all();

  gasneti_flush_streams();  // flush above messages, and ensure FS_SYNC envvar is initted
}

/* ------------------------------------------------------------------------------------ */
#ifndef _GASNET_ERRORNAME
extern const char *gasnet_ErrorName(int errval) {
  switch (errval) {
    case GASNET_OK:           return "GASNET_OK";      
    case GASNET_ERR_NOT_INIT: return "GASNET_ERR_NOT_INIT";      
    case GASNET_ERR_BAD_ARG:  return "GASNET_ERR_BAD_ARG";       
    case GASNET_ERR_RESOURCE: return "GASNET_ERR_RESOURCE";      
    case GASNET_ERR_BARRIER_MISMATCH: return "GASNET_ERR_BARRIER_MISMATCH";      
    case GASNET_ERR_NOT_READY: return "GASNET_ERR_NOT_READY";      
    default: return "*unknown*";
  }
}
#endif

#ifndef _GASNET_ERRORDESC
extern const char *gasnet_ErrorDesc(int errval) {
  switch (errval) {
    case GASNET_OK:           return "No error";      
    case GASNET_ERR_NOT_INIT: return "GASNet message layer not initialized"; 
    case GASNET_ERR_BAD_ARG:  return "Invalid function parameter passed";    
    case GASNET_ERR_RESOURCE: return "Problem with requested resource";      
    case GASNET_ERR_BARRIER_MISMATCH: return "Barrier id's mismatched";      
    case GASNET_ERR_NOT_READY: return "Non-blocking operation not complete";      
    default: return "no description available";
  }
}
#endif
/* ------------------------------------------------------------------------------------ */
extern void gasneti_freezeForDebugger(void) {
  if (gasneti_getenv_yesno_withdefault("GASNET_FREEZE",0)) {
    gasneti_freezeForDebuggerNow(&gasnet_frozen,"gasnet_frozen");
  }
}
/* ------------------------------------------------------------------------------------ */

#ifndef _GEX_CLIENT_T
#ifndef gasneti_import_client
gasneti_Client_t gasneti_import_client(gex_Client_t _client) {
  const gasneti_Client_t _real_client = GASNETI_IMPORT_POINTER(gasneti_Client_t,_client);
  GASNETI_IMPORT_MAGIC(_real_client, CLIENT);
  return _real_client;
}
#endif

#ifndef gasneti_export_client
gex_Client_t gasneti_export_client(gasneti_Client_t _real_client) {
  GASNETI_CHECK_MAGIC(_real_client, GASNETI_CLIENT_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_Client_t, _real_client);
}
#endif

// TODO-EX: either ensure name is unique OR perform "auto-increment" according to flags
gasneti_Client_t gasneti_alloc_client(
                       const char *name,
                       gex_Flags_t flags,
                       size_t requested_sz)
{
  gasneti_Client_t client;
  if (requested_sz) gasneti_assert_uint(requested_sz ,>=, sizeof(*client));
  size_t alloc_size = requested_sz ? requested_sz : sizeof(*client);
  client = gasneti_malloc(alloc_size);
  GASNETI_INIT_MAGIC(client, GASNETI_CLIENT_MAGIC);
  client->_tm0 = NULL;
  client->_name = gasneti_strdup(name);
  client->_cdata = NULL;
  client->_flags = flags;
#ifdef GASNETI_CLIENT_ALLOC_EXTRA
  GASNETI_CLIENT_ALLOC_EXTRA(client);
#else
  if (requested_sz) memset(client + 1, 0, alloc_size - sizeof(*client));
#endif
  return client;
}

void gasneti_free_client(gasneti_Client_t client)
{
#ifdef GASNETI_CLIENT_FREE_EXTRA
  GASNETI_CLIENT_FREE_EXTRA(client);
#endif
  gasneti_free((/*non-const*/char*)client->_name);
  GASNETI_INIT_MAGIC(client, GASNETI_CLIENT_BAD_MAGIC);
  gasneti_free(client);
}
#endif // _GEX_CLIENT_T


#ifndef _GEX_SEGMENT_T
#ifndef gasneti_import_segment
gasneti_Segment_t gasneti_import_segment(gex_Segment_t _segment) {
  const gasneti_Segment_t _real_segment = GASNETI_IMPORT_POINTER(gasneti_Segment_t,_segment);
  GASNETI_IMPORT_MAGIC(_real_segment, SEGMENT);
  return _real_segment;
}
#endif

#ifndef gasneti_export_segment
gex_Segment_t gasneti_export_segment(gasneti_Segment_t _real_segment) {
  GASNETI_CHECK_MAGIC(_real_segment, GASNETI_SEGMENT_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_Segment_t, _real_segment);
}
#endif

// TODO-EX: probably need to add to a per-client container of some sort
gasneti_Segment_t gasneti_alloc_segment(
                       gasneti_Client_t client,
                       void *addr,
                       uintptr_t size,
                       gex_Flags_t flags,
                       size_t requested_sz)
{
  gasneti_Segment_t segment;
  if (requested_sz) gasneti_assert_uint(requested_sz ,>=, sizeof(*segment));
  size_t alloc_size = requested_sz ? requested_sz : sizeof(*segment);
  segment = gasneti_malloc(alloc_size);
  GASNETI_INIT_MAGIC(segment, GASNETI_SEGMENT_MAGIC);
  segment->_client = client;
  segment->_cdata = NULL;
  segment->_flags = flags;
  segment->_addr = addr;
  segment->_ub = (void*)((uintptr_t)addr + size);
  segment->_size = size;
#ifdef GASNETI_SEGMENT_ALLOC_EXTRA
  GASNETI_SEGMENT_ALLOC_EXTRA(segment);
#else
  if (requested_sz) memset(segment + 1, 0, alloc_size - sizeof(*segment));
#endif
  return segment;
}

void gasneti_free_segment(gasneti_Segment_t segment)
{
#ifdef GASNETI_SEGMENT_FREE_EXTRA
  GASNETI_SEGMENT_FREE_EXTRA(segment);
#endif
  GASNETI_INIT_MAGIC(segment, GASNETI_SEGMENT_BAD_MAGIC);
  gasneti_free(segment);
}
#endif // _GEX_SEGMENT_T


#ifndef _GEX_EP_T
#ifndef gasneti_import_ep
gasneti_EP_t gasneti_import_ep(gex_EP_t _ep) {
  const gasneti_EP_t _real_ep = GASNETI_IMPORT_POINTER(gasneti_EP_t,_ep);
  GASNETI_IMPORT_MAGIC(_real_ep, EP);
  return _real_ep;
}
#endif

#ifndef gasneti_export_ep
gex_EP_t gasneti_export_ep(gasneti_EP_t _real_ep) {
  GASNETI_CHECK_MAGIC(_real_ep, GASNETI_EP_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_EP_t, _real_ep);
}
#endif

// TODO-EX: probably need to add to a per-client container of some sort
extern gasneti_EP_t gasneti_alloc_ep(
                       gasneti_Client_t client,
                       gex_Flags_t flags,
                       size_t requested_sz)
{
  gasneti_EP_t endpoint;
  if (requested_sz) gasneti_assert_uint(requested_sz ,>=, sizeof(*endpoint));
  size_t alloc_size = requested_sz ? requested_sz : sizeof(*endpoint);
  endpoint = gasneti_malloc(alloc_size);
  GASNETI_INIT_MAGIC(endpoint, GASNETI_EP_MAGIC);
  endpoint->_client = client;
  endpoint->_cdata = NULL;
  endpoint->_segment = NULL;
  endpoint->_flags = flags;
  gasneti_amtbl_init(endpoint->_amtbl);
#ifdef GASNETI_EP_ALLOC_EXTRA
  GASNETI_EP_ALLOC_EXTRA(endpoint);
#else
  if (requested_sz) memset(endpoint + 1, 0, alloc_size - sizeof(*endpoint));
#endif
  return endpoint;
}

void gasneti_free_ep(gasneti_EP_t endpoint)
{
#ifdef GASNETI_EP_FREE_EXTRA
  GASNETI_EP_FREE_EXTRA(endpoint);
#endif
  GASNETI_INIT_MAGIC(endpoint, GASNETI_EP_BAD_MAGIC);
  gasneti_free(endpoint);
}
#endif // _GEX_EP_T


#ifndef _GEX_TM_T
#ifndef gasneti_import_tm
gasneti_TM_t gasneti_import_tm(gex_TM_t _tm) {
  const gasneti_TM_t _real_tm = GASNETI_IMPORT_POINTER(gasneti_TM_t,_tm);
  GASNETI_IMPORT_MAGIC(_real_tm, TM);
  return _real_tm;
}
#endif

#ifndef gasneti_export_tm
gex_TM_t gasneti_export_tm(gasneti_TM_t _real_tm) {
  GASNETI_CHECK_MAGIC(_real_tm, GASNETI_TM_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_TM_t, _real_tm);
}
#endif

// TODO-EX: probably need to add to a per-client container of some sort
extern gasneti_TM_t gasneti_alloc_tm(
                       gasneti_EP_t ep,
                       gex_Rank_t rank,
                       gex_Rank_t size,
                       gex_Flags_t flags,
                       size_t requested_sz)
{
  gasneti_assert_uint(rank ,<, size);
  gasneti_assert_uint(size ,>, 0);

  gasneti_assert(ep);
  gasneti_assert(ep->_client);
  const int is_tm0 = (ep->_client->_tm0 == NULL);

  // TM0 is aligned to GASNETI_TM0_ALIGN, and all others to half that
  gasneti_TM_t tm;
  if (requested_sz) gasneti_assert_uint(requested_sz ,>=, sizeof(*tm));
  size_t disalign = (is_tm0 ? 0 : GASNETI_TM0_ALIGN/2);
  size_t actual_sz = (requested_sz ? requested_sz : sizeof(*tm)) + disalign;
  tm = (gasneti_TM_t)(disalign + (uintptr_t)gasneti_malloc_aligned(GASNETI_TM0_ALIGN, actual_sz));

  GASNETI_INIT_MAGIC(tm, GASNETI_TM_MAGIC);
  tm->_ep = ep;
  tm->_cdata = NULL;
  tm->_flags = flags;
  tm->_rank = rank;
  tm->_size = size;
  tm->_coll_team = NULL;
#ifdef GASNETI_TM_ALLOC_EXTRA
  GASNETI_TM_ALLOC_EXTRA(tm);
#else
  if (requested_sz) memset(tm + 1, 0, (actual_sz - disalign) - sizeof(*tm));
#endif
  
  if (is_tm0) {
    gasneti_legacy_alloc_tm_hook(tm); // init g2ex layer if appropriate

    ep->_client->_tm0 = tm;

    // TODO-EX: Please remove this!
    gasneti_assert(! gasneti_thing_that_goes_thunk_in_the_dark);
    gasneti_thing_that_goes_thunk_in_the_dark = tm;
  }

  return tm;
}

void gasneti_free_tm(gasneti_TM_t tm)
{
#ifdef GASNETI_TM_FREE_EXTRA
  GASNETI_TM_FREE_EXTRA(tm);
#endif
  GASNETI_INIT_MAGIC(tm, GASNETI_TM_BAD_MAGIC);
  gasneti_free_aligned((void*)((uintptr_t)tm & (GASNETI_TM0_ALIGN-1)));
}
#endif // _GEX_TM_T

/* ------------------------------------------------------------------------------------ */

#if GASNET_DEBUG
  // Verify that client did actually write to gasnet-allocated buffer
  //
  // gasneti_init_sd_poison(sd) - write a "canary"
  //   For (sd->_size >= gasneti_sd_init_len) writes a "canary" value (also of length
  //   gasneti_sd_init_len) to sd->_addr if (and only if) the buffer is gasnet-owned
  // gasneti_test_sd_poison(addr, len) - test a "canary"
  //   For (len >= gasneti_sd_init_len) looks for the same "canary" value,
  //   returning non-zero if it is present.
  // Note that 'len' at "test" may be less than the one given at "init",
  // as is permitted for the nbytes values passed to Prepare/Commit.

  static uint64_t gasneti_memalloc_envint(const char *name, const char *deflt);
  static void gasneti_memalloc_valset(void *p, size_t len, uint64_t val);
  static const void *gasneti_memalloc_valcmp(const void *p, size_t len, uint64_t val);

  static int gasneti_sd_init_enabled = 1;
  static uint64_t gasneti_sd_init_val = 0; // Value used to initialize gasnet-allocated SrcDesc buffers
  static size_t gasneti_sd_init_len = 128; // Max length to init at Prepare, and min to check at Commit

  extern void gasneti_init_sd_poison(gasneti_AM_SrcDesc_t sd) {
    if (!gasneti_sd_init_enabled) return;
    if (sd->_addr != sd->_gex_buf) return;
    gasneti_assert_uint(((uintptr_t)sd->_addr) % GASNETI_MEDBUF_ALIGNMENT ,==, 0);
    static int isinit = 0;
    if_pf (!isinit) {
      static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
      gasneti_mutex_lock(&lock);
        if (!isinit) {
          gasneti_sd_init_enabled = gasneti_getenv_yesno_withdefault("GASNET_SD_INIT",1);
          gasneti_sd_init_val = gasneti_memalloc_envint("GASNET_SD_INITVAL","NAN");
          gasneti_sd_init_len = MAX((int64_t)1,gasneti_getenv_int_withdefault("GASNET_SD_INITLEN",128,0));
          isinit = 1;
        }
      gasneti_mutex_unlock(&lock);
      if (!gasneti_sd_init_enabled) return;
    } else gasneti_sync_reads();
    if (sd->_size < gasneti_sd_init_len) return;
    gasneti_memalloc_valset(sd->_addr, gasneti_sd_init_len, gasneti_sd_init_val);
  }

  extern int gasneti_test_sd_poison(void *addr, size_t len) { // return non-zero if still poison
    return gasneti_sd_init_enabled &&
           (len >= gasneti_sd_init_len) &&
           !gasneti_memalloc_valcmp(addr, gasneti_sd_init_len, gasneti_sd_init_val);
  }
#endif

/* ------------------------------------------------------------------------------------ */

#ifndef GASNETC_FATALSIGNAL_CALLBACK
#define GASNETC_FATALSIGNAL_CALLBACK(sig)
#endif
#ifndef GASNETC_FATALSIGNAL_CLEANUP_CALLBACK
#define GASNETC_FATALSIGNAL_CLEANUP_CALLBACK(sig)
#endif

void gasneti_defaultSignalHandler(int sig) {
  gasneti_sighandlerfn_t oldsigpipe = NULL;
  const char *signame =  gasnett_signame_fromval(sig);

  gasneti_assert(signame);

  switch (sig) {
    case SIGQUIT:
      /* client didn't register a SIGQUIT handler, so just exit */
      gasnet_exit(1);
      break;
    case SIGABRT:
    case SIGILL:
    case SIGSEGV:
    case SIGBUS:
    case SIGFPE: {
      oldsigpipe = gasneti_reghandler(SIGPIPE, SIG_IGN);

      GASNETC_FATALSIGNAL_CALLBACK(sig); /* give conduit first crack at it */

      gasneti_console_message("Caught a fatal signal", "%s(%i)", signame, sig);

      gasnett_freezeForDebuggerErr(); /* allow freeze */

      gasneti_print_backtrace_ifenabled(STDERR_FILENO); /* try to print backtrace */

      // Try to flush I/O (especially the tracefile) before crashing
      signal(SIGALRM, _exit); alarm(5); 
      gasneti_flush_streams();
      alarm(0);

      (void) gasneti_reghandler(SIGPIPE, oldsigpipe);

      GASNETC_FATALSIGNAL_CLEANUP_CALLBACK(sig); /* conduit hook to kill the job */

      signal(sig, SIG_DFL); /* restore default core-dumping handler and re-raise */
      gasneti_raise(sig);
      break;
    }
    default: 
      /* translate signal to SIGQUIT */
      { static int sigquit_raised = 0;
        if (sigquit_raised) {
          /* sigquit was already raised - we cannot safely reraise it, so just die */
          _exit(1);
        } else sigquit_raised = 1;
      }

      oldsigpipe = gasneti_reghandler(SIGPIPE, SIG_IGN);
      gasneti_console_message("Caught a signal", "%s(%i)", signame, sig);
      (void) gasneti_reghandler(SIGPIPE, oldsigpipe);

      gasneti_raise(SIGQUIT);
  }
}

extern int gasneti_set_waitmode(int wait_mode) {
  const char *desc = NULL;
  GASNETI_CHECKINIT();
  switch (wait_mode) {
    case GASNET_WAIT_SPIN:      desc = "GASNET_WAIT_SPIN"; break;
    case GASNET_WAIT_BLOCK:     desc = "GASNET_WAIT_BLOCK"; break;
    case GASNET_WAIT_SPINBLOCK: desc = "GASNET_WAIT_SPINBLOCK"; break;
    default:
      GASNETI_RETURN_ERRR(BAD_ARG, "illegal wait mode");
  }
  GASNETI_TRACE_PRINTF(I, ("gasnet_set_waitmode(%s)", desc));
  #ifdef gasnetc_set_waitmode
    gasnetc_set_waitmode(wait_mode);
  #endif
  gasneti_wait_mode = wait_mode;
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/* Global environment variable handling */

extern char **environ; 

static void gasneti_serializeEnvironment(uint8_t **pbuf, int *psz) {
  /* flatten a snapshot of the environment to make it suitable for transmission
   * here we assume the standard representation where a pointer to the environment 
   * is stored in a global variable 'environ' and the environment is represented as an array 
   * of null-terminated strings where each has the form 'key=value' and value may be empty, 
   * and the final string pointer is a NULL pointer
   * we flatten this into a list of null-terminated 'key=value' strings, 
   * terminated with a double-null
   */
  uint8_t *buf; 
  uint8_t *p;
  int i;
  int totalEnvSize = 0;
  if (!environ) {
    /* T3E stupidly omits environ support, despite documentation to the contrary */
    GASNETI_TRACE_PRINTF(I,("WARNING: environ appears to be empty -- ignoring it"));
    *pbuf = NULL;
    *psz = 0;
    return;
  }
  for(i = 0; environ[i]; i++) 
    totalEnvSize += strlen(environ[i]) + 1;
  totalEnvSize++;

  buf = (uint8_t *)gasneti_malloc(totalEnvSize);
  p = buf;
  p[0] = 0;
  for(i = 0; environ[i]; i++) {
    strcpy((char*)p, environ[i]);
    p += strlen((char*)p) + 1;
    }
  *p = 0;
  gasneti_assert_int((p+1) - buf ,==, totalEnvSize);

  *pbuf = buf;
  *psz = totalEnvSize;
}

extern char *gasneti_globalEnv;

typedef struct {
  int sz;
  uint64_t checksum;
} gasneti_envdesc_t;

/* do the work necessary to setup the global environment for use by gasneti_getenv
   broadcast the environment variables from one node to all nodes
   Note this currently assumes that at least one of the compute nodes has the full
    environment - systems where the environment is not propagated to any compute node
    will need something more sophisticated.
   exchangefn is required function for exchanging data 
   broadcastfn is optional (can be NULL) but highly recommended for scalability
 */
extern void gasneti_setupGlobalEnvironment(gex_Rank_t numnodes, gex_Rank_t mynode,
                                           gasneti_bootstrapExchangefn_t exchangefn,
                                           gasneti_bootstrapBroadcastfn_t broadcastfn) {
  uint8_t *myenv; 
  int sz; 
  uint64_t checksum;
  gasneti_envdesc_t myenvdesc;
  gasneti_envdesc_t *allenvdesc;

  gasneti_assert(exchangefn);

  gasneti_serializeEnvironment(&myenv,&sz);
  checksum = gasneti_checksum(myenv,sz);

  myenvdesc.sz = sz;
  myenvdesc.checksum = checksum;

  allenvdesc = gasneti_malloc(numnodes*sizeof(gasneti_envdesc_t));
  /* gather environment description from all nodes */
  (*exchangefn)(&myenvdesc, sizeof(gasneti_envdesc_t), allenvdesc);

  { /* see if the node environments differ and find the largest */
    int i;
    int rootid = 0;
    int identical = 1;
    gasneti_envdesc_t rootdesc = allenvdesc[rootid];
    for (i=1; i < numnodes; i++) {
      if (rootdesc.checksum != allenvdesc[i].checksum || 
          rootdesc.sz != allenvdesc[i].sz) 
          identical = 0;
      if (allenvdesc[i].sz > rootdesc.sz) { 
        /* assume the largest env is the one we want */
        rootdesc = allenvdesc[i];
        rootid = i;
      }
    }
    if (identical) { /* node environments all identical - don't bother to propagate */
      gasneti_free(allenvdesc);
      gasneti_free(myenv);
      return;
    } else {
      int envsize = rootdesc.sz;
      gasneti_globalEnv = gasneti_malloc(envsize);
      gasneti_leak(gasneti_globalEnv);
      if (broadcastfn) {
        (*broadcastfn)(myenv, envsize, gasneti_globalEnv, rootid);
      } else {
        /* this is wasteful of memory and bandwidth, and non-scalable */
        char *tmp = gasneti_malloc(envsize*numnodes);
        memcpy(tmp+mynode*envsize, myenv, sz);
        (*exchangefn)(tmp+mynode*envsize, envsize, tmp);
        memcpy(gasneti_globalEnv, tmp+rootid*envsize, envsize);
        gasneti_free(tmp);
      }
      gasneti_assert_uint(gasneti_checksum(gasneti_globalEnv,envsize) ,==, rootdesc.checksum);
      gasneti_free(allenvdesc);
      gasneti_free(myenv);
      return;
    }
  }

}

/* decode src into dst, arguments permitted to overlap exactly */
extern size_t gasneti_decodestr(char *dst, const char *src) {
  #define IS_HEX_DIGIT(c)  (isdigit(c) || (isalpha(c) && toupper(c) <= 'F'))
  #define VAL_HEX_DIGIT(c) ((unsigned int)(isdigit(c) ? (c)-'0' : 10 + toupper(c) - 'A'))
  size_t dstidx = 0;
  const char *p = src;
  gasneti_assert(src && dst);
  while (*p) {
    char c;
    if (p[0] == '%' && p[1] == '0' && 
        p[2] && IS_HEX_DIGIT(p[2]) && p[3] && IS_HEX_DIGIT(p[3])) {
      c = (char)(VAL_HEX_DIGIT(p[2]) << 4) | VAL_HEX_DIGIT(p[3]);
      p += 4;
    } else c = *(p++);
    dst[dstidx++] = c;
  }
  dst[dstidx] = '\0';
  return dstidx;
  #undef VAL_HEX_DIGIT
  #undef IS_HEX_DIGIT
}

extern const char *gasneti_decode_envval(const char *val) {
  static struct _gasneti_envtable_S {
    const char *pre;
    char *post;
    struct _gasneti_envtable_S *next;
  } *gasneti_envtable = NULL;
  static gasneti_mutex_t gasneti_envtable_lock = GASNETI_MUTEX_INITIALIZER;
  static int firsttime = 1;
  static int decodeenv = 1;
  if (firsttime) {
    decodeenv = !gasneti_getenv("GASNET_DISABLE_ENVDECODE");
    if (gasneti_init_done && gasneti_mynode != (gex_Rank_t)-1) {
      gasneti_envstr_display("GASNET_DISABLE_ENVDECODE",(decodeenv?"NO":"YES"),decodeenv);
      gasneti_sync_writes();
      firsttime = 0;
    }
  } else gasneti_sync_reads();
  if (!decodeenv) return val;

  if (strstr(val,"%0")) {
    struct _gasneti_envtable_S *p;
    gasneti_mutex_lock(&gasneti_envtable_lock);
      p = gasneti_envtable;
      while (p) {
        if (!strcmp(val, p->pre)) break;
        p = p->next;
      }
      if (p) val = p->post;
      else { /* decode it and save the result (can't trust setenv to safely set it back) */
        struct _gasneti_envtable_S *newentry = gasneti_malloc(sizeof(struct _gasneti_envtable_S));
        newentry->pre = gasneti_strdup(val);
        newentry->post = gasneti_malloc(strlen(val)+1);
        gasneti_decodestr(newentry->post, newentry->pre);
        if (!strcmp(newentry->post, newentry->pre)) { 
          gasneti_free(newentry); 
        } else {
          newentry->next = gasneti_envtable;
          gasneti_envtable = newentry;
          val = newentry->post;
        }
      }
    gasneti_mutex_unlock(&gasneti_envtable_lock);
  }
  return val;
}

/* gasneti_verboseenv_fn returns an expression that defines whether the given process should report to the console
   on env queries - needs to work before gasnet_init
   1 = yes, 0 = no, -1 = not yet / don't know
 */
#ifndef GASNETI_ENV_OUTPUT_NODE
#define GASNETI_ENV_OUTPUT_NODE()  (gasneti_mynode == 0)
#endif
extern int _gasneti_verboseenv_fn(void) {
  static int verboseenv = -1;
  if (verboseenv == -1) {
    if (gasneti_init_done && gasneti_mynode != (gex_Rank_t)-1) {
      #if GASNET_DEBUG_VERBOSE
        verboseenv = GASNETI_ENV_OUTPUT_NODE();
      #else
        verboseenv = !!gasneti_getenv("GASNET_VERBOSEENV") && GASNETI_ENV_OUTPUT_NODE();
      #endif
      gasneti_sync_writes();
    }
  } else gasneti_sync_reads();
  return verboseenv;
}
extern int (*gasneti_verboseenv_fn)(void);
int (*gasneti_verboseenv_fn)(void) = &_gasneti_verboseenv_fn;

extern const char * gasneti_backtraceid(void) {
  static char myid[255];
  sprintf(myid, "[%i] ", (int)gasneti_mynode);
  return myid;
}

extern void gasneti_decode_args(int *argc, char ***argv) {
  static int firsttime = 1;
  if (!firsttime) return; /* ignore subsequent calls, to allow early decode */
  firsttime = 0;
  if (!gasneti_getenv_yesno_withdefault("GASNET_DISABLE_ARGDECODE",0)) {
    int argidx;
    char **origargv = *argv;
    for (argidx = 0; argidx < *argc; argidx++) {
      if (strstr((*argv)[argidx], "%0")) {
        char *tmp = gasneti_strdup((*argv)[argidx]);
        int newsz = gasneti_decodestr(tmp, tmp);
        if (newsz == strlen((*argv)[argidx])) gasneti_free(tmp); /* no change */
        else {
          int i, newcnt = 0;
          for (i = 0; i < newsz; i++) if (!tmp[i]) newcnt++; /* count growth due to inserted NULLs */
          if (newcnt == 0) { /* simple parameter replacement */
            (*argv)[argidx] = tmp;
          } else { /* need to grow argv */
            char **newargv = gasneti_malloc(sizeof(char *)*(*argc+1+newcnt));
            memcpy(newargv, *argv, sizeof(char *)*argidx);
            newargv[argidx] = tmp; /* base arg */
            memcpy(newargv+argidx+newcnt, (*argv)+argidx, sizeof(char *)*(*argc - argidx - 1));
            for (i = 0; i < newsz; i++) /* hook up new args */
              if (!tmp[i]) newargv[1+argidx++] = &(tmp[i+1]); 
            *argc += newcnt;
            if (*argv != origargv) gasneti_free(*argv);
            *argv = newargv;
            (*argv)[*argc] = NULL; /* ensure null-termination of arg list */
          }
        } 
      }
    }
  }
}

/* Propagate requested env vars from GASNet global env to the local env */

void (*gasneti_propagate_env_hook)(const char *, int) = NULL; // spawner- or conduit-specific hook

extern void gasneti_propagate_env_helper(const char *environ, const char * keyname, int flags) {
  const int is_prefix = flags & GASNETI_PROPAGATE_ENV_PREFIX;
  const char *p = environ;

  gasneti_assert(environ);
  gasneti_assert(keyname && !strchr(keyname,'='));

  int keylen = strlen(keyname);
  while (*p) {
    if (!strncmp(keyname, p, keylen) && (is_prefix || p[keylen] == '=')) {
      gasneti_assert(NULL != strchr(p+keylen, '='));
      char *var = gasneti_strdup(p);
      char *val = strchr(var, '=');
      *(val++) = '\0';
      val = (char *)gasneti_decode_envval(val);
      gasnett_setenv(var, val);
      GASNETI_TRACE_PRINTF(I,("gasneti_propagate_env(%s) => '%s'", var, val));
      gasneti_free(var);
      if (!is_prefix) break;
    }
    p += strlen(p) + 1;
  }
}

extern void gasneti_propagate_env(const char * keyname, int flags) {
  gasneti_assert(keyname);
  gasneti_assert(NULL == strchr(keyname, '='));

  // First look for matches in gasneti_globalEnv (if any)
  if (gasneti_globalEnv) {
    gasneti_propagate_env_helper(gasneti_globalEnv, keyname, flags);
  }

  // Next allow conduit-specific getenv (if any) to overwrite
  if (gasneti_propagate_env_hook) {
    gasneti_propagate_env_hook(keyname, flags);
  }
}


/* Process environment for exittimeout.
 * If (GASNET_EXITTIMEOUT is set), it is returned
 * else return = min(GASNET_EXITTIMEOUT_MAX,
 *                   GASNET_EXITTIMEOUT_MIN + gasneti_nodes * GASNET_EXITTIMEOUT_FACTOR)
 * Where all the GASNET_EXITTIMEOUT* tokens above are env vars.
 * The arguments are defaults for MAX, MIN and FACTOR, and the lowest value to allow.
 */
extern double gasneti_get_exittimeout(double dflt_max, double dflt_min, double dflt_factor, double lower_bound)
{
  double my_max = gasneti_getenv_dbl_withdefault("GASNET_EXITTIMEOUT_MAX", dflt_max);
  double my_min = gasneti_getenv_dbl_withdefault("GASNET_EXITTIMEOUT_MIN", dflt_min);
  double my_factor = gasneti_getenv_dbl_withdefault("GASNET_EXITTIMEOUT_FACTOR", dflt_factor);
  double result = gasneti_getenv_dbl_withdefault("GASNET_EXITTIMEOUT",
						 MIN(my_max, my_min + my_factor * gasneti_nodes));

  if (result < lower_bound) {
    gasneti_assert_dbl(MIN(dflt_max, dflt_min + dflt_factor * gasneti_nodes) ,>=, lower_bound);
    if (gasneti_getenv("GASNET_EXITTIMEOUT")) {
      gasneti_fatalerror("If used, environment variable GASNET_EXITTIMEOUT must be set to a value no less than %g", lower_bound);
    } else {
      gasneti_fatalerror("Environment variables GASNET_EXITTIMEOUT_{MAX,MIN,FACTOR} yield a timeout less than %g seconds", lower_bound);
    }
  }

  return result;
}

/* ------------------------------------------------------------------------------------ */
/* Bits for conduits which want/need to override pthread_create() */

#if defined(PTHREAD_MUTEX_INITIALIZER) /* only if pthread.h available */ && !GASNET_SEQ
  #ifndef GASNETC_PTHREAD_CREATE_OVERRIDE
    /* Default is just pass through */
    #define GASNETC_PTHREAD_CREATE_OVERRIDE(create_fn, thread, attr, start_routine, arg) \
      (*create_fn)(thread, attr, start_routine, arg)
  #endif

  int gasneti_pthread_create(gasneti_pthread_create_fn_t *create_fn, pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    // There is no portable way to printf a pointer-to-function. This way avoids warnings with -pedantic
    union { void *vp; gasneti_pthread_create_fn_t *cfp; void *(*sfp)(void *); } ucreate, ustart; 
    ucreate.cfp = create_fn;
    ustart.sfp = start_routine;
    GASNETI_TRACE_PRINTF(I, ("gasneti_pthread_create(%p, %p, %p, %p, %p)", ucreate.vp, (void *)thread, (void *)attr, ustart.vp, arg));
    return GASNETC_PTHREAD_CREATE_OVERRIDE(create_fn, thread, attr, start_routine, arg);
  }
#endif

/* ------------------------------------------------------------------------------------ */
static void gasneti_check_portable_conduit(void) { /* check for portable conduit abuse */
  char mycore[80], myext[80];
  char const *mn = GASNET_CORE_NAME_STR;
  char *m;
  m = mycore; while (*mn) { *m = tolower(*mn); m++; mn++; }
  *m = '\0';
  mn = GASNET_EXTENDED_NAME_STR;
  m = myext; while (*mn) { *m = tolower(*mn); m++; mn++; }
  *m = '\0';
  int haveOmniPath = 0; // bug 3609: this oddball needs special handling
  #if PLATFORM_OS_LINUX
    const char *filename = "/sys/class/infiniband/hfi1_0/board_id";
    FILE *fp = fopen(filename,"r");
    if (fp) {
      char buffer[128];
      size_t r = fread(&buffer, 1, sizeof(buffer), fp);
      if (r) { // eg: "Intel Omni-Path HFI Adapter 100 Series, 1 Port, PCIe x16"
        buffer[r-1] = 0;
        if (strstr(buffer, "Omni-Path")) haveOmniPath = 1;
      }
      fclose(fp);
    }
  #endif
  
  if ( /* is a portable network conduit */
         (!strcmp("mpi",mycore) && !strcmp("reference",myext))
      || (!strcmp("udp",mycore) && !strcmp("reference",myext))
      || (!strcmp("ofi",mycore) && !strcmp("ofi",myext) && !haveOmniPath)
      ) {
    const char *p = GASNETI_CONDUITS;
    char natives[255];
    char reason[255];
    natives[0] = 0;
    reason[0] = 0;
    while (*p) { /* look for configure-detected native conduits */
      #define GASNETI_CONDUITS_DELIM " ,/;\t\n"
      char name[80];
      p += strspn(p,GASNETI_CONDUITS_DELIM);
      if (*p) {
        int len = strcspn(p,GASNETI_CONDUITS_DELIM);
        strncpy(name, p, len);
        name[len] = 0;
        p += len;
        p += strspn(p,GASNETI_CONDUITS_DELIM);
        /* Ignore the portable conduits */
        if (!strcmp(name,"smp")) continue;
        if (!strcmp(name,"mpi")) continue;
        if (!strcmp(name,"udp")) continue;
        if (!strcmp(name,"ofi") && !haveOmniPath) continue;
        if (!strcmp(name,"ibv") && haveOmniPath) continue; // never recommend ibv over OPA
        if (strlen(natives)) strcat(natives,", ");
        strcat(natives,name);
      }
      #undef GASNETI_CONDUITS_DELIM
    }
    if (natives[0]) {
      sprintf(reason, "WARNING: Support was detected for native GASNet conduits: %s",natives);
    } else { /* look for hardware devices supported by native conduits */
      struct { 
        const char *filename;
        mode_t filemode;
        const char *desc;
        int hwid;
      } known_devs[] = {
        #if PLATFORM_OS_LINUX && PLATFORM_ARCH_IA64 && GASNET_SEQ
          { "/dev/hw/cpunum",      S_IFDIR, "SGI Altix", 0 },
          { "/dev/xpmem",          S_IFCHR, "SGI Altix", 0 },
        #endif
        { "/dev/infiniband/uverbs0",     S_IFCHR, "InfiniBand IBV", 2 },  /* OFED 1.0 */
        { "/dev/infiniband/ofs/uverbs0", S_IFCHR, "InfiniBand IBV", 2 },  /* Solaris */
        #if !GASNET_SEGMENT_EVERYTHING
          { "/dev/kgni0",            S_IFCHR, "Cray Gemini", 6 },
          { "/proc/kgnilnd",         S_IFDIR, "Cray Gemini", 6 },
        #endif
        { "/list_terminator", S_IFDIR, "", 9999 }
      };
      int i, lim = sizeof(known_devs)/sizeof(known_devs[0]);
      for (i = 0; i < lim; i++) {
        struct stat stat_buf;
        if (!stat(known_devs[i].filename,&stat_buf) && 
            (!known_devs[i].filemode || (known_devs[i].filemode & stat_buf.st_mode))) {
            int hwid = known_devs[i].hwid;
            if (hwid == 2 && haveOmniPath) continue; // never recommend ibv over OPA
            if (strlen(natives)) strcat(natives,", ");
            strcat(natives,known_devs[i].desc);
            while (i < lim && hwid == known_devs[i].hwid) i++; /* don't report a network twice */
        }
      }
      #if PLATFORM_OS_CNL
        if (strlen(natives)) strcat(natives,", ");
        strcat(natives,"Cray Gemini (XE and XK) or Aries (XC)");
      #endif
      if (natives[0]) {
        sprintf(reason, "WARNING: This system appears to contain recognized network hardware: %s\n"
                        "WARNING: which is supported by a GASNet native conduit, although\n"
                        "WARNING: it was not detected at configure time (missing drivers?)",
                        natives);
      }
    }
    if (reason[0] && !gasneti_getenv_yesno_withdefault("GASNET_QUIET",0) && gasneti_mynode == 0) {
      fprintf(stderr,"WARNING: Using GASNet's %s-conduit, which exists for portability convenience.\n"
                     "%s\n"
                     "WARNING: You should *really* use the high-performance native GASNet conduit\n"
                     "WARNING: if communication performance is at all important in this program run.\n",
              mycore, reason);
      fflush(stderr);
    }
  }
}

static void gasneti_check_architecture(void) { // check for bad build configurations
  #if PLATFORM_OS_CNL && PLATFORM_ARCH_X86_64 // bug 3743, verify correct processor tuning
  { FILE *fp = fopen("/proc/cpuinfo","r");
    char model[255];
    if (!fp) gasneti_fatalerror("Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));
    while (!feof(fp) && fgets(model, sizeof(model), fp)) {
      if (strstr(model,"model name")) break;
    }
    fclose(fp);
    GASNETI_TRACE_PRINTF(I,("CPU %s",model));
    int isKNL = !!strstr(model, "Phi");
    #ifdef __CRAY_MIC_KNL  // module craype-mic-knl that tunes for AVX512
      const char *warning = isKNL ? 0 :
      "WARNING: This executable was optimized for MIC KNL (module craype-mic-knl) but run on another processor!\n";
    #else // some other x86 tuning mode
      const char *warning = isKNL ? 
      "WARNING: This executable is running on a MIC KNL architecture, but was not optimized for MIC KNL.\n"
      "WARNING: This often has a MAJOR impact on performance. Please re-build with module craype-mic-knl!\n"
      : 0;
    #endif
    if (warning && gasneti_mynode == 0) {
      fprintf(stderr, warning);
      fflush(stderr);
    }
  }
  #endif
}

/* ------------------------------------------------------------------------------------ */
/* Nodemap handling
 */

gex_Rank_t *gasneti_nodemap = NULL;
gasneti_nodegrp_t gasneti_myhost = {NULL,0,(gex_Rank_t)(-1),0,(gex_Rank_t)(-1)};
gasneti_nodegrp_t gasneti_mysupernode = {NULL,0,(gex_Rank_t)(-1),0,(gex_Rank_t)(-1)};
gasnet_nodeinfo_t *gasneti_nodeinfo = NULL;

/* This code is "good" for all "sensible" process layouts, where "good"
 * means identifing all sharing for such a mapping in one pass and the
 * term "sensible" includes:
 *   "Block" layouts like       |0.1.2.3|4.5.6.7|8.9._._|
 *                           or |0.1.2.3|4.5.6._|7.8.9._|
 *   "Block-cyclic" like        |0.1.6.7|2.3.8.9|4.5._._|
 *   "Cyclic/Round-robin" like  |0.3.6.9|1.4.7._|2.5.8._|
 *   and all 24 permutations of the XYZT dimensions on the BG/P.
 *
 * This is also "safe" for an arbitrary mapping, but may fail to
 * identify some or all of the potential sharing in such a case.
 */
static void gasneti_nodemap_helper_linear(const char *ids, size_t sz, size_t stride) {
  gex_Rank_t i, prev, base;
  const char *p, *base_p, *prev_p;

  prev   = base   = gasneti_nodemap[0] = 0;
  prev_p = base_p = ids;
  p = base_p + stride;

  for (i = 1; i < gasneti_nodes; ++i, p += stride) {
    if (!memcmp(p, prev_p, sz)) {                  /* Repeat the previous id */
      gasneti_nodemap[i] = gasneti_nodemap[prev];
      prev += 1;       prev_p += stride;
      continue;
    }

    gasneti_nodemap[i] = i;
    if (!memcmp(p, ids, sz)) {                     /* Restart the first "row" */
      prev = 0;        prev_p = ids;
    } else if (!memcmp(p, base_p, sz)) {           /* Restart the previous "row" */
      prev = base;     prev_p = base_p;
    } else if (!memcmp(p, prev_p + stride, sz)) {  /* Continue current "row" if any */
      prev += 1;       prev_p += stride;
    } else {                                       /* Begin a new "row" */
      prev = base = i; prev_p = base_p = p;
    }
    gasneti_nodemap[i] = gasneti_nodemap[prev];
  }
}

/* This code is "good" for all possible process layouts, where "good"
 * means identifing all sharing.  However, the running time is O(n*log(n)).
 */
static struct {
  const char *ids;
  size_t sz;
  size_t stride;
} _gasneti_nodemap_sort_aux;
static int _gasneti_nodemap_sort_fn(const void *a, const void *b) {
  gex_Rank_t key1 = *(const gex_Rank_t *)a;
  gex_Rank_t key2 = *(const gex_Rank_t *)b;
  const char *val1 = _gasneti_nodemap_sort_aux.ids + key1 * _gasneti_nodemap_sort_aux.stride;
  const char *val2 = _gasneti_nodemap_sort_aux.ids + key2 * _gasneti_nodemap_sort_aux.stride;
  int retval = memcmp(val1, val2, _gasneti_nodemap_sort_aux.sz);
  if (!retval) { /* keep sort stable */
    gasneti_assert_uint(key1 ,!=, key2);
    retval = (key1 < key2) ? -1 : 1;
  }
  return retval;
}
static void gasneti_nodemap_helper_qsort(const char *ids, size_t sz, size_t stride) {
  gex_Rank_t *work    = gasneti_malloc(gasneti_nodes * sizeof(gex_Rank_t));
  const char *prev_id;
  int i, prev; /* If these are gex_Rank_t then bug 2634 can crash XLC */

  _gasneti_nodemap_sort_aux.ids    = ids;
  _gasneti_nodemap_sort_aux.sz     = sz;
  _gasneti_nodemap_sort_aux.stride = stride;
  for (i = 0; i < gasneti_nodes; ++i) work[i] = i;
  qsort(work, gasneti_nodes, sizeof(gex_Rank_t), &_gasneti_nodemap_sort_fn);

  prev = work[0];
  gasneti_nodemap[prev] = prev;
  prev_id = ids + prev*stride;
  for (i = 1; i < gasneti_nodes; ++i) {
    int node = work[i]; /* Also subject to bug 2634 */
    const char *tmp_id = ids + node*stride;
    prev = gasneti_nodemap[node] = memcmp(tmp_id, prev_id, sz) ? node : prev;
    prev_id = tmp_id;
  }
  gasneti_free(work);
}

/* gasneti_nodemap_helper
 * Construct a nodemap from a vector of "IDs"
 */
GASNETI_NEVER_INLINE(gasneti_nodemap_helper,
static void gasneti_nodemap_helper(const void *ids, size_t sz, size_t stride)) {
  #ifndef GASNETC_DEFAULT_NODEMAP_EXACT
    // Default to slow-but-steady (it wins the race)
    // However, see Bug 3770 - RFE: restore default linear-time nodemap behavior
    #define GASNETC_DEFAULT_NODEMAP_EXACT 1
  #endif
  gasneti_assert(ids);
  gasneti_assert_uint(sz ,>, 0);
  gasneti_assert_uint(stride ,>=, sz);

  if (gasneti_getenv_yesno_withdefault("GASNET_NODEMAP_EXACT",GASNETC_DEFAULT_NODEMAP_EXACT)) {
    /* "exact" but potentially costly */
    gasneti_nodemap_helper_qsort(ids, sz, stride);
  } else {
    /* cheap and correct for all "normal" cases */
    gasneti_nodemap_helper_linear(ids, sz, stride);
  }
}

/* Last-resort nodemap constructor
 * Used when neither platform nor conduit can provide any IDs,
 * or when no exchangefn is available to disseminate them.
 */
void gasneti_nodemap_trivial(void) {
  gex_Rank_t i;
  for (i = 0; i < gasneti_nodes; ++i) gasneti_nodemap[i] = i;
}

// gasneti_hosthash(): 64-bit hash of hostname
//
// NOTE: gasneti_checksum() is not suitable
// e.g. "4001.0004" and "1001.0001" hash the same, and when
// we fold down to 32-bits the problem would get even worse.
// At 32-bits the cancellation is at period 4, so that names
// "c03-00", "c13-01" and "c23-02" share the same hash, as
// would the pair "172.16.0.6" and "172.18.0.8".
extern uint64_t gasneti_hosthash(void) {
  const char *myname = gasneti_gethostname();
  const uint8_t *buf = (uint8_t *)myname;
  size_t len = strlen(myname);
  uint64_t csum = 0;
  for (int i=0;i<len;i++) {
    uint8_t c = *(buf++);
    /* The "c = ..." squeezes ASCII down to 6 bits, while encoding
     * all chars valid in hostnames and IP addresses (IPV4 and IPV6).
     * A unique value is assigned to each of the digits, the lower
     * case letters, '-', '.' and ':'.  The upper case letters map
     * to the same values as the corresponding lower-case.
     */
    c = ((c & 0x40) >> 1) | (c & 0x1f);
    csum = ((csum << 6) | ((csum >> 58) & 0x3F)) ^ c;
  }
  return csum;
}

/* Wrapper around gethostid() */
extern uint32_t gasneti_gethostid(void) {
    static uint32_t myid = 0;

    if_pf (!myid) {
    #if PLATFORM_OS_CYGWIN
      /* gethostid() is known to be unreliable - we'll hash the hostname */
    #elif HAVE_GETHOSTID
      myid = (uint32_t)gethostid();
    #endif

      /* Fall back to hashing the hostname if the hostid is obviously invalid */
      if (!myid || !(~myid)        /* 0.0.0.0 or 255.255.255.255 */
          || (myid == 0x7f000001)  /* All 12 distinct permutations of 127.0.0.1: */
          || (myid == 0x7f000100)
          || (myid == 0x7f010000)
          || (myid == 0x007f0001)
          || (myid == 0x007f0100)
          || (myid == 0x017f0000)
          || (myid == 0x00007f01)
          || (myid == 0x00017f00)
          || (myid == 0x01007f00)
          || (myid == 0x0000017f)
          || (myid == 0x0001007f)
          || (myid == 0x0100007f)) {
        uint64_t csum = gasneti_hosthash();
        myid = GASNETI_HIWORD(csum) ^ GASNETI_LOWORD(csum);
      }
    }

    return myid;
}

/* Platform-depended default nodemap constructor
 * Used when no conduit-specific IDs are provided.
 */
static void gasneti_nodemap_dflt(gasneti_bootstrapExchangefn_t exchangefn) {
#if !HAVE_GETHOSTID
    /* Nodes are either (at least effectively) single process,
     * or we don't have a usable gethostid().  So, build a trivial nodemap. */
    gasneti_nodemap_trivial();
#else
    /* Construct nodemap from gethostid and conduit-provided exchangefn 
     */
    uint32_t *allids = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));
    uint32_t myid = gasneti_gethostid();
  
    gasneti_assert(exchangefn);
    (*exchangefn)(&myid, sizeof(uint32_t), allids);

    gasneti_nodemap_helper(allids, sizeof(uint32_t), sizeof(uint32_t));

    gasneti_free(allids);
#endif
}

/* gasneti_nodemapParse()
 *
 * Performs "common" tasks after gasneti_nodemap[] has been constucted.
 * A conduit which builds a gasneti_nodemap[] w/o calling gasneti_nodemapInit()
 * should still call this function to perform the "common" work.
 *
 * Constructs:
 *   gasneti_nodeinfo[] = array of length gasneti_nodes of supernode ids and mmap offsets
 * and fills in the fields in gasneti_mysupernode:
 *   gasneti_mysupernode.nodes       array of nodes in my supernode
 *   gasneti_mysupernode.node_count  count of nodes in my supernode (length of 'nodes' array)
 *   gasneti_mysupernode.node_rank   my ranks in 'nodes' array
 *   gasneti_mysupernode.grp_count   number of supernodes in the job
 *   gasneti_mysupernode.grp_rank    my supernode's rank within all supernodes
 *   gasneti_myhost.*                same quantities for 'host' rather than for 'supernode'
 * Those first five quantities are also available via the following respective aliases:
 *   gasneti_nodemap_local[]
 *   gasneti_nodemap_local_count
 *   gasneti_nodemap_local_rank
 *   gasneti_nodemap_global_count
 *   gasneti_nodemap_global_rank
 *
 * NOTE: may modify gasneti_nodemap[] if env var GASNET_SUPERNODE_MAXSIZE is set,
 *        or if gasneti_nodemap_local_count would exceed GASNETI_PSHM_MAX_NODES.
 * TODO: splitting by socket or other criteria for/with GASNET_SUPERNODE_MAXSIZE.
 * TODO: keep widths around for conduits to use? (at least ibv and aries both use)
 */
extern void gasneti_nodemapParse(void) {
  gex_Rank_t i,j,limit;
  gex_Rank_t initial,final;

  struct { /* TODO: alloca? */
    gex_Rank_t width;
    gex_Rank_t h_lead;
    gex_Rank_t sn_lead;
    gex_Rank_t host;
    gex_Rank_t supernode;
  } *s = gasneti_calloc(gasneti_nodes, sizeof(*s));

  gasneti_assert(gasneti_nodemap);
  gasneti_assert(gasneti_nodemap[0] == 0);
  gasneti_assert_uint(gasneti_nodemap[gasneti_mynode] ,<=, gasneti_mynode);

  /* Check for user-imposed limit: 0 (or negative) means no limit */
#if GASNET_PSHM
  limit = gasneti_getenv_int_withdefault("GASNET_SUPERNODE_MAXSIZE", 0, 0);
 #if GASNET_CONDUIT_SMP
  if (limit && !gasneti_mynode) {
    fprintf(stderr, "WARNING: ignoring GASNET_SUPERNODE_MAXSIZE for smp-conduit with PSHM.\n");
    fflush(stderr);
  }
  limit = gasneti_nodes;
 #else
  if (limit <= 0) {
     limit = GASNETI_PSHM_MAX_NODES;
  } else if (limit > GASNETI_PSHM_MAX_NODES) {
     gasneti_fatalerror("GASNET_SUPERNODE_MAXSIZE %d exceeds GASNETI_PSHM_MAX_NODES (%d)", limit, GASNETI_PSHM_MAX_NODES);
  }
 #endif
#else
  limit = 1; /* No PSHM */
#endif

  gasneti_assert(!gasneti_nodeinfo);
  gasneti_nodeinfo = gasneti_calloc(gasneti_nodes, sizeof(gasnet_nodeinfo_t));
  gasneti_leak(gasneti_nodeinfo);

  /* First pass: 
   * + Apply the supernode size limit, if any.
   * + Determine the counts and ranks
   * + Construct gasneti_nodeinfo[].{host,supernode}
   */
  initial = gasneti_nodemap[gasneti_mynode];
  for (i = 0; i < gasneti_nodes; ++i) {
    const gex_Rank_t n = gasneti_nodemap[i];
    const gex_Rank_t width = s[n].width++;
    const gex_Rank_t lrank = width % limit;
    if (!width) { /* First node on host */
      s[n].host = gasneti_myhost.grp_count++;
    }
    if (!lrank) { /* First node on supernode */
      s[n].sn_lead = i;
      s[n].supernode = gasneti_mysupernode.grp_count++;
    }
    if (i == gasneti_mynode) {
      gasneti_mysupernode.node_rank = lrank;
      gasneti_myhost.node_rank = width;
    }
    s[i].h_lead = n;
    gasneti_nodemap[i] = s[n].sn_lead;
    gasneti_nodeinfo[i].supernode = s[n].supernode;
    gasneti_nodeinfo[i].host = s[n].host;
  }
  final = gasneti_nodemap[gasneti_mynode];
  gasneti_mysupernode.node_count = (final == s[initial].sn_lead) ? (((s[initial].width - 1) % limit) + 1) : limit;
  gasneti_myhost.node_count = s[initial].width;
  gasneti_mysupernode.grp_rank = gasneti_nodeinfo[gasneti_mynode].supernode;
  gasneti_myhost.grp_rank = gasneti_nodeinfo[gasneti_mynode].host;

  /* Second pass: Construct arrays of local nodes */
  gasneti_assert_uint(gasneti_myhost.node_count ,>=, gasneti_mysupernode.node_count);
  gasneti_myhost.nodes = gasneti_malloc(gasneti_myhost.node_count*sizeof(gex_Rank_t));
  gasneti_leak(gasneti_myhost.nodes);
  for (i = initial, j = 0; j < gasneti_myhost.node_count; ++i) {
    gasneti_assert_uint(i ,<, gasneti_nodes);
    if (s[i].h_lead == initial) {
      if (i == final) gasneti_mysupernode.nodes = gasneti_myhost.nodes + j;
      gasneti_myhost.nodes[j++] = i;
    }
  }
  gasneti_assert(gasneti_mysupernode.nodes != NULL);

  gasneti_free(s);

  #if GASNET_DEBUG_VERBOSE
  if (!gasneti_mynode) {
    for (i = 0; i < gasneti_nodes; ++i) {
      fprintf(stderr, "gasneti_nodemap[%i] = %i\n", (int)i, (int)gasneti_nodemap[i]);
    }
  }
  #endif
  
}

/* gasneti_nodemapInit(exchangefn, ids, sz, stride)
 *
 * Collectively called to construct the gasneti_nodemap[] such that
 *   For all i: gasneti_nodemap[i] is the lowest node number collocated w/ node i
 * GASNet nodes are considered collocated if they have the same node "ID" (see below).
 *
 * Calls gasneti_nodemapParse() after construction of the nodemap.
 *
 * There are 4 possible cases based on the first two arguments:
 *   Case 1: exchangefn == NULL  and  ids != NULL  (PREFERRED)
 *     The conduit has provided a vector of IDs with gasneti_nodes elements:
 *       'ids' is address of first ID
 *       'sz' is length of an ID in bytes
 *       'stride' is bytes between consecutive IDs (>=sz)
 *     The vector of IDs need not be "single valued" across calling nodes, so
 *     long as the resulting nodemap is the same.  This allows, for instance,
 *     the use of "local/relative" IDs as well as "global/absolute" ones.
 *   Case 2: exchangefn != NULL  and  ids == NULL
 *     The conduit has provided no IDs, but does have an exchangefn.
 *     This results in building a nodemap from a platform-specific node ID,
 *     such as gethostid() when available.  If the platform does not support
 *     any node ID, then the trivial [0,1,2,...] nodemap will be generated.
 *     The 'sz' and 'stride' arguments are unused.
 *   Case 3: exchangefn == NULL  and  ids == NULL
 *     The conduit has provided no bootstrapExchange function with
 *     which to communicate the platform-specific IDs (if any).
 *     This results in the trivial [0,1,2,...] nodemap.
 *     The 'sz' and 'stride' arguments are unused.
 *   Case 4: exchangefn != NULL  and  ids != NULL
 *     The conduit has provided an exchange function and a *local* ID:
 *       'ids' is address of the local ID
 *       'sz' is length of an ID in bytes
 *     The 'stride' argument is unused.
 */
extern void gasneti_nodemapInit(gasneti_bootstrapExchangefn_t exchangefn,
                                const void *ids, size_t sz, size_t stride) {
  gasneti_nodemap = gasneti_malloc(gasneti_nodes * sizeof(gex_Rank_t));

  if (ids) {
    /* Cases 1 or 4: conduit-provided vector of all IDs or a single local ID*/
    void *tmp = NULL;
    if (exchangefn) {
      // Perform exchange for 'Case 4'
      tmp = gasneti_malloc(gasneti_nodes * sz);
      (*exchangefn)((void*)ids, sz, tmp);
      ids = tmp;
      stride = sz;
    }
    gasneti_nodemap_helper(ids, sz, stride);
    gasneti_free(tmp);
  } else if (exchangefn) {
    /* Case 2: conduit-provided exchange fn, platform-default IDs */
    gasneti_nodemap_dflt(exchangefn);
  } else {
    /* Case 3: conduit provided neither exchangefn nor IDs */
    gasneti_nodemap_trivial();
  }
  /* Perform "common" work w.r.t the nodemap */
  gasneti_nodemapParse();
}

/* Presently just frees the space allocated for the full nodemap.
 */
extern void gasneti_nodemapFini(void) {
  gasneti_free(gasneti_nodemap);
#if GASNET_DEBUG
  /* To help catch any use-afer-Fini: */
  gasneti_nodemap = NULL;
#endif
}

/* ------------------------------------------------------------------------------------ */
/* Get a line up to '\n' or EOF using dynamicly grown buffer
 *  If buffer is too small (or NULL) then it is gasneti_realloc()ed.
 *  Buffer and length written to *buf_p and *n_p, even on error.
 *  Subsequent calls may reuse the buffer and length.
 *  Caller is responsible for eventual gasneti_free().
 *  Buffer is always terminated by '\0', even on error.
 *  If a '\n' was read, it is perserved.
 *  A '\n' is NOT added if EOF was reached first.
 *  Returns bytes read on success; -1 on EOF or other error.
 */
#ifdef gasneti_getline
/* Using glibc version */
#else
ssize_t gasneti_getline(char **buf_p, size_t *n_p, FILE *fp) {
    char   *buf = *buf_p;
    char   *p   = buf;
    size_t  n   = buf ? *n_p : 0;
    ssize_t len = 0;

    gasneti_assert_int((ssize_t)n ,>=, 0);

    do {
        size_t space = n - len;
        if_pf (space < 2) {
            n += MAX(2, n);
            buf = gasneti_realloc(buf, n);
            p = buf + len;
            space = n - len;
        }
        if (fgets(p, space, fp) == NULL) {
            *p = '\0';
            len = -1;
            break; /* error before full line read */
        }
        len += strlen(p);
        p = buf + len;
    } while (!feof(fp) && (p[-1] != '\n'));

    *buf_p = buf;
    *n_p = n;
    return len;
}
#endif /* gasneti_getline */

/* ------------------------------------------------------------------------------------ */
// Internal conduit interface to spawner

#if HAVE_SSH_SPAWNER
  extern gasneti_spawnerfn_t const *gasneti_bootstrapInit_ssh(int *argc, char ***argv, gex_Rank_t *nodes, gex_Rank_t *mynode);
#endif
#if HAVE_MPI_SPAWNER
  extern gasneti_spawnerfn_t const *gasneti_bootstrapInit_mpi(int *argc, char ***argv, gex_Rank_t *nodes, gex_Rank_t *mynode);
#endif
#if HAVE_PMI_SPAWNER
  extern gasneti_spawnerfn_t const *gasneti_bootstrapInit_pmi(int *argc, char ***argv, gex_Rank_t *nodes, gex_Rank_t *mynode);
#endif

extern gasneti_spawnerfn_t const *gasneti_spawnerInit(int *argc_p, char ***argv_p,
                                  const char *force_spawner,
                                  gex_Rank_t *nodes_p, gex_Rank_t *mynode_p) {
  gasneti_spawnerfn_t const *res = NULL;
  const char *not_set = "(not set)";
  const char *spawner;
  char *tmp = NULL;
  if (force_spawner) spawner = force_spawner;
  else { 
    // Purposely hide this variable from verbose output, since it's only for use as an internal hand-off
    // from gasnetrun scripts. End users should set GASNET_<conduit>_SPAWNER
    spawner = gasneti_getenv("GASNET_SPAWN_CONTROL");
    if (!spawner) spawner = not_set;
  }

  if (spawner != not_set) { // upper-case
    tmp = gasneti_strdup(spawner);
    for (char *p = tmp; *p; p++) *p = toupper(*p);
    spawner = tmp;
  }

#if HAVE_MPI_SPAWNER
  /* bug 3406: Try MPI-based spawn first, EVEN if the var is not set.
   * This is a requirement for spawning using bare mpirun
   */
  if (!res && (spawner == not_set || !strcmp(spawner, "MPI"))) {
    res = gasneti_bootstrapInit_mpi(argc_p, argv_p, nodes_p, mynode_p);
  }
#endif

#if HAVE_SSH_SPAWNER
  /* GASNET_SPAWN_CONTROL=ssh is set by gasnetrun for the ssh spawn master,
   * and by the ssh command line for other processes (ie all normal uses).
   * We no longer claim to support ssh-based launch without gasnetrun.
   * TODO: should we remove the "spawner == not_set" case?
   */
  if (!res && (spawner == not_set || !strcmp(spawner, "SSH"))) {
    res = gasneti_bootstrapInit_ssh(argc_p, argv_p, nodes_p, mynode_p);
  }
#endif

#if HAVE_PMI_SPAWNER
  /* GASNET_SPAWN_CONTROL=pmi is set by gasnetrun for the pmi spawn case.
   * We no longer claim to support direct launch with srun, yod, etc.
   * TODO: should we remove the "spawner == not_set" case?
   */
  if (!res && (spawner == not_set || !strcmp(spawner, "PMI"))) {
    res = gasneti_bootstrapInit_pmi(argc_p, argv_p, nodes_p, mynode_p);
  }
#endif

  if (!res) {
    gasneti_fatalerror("Requested spawner \"%s\" is unknown or not supported in this build", spawner);
  }

  gasneti_free(tmp);

  return res;
}

/* ------------------------------------------------------------------------------------ */
/* Buffer management
 */
#if GASNET_DEBUGMALLOC || GASNET_DEBUG
  static uint64_t gasneti_memalloc_envint(const char *name, const char *deflt) {
    /* Signaling NaN: any bit pattern between 0x7ff0000000000001 and 0x7ff7ffffffffffff  
                   or any bit pattern between 0xfff0000000000001 and 0xfff7ffffffffffff
       Quiet NaN: any bit pattern between 0x7ff8000000000000 and 0x7fffffffffffffff 
               or any bit pattern between 0xfff8000000000000 and 0xffffffffffffffff
    */
    uint64_t sNAN = ((uint64_t)0x7ff7ffffffffffffULL); 
    uint64_t qNAN = ((uint64_t)0x7fffffffffffffffULL);
    uint64_t val = 0;
    const char *envval = gasneti_getenv_withdefault(name, deflt);
    const char *p = envval;
    char tmp[255];
    int i = 0;
    for ( ; *p; p++) {
      if (!isspace(*p)) tmp[i++] = toupper(*p);
      if (i == 254) break;
    }
    tmp[i] = '\0';
    if (!strcmp(tmp, "NAN")) return sNAN;
    else if (!strcmp(tmp, "SNAN")) return sNAN;
    else if (!strcmp(tmp, "QNAN")) return qNAN;
    else val = gasneti_parse_int(tmp, 0);
    if (val <= 0xFF) {
      int i;
      uint64_t byte = val;
      for (i = 0; i < 7; i++) {
        val = (val << 8) | byte;
      }
    }
    return val;
  }
  static void gasneti_memalloc_valset(void *p, size_t len, uint64_t val) {
    gasneti_assert(! ((uintptr_t)p & 7));
    uint64_t *output = p;
    size_t blocks = len/8;
    size_t extra = len%8;
    size_t i;
    for (i = 0; i < blocks; i++) {
      *output = val; 
      output++;
    }
    if (extra) memcpy(output, &val, extra);
  }
  static const void *gasneti_memalloc_valcmp(const void *p, size_t len, uint64_t val) {
    gasneti_assert(! ((uintptr_t)p & 7));
    const uint64_t *input = p;
    size_t blocks = len/8;
    size_t extra = len%8;
    size_t i;
    for (i = 0; i < blocks; i++) {
      if (*input != val) {
        const uint8_t *in = (uint8_t *)input;
        const uint8_t *cmp = (uint8_t *)&val;
        for (i = 0; i < 8; i++, in++, cmp++)
          if (*in != *cmp) return in;
        gasneti_fatalerror("bizarre failure in gasneti_memalloc_valcmp");
      }
      input++;
    }
    if (extra) {
      const uint8_t *in = (uint8_t *)input;
      const uint8_t *cmp = (uint8_t *)&val;
      for (i = 0; i < extra; i++, in++, cmp++)
        if (*in != *cmp) return in;
    }
    return NULL;
  }
#endif
#if GASNET_DEBUGMALLOC
/* ------------------------------------------------------------------------------------ */
/* Debug memory management
   debug memory format:
  | prev | next | allocdesc (2*sizeof(void*)) | datasz | BEGINPOST | <user data> | ENDPOST |
                                             ptr returned by malloc ^
 */
  /* READ BEFORE MODIFYING gasneti_memalloc_desc_t:
   *
   * malloc() is specified as returning memory "suitably aligned for any kind of variable".
   * We don't know a priori what that alignment is, but we MUST preserve it in
   * this debugging malloc implementation if we are to meet that same requirement.
   * The current length of gasneti_memalloc_desc_t is:
   *     ILP32: 32 bytes (4+4+4+4+8+8)
   *   [I]LP64: 48 bytes (8+8+8+8+8+8)
   * This means that any alignment up to 16-bytes will be preserved.  That is ideal
   * since 16 is the strictest alignment requirement (long double on some platforms)
   * that we have encountered in practice.
   *
   * If you change this structure, you MUST add padding to maintain the length
   * at a multiple of 16-bytes AND please update the lengths above.
   *
   * NOTE: If the malloc() returns less than 16-byte alignment, then
   * it is not our responsibility to create it where it did not exists.
   * Any GASNet code needing larger alignment than 4- or 8-bytes should
   * probably be using gasneti_{malloc,free}_aligned() (or gasnett_*).
   */
  typedef struct gasneti_memalloc_desc {  
    struct gasneti_memalloc_desc * volatile prevdesc;
    struct gasneti_memalloc_desc * volatile nextdesc;
    const char *allocdesc_str; /* a file name, or file name:linenum */
    uintptr_t   allocdesc_num; /* a line number, or zero for none */
    uint64_t datasz;
    uint64_t beginpost;
  } gasneti_memalloc_desc_t;
  static uint64_t gasneti_memalloc_allocatedbytes = 0;   /* num bytes ever allocated */
  static uint64_t gasneti_memalloc_freedbytes = 0;       /* num bytes ever freed */
  static uint64_t gasneti_memalloc_allocatedobjects = 0; /* num objects ever allocated */
  static uint64_t gasneti_memalloc_freedobjects = 0;     /* num objects ever freed */
  static uint64_t gasneti_memalloc_ringobjects = 0;      /* num objects in the ring */
  static uint64_t gasneti_memalloc_ringbytes = 0;        /* num bytes in the ring */
  static size_t   gasneti_memalloc_maxobjectsize = 0;    /* max object size ever allocated */
  static uintptr_t gasneti_memalloc_maxobjectloc = 0;    /* max address ever allocated */
  static uint64_t gasneti_memalloc_maxlivebytes = 0;     /* max num bytes live at any given time */
  static uint64_t gasneti_memalloc_maxliveobjects = 0;   /* max num bytes live at any given time */
  static int gasneti_memalloc_extracheck = 0;
  static int gasneti_memalloc_init = -1;
  static uint64_t gasneti_memalloc_initval = 0;
  static int gasneti_memalloc_clobber = -1;
  static uint64_t gasneti_memalloc_clobberval = 0;
  static int gasneti_memalloc_leakall = -1;
  static int gasneti_memalloc_scanfreed = -1;
  static int gasneti_memalloc_envisinit = 0;
  static gasneti_mutex_t gasneti_memalloc_lock = GASNETI_MUTEX_INITIALIZER;
  static gasneti_memalloc_desc_t *gasneti_memalloc_pos = NULL;
  #define GASNETI_MEM_BEGINPOST   ((uint64_t)0xDEADBABEDEADBABEULL)
  #define GASNETI_MEM_LEAKMARK    ((uint64_t)0xBABEDEADCAFEBEEFULL)
  #define GASNETI_MEM_ENDPOST     ((uint64_t)0xCAFEDEEDCAFEDEEDULL)
  #define GASNETI_MEM_FREEMARK    ((uint64_t)0xBEEFEFADBEEFEFADULL)
  #define GASNETI_MEM_HEADERSZ    (sizeof(gasneti_memalloc_desc_t))
  #define GASNETI_MEM_TAILSZ      8     
  #define GASNETI_MEM_EXTRASZ     (GASNETI_MEM_HEADERSZ+GASNETI_MEM_TAILSZ)     
  #define GASNETI_MEM_MALLOCALIGN 4
  #define gasneti_looksaligned(p) (!(((uintptr_t)(p)) & (GASNETI_MEM_MALLOCALIGN-1)))

  GASNETI_INLINE(gasneti_memalloc_envinit)
  void gasneti_memalloc_envinit(void) {
    if (!gasneti_memalloc_envisinit) {
      gasneti_mutex_lock(&gasneti_memalloc_lock);
        if (!gasneti_memalloc_envisinit && gasneti_init_done) {
          gasneti_memalloc_envisinit = 1; /* set first, because getenv might call malloc when tracing */
          gasneti_memalloc_init =       gasneti_getenv_yesno_withdefault("GASNET_MALLOC_INIT",0);
          gasneti_memalloc_initval =    gasneti_memalloc_envint("GASNET_MALLOC_INITVAL","NAN");
          gasneti_memalloc_clobber =    gasneti_getenv_yesno_withdefault("GASNET_MALLOC_CLOBBER",0);
          gasneti_memalloc_clobberval = gasneti_memalloc_envint("GASNET_MALLOC_CLOBBERVAL","NAN");
          gasneti_memalloc_leakall =    gasneti_getenv_yesno_withdefault("GASNET_MALLOC_LEAKALL", 0);
          gasneti_memalloc_scanfreed =  gasneti_getenv_yesno_withdefault("GASNET_MALLOC_SCANFREED", 0);
          gasneti_memalloc_extracheck = gasneti_getenv_yesno_withdefault("GASNET_MALLOC_EXTRACHECK", 0);
          if (gasneti_memalloc_scanfreed && !gasneti_memalloc_clobber) {
            gasneti_memalloc_clobber = 1;
            if (gasneti_mynode == 0) { 
              fprintf(stderr, "WARNING: GASNET_MALLOC_SCANFREED requires GASNET_MALLOC_CLOBBER: enabling it.\n");
              fflush(stderr);
            }
          }
          if (gasneti_memalloc_scanfreed && !gasneti_memalloc_leakall) {
            gasneti_memalloc_leakall = 1;
            if (gasneti_mynode == 0) { 
              fprintf(stderr, "WARNING: GASNET_MALLOC_SCANFREED requires GASNET_MALLOC_LEAKALL: enabling it.\n");
              fflush(stderr);
            }
          }
        }
      gasneti_mutex_unlock(&gasneti_memalloc_lock);
    }
  }

  #define GASNETI_MAX_LOCSZ 280
  /* formats a curloc into buffer[GASNETI_MAX_LOCSZ] and returns buffer */
  static char *_gasneti_format_curloc(char *buffer, const char *curloc) {
      char retval[GASNETI_MAX_LOCSZ];

      if (curloc == NULL) {
        sprintf(retval, buffer, "<unknown>");
      } else if (!strcmp(curloc,"SRCPOS")) {
        const char *filename = "<unknown>"; 
        unsigned int linenum = 0;
        char temp[GASNETI_MAX_LOCSZ];
        GASNETI_TRACE_GETSOURCELINE(&filename, &linenum); /* noop if not avail */
        sprintf(temp,"%s:%i", filename, linenum);
        sprintf(retval, buffer, temp);
      } else {
        sprintf(retval, buffer, curloc);
      }
      strcpy(buffer, retval);
      return buffer;
  }

  extern void _gasneti_memcheck_one(const char *curloc) {
    if (gasneti_memalloc_extracheck) _gasneti_memcheck_all(curloc);
    else {
      gasneti_mutex_lock(&gasneti_memalloc_lock);
        if (gasneti_memalloc_pos) {
          _gasneti_memcheck(gasneti_memalloc_pos+1, curloc, 2);
          gasneti_memalloc_pos = gasneti_memalloc_pos->nextdesc;
        } else gasneti_assert_always(gasneti_memalloc_ringobjects == 0 && gasneti_memalloc_ringbytes == 0);
      gasneti_mutex_unlock(&gasneti_memalloc_lock);
    }
  }
  extern void _gasneti_memcheck_all(const char *curloc) {
    gasneti_mutex_lock(&gasneti_memalloc_lock);
      if (gasneti_memalloc_pos) {
        gasneti_memalloc_desc_t *begin = gasneti_memalloc_pos;
        uint64_t cnt;
        uint64_t sumsz = 0;
        for (cnt=0; cnt < gasneti_memalloc_ringobjects; cnt++) {
          sumsz += _gasneti_memcheck(gasneti_memalloc_pos+1, curloc, 2);
          gasneti_memalloc_pos = gasneti_memalloc_pos->nextdesc;
          if (gasneti_memalloc_pos == begin) break;
        } 
        if (cnt+1 != gasneti_memalloc_ringobjects || gasneti_memalloc_pos != begin || 
            sumsz != gasneti_memalloc_ringbytes) {
          gasneti_fatalerror("Debug malloc memcheck_all (called at %s) detected an error "
                             "in the memory ring linkage, most likely as a result of memory corruption.", 
                             curloc);
        }
      } else gasneti_assert_always(gasneti_memalloc_ringobjects == 0 && gasneti_memalloc_ringbytes == 0);
    gasneti_mutex_unlock(&gasneti_memalloc_lock);
  }

  /* assert the integrity of given memory block and return size of the user object 
      checktype == 0: check a live object
      checktype == 1: check an object which is about to be freed
      checktype == 2: check an object which resides in the ring (and may be dead)
  */
  extern size_t _gasneti_memcheck(void *ptr, const char *curloc, int checktype) {
    const char *corruptstr = NULL;
    char tmpstr[255];
    size_t nbytes = 0;
    const char *allocdesc_str = NULL;
    uintptr_t allocdesc_num = 0;
    uint64_t beginpost = 0;
    uint64_t endpost = 0;
    int doscan = 0;
    gasneti_assert_always_uint((unsigned int)checktype ,<=, 2);
    if (gasneti_looksaligned(ptr)) {
      gasneti_memalloc_desc_t *desc = ((gasneti_memalloc_desc_t *)ptr) - 1;
      beginpost = (desc->beginpost == GASNETI_MEM_LEAKMARK)
                     ? GASNETI_MEM_BEGINPOST : desc->beginpost;
      nbytes = (size_t)desc->datasz;
      if (nbytes == 0 || nbytes > gasneti_memalloc_maxobjectsize || 
          ((uintptr_t)ptr)+nbytes > gasneti_memalloc_maxobjectloc ||
          !desc->prevdesc || !desc->nextdesc || 
          !gasneti_looksaligned(desc->prevdesc) || 
          !gasneti_looksaligned(desc->nextdesc)) {
            nbytes = 0; /* bad metadata, don't trust any of it */
      } else {
        allocdesc_str = desc->allocdesc_str;
        allocdesc_num = desc->allocdesc_num;
        memcpy(&endpost,((char*)ptr)+nbytes,GASNETI_MEM_TAILSZ);
      }
    }
    if (beginpost == GASNETI_MEM_FREEMARK) {
      switch (checktype) {
        case 0: /* should be a live object */
          corruptstr = "Debug malloc memcheck() called on freed memory (may indicate local heap corruption)";
          break;
        case 1: /* about to be freed - should still be a live object */
          corruptstr = "Debug free detected a duplicate free() or local heap corruption";
          break;
        case 2:
          if (gasneti_memalloc_scanfreed <= 0) /* freed objects should not be in ring */
            corruptstr = "Debug malloc found a freed object in the memory ring, indicating local heap corruption";
          else doscan = 1;
          break;
      }
    }  
    if (beginpost != GASNETI_MEM_FREEMARK && 
        (beginpost != GASNETI_MEM_BEGINPOST || endpost != GASNETI_MEM_ENDPOST)) {
      const char *diagnosis = "a bad pointer or local heap corruption";
      #if !GASNET_SEGMENT_EVERYTHING
        // TODO-EX: multi-segment equivalent?
        gasneti_EP_t i_ep = gasneti_import_ep(gasneti_THUNK_EP);
        if (gasneti_attach_done && gasneti_in_local_segment(i_ep,ptr,1))
          diagnosis = "a bad pointer, referencing the shared segment (outside malloc heap)";
        else 
      #endif
      if (nbytes && beginpost == GASNETI_MEM_BEGINPOST && endpost != GASNETI_MEM_ENDPOST)
        diagnosis = "local heap corruption (probable buffer overflow)";
      else if (nbytes && beginpost != GASNETI_MEM_BEGINPOST && endpost == GASNETI_MEM_ENDPOST)
        diagnosis = "local heap corruption (probable buffer underflow)";
      if (checktype == 1) {
        sprintf(tmpstr, "Debug free detected %s", diagnosis);
      } else {
        sprintf(tmpstr, "Debug malloc memcheck() detected %s", diagnosis);
      }
      corruptstr = tmpstr;
    }
    if (corruptstr == NULL && doscan) {
      const void *badloc = gasneti_memalloc_valcmp(ptr, nbytes, gasneti_memalloc_clobberval);
      if (badloc) {
        sprintf(tmpstr, "Debug malloc memcheck() detected a write to freed memory at object offset: %i bytes",
                        (int)((uintptr_t)badloc - (uintptr_t)ptr));
        corruptstr = tmpstr;
      }
    }

    if (corruptstr != NULL) {
      char nbytesstr[80];
      char allocstr[GASNETI_MAX_LOCSZ];
      const char *allocdesc;
      char curlocstr[GASNETI_MAX_LOCSZ];

      if (allocdesc_str != NULL && memchr(allocdesc_str,'\0',255) == 0) { /* allocptr may be bad */
        allocdesc = NULL; 
      } else {
        if (allocdesc_num) {
          sprintf(allocstr,"\n   allocated at: %s:%i",allocdesc_str,(int)allocdesc_num);
        } else {
          sprintf(allocstr,"\n   allocated at: %s",allocdesc_str);
        }
        allocdesc = allocstr;
      }
      if (allocdesc == NULL) nbytesstr[0] = '\0';
      else sprintf(nbytesstr," nbytes=%i",(int)nbytes);

      if (checktype == 1) strcpy(curlocstr,"\n   freed at: %s");
      else                strcpy(curlocstr,"\n   detected at: %s");

      gasneti_fatalerror("%s\n   ptr="GASNETI_LADDRFMT"%s%s%s",
           corruptstr,
           GASNETI_LADDRSTR(ptr), nbytesstr,
           (allocdesc!=NULL?allocdesc:""),
           _gasneti_format_curloc(curlocstr,curloc));
    }
    return nbytes;
  }

  /* get access to system malloc/free */
  #undef malloc
  #undef free
  static void *_gasneti_malloc_inner(int allowfail, size_t nbytes, const char *curloc) {
    void *ret = NULL;
    gasneti_memalloc_envinit();
    _gasneti_memcheck_one(curloc);
    GASNETI_STAT_EVENT_VAL(I, GASNET_MALLOC, nbytes);
    if_pf (nbytes == 0) {
      return NULL;
    }
    ret = malloc(nbytes+GASNETI_MEM_EXTRASZ);
    gasneti_assert_always_uint((((uintptr_t)ret) & 0x3) ,==, 0); /* should have at least 4-byte alignment */
    if_pf (ret == NULL) {
      char curlocstr[GASNETI_MAX_LOCSZ];
      strcpy(curlocstr, "\n   at: %s");
      if (allowfail) {
        GASNETI_TRACE_PRINTF(I,("Warning: returning NULL for a failed gasneti_malloc(%"PRIuPTR")%s",
                                (uintptr_t)nbytes, _gasneti_format_curloc(curlocstr,curloc)));
        return NULL;
      }
      gasneti_fatalerror("Debug malloc(%"PRIuPTR") failed (%"PRIu64" bytes in use, in %"PRIu64" objects)%s", 
                     (uintptr_t)nbytes, 
                     (gasneti_memalloc_allocatedbytes - gasneti_memalloc_freedbytes),
                     (gasneti_memalloc_allocatedobjects - gasneti_memalloc_freedobjects),
                     _gasneti_format_curloc(curlocstr,curloc));
    } else {
      uint64_t gasneti_endpost_ref = GASNETI_MEM_ENDPOST;
      gasneti_memalloc_desc_t *desc = ret;
      if (!strcmp(curloc,"SRCPOS")) {
        const char *filename = "<unknown>"; 
        unsigned int linenum = 0;
        GASNETI_TRACE_GETSOURCELINE(&filename, &linenum); /* noop if not avail */
        desc->allocdesc_str = filename;
        desc->allocdesc_num = linenum;
      } else {
        desc->allocdesc_str = curloc;
        desc->allocdesc_num = 0;
      }
      desc->datasz = (uint64_t)nbytes;
      desc->beginpost = GASNETI_MEM_BEGINPOST;
      memcpy(((char*)ret)+nbytes+GASNETI_MEM_HEADERSZ, &gasneti_endpost_ref, GASNETI_MEM_TAILSZ);

      gasneti_mutex_lock(&gasneti_memalloc_lock);
        gasneti_memalloc_allocatedbytes += nbytes;
        gasneti_memalloc_allocatedobjects++;
        gasneti_memalloc_ringobjects++;
        gasneti_memalloc_ringbytes += nbytes;
        if (nbytes > gasneti_memalloc_maxobjectsize) gasneti_memalloc_maxobjectsize = nbytes;
        if (((uintptr_t)ret)+nbytes+GASNETI_MEM_HEADERSZ > gasneti_memalloc_maxobjectloc) 
          gasneti_memalloc_maxobjectloc = ((uintptr_t)ret)+nbytes+GASNETI_MEM_HEADERSZ;
        gasneti_memalloc_maxlivebytes = 
          MAX(gasneti_memalloc_maxlivebytes, gasneti_memalloc_allocatedbytes-gasneti_memalloc_freedbytes);
        gasneti_memalloc_maxliveobjects = 
          MAX(gasneti_memalloc_maxliveobjects, gasneti_memalloc_allocatedobjects-gasneti_memalloc_freedobjects);
        if (gasneti_memalloc_pos == NULL) { /* first object */
          gasneti_memalloc_pos = desc;
          desc->prevdesc = desc;
          desc->nextdesc = desc;
        } else { /* link into ring */
          desc->prevdesc = gasneti_memalloc_pos->prevdesc;
          desc->nextdesc = gasneti_memalloc_pos;
          gasneti_memalloc_pos->prevdesc->nextdesc = desc;
          gasneti_memalloc_pos->prevdesc = desc;
        }
      gasneti_mutex_unlock(&gasneti_memalloc_lock);

      ret = desc+1;
      if (gasneti_memalloc_init > 0) gasneti_memalloc_valset(ret, nbytes, gasneti_memalloc_initval);
    }
    _gasneti_memcheck(ret,curloc,0);
    return ret;
  }
  extern void *_gasneti_malloc(size_t nbytes, const char *curloc) {
    return _gasneti_malloc_inner(0, nbytes, curloc);
  }
  extern void *_gasneti_malloc_allowfail(size_t nbytes, const char *curloc) {
    return _gasneti_malloc_inner(1, nbytes, curloc);
  }

  extern void _gasneti_free(void *ptr, const char *curloc) {
    size_t nbytes;
    gasneti_memalloc_desc_t *desc;
    gasneti_memalloc_envinit();
    _gasneti_memcheck_one(curloc);
    if_pf (ptr == NULL) return;
    nbytes = _gasneti_memcheck(ptr, curloc, 1);
    GASNETI_STAT_EVENT_VAL(I, GASNET_FREE, nbytes);
    desc = ((gasneti_memalloc_desc_t *)ptr) - 1;
    if (gasneti_memalloc_clobber > 0) gasneti_memalloc_valset(desc+1, nbytes, gasneti_memalloc_clobberval);

    gasneti_mutex_lock(&gasneti_memalloc_lock);
      desc->beginpost = GASNETI_MEM_FREEMARK;
      gasneti_memalloc_freedbytes += nbytes;
      gasneti_memalloc_freedobjects++;
      if (gasneti_memalloc_scanfreed <= 0) {
        gasneti_memalloc_ringobjects--;
        gasneti_memalloc_ringbytes -= nbytes;
        if (desc->nextdesc == desc) { /* last item in list */
          gasneti_assert_always(desc->prevdesc == desc && gasneti_memalloc_ringobjects == 0);
          gasneti_memalloc_pos = NULL;
        } else {
          if (gasneti_memalloc_pos == desc) gasneti_memalloc_pos = desc->nextdesc;
          desc->prevdesc->nextdesc = desc->nextdesc;
          desc->nextdesc->prevdesc = desc->prevdesc;
        }
      }
    gasneti_mutex_unlock(&gasneti_memalloc_lock);

    if (gasneti_memalloc_leakall <= 0) free(desc);
  }

  extern void *_gasneti_calloc(size_t N, size_t S, const char *curloc) {
    void *ret;
    size_t nbytes = N*S;
    if_pf (nbytes == 0) return NULL;
    ret = _gasneti_malloc(nbytes, curloc);
    memset(ret,0,nbytes);
    _gasneti_memcheck(ret,curloc,0);
    return ret;
  }
  extern void *_gasneti_realloc(void *ptr, size_t sz, const char *curloc) {
    void *ret = _gasneti_malloc(sz, curloc);
    if_pt (ptr != NULL) {
      size_t nbytes = _gasneti_memcheck(ptr, curloc, 0);
      memcpy(ret, ptr, MIN(nbytes, sz));
      _gasneti_free(ptr, curloc);
    }
    if (sz) {
      _gasneti_memcheck(ret,curloc,0);
    }
    return ret;
  }
  extern void _gasneti_leak(void *ptr, const char *curloc) {
    gasneti_memalloc_desc_t *desc;
    if_pf (ptr == NULL) return;
    _gasneti_memcheck(ptr, curloc, 0);
    desc = ((gasneti_memalloc_desc_t *)ptr) - 1;
    gasneti_mutex_lock(&gasneti_memalloc_lock);
      desc->beginpost = GASNETI_MEM_LEAKMARK;
    gasneti_mutex_unlock(&gasneti_memalloc_lock);
  }

  extern int gasneti_getheapstats(gasneti_heapstats_t *pstat) {
    pstat->allocated_bytes = gasneti_memalloc_allocatedbytes;
    pstat->freed_bytes = gasneti_memalloc_freedbytes;
    pstat->live_bytes = gasneti_memalloc_allocatedbytes - gasneti_memalloc_freedbytes;
    pstat->live_bytes_max = gasneti_memalloc_maxlivebytes;
    pstat->allocated_objects = gasneti_memalloc_allocatedobjects;
    pstat->freed_objects = gasneti_memalloc_freedobjects;
    pstat->live_objects = gasneti_memalloc_allocatedobjects - gasneti_memalloc_freedobjects;
    pstat->live_objects_max = gasneti_memalloc_maxliveobjects;
    pstat->overhead_bytes = gasneti_memalloc_ringbytes - pstat->live_bytes + 
                            gasneti_memalloc_ringobjects*GASNETI_MEM_EXTRASZ;
    return 0;
  }

  extern void gasneti_malloc_dump_liveobjects(FILE *fp) {
    gasneti_mutex_lock(&gasneti_memalloc_lock);
      if (gasneti_memalloc_pos) {
        gasneti_memalloc_desc_t *pos = gasneti_memalloc_pos;
        uint64_t cnt;
        for (cnt=0; cnt < gasneti_memalloc_ringobjects; cnt++) {
          uint64_t datasz = pos->datasz;
          const char * allocptr = NULL;
          char allocdesc[GASNETI_MAX_LOCSZ];
          if (!pos->allocdesc_str) {
            allocptr = NULL;
          } else if (pos->allocdesc_num) {
            sprintf(allocdesc,"%s:%i",pos->allocdesc_str,(int)pos->allocdesc_num);
            allocptr = allocdesc;
          } else {
            allocptr = pos->allocdesc_str;
          }
          fprintf(fp, "   %10lu %c   %s\n",
                  (unsigned long)datasz,
                  (pos->beginpost == GASNETI_MEM_LEAKMARK ? '*' : ' '),
                  (allocptr?allocptr:"<unknown>"));
          pos = pos->nextdesc;
        } 
      } 
    gasneti_mutex_unlock(&gasneti_memalloc_lock);
  }

#endif
/* extern versions of gasnet malloc fns for use in public headers */
extern void *_gasneti_extern_malloc(size_t sz GASNETI_CURLOCFARG) {
  return _gasneti_malloc(sz GASNETI_CURLOCPARG);
}
extern void *_gasneti_extern_realloc(void *ptr, size_t sz GASNETI_CURLOCFARG) {
  return _gasneti_realloc(ptr, sz GASNETI_CURLOCPARG);
}
extern void *_gasneti_extern_calloc(size_t N, size_t S GASNETI_CURLOCFARG) {
  return _gasneti_calloc(N,S GASNETI_CURLOCPARG);
}
extern void _gasneti_extern_free(void *ptr GASNETI_CURLOCFARG) {
  _gasneti_free(ptr GASNETI_CURLOCPARG);
}
extern void _gasneti_extern_leak(void *ptr GASNETI_CURLOCFARG) {
  _gasneti_leak(ptr GASNETI_CURLOCPARG);
}
extern char *_gasneti_extern_strdup(const char *s GASNETI_CURLOCFARG) {
  return _gasneti_strdup(s GASNETI_CURLOCPARG);
}
extern char *_gasneti_extern_strndup(const char *s, size_t n GASNETI_CURLOCFARG) {
  return _gasneti_strndup(s,n GASNETI_CURLOCPARG);
}

#if GASNET_DEBUGMALLOC
  extern void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc);
  extern void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc);
  extern void *(*gasnett_debug_realloc_fn)(void *ptr, size_t sz, const char *curloc);
  extern void (*gasnett_debug_free_fn)(void *ptr, const char *curloc);
  extern char *(*gasnett_debug_strdup_fn)(const char *ptr, const char *curloc);
  extern char *(*gasnett_debug_strndup_fn)(const char *ptr, size_t n, const char *curloc);
  void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc) =
         &_gasneti_extern_malloc;
  void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc) =
         &_gasneti_extern_calloc;
  void *(*gasnett_debug_realloc_fn)(void *ptr, size_t sz, const char *curloc) =
        &_gasneti_extern_realloc;
  void (*gasnett_debug_free_fn)(void *ptr, const char *curloc) =
         &_gasneti_extern_free;
  char *(*gasnett_debug_strdup_fn)(const char *s, const char *curloc) =
         &_gasneti_extern_strdup;
  char *(*gasnett_debug_strndup_fn)(const char *s, size_t sz, const char *curloc) =
         &_gasneti_extern_strndup;
  /* these only exist with debug malloc */
  extern void (*gasnett_debug_memcheck_fn)(void *ptr, const char *curloc);
  extern void (*gasnett_debug_memcheck_one_fn)(const char *curloc);
  extern void (*gasnett_debug_memcheck_all_fn)(const char *curloc);
  extern void _gasneti_extern_memcheck(void *ptr, const char *curloc) {
    _gasneti_memcheck(ptr, curloc, 0);
  }
  void (*gasnett_debug_memcheck_fn)(void *ptr, const char *curloc) = 
        &_gasneti_extern_memcheck;
  void (*gasnett_debug_memcheck_one_fn)(const char *curloc) = 
        &_gasneti_memcheck_one;
  void (*gasnett_debug_memcheck_all_fn)(const char *curloc) =
        &_gasneti_memcheck_all;
#endif

/* don't put anything here - malloc stuff must come last */
