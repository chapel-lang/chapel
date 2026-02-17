#!/usr/bin/env bash

set -ex

CRAY_INTERNAL_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export target_cpu_module=craype-x86-rome

$CRAY_INTERNAL_DIR/setenv-hpe-cray-ex.bash "$@"
