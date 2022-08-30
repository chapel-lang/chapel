#!/usr/bin/env bash
#
# Configure settings for Cray CS HDR performance testing.

export CHPL_LAUNCHER_PARTITION=clx24
export CHPL_TARGET_CPU=none

COMMON_DIR=/cray/css/users/chapelu
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

SETUP_PYTHON=$COMMON_DIR/setup_python_nightly.bash
if [ -f "$SETUP_PYTHON" ]; then
  source $SETUP_PYTHON
fi

echo "Location of Python3"
which python3
echo "Python3 version"
python3 --version

# the lengths we go to, to avoid line wrap ...
pcca=(-performance-configs gn-ibv-large:v,gn-ibv-fast:v,gn-mpi,ofi \
      -performance \
      -perflabel ml- \
      -startdate 03/11/21)
perf_cray_cs_args=${pcca[*]}
