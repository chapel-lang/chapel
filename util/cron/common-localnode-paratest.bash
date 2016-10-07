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
# be set with `${1}`. Limits testing to only run 1 executable at a time unless
# the "no_limit_exec" variant is used. Returns the args for using the nodefile
# with nightly script (`-parnodefile <abs_nodefile>`)
#

function get_nightly_no_limit_exec_paratest_args
{
    nodepara=${1:-2}
    parnodefile="${CHPL_HOME}/test/Nodes/${nodepara}-localhost"
    gen_nodefile ${nodepara} ${parnodefile}
    echo "-parnodefile ${parnodefile}"
}

function get_nightly_paratest_args
{
    get_nightly_no_limit_exec_paratest_args ${@}
    export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes
}
