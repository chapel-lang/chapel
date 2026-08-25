#!/usr/bin/env bash

# Shows the commit on each image.

set -exuo pipefail

./tryit.py "$@" ../../provision-scripts/chapel-showcommit.sh
