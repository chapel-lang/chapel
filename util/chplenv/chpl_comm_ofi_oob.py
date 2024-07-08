#!/usr/bin/env python3
import sys
import os

import chpl_comm, chpl_launcher, chpl_platform
import overrides
import third_party_utils

from utils import error, memoize

@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val != 'ofi':
        return 'none'

    oob_val = overrides.get('CHPL_COMM_OFI_OOB')
    if oob_val:
        if oob_val not in ('mpi', 'pmi2', 'sockets'):
            error("CHPL_COMM_OFI_OOB must be 'mpi', 'pmi2', or 'sockets'")
        return oob_val

    #
    # By default, use PMI2 out-of-band support on Cray X* and HPE Cray EX
    # systems, MPI on other Cray systems or with an MPI-based launcher,
    # and "sockets" otherwise.
    #
    platform_val = chpl_platform.get('target')
    launcher_val = chpl_launcher.get()
    if platform_val in ('cray-xc', 'hpe-cray-ex'):
        oob_val = 'pmi2'
    elif 'cray-' in platform_val:
        oob_val = 'mpi'
    elif 'mpi' in launcher_val:
        oob_val = 'mpi'
    else:
        oob_val = 'sockets'

    return oob_val

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    args = ([ ], [ ])
    libfabric_val = get()
    if libfabric_val != 'system' and libfabric_val != 'bundled':
        return args

    flags = [ ]
    ofi_oob_val = get()
    if ofi_oob_val == 'mpi':
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
    if libfabric_val != 'system' and libfabric_val != 'bundled':
        return args

    libs = [ ]
    ofi_oob_val = get()
    if ofi_oob_val == 'mpi':
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
    if ofi_oob_val == 'pmi2' and 'cray-xc' != platform_val:

        # try and use pkg-config to get the libraries to link, but if that fails still just use the default
        tup = third_party_utils.pkgconfig_get_system_link_args('pmi2')
        if tup == (None, None):
            libs.append('-lpmi2')
        else:
            assert len(tup[0]) == 0
            libs.extend(tup[1])
    args[1].extend(libs)

    return args

def _main():
    oob_val = get()
    sys.stdout.write("{0}\n".format(oob_val))


if __name__ == '__main__':
    _main()
