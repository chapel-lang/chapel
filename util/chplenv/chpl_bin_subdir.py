#!/usr/bin/env python

import optparse
import os
from sys import stderr, stdout
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_home_utils
import chpl_platform, chpl_compiler, chpl_machine, chpl_arch, chpl_lib_pic

from utils import memoize

@memoize
def get(location):
    platform = chpl_platform.get(location)
    compiler = chpl_compiler.get(location)
    machine = chpl_machine.get(location)
    (flag, arch) = chpl_arch.get(location, map_to_compiler=True,
                                 get_lcd=chpl_home_utils.using_chapel_module())
    pic = chpl_lib_pic.get()

    # platform
    result = platform
    # compiler -- included for target
    if location == 'target':
      result += '-' + compiler
    # machine
    result += '-' + machine
    # arch
    if arch != 'none' and arch != 'unknown':
      result += '-' + arch
    # pic
    if pic != 'none':
      result += '-' + pic
    return result

def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target]")
    parser.add_option('--target', dest='location', action='store_const',
                      const='target')
    parser.add_option('--host', dest='location', action='store_const',
                      const='host', default='host')
    (options, args) = parser.parse_args()

    bin_subdir = get(options.location)

    stdout.write("{0}\n".format(bin_subdir))

if __name__ == '__main__':
    _main()
