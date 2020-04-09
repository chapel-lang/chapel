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
# $CHPL_HOME/third-party/chpl-venv/install/$bin_subdir/$py_version/chpl-virtualenv
@memoize
def get_chpl_venv():
    chpl_venv = os.path.join(get_chpl_third_party(), 'chpl-venv')
    bin_subdir = chpl_bin_subdir.get('host')
    py_version = 'py{0}'.format(chpl_python_version.get())
    uniq_path = os.path.join(bin_subdir, py_version)
    venv_dir = os.path.join(chpl_venv, 'install', uniq_path, 'chpl-virtualenv')
    return venv_dir

# Get the test chpl-venv directory. Defaults to get_chpl_venv(), but can be
# overridden with CHPL_TEST_VENV_DIR
@memoize
def get_chpl_test_venv():
    venv_dir = os.getenv('CHPL_TEST_VENV_DIR', get_chpl_venv())
    return venv_dir

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
    parser.add_option('--venv', action='store_const',
                      dest='func', const=get_chpl_venv)
    parser.add_option('--test-venv', action='store_const',
                      dest='func', const=get_chpl_test_venv)
    parser.add_option('--using-module', action='store_const',
                      dest='func', const=using_chapel_module)
    (options, args) = parser.parse_args()

    if options.func:
        sys.stdout.write("{0}\n".format(options.func()))


if __name__ == '__main__':
    _main()
