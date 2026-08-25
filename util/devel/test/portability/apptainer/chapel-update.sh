#!/usr/bin/env bash

# Updates the chapel checkout to be 'main' (makes sure it exists)
# Prints a summary at the end.

set -exuo pipefail

./tryit.py "$@" ../../provision-scripts/chapel-update.sh
