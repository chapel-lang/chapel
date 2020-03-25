import chpl_launcher, overrides, third_party_utils
from utils import error, memoize


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_compile_args(libfabric):
    flags = []
    if libfabric == 'system':
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
    elif libfabric == 'libfabric':
        error("CHPL_LIBFABRIC=libfabric is not yet supported", ValueError)

    launcher_val = chpl_launcher.get()
    ofi_oob_val = overrides.get_environ('CHPL_RT_COMM_OFI_OOB')
    if 'mpi' in launcher_val or ( ofi_oob_val and 'mpi' in ofi_oob_val ):
        mpi_dir_val = overrides.get_environ('MPI_DIR')
        if mpi_dir_val:
            flags.append('-I' + mpi_dir_val + '/include')

    return flags

@memoize
def get_link_args(libfabric):
    libs = []
    if libfabric == 'system':
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
    elif libfabric == 'libfabric':
        error("CHPL_LIBFABRIC=libfabric is not yet supported", ValueError)

    return libs
