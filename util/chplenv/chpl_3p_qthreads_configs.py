import chpl_compiler, chpl_hwloc, chpl_llvm, chpl_locale_model, chpl_mem
import third_party_utils
from utils import memoize


@memoize
def get_uniq_cfg_path():
    def_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    lm = chpl_locale_model.get();
    target_mem = chpl_mem.get('target')
    hwloc = chpl_hwloc.get()
    return '{0}-{1}-{2}-{3}'.format(def_uniq_cfg, lm, target_mem, hwloc)

@memoize
def get_link_args():
    # Qthreads may call back to the runtime, so re-search libchpl after.
    link_args = \
        third_party_utils.default_get_link_args('qthread',
                                                ucp=get_uniq_cfg_path(),
                                                libs=['libqthread.la',
                                                      '-lchpl'])
    return link_args
