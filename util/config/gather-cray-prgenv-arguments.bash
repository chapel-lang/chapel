#!/usr/bin/env bash

# Gather important arguments provided to PrgEnv C compilers

# $1 = compile | link
# $2 = CHPL_COMM
# $3 = CHPL_COMM_SUBSTRATE
# $4 = CHPL_AUXFS

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
if [[ $FAIL == 1 || -z $2 || -z $3 || -z $4 ]]; then
    echo "Usage: $0 [compile|link] \$CHPL_COMM \$CHPL_COMM_SUBSTRATE \$CHPL_AUXFS"
    echo " <compile> returns key flags provided to PrgEnv C compilers"
    echo " <link> returns key flags provided to PrgEnv C compilers during linking"
    exit 1
fi

# Try loading PrgEnv-gnu if no module is loaded
module list --terse 2>&1 | grep -q PrgEnv
if [ $? != 0 ]
then
  module load PrgEnv-gnu
fi

# Set up the environment to make the proper libraries and include
# files available.
export PE_PKGCONFIG_PRODUCTS="PE_CHAPEL:$PE_PKGCONFIG_PRODUCTS"
export PE_CHAPEL_MODULE_NAME="chapel"
export PE_CHAPEL_PKGCONFIG_LIBS=`$CHPL_HOME/util/config/gather-pe-chapel-pkgconfig-libs.bash "$2" "$3" "$4"`

COMMANDS=`cc -craype-verbose 2>/dev/null`

  for arg in $COMMANDS
  do
    if [[ $arg == -I* && $COMPILE == 1 ]]
    then
      echo $arg
    elif [[ $arg == -D* && $COMPILE == 1 ]]
    then
      echo $arg
    elif [[ $arg == -W* && $arg != -Wl,--as-needed,* && $LINK == 1 ]]
    then
        # don't echo -Wl these aren't found.
      echo $arg
    elif [[ $arg == -L* && $LINK == 1 ]]
    then
      echo $arg
    elif [[ $arg == -l* && $LINK == 1 ]]
    then
      echo $arg
    fi
  done

