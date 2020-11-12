""" Access to home variables """

import optparse
import os
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
def get_chpl_third_party():
    default = os.path.join(get_chpl_home(), 'third-party')
    chpl_third_party = overrides.get('CHPL_THIRD_PARTY', default)
    return chpl_third_party

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
