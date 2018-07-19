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
#       To build chpldoc, test-venv, etc

set +x -e
if [ -z "$BUILD_CONFIGS_config" ]; then

    # Top-level: this setenv script is being run outside of build_configs.py
    #   to orchestrate multiple build_config.py runs

    setenv=$( basename "$0" )
    cwd=$( cd $(dirname "$0") && pwd )
    source $cwd/functions.bash

    log_info "Begin project $setenv"

    CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../.. && pwd )}
    log_debug "with CHPL_HOME=$CHPL_HOME"
    ck_chpl_home "$CHPL_HOME"

    log_info "Chapel printchplenv, with initial env:"
    $CHPL_HOME/util/printchplenv --all --no-tidy || echo >&2 ignore error

    # Chapel make compiler for this Chapel platform, with users env as modified below

    compiler_targets=   # would be "compiler", except "make mason" will fail in the next step
    compiler_config="--tasks=UNSET --comm=none --launcher=none --auxfs=none --substrate=none --regex=re2"

    log_info "Start build_configs $dry_run $verbose $compiler_targets # (with compiler config)"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.compiler.log" \
        $compiler_config $compiler_targets

    # Chapel make "tools" == chpldoc test-venv mason, same config as compiler

    tools_targets="test-venv chpldoc mason"

    log_info "Start build_configs $dry_run $verbose $tools_targets # (tools with compiler config)"

    $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.tools.log" \
        $compiler_config $tools_targets

    # Chapel make "runtime" for multiple Chapel build_configs (no make target)

    log_info "Start build_configs $dry_run $verbose # (runtime == no make target)"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.runtime.log" \
        --tasks=UNSET --comm=none,gasnet --launcher=UNSET --auxfs=UNSET --substrate=none,udp,UNSET

    # make cleanall, the build config should not matter

    clean_targets=cleanall

    log_info "Start build_configs $dry_run $verbose $clean_targets # (cleanall with compiler config)"

    $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.cleanall.log" \
        $compiler_config $clean_targets

    log_info "End $setenv"
else

    # build_configs-make-level: this setenv script is being run by build_configs.py,
    #   to set the appropriate environment to "make" this Chapel config.
    # This script can also just skip the "make" by doing a bash exit.

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )
    source $cwd/functions.bash

    log_info "Begin callback $setenv"
    log_debug "with config=$BUILD_CONFIGS_config"

    if [ "$CHPL_COMM" == none ]; then
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # exit now because we already built the runtime with COMM=none
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        else

            # COMM=none means no substrate
            log_info "Unset CHPL_COMM_SUBSTRATE for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            unset CHPL_COMM_SUBSTRATE
        fi
    else
        if [ "$CHPL_COMM_SUBSTRATE" == none ]; then

            # exit now because gasnet needs a substrate
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        fi
    fi

    if [ -z "$dry_run" ] ; then
        log_info "Chapel config from printchplenv, after build_configs and setenv:"
        $BUILD_CONFIGS_chplenv_exe --all --no-tidy
    fi

    log_info "End $setenv"
fi
