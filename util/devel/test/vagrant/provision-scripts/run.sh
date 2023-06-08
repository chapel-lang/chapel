#!/usr/bin/env bash

# Just run the provided command in the current shell.
echo "Running command in image: ${@@Q}"
"$@"
