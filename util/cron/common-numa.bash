#!/usr/bin/env bash
#
# Configure environment for numa testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

source $(cd $(dirname $0) ; pwd)/common-qthreads.bash

export CHPL_LOCALE_MODEL=numa
