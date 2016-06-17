import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import third_party_utils, utils
import chpl_compiler, chpl_platform
from utils import memoize


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args(unwind):
    platform_val = chpl_platform.get('target')
    linux = platform_val.startswith('linux64')
    osx = platform_val.startswith('darwin')
    if (unwind == 'libunwind' or unwind == 'system') and linux:
        return ['-lunwind']
    return []
