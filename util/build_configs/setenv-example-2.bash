#!/usr/bin/env bash

# Example setenv script to illustrate a more elaborate, production-grade
# build script, as in a Jenkins CI job.

# This file builds the same trivial Chapel configuration as setenv-example-1,
# with and without Gasnet (Gasnet with two different comm substrates).

# However, this example uses a more elaborate bash style that provides more
# features.  This file also skips the tutorial-style bash comments used in
# setenv-example-1, yet includes comments on new features introduced here.

set +x -e   # exit if any errors

if [ -z "$BUILD_CONFIGS_CALLBACK" ]; then

# ==============
# setenv project
# ==============

    # setenv, cwd == name and directory location of this script file

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )

    # functions.bash provides local bash functions like log_info, and
    #   sets shell variable project from the filename of this setenv file

    source $cwd/functions.bash

    # cmdline options

    # current list of selected components initially empty
    components=
    valid_components="compiler,tools,runtime,clean"
    default_components="compiler,tools,runtime"
        ## clean is not default; more developer-friendly

    usage() {
        echo "Usage:  $setenv [-v] [-n] [-B COMPONENT [-B COMPONENT] ...]"
        echo "  where"
        echo "    -v : verbose/debug output"
        echo "    -n : dry-run: show make commands but do not actually run them"
        echo "    -B COMPONENT : Chapel component to build (optional, repeatable)"
        echo "               Valid components: '$valid_components'"
        echo "          Default if -B omitted: '$default_components'"
        echo "          Selected components are always built in the order shown above."
        echo "          COMPONENT may be prefixed with a plus sign or a minus sign."
        echo "          For example, if '-B +clean' is given (as the only -B parameter),"
        echo "          then the default list of components will be built, followed by"
        echo "          'make cleanall'. If '-B clean' was given instead, then"
        echo "          'make cleanall' would be the ONLY thing done by this script."
        exit 1
    }
    while getopts :vnB:h opt; do
        case $opt in
        ( v ) verbose=-v ;;
        ( n ) dry_run=-n ;;
        ( B )
            # ck valid component
            case "$OPTARG" in
            ( *compiler* | *tools* | *runtime* | *clean* ) ;;
            ( * )   log_error "Invalid -B COMPONENT='$OPTARG'"; usage ;;
            esac
            # if + or - prefix, modify the current list of selected components
            case "$OPTARG" in
            ( +* )  components="${components:-$default_components}" ;;
            ( -* )  components= ;;
            esac
            # add option to current list of selected components
            components="${components:+$components,}$OPTARG"
            ;;
        ( h ) usage;;
        ( \?) log_error "Invalid option: -$OPTARG"; usage;;
        ( : ) log_error "Option -$OPTARG requires an argument."; usage;;
        esac
    done

    log_info "Begin project $setenv"

    # CHPL_HOME from the environment, or default to this Chapel workspace

    export CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../.. && pwd )}
    log_debug "with CHPL_HOME=$CHPL_HOME"
    ck_chpl_home "$CHPL_HOME"

    # if no components were given on cmdline, then use the default list
    if [ -z "$components" ]; then
        components=$default_components
    fi
    log_info "Selected Chapel components: $components"
    log_info "Valid Chapel components:    $valid_components"

    # Default Chapel build config values may be defined here

    export CHPL_RE2=bundled  # to support mason

    substrates=smp,udp  # desired gasnet substrate(s).

    # Show the initial/default Chapel build config with printchplenv

    log_info "Chapel printchplenv, with initial env:"
    $CHPL_HOME/util/printchplenv --all --no-tidy --anonymize || echo >&2 ignore error

    # Use build_configs.py to make Chapel compiler

    compiler_targets=  # would be "compiler", except "make mason" may fail in the next step
    compiler_config="--tasks=UNSET --comm=none --launcher=none --auxfs=none --substrate=none"
    case "$components" in
    ( *compiler* )
        log_info "Building Chapel component: compiler"

        log_info "Start build_configs $dry_run $verbose $compiler_targets # (with compiler config)"

        $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.compiler.log" \
            $compiler_config $compiler_targets
        ;;
    ( * )
        log_info "NO building Chapel component: compiler"
        ;;
    esac

    # Use build_configs.py to make Chapel "tools" == chpldoc test-venv mason, same config as compiler

    tools_targets="test-venv chpldoc mason"
    case "$components" in
    ( *tools* )
        log_info "Building Chapel component: tools ($tools_targets)"

        log_info "Start build_configs $dry_run $verbose $tools_targets # (tools with compiler config)"

        $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.tools.log" \
            $compiler_config $tools_targets
        ;;
    ( * )
        log_info "NO building Chapel component: tools"
        ;;
    esac

    # Use build_configs.py to make Chapel "runtime" for multiple Chapel build_configs

    case "$components" in
    ( *runtime* )
        log_info "Building Chapel component: runtime"

        log_info "Start build_configs $dry_run $verbose # (runtime == no make target)"

        $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.runtime.log" \
            --tasks=UNSET --auxfs=UNSET --comm=none,gasnet --launcher=UNSET --substrate=none,$substrates

        # The following 2x3 matrix of Chapel runtime configurations are defined by the
        # "--comm" and "--substrate" arguments passed to build_configs.py, above.
        #
        # CHPL_COMM   CHPL_COMM_SUBSTRATE   Make Chapel?
        # ---------   -------------------   ----------------
        # none        none                  Yes
        # none        mpi                   No  (skip)
        # none        udp                   No  (skip)
        # gasnet      none                  No  (skip)
        # gasnet      mpi                   Yes
        # gasnet      udp                   Yes
        #
        # Only three of the six possible configurations will be built.
        # The rest will be skipped by an "exit 0" in the setenv callback script found
        # in the lower part of this file.
        ;;
    ( * )
        log_info "NO building Chapel component: runtime"
        ;;
    esac

    # Use build_configs.py to make cleanall, the build config should not matter

    clean_targets=cleanall
    case "$components" in
    ( *clean* )
        log_info "Building Chapel component: clean ($clean_targets)"

        log_info "Start build_configs $dry_run $verbose $clean_targets # (cleanall with compiler config)"

        $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.cleanall.log" \
            $compiler_config $clean_targets
        ;;
    ( * )
        log_info "NO building Chapel component: clean"
        ;;
    esac

    log_info "End $setenv"
else

# ===============
# setenv callback
# ===============

    # setenv, cwd == name and directory location of this script file

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )

    # functions.bash provides local bash functions like log_info, and sets
    #   shell variables verbose and dry_run from cmdline options -v -n

    source $cwd/functions.bash

    log_info "Begin callback $setenv"
    log_debug "with config=$BUILD_CONFIGS_CALLBACK"

    if [ "$CHPL_COMM" == none ]; then
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # skip Chapel make because we already built the runtime with COMM=none
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        else

            # COMM=none means no substrate
            log_info "Unset CHPL_COMM_SUBSTRATE for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            unset CHPL_COMM_SUBSTRATE
        fi
    else
        if [ "$CHPL_COMM_SUBSTRATE" == none ]; then

            # skip Chapel make because gasnet needs a substrate
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        fi
    fi

    if [ -z "$dry_run" ] ; then
        log_info "Chapel config from printchplenv, after build_configs and setenv:"
        $BUILD_CONFIGS_CHPLENV_EXE --all --no-tidy
    fi

    log_info "End callback $setenv"
fi
