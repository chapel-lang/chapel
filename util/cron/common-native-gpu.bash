# Use latest system LLVM, to use an earlier version pass version number as a
# parameter to the script.
source /cray/css/users/chapelu/setup_system_llvm.bash
# Lie to prevent common.bash adding a newer llvm
export OFFICIAL_SYSTEM_LLVM=true

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

module load cudatoolkit
export CHPL_LLVM=system
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_PARTITION=stormP100
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/"
