#!/usr/bin/env bash
#
# Run the compiler with CHPL_GMP=<opposite of what is set>, which should
# generate the runtime error about configuration not having been built.

test_executable=$1
output=$2
compiler=$3

current_gmp=$($CHPL_HOME/util/chplenv/chpl_gmp.py)

case $current_gmp in
    gmp|system)
        export CHPL_GMP=none
        ;;
    none)
        export CHPL_GMP=gmp
        ;;
    *)
        echo "[Warning: Did not recognize GMP value: ${current_gmp}]"
esac

main_o=$($CHPL_HOME/util/config/compileline --main.o)
rt_config=$(basename $(dirname $main_o))

# /Users/tvandoren/src/chapel/runtime/etc/Makefile.include:83: *** The runtime has not been built for this configuration. Check $CHPL_HOME/util/printchplenv and try (re)building runtime.  Stop.
$compiler "${test_executable}.chpl" 2>&1 | \
    grep 'runtime/etc/Makefile.include' | \
    sed "s:${CHPL_HOME}:\$CHPL_HOME:g" | \
    sed "s:${rt_config}:<runtime_config>:g" | \
    sed "s:'.*make':\$CHPL_MAKE:g" | \
    sed "s/:[0-9]*:/:nnnnn:/g" \
    > $output
