""" Access to home variables """

import optparse
import os
import re
import sys

import chpl_bin_subdir, chpl_python_version, overrides
from utils import memoize


@memoize
def get_chpl_home():
    chpl_home = overrides.get('CHPL_HOME', '')
    if not chpl_home:
        dirname = os.path.dirname
        chpl_home = dirname(dirname(dirname(os.path.realpath(__file__))))
    return chpl_home

@memoize
def get_chpl_runtime_incl():
    default = os.path.join(get_chpl_home(), 'runtime', 'include')
    chpl_runtime_incl = overrides.get('CHPL_RUNTIME_INCL', default)
    return chpl_runtime_incl

@memoize
def get_chpl_runtime_lib():
    default = os.path.join(get_chpl_home(), 'lib')
    chpl_runtime_lib = overrides.get('CHPL_RUNTIME_LIB', default)
    return chpl_runtime_lib

@memoize
def get_chpl_third_party():
    default = os.path.join(get_chpl_home(), 'third-party')
    chpl_third_party = overrides.get('CHPL_THIRD_PARTY', default)
    return chpl_third_party

# Fix paths in the passed string according to the provided list of pairs
# in tofix
def add_vars_to_paths_helper(s, tofix):
    lines = s.splitlines()
    output = [ ]

    for line in lines:
        # Find things that look like absolute paths
        # Note that this needs to handle e.g. -I/some/directory
        # (and not think it's a relative path starting with I)

        pattern = r'/[^ ]+'

        fixed = line

        for m in re.findall(pattern, line):
            origpath = m
            path = os.path.realpath(origpath)

            if os.path.isfile(path):
              # Leave out the filename, since we're only trying to replace
              # directories. This prevents clang++ from being replaced
              # with e.g. clang-6.0 when clang++ is a symbolic link.
              origpath = os.path.dirname(origpath)
              path = os.path.dirname(path)

            for kv in tofix:
              key = kv[0]
              val = kv[1];

              if path.startswith(val):
                rel = os.path.relpath(path, val)
                fixed = fixed.replace(origpath, key + "/" + rel)
                break

        output.append(fixed)

    return "\n".join(output)

# Fix paths in the passed string to use $CHPL_HOME, $CHPL_THIRD_PARTY, etc
def add_vars_to_paths(s):
    tofix = [ ]

    tofix.append( ['$CHPL_RUNTIME_LIB', get_chpl_runtime_lib()] )
    tofix.append( ['$CHPL_RUNTIME_INCL', get_chpl_runtime_incl()] )
    tofix.append( ['$CHPL_THIRD_PARTY', get_chpl_third_party()] )
    tofix.append( ['$CHPL_HOME', get_chpl_home()] )

    return add_vars_to_paths_helper(s, tofix)

# Get the chpl-venv install directory:
# $CHPL_HOME/third-party/chpl-venv/install/chpldeps
@memoize
def get_chpldeps():
    chpl_venv = os.path.join(get_chpl_third_party(), 'chpl-venv',
                             'install', 'chpldeps')
    return chpl_venv

@memoize
def using_chapel_module():
    chpl_home = overrides.get('CHPL_HOME', None)
    chpl_module_home = os.environ.get('CHPL_MODULE_HOME', None)
    if chpl_home and chpl_module_home:
        return os.path.normpath(chpl_home) == os.path.normpath(chpl_module_home)
    return False


def _main():
    parser = optparse.OptionParser()
    parser.add_option('--home', action='store_const',
                      dest='func', const=get_chpl_home)
    parser.add_option('--third-party', action='store_const',
                      dest='func', const=get_chpl_third_party)
    parser.add_option('--chpldeps', action='store_const',
                      dest='func', const=get_chpldeps)
    parser.add_option('--using-module', action='store_const',
                      dest='func', const=using_chapel_module)
    (options, args) = parser.parse_args()

    if options.func:
        sys.stdout.write("{0}\n".format(options.func()))


if __name__ == '__main__':
    _main()
