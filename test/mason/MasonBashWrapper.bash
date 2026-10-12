#!/usr/bin/env bash
source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/bash_helpers.bash
bash -eo pipefail "$@"
