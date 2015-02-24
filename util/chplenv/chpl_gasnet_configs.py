#!/usr/bin/env python
import utils
from utils import memoize
import third_party_utils

I was here
def get_uniq_cfg_path():
    return '{0}-{1}-{2}'.format(third_party_utils.default_uniq_cfg_path(),
                                CHPL_MAKE_GASNET_SEGMENT,
                                CHPL_GASNET_DEBUG)

def get_link_args():
    return third_party_utils.default_get_link_args('gasnet',
                                                   ucp=get_uniq_cfg_path,
                                                   GASNET_LIBS)
