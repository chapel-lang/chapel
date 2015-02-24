#!/usr/bin/env python
import utils
from utils import memoize
import chpl_locale_model
import third_party_utils

@memoize
def get_uniq_cfg_path():
    return '{0}-{1}'.format(third_party_utils.default_uniq_cfg_path(),
                            chpl_locale_model.get())

def get_link_args():
    return third_party_utils.default_get_link_args('qthread',
                                                   ucp=get_uniq_cfg_path(),
                                                   libs=['libqthread_chpl.la',
                                                         '-lchpl',
                                                         'libqthread.la'])
