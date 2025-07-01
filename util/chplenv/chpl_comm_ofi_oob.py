#!/usr/bin/env python3
import sys
import os
import optparse

import chpl_comm, chpl_launcher, chpl_platform
import overrides
import third_party_utils
import glob

from utils import error, memoize, check_valid_var

@memoize
def _find_pmi2():
    """
    PMI2 can have multiple names, look for all of them
    If its found, return the name used, CFLAGS, and LDFLAGS
    Otherwise, return None
    """
    pkg_config_names = ('pmi2', 'cray-pmi')
    for name in pkg_config_names:
        if third_party_utils.pkgconfig_system_has_package(name):
            cflags = third_party_utils.pkgconfig_get_system_compile_args(name)
            ldflags = third_party_utils.pkgconfig_get_system_link_args(name)
            assert cflags != (None, None)
            assert ldflags != (None, None)
            return (name, cflags, ldflags)
    # if we can't use pkg-config, look in common locations
    paths = ('/opt/cray/pe/pmi/default',)
    for path in paths:
        dirs = (path, os.path.join(path, 'include'), os.path.join(path, 'lib'))
        if all(map(os.path.exists, dirs)):
            cflags = ([], ['-I' + os.path.join(path, 'include')])
            ldflags = ([], ['-L' + os.path.join(path, 'lib'), '-lpmi2'])
            return (path, cflags, ldflags)
    return None

@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val != 'ofi':
        return 'none'

    oob_val = overrides.get('CHPL_COMM_OFI_OOB')
    if not oob_val:
        #
        # By default, use PMI2 out-of-band support on Cray X* and HPE Cray EX
        # systems, MPI on other Cray systems or with an MPI-based launcher,
        # and "sockets" otherwise.
        #
        platform_val = chpl_platform.get('target')
        launcher_val = chpl_launcher.get()
        if 'cray-x' in platform_val or chpl_platform.is_hpe_cray('target'):
            oob_val = 'pmi2'
        elif 'cray-' in platform_val:
            oob_val = 'mpi'
        elif 'mpi' in launcher_val:
            oob_val = 'mpi'
        else:
            import chpl_compiler
            if _find_pmi2() is not None:
                oob_val = 'pmi2'
            elif "-lpmi2" in chpl_compiler.get_system_link_args('target'):
                oob_val = 'pmi2'
            else:
                oob_val = 'sockets'

    check_valid_var("CHPL_COMM_OFI_OOB", oob_val, ("mpi", "pmi2", "sockets"))
    return oob_val

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    args = ([ ], [ ])

    flags = [ ]
    ofi_oob_val = get()
    if ofi_oob_val == 'mpi':
        mpi_dir_val = overrides.get_environ('MPI_DIR')
        if mpi_dir_val:
            flags.append('-I' + mpi_dir_val + '/include')
    elif ofi_oob_val == 'pmi2':
        # try and use pkg-config to get the libraries to link, but if that fails still just use the default of nothing
        tup = _find_pmi2()
        if tup is not None:
            cflags = tup[1]
            assert len(cflags[0]) == 0
            flags.extend(cflags[1])

    args[1].extend(flags)

    return args


# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    args = ([ ], [ ])

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
        tup = _find_pmi2()
        if tup is None:
            libs.append('-lpmi2')
        else:
            ldflags = tup[2]
            assert len(ldflags[0]) == 0
            libs.extend(ldflags[1])
    args[1].extend(libs)

    return args

def _main():
    parser = optparse.OptionParser(usage='usage: %prog [options]')
    parser.add_option('--compile', dest='which', action='store_const',
                      const='compile', default=None)
    parser.add_option('--link', dest='which', action='store_const',
                      const='link')
    options, args = parser.parse_args()
    if options.which == 'compile':
        oob_val = get_compile_args()
    elif options.which == 'link':
        oob_val = get_link_args()
    else:
        oob_val = get()
    sys.stdout.write("{0}\n".format(oob_val))


if __name__ == '__main__':
    _main()
