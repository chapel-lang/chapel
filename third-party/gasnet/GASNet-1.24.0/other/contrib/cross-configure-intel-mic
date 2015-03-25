#!/bin/sh

## Cross configure script for Intel MIC

################################################
# Usage Instructions: 
#  1. fill in the following values to point to the target and host compilers:

CC='icc -mmic' ; export CC  # vanilla target C compiler
CXX='icpc -mmic' ; export CXX  # vanilla target C++ compiler

# Host C compiler
# NOTE: you may need to set GCC_EXEC_PREFIX=/usr/bin/
# to get the correct assembler if gcc is your host compiler
HOST_CC='icc' ; export HOST_CC
HOST_CFLAGS='-g -O2' ; export HOST_CFLAGS

# Host C++ compiler - required by Titanium, not required for UPC or GASNet
HOST_CXX='icpc' ; export HOST_CXX
HOST_CXXFLAGS='-g -O2' ; export HOST_CXXFLAGS

# Optional additional settings: (see configure --help for complete list)

MPI_CC='mpiicc -mmic' ; export MPI_CC     # MPI-enabled C compiler
MPI_CFLAGS='' ; export MPI_CFLAGS  # flags for MPI_CC
MPI_LIBS='' ; export MPI_LIBS      # libs for linking with MPI_CC
MPIRUN_CMD='mpirun -n %N -hosts mic0 %C' ; # Todo: use a hostfile for MICs, e.g., "-hostfile $PBS_MICFILE"
export MPIRUN_CMD  # launch command for MPI jobs
EXTRA_CONFIGURE_ARGS='--disable-aligned-segments --enable-pshm' ; export EXTRA_CONFIGURE_ARGS  # misc configure args to pass

# 2. Fill in the canonical target machine type. You can usually obtain this
#   by running config-aux/config.guess on the target machine
TARGET_ID='mic-unknown-linux-gnu'

# 3. Optionally cross-compile and run the detect-cachesz.c utility and
#    fill in the value below and uncomment.  If not set the default is 128.
CROSS_CACHE_LINE_BYTES=64; export CROSS_CACHE_LINE_BYTES

# 4. Review the automatically-detected settings below and make corrections as necessary.

# 5. Place this output script in your top-level source directory and run it,
#   passing it any additional configure arguments as usual (see configure --help).

################################################
# AUTOMATICALLY DETECTED SETTINGS:


# Whether the system has a working version of anonymous mmap

CROSS_HAVE_MMAP='1' ; export CROSS_HAVE_MMAP

# The system VM page size (ie mmap granularity, even if swapping is not supported)
## YZ: MIC supports 2MB pages with the "MAP_HUGETLB" flag in
## mmap. Shoud check how to enable huge pages later as an
## optimization!
CROSS_PAGESIZE='4096' ; export CROSS_PAGESIZE

# Does the system stack grow up?

CROSS_STACK_GROWS_UP='0' ; export CROSS_STACK_GROWS_UP

# Is char a signed type?

CROSS_CHAR_IS_SIGNED='1' ; export CROSS_CHAR_IS_SIGNED

# Basic primitive C type sizes (in bytes)

CROSS_SIZEOF_CHAR='1' ; export CROSS_SIZEOF_CHAR
CROSS_SIZEOF_SHORT='2' ; export CROSS_SIZEOF_SHORT
CROSS_SIZEOF_INT='4' ; export CROSS_SIZEOF_INT
CROSS_SIZEOF_LONG='8' ; export CROSS_SIZEOF_LONG
CROSS_SIZEOF_LONG_LONG='8' ; export CROSS_SIZEOF_LONG_LONG
CROSS_SIZEOF_VOID_P='8' ; export CROSS_SIZEOF_VOID_P
CROSS_SIZEOF_SIZE_T='8' ; export CROSS_SIZEOF_SIZE_T
CROSS_SIZEOF_PTRDIFF_T='8' ; export CROSS_SIZEOF_PTRDIFF_T

# System signal values

CROSS_SIGHUP='1' ; export CROSS_SIGHUP
CROSS_SIGINT='2' ; export CROSS_SIGINT
CROSS_SIGQUIT='3' ; export CROSS_SIGQUIT
CROSS_SIGKILL='9' ; export CROSS_SIGKILL
CROSS_SIGTERM='15' ; export CROSS_SIGTERM
CROSS_SIGUSR1='10' ; export CROSS_SIGUSR1

# MIC doesn't have cmpxchg16b instruction! 
CROSS_HAVE_X86_CMPXCHG16B='0'; export CROSS_HAVE_X86_CMPXCHG16B

# Enable Posix shared memory
CROSS_HAVE_SHM_OPEN='1'; export CROSS_HAVE_SHM_OPEN

SRCDIR=`dirname $0`
if test ! -f "$SRCDIR/configure" ; then
  echo "ERROR: The $0 script should be placed in the same directory as the configure script before execution"
  exit 1
fi
# Detect the build host machine type
HOST_ARG=`echo "$@" | grep -e --host`
HOST_APPEND=
if test "$HOST_ARG" = ""; then
  oldCC_FOR_BUILD="$CC_FOR_BUILD"
  oldHOST_CC="$HOST_CC"
  oldCC="$CC"
  CC_FOR_BUILD=
  HOST_CC=
  CC=
  if test "$HOST_ID" = ""; then
    HOST_ID=`$SRCDIR/config-aux/config.guess`
  fi
  if test "$HOST_ID" = ""; then
    echo 'ERROR: failed to auto-detect build host. Please run with --host=machineid to identify the host machine running this script'
    exit 1
  else
    HOST_APPEND="--host=$HOST_ID"
  fi
  CC_FOR_BUILD="$oldCC_FOR_BUILD"
  HOST_CC="$oldHOST_CC"
  CC="$oldCC"
fi
# Now that everything is setup, run the actual configure script
$SRCDIR/configure --enable-cross-compile $HOST_APPEND --build=$TARGET_ID --target=$TARGET_ID --program-prefix='' $EXTRA_CONFIGURE_ARGS "$@"
