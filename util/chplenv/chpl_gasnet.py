import chpl_comm_debug, chpl_comm_segment, chpl_comm_substrate
import chpl_home_utils, third_party_utils
from utils import error, warning, memoize
import os

@memoize
def get_uniq_cfg_path():
    base_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    if chpl_comm_debug.get() == 'debug':
        base_uniq_cfg += '-debug'
    substrate = chpl_comm_substrate.get()
    segment = chpl_comm_segment.get()
    return base_uniq_cfg + '/substrate-' + substrate + '/seg-' + segment


@memoize
def get_gasnet_pc_file():
    substrate = chpl_comm_substrate.get()
    pcfile = "gasnet-{0}-par.pc".format(substrate)
    return pcfile

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    return third_party_utils.pkgconfig_get_bundled_compile_args(
                       'gasnet', get_uniq_cfg_path(), get_gasnet_pc_file())

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    return third_party_utils.pkgconfig_get_bundled_link_args(
                       'gasnet', get_uniq_cfg_path(), get_gasnet_pc_file())
