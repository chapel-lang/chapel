/*   $Source: /var/local/cvs/gasnet/other/amxtests/apputils.h,v $
 *     $Date: 2009/04/01 23:44:23 $
 * $Revision: 1.21 $
 * Description: AMX Application utilities
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef _APPUTILS_H
#define _APPUTILS_H

#include <errno.h>
#ifdef WIN32
  #include <windows.h>  
  #define sleep(x) Sleep(1000*x)
#endif

#if defined(AMUDP)
  #include <amudp.h>
  #include <amudp_spmd.h>
#elif defined(AMMPI)
  #include <ammpi.h>
  #include <ammpi_spmd.h>
#else
  #error You should #define AMUDP/AMMPI (or #include amudp.h/ammpi.h) before including apputils.h
#endif

#if !defined(DEBUG) && !defined(NDEBUG)
  #ifdef AMX_DEBUG
    #define DEBUG 1
  #else
    #define NDEBUG 1
  #endif
#endif

#ifndef VERBOSE
  #if AMX_DEBUG_VERBOSE || GASNET_DEBUG_VERBOSE
    #define VERBOSE 1
  #else
    #define VERBOSE 0
  #endif
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#if PLATFORM_COMPILER_MICROSOFT
  #pragma warning(disable: 4127)
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/* in a multi-threaded program, this would also include a lock */
#define AM_Safe(fncall) do {                \
  if ((fncall) != AM_OK) {                  \
    printf("Error calling: %s\n", #fncall); \
    AMX_SPMDExit(-1);                       \
    AMX_FatalErr("AMX_SPMDExit failed");    \
    }                                       \
  } while(0)

#define AM_PollBlock(eb) do {                       \
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY));  \
        AM_Safe(AM_WaitSema(eb));                   \
        AM_Safe(AM_Poll(eb));                       \
      } while (0)

#if defined(AMUDP)
  #define LEADING_ARGS      2
  #define LEADING_ARGS_STR  " numprocs spawnfn"
#else
  #define LEADING_ARGS      0
  #define LEADING_ARGS_STR  ""
#endif

#define TEST_STARTUP(argc, argv, networkpid, eb, ep, minargs, maxargs, usagestr) do { \
    AMX_VerboseErrors = 1;                                                            \
    if (AMX_SPMDIsWorker(argv)) { /* slave */                                         \
      AM_Safe(AMX_SPMDStartup(&(argc), &(argv), 0, &(networkpid), &(eb), &(ep)));     \
      if ((argc) < (minargs)+1 || (argc) > (maxargs)+1 ) {                                \
        fprintf(stderr, "Usage: %s %s\n", (argv)[0], (usagestr));                     \
        fflush(stderr);                                                               \
        AMX_SPMDExit(-1);                                                             \
      }                                                                               \
    } else { /* implicit master */                                                    \
      if ((argc) < (minargs)+LEADING_ARGS+1 || (argc) > (maxargs)+LEADING_ARGS+1 ) {  \
        fprintf(stderr, "Usage: %s%s %s\n", (argv)[0], LEADING_ARGS_STR, (usagestr)); \
        fflush(stderr);                                                               \
        exit(-1);                                                                     \
      }                                                                               \
      AM_Safe(AMX_SPMDStartup(&argc, &argv, 0, &networkpid, &eb, &ep));               \
    }                                                                                 \
  } while (0)

/* app can define this before including to move our handlers 
   NO - that doesn't work unless apputils.c is recompiled */
#ifndef APPUTIL_HANDLER_BASE
  #define APPUTIL_HANDLER_BASE  225
#endif

/* call first to setup handlers for all app utils */
void setupUtilHandlers(ep_t activeep, eb_t activeeb);

void printGlobalStats(void);


#ifdef UETH
  #define getCurrentTimeMicrosec() ueth_getustime()
#else
  extern int64_t getCurrentTimeMicrosec(void);
#endif
extern void outputTimerStats(void);

#define TEST_32BIT_ONLY() do {                                         \
    if (sizeof(void*) != 4) {                                          \
      if (AMX_SPMDMyProc() == 0) {                                     \
        printf("Test SKIPPED -- not implemented on 64-bit systems\n"); \
        fflush(stdout);                                                \
      }                                                                \
      AM_Safe(AMX_SPMDBarrier());                                      \
      AM_Safe(AMX_SPMDExit(0));                                        \
    }                                                                  \
  } while(0)

#ifndef APPUTILS_OMIT_READWRITE
uint32_t getWord(int proc, void *addr);
void putWord(int proc, void *addr, uint32_t val);

void readWord(void *destaddr, int proc, void *addr);
void readSync(void);

void writeWord(int proc, void *addr, uint32_t val);
void writeSync(void);
#else
  #define getWord(a,b)     (AMX_FatalErr("APPUTILS_OMIT_READWRITE violation"),0)
  #define putWord(a,b,c)   AMX_FatalErr("APPUTILS_OMIT_READWRITE violation")
  #define readWord(a,b,c)  AMX_FatalErr("APPUTILS_OMIT_READWRITE violation")
  #define readSync()       AMX_FatalErr("APPUTILS_OMIT_READWRITE violation")
  #define writeWord(a,b,c) AMX_FatalErr("APPUTILS_OMIT_READWRITE violation")
  #define writeSync()      AMX_FatalErr("APPUTILS_OMIT_READWRITE violation")
#endif

#ifdef __cplusplus
  }
#endif
#endif
