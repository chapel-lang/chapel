#!/usr/bin/env python3
import optparse
import sys

import chpl_cpu, overrides
from utils import error, memoize, warning

@memoize
def get(flag='host'):

    if flag == 'host':
        arch_val = overrides.get('CHPL_HOST_ARCH', '')
    elif flag == 'target':
        arch_val = overrides.get('CHPL_TARGET_ARCH', '')
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if arch_val:
        return arch_val

    # compute the default
    return chpl_cpu.get_default_machine(flag)

def validate(flag='host'):
    pass

def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target]")
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    (options, args) = parser.parse_args()

    arch = get(options.flag)

    sys.stdout.write("{0}\n".format(arch))

if __name__ == '__main__':
    _main()
