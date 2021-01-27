/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/amx_internal.c $
 * Description: AMX internal support code
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _AMX_INTERNAL_H
#error header inclusion error
#endif

/* definitions for internal declarations */
int amx_Initialized = 0;

#if AMX_DEBUG_VERBOSE
  int AMX_VerboseErrors = 1;
#else
  int AMX_VerboseErrors = 0;
#endif
int AMX_SilentMode = 0; 
const char *AMX_ProcessLabel = NULL;

volatile int amx_frozen = 1; /* non-static to prevent a SunC problem (bug1626) */
/*  all this to make sure we get a full stack frame for debugger */
AMX_NEVER_INLINE(_freezeForDebugger,
static void _freezeForDebugger(int depth)) {
  if (!depth) _freezeForDebugger(1);
  else {
    volatile int i = 0;
    while (amx_frozen) {
      i++;
      AMX_sched_yield();
    }
  }
}
extern void AMX_freezeForDebugger(void) {
  char name[255];
  gethostname(name, sizeof(name));
  AMX_Info("worker frozen for debugger: host=%s  pid=%i : Attach and set amx_frozen=0", name, (int)getpid());
  _freezeForDebugger(0);
}


/* ------------------------------------------------------------------------------------ */
/* error handling */
AMX_FORMAT_PRINTF(AMX_Msg,2,0,
static int AMX_Msg(const char *prefix, const char *msg, va_list argptr)) {
  static char _expandedmsg[255]; // use static storage when possible for robustness in panic-mode
  static char plabel[80];

  if (AMX_ProcessLabel && !*plabel) snprintf(plabel, sizeof(plabel), "(%s)", AMX_ProcessLabel);
  char *expandedmsg = _expandedmsg;
  size_t sz = strlen(prefix) + strlen(plabel) + strlen(msg) + 8;
  if (sz > sizeof(_expandedmsg)) expandedmsg = (char *)AMX_malloc(sz);
  int chk = snprintf(expandedmsg, sz, "%s%s: %s\n", prefix, plabel, msg);
  AMX_assert(chk < (int)sz); // truncation should not occur
  int retval = vfprintf(stderr, expandedmsg, argptr);
  fflush(stderr);
  if (expandedmsg != _expandedmsg) AMX_free(expandedmsg);
  
  return retval; 
}

extern int AMX_Info(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMX_Msg(AMX_ENV_PREFIX_STR, msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMX_Warn(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMX_Msg("*** " AMX_ENV_PREFIX_STR " WARNING", msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMX_Err(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMX_Msg("*** " AMX_ENV_PREFIX_STR " ERROR", msg, argptr);
  va_end(argptr);
  return retval;
}

extern void AMX_FatalErr(const char *msg, ...) {
  va_list argptr;
  va_start(argptr, msg); // pass in last argument
    AMX_Msg("*** FATAL ERROR", msg, argptr);
  va_end(argptr);
  abort();
}
/* ------------------------------------------------------------------------------------ */
static void AMX_defaultAMHandler(void *token) {
  int srcnode = -1;
  AMX_GetSourceId(token, &srcnode);
  AMX_FatalErr(AMX_LIB_STR" received an AM message from node %i for a handler index "
                     "with no associated AM handler function registered", srcnode);
}
amx_handler_fn_t amx_unused_handler = (amx_handler_fn_t)&AMX_defaultAMHandler;
/*------------------------------------------------------------------------------------
 * System initialization/termination
 *------------------------------------------------------------------------------------ */
extern int AMX_Init() {
  int firstcall = (amx_Initialized == 0);
  if (firstcall) {
    /* check system attributes */
    AMX_assert(sizeof(int8_t) == 1);
    AMX_assert(sizeof(uint8_t) == 1);
    #ifndef INTTYPES_16BIT_MISSING
      AMX_assert(sizeof(int16_t) == 2);
      AMX_assert(sizeof(uint16_t) == 2);
    #endif
    AMX_assert(sizeof(int32_t) == 4);
    AMX_assert(sizeof(uint32_t) == 4);
    AMX_assert(sizeof(int64_t) == 8);
    AMX_assert(sizeof(uint64_t) == 8);

    AMX_assert(sizeof(uintptr_t) >= sizeof(void *));
  }
  amx_Initialized++;
  return firstcall;
}
/* ------------------------------------------------------------------------------------ */
extern int AMX_Terminate() {
  AMX_CHECKINIT();

  int lastcall = (amx_Initialized == 1);
  amx_Initialized--;

  return lastcall;
}
/* ------------------------------------------------------------------------------------ */

