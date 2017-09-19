/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_timer.h $
 * Description: GASNet Timer library (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnet.h or gasnet_tools.h
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

#if defined(GASNETC_CONDUIT_SPECIFIC_TIMERS)
  #if !defined(gasneti_ticks_to_ns) || !defined(gasneti_ticks_now)
    /* conduit-specific timers must be implemented using a macro */
    #error Incomplete conduit-specific timer impl.
  #endif
#elif defined(GASNETI_FORCE_GETTIMEOFDAY) || defined(GASNETI_FORCE_POSIX_REALTIME)
/* bug3508: forced portable timer implementation overrides compilation of native timers */
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_OS_MTA
  #include <sys/mta_task.h>
  #include <machine/mtaops.h>

  typedef int64_t gasneti_tick_t;
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t ticks) {
    if_pf(gasneti_timer_firstTime) {
      double freq = mta_clock_freq();
      gasneti_timer_Tick = 1.0E9/freq;
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
      #if 0
        printf("first time: ticks=%" PRId64 "  freq=%f adjust=%f\n", 
               ticks, freq, adjust);
      #endif
    } else gasneti_sync_reads();
    return (uint64_t)(((double)ticks) * gasneti_timer_Tick);
  }
  #define gasneti_ticks_now()      (MTA_CLOCK(0))
  #define GASNETI_TICK_MAX        ((gasneti_tick_t)(((uint64_t)-1)>>1))
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
      hrtime_t in;
      gasneti_tick_t out;
    } t;
    t.in = gethrtime();
    return t.out;
  }
  #define gasneti_ticks_to_ns(st)  (st)
#else
  typedef hrtime_t gasneti_tick_t;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    gasneti_assert(sizeof(gasneti_tick_t) == 8);
    return *(uint64_t*)&st;
  }
  #define gasneti_ticks_now()      (gethrtime())
  #define GASNETI_TICK_MAX        ((gasneti_tick_t)(((uint64_t)-1)>>1))
#endif
/* ------------------------------------------------------------------------------------ */
#elif GASNETI_USE_MMTIMER
  /* use IA-PC HPET (High Precision Event Timers) */
  #define GASNETI_HPET_MMAP 1
  #include <sys/ioctl.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  #include <fcntl.h>
  #include <unistd.h>
  #if HAVE_SN_MMTIMER_H
    #include <sn/mmtimer.h> 
  #elif HAVE_LINUX_MMTIMER_H
    #include <linux/mmtimer.h>
  #endif
  #ifndef MMTIMER_FULLNAME
  #define MMTIMER_FULLNAME "/dev/mmtimer"
  #endif
  typedef uint64_t gasneti_tick_t;
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_tick = 0.0; \
         int gasneti_timer_fd = 0; \
         volatile uint64_t *gasneti_tick_p = NULL;
  extern double gasneti_timer_tick; /* tick conversion factor */
  extern int gasneti_timer_fd; /* HPET device file descriptor */
  extern volatile uint64_t *gasneti_tick_p; /* pointer to mapped counter, and init flag */
  GASNETI_NEVER_INLINE(gasneti_timer_init,
  static volatile uint64_t *gasneti_timer_init(void)) {
    if_pf (!gasneti_tick_p) {
      int result;
      uint64_t val = 0;
      if ((gasneti_timer_fd = open(MMTIMER_FULLNAME, O_RDONLY)) == -1) 
         gasneti_fatalerror("failed to open %s", MMTIMER_FULLNAME);
      if ((result = ioctl(gasneti_timer_fd, MMTIMER_GETFREQ, &val)) == -ENOSYS) 
         gasneti_fatalerror("failed to MMTIMER_GETFREQ");
      gasneti_assert(val >= 10000000); /* 10 MHz min reqd by spec */
      gasneti_timer_tick = 1.0E9 / val;
      gasneti_assert(gasneti_timer_tick != 0.0);
      #if GASNETI_HPET_MMAP
      { void *loc;
        int offset; 
        gasneti_assert_always(ioctl(gasneti_timer_fd, MMTIMER_MMAPAVAIL, 0) == 1);
        offset = ioctl(gasneti_timer_fd, MMTIMER_GETOFFSET, 0); /* fetch offset of counter in mmap page */
        gasneti_assert_always(offset >= 0 && offset < GASNETI_PAGESIZE-8);
        loc = mmap(NULL, GASNETI_PAGESIZE, PROT_READ, MAP_PRIVATE, gasneti_timer_fd, 0);
        if (loc == NULL || loc == MAP_FAILED) 
          gasneti_fatalerror("failed to mmap MMTIMER: %s",strerror(errno));
        close(gasneti_timer_fd); /* fd is no longer required */
        gasneti_timer_fd = -1;
        gasneti_sync_writes();
        gasneti_tick_p = (uint64_t *)(((char*)loc) + offset);
      }
      #else
        gasneti_sync_writes();
        gasneti_tick_p = (volatile uint64_t *)1;
      #endif
    } else gasneti_sync_reads();
    return gasneti_tick_p;
  }
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    volatile uint64_t *ptr = gasneti_tick_p; 
    if_pf (!ptr) ptr = gasneti_timer_init();
    #if GASNETI_HPET_MMAP
      return *ptr;
    #else /* use ioctl - this works, but is actually slower than gettimeofday */
    { uint64_t val = 0;
      int result;
      #if GASNET_DEBUG
        result = ioctl(gasneti_timer_fd, MMTIMER_GETCOUNTER, &val);
        if_pf (result == -ENOSYS) gasneti_fatalerror("failed to MMTIMER_GETCOUNTER: %i %s", result, strerror(result));
      #else
        ioctl(gasneti_timer_fd, MMTIMER_GETCOUNTER, &val); 
      #endif
      return (gasneti_tick_t)val;
    }
    #endif
  }
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    gasneti_assert(gasneti_tick_p);
    return (uint64_t)(st * gasneti_timer_tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif (PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL || PLATFORM_OS_OPENBSD || \
       GASNETI_HAVE_SYSCTL_MACHDEP_TSC_FREQ) && \
     (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || PLATFORM_COMPILER_SUN || \
      PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_PGI || PLATFORM_COMPILER_TINY || \
      PLATFORM_COMPILER_OPEN64 || PLATFORM_COMPILER_CRAY || PLATFORM_COMPILER_CLANG) && \
     (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC || PLATFORM_ARCH_IA64) && \
      !(PLATFORM_ARCH_IA64 && GASNETI_ARCH_ALTIX) /* bug 1622 */
  #if PLATFORM_ARCH_IA64 && PLATFORM_COMPILER_INTEL
    #include <ia64intrin.h>
  #elif GASNETI_HAVE_SYSCTL_MACHDEP_TSC_FREQ || PLATFORM_OS_OPENBSD
    #include <sys/sysctl.h> 
  #endif
  typedef uint64_t gasneti_tick_t;
 #if (PLATFORM_COMPILER_PGI && !GASNETI_PGI_ASM_GNU) || PLATFORM_COMPILER_SUN
   /* The current compiler lacks full GNU-style asm() support.
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
 #else
  GASNETI_INLINE(gasneti_ticks_now)
  uint64_t gasneti_ticks_now (void) {
    uint64_t ret;
    #if PLATFORM_COMPILER_CRAY
      ret = _rtc();
    #elif PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC || \
        (PLATFORM_COMPILER_PGI && PLATFORM_ARCH_X86 && !GASNETI_PGI_ASM_X86_A)
      /* This asm() for x86-64 also works for x86 compilers w/o working support
       * for the "A" constraint (currently only pgcc 6.1-x, which crashes).
       */
      uint32_t lo, hi;
      __asm__ __volatile__("rdtsc"
                           : "=a" (lo), "=d" (hi)
                           /* no inputs */); 
      ret = ((uint64_t)lo) | (((uint64_t)hi)<<32);
    #elif PLATFORM_ARCH_X86
      __asm__ __volatile__("rdtsc"
                           : "=A" (ret)
                           /* no inputs */); 
    #elif PLATFORM_ARCH_IA64 && PLATFORM_COMPILER_INTEL
      ret = (uint64_t)__getReg(_IA64_REG_AR_ITC);
    #elif PLATFORM_ARCH_IA64
      __asm__ __volatile__("mov %0=ar.itc" 
                           : "=r"(ret) 
                           /* no inputs */);
    #else
      #error "unsupported CPU"
    #endif
    return ret;
  } 
 #endif
  extern double gasneti_calibrate_tsc(void);
  #define GASNETI_CALIBRATE_TSC 1
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    if_pf (gasneti_timer_firstTime) {
      gasneti_timer_Tick = gasneti_calibrate_tsc(); /* Too much to inline */
      gasneti_assert(gasneti_timer_Tick != 0.0);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_POWERPC && \
      ( PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_XLC || \
        PLATFORM_COMPILER_CLANG || PLATFORM_COMPILER_PGI ) && \
      ( PLATFORM_OS_LINUX || PLATFORM_OS_BGQ)
  /* Use the 64-bit "timebase" register on both 32- and 64-bit PowerPC CPUs */
  #include <sys/types.h>
  #include <dirent.h>
  typedef uint64_t gasneti_tick_t;
 #if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_CLANG || PLATFORM_COMPILER_PGI || \
     (PLATFORM_COMPILER_XLC && GASNETI_HAVE_GCC_ASM && !GASNETI_HAVE_XLC_ASM)
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
    uint64_t ret;
    #if PLATFORM_ARCH_64
      __asm__ __volatile__(GASNETI_MFTB(0)
                           : "=r" (ret)
                           : /* no inputs */); 
    #else
      /* Note we must read hi twice to protect against wrap of lo */
      uint32_t o_hi, hi, lo;
      __asm__ __volatile__("0: \n\t"
		           GASNETI_MFTBU(0) "\n\t"
			   GASNETI_MFTBL(1) "\n\t"
			   GASNETI_MFTBU(2) "\n\t"
			   "cmpw  %0, %2\n\t"
			   "bne- 0b\n\t"
                           : "=r" (o_hi), "=r" (lo), "=r" (hi)
                           : /* no inputs */); 
      ret = ((uint64_t)hi << 32) | lo;
    #endif
    return ret;
  } 

  #undef GASNETI_MFTB
  #undef GASNETI_MFTBL
  #undef GASNETI_MFTBU
 #elif GASNETI_HAVE_XLC_ASM
   #if PLATFORM_ARCH_64
      static uint64_t gasneti_ticks_now(void);
      #pragma mc_func gasneti_ticks_now {  \
        "7c6c42e6"      /* mftb r3         */ \
        /* RETURN counter in r3 */            \
      }
      #pragma reg_killed_by gasneti_ticks_now 
   #else
      static uint32_t gasneti_mftb_low(void);
      #pragma mc_func gasneti_mftb_low {  \
        "7c6c42e6"      /* mftb r3     */ \
        /* RETURN counter in r3 */        \
      }
      #pragma reg_killed_by gasneti_mftb_low 
      
      static uint32_t gasneti_mftb_high(void);
      #pragma mc_func gasneti_mftb_high {  \
        "7c6d42e6"      /* mftbu r3     */ \
        /* RETURN counter in r3 */         \
      }
      #pragma reg_killed_by gasneti_mftb_high 
      
      GASNETI_INLINE(gasneti_ticks_now)
      uint64_t gasneti_ticks_now(void) {
        uint32_t hi, hi2, lo;
        /* Note we must read hi twice to protect against wrap of lo */
        do {
           hi = gasneti_mftb_high();
           lo = gasneti_mftb_low();        
           hi2 = gasneti_mftb_high();
        } while (hi != hi2);
        return ((uint64_t)hi << 32) | lo;
      } 
   #endif
 #endif
  #define GASNETI_TIMER_DEFN \
         double gasneti_timer_Tick = 0.0; \
         int    gasneti_timer_firstTime = 1;
  extern double gasneti_timer_Tick; /* inverse GHz */
  extern int    gasneti_timer_firstTime;
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    if_pf (gasneti_timer_firstTime) {
      uint32_t freq;
     #if PLATFORM_OS_BGQ
      /* don't know how to query this, so hard-code it for now */
      freq = 1600000000;
     #else 
      DIR *dp = opendir("/proc/device-tree/cpus");
      struct dirent *de = NULL;
      FILE *fp = NULL;
      double MHz = 0.0;
      char fname[128];
      if (!dp) gasneti_fatalerror("*** ERROR: Failure in opendir('/proc/device-tree/cpus'): %s",strerror(errno));
      do {
        de = readdir(dp);
	if (de && 
           ( de->d_name == strstr(de->d_name, "PowerPC,") || /* PowerPC */
             de->d_name == strstr(de->d_name, "cpu@0") /* IBM cell */
           )) {
	  break;
	}
      } while (de);
      if (!de) gasneti_fatalerror("*** ERROR: Failure to find a PowerPC CPU in /proc/device-tree/cpus");
      snprintf(fname, sizeof(fname), "/proc/device-tree/cpus/%.*s/timebase-frequency", 24, de->d_name);
      closedir(dp);
      fp = fopen(fname, "r");
      if (!fp) gasneti_fatalerror("*** ERROR: Failure in fopen('%s','r'): %s\n",fname,strerror(errno));
      if (fread((void *)(&freq), sizeof(uint32_t), 1, fp) != 1) 
        gasneti_fatalerror("*** ERROR: Failure to read timebase frequency from '%s': %s", fname, strerror(errno));
    #if PLATFORM_ARCH_LITTLE_ENDIAN /* value is always big-endian */
      freq = ((freq & 0x000000ff) << 24) |
             ((freq & 0x0000ff00) <<  8) |
             ((freq & 0x00ff0000) >>  8) |
             ((freq & 0xff000000) >> 24);
    #endif
      fclose(fp);
      if (freq == 0) { /* Playstation3 */
        char input[255];
        fp = fopen("/proc/cpuinfo", "r");
        if (!fp) gasneti_fatalerror("*** ERROR: Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));
        while (!feof(fp) && fgets(input, 255, fp)) {
          if (strstr(input,"timebase")) {
            char *p = strchr(input,':');
            if (p) { freq = atoi(p+1); break; }
          }
        }
        fclose(fp);
      }
     #endif
      gasneti_assert(freq > 1000000 && freq < 2000000000); /* ensure it looks reasonable (1MHz to 2Ghz) */
      gasneti_timer_Tick = 1.0e9 / freq;
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(st * gasneti_timer_Tick);
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
    LARGE_INTEGER val;
    gasneti_assert_nzeroret(QueryPerformanceCounter(&val));
    gasneti_assert(val.QuadPart > 0);
    return (gasneti_tick_t)val.QuadPart;
  }
  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    if_pf (gasneti_timer_firstTime) {
      LARGE_INTEGER temp;
      gasneti_assert_nzeroret(QueryPerformanceFrequency(&temp));
      gasneti_timer_Tick = 1.0E9 / ((double)temp.QuadPart);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(st * gasneti_timer_Tick);
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
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    if_pf (gasneti_timer_firstTime) {
      mach_timebase_info_data_t tb;
      gasneti_assert_zeroret(mach_timebase_info(&tb));
      gasneti_timer_Tick = ((double)tb.numer) / ((double)tb.denom);
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(st * gasneti_timer_Tick);
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
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    gasneti_tick_t ret;

    __asm__ __volatile__ ("isb\n\t"
                          "mrs %0,CNTVCT_EL0" :
                          "=r" (ret) :
                          /* no inputs */ :
                          "memory");
    return ret;
  }

  GASNETI_INLINE(gasneti_ticks_to_ns)
  uint64_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    if_pf (gasneti_timer_firstTime) {
      uint64_t freq;
      __asm__ __volatile__ ("mrs %0,CNTFRQ_EL0" : "=r" (freq));
      gasneti_timer_Tick = 1.0E9/freq;
      gasneti_sync_writes();
      gasneti_timer_firstTime = 0;
    } else gasneti_sync_reads();
    return (uint64_t)(st * gasneti_timer_Tick);
  }
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_MICROBLAZE && (defined(MB_CC) || defined(MB_FSL_CC))
  typedef uint64_t gasneti_tick_t;
  GASNETI_INLINE(gasneti_ticks_now)
  gasneti_tick_t gasneti_ticks_now(void) {
    unsigned int msr, tmp;
    gasneti_tick_t retval;

    #if defined(MB_CC)
      __asm__ __volatile__("mfs %0, rmsr\n\t"
                           "andi %1, %0, ~2\n\t"
                           "mts rmsr, %1\n\t"
                           "mfchl %M2\n\t"
                           "mfccl %L2\n\t"
                           "mts rmsr, %0\n\t"
                           : "=r"(msr), "=r"(tmp), "=r"(retval)
                           : /* no inputs */);
    #elif defined(MB_FSL_CC)
      __asm__ __volatile__("mfs %0, rmsr\n\t"
                           "andi %1, %0, ~2\n\t"
                           "mts rmsr, %1\n\t"
                           "get %M2, rfsl6\n\t"
                           "cget %L2, rfsl7\n\t"
                           "mts rmsr, %0\n\t"
                           : "=r"(msr), "=r"(tmp), "=r"(retval)
                           : /* no inputs */);
    #endif
    return retval;
  }

  GASNETI_INLINE(gasneti_ticks_to_us)
  gasneti_tick_t gasneti_ticks_to_us(gasneti_tick_t st) {
    unsigned int h0 = (unsigned int) (st >> 32);
    unsigned int l0 = (unsigned int) (st >>  0);

    if (h0 == 0) return (((gasneti_tick_t) l0) / MB_TICKS_PER_US);
    else return (st / MB_TICKS_PER_US);
  }

  GASNETI_INLINE(gasneti_ticks_to_ns)
  gasneti_tick_t gasneti_ticks_to_ns(gasneti_tick_t st) {
    unsigned int h0 = (unsigned int) (st >> 32);
    unsigned int l0 = (unsigned int) (st >>  0);

    if (h0 == 0) return (((gasneti_tick_t) l0) * (1000 / MB_TICKS_PER_US));
    else return (st * (1000 / MB_TICKS_PER_US));
  }
/* ------------------------------------------------------------------------------------ */
#elif HAVE_CLOCK_GETTIME
  /* POSIX realtime support - when available, usually outperforms gettimeofday */
  #define GASNETI_USING_POSIX_REALTIME 1
/* ------------------------------------------------------------------------------------ */
#else /* use slow, portable timers */
  #define GASNETI_USING_GETTIMEOFDAY 1
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
  #undef gasneti_tick_t
  #define gasneti_tick_t _gasneti_tick_t
  #undef gasneti_ticks_to_us
  #define gasneti_ticks_to_us(st)  ((gasneti_tick_t)(st))
  #undef gasneti_ticks_to_ns
  #define gasneti_ticks_to_ns(st)  (((gasneti_tick_t)(st))*1000)
  #undef gasneti_ticks_now
  #define gasneti_ticks_now()      ((gasneti_tick_t)gasneti_gettimeofday_us())
#elif defined(GASNETI_FORCE_POSIX_REALTIME) || defined(GASNETI_USING_POSIX_REALTIME)
  #undef GASNETI_USING_POSIX_REALTIME 
  #define GASNETI_USING_POSIX_REALTIME 1
  typedef uint64_t _gasneti_tick_t;
  #undef gasneti_tick_t
  #define gasneti_tick_t _gasneti_tick_t
  #undef gasneti_ticks_now
  #define gasneti_ticks_now() gasneti_wallclock_ns()

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
extern double gasneti_tick_metric(int idx);
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
