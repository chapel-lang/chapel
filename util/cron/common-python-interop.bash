#!/usr/bin/env bash

# Ensure we fail early if our dependencies for Python interoperability testing
# are not met

set -e

echo "Ensuring numpy and Cython are installed"

python3 -c "import numpy"
python3 -c "import cython"
