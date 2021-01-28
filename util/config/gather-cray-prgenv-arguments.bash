#!/usr/bin/env bash

# Gather important arguments provided to PrgEnv C compilers

# $1 = compile | link
# $2 = CHPL_TARGET_PLATFORM
# $3 = CHPL_COMM
# $4 = CHPL_COMM_SUBSTRATE
# $5 = CHPL_AUXFS

COMPILE=0
LINK=0
case $1 in
  compile)
    COMPILE=1
    ;;
  link)
    LINK=1
    ;;
  *)
    FAIL=1
    ;;
esac
if [[ $FAIL == 1 || -z $2 || -z $3 || -z $4 || -z $5 ]]; then
    echo "Usage: $0 [compile|link] \$CHPL_TARGET_PLATFORM \$CHPL_COMM \$CHPL_COMM_SUBSTRATE \$CHPL_AUXFS"
    echo " <compile> returns key flags provided to PrgEnv C compilers"
    echo " <link> returns key flags provided to PrgEnv C compilers during linking"
    exit 1
fi

#
# We need to ensure that the Programming Environment is set up to use
# the Gnu compiler.  This is complicated by the fact that PrgEnv-gnu
# could be either a meta-module or a collection.
#
if module avail PrgEnv- 2>&1 | grep --quiet PrgEnv- ; then
  # We have PrgEnv-* meta-modules.
  existing_prgenv=$(module -t list 2>&1 | grep PrgEnv-)
  if [ -z "${existing_prgenv}" ]
  then
    # No PrgEnv loaded, load PrgEnv-gnu
    module load PrgEnv-gnu 2>/dev/null
  elif [[ "${existing_prgenv}" != PrgEnv-gnu* ]]
  then
    # Replace current PrgEnv with PrgEnv-gnu
    module unload "${existing_prgenv}" 2>/dev/null
    module load PrgEnv-gnu 2>/dev/null
  fi
elif module savelist 2>&1 | grep --quiet PrgEnv-gnu ; then
  # We have a PrgEnv-gnu collection.
  #
  # The 'restore' will unload our Chapel module, so we have to preserve
  # CHPL_HOME, which we'll need below.
  save_CHPL_HOME=$CHPL_HOME
  module restore PrgEnv-gnu &> /dev/null
  export CHPL_HOME=$save_CHPL_HOME
else
  # No PrgEnv-* meta-modules are available.  We can't tell the compiler
  # what to link.
  exit 0
fi

# Set up the environment to make the proper libraries and include
# files available.
export PE_PKGCONFIG_PRODUCTS="PE_CHAPEL:$PE_PKGCONFIG_PRODUCTS"
export PE_CHAPEL_MODULE_NAME="chapel"
export PE_CHAPEL_PKGCONFIG_LIBS=`$CHPL_HOME/util/config/gather-pe-chapel-pkgconfig-libs.bash "$2" "$3" "$4" "$5"`

# Adding -lhugetlbfs gets the PrgEnv driver to add the appropriate
# linker option for static linking with it. While it's not always
# used with Chapel programs, it is expected to be the common case
# when running on a Cray X*, so just always linking it is acceptable.
# (We don't add it for HPE Cray EX systems, where it's not needed.)
#
# Note that the GCC option -### causes the compiler to not actually
# compile anything but just print out what it would do to stderr.
#
# Note that the first /dev/null (before the stderr redirection) is
# actually an "input file" for the compilation.
#
# -lchpl_lib_token allows the Chapel compiler to know
# where to put additional arguments (they replace that argument).
LHUGETLBFS="-lhugetlbfs"
if [[ "$2" == "hpe-cray-ex" ]] ; then
  LHUGETLBFS=""
fi

# LIBRARY_PATH is only output to stderr but sometimes includes paths
# we need to include with -L so gather that as well.
# The 2> >(command) is a process substitution.
COMMANDS=`cc -craype-verbose -### $LHUGETLBFS -lchpl_lib_token /dev/null 2> >(grep LIBRARY_PATH)`

for arg in $COMMANDS
do
  if [[ $arg == LIBRARY_PATH* && $LINK == 1 ]]
  then
    # remove LIBRARY_PATH=
    arg=${arg#LIBRARY_PATH=}
    # convert : to spaces
    arg=${arg//:/ }
    for subarg in $arg
    do
      echo -L$subarg
    done
  elif [[ $arg == -I* && $COMPILE == 1 ]]
  then
    echo $arg
  elif [[ $arg == -D* && $COMPILE == 1 ]]
  then
    echo $arg
  elif [[ $arg == -Wl* && $LINK == 1 ]]
  then
    echo $arg
  elif [[ $arg == -L* && $LINK == 1 ]]
  then
    echo $arg
  elif [[ $arg == -l* && $LINK == 1 ]]
  then
    echo $arg
  fi
done

