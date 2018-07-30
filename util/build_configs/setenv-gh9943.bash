#!/usr/bin/env bash

# Example setenv script that illustrates a solution to
#   https://github.com/chapel-lang/chapel/issues/9943,
# using build_configs.py with a custom setenv script (ie, this file).

# How to use this example:
#
#   git clone ssh://git@github.com/chapel-lang/chapel.git
#   cd chapel/util/build_configs
#
#   # build Chapel with and without gasnet
#   ./setenv-gh9943.bash
#
#   # run a test with and without gasnet (optional)
#   source ../setchplenv.bash
#
#   export CHPL_COMM=gasnet
#   export CHPL_COMM_SUBSTRATE=smp
#   chpl -o hello6 $CHPL_HOME/test/release/examples/hello6*.chpl
#   ./hello6 -nl 2
#
#   export CHPL_COMM=gasnet
#   export CHPL_COMM_SUBSTRATE=udp
#   export GASNET_SPAWNFN=L
#   chpl -o hello6 $CHPL_HOME/test/release/examples/hello6*.chpl
#   ./hello6 -nl 2
#
#   export CHPL_COMM=none
#   chpl -o hello6 $CHPL_HOME/test/release/examples/hello6*.chpl
#   ./hello6

if [ -z "$BUILD_CONFIGS_config" ]; then

    # This is a top-level setenv "project":
    #
    #   The user runs this script, and this script runs build_config.py,
    #   as many times as needed, passing the desired Chapel build config
    #   parameters and make targets each time. It can also make changes
    #   to the default environment seen by build_config.py.

    # CHPL_HOME from the environment, or default to this Chapel workspace

    CHPL_HOME=${CHPL_HOME:-$( cd $cwd/../.. && pwd )}

    # Default Chapel build config values may be defined here

    export CHPL_REGEXP=re2  # required by mason

    substrates=smp,udp      # two widely-supported gasnet substrates for this example

    # Use build_configs.py to make Chapel with and without gasnet

    ./build_configs.py -p $dry_run $verbose -s setenv-gh9943.bash -l setenv-gh9943.make.log \
        --comm=none,gasnet --substrate=none,$substrates  # no make target

    # We can use build_configs.py to make mason, though we need only one Chapel config.

    ./build_configs.py -s setenv-gh9943.bash -l setenv-gh9943.mason.log \
        --comm=none --substrate=UNSET mason

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

    if [ "$CHPL_COMM" == none ]; then
        if [ "$CHPL_COMM_SUBSTRATE" != none ]; then

            # skip Chapel make because we already built the runtime with COMM=none

            exit 0
        else

            # COMM==none means no substrate, so unset the env variable

            unset CHPL_COMM_SUBSTRATE
        fi
    else
        if [ "$CHPL_COMM_SUBSTRATE" == none ]; then

            # skip Chapel make because gasnet needs a substrate

            exit 0
        fi
    fi
fi
