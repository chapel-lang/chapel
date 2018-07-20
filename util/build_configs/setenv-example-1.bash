#!/usr/bin/env bash

# Example setenv script

# Demonstrates:
#   Top-level setenv script usage:
#       Calls build_configs.py from within the setenv script, to
#       pass appropriate Chapel build config parameters to build_configs.py.
#   Make-level setenv script usage:
#       The same setenv script can source'd before each Chapel make, to
#       make the appropriate environment changes for each build config.
#   Sparse build configs matrix, arbitrary Chapel env changes:
#       If COMM=gasnet, then we can skip all but one COMM_SUBSTRATE value
#       CHPL_COMM==gasnet (or not) affects CHPL_COMM_SUBSTRATE, for example
#   Multiple make targets:
#       To build chpldocs, test-venv, etc

env | sort | grep -e '^CHPL_' -e '^BUILD_CONFIGS_'

if [ -z "$BUILD_CONFIGS_config" ]; then

    # Top-level: this is being run outside of build_configs.py, to
    #   orchestrate multiple build_config.py runs

    set -e
    this=$( basename "$0" )
    cwd=$( cd $(dirname "$0") && pwd )

    # compiler make for this Chapel platform and users current env (as modified by parameters below)
    $cwd/build_configs.py -s $cwd/$this -l "$this.compiler.log" \
        --tasks=UNSET --comm=none --launcher=none --auxfs=none --substrate=none \
            compiler

    # chpldocs and test-env only

    $cwd/build_configs.py -s $cwd/$this -l "$this.venv.log" \
        --tasks=UNSET --comm=none --launcher=none --auxfs=none --substrate=none \
            chpldoc test-venv

    # runtimes
    $cwd/build_configs.py -s $cwd/$this -l "$this.runtime.log" \
        --tasks=UNSET --comm=none,gasnet --launcher=UNSET --auxfs=UNSET --substrate=none,udp,UNSET

else
    # make-level: this is being run from build_configs.py, to make the
    #   appropriate environment changes before running "make" for this
    #   Chapel build config. It also can skip a make.

    set +e -x
    if [ "$CHPL_COMM" == none ]; then
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # exit now because we already built the runtime with COMM=none
            exit 0
        else

            # COMM=none means no substrate
            unset CHPL_COMM_SUBSTRATE
        fi
    else
        if [ "$CHPL_COMM_SUBSTRATE" == none ]; then

            # exit now because gasnet needs a substrate
            exit 0
        fi
        
    fi

    # show the Chapel env
    $BUILD_CONFIGS_chplenv_exe --all --no-tidy
fi
