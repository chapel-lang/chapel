/* Cross-compilation Instructions: 
  1. Build the program below using the target compiler (the one that builds
     executables for your compute nodes). It is best *not* to pass '-O' or
     similar, because optimization can interfere with some tests. If compilation
     fails, try tweaking one of the test control variables below (and you'll
     need to manually indicate the result for that test). This program basically
     precomputes all the runtime values that configure will need and outputs a
     script that feeds the canned answers to configure.
  2. Run the built program on one of the compute nodes and save the output
     into a file in the top-level source directory named "cross-configure".
  3. Set the new script to be executable: chmod +x cross-configure
  4. Edit the "cross-configure" script for completeness, notably setting the 
     full path to your target and host compilers.
  5. Run cross-configure with the same options you'd pass to configure
     (eg. see cross-configure --help)
 */

/* --- Test control variables --- */
#ifndef CHECK_STACK
#define CHECK_STACK 1        /* test for stack growth direction */
#endif
#ifndef CHECK_TYPES
#define CHECK_TYPES 1        /* test for sizes of basic C types */
#endif
#ifndef CHECK_SIGNED_CHAR
#define CHECK_SIGNED_CHAR 1  /* test whether char is signed */
#endif
#ifndef CHECK_SIGNALS
#define CHECK_SIGNALS 1      /* test for system signal values */
#endif
#ifndef CHECK_PAGESIZE
#define CHECK_PAGESIZE 1     /* test for system page size */
#endif
#ifndef CHECK_MMAP
  #ifdef __CYGWIN__
    #define CHECK_MMAP 0     /* bug 3370: mmap() is known to be broken in subtle ways on cygwin */
  #else
    #define CHECK_MMAP 1     /* test for working mmap() */
  #endif
#endif
#ifndef CHECK_ARM_CMPXCHG
#define CHECK_ARM_CMPXCHG 1  /* test for ARM cmpxchg support (ignored on other arch) */
#endif
#ifndef CHECK_ARM_MEMBAR
#define CHECK_ARM_MEMBAR 1   /* test for ARM membar/SMP support (ignored on other arch) */
#endif
/* ------------------------------ */
#ifndef CHECK_VERBOSE
#define CHECK_VERBOSE 1         /* enable verbose error reporting during checks */
#endif

#include <stdio.h>
#include <string.h>

void warning(const char *str) {
  fprintf(stdout,"# WARNING: %s - you'll need to edit the value below for correctness\n", str);
  fflush(stdout);
  fprintf(stderr,"# WARNING: %s - you'll need to edit the resulting script for correctness\n", str);
  fflush(stderr);
}
#if CHECK_VERBOSE
  #include <errno.h>
  #define VERBOSE(args) do { \
      printf("# Auto-detect status: "); printf args; \
      printf(", errno=%i '%s'\n", errno, strerror(errno)); \
  } while (0)
#else
  #define VERBOSE(args) ((void)0)
#endif
#define NOOP_CHECK(type, name,str) \
type name(void) { \
    char msg[255]; \
    sprintf(msg,"failed to auto-detect %s",str); \
    warning(msg); \
    return 0; \
}

#if CHECK_STACK
int stack_check_help(volatile int *p, int x) {
    volatile int local = 1;
    if (x == 0) return stack_check_help(&local,x+1);
    else if (x < 100) return stack_check_help(p,x+1);
    else if (&local > p) return 1;
    else return 0;
}
int stack_check(void) { 
  volatile int local = 0;
  return stack_check_help(NULL, 0);
}
#else
NOOP_CHECK(int,stack_check,"stack growth direction")
#endif

#if CHECK_TYPES
 #include <unistd.h>
 #include <stddef.h>
 #define CHECK_TYPE(tname,name) do { \
   int typesz = (int)(sizeof(tname)); \
   printf("CROSS_SIZEOF_"#name"='%i' ; export CROSS_SIZEOF_"#name"\n", typesz); \
   if (typesz < 1) warning("failed to auto-detect sizeof(" #tname ")"); \
 } while (0)
#else
 #define CHECK_TYPE(tname,name) do { \
   warning("failed to auto-detect sizeof(" #tname ")"); \
   printf("CROSS_SIZEOF_"#name"='' ; export CROSS_SIZEOF_"#name"\n"); \
 } while (0)
#endif

#if CHECK_SIGNED_CHAR
int signed_char_check(void) { 
  char c = (char)(int)-5;
  int char_signed = 1;
  if (c > 0) char_signed = 0;
  return char_signed;
}
#else
NOOP_CHECK(int,signed_char_check,"char signedness")
#endif

#if CHECK_SIGNALS
 #include <signal.h>
 #define CHECK_SIG(name) do { \
   int sigval = (int)(SIG##name); \
   printf("CROSS_SIG"#name"='%i' ; export CROSS_SIG"#name"\n",sigval); \
   if (sigval < 1) warning("failed to auto-detect system's SIG" #name " value"); \
 } while (0)
#else
 #define CHECK_SIG(name) do { \
   warning("failed to auto-detect system's SIG" #name " value"); \
   printf("CROSS_SIG"#name"='' ; export CROSS_SIG"#name"\n"); \
 } while (0)
#endif

#if CHECK_PAGESIZE
#include <unistd.h>
#include <limits.h>
unsigned long pagesize_check(void) { 
  unsigned long pagesize = 0;
  /* take the first non-zero value, checked in this order */
  #ifdef PAGESIZE
    if (pagesize < 1) pagesize = PAGESIZE;
  #endif
  #ifdef PAGE_SIZE
    if (pagesize < 1) pagesize = PAGE_SIZE;
  #endif
  #ifdef _SC_PAGESIZE
    if (pagesize < 1) pagesize = sysconf(_SC_PAGESIZE);
  #endif
  #ifdef _SC_PAGE_SIZE
    if (pagesize < 1) pagesize = sysconf(_SC_PAGE_SIZE);
  #endif
  if (pagesize < 1)
      warning("failed to auto-detect system page size");
  return pagesize;
}
#else
NOOP_CHECK(unsigned long,pagesize_check,"system page size")
#endif

int mmap_check();
#if defined(__arm__)
  int arm_cmpxchg_check();
  int arm_membar_check();
#endif

int main(void) {

  printf("#!/bin/sh\n\n");
  printf("# This is an automatically-generated cross-configuration setup script\n");
  printf("\n################################################\n");
  printf("# Usage Instructions: \n");
  printf("#  1. fill in the following values to point to the target and host compilers:\n\n");
  printf("CC='cc' ; export CC  # vanilla target C compiler\n");
  printf("CXX='c++' ; export CXX  # vanilla target C++ compiler\n");

  printf("\n# Host C compiler\n");
  printf("# NOTE: you may need to set GCC_EXEC_PREFIX=/usr/bin/\n");
  printf("# to get the correct assembler if gcc is your host compiler\n");
  printf("HOST_CC='/usr/bin/gcc' ; export HOST_CC\n");
  printf("HOST_CFLAGS='-g -O2' ; export HOST_CFLAGS\n");

  printf("\n# Host C++ compiler - required by Titanium, not required for UPC or GASNet\n");
  printf("HOST_CXX='/usr/bin/g++' ; export HOST_CXX\n");
  printf("HOST_CXXFLAGS='' ; export HOST_CXXFLAGS\n");

  printf("\n# Optional additional settings: (see configure --help for complete list)\n\n");
  printf("#MPI_CC='mpicc' ; export MPI_CC     # MPI-enabled C compiler\n");
  printf("#MPI_CFLAGS='' ; export MPI_CFLAGS  # flags for MPI_CC\n");
  printf("#MPI_LIBS='' ; export MPI_LIBS      # libs for linking with MPI_CC\n");
  printf("#MPIRUN_CMD='mpirun -np %%N %%C' ; export MPIRUN_CMD  # launch command for MPI jobs\n");
  printf("EXTRA_CONFIGURE_ARGS='' ; export EXTRA_CONFIGURE_ARGS  # misc configure args to pass\n");
  printf("\n# 2. Fill in the canonical target machine type. You can usually obtain this\n");
  printf("#   by running config-aux/config.guess on the target machine\n");
  printf("TARGET_ID=''\n");
  printf("\n# 3. Optionally cross-compile and run the detect-cachesz.c utility and\n");
  printf("#    fill in the value below and uncomment.  If not set the default is platform-dependent.\n");
  printf("#CACHE_LINE_BYTES=???; export CACHE_LINE_BYTES\n");
  printf("\n# 4. Review the automatically-detected settings below and make corrections as necessary.\n");
  printf("\n# 5. Place this output script in your top-level source directory and run it,\n");
  printf("#   passing it any additional configure arguments as usual (see configure --help).\n");

  printf("\n################################################\n");
  printf("# AUTOMATICALLY DETECTED SETTINGS:\n\n");
#if defined(__arm__)
  printf("\n# Whether the ARM system has working kernel support for cmpxchg\n\n");
  printf("CROSS_HAVE_ARM_CMPXCHG='%i' ; export CROSS_HAVE_ARM_CMPXCHG\n",arm_cmpxchg_check());

  printf("\n# Whether the ARM system has working kernel support for memory barriers\n\n");
  printf("CROSS_HAVE_ARM_MEMBAR='%i' ; export CROSS_HAVE_ARM_MEMBAR\n",arm_membar_check());
#endif

  printf("\n# Whether the system has a working version of anonymous mmap\n\n");
  printf("CROSS_HAVE_MMAP='%i' ; export CROSS_HAVE_MMAP\n",mmap_check());

  printf("\n# The system VM page size (ie mmap granularity, even if swapping is not supported)\n\n");
  printf("CROSS_PAGESIZE='%lu' ; export CROSS_PAGESIZE\n", pagesize_check());

  printf("\n# Does the system stack grow up?\n\n");
  printf("CROSS_STACK_GROWS_UP='%i' ; export CROSS_STACK_GROWS_UP\n", stack_check());

  printf("\n# Is char a signed type?\n\n");
  printf("CROSS_CHAR_IS_SIGNED='%i' ; export CROSS_CHAR_IS_SIGNED\n", signed_char_check());

  printf("\n# Basic primitive C type sizes (in bytes)\n\n");
  CHECK_TYPE(char,CHAR);
  CHECK_TYPE(short,SHORT);
  CHECK_TYPE(int,INT);
  CHECK_TYPE(long,LONG);
  CHECK_TYPE(long long,LONG_LONG);
  CHECK_TYPE(void *,VOID_P);
  CHECK_TYPE(size_t,SIZE_T);
  CHECK_TYPE(ptrdiff_t,PTRDIFF_T);

  printf("\n# System signal values\n\n");
  CHECK_SIG(HUP);
  CHECK_SIG(INT);
  CHECK_SIG(QUIT);
  CHECK_SIG(KILL);
  CHECK_SIG(TERM);
  CHECK_SIG(USR1);

  printf(
"\n# Find source directory\n\n"
"if test -z \"$SRCDIR\" || test ! -f \"$SRCDIR/configure\" ; then\n"
"  SRCDIR=`dirname $0`\n"
"fi\n"
"if test ! -f \"$SRCDIR/configure\" ; then\n"
"  echo \"ERROR: Source directory not found. Either set \\$SRCDIR=/path/to/src or copy/link the `basename $0` script to the same directory as the configure script\"\n"
"  exit 1\n"
"fi\n\n"
"# Detect the build+host machine type\n"
"HOST_APPEND=\n"
"for sys in host build; do\n"
"  SYS_ARG=`echo \"$@\" | grep -i -e --$sys`\n"
"  if test -z \"$SYS_ARG\"; then\n"
"    if test -z \"$HOST_ID\"; then\n"
"      HOST_ID=`env CC= HOST_CC= CC_FOR_BUILD= $SRCDIR/config-aux/config.guess`\n"
"      if test -z \"$HOST_ID\"; then\n"
"        echo 'ERROR: failed to auto-detect build host. Please run with --host=machineid --build=machineid to identify the host machine running this script'\n"
"        exit 1\n"
"      fi\n"
"    fi\n"
"    HOST_APPEND=\"$HOST_APPEND --$sys=$HOST_ID\"\n"
"  fi\n"
"done\n"
"# Now that everything is setup, run the actual configure script\n"
"$SRCDIR/configure --enable-cross-compile$HOST_APPEND --target=$TARGET_ID $EXTRA_CONFIGURE_ARGS \"$@\"\n"
);

  fflush(stdout);
  return 0;
}

#if CHECK_MMAP
  #include <unistd.h>
  #include <sys/mman.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <stdlib.h>
  char junk[16384];
  int testfd(int);
  int mmap_check(void) {
    int fd, retval;
    #if 0
      char filename[255];
      tmpnam(filename); /* unsafe */
      fd = open(filename, O_RDWR | O_CREAT);
    #else
      char filename[255];
      if (getenv("TMPDIR")) strcpy(filename,getenv("TMPDIR"));
      else strcpy(filename,"/tmp");
      strcat(filename,"/gasnet-conftemp-XXXXXX");
      fd = mkstemp(filename); /* leaves crap laying around */
    #endif
    retval = testfd(fd);
    close(fd);
    remove(filename);
    return (retval == 0);
  }
  int testfd(int fd) {
    void *ptr,*ptr2;
    if (fd == -1) { VERBOSE(("failed to open file")); return 1; }
    if (write(fd, junk, 16384) == -1) { VERBOSE(("failed to write")); return 2; }
    ptr = mmap(0, 16384, (PROT_READ|PROT_WRITE),
        MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED || ptr == NULL) { VERBOSE(("failed to mmap anon")); return 3; }
    if (munmap(ptr,16384) != 0)  { VERBOSE(("failed to munmap anon")); return 4; }
    ptr2 = mmap(ptr, 16384, (PROT_READ|PROT_WRITE),
        (MAP_PRIVATE | MAP_FIXED), fd, 0);
    if (ptr2 == MAP_FAILED || ptr2 == NULL || ptr2 != ptr)  { VERBOSE(("failed to mmap fixed")); return 5; }
    if (munmap(ptr,16384) != 0) { VERBOSE(("failed to munmap fixed")); return 6; }
    return 0;
  }
#else
  NOOP_CHECK(int,mmap_check,"mmap operation")
#endif

#if defined(__arm__) && \
    (defined(__linux) || defined(__linux__) || defined(__gnu_linux__))
  #define GASNETI_IN_CONFIGURE 1
  #include "gasnet_arch_arm.h"

  #if CHECK_ARM_CMPXCHG
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    int arm_cmpxchg_check(void) {
	/* Since failure may crash, run in a child process */
	int pid;

	fflush(NULL);
	pid = fork();
	if (pid < 0) {
	    warning("ARM cmpxchg check failed to fork()");
	    return 0;
	} else if (pid == 0) {
            exit( gasneti_arm_cmpxchg_check() );
	} else {
	    int rc, status;
	    rc = waitpid(pid, &status, 0);
	    return (WIFEXITED(status) && !WEXITSTATUS(status));
	}
    }
  #else
    NOOP_CHECK(int,arm_cmpxchg_check,"ARM cmpxchg support")
  #endif

  #if CHECK_ARM_MEMBAR
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    int arm_membar_check(void) {
	/* Since failure may crash, run in a child process */
	int pid;

	fflush(NULL);
	pid = fork();
	if (pid < 0) {
	    warning("ARM membar check failed to fork()");
	    return 0;
	} else if (pid == 0) {
            exit( gasneti_arm_membar_check() );
	} else {
	    int rc, status;
	    rc = waitpid(pid, &status, 0);
	    return (WIFEXITED(status) && !WEXITSTATUS(status));
	}
    }
  #else
    NOOP_CHECK(int,arm_membar_check,"ARM membar support")
  #endif
#endif
