#!/usr/bin/env python
from utils import memoize

from . import third_party_utils, utils


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args():
    return third_party_utils.default_get_link_args('re2',
                                                   libs=['-lre2', '-lpthread'])
