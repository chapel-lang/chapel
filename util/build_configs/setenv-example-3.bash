#!/usr/bin/env bash

# Example setenv script to build a customized Chapel Cray module
# as envisioned in GitHub #9551:
#   with and without CHPL_COMM=ugni;
#   with and without CHPL_LAUNCHER=slurm-srun;
#   all built with two CHPL_TARGET_COMPILERs:
#     the Cray Programming Environment for GNU (gcc), and
#     the Cray Programming Environment for Intel compiler

set +x -e

if [ -z "$BUILD_CONFIGS_CALLBACK" ]; then

# ==============
# setenv project
# ==============

    setenv=$( basename "$0" )
    cwd=$( cd $(dirname "$0") && pwd )
    source $cwd/functions.bash

    # cmdline options

    usage() {
        echo "Usage:  $setenv [-v] [-n]"
        echo "  where"
        echo "    -v : verbose/debug output"
        echo "    -n : dry-run: show make commands but do not actually run them"
        exit 1
    }
    while getopts :vnh opt; do
        case $opt in
        ( v ) verbose=-v ;;
        ( n ) dry_run=-n ;;
        ( h ) usage;;
        ( \?) log_error "Invalid option: -$OPTARG"; usage;;
        ( : ) log_error "Option -$OPTARG requires an argument."; usage;;
        esac
    done

    log_info "Begin project $setenv"

    export CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../.. && pwd )}
    log_debug "with CHPL_HOME=$CHPL_HOME"
    ck_chpl_home "$CHPL_HOME"

    # Default config

##  # If the CHPL_ env variable is NOT defined here (and not overridden
##  # by build_configs.py cmdline arguments), Chapel make will determine
##  # the appropriate default value for the platform / environment it finds.

    export CHPL_HOST_PLATFORM=cray-xc
    export CHPL_TARGET_PLATFORM=cray-xc
    export CHPL_REGEXP=re2      # re2 required for mason
    export CHPL_LOCAL_MODEL=flat
    export CHPL_COMM=none
    export CHPL_COMM_SUBSTRATE=none
    export CHPL_TASKS=qthreads
    export CHPL_LAUNCHER=none
    export CHPL_LLVM=none       # llvm requires py27 and cmake
    export CHPL_AUX_FILESYS=none

    # More CPUs --> faster make. Or, unset CHPL_MAKE_MAX_CPU_COUNT to use all CPUs.

    export CHPL_MAKE_MAX_CPU_COUNT=${CHPL_MAKE_MAX_CPU_COUNT:-4}

    # Show the initial/default Chapel build config with printchplenv

    if [ -n "$verbose" ]; then
        log_debug "Chapel printchplenv, with initial env:"
        $CHPL_HOME/util/printchplenv --all --no-tidy || echo >&2 ignore error
    fi

    # NOTE: The --target-compiler values used in this setenv project will never be
    #       seen by Chapel make. They will be recognized (and discarded) in the
    #       setenv callback script in the lower section of this file.


    # Chapel compiler

    log_info "Start build_configs $dry_run $verbose compiler"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.compiler.log" \
        --target-compiler=compiler compiler


    # Chapel runtimes

    compilers="gnu,intel"
    comms="none,ugni"
    launchers="none,slurm-srun"

    log_info "Start build_configs $dry_run $verbose # no make target"

    $cwd/build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.runtime.log" \
        --target-compiler=$compilers --comm=$comms --launcher=$launchers

        # NOTE: "--target-compiler" values shown above will be discarded by the setenv callback.

        # The following 2x2x2 matrix of Chapel runtime configurations are defined by the
        # arguments passed to build_configs.py, above.

        # CHPL_TARGET_COMPILER  CHPL_COMM   CHPL_LAUNCHER   Make Chapel?
        # --------------------  ---------   -------------   -----------
        # cray-prgenv-gnu       none        none            Yes
        # cray-prgenv-gnu       none        slurm-srun      Yes
        # cray-prgenv-gnu       ugni        none            No (skip)
        # cray-prgenv-gnu       ugni        slurm-srun      Yes
        # cray-prgenv-intel     none        none            Yes
        # cray-prgenv-intel     none        slurm-srun      Yes
        # cray-prgenv-intel     ugni        none            No (skip)
        # cray-prgenv-intel     ugni        slurm-srun      Yes

        # Only six of the eight possible runtime configurations will be built.
        # The other two will be skipped by an "exit 0" in the setenv callback script
        # found in the lower part of this file.


    # Chapel tools (ie, mason, chpldoc, test-venv)

##  # Building Chapel tools requires a working internet connection and either:
##  # - modern version of libssl that supports TLS 1.1
##  # - local workarounds such as the variables shown in the "tools" callback, below.
##  #
##  # Commented out here to make this setenv example more portable.
##
##  tools_targets="test-venv chpldoc mason"
##
##  log_info "Start build_configs $dry_run $verbose $tools_targets"
##
##  $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.tools.log" \
##      --target-compiler=tools $tools_targets


    # make clean

    clean_targets="cleanall"

    log_info "Start build_configs $dry_run $verbose $clean_targets"

    $cwd/build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.clean.log" \
        --target-compiler=compiler $clean_targets

    log_info "End $setenv"
else

# ===============
# setenv callback
# ===============

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )
    source $cwd/functions.bash

    log_info "Begin callback $setenv"
    log_debug "with config=$BUILD_CONFIGS_CALLBACK"

# =========================
# setenv callback functions
# =========================

    source $cwd/module-functions.bash

    # check module system
    ck_module_list

    if [ -n "$verbose" ] ; then
        log_debug "Module list (sorted), after build_configs, before setenv:"
        list_loaded_modules
    fi

    gen_version_gcc=6.1.0
 ## gen_version_intel=18.0.3.222    # For portability, this example just accepts the default version.
    target_cpu_module=craype-sandybridge

    function load_prgenv_gnu() {

        local target_prgenv="PrgEnv-gnu"
        local target_compiler="gcc"
        local target_version=$gen_version_gcc

        # unload any existing PrgEnv
        unload_module_re PrgEnv-

        # load target PrgEnv with compiler version
        load_module $target_prgenv
        load_module_version $target_compiler $target_version
    }

    function load_prgenv_intel() {

        local target_prgenv="PrgEnv-intel"
        local target_compiler="intel"
     ## local target_version=$gen_version_intel     # Just accept the default version

        # unload any existing PrgEnv
        unload_module_re PrgEnv-

        # load target PrgEnv with compiler version
        load_module $target_prgenv
     ## load_module_version $target_compiler $target_version
    }

    function load_target_cpu() {

        # legacy
        unload_module perftools-base acml totalview atp cray-libsci xt-libsci
        load_module cray-libsci

        case "$1" in ( "" ) log_error "load_target_cpu missing arg 1"; exit 2;; esac
        local target=$1

        # target CPU: unload existing target-cpu modules, if any, then load our target-cpu
        unload_module_re -v -network- craype-
        load_module $target
    }

    # ---

    # NOTE: The CHPL_TARGET_COMPILER env values from build_configs are not passed to Chapel.
    # Instead, they drive module load commands in the setenv callback

    case "$CHPL_TARGET_COMPILER" in
    ( compiler)
        load_prgenv_gnu
        ;;
    ( tools )
        load_prgenv_gnu

    ##  # If the installed libssl is too old to support TLS 1.1, some workarounds exist to
    ##  # enable building Chapel tools anyway.
    ##  # For example, the following gives a URL to a local PyPI mirror that accepts http,
    ##  # and the location of a pre-installed "pip" on the host machine.
    ##  #
    ##  # Commented out here to make this setenv example more portable.
    ##
    ##  export CHPL_EASY_INSTALL_PARAMS="-i http://mirror-pypi.example.com/pypi/simple"
    ##  export CHPL_PIP_INSTALL_PARAMS="-i http://mirror-pypi.example.com/pypi/simple --trusted-host mirror-pypi.example.com"
    ##  export CHPL_PIP=/data/example/opt/lib/pip/__main__.py
    ##  export CHPL_PYTHONPATH=/data/example/opt/lib
        ;;
    ( gnu )
        load_prgenv_gnu
        ;;
    ( intel )
        load_prgenv_intel
        ;;
    ( "" )
        : ok
        ;;
    ( * )
        log_error "unrecognized CHPL_TARGET_COMPILER=$CHPL_TARGET_COMPILER"
        exit 2
        ;;
    esac

    # Discard the artificial CHPL_TARGET_COMPILER value.
    # Chapel make will select cray-prgenv-gnu or cray-prgenv-intel by itself.

    unset CHPL_TARGET_COMPILER
    load_target_cpu $target_cpu_module

    # ---

    if [ "$CHPL_COMM" == ugni ]; then
        if [ "$CHPL_LAUNCHER" == none ]; then

            # skip Chapel make because comm == ugni needs a Chapel launcher
            log_info "Skip Chapel make for comm=$CHPL_COMM, launcher=$CHPL_LAUNCHER"
            exit 0
        fi
        unset CHPL_COMM_SUBSTRATE
    fi

    # ---

    if [ -n "$verbose" ] ; then
        log_debug "Module list (sorted), after build_configs and setenv:"
        list_loaded_modules
    fi

    log_info "End callback $setenv"
fi
