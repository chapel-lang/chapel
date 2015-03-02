#!/usr/bin/env python
import utils
from utils import memoize
import third_party_utils

@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args():
    return third_party_utils.default_get_link_args('gmp')
