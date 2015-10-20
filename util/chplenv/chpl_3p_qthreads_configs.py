#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler, chpl_llvm, chpl_locale_model, third_party_utils, utils
from utils import memoize


@memoize
def get_uniq_cfg_path():
    return '{0}-{1}'.format(third_party_utils.default_uniq_cfg_path(),
                            chpl_locale_model.get())

@memoize
def get_link_args():
    link_args = \
        third_party_utils.default_get_link_args('qthread',
                                                ucp=get_uniq_cfg_path(),
                                                libs=['libqthread_chpl.la',
                                                      '-lchpl',
                                                      'libqthread.la'])
    if ( chpl_compiler.get('target').startswith('cray-prgenv') and
         chpl_llvm.get() != 'none' ):
        link_args.append('-lrt')
    return link_args
