import chpl_compiler, chpl_hwloc, chpl_llvm, chpl_locale_model, chpl_mem
import third_party_utils
from utils import memoize


@memoize
def get_uniq_cfg_path():
    def_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    lm = chpl_locale_model.get()
    target_mem = chpl_mem.get('target')
    hwloc = chpl_hwloc.get()
    return '{0}-{1}-{2}-{3}'.format(def_uniq_cfg, lm, target_mem, hwloc)

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    ucp_val = get_uniq_cfg_path()
    bundled, system = third_party_utils.get_bundled_compile_args('qthread', ucp=ucp_val)
    # qthread headers may have unused variables
    bundled.append('-Wno-error=unused-variable')
    return (bundled, system)


# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    (bundled, system) = third_party_utils.libtool_get_bundled_link_args(
                                      'qthread', ucp=get_uniq_cfg_path())
    return (bundled, system)
