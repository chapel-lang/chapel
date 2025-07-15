#!/usr/bin/env bash

# Checks if a fresh Quickstart build passes make check
# Prints a summary at the end.

# apptainer forwards env vars to container by default, so unset
# CHPL_DEVELOPER b/c it can change warning behavior
unset CHPL_DEVELOPER

./tryit.py "$@" --rebuild --cleanup ../../provision-scripts/chapel-quickstart.sh
