""" Access to home variables """
import os
import sys

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
def get_chpl_module_home():
    return os.environ.get('CHPL_MODULE_HOME', '')

@memoize
def get_chpl_third_party():
    default = os.path.join(get_chpl_home(), 'third-party')
    chpl_third_party = overrides.get('CHPL_THIRD_PARTY', default)
    return chpl_third_party

@memoize
def using_chapel_module():
    chpl_home = os.path.normpath(overrides.get('CHPL_HOME', ''))
    chpl_module_home = os.path.normpath(get_chpl_module_home())
    if chpl_home != '':
        return chpl_home == chpl_module_home
    return False

