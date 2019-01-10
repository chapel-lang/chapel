#!/usr/bin/env python
import optparse
import sys

import chpl_cpu, overrides
from utils import error, memoize

@memoize
def get(flag='host'):

    if flag == 'host':
        arch_val = overrides.get('CHPL_HOST_ARCH', '')
    elif flag == 'target':
        arch_val = overrides.get('CHPL_TARGET_ARCH', '')
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    cpuarch = chpl_cpu.arch_for_cpu(arch_val, flag)
    if cpuarch:
        oldname = "CHPL_TARGET_ARCH"
        newname = "CHPL_TARGET_CPU"
        if flag == 'host':
          oldname = "CHPL_HOST_ARCH"
          newname = "CHPL_HOST_CPU"

        sys.stderr.write('Warning: {0}={1} is deprecated. '
                         'Please use {2}={3}\n'.format(oldname,
                                                       arch_val,
                                                       newname,
                                                       arch_val))
        arch_val = cpuarch

    if arch_val:
        return arch_val

    # compute the default
    cpu_val = chpl_cpu.get(flag).arch
    cpuarch = chpl_cpu.arch_for_cpu(cpu_val, flag)
    if cpuarch:
        return cpuarch
    else:
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
