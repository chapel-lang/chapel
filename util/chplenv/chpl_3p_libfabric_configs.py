import chpl_libfabric, chpl_comm_debug
import chpl_launcher, overrides, third_party_utils
from utils import error, memoize


@memoize
def get_uniq_cfg_path():
    base_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    if chpl_comm_debug.get() == 'debug':
        suffix = '-debug'
    else:
        suffix = ''
    return base_uniq_cfg + suffix

@memoize
def get_compile_args(libfabric=chpl_libfabric.get()):
    flags = []
    if libfabric == 'libfabric':
        flags = third_party_utils.default_get_compile_args('libfabric',
                                                           ucp=get_uniq_cfg_path())
    elif libfabric == 'system':
        # Allow overriding pkg-config via LIBFABRIC_DIR, for platforms
        # without pkg-config.
        libfab_dir_val = overrides.get('LIBFABRIC_DIR')
        if libfab_dir_val:
            flags.append('-I' + libfab_dir_val + '/include')
        else:
            # Try using pkg-config to get the compile-time flags.
            pcflags = third_party_utils.pkgconfig_get_compile_args('libfabric',
                                                                   system=True)
            for pcl in pcflags:
                flags.append(pcl)

    launcher_val = chpl_launcher.get()
    ofi_oob_val = overrides.get_environ('CHPL_RT_COMM_OFI_OOB')
    if 'mpi' in launcher_val or ( ofi_oob_val and 'mpi' in ofi_oob_val ):
        mpi_dir_val = overrides.get_environ('MPI_DIR')
        if mpi_dir_val:
            flags.append('-I' + mpi_dir_val + '/include')

    return flags

@memoize
def get_link_args(libfabric=chpl_libfabric.get()):
    libs = []
    if libfabric == 'libfabric':
        return third_party_utils.default_get_link_args('libfabric',
                                                       ucp=get_uniq_cfg_path(),
                                                       libs=['libfabric.la'],
                                                       add_L_opt=True)
    elif libfabric == 'system':
        # Allow overriding pkg-config via LIBFABRIC_DIR, for platforms
        # without pkg-config.
        libfab_dir_val = overrides.get('LIBFABRIC_DIR')
        if libfab_dir_val:
            libs.extend(['-L' + libfab_dir_val + '/lib',
                         '-Wl,-rpath,' + libfab_dir_val + '/lib',
                         '-lfabric'])
        else:
            # Try using pkg-config to get the libraries to link
            # libfabric with.
            pclibs = third_party_utils.pkgconfig_get_link_args('libfabric',
                                                               system=True)
            for pcl in pclibs:
                libs.append(pcl)
                if pcl.startswith('-L'):
                    libs.append(pcl.replace('-L', '-Wl,-rpath,', 1))

    return libs
