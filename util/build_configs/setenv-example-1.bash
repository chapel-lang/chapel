#!/usr/bin/env bash

# Example setenv script that illustrates a possible solution to
#   https://github.com/chapel-lang/chapel/issues/9943,
# using build_configs.py with a custom setenv script (ie, this file).

# This setenv builds a trivial array of Chapel configurations:
# with and without Gasnet (Gasnet with two comm substrates).

# This example uses a very simple style of bash scripting.
# It is meant as a setenv tutorial only.

# How to use this example:
#
#   git clone ssh://git@github.com/chapel-lang/chapel.git
#   cd chapel/util/build_configs
#
#   # build Chapel with and without gasnet
#   ./setenv-example-1.bash
#
#   # run a test with and without gasnet (optional)
#   source ../setchplenv.bash
#
#   export CHPL_COMM=gasnet
#   export CHPL_COMM_SUBSTRATE=smp
#   chpl -o hello6 $CHPL_HOME/examples/hello6*.chpl
#   ./hello6 -nl 2
#
#   export CHPL_COMM=gasnet
#   export CHPL_COMM_SUBSTRATE=udp
#   export GASNET_SPAWNFN=L
#   chpl -o hello6 $CHPL_HOME/examples/hello6*.chpl
#   ./hello6 -nl 2
#
#   export CHPL_COMM=none
#   chpl -o hello6 $CHPL_HOME/examples/hello6*.chpl
#   ./hello6

if [ -z "$BUILD_CONFIGS_CALLBACK" ]; then

    # This is a top-level setenv "project":
    #
    #   The user runs this script, and this script runs build_config.py,
    #   as many times as needed, passing the desired Chapel build config
    #   parameters and make targets each time. It can also make changes
    #   to the default environment seen by build_config.py.

    # CHPL_HOME from the environment, or default to this Chapel workspace

    export CHPL_HOME=${CHPL_HOME:-$( cd $( dirname "${BASH_SOURCE[0]}" )/../.. && pwd )}

    # Default Chapel build config values may be defined here

    export CHPL_RE2=bundled # required by mason

    substrates=smp,udp      # two widely-supported gasnet substrates for this example


    # Use build_configs.py to make Chapel with and without gasnet

    ./build_configs.py -p $dry_run $verbose -s setenv-example-1.bash -l setenv-example-1.make.log \
        --comm=none,gasnet --substrate=none,$substrates  # no make target

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


    # We can use build_configs.py to make mason, though we need only one Chapel config.

    ./build_configs.py -s setenv-example-1.bash -l setenv-example-1.mason.log \
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
