#!/usr/bin/env bash
#
# Run the compiler with CHPL_HWLOC=<opposite of what is set>, which should
# generate the runtime error about configuration not having been built.

test_executable=$1
output=$2
compiler=$3

current_hwloc=$($CHPL_HOME/util/chplenv/chpl_hwloc.py)

case $current_hwloc in
    bundled|system)
        export CHPL_HWLOC=none
        ;;
    none)
        export CHPL_HWLOC=bundled
        ;;
    *)
        echo "[Warning: Did not recognize HWLOC value: ${current_hwloc}]"
esac

#main_o=$($CHPL_HOME/util/config/compileline --main.o)

# /Users/tvandoren/src/chapel/runtime/etc/Makefile.include:83: *** The runtime has not been built for this configuration. Check $CHPL_HOME/util/printchplenv and try (re)building runtime.  Stop.
$compiler "${test_executable}.chpl" 2>&1 | \
    grep '^error\|runtime/etc/Makefile.include' | \
    grep -v 'Expected runtime library' | \
    grep -v 'compiling generated source' | \
    sed "s:${CHPL_HOME}:\$CHPL_HOME:g" | \
    sed "s:'.*make':\$CHPL_MAKE:g" | \
    sed "s/:[0-9]*:/:nnnnn:/g" | \
    sed 's^\$CHPL_HOME/runtime/etc/Makefile.include:nnnnn: \*\*\* ^^g' | \
    sed 's/  Stop.//g' | \
    sed 's/error: //g' \
    > $output
