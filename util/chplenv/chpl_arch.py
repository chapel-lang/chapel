#!/usr/bin/env python
import optparse
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_cpu, overrides
from utils import error, memoize

@memoize
def get(flag='host'):

    if flag == 'host':
        machine_val = overrides.get('CHPL_HOST_ARCH', '')
    elif flag == 'target':
        machine_val = overrides.get('CHPL_TARGET_ARCH', '')
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if machine_val == 'native' or machine_val == 'none':
        sys.stderr.write(
            "CHPL_ARCH={0} is no longer supported. Try CHPL_CPU={0} instead.\n"
            .format(machine_val))
        machine_val = None

    if machine_val:
        return machine_val

    # compute the default
    return chpl_cpu.get_default_machine(flag)

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
