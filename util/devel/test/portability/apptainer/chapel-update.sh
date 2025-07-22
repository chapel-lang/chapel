#!/usr/bin/env bash

# Updates the chapel checkout to be 'main' (makes sure it exists)
# Prints a summary at the end.

./tryit.py "$@" ../../provision-scripts/chapel-update.sh
