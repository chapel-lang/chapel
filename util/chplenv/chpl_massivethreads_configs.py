#!/usr/bin/env python
import utils
from utils import memoize
import third_party_utils

def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

def get_link_args():
    return third_party_utils.default_get_link_args('massivethreads',
                                                   libs=['libmyth-native.la'])
