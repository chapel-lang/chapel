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
def using_chapel_module():
    chpl_home = overrides.get('CHPL_HOME', '')
    if chpl_home != '':
        return chpl_home == os.environ.get('CHPL_MODULE_HOME', '')
    return False

