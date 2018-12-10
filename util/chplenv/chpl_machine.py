#!/usr/bin/env python
import optparse
import sys

import chpl_arch, overrides
from utils import error, memoize

@memoize
def get(flag='host'):

    if flag == 'host':
        machine_val = overrides.get('CHPL_HOST_MACHINE', '')
    elif flag == 'target':
        machine_val = overrides.get('CHPL_TARGET_MACHINE', '')
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if machine_val:
        return machine_val

    # compute the default
    return chpl_arch.get_default_machine(flag)

def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target]")
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    (options, args) = parser.parse_args()

    machine = get(options.flag)

    sys.stdout.write("{0}\n".format(machine))

if __name__ == '__main__':
    _main()
