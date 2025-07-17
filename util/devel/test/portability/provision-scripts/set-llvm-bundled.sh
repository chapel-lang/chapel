#!/bin/sh

alias unsudo="sudo -u bin"
alias hide=eval

# NOTE: Root is not needed here, but using `unsudo` doesn't work as it's an
# invocation of `sudo` and you can't use shell builtins like `export` with
# `sudo`.
export CHPL_LLVM=bundled
