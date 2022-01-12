#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

if [[ -z "$BATS_LOG_DIRECTORY" ]] ; then
    export BATS_LOG_DIRECTORY=$PWD
fi

if [[ ! -d $BATS_LOG_DIRECTORY ]] ; then
    mkdir -p $BATS_LOG_DIRECTORY
fi

export CONTRIB_BIN=$DIR/../bin

_cancel() {
    echo caught sigint
    kill -s SIGKILL
}

setup() {
    export BATS_TEST_LOGFILE="${BATS_LOG_DIRECTORY}/${BATS_TEST_NAME}.log"
}

teardown() {
    cat ${BATS_TEST_LOGFILE} || true
}

function batch_launcher {
    if [[ $# -lt 2 ]] ; then
        error "not enough arguments"
    fi
    NUM_RANKS=$1
    NUM_RANKS_PER_NODE=$2
    echo "srun -n ${NUM_RANKS} --ntasks-per-node ${NUM_RANKS_PER_NODE}"
}

# necessary to avoid deadlocks during termination/interrupt
trap _cancel SIGINT
