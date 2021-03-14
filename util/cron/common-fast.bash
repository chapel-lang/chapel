#!/usr/bin/env bash
#
# Configure environment for fast testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

unset CHPL_TARGET_CPU

nightly_args="${nightly_args} -fast"
