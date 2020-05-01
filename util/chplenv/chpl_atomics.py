#!/usr/bin/env python
import optparse
import sys

import chpl_comm, chpl_compiler, chpl_platform, overrides
from compiler_utils import CompVersion, get_compiler_version, has_std_atomics
from utils import error, memoize


@memoize
def get(flag='target'):
    if flag == 'network':
        atomics_val = overrides.get('CHPL_NETWORK_ATOMICS')
        if not atomics_val:
            comm_val = chpl_comm.get()
            if comm_val in ['ofi', 'ugni'] and get('target') != 'locks':
                atomics_val = comm_val
            else:
                atomics_val = 'none'
    elif flag == 'target':
        atomics_val = overrides.get('CHPL_ATOMICS')
        if not atomics_val:
            compiler_val = chpl_compiler.get('target')
            platform_val = chpl_platform.get('target')

            # We default to C standard atomics (cstdlib) for gcc 5 and newer.
            # Some prior versions of gcc look like they support standard
            # atomics, but have buggy or missing parts of the implementation,
            # so we do not try to use cstdlib with gcc < 5. If support is
            # detected for clang (via preprocessor checks) we also default to
            # cstdlib atomics. For llvm/clang-included we always default to
            # cstdlib atomics. We know our clang-included will have compiler
            # support for atomics and llvm requires gcc 4.8 (or a compiler with
            # equivalent features) to be built so we know we'll have system
            # header support too.
            #
            # We support intrinsics for gcc, intel, cray and clang. gcc added
            # initial support in 4.1, and added support for 64-bit atomics on
            # 32-bit platforms with 4.8. clang and intel also support 64-bit
            # atomics on 32-bit platforms and the cray compiler will never run
            # on a 32-bit machine.
            #
            # For pgi or 32-bit platforms with an older gcc, we fall back to
            # locks
            if compiler_val in ['gnu', 'cray-prgenv-gnu', 'mpi-gnu']:
                version = get_compiler_version('gnu')
                if version >= CompVersion('5.0'):
                    atomics_val = 'cstdlib'
                elif version >= CompVersion('4.8'):
                    atomics_val = 'intrinsics'
                elif version >= CompVersion('4.1') and not platform_val.endswith('32'):
                    atomics_val = 'intrinsics'
            elif compiler_val == 'intel' or compiler_val == 'cray-prgenv-intel':
                atomics_val = 'intrinsics'
            elif compiler_val == 'cray-prgenv-cray':
                atomics_val = 'cstdlib'
            elif compiler_val in ['allinea', 'cray-prgenv-allinea']:
                atomics_val = 'cstdlib'
            elif compiler_val == 'clang':
                if has_std_atomics(compiler_val):
                    atomics_val = 'cstdlib'
                else:
                    atomics_val = 'intrinsics'
            elif compiler_val == 'clang-included':
                atomics_val = 'cstdlib'

            # we can't use intrinsics, fall back to locks
            if not atomics_val:
                atomics_val = 'locks'
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)
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
