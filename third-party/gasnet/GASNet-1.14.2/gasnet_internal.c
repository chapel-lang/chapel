/*   $Source: /var/local/cvs/gasnet/gasnet_internal.c,v $
 *     $Date: 2010/05/15 05:18:30 $
 * $Revision: 1.206.2.3 $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
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
    #define _gasneti_atomic_lock_initializer	GASNET_HSL_INITIALIZER
    #define _gasneti_atomic_lock_init(x)	gasnet_hsl_init(x)
    #define _gasneti_atomic_lock_malloc		gasneti_malloc
    GASNETI_ATOMIC_LOCK_TBL_DEFNS(gasneti_hsl_atomic_, gasnet_hsl_)
    #undef _gasneti_atomic_lock_initializer
    #undef _gasneti_atomic_lock_init
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

#define GASNET_VERSION_STR  _STRINGIFY(GASNET_VERSION)
GASNETI_IDENT(gasneti_IdentString_APIVersion, "$GASNetAPIVersion: " GASNET_VERSION_STR " $");

#define GASNETI_THREAD_MODEL_STR _STRINGIFY(GASNETI_THREAD_MODEL)
GASNETI_IDENT(gasneti_IdentString_ThreadModel, "$GASNetThreadModel: GASNET_" GASNETI_THREAD_MODEL_STR " $");

#define GASNETI_SEGMENT_CONFIG_STR _STRINGIFY(GASNETI_SEGMENT_CONFIG)
GASNETI_IDENT(gasneti_IdentString_SegConfig, "$GASNetSegment: GASNET_SEGMENT_" GASNETI_SEGMENT_CONFIG_STR " $");

#ifdef GASNETI_BUG1389_WORKAROUND
  GASNETI_IDENT(gasneti_IdentString_ConservativeLocalCopy, "$GASNetConservativeLocalCopy: 1 $");
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

int gasneti_wait_mode = GASNET_WAIT_SPIN;

int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME)) = 1;
int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME)) = 1;

extern int gasneti_internal_idiotcheck(gasnet_handlerentry_t *table, int numentries,
                                       uintptr_t segsize, uintptr_t minheapoffset) {
  gasneti_fatalerror("GASNet client code must NOT #include <gasnet_internal.h>\n"
                     "gasnet_internal.h is not installed, and modifies the behavior "
                     "of various internal operations, such as segment safety bounds-checking.");
  return GASNET_ERR_NOT_INIT;
}

/* Default global definitions of GASNet-wide internal variables
   if conduits override one of these, they must
   still provide variable or macro definitions for these tokens */
#ifdef _GASNET_MYNODE_DEFAULT
  gasnet_node_t gasneti_mynode = (gasnet_node_t)-1;
#endif
#ifdef _GASNET_NODES_DEFAULT
  gasnet_node_t gasneti_nodes = 0;
#endif

#if defined(_GASNET_GETMAXSEGMENTSIZE_DEFAULT) && !GASNET_SEGMENT_EVERYTHING
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

#ifdef _GASNETI_SEGINFO_DEFAULT
  gasnet_seginfo_t *gasneti_seginfo = NULL;
  gasnet_seginfo_t *gasneti_seginfo_client = NULL;
  void **gasneti_seginfo_ub = NULL; /* cached result of gasneti_seginfo[i].addr + gasneti_seginfo[i].size */
  void **gasneti_seginfo_client_ub = NULL;
#endif

/* ------------------------------------------------------------------------------------ */
/* conduit-independent sanity checks */
extern void gasneti_check_config_preinit(void) {
  gasneti_assert_always(sizeof(int8_t) == 1);
  gasneti_assert_always(sizeof(uint8_t) == 1);
  gasneti_assert_always(sizeof(gasnete_anytype8_t) == 1);
  #ifndef INTTYPES_16BIT_MISSING
    gasneti_assert_always(sizeof(int16_t) == 2);
    gasneti_assert_always(sizeof(uint16_t) == 2);
    gasneti_assert_always(sizeof(gasnete_anytype16_t) == 2);
  #endif
  gasneti_assert_always(sizeof(int32_t) == 4);
  gasneti_assert_always(sizeof(uint32_t) == 4);
  gasneti_assert_always(sizeof(gasnete_anytype32_t) == 4);
  gasneti_assert_always(sizeof(int64_t) == 8);
  gasneti_assert_always(sizeof(uint64_t) == 8);
  gasneti_assert_always(sizeof(gasnete_anytype64_t) == 8);

  gasneti_assert_always(sizeof(uintptr_t) >= sizeof(void *));

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
  gasneti_assert_always(GASNET_PAGESIZE > 0);
  gasneti_assert_always(GASNETI_POWEROFTWO(GASNET_PAGESIZE));

  gasneti_assert_always(SIZEOF_GASNET_REGISTER_VALUE_T == sizeof(gasnet_register_value_t));
  gasneti_assert_always(SIZEOF_GASNET_REGISTER_VALUE_T >= sizeof(int));
  gasneti_assert_always(SIZEOF_GASNET_REGISTER_VALUE_T >= sizeof(void *));

  #if    PLATFORM_ARCH_32 && !PLATFORM_ARCH_64
    gasneti_assert_always(sizeof(void*) == 4);
  #elif !PLATFORM_ARCH_32 &&  PLATFORM_ARCH_64
    gasneti_assert_always(sizeof(void*) == 8);
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
extern void gasneti_check_config_postattach(void) {
  gasneti_check_config_preinit();

  /*  verify sanity of the core interface */
  gasneti_assert_always(gasnet_AMMaxArgs() >= 2*MAX(sizeof(int),sizeof(void*)));      
  gasneti_assert_always(gasnet_AMMaxMedium() >= 512);
  gasneti_assert_always(gasnet_AMMaxLongRequest() >= 512);
  gasneti_assert_always(gasnet_AMMaxLongReply() >= 512);  

  gasneti_assert_always(gasnet_nodes() >= 1);
  gasneti_assert_always(gasnet_mynode() < gasnet_nodes());
  { static int firstcall = 1;
    if (firstcall) { /* miscellaneous conduit-independent initializations */
      firstcall = 0;

      if (gasneti_getenv_yesno_withdefault("GASNET_DISABLE_MUNMAP",0)) {
        #if HAVE_PTMALLOC                                        
          mallopt(M_TRIM_THRESHOLD, -1);
          mallopt(M_MMAP_MAX, 0);
          GASNETI_TRACE_PRINTF(I,("Setting mallopt M_TRIM_THRESHOLD=-1 and M_MMAP_MAX=0"));
        #else
          GASNETI_TRACE_PRINTF(I,("WARNING: GASNET_DISABLE_MUNMAP set on an unsupported platform"));
          if (gasneti_verboseenv()) 
            fprintf(stderr, "WARNING: GASNET_DISABLE_MUNMAP set on an unsupported platform\n");
        #endif
      }
      #if GASNET_NDEBUG
        gasneti_check_portable_conduit();
      #endif
    }
  }
  gasneti_memcheck_all();
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
extern void gasneti_defaultAMHandler(gasnet_token_t token) {
  gasnet_node_t srcnode = (gasnet_node_t)-1;
  gasnet_AMGetMsgSource(token, &srcnode);
  gasneti_fatalerror("GASNet node %i/%i received an AM message from node %i for a handler index "
                     "with no associated AM handler function registered", 
                     (int)gasnet_mynode(), (int)gasnet_nodes(), (int)srcnode);
}
/* ------------------------------------------------------------------------------------ */
#define DEF_SIGNAL(name) { name, #name, NULL }
static struct {
  int signum;
  const char *signame;
  gasneti_sighandlerfn_t oldhandler;
} gasneti_signals[] = {
  /* abort signals */
  DEF_SIGNAL(SIGABRT),
  DEF_SIGNAL(SIGILL),
  DEF_SIGNAL(SIGSEGV),
  DEF_SIGNAL(SIGBUS),
  DEF_SIGNAL(SIGFPE),
  /* termination signals */
  DEF_SIGNAL(SIGQUIT),
  DEF_SIGNAL(SIGINT),
  DEF_SIGNAL(SIGTERM),
  DEF_SIGNAL(SIGHUP),
  DEF_SIGNAL(SIGPIPE)
};

#ifndef GASNETC_FATALSIGNAL_CALLBACK
#define GASNETC_FATALSIGNAL_CALLBACK(sig)
#endif

void gasneti_defaultSignalHandler(int sig) {
  gasneti_sighandlerfn_t oldhandler = NULL;
  const char *signame = NULL;
  int i;
  for (i = 0; i < sizeof(gasneti_signals)/sizeof(gasneti_signals[0]); i++) {
    if (gasneti_signals[i].signum == sig) {
      oldhandler = gasneti_signals[i].oldhandler;
      signame = gasneti_signals[i].signame;
    }
  }
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
    case SIGFPE:
      GASNETC_FATALSIGNAL_CALLBACK(sig); /* give conduit first crack at it */
      fprintf(stderr,"*** Caught a fatal signal: %s(%i) on node %i/%i\n",
        signame, sig, (int)gasnet_mynode(), (int)gasnet_nodes()); 
      fflush(stderr);

      gasnett_freezeForDebuggerErr(); /* allow freeze */

      gasneti_print_backtrace_ifenabled(STDERR_FILENO); /* try to print backtrace */

      signal(sig, SIG_DFL); /* restore default core-dumping handler and re-raise */
      #if 1
        raise(sig);
      #elif 0
        kill(getpid(),sig);
      #else
        oldhandler(sig);
      #endif
      break;
    default: 
      /* translate signal to SIGQUIT */
      { static int sigquit_raised = 0;
        if (sigquit_raised) {
          /* sigquit was already raised - we cannot safely reraise it, so just die */
          _exit(1);
        } else sigquit_raised = 1;
      }
      fprintf(stderr,"*** Caught a signal: %s(%i) on node %i/%i\n",
        signame, sig, (int)gasnet_mynode(), (int)gasnet_nodes()); 
      fflush(stderr);
      #if 1
        raise(SIGQUIT);
      #elif 0
        kill(getpid(),SIGQUIT);
      #else
        oldhandler(SIGQUIT);
      #endif
      break;
  }
}

void gasneti_registerSignalHandlers(gasneti_sighandlerfn_t handler) {
  int i;
  for (i = 0; i < sizeof(gasneti_signals)/sizeof(gasneti_signals[0]); i++) {
    gasneti_signals[i].oldhandler = 
      gasneti_reghandler(gasneti_signals[i].signum, handler);
  }
  gasneti_ondemand_init(); /* allow user override of signal handlers */
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
  gasneti_assert((p+1) - buf == totalEnvSize);

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
extern void gasneti_setupGlobalEnvironment(gasnet_node_t numnodes, gasnet_node_t mynode,
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
      gasneti_assert(gasneti_checksum(gasneti_globalEnv,envsize) == rootdesc.checksum);
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

static const char *gasneti_decode_envval(const char *val) {
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
    if (gasneti_init_done && gasneti_mynode != (gasnet_node_t)-1) {
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
/* expose environment decode to external packages in case we ever need it */
extern const char * (*gasnett_decode_envval_fn)(const char *);
const char * (*gasnett_decode_envval_fn)(const char *) = &gasneti_decode_envval;

/* expression that defines whether the given process should report to the console
   on env queries - needs to work before gasnet_init
   1 = yes, 0 = no, -1 = not yet / don't know
 */
#define GASNETI_ENV_OUTPUT_NODE()  (gasneti_mynode == 0)
extern int _gasneti_verboseenv_fn(void) {
  static int verboseenv = -1;
  if (verboseenv == -1) {
    if (gasneti_init_done && gasneti_mynode != (gasnet_node_t)-1) {
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
int (*gasneti_verboseenv_fn)(void) = &_gasneti_verboseenv_fn;

extern const char * _gasneti_backtraceid_fn(void) {
  static char myid[255];
  sprintf(myid, "[%i] ", (int)gasneti_mynode);
  return myid;
}
const char *(*gasneti_backtraceid_fn)(void) = &_gasneti_backtraceid_fn;

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
  gasneti_backtrace_init((*argv)[0]);
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
    gasneti_assert(MIN(dflt_max, dflt_min + dflt_factor * gasneti_nodes) >= lower_bound);
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
    GASNETI_TRACE_PRINTF(I, ("gasneti_pthread_create(%p, %p, %p, %p, %p)", create_fn, thread, attr, start_routine, arg));
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
  if ( /* is a portable network conduit */
      (!strcmp("mpi",mycore) && !strcmp("reference",myext)) || 
      (!strcmp("udp",mycore) && !strcmp("reference",myext))
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
        if (!strcmp(name,"mpi")) continue;
        if (!strcmp(name,"udp")) continue;
        if (!strcmp(name,"smp")) continue;
      #if !GASNET_SEQ
        /* Ignore conduits that lack thread safety */
        if (!strcmp(name,"shmem")) continue;
      #endif
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
        #if PLATFORM_OS_AIX
          { "/dev/nampd0",         S_IFCHR, "IBM LAPI", 1 }, /* could also run lslpp -l | grep lapi */
        #endif
        { "/dev/infiniband/uverbs0", S_IFCHR, "InfiniBand IBV", 2 },  /* OFED 1.0 */
        { "/dev/vipkl",          S_IFCHR, "InfiniBand VAPI", 2 },  /* Mellanox drivers */
        { "/dev/ib_dsc",         S_IFCHR, "InfiniBand VAPI", 2 },  /* wotan - could also run system_profiler */
        { "/dev/gm3",            S_IFCHR, "Myrinet", 3 }, /* could also look in /proc/devices and /proc/pci */
        { "/dev/elan3/control0", S_IFCHR, "Quadrics QsNetI", 4 },
        { "/dev/elan4/control0", S_IFCHR, "Quadrics QsNetII", 4 },
        { "/proc/qsnet/version", S_IFREG, "Quadrics QsNet", 4 },
        #if !GASNET_SEGMENT_EVERYTHING
          { "/dev/ukbridge",         S_IFCHR, "Cray XT", 5 },
          { "/proc/portals/meminfo", S_IFREG, "Cray Portals", 5 }
        #endif
      };
      int i, lim = sizeof(known_devs)/sizeof(known_devs[0]);
      for (i = 0; i < lim; i++) {
        struct stat stat_buf;
        if (!stat(known_devs[i].filename,&stat_buf) && 
            (!known_devs[i].filemode || (known_devs[i].filemode & stat_buf.st_mode))) {
            int hwid = known_devs[i].hwid;
            if (strlen(natives)) strcat(natives,", ");
            strcat(natives,known_devs[i].desc);
            while (i < lim && hwid == known_devs[i].hwid) i++; /* don't report a network twice */
        }
      }
      #if PLATFORM_ARCH_CRAYX1
        if (strlen(natives)) strcat(natives,", ");
        strcat(natives,"Cray X1");
      #elif PLATFORM_OS_CATAMOUNT || PLATFORM_OS_CNL
        if (strlen(natives)) strcat(natives,", ");
        strcat(natives,"Cray XT");
      #elif PLATFORM_OS_BGP
        if (strlen(natives)) strcat(natives,", ");
        strcat(natives,"IBM BG/P");
      #endif
      if (natives[0]) {
        sprintf(reason, "WARNING: This system appears to contain recognized network hardware: %s\n"
                        "WARNING: which is supported by a GASNet native conduit, although\n"
                        "WARNING: it was not detected at configure time (missing drivers?)",
                        natives);
      }
    }
    if (reason[0] && !gasneti_getenv_yesno_withdefault("GASNET_QUIET",0) && gasnet_mynode() == 0) {
      fprintf(stderr,"WARNING: Using GASNet's %s-conduit, which exists for portability convenience.\n"
                     "%s\n"
                     "WARNING: You should *really* use the high-performance native GASNet conduit\n"
                     "WARNING: if communication performance is at all important in this program run.\n",
              mycore, reason);
      fflush(stderr);
    }
  }
}

/* ------------------------------------------------------------------------------------ */
/* Nodemap handling
 */

gasnet_node_t *gasneti_nodemap = NULL;
gasnet_node_t *gasneti_nodemap_local = NULL;
gasnet_node_t gasneti_nodemap_local_count = 0;
gasnet_node_t gasneti_nodemap_local_rank = (gasnet_node_t)-1;
gasnet_node_t gasneti_nodemap_global_count = 0;
gasnet_node_t gasneti_nodemap_global_rank = (gasnet_node_t)-1;

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
  gasnet_node_t i, prev, base;
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
  gasnet_node_t key1 = *(const gasnet_node_t *)a;
  gasnet_node_t key2 = *(const gasnet_node_t *)b;
  const char *val1 = _gasneti_nodemap_sort_aux.ids + key1 * _gasneti_nodemap_sort_aux.stride;
  const char *val2 = _gasneti_nodemap_sort_aux.ids + key2 * _gasneti_nodemap_sort_aux.stride;
  int retval = memcmp(val1, val2, _gasneti_nodemap_sort_aux.sz);
  if (!retval) { /* keep sort stable */
    gasneti_assert(key1 != key2);
    retval = (key1 < key2) ? -1 : 1;
  }
  return retval;
}
static void gasneti_nodemap_helper_qsort(const char *ids, size_t sz, size_t stride) {
  gasnet_node_t *work    = gasneti_malloc(gasneti_nodes * sizeof(gasnet_node_t));
  const char *prev_id;
  int i, prev; /* If these are gasnet_node_t then bug 2634 can crash XLC */

  _gasneti_nodemap_sort_aux.ids    = ids;
  _gasneti_nodemap_sort_aux.sz     = sz;
  _gasneti_nodemap_sort_aux.stride = stride;
  for (i = 0; i < gasneti_nodes; ++i) work[i] = i;
  qsort(work, gasneti_nodes, sizeof(gasnet_node_t), &_gasneti_nodemap_sort_fn);

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
    #define GASNETC_DEFAULT_NODEMAP_EXACT 0
  #endif
  gasneti_assert(ids);
  gasneti_assert(sz > 0);
  gasneti_assert(stride >= sz);

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
  gasnet_node_t i;
  for (i = 0; i < gasneti_nodes; ++i) gasneti_nodemap[i] = i;
}

/* Platform-depended default nodemap constructor
 * Used when no conduit-specific IDs are provided.
 */
static void gasneti_nodemap_dflt(gasneti_bootstrapExchangefn_t exchangefn) {
#if PLATFORM_OS_BGP && GASNETI_HAVE_BGP_INLINES
    /* Build gasneti_nodemap from <X,Y,Z> coords of all ranks. */
    gasnet_node_t i;
    _BGP_SprgShMem sprg4;

    GASNETI_BGP_SPR(sprg4.shmem, _BGP_SPRGRO_SHMem); /* SPRG4 30:31 = (processes per node) - 1 */

    if ((0 == gasneti_getenv_int_withdefault("BG_SHAREDMEMPOOLSIZE",0,0)) ||
        !sprg4.ShmNumProcs || (gasneti_nodes == 1)) {
      /* Just build the trivial map if BG_SHAREDMEMPOOLSIZE is unset or zero,
         or are in SMP mode, or we have just a single node */
      gasneti_nodemap_trivial();
    } else {
      uint32_t *allids = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));

      /* Kernel call to get torus coords for all ranks */
      { int rc;
        GASNETI_BGP_SYSCALL2(rc, RANKS2COORDS, (uintptr_t)allids, (uint32_t)gasneti_nodes);
        gasneti_assert(!rc);
      }

      /* Compare only the upper 3 bytes, discarding the T coordinate */
      gasneti_nodemap_helper(allids, 3, sizeof(uint32_t));

      gasneti_free(allids);
    }
#elif PLATFORM_OS_BGP || PLATFORM_OS_BLRTS  || PLATFORM_OS_CATAMOUNT || !HAVE_GETHOSTID
    /* Nodes are either (at least effectively) single process,
     * or we don't have a usable gethostid().  So, build a trivial nodemap. */
    gasneti_nodemap_trivial();
#else
    /* Construct nodemap from gethostid and conduit-provided exchangefn 
     * gethostid() from Single Unix Specification (IEEE Std 1003.1-2001)
     * spec says return type is long, but that the result is 32 bits.
     * AIX and others have int.
     */
    uint32_t *allids = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));
    uint32_t myid = (uint32_t)gethostid();

    /* Fall back to hashing the hostname if the hostid is obviously invalid */
    if (!myid || !(~myid)) {
      const char *myname = gasneti_gethostname();
      uint64_t csum = gasneti_checksum(myname, strlen(myname));
      myid = GASNETI_HIWORD(csum) ^ GASNETI_LOWORD(csum);
    }
  
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
 * Currently computes some local statistics:
 *   gasneti_nodemap_local_count = number of GASNet nodes collocated w/ gasneti_mynode
 *   gasneti_nodemap_local_rank  = rank of gasneti_mynode among gasneti_nodemap_local_count
 *   gasneti_nodemap_local[]     = array (length gasneti_nodemap_local_count) of local nodes
 *   gasneti_nodemap_global_count = number of unique values in the nodemap
 *   gasneti_nodemap_global_rank  = rank of gasneti_mynode among gasneti_nodemap_global_count
 *
 */
extern void gasneti_nodemapParse(void) {
  gasnet_node_t i,j,first;

  gasneti_assert(gasneti_nodemap);
  gasneti_assert(gasneti_nodemap[0] == 0);
  gasneti_assert(gasneti_nodemap[gasneti_mynode] <= gasneti_mynode);

  first = gasneti_nodemap[gasneti_mynode];

  /* Compute local stats */
  {
    gasneti_assert(gasneti_nodemap_local_count == 0);
    for (i = first; i < gasneti_nodes; ++i) {
      if (i == gasneti_mynode) gasneti_nodemap_local_rank = gasneti_nodemap_local_count;
      if (gasneti_nodemap[i] == first) ++gasneti_nodemap_local_count;
    }
    gasneti_assert(gasneti_nodemap_local_count != 0);
    gasneti_assert(gasneti_nodemap_local_rank < gasneti_nodemap_local_count);
  }
  gasneti_nodemap_local = gasneti_malloc(gasneti_nodemap_local_count*sizeof(gasnet_node_t));

  /* construct array of local nodes and determine global count & rank */
  gasneti_nodemap_global_count = 0;
  for (i = j = 0; i < gasneti_nodes; ++i) {
    if (gasneti_nodemap[i] == first){
      gasneti_nodemap_local[j++] = i;
    }
    if (gasneti_nodemap[i] == i) {
      if (i == first) {
        gasneti_nodemap_global_rank = gasneti_nodemap_global_count;
      }
      ++gasneti_nodemap_global_count;
    }
  }

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
 *     This case is not supported.
 */
extern void gasneti_nodemapInit(gasneti_bootstrapExchangefn_t exchangefn,
                                const void *ids, size_t sz, size_t stride) {
  gasneti_nodemap = gasneti_malloc(gasneti_nodes * sizeof(gasnet_node_t));

  if (ids) {
    /* Case 1: conduit-provided vector of IDs */
    gasneti_assert(!exchangefn); /* Prohibit 'Case 4' */
    gasneti_nodemap_helper(ids, sz, stride);
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

/* Presently just frees the space allocated for the nodemaps.
 */
extern void gasneti_nodemapFini(void) {
  gasneti_free(gasneti_nodemap);
  gasneti_free(gasneti_nodemap_local);
#if GASNET_DEBUG
  /* To help catch any use-afer-Fini: */
  gasneti_nodemap = NULL;
  gasneti_nodemap_local = NULL;
#endif
}

/* ------------------------------------------------------------------------------------ */
/* Debug memory management
   debug memory format:
  | prev | next | allocdesc (2*sizeof(void*)) | datasz | BEGINPOST | <user data> | ENDPOST |
                                             ptr returned by malloc ^
 */
#if GASNET_DEBUG
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
  #define GASNETI_MEM_BEGINPOST   ((uint64_t)0xDEADBABEDEADBABEllu)
  #define GASNETI_MEM_ENDPOST     ((uint64_t)0xCAFEDEEDCAFEDEEDllu)
  #define GASNETI_MEM_FREEMARK    ((uint64_t)0xBEEFEFADBEEFEFADllu)
  #define GASNETI_MEM_HEADERSZ    (sizeof(gasneti_memalloc_desc_t))
  #define GASNETI_MEM_TAILSZ      8     
  #define GASNETI_MEM_EXTRASZ     (GASNETI_MEM_HEADERSZ+GASNETI_MEM_TAILSZ)     
  #define GASNETI_MEM_MALLOCALIGN 4
  #define gasneti_looksaligned(p) (!(((uintptr_t)(p)) & (GASNETI_MEM_MALLOCALIGN-1)))

  static uint64_t gasneti_memalloc_envint(const char *name, const char *deflt) {
    /* Signaling NaN: any bit pattern between 0x7ff0000000000001 and 0x7ff7ffffffffffff  
                   or any bit pattern between 0xfff0000000000001 and 0xfff7ffffffffffff
       Quiet NaN: any bit pattern between 0x7ff8000000000000 and 0x7fffffffffffffff 
               or any bit pattern between 0xfff8000000000000 and 0xffffffffffffffff
    */
    uint64_t sNAN = 0x7ff7ffffffffffffllu; 
    uint64_t qNAN = 0x7fffffffffffffffllu;
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
      if_pt (gasneti_attach_done) gasnet_hold_interrupts();
      gasneti_mutex_lock(&gasneti_memalloc_lock);
        if (gasneti_memalloc_pos) {
          _gasneti_memcheck(gasneti_memalloc_pos+1, curloc, 2);
          gasneti_memalloc_pos = gasneti_memalloc_pos->nextdesc;
        } else gasneti_assert(gasneti_memalloc_ringobjects == 0 && gasneti_memalloc_ringbytes == 0);
      gasneti_mutex_unlock(&gasneti_memalloc_lock);
      if_pt (gasneti_attach_done) gasnet_resume_interrupts();
    }
  }
  extern void _gasneti_memcheck_all(const char *curloc) {
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
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
      } else gasneti_assert(gasneti_memalloc_ringobjects == 0 && gasneti_memalloc_ringbytes == 0);
    gasneti_mutex_unlock(&gasneti_memalloc_lock);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
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
    gasneti_assert(checktype >= 0 && checktype <= 2);
    if (gasneti_looksaligned(ptr)) {
      gasneti_memalloc_desc_t *desc = ((gasneti_memalloc_desc_t *)ptr) - 1;
      beginpost = desc->beginpost;
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
        if (gasneti_attach_done && gasneti_in_fullsegment(gasneti_mynode,ptr,1))
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
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    if_pf (nbytes == 0) {
      if_pt (gasneti_attach_done) gasnet_resume_interrupts();
      return NULL;
    }
    ret = malloc(nbytes+GASNETI_MEM_EXTRASZ);
    gasneti_assert((((uintptr_t)ret) & 0x3) == 0); /* should have at least 4-byte alignment */
    if_pf (ret == NULL) {
      char curlocstr[GASNETI_MAX_LOCSZ];
      if (allowfail) {
        if_pt (gasneti_attach_done) gasnet_resume_interrupts();
        GASNETI_TRACE_PRINTF(I,("Warning: returning NULL for a failed gasneti_malloc(%lu): %s",
                                (unsigned long)nbytes, _gasneti_format_curloc(curlocstr,curloc)));
        return NULL;
      }
      gasneti_fatalerror("Debug malloc(%lu) failed (%lu bytes in use, in %lu objects): %s", 
                     (unsigned long)nbytes, 
                     (unsigned long)(gasneti_memalloc_allocatedbytes - gasneti_memalloc_freedbytes),
                     (unsigned long)(gasneti_memalloc_allocatedobjects - gasneti_memalloc_freedobjects),
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
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
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
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
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
          gasneti_assert(desc->prevdesc == desc && gasneti_memalloc_ringobjects == 0);
          gasneti_memalloc_pos = NULL;
        } else {
          if (gasneti_memalloc_pos == desc) gasneti_memalloc_pos = desc->nextdesc;
          desc->prevdesc->nextdesc = desc->nextdesc;
          desc->nextdesc->prevdesc = desc->prevdesc;
        }
      }
    gasneti_mutex_unlock(&gasneti_memalloc_lock);

    if (gasneti_memalloc_leakall <= 0) free(desc);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
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
    _gasneti_memcheck(ret,curloc,0);
    return ret;
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
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
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
          fprintf(fp, "   %10lu     %s\n", (unsigned long)datasz, (allocptr?allocptr:"<unknown>"));
          pos = pos->nextdesc;
        } 
      } 
    gasneti_mutex_unlock(&gasneti_memalloc_lock);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
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
extern char *_gasneti_extern_strdup(const char *s GASNETI_CURLOCFARG) {
  return _gasneti_strdup(s GASNETI_CURLOCPARG);
}
extern char *_gasneti_extern_strndup(const char *s, size_t n GASNETI_CURLOCFARG) {
  return _gasneti_strndup(s,n GASNETI_CURLOCPARG);
}

#if GASNET_DEBUG
  extern void *(*gasnett_debug_malloc_fn)(size_t sz GASNETI_CURLOCFARG);
  extern void *(*gasnett_debug_calloc_fn)(size_t N, size_t S GASNETI_CURLOCFARG);
  extern void (*gasnett_debug_free_fn)(void *ptr GASNETI_CURLOCFARG);
  void *(*gasnett_debug_malloc_fn)(size_t sz GASNETI_CURLOCFARG) =
         &_gasneti_extern_malloc;
  void *(*gasnett_debug_calloc_fn)(size_t N, size_t S GASNETI_CURLOCFARG) =
         &_gasneti_extern_calloc;
  void (*gasnett_debug_free_fn)(void *ptr GASNETI_CURLOCFARG) =
         &_gasneti_extern_free;
#endif

/* don't put anything here - malloc stuff must come last */
