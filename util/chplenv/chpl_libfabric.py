#!/usr/bin/env python3
import sys
import glob
import os

import chpl_comm, chpl_comm_debug, chpl_launcher, chpl_platform
import overrides, third_party_utils

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
            args[1].append('-I' + libfab_dir_val + '/include')
        else:
            # Try using pkg-config to get the compile-time flags.
            x = third_party_utils.pkgconfig_get_system_compile_args('libfabric')
            args = x

    if libfabric_val == 'system' or libfabric_val == 'bundled':
        flags = [ ]
        launcher_val = chpl_launcher.get()
        ofi_oob_val = overrides.get_environ('CHPL_COMM_OFI_OOB')
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
    args = ([ ], [ ])
    libfabric_val = get()
    if libfabric_val == 'bundled':
        args = third_party_utils.pkgconfig_get_bundled_link_args(
                                          'libfabric', ucp=get_uniq_cfg_path())
    elif libfabric_val == 'system':
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
            tup = third_party_utils.pkgconfig_get_system_link_args('libfabric')
            # put the two lists together (but expect tup[0] to be empty)
            pclibs = tup[0] + tup[1]

            # add -Wl,-rpath for the -L options
            # this was a workaround and is probably not needed anymore
            for pcl in pclibs:
                libs.append(pcl)
                if pcl.startswith('-L'):
                    libs.append(pcl.replace('-L', '-Wl,-rpath,', 1))

        args[1].extend(libs)

    if libfabric_val == 'system' or libfabric_val == 'bundled':
        libs = [ ]
        launcher_val = chpl_launcher.get()
        ofi_oob_val = overrides.get_environ('CHPL_COMM_OFI_OOB')
        if 'mpi' in launcher_val or ( ofi_oob_val and 'mpi' in ofi_oob_val ):
            mpi_dir_val = overrides.get_environ('MPI_DIR')
            if mpi_dir_val:
                mpi_lib_dir = os.path.join(mpi_dir_val, 'lib64')
                if not os.path.exists(mpi_lib_dir):
                    mpi_lib_dir = os.path.join(mpi_dir_val, 'lib')
                    if not os.path.exists(mpi_lib_dir):
                        mpi_lib_dir = None

                if mpi_lib_dir:
                    libs.append('-L' + mpi_lib_dir)
                    libs.append('-Wl,-rpath,' + mpi_lib_dir)
                    mpi_lib_name = 'mpi'
                    if glob.glob(mpi_lib_dir + '/libmpich.*'):
                        mpi_lib_name = 'mpich'
                    libs.append('-l' + mpi_lib_name)

        # If we're using the PMI2 out-of-band support we have to reference
        # libpmi2 explicitly, except on Cray XC systems.
        platform_val = chpl_platform.get('target')
        if platform_val == 'hpe-cray-ex' or ofi_oob_val == 'pmi2':
            libs.append('-lpmi2')

        args[1].extend(libs)


    return args


def _main():
    libfabric_val = get()
    sys.stdout.write("{0}\n".format(libfabric_val))


if __name__ == '__main__':
    _main()
