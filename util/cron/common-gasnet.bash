#!/usr/bin/env bash
#
# Configure environment for GASNet testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common.bash

export CHPL_COMM=gasnet
export GASNET_SPAWNFN=L
export GASNET_ROUTE_OUTPUT=0
export CHPL_GASNET_CFG_OPTIONS=--disable-ibv
