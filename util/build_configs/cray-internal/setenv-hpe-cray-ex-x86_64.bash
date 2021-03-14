#!/usr/bin/env bash

# setenv script to build a standard Chapel HPE Cray EX module for release

set +x -e

if [ -z "$BUILD_CONFIGS_CALLBACK" ]; then

# ==============
# setenv project
# ==============

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )
    source $cwd/../functions.bash

    # cmdline options

    # current list of selected components initially empty
    components=
    valid_components="compiler,runtime,venv,mason,clean"
    default_components="compiler,runtime,venv,mason"
        ## clean is not default; more developer-friendly

    usage() {
        echo "Usage:  $setenv [-v] [-n]"
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
            case ",${OPTARG#[+-]}," in
            ( *,compiler,* | *,runtime,* | *,venv,* | *,mason,* | *,clean,* ) ;;
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

    export CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../../.. && pwd )}
    log_debug "with CHPL_HOME=$CHPL_HOME"
    ck_chpl_home "$CHPL_HOME"

    # if no components were given on cmdline, then use the default list
    if [ -z "$components" ]; then
        components=$default_components
    fi
    log_info "Selected Chapel components: $components"
    log_info "Valid Chapel components:    $valid_components"

    # Default config

##  # If the CHPL_ env variable is NOT defined here (and not overridden
##  # by build_configs.py cmdline arguments), Chapel make will determine
##  # the appropriate default value for the platform / environment it finds.

    export CHPL_HOST_PLATFORM=hpe-cray-ex
    export CHPL_TARGET_PLATFORM=hpe-cray-ex
    export CHPL_REGEXP=re2      # re2 required for mason
    export CHPL_LOCAL_MODEL=flat
    export CHPL_COMM=none
    export CHPL_COMM_SUBSTRATE=none
    export CHPL_TASKS=qthreads
    export CHPL_LAUNCHER=none
    export CHPL_LIBFABRIC=system
    export CHPL_LLVM=bundled       # llvm requires py27 and cmake
    export CHPL_AUX_FILESYS=none

    # We default to CHPL_LIBFABRIC=system for EX.  We need to point to
    # a libfabric install for the builds.  On EX a module will supply
    # this but on XC we have to reference a private build.
    if ! pkg-config --exists libfabric ; then
      private_libfab_dir=/cray/css/users/chapelu/libfabric/install
      export PKG_CONFIG_PATH=${PKG_CONFIG_PATH:+${PKG_CONFIG_PATH}:}${private_libfab_dir}/cray-xc/lib/pkgconfig
    fi

    # As a general rule, more CPUs --> faster make.
    # To use all available CPUs, export CHPL_MAKE_MAX_CPU_COUNT=0 before running this setenv.

    export CHPL_MAKE_MAX_CPU_COUNT=${CHPL_MAKE_MAX_CPU_COUNT:-4}

    # Show the initial/default Chapel build config with printchplenv

    if [ -n "$verbose" ]; then
        log_debug "Chapel printchplenv, with initial env:"
        $CHPL_HOME/util/printchplenv --all --no-tidy --anonymize || echo >&2 ignore error
    fi

    # NOTE: The --target-compiler values used in this setenv project will never be
    #       seen by Chapel make. They will be recognized (and discarded) in the
    #       setenv callback script in the lower section of this file.

    case "$components" in
    ( *compiler* )
        log_info "Building Chapel component: compiler"

        log_info "Start build_configs $dry_run $verbose -- compiler"

        $cwd/../build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.compiler.log" \
            --target-compiler=compiler -- compiler
        ;;
    ( * )
        log_info "NO building Chapel component: compiler"
        ;;
    esac

    case "$components" in
    ( *runtime* )
        log_info "Building Chapel component: runtime"

        compilers=gnu,cray
        comms=none,ofi
        launchers=none,pals,slurm-srun
        substrates=none
        locale_models=flat
        auxfs=none,lustre
        libpics=none,pic

        log_info "Start build_configs $dry_run $verbose # no make target"

        $cwd/../build_configs.py -p $dry_run $verbose -s $cwd/$setenv -l "$project.runtime.log" \
            --target-compiler=$compilers \
            --comm=$comms \
            --launcher=$launchers \
            --substrate=$substrates \
            --locale-model=$locale_models \
            --auxfs=$auxfs \
            --lib-pic=$libpics \
            -- notcompiler

        # NOTE: don't rebuild compiler above (or else problems with switching GCC versions)
        # NOTE: "--target-compiler" values shown above will be discarded by the setenv callback.

        if [ $private_libfab_dir ] ; then
          # Remove references to our private libfabric directory from
          # the list* files in the built runtime library subdirs.  Our
          # private dir won't be present in that environment and we'll
          # be using the system libfabric module anyway.
          log_info "Wipe $private_libfab_dir refs in $CHPL_HOME/lib/.../list-*"
          find $CHPL_HOME/lib/. -type f -name list-\* \
            | grep '\(list-includes-and-defines\|list-libraries\)$' \
            | xargs sed --in-place "s= *[^ ]*${private_libfab_dir}/[^ ]*==g"
        fi
        ;;
    ( * )
        log_info "NO building Chapel component: runtime"
        ;;
    esac

    venv_targets="test-venv chpldoc"
    case ",$components," in
    ( *,venv,* )

        # Build Chapel python-venv tools with the system-installed Python, no matter which version

        log_info "Building Chapel component: venv ($venv_targets)"

        log_debug "Checking the system-installed Python"

        # Chapel python-venv tools requires a working internet connection and either:
        # - modern version of libssl that supports TLS 1.1
        # - local workarounds such as the variables shown in the "venv" callback, below.

        log_info "Start build_configs $dry_run $verbose -- $venv_targets"

        $cwd/../build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.venv.log" \
            --target-compiler=venv -- $venv_targets

        # If we are not using the system-installed python, update
        # the scripts in chpl-venv to use the system-installed python.
        findPy=$($CHPL_HOME/util/config/find-python.sh)
        whichPy=$( which $findPy || : ok )
        case "$whichPy" in
        ( /usr/bin/python* )
            log_info "Found system python $whichPy"
            ;;
        ( * )
            log_info "Found non-system python $whichPy - adjusting paths"
            # Custom Chapel make target to rm files or links named "python3"
            # from the installed bin dir, forcing users of the installed
            # Chapel RPM to get "python3" from the system.
            use_system_python="-C third-party/chpl-venv use-system-python"
            $cwd/../build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.venv_py27-use_system_python.log" \
                --target-compiler=venv -- $use_system_python
            ;;
        esac
        ;;
    ( * )
        log_info "NO building Chapel component: venv"
        ;;
    esac

    case "$components" in
    ( *mason* )
        log_info "Building Chapel component: mason"

        log_info "Start build_configs $dry_run $verbose -- mason"

        $cwd/../build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.mason.log" \
            --target-compiler=gnu -- mason
        ;;
    ( * )
        log_info "NO building Chapel component: mason"
        ;;
    esac

    clean_targets="cleanall"
    case "$components" in
    ( *clean* )
        log_info "Building Chapel component: clean (make $clean_targets)"

        log_info "Start build_configs $dry_run $verbose -- $clean_targets"

        $cwd/../build_configs.py $dry_run $verbose -s $cwd/$setenv -l "$project.clean.log" \
            --target-compiler=gnu -- $clean_targets
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

    setenv=$( basename "${BASH_SOURCE[0]}" )
    cwd=$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )
    source $cwd/../functions.bash

    log_info "Begin callback $setenv"
    log_debug "with config=$BUILD_CONFIGS_CALLBACK"

    # Exit immediately to skip (avoid building) unwanted Chapel configs

    if [ "$CHPL_LIB_PIC" == pic ]; then
      # skip Chapel make for any communication and launcher that are not none
      # because pic support is for python interoperability which
      # only runs on the login node at the moment.
      if [ "$CHPL_COMM" != none ]; then
        log_info "Skip Chapel make for libpic=$CHPL_LIB_PIC, comm=$CHPL_COMM"
        exit 0
      fi
      if [ "$CHPL_LAUNCHER" != none ]; then
        log_info "Skip Chapel make for libpic=$CHPL_LIB_PIC, launcher=$CHPL_LAUNCHER"
        exit 0
      fi
    fi

    if [ "$CHPL_COMM" == ofi ]; then
        if [ "$CHPL_LAUNCHER" == none ]; then

            # skip Chapel make because comm == ofi needs a Chapel launcher
            log_info "Skip Chapel make for comm=$CHPL_COMM, launcher=$CHPL_LAUNCHER"
            exit 0
        fi
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # skip Chapel make because comm == ofi does not take a substrate
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        fi
        log_info "Unset CHPL_COMM_SUBSTRATE for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
        unset CHPL_COMM_SUBSTRATE
    elif [ "$CHPL_COMM" == gasnet ]; then
        if [ "$CHPL_COMM_SUBSTRATE" == none ]; then

            # skip Chapel make because comm == gasnet needs a substrate
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        fi
    elif [ "$CHPL_COMM" == none ]; then
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # skip Chapel make because we already built the runtime with comm=none
            log_info "Skip Chapel make for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
            exit 0
        fi
        log_info "Unset CHPL_COMM_SUBSTRATE for comm=$CHPL_COMM, substrate=$CHPL_COMM_SUBSTRATE"
        unset CHPL_COMM_SUBSTRATE
    else
        log_error "unrecognized CHPL_COMM=$CHPL_COMM"
        exit 2
    fi

# =========================
# setenv callback functions
# =========================

    source $cwd/../module-functions.bash

    # check module system
    ck_module_list

    if [ -n "$verbose" ] ; then
        log_debug "Module list (sorted), after build_configs, before setenv:"
        list_loaded_modules
    fi

    gen_version_gcc=10.2.0
    #[TODO] gen_version_intel=16.0.3.210
    gen_version_cce=10.0.2

    target_cpu_module=craype-x86-rome

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
        local target_version=$gen_version_intel

        # unload any existing PrgEnv
        unload_module_re PrgEnv-

        # load target PrgEnv with compiler version
        load_module $target_prgenv
        load_module_version $target_compiler $target_version
    }

    function load_prgenv_cray() {

        local target_prgenv="PrgEnv-cray"
        local target_compiler="cce"
        local target_version=$gen_version_cce

        # unload any existing PrgEnv
        unload_module_re PrgEnv-

        # load target PrgEnv with compiler version
        load_module $target_prgenv
        load_module_version $target_compiler $target_version
    }

    function load_target_cpu() {

        # legacy
        unload_module perftools-base acml totalview atp cray-libsci xt-libsci
        #[TODO] load_module cray-libsci

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
    ( gnu )
        load_prgenv_gnu
        ;;
    ( intel )
        load_prgenv_intel
        ;;
    ( cray )
        load_prgenv_cray
        ;;
    ( compiler )
        load_prgenv_gnu
        ;;
    ( venv )
        load_prgenv_gnu
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
    # Chapel make will select cray-prgenv-gnu, cray-prgenv-intel, or cray-prgenv-cray,
    # based on which module (PrgEnv-gnu, PrgEnv-intel, or PrgEnv-cray) it finds in the environment

    unset CHPL_TARGET_COMPILER
    load_target_cpu $target_cpu_module

    if [ "$CHPL_AUX_FILESYS" == lustre ]; then
        # on login/compute nodes, lustre requires the devel api to make
        # lustre/lustreapi.h available (it's implicitly available on esl nodes)
        if pkg-config --exists cray-lustre-api-devel; then
            export CHPL_AUXIO_INCLUDE="$CHPL_AUXIO_INCLUDE $( pkg-config --cflags cray-lustre-api-devel )"
            export CHPL_AUXIO_LIBS="$CHPL_AUXIO_LIBS $( pkg-config --libs cray-lustre-api-devel )"
        fi
    fi

    # ---

    if [ -n "$verbose" ] ; then
        log_debug "Module list (sorted), after build_configs and setenv:"
        list_loaded_modules
    fi

    log_info "End callback $setenv"
fi
