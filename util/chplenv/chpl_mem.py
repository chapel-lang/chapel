#!/usr/bin/env python3
import optparse
import sys

import chpl_platform, overrides
from utils import error, memoize, warning

@memoize
def get(flag='host'):
    platform_val = chpl_platform.get(flag)
    cygwin = platform_val.startswith('cygwin')
    chpl_host_mem = overrides.get('CHPL_HOST_MEM')
    chpl_target_mem = overrides.get('CHPL_TARGET_MEM')
    chpl_mem = overrides.get('CHPL_MEM')

    if flag == 'target':
        if cygwin:
            mem_val = 'cstdlib'
        elif chpl_target_mem:
            mem_val = chpl_target_mem
            if chpl_mem:
                warning("CHPL_MEM and CHPL_TARGET_MEM are both set, "
                        "taking value from CHPL_TARGET_MEM")
        elif chpl_mem:
            mem_val = chpl_mem
        else:
            mem_val = 'jemalloc'
    elif flag == 'host':
        if cygwin:
            mem_val = 'cstdlib'
        elif chpl_host_mem:
            mem_val = chpl_host_mem
        else:
            mem_val = 'cstdlib'
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
