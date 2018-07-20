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

set +x -e   # exit if any errors

if [ -z "$BUILD_CONFIGS_config" ]; then

# This is a top-level setenv "project":
#
#   The user runs this script, and this script runs build_config.py,
#   as many times as needed, passing the desired Chapel build config
#   parameters and make targets each time. It can also make changes
#   to the default environment seen by build_config.py.

    # setenv, cwd == name and directory location of this script file

    setenv=$( basename "$0" )
    cwd=$( cd $(dirname "$0") && pwd )

    # functions.bash provides local bash functions like log_info, and
    #   sets shell variable project from the filename of this setenv file

    source $cwd/functions.bash

    # cmdline options

    usage() {
        echo "Usage:  $setenv [-v] [-n]"
        echo "  where"
        echo "    -v : verbose/debug output"
        echo "    -n : dry-run: show make commands but do not actually run them"
        exit 1
    }
    while getopts :vn opt; do
        case $opt in
        ( v ) verbose=-v ;;
        ( n ) dry_run=-n ;;
        ( \?) log_error "Invalid option: -$OPTARG"; usage;;
        ( : ) log_error "Option -$OPTARG requires an argument."; usage;;
        esac
    done

    log_info "Begin project $setenv"

    # CHPL_HOME from the environment, or default to this Chapel workspace

    CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../.. && pwd )}
    log_debug "with CHPL_HOME=$CHPL_HOME"
    ck_chpl_home "$CHPL_HOME"

    export CHPL_REGEXP=re2  # to support mason

    # Show the initial/default Chapel build config with printchplenv

    log_info "Chapel printchplenv, with initial env:"
    $CHPL_HOME/util/printchplenv --all --no-tidy || echo >&2 ignore error

    # Use build_configs.py to make Chapel compiler

    compiler_targets=  # would be "compiler", except "make mason" may fail in the next step
    compiler_config="--tasks=UNSET --comm=none --launcher=none --auxfs=none --substrate=none --regex=re2"

    log_info "Start build_configs $dry_run $verbose $compiler_targets # (with compiler config)"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.compiler.log" \
        $compiler_config $compiler_targets

    # Use build_configs.py to make Chapel "tools" == chpldoc test-venv mason, same config as compiler

    tools_targets="test-venv chpldoc mason"

    log_info "Start build_configs $dry_run $verbose $tools_targets # (tools with compiler config)"

    $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.tools.log" \
        $compiler_config $tools_targets

    # Use build_configs.py to make Chapel "runtime" for multiple Chapel build_configs

    log_info "Start build_configs $dry_run $verbose # (runtime == no make target)"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.runtime.log" \
        --tasks=UNSET --comm=none,gasnet --launcher=UNSET --auxfs=UNSET --substrate=none,udp,UNSET

    # Use build_configs.py to make cleanall, the build config should not matter

    clean_targets=cleanall

    log_info "Start build_configs $dry_run $verbose $clean_targets # (cleanall with compiler config)"

    $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.cleanall.log" \
        $compiler_config $clean_targets

    log_info "End $setenv"
else

# This is a setenv "callback":
#
#   This same setenv script may be source'd by build_config.py, once for each
#   Chapel build config, to customize the host environment seen by each Chapel make.
#
#   build_config.py defines CHPL_HOME and other CHPL_XXXX env values for each Chapel
#   build config, then source's this script, and finally runs Chapel make.
#
#   This script can make arbitrary changes to the environment seen by Chapel make.
#   It can also skip the Chapel make command entirely, by doing a shell exit.

    # setenv, cwd == name and directory location of this script file

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )

    # functions.bash provides local bash functions like log_info, and sets
    #   shell variables verbose and dry_run from cmdline options -v -n

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
