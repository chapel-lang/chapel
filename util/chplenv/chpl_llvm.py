#!/usr/bin/env python
import optparse
import os
import sys

import chpl_bin_subdir, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party
from utils import memoize

@memoize
def get_uniq_cfg_path():
    host_bin_subdir = chpl_bin_subdir.get('host')
    host_compiler = chpl_compiler.get('host')
    llvm_target_dir = '{0}-{1}'.format(host_bin_subdir, host_compiler)
    return llvm_target_dir


@memoize
def get():
    llvm_val = overrides.get('CHPL_LLVM')
    if not llvm_val:
        chpl_third_party = get_chpl_third_party()
        llvm_target_dir = get_uniq_cfg_path()
        llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
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

    parser = optparse.OptionParser(usage='usage: %prog [--needs-llvm-runtime]')
    parser.add_option('--needs-llvm-runtime', dest='needsllvm',
                      action='store_const',
                      const='needsllvm', default='')
    (options, args) = parser.parse_args()

    #if --needs-llvm-runtime is set, print out llvm if runtime is needed,
    # and print out nothing if it is not.
    if options.needsllvm:
      if llvm_val == 'system' or llvm_val == 'llvm':
        sys.stdout.write("llvm\n");
    else:
      sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
