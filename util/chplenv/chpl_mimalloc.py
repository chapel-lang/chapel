#!/usr/bin/env python3
import os
import sys
import optparse

import chpl_mem, overrides, third_party_utils, chpl_bin_subdir, chpl_compiler, chpl_platform
from utils import error, memoize, warning, check_valid_var


def var_name(flag):
    return 'CHPL_{0}_MIMALLOC'.format(flag.upper())

@memoize
def get(flag='target'):

    mimalloc = 'none'
    if flag in ('target', 'host'):
        # mimalloc is only used if explicitly requested
        mimalloc = overrides.get(var_name(flag))

        mem = chpl_mem.get(flag)
        if mimalloc is None:
            # if its none, we set it based on chpl_mem
            if mem == 'mimalloc':
                mimalloc = 'bundled' # default to a bundled mimalloc
            else:
                mimalloc = 'none'
        else:
            # if mem is mimalloc, then mimalloc cannot be none
            if mem == 'mimalloc' and mimalloc == 'none':
                memvar = 'CHPL_{}_MEM'.format(flag.upper())
                error("{0} must not be 'none' when {1} is 'mimalloc'".format(var_name(flag), memvar))

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    # if mimalloc is system, check if we can find it with pkg-config
    # cmake handles the host case already, but we can provide better error messages here
    if mimalloc == 'system' and not third_party_utils.pkgconfig_system_has_package("mimalloc"):
        third_party_utils.could_not_find_pkgconfig_pkg("mimalloc", var_name(flag))

    check_valid_var(var_name(flag), mimalloc, ('none', 'bundled', 'system'))
    return mimalloc


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


# flag is host or target
# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args(flag):
    # cmake handles 'host' mimalloc args
    if flag == 'host':
        return [], []

    mimalloc_val = get(flag)
    if mimalloc_val == 'bundled':
        return third_party_utils.get_bundled_compile_args("mimalloc", ucp=get_uniq_cfg_path(flag))
    elif mimalloc_val == 'system':
        # try pkg-config
        args = third_party_utils.pkgconfig_get_system_compile_args("mimalloc")
        if args != (None, None):
            return args
        else:
            third_party_utils.could_not_find_pkgconfig_pkg("mimalloc", var_name(flag))
    return [], []

# flag is host or target
# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args(flag):
    # cmake handles 'host' mimalloc args for the compiler,
    # but we still need it for chapel-py on MacOS (because it is dynamic linked)
    if flag == 'host' and chpl_platform.get() != "darwin":
        return [], []

    mimalloc_val = get(flag)
    if mimalloc_val == 'bundled':
        return third_party_utils.pkgconfig_get_bundled_link_args("mimalloc",ucp=get_uniq_cfg_path(flag))
    elif mimalloc_val == 'system':
        # try pkg-config
        args = third_party_utils.pkgconfig_get_system_link_args("mimalloc")
        if args != (None, None):
            return args
        else:
            third_party_utils.could_not_find_pkgconfig_pkg("mimalloc", var_name(flag))
    return [] , []

def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const', const='host')
    (options, args) = parser.parse_args()

    mimalloc_val = get(options.flag)
    sys.stdout.write("{0}\n".format(mimalloc_val))


if __name__ == '__main__':
    _main()
