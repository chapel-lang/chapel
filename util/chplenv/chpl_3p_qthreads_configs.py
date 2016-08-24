import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler, chpl_llvm, chpl_locale_model, third_party_utils
from compiler_utils import compiler_is_prgenv
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
    compiler_val = chpl_compiler.get('target')
    if ( compiler_val == 'cray-prgenv-cray' or
         (compiler_is_prgenv(compiler_val) and chpl_llvm.get() != 'none' )):
        link_args.append('-lrt')
    return link_args
