#!/usr/bin/env python3
import os
import sys
import optparse

import chpl_bin_subdir, chpl_compiler, chpl_mem, chpl_platform, overrides, third_party_utils
from utils import error, memoize, run_command, warning


@memoize
def get(flag='target'):
    chpl_jemalloc = overrides.get('CHPL_JEMALLOC')
    chpl_host_jemalloc = overrides.get('CHPL_HOST_JEMALLOC')
    chpl_target_jemalloc = overrides.get('CHPL_TARGET_JEMALLOC')

    mem_val = chpl_mem.get(flag)
    platform_val = chpl_platform.get(flag)

    darwin = platform_val.startswith('darwin')
    cygwin = platform_val.startswith('cygwin')
    linux = not darwin and not cygwin

    if flag == 'target':
        if chpl_target_jemalloc:
            jemalloc_val = chpl_target_jemalloc
            if chpl_jemalloc:
                warning("CHPL_JEMALLOC and CHPL_TARGET_JEMALLOC are both set, "
                        "taking value from CHPL_TARGET_JEMALLOC")
        elif chpl_jemalloc:
            jemalloc_val = chpl_jemalloc
        elif mem_val == 'jemalloc':
            jemalloc_val = 'bundled'
        else:
            jemalloc_val = 'none'
    elif flag == 'host':
        if chpl_host_jemalloc:
            jemalloc_val = chpl_host_jemalloc
        elif linux and mem_val == 'jemalloc':
            jemalloc_val = 'bundled'
        elif darwin and mem_val == 'jemalloc':
            jemalloc_val = 'system'
        else:
            jemalloc_val = 'none'
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if flag == 'host':
        if linux and mem_val == 'jemalloc' and jemalloc_val == 'system':
            error("CHPL_HOST_JEMALLOC=system is not supported on Linux for host builds")

        if darwin and mem_val == 'jemalloc' and jemalloc_val == 'bundled':
            error("CHPL_HOST_JEMALLOC=bundled is not supported on Mac for host builds")

    if mem_val == 'jemalloc' and jemalloc_val == 'none':
        error("CHPL_JEMALLOC must not be 'none' when CHPL_MEM is jemalloc")

    if mem_val != 'jemalloc' and jemalloc_val != 'none':
        error("CHPL_JEMALLOC must be 'none' when CHPL_MEM is not jemalloc")

    return jemalloc_val


@memoize
def get_uniq_cfg_path(flag):
    # uses host/ or target/ before the usual subdir
    # host subdir computed here since third_party_utils doesn't have a default
    if flag == 'host':
        host_bin_subdir = chpl_bin_subdir.get('host')
        host_compiler = chpl_compiler.get('host')
        host_info = '{0}-{1}'.format(host_bin_subdir, host_compiler)
        return os.path.join('host', host_info)
    else:
        # add target/ before the usual subdir
        return os.path.join('target', third_party_utils.default_uniq_cfg_path())


# Instead of libtool or pkg-config, jemalloc uses a jemalloc-config script to
# determine dependencies/link args . It's located in the bin directory
@memoize
def get_jemalloc_config_file(flag):
    ucp = get_uniq_cfg_path(flag)
    install_path = third_party_utils.get_bundled_install_path('jemalloc',
                                                              ucp=ucp)
    config_file = os.path.join(install_path, 'bin', 'jemalloc-config')
    return config_file

# flag is host or target
# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args(flag):
    jemalloc_val = get(flag)
    if jemalloc_val == 'bundled':
        ucp_val = get_uniq_cfg_path(flag)
        return third_party_utils.get_bundled_compile_args('jemalloc',
                                                          ucp=ucp_val)
    return ([ ], [ ])

# flag is host or target
# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args(flag):
    jemalloc_val = get(flag)
    if jemalloc_val == 'bundled':
        ucp = get_uniq_cfg_path(flag)
        jemalloc_config = get_jemalloc_config_file(flag)
        install_path = third_party_utils.get_bundled_install_path('jemalloc',
                                                                  ucp=ucp)
        lib_path = os.path.join(install_path, 'lib')
        libs = ['-L{}'.format(lib_path), '-ljemalloc']

        morelibs = [ ]
        # should this be an error if we can't find it?
        if os.access(jemalloc_config, os.X_OK):
            jemalloc_libs = run_command([jemalloc_config, '--libs'])
            morelibs += jemalloc_libs.split()
        return (libs, morelibs)

    if jemalloc_val == 'system':
        return ([ ], ['-ljemalloc'])

    return ([ ], [ ])

def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    (options, args) = parser.parse_args()

    jemalloc_val = get(options.flag)
    sys.stdout.write("{0}\n".format(jemalloc_val))


if __name__ == '__main__':
    _main()
