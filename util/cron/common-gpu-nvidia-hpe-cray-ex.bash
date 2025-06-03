# common settings for running GPU nightly testing on the HPE Cray EX system

export CHPL_LAUNCHER_PARTITION=griz512

# required due to https://github.com/chapel-lang/chapel/issues/27273
export CHPL_LLVM=bundled
unset CHPL_LLVM_CONFIG
