#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_home
from utils import memoize


@memoize
def get():
    llvm_val = overrides.get('CHPL_LLVM')
    if not llvm_val:
        host_platform = chpl_platform.get('host')
        host_compiler = chpl_compiler.get('host')
        chpl_home = get_chpl_home()
        llvm_target_dir = '{0}-{1}'.format(host_platform, host_compiler)
        llvm_subdir = os.path.join(chpl_home, 'third-party', 'llvm', 'install',
                                   llvm_target_dir)
        llvm_header = os.path.join(llvm_subdir, 'include', 'llvm',
                                   'PassSupport.h')
        if os.path.exists(llvm_header):
            llvm_val = 'llvm'
        else:
            llvm_val = 'none'
    return llvm_val


def _main():
    llvm_val = get()
    sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
