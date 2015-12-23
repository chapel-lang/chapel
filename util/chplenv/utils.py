import os
import re
import subprocess
from collections import namedtuple
from distutils.spawn import find_executable


def memoize(func):
    cache = func.cache = {}

    def memoize_wrapper(*args, **kwargs):
        if kwargs:
            return func(*args, **kwargs)
        if args not in cache:
            cache[args] = func(*args)
        return cache[args]
    return memoize_wrapper


@memoize
def get_chpl_home():
    chpl_home = os.environ.get('CHPL_HOME', '')
    if not chpl_home:
        dirname = os.path.dirname
        chpl_home = dirname(dirname(dirname(os.path.realpath(__file__))))
    return chpl_home

@memoize
def using_chapel_module():
    chpl_home = os.environ.get('CHPL_HOME', '')
    if chpl_home != '':
        return chpl_home == os.environ.get('CHPL_MODULE_HOME', '')
    return False

@memoize
def get_compiler_version(compiler):
    version_string = '0'
    if 'gnu' in compiler:
        version_string = run_command(['gcc', '-dumpversion'])
    elif 'cray-prgenv-cray' == compiler:
        version_string = os.environ.get('CRAY_CC_VERSION', '0')
    return CompVersion(version_string)

# Takes a version string of the form 'major', 'major.minor',
# 'major.minor.revision', or 'major.minor,revision.build' and returns the named
# tuple (major, minor, revision, build). If minor, revision, or build are not
# specified, 0 will be used for their value(s)
@memoize
def CompVersion(version_string):
    CompVersionT = namedtuple('CompVersion', ['major', 'minor', 'revision', 'build'])
    match = re.search(u'(\d+)(\.(\d+))?(\.(\d+))?(\.(\d+))?', str(version_string))
    if match:
        major    = int(match.group(1))
        minor    = int(match.group(3) or 0)
        revision = int(match.group(5) or 0)
        build    = int(match.group(7) or 0)
        return CompVersionT(major=major, minor=minor, revision=revision, build=build)
    else:
        raise ValueError("Could not convert version '{0}' to "
                         "a tuple".format(version_string))

class CommandError(Exception):
    pass

# This could be replaced by subprocess.check_output, but that isn't available
# until python 2.7 and we only have 2.6 on most machines :(
def run_command(command, stdout=True, stderr=False):
    process = subprocess.Popen(command,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()
    if process.returncode != 0:
        raise CommandError(
            "command `{0}` failed - output was \n{1}".format(command,
                                                             output[1]))
    else:
        if stdout and stderr:
            return output
        elif stdout:
            return output[0]
        elif stderr:
            return output[1]
        else:
            return ''

def compiler_is_prgenv(compiler_val):
  return (compiler_val.startswith('cray-prgenv') or
     os.environ.get('CHPL_ORIG_TARGET_COMPILER','').startswith('cray-prgenv'))
