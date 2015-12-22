import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import third_party_utils, utils
from utils import memoize


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args():
    # We have to manually link libpthread since we build statically and .a's
    # don't have a way to specify dependencies.
    return third_party_utils.default_get_link_args('jemalloc',
                                                   libs=['-ljemalloc', '-lpthread'])
