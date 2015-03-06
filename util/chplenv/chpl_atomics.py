#!/usr/bin/env python
import sys, os, optparse

import chpl_platform, chpl_comm, chpl_compiler, utils
from utils import memoize

@memoize
def get(flag='target'):
    if flag == 'network':
        atomics_val = os.environ.get('CHPL_NETWORK_ATOMICS')
        if not atomics_val:
            if chpl_comm.get() == 'ugni':
                atomics_val = 'ugni'
            else:
                atomics_val = 'none'
    elif flag == 'target':
        atomics_val = os.environ.get('CHPL_ATOMICS')
        if not atomics_val:
            compiler_val = chpl_compiler.get('target')
            platform_val = chpl_platform.get('target')

            # we currently support intrinsics for gcc, intel, cray and clang.
            # gcc added initial support in 4.1, and added support for 64 bit
            # atomics on 32 bit platforms with 4.8. clang and intel also
            # support 64 bit atomics on 32 bit platforms and the cray compiler
            # will never run on a 32 bit machine. For pgi or 32 bit platforms
            # with an older gcc, we fall back to locks
            if compiler_val == 'gnu' or compiler_val == 'cray-prgenv-gnu':
                version = utils.get_compiler_version('gnu')
                if version.major > 4:
                    atomics_val = 'intrinsics'
                if version.major == 4:
                    if version.minor >= 8:
                        atomics_val = 'intrinsics'
                    elif version.minor >= 1 and not platform_val.endswith('32'):
                        atomics_val = 'intrinsics'
            elif compiler_val == 'intel' or compiler_val == 'cray-prgenv-intel':
                atomics_val = 'intrinsics'
            elif compiler_val == 'cray-prgenv-cray':
                atomics_val = 'intrinsics'
            elif compiler_val == 'clang':
                atomics_val = 'intrinsics'

            # we can't use intrinsics, fall back to locks
            if not atomics_val:
                atomics_val = 'locks'
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))
    return atomics_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--network|target])')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--network', dest='flag', action='store_const',
                      const='network')
    (options, args) = parser.parse_args()

    atomics_val = get(options.flag)
    sys.stdout.write("{0}\n".format(atomics_val))


if __name__ == '__main__':
    _main()
