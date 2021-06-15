""" Backend compiler utility functions for chplenv modules """
import os
import re

import chpl_compiler

from collections import namedtuple

from utils import error, memoize, run_command

@memoize
def get_compiler_version(compiler):
    version_string = '0'
    if 'gnu' in compiler:
        # Asssuming the 'compiler' version matches the gcc version
        # e.g., `mpicc -dumpversion == gcc -dumpversion`
        gcc = chpl_compiler.get_compiler_name_c(compiler)
        version_string = run_command([gcc, '-dumpversion'])
    elif 'cray-prgenv-cray' == compiler:
        version_string = os.environ.get('CRAY_CC_VERSION', '0')
    return CompVersion(version_string)


@memoize
def CompVersion(version_string):
    """
    Takes a version string of the form 'major', 'major.minor',
    'major.minor.revision', or 'major.minor,revision.build' and returns the
    named tuple (major, minor, revision, build). If minor, revision, or build
    are not specified, 0 will be used for their value(s)
    """
    CompVersionT = namedtuple('CompVersion', ['major', 'minor', 'revision', 'build'])
    match = re.search(u'(\d+)(\.(\d+))?(\.(\d+))?(\.(\d+))?', version_string)
    if match:
        major    = int(match.group(1))
        minor    = int(match.group(3) or 0)
        revision = int(match.group(5) or 0)
        build    = int(match.group(7) or 0)
        return CompVersionT(major=major, minor=minor, revision=revision, build=build)
    else:
        error("Could not convert version '{0}' to a tuple".format(
            version_string), ValueError)


@memoize
def target_compiler_is_prgenv(bypass_llvm=True):
    compiler_val = chpl_compiler.get('target')

    # But for CHPL_TARGET_COMPILER=llvm, look at the original target compiler
    if bypass_llvm:
        if compiler_val == 'llvm':
            compiler_val = chpl_compiler.get_prgenv_compiler()

    isprgenv = chpl_compiler.compiler_is_prgenv(compiler_val)
    return isprgenv


def strip_preprocessor_lines(lines):
    lines = [line for line in lines if len(line.split('#')[0].strip()) > 0]
    return lines

#
# Determine whether a given compiler's default compilation mode
# supports standard atomics by running the compiler and checking
# how it expands key feature-test macros.
#
# The assumption is that if standard atomics make it into the
# compiler's default compilation mode, then they actually work.
# If they are not available in the default mode, they probably
# have problems and we don't want to use them.
#
# Due to the command-line options required, this works for GCC,
# Clang, and the Intel compiler, but probably not others.
#
@memoize
def has_std_atomics(compiler_val):
    try:
        compiler_name = chpl_compiler.get_compiler_name_c(compiler_val)
        if compiler_name == 'unknown-c-compiler':
            return False

        version_key='version'
        atomics_key='atomics'

        cmd_input = '{0}=__STDC_VERSION__\n{1}=__STDC_NO_ATOMICS__'.format(version_key, atomics_key)
        cmd = [compiler_name, '-E', '-x', 'c', '-']
        output = run_command(cmd, cmd_input=cmd_input)
        output = strip_preprocessor_lines(output.splitlines())

        output_dict = dict(line.split('=') for line in output)
        version = output_dict[version_key].rstrip("L")
        atomics = output_dict[atomics_key]

        if version == "__STDC_VERSION__" or int(version) < 201112:
            return False
        # If the atomics macro was expanded, then we do not have support.
        if atomics != "__STDC_NO_ATOMICS__":
            return False
        return True
    except:
        return False
