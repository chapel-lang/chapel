""" 'and other things': other Chapel environment variables interfaces """

import os
import re
import sys

from collections import namedtuple

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import overrides
from utils import memoize


@memoize
def get_chpl_home():
    chpl_home = overrides.get('CHPL_HOME', '')
    if not chpl_home:
        dirname = os.path.dirname
        chpl_home = dirname(dirname(dirname(os.path.realpath(__file__))))
    return chpl_home


@memoize
def using_chapel_module():
    chpl_home = overrides.get('CHPL_HOME', '')
    if chpl_home != '':
        return chpl_home == os.environ.get('CHPL_MODULE_HOME', '')
    return False


@memoize
def get_compiler_version(compiler):
    version_string = '0'
    if 'gnu' in compiler:
        # Asssuming the 'compiler' version matches the gcc version
        # e.g., `mpicc -dumpversion == gcc -dumpversion`
        version_string = run_command(['gcc', '-dumpversion'])
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
        raise ValueError("Could not convert version '{0}' to "
                         "a tuple".format(version_string))


def compiler_is_prgenv(compiler_val):
  return (compiler_val.startswith('cray-prgenv') or
     os.environ.get('CHPL_ORIG_TARGET_COMPILER','').startswith('cray-prgenv'))
