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

    arch_flag = "CHPL_TARGET_ARCH"
    cpu_flag = "CHPL_TARGET_CPU"
    if flag == 'host':
      arch_flag = "CHPL_HOST_ARCH"
      cpu_flag = "CHPL_HOST_CPU"

    if arch_val:
        return arch_val

    # compute the default
    cpu_val = chpl_cpu.get(flag).cpu
    cpuarch = chpl_cpu.arch_for_cpu(cpu_val, flag)
    machine = chpl_cpu.get_default_machine(flag)
    if cpuarch:
        if cpuarch != machine:
            sys.stderr.write('Warning: Cross compilation not yet supported. '
                             'Inferred {0}={1} based upon {2}={3} '
                             'but running on {4}.\n'.format(arch_flag,
                                                            cpuarch,
                                                            cpu_flag,
                                                            cpu_val,
                                                            machine))

    return chpl_cpu.get_default_machine(flag)

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
