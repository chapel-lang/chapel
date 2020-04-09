#!/usr/bin/env bash
#
# Helpers to create "localhost" paratest file to use with nightly -parnodefile

# Generate a "localhost" paratest nodefile
function gen_nodefile
{
    nodepara=${1}
    nodefile=${2}

    for _ in $(seq ${nodepara}); do
        echo "localhost"
    done > "${nodefile}"
}


#
# Generate a "localhost" paratest nodefile. Defaults to `nodepara 2`, but can
# be set with `${1}`. Limits testing to only run 1 executable at a time, unset
# CHPL_TEST_LIMIT_RUNNING_EXECUTABLES to prevent this. Returns the args for
# using the nodefile with nightly script (`-parnodefile <abs_nodefile>`)
#

export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes

function get_nightly_paratest_args
{
    nodepara=${1:-2}
    parnodefile="${CHPL_HOME}/test/Nodes/${nodepara}-localhost"
    gen_nodefile ${nodepara} ${parnodefile}
    echo "-parnodefile ${parnodefile}"
}
