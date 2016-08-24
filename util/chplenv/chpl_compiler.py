#!/usr/bin/env python
import optparse
import os
import sys

from distutils.spawn import find_executable
from sys import stderr, stdout

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform, overrides
from utils import memoize



@memoize
def get(flag='host'):
    if flag == 'host':
        compiler_val = overrides.get('CHPL_HOST_COMPILER', '')
    elif flag == 'target':
        compiler_val = overrides.get('CHPL_TARGET_COMPILER', '')
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))

    if compiler_val:
        return compiler_val

    platform_val = chpl_platform.get(flag)
    # The cray platforms are a special case in that we want to "cross-compile"
    # by default. (the compiler is different between host and target, but the
    # platform is the same)
    if platform_val.startswith('cray-x'):
        if flag == 'host':
            compiler_val = 'gnu'
        else:
            subcompiler = os.environ.get('PE_ENV', 'none')
            if subcompiler == 'none':
                stderr.write("Warning: Compiling on {0} without a PrgEnv loaded\n".format(platform_val))
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
        elif platform_val == 'marenostrum':
            compiler_val = 'ibm'
        elif platform_val == 'darwin':
            if find_executable('clang'):
                compiler_val = 'clang'
            else:
                compiler_val = 'gnu'
        else:
            compiler_val = 'gnu'
    return compiler_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host', default='host')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target')
    (options, args) = parser.parse_args()

    compiler_val = get(options.flag)
    stdout.write("{0}\n".format(compiler_val))


if __name__ == '__main__':
    _main()
