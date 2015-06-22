#!/usr/bin/env bash

# Gather important arguments provided to PrgEnv C compilers

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
    echo "Usage: $0 [compile|link]"
    echo " <compile> returns key flags provided to PrgEnv C compilers"
    echo " <link> returns key flags provided to PrgEnv C compilers during linking"
    exit 1
    ;;
esac

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

