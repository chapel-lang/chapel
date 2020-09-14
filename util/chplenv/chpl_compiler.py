#!/usr/bin/env python
import optparse
import os
import sys

from distutils.spawn import find_executable

import chpl_platform, overrides
from utils import error, memoize


#
# If we can't find a file $CHPL_HOME/make/Makefile.<compiler_val>,
# that suggests that this is a compiler that we're not familiar with.
# In practice, this will cause our Makefiles to use defaults like CC
# and CXX to compile things, for better or worse.
#
@memoize
def validate(compiler_val):
    import chpl_home_utils
    chpl_home = chpl_home_utils.get_chpl_home()
    comp_makefile = os.path.join(chpl_home, 'make', 'compiler', 'Makefile.{0}'.format(compiler_val))
    if not os.path.isfile(comp_makefile):
        sys.stderr.write('Warning: Unknown compiler: "{0}"\n'.format(compiler_val))



@memoize
def get(flag='host', llvm_mode='default'):

    if flag == 'host':
        compiler_val = overrides.get('CHPL_HOST_COMPILER', '')

        if llvm_mode != 'default':
            error("bad call to chpl_compiler.get('host', llvm_mode!=default)")

    elif flag == 'target':
        compiler_val = overrides.get('CHPL_TARGET_COMPILER', '')

        if llvm_mode == 'llvm':
            compiler_val = 'clang-included'
        elif llvm_mode == 'default':
            if ("CHPL_LLVM_CODEGEN" in os.environ and
                os.environ["CHPL_LLVM_CODEGEN"] != "0"):
                compiler_val = 'clang-included'
        elif llvm_mode == 'orig':
            # If we're in the middle of a chpl --llvm call,
            # CHPL_TARGET_COMPILER is already clang-included, but
            # the original target compiler is saved in an environment variable.
            # (otherwise, use compiler_val set above with CHPL_TARGET_COMPILER)
            if "CHPL_ORIG_TARGET_COMPILER" in os.environ:
                compiler_val = os.environ["CHPL_ORIG_TARGET_COMPILER"]

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if compiler_val:
        validate(compiler_val)
        return compiler_val

    platform_val = chpl_platform.get(flag)
    # The cray platforms are a special case in that we want to "cross-compile"
    # by default. (the compiler is different between host and target, but the
    # platform is the same)
    if platform_val.startswith('cray-x') or platform_val == 'hpe-cray-ex':
        if flag == 'host':
            compiler_val = 'gnu'
        else:
            subcompiler = os.environ.get('PE_ENV', 'none')
            if subcompiler == 'none':
                sys.stderr.write("Warning: Compiling on {0} without a PrgEnv loaded\n".format(platform_val))
            compiler_val = "cray-prgenv-{0}".format(subcompiler.lower())
    else:
        # Normal compilation (not "cross-compiling")
        # inherit the host compiler if the target compiler is not set and
        # the host and target platforms are the same
        if flag == 'target':
            if chpl_platform.get('host') == platform_val:
                compiler_val = get('host')
        elif platform_val.startswith('pwr'):
            compiler_val = 'ibm'
        elif platform_val == 'darwin' or platform_val == 'freebsd':
            if find_executable('clang'):
                compiler_val = 'clang'
            else:
                compiler_val = 'gnu'
        else:
            compiler_val = 'gnu'

    validate(compiler_val)
    return compiler_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host', default='host')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target')
    (options, args) = parser.parse_args()

    compiler_val = get(options.flag)
    sys.stdout.write("{0}\n".format(compiler_val))


if __name__ == '__main__':
    _main()
