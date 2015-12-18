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
    # -lpthread required to satisfy "undefined reference to `pthread_atfork"
    # and others. Note that this only appears to be needed because we build
    # jemalloc statically. When using a .so the -lpthread is not required.
    # @gbt any idea why that is/anything to change or is this ok?
    return third_party_utils.default_get_link_args('jemalloc',
                                                   libs=['-ljemalloc', '-lpthread'])

# TODO (EJR 12/17/15): should we be manually linking the .a like  we do for
# tcmalloc? i.e: `libs=['libjemalloc.a'])`
#
# If the answer is yes, then we'll have to update third_party_utils.py to
# search for .a's just like we do for .la's.
# @gbt any thoughts?
