#!/usr/bin/env python
import optparse
import sys

import chpl_platform, overrides
from utils import error, memoize


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
        error("Invalid flag: '{0}'".format(flag), ValueError)
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
