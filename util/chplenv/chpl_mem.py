#!/usr/bin/env python
import optparse
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform, overrides
from utils import memoize


@memoize
def get(flag='host'):
    if flag == 'host':
        mem_val = 'cstdlib'
    elif flag == 'target':
        mem_val = overrides.get('CHPL_MEM')
        if not mem_val:
            platform_val = chpl_platform.get('target')
            cygwin = platform_val.startswith('cygwin')

            if cygwin:
                mem_val = 'cstdlib'
            else:
                mem_val = 'jemalloc'
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))
    return mem_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    (options, args) = parser.parse_args()

    mem_val = get(options.flag)
    sys.stdout.write("{0}\n".format(mem_val))


if __name__ == '__main__':
    _main()
