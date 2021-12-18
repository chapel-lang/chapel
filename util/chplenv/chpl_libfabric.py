#!/usr/bin/env python3
import sys

import chpl_comm, chpl_comm_debug, chpl_launcher, chpl_platform, overrides, third_party_utils
from utils import error, memoize, try_run_command, warning


@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val == 'ofi':
        libfabric_val = overrides.get('CHPL_LIBFABRIC')
        platform_val = chpl_platform.get('target')
        if not libfabric_val:
            cmd_exists, returncode = try_run_command(['pkg-config',
                                                      '--exists',
                                                      'libfabric'])[0:2]
            if cmd_exists and returncode == 0:
                libfabric_val = 'system'
            else:
                libfabric_val = 'bundled'
        if libfabric_val == 'none':
            error("CHPL_LIBFABRIC must not be 'none' when CHPL_COMM is ofi")
        if platform_val == 'hpe-cray-ex' and libfabric_val != 'system':
            warning('CHPL_LIBFABRIC!=system is discouraged on HPE Cray EX')
    else:
        libfabric_val = 'none'

    return libfabric_val


@memoize
def get_uniq_cfg_path():
    base_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    if chpl_comm_debug.get() == 'debug':
        suffix = '-debug'
    else:
        suffix = ''
    return base_uniq_cfg + suffix


# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    args = ([ ], [ ])
    libfabric_val = get()
    if libfabric_val == 'bundled':
        ucp_val = get_uniq_cfg_path()
        args = third_party_utils.get_bundled_compile_args('libfabric',
                                                          ucp=ucp_val)
    elif libfabric_val == 'system':
        flags = [ ]

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

        args[1].extend(flags)

    if libfabric == 'system' or libfabric == 'bundled':
        flags = [ ]
        launcher_val = chpl_launcher.get()
        ofi_oob_val = overrides.get_environ('CHPL_RT_COMM_OFI_OOB')
        if 'mpi' in launcher_val or ( ofi_oob_val and 'mpi' in ofi_oob_val ):
            mpi_dir_val = overrides.get_environ('MPI_DIR')
            if mpi_dir_val:
                flags.append('-I' + mpi_dir_val + '/include')

        args[1].extend(flags)

    return args


# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    libfabric_val = get()
    if libfabric_val == 'bundled':
        return third_party_utils.get_bundled_link_args('libfabric',
                                                       ucp=get_uniq_cfg_path(),
                                                       libs=['libfabric.la'],
                                                       add_L_opt=True)
    elif libfabric == 'system':
        libs = [ ]
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
        return ([ ], libs)

    return ([ ], [ ])


def _main():
    libfabric_val = get()
    sys.stdout.write("{0}\n".format(libfabric_val))


if __name__ == '__main__':
    _main()
