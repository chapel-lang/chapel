/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_timer.h $
 * Description: GASNet Timer library (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNETEX_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_TIMER_H
#define _GASNET_TIMER_H

/* general includes (to avoid repetition below) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ------------------------------------------------------------------------------------ */
/* High-performance system timer library 
   see README-tools for usage information
*/

// Note to implementers:
//
// When using assembly (inline or special) to implement gasneti_ticks_now() care
// must be taken to ensure (via preprocessor) that such code is reachable only
// by compilers which will accept it.  In particular, unknown compilers should
// result in "#define GASNETI_USING_SLOW_TIMERS 1" to use a function call.

#if defined(GASNETC_CONDUIT_SPECIFIC_TIMERS)
  #if !defined(gasneti_ticks_to_ns) || !defined(gasneti_ticks_now)
    /* conduit-specific timers must be implemented using a macro */
    #error Incomplete conduit-specific timer impl.
  #endif
#elif defined(GASNETI_FORCE_GETTIMEOFDAY) || defined(GASNETI_FORCE_POSIX_REALTIME)
/* bug3508: forced portable timer implementation overrides compilation of native timers */
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_OS_SOLARIS
#if 1
  /* workaround bizarre failures on gcc 3.2.1 - seems they sometimes use a
     union to implement longlong_t and hence hrtime_t, and the test to
     determine this is (__STDC__ - 0 == 0) which is totally bogus */
  typedef uint64_t gasneti_tick_t;
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    union {
      hrtime_t _in;
      gasneti_tick_t _out;
    } _t;
    _t._in = gethrtime();
    return _t._out;
  }
  #define gasneti_ticks_to_ns(st)  (st)
#else
  typedef hrtime_t gasneti_tick_t;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    gasneti_static_assert(sizeof(gasneti_tick_t) == 8);
    return *(uint64_t*)&_st;
  }
  #define gasneti_ticks_now()      (gethrtime())
  #define GASNETI_TICK_MAX        ((gasneti_tick_t)(((uint64_t)-1)>>1))
#endif
/* ------------------------------------------------------------------------------------ */
#elif (PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL || PLATFORM_OS_OPENBSD || \
       GASNETI_HAVE_SYSCTL_MACHDEP_TSC_FREQ) && \
     (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC || PLATFORM_ARCH_IA64)
  typedef uint64_t gasneti_tick_t;
 #if GASNETI_HAVE_GCC_ASM
    GASNETI_INLINE(gasneti_ticks_now)
    uint64_t gasneti_ticks_now (void) {
      uint64_t _ret;
    #if (PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC)
      uint32_t _lo, _hi;
      __asm__ __volatile__("rdtsc"
                           : "=a" (_lo), "=d" (_hi)
                           /* no inputs */); 
      _ret = ((uint64_t)_lo) | (((uint64_t)_hi)<<32);
    #elif PLATFORM_ARCH_X86
      __asm__ __volatile__("rdtsc"
                           : "=A" (_ret)
                           /* no inputs */); 
    #elif PLATFORM_ARCH_IA64
      __asm__ __volatile__("mov %0=ar.itc" 
                           : "=r"(_ret) 
                           /* no inputs */);
    #else
      #error Unreachable
    #endif
      return _ret;
    }
 #elif PLATFORM_COMPILER_SUN && GASNETI_ASM_AVAILABLE
   /* The current compiler has asm, but lacks full GNU-style asm() support.
    *
    * Defining GASNETI_TICKS_NOW_BODY at library build time will use the
    * given asm() as the body of a function gasneti_slow_ticks_now() and
    * replace calls to gasneti_ticks_now() with calls to the "special"
    * gasneti_slow_ticks_now().
    *
    * Defining GASNETI_TICKS_NOW_BODY when compiling client code will only
    * perform the gasneti_ticks_now()->gasneti_slow_ticks_now() redirection
    * and the actual asm() here is ignored.  Since gasneti_slow_ticks_now()
    * is always present in the library, this makes no assumption about
    * what level of inline asm() support was present at library built time.
    */
   #if PLATFORM_ARCH_X86
     #define GASNETI_TICKS_NOW_BODY GASNETI_ASM_SPECIAL("rdtsc");
   #elif PLATFORM_ARCH_X86_64
     #define GASNETI_TICKS_NOW_BODY                   \
		GASNETI_ASM_SPECIAL(                  \
			     "\txorq %rax, %rax	\n" \
			     "\trdtsc		\n" \
			     "\tshlq $32, %rdx	\n" \
			     "\torq %rdx, %rax" );
   #elif PLATFORM_ARCH_IA64
     /* For completeness. */
     #define GASNETI_TICKS_NOW_BODY \
		GASNETI_ASM_SPECIAL( "mov.m r8=ar.itc;" );
   #endif
 #elif PLATFORM_COMPILER_CRAY
    GASNETI_INLINE(gasneti_ticks_now)
    uint64_t gasneti_ticks_now (void) {
      return (uint64_t) _rtc();
    }
 #elif PLATFORM_COMPILER_INTEL && PLATFORM_ARCH_IA64
    #include <ia64intrin.h>
    GASNETI_INLINE(gasneti_ticks_now)
    uint64_t gasneti_ticks_now (void) {
      return (uint64_t)__getReg(_IA64_REG_AR_ITC);
    }
 #else
    #define GASNETI_USING_SLOW_TIMERS 1
 #endif
  extern double gasneti_calibrate_tsc(void);
  #define GASNETI_CALIBRATE_TSC 1
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    if_pf (gasneti_timer_firstTime) {
      gasneti_timer_Tick = gasneti_calibrate_tsc(); /* Too much to inline */
      gasneti_assert(gasneti_timer_Tick != 0.0);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(_st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_POWERPC && PLATFORM_OS_LINUX
  /* Use the 64-bit "timebase" register on both 32- and 64-bit PowerPC CPUs */
  #include <sys/types.h>
  #include <dirent.h>
  typedef uint64_t gasneti_tick_t;
 #if GASNETI_HAVE_GCC_ASM
  #if PLATFORM_COMPILER_CLANG /* or something to force? */
    /* Clang's integrated assembler (correctly) warns that mftb* are deprecated */
    #define GASNETI_MFTB(_reg)  "mfspr %" #_reg ",268"
    #define GASNETI_MFTBL(_reg) "mfspr %" #_reg ",268"
    #define GASNETI_MFTBU(_reg) "mfspr %" #_reg ",269"
  #else
    #define GASNETI_MFTB(_reg)  "mftb %" #_reg
    #define GASNETI_MFTBL(_reg) "mftbl %" #_reg
    #define GASNETI_MFTBU(_reg) "mftbu %" #_reg
  #endif

  GASNETI_INLINE(gasneti_ticks_now)
  uint64_t gasneti_ticks_now(void) {
    uint64_t _ret;
    #if PLATFORM_ARCH_64
      __asm__ __volatile__(GASNETI_MFTB(0)
                           : "=r" (_ret)
                           : /* no inputs */); 
    #else
      /* Note we must read hi twice to protect against wrap of lo */
      uint32_t _o_hi, _hi, _lo;
      __asm__ __volatile__("0: \n\t"
		           GASNETI_MFTBU(0) "\n\t"
			   GASNETI_MFTBL(1) "\n\t"
			   GASNETI_MFTBU(2) "\n\t"
			   "cmpw  %0, %2\n\t"
			   "bne- 0b\n\t"
                           : "=r" (_o_hi), "=r" (_lo), "=r" (_hi)
                           : /* no inputs */); 
      _ret = ((uint64_t)_hi << 32) | _lo;
    #endif
    return _ret;
  } 

  #undef GASNETI_MFTB
  #undef GASNETI_MFTBL
  #undef GASNETI_MFTBU
 #else
   #define GASNETI_USING_SLOW_TIMERS 1
 #endif
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    if_pf (gasneti_timer_firstTime) {
      uint32_t _freq;
      DIR *_dp = opendir("/proc/device-tree/cpus");
      struct dirent *_de = NULL;
      FILE *_fp = NULL;
      double _MHz = 0.0;
      char _fname[128];
      if (!_dp) gasneti_fatalerror("*** ERROR: Failure in opendir('/proc/device-tree/cpus'): %s",strerror(errno));
      do {
        _de = readdir(_dp);
	if (_de && 
           ( _de->d_name == strstr(_de->d_name, "PowerPC,") || /* PowerPC */
             _de->d_name == strstr(_de->d_name, "cpu@0") /* IBM cell */
           )) {
	  break;
	}
      } while (_de);
      if (!_de) gasneti_fatalerror("*** ERROR: Failure to find a PowerPC CPU in /proc/device-tree/cpus");
      snprintf(_fname, sizeof(_fname), "/proc/device-tree/cpus/%.*s/timebase-frequency", 24, _de->d_name);
      closedir(_dp);
      _fp = fopen(_fname, "r");
      if (!_fp) gasneti_fatalerror("*** ERROR: Failure in fopen('%s','r'): %s\n",_fname,strerror(errno));
      if (fread((void *)(&_freq), sizeof(uint32_t), 1, _fp) != 1) 
        gasneti_fatalerror("*** ERROR: Failure to read timebase frequency from '%s': %s", _fname, strerror(errno));
    #if PLATFORM_ARCH_LITTLE_ENDIAN /* value is always big-endian */
      _freq = ((_freq & 0x000000ff) << 24) |
             ((_freq & 0x0000ff00) <<  8) |
             ((_freq & 0x00ff0000) >>  8) |
             ((_freq & 0xff000000) >> 24);
    #endif
      fclose(_fp);
      if (_freq == 0) { /* Playstation3 */
        char _input[255];
        _fp = fopen("/proc/cpuinfo", "r");
        if (!_fp) gasneti_fatalerror("*** ERROR: Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));
        while (!feof(_fp) && fgets(_input, 255, _fp)) {
          if (strstr(_input,"timebase")) {
            char *_p = strchr(_input,':');
            if (_p) { _freq = atoi(_p+1); break; }
          }
        }
        fclose(_fp);
      }
      // ensure it looks reasonable (1MHz to 2Ghz)
      gasneti_assert_uint(_freq ,>,    1000000);
      gasneti_assert_uint(_freq ,<, 2000000000); 
      gasneti_timer_Tick = 1.0e9 / _freq;
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(_st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_OS_CYGWIN
  #include <windows.h>
  /* note: QueryPerformanceCounter is a Win32 API call
     On modern processors with constant-rate TSC this expands to rdtsc.
     Older systems have a QueryPerformanceFrequency() which use a system call to query the
     ACPI counter to ensure reliablity across CPU cycle speedstepping, etc.
     rdtsc has lower overhead, but only works on Pentium or later, and on certain older CPUs
     produces wildly incorrect results if the CPU decides to change clock rate 
     mid-run (and there's no reliable way to get the correct cycle multiplier 
     short of timing a known-length delay and hoping for the best)
     See https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
  */
  typedef uint64_t gasneti_tick_t;
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    LARGE_INTEGER _val;
    gasneti_assert_nzeroret(QueryPerformanceCounter(&_val));
    gasneti_assert(_val.QuadPart > 0);
    return (gasneti_tick_t)_val.QuadPart;
  }
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    if_pf (gasneti_timer_firstTime) {
      LARGE_INTEGER _temp;
      gasneti_assert_nzeroret(QueryPerformanceFrequency(&_temp));
      gasneti_timer_Tick = 1.0E9 / ((double)_temp.QuadPart);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(_st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_OS_DARWIN
  /* See http://developer.apple.com/qa/qa2004/qa1398.html */
  #include <mach/mach_time.h>
  typedef uint64_t gasneti_tick_t;
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  #define gasneti_ticks_now() mach_absolute_time()
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    if_pf (gasneti_timer_firstTime) {
      mach_timebase_info_data_t _tb;
      gasneti_assert_zeroret(mach_timebase_info(&_tb));
      gasneti_timer_Tick = ((double)_tb.numer) / ((double)_tb.denom);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(_st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif GASNETI_HAVE_AARCH64_CNTVCT_EL0 /* AARCH64/ARMv8 Virtual Timer Count register */
  #include <sys/times.h>
  typedef uint64_t gasneti_tick_t;
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
 #if GASNETI_HAVE_GCC_ASM
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    gasneti_tick_t _ret;

    __asm__ __volatile__ ("isb\n\t"
                          "mrs %0,CNTVCT_EL0" :
                          "=r" (_ret) :
                          /* no inputs */ :
                          "memory");
    return _ret;
  }
 #else
  #define GASNETI_USING_SLOW_TIMERS
 #endif

  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    if_pf (gasneti_timer_firstTime) {
      uint64_t _freq;
      __asm__ __volatile__ ("mrs %0,CNTFRQ_EL0" : "=r" (_freq));
      gasneti_timer_Tick = 1.0E9/_freq;
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(_st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_MICROBLAZE && (defined(MB_CC) || defined(MB_FSL_CC))
  typedef uint64_t gasneti_tick_t;
  GASNETI_INLINE(gasneti_ticks_now)
 #if GASNETI_HAVE_GCC_ASM
  gasneti_tick_t gasneti_ticks_now(void) {
    unsigned int _msr, _tmp;
    gasneti_tick_t _retval;

    #if defined(MB_CC)
      __asm__ __volatile__("mfs %0, rmsr\n\t"
                           "andi %1, %0, ~2\n\t"
                           "mts rmsr, %1\n\t"
                           "mfchl %M2\n\t"
                           "mfccl %L2\n\t"
                           "mts rmsr, %0\n\t"
                           : "=r"(_msr), "=r"(_tmp), "=r"(_retval)
                           : /* no inputs */);
    #elif defined(MB_FSL_CC)
      __asm__ __volatile__("mfs %0, rmsr\n\t"
                           "andi %1, %0, ~2\n\t"
                           "mts rmsr, %1\n\t"
                           "get %M2, rfsl6\n\t"
                           "cget %L2, rfsl7\n\t"
                           "mts rmsr, %0\n\t"
                           : "=r"(_msr), "=r"(_tmp), "=r"(_retval)
                           : /* no inputs */);
    #endif
    return _retval;
  }
 #else
   #define GASNETI_USING_SLOW_TIMERS 1
 #endif

  GASNETI_INLINE(gasneti_ticks_to_us)
  gasneti_tick_t gasneti_ticks_to_us(gasneti_tick_t _st) {
    unsigned int _h0 = (unsigned int) (_st >> 32);
    unsigned int _l0 = (unsigned int) (_st >>  0);

    if (_h0 == 0) return (((gasneti_tick_t) _l0) / MB_TICKS_PER_US);
    else return (_st / MB_TICKS_PER_US);
  }

  GASNETI_INLINE(gasneti_ticks_to_ns)
  gasneti_tick_t gasneti_ticks_to_ns(gasneti_tick_t _st) {
    unsigned int _h0 = (unsigned int) (_st >> 32);
    unsigned int _l0 = (unsigned int) (_st >>  0);

    if (_h0 == 0) return (((gasneti_tick_t) _l0) * (1000 / MB_TICKS_PER_US));
    else return (_st * (1000 / MB_TICKS_PER_US));
  }
/* ------------------------------------------------------------------------------------ */
#elif HAVE_CLOCK_GETTIME
  /* POSIX realtime support - when available, usually outperforms gettimeofday */
  #define GASNETI_USING_POSIX_REALTIME 1
/* ------------------------------------------------------------------------------------ */
#else /* use slow, portable timers */
  #define GASNETI_USING_GETTIMEOFDAY 1
#endif

#if GASNETI_USING_SLOW_TIMERS && GASNETI_COMPILER_IS_CC
  #error unknown compiler - dont know how to read timer registers
#endif

/* ------------------------------------------------------------------------------------ */
/* completely portable (low-performance) microsecond granularity wall-clock time */
extern uint64_t gasneti_gettimeofday_us(void);

/* completely portable (low-performance) potentially-nanosecond granularity wall-clock time */
extern uint64_t gasneti_wallclock_ns(void);

/* portable implementations */
#if defined(GASNETI_FORCE_GETTIMEOFDAY) || defined(GASNETI_USING_GETTIMEOFDAY)
  #undef GASNETI_USING_GETTIMEOFDAY
  #define GASNETI_USING_GETTIMEOFDAY 1
  /* portable microsecond granularity wall-clock timer */
  typedef uint64_t _gasneti_tick_t;
  extern uint64_t gasneti_ticks_gtod_us(void);
  #undef gasneti_tick_t
  #define gasneti_tick_t _gasneti_tick_t
  #undef gasneti_ticks_to_us
  #define gasneti_ticks_to_us(st)  ((gasneti_tick_t)(st))
  #undef gasneti_ticks_to_ns
  #define gasneti_ticks_to_ns(st)  (((gasneti_tick_t)(st))*1000)
  #undef gasneti_ticks_now
  #define gasneti_ticks_now()      ((gasneti_tick_t)gasneti_ticks_gtod_us())
#elif defined(GASNETI_FORCE_POSIX_REALTIME) || defined(GASNETI_USING_POSIX_REALTIME)
  #undef GASNETI_USING_POSIX_REALTIME 
  #define GASNETI_USING_POSIX_REALTIME 1
  typedef uint64_t _gasneti_tick_t;
  extern uint64_t gasneti_ticks_posix_ns(void);
  #undef gasneti_tick_t
  #define gasneti_tick_t _gasneti_tick_t
  #undef gasneti_ticks_now
  #define gasneti_ticks_now() gasneti_ticks_posix_ns()

  #undef gasneti_ticks_to_us
  #define gasneti_ticks_to_us(st)  (((gasneti_tick_t)(st))/1000)
  #undef gasneti_ticks_to_ns
  #define gasneti_ticks_to_ns(st)  ((gasneti_tick_t)(st))
#endif

#ifdef gasneti_ticks_now
  // bug 3458: may be using posixrt or gettimeofday
  #undef GASNETI_TICKS_NOW_BODY
#elif defined(GASNETI_TICKS_NOW_BODY)
  #define GASNETI_USING_SLOW_TIMERS 1
  extern void gasneti_slow_ticks_now(void);
  #define gasneti_ticks_now()    ((*(gasneti_tick_t (*)(void))(&gasneti_slow_ticks_now))())
#elif defined(GASNETI_USING_SLOW_TIMERS)
  // Compiler w/o necessary asm support (could be CXX, MPI_CC or unknown)
  extern gasneti_tick_t gasneti_slow_ticks_now(void);
  #define gasneti_ticks_now()    gasneti_slow_ticks_now()
#endif

#ifndef gasneti_ticks_to_us
  #define gasneti_ticks_to_us(st)  (gasneti_ticks_to_ns(st)/1000)
#endif

#ifndef GASNETI_TICK_MIN
#define GASNETI_TICK_MIN        ((gasneti_tick_t)0)
#endif
#ifndef GASNETI_TICK_MAX
#define GASNETI_TICK_MAX        ((gasneti_tick_t)-1)
#endif

#if GASNETI_USING_GETTIMEOFDAY
  #if defined(GASNETI_FORCE_GETTIMEOFDAY)
    #define GASNETI_TIMER_CONFIG   timers_forced_os
  #else
    #define GASNETI_TIMER_CONFIG   timers_os
  #endif
#elif GASNETI_USING_POSIX_REALTIME
  #if defined(GASNETI_FORCE_POSIX_REALTIME)
    #define GASNETI_TIMER_CONFIG   timers_forced_posixrt
  #else
    #define GASNETI_TIMER_CONFIG   timers_posixrt
  #endif
#else
  #define GASNETI_TIMER_CONFIG   timers_native
#endif

/* return a double value representing the approximate microsecond
   overhead and granularity of the current timers. Overhead is the 
   average wall-clock time consumed while reading the timer value once,
   and granularity is the minimum observable non-zero interval between 
   two timer readings (which may be limited only by overhead, or may 
   be significantly higher on systems where the underlying timer 
   advances in discrete "jumps" of time much larger than the overhead)
   When measuring an event of length (L) using two surrounding timer calls,
   the measured time interval will be: L + overhead +- granularity
*/
extern double gasneti_tick_metric(int _idx);
#define gasneti_tick_granularity() gasneti_tick_metric(0)
#define gasneti_tick_overhead()    gasneti_tick_metric(1)
/* ------------------------------------------------------------------------------------ */

/* Initialization of timer subsystem.
 * Should perform any/all expensive operations.*/
#ifndef GASNETI_TICKS_INIT
  #define GASNETI_TICKS_INIT() do {       \
    (void)gasneti_ticks_now();          \
    (void)gasneti_ticks_to_ns(1);       \
    (void)gasneti_tick_granularity();   \
  } while (0)
#endif

/* ------------------------------------------------------------------------------------ */

#endif
