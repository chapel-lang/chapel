#!/usr/bin/env python3

import optparse
import sys

import chpl_home_utils
import chpl_platform, chpl_arch, chpl_cpu

from utils import memoize

@memoize
def get(flag='host'):
    platform = chpl_platform.get(flag)
    arch = chpl_arch.get(flag)
    (_, cpu) = chpl_cpu.get(flag, map_to_compiler=True,
                            get_lcd=chpl_home_utils.using_chapel_module())

    # platform
    result = platform

    # compiler is never included since it shouldn't be needed
    # for a bin/ path.

    # arch
    result += '-' + arch

    # cpu
    if cpu != 'none' and cpu != 'unknown':
        result += '-' + cpu
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
