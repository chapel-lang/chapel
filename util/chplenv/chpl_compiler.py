#!/usr/bin/env python3
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
    if compiler_val != 'llvm':
        import chpl_home_utils
        chpl_home = chpl_home_utils.get_chpl_home()
        comp_makefile = os.path.join(chpl_home, 'make', 'compiler', 'Makefile.{0}'.format(compiler_val))
        if not os.path.isfile(comp_makefile):
            sys.stderr.write('Warning: Unknown compiler: "{0}"\n'.format(compiler_val))



@memoize
def get_prgenv_compiler():
    platform_val = chpl_platform.get('target')
    if platform_val.startswith('cray-x') or platform_val == 'hpe-cray-ex':
        subcompiler = os.environ.get('PE_ENV', 'none')
        if subcompiler != 'none':
            return "cray-prgenv-{0}".format(subcompiler.lower())

        else:
            sys.stderr.write("Warning: Compiling on {0} without a PrgEnv loaded\n".format(platform_val))

    return 'none'

@memoize
def get(flag='host'):

    if flag == 'host':
        compiler_val = overrides.get('CHPL_HOST_COMPILER', '')

    elif flag == 'target':
        compiler_val = overrides.get('CHPL_TARGET_COMPILER', '')

        import chpl_llvm
        has_llvm = chpl_llvm.get()

        if has_llvm == 'bundled' or has_llvm  == 'system':
            if compiler_val:
                sys.stderr.write("Warning: CHPL_TARGET_COMPILER ignored when compiling with LLVM")
            # Default to LLVM backend if available
            compiler_val = 'llvm'

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if compiler_val:
        validate(compiler_val)
        return compiler_val

    # The cray platforms are a special case in that we want to "cross-compile"
    # by default. (the compiler is different between host and target, but the
    # platform is the same).
    prgenv_compiler = get_prgenv_compiler()
    if prgenv_compiler != 'none':
        compiler_val = prgenv_compiler
    else:
        platform_val = chpl_platform.get(flag)
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

@memoize
def get_cc(flag='host'):
    compiler = get(flag=flag)
    if compiler == 'llvm':
        import chpl_llvm
        return chpl_llvm.get_clang_cc()

@memoize
def get_cpp(flag='host'):
    compiler = get(flag=flag)
    if compiler == 'llvm':
        import chpl_llvm
        return chpl_llvm.get_clang_cpp()


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
