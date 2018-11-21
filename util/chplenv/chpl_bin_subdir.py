#!/usr/bin/env python

import optparse
import os
import sys
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_home_utils
import chpl_platform, chpl_compiler, chpl_machine, chpl_arch

from utils import memoize

@memoize
def get(flag):
    platform = chpl_platform.get(flag)
    compiler = chpl_compiler.get(flag)
    machine = chpl_machine.get(flag)
    (flag, arch) = chpl_arch.get(flag, map_to_compiler=True,
                                 get_lcd=chpl_home_utils.using_chapel_module())

    # platform
    result = platform
    # compiler -- included for target
    if flag == 'target':
      result += '-' + compiler
    # machine
    result += '-' + machine
    # arch
    if arch != 'none' and arch != 'unknown':
      result += '-' + arch
    return result

def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target]")
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host', default='host')
    (options, args) = parser.parse_args()

    bin_subdir = get(options.flag)

    sys.stdout.write("{0}\n".format(bin_subdir))

if __name__ == '__main__':
    _main()
