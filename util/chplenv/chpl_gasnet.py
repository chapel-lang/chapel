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
def get_gasnet_pc_dict():
    substrate = chpl_comm_substrate.get()
    ucp = get_uniq_cfg_path()
    install_path = third_party_utils.get_cfg_install_path('gasnet', ucp=ucp)
    pcname = "gasnet-{0}-par.pc".format(substrate)
    pcfile = os.path.join(install_path, 'lib', 'pkgconfig', pcname)
    ret = { }

    # assume we haven't built gasnet yet if the install dir doesn't exist
    if os.path.exists(install_path):
        return third_party_utils.read_pkg_config_file(
                    pcfile,
                    'third-party/gasnet/install/' + ucp,
                    install_path)
    else:
        warning("gasnet path {0} missing".format(install_path))

    return ret

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    bundled = [ ]
    system = [ ]

    d = get_gasnet_pc_dict()

    if 'Cflags' in d:
        bundled.extend(d['Cflags'].split())

    return (bundled, system)


# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    bundled = [ ]
    system = [ ]

    d = get_gasnet_pc_dict()

    if 'Libs' in d:
        bundled.extend(d['Libs'].split())

    return (bundled, system)
