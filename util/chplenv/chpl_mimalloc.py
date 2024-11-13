#!/usr/bin/env python3
import os
import sys
import optparse

import chpl_mem, overrides, third_party_utils, chpl_bin_subdir, chpl_compiler
from utils import error, memoize, warning


@memoize
def get(flag='target'):

    var = 'CHPL_{0}_MIMALLOC'.format(flag.upper())
    mimalloc = 'none'
    if flag in ('target', 'host'):
        # mimalloc is only used if explicitly requested
        mimalloc = overrides.get(var)

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
                error("{0} must not be 'none' when CHPL_TARGET_MEM is 'mimalloc'".format(var, memvar))

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    supported_mimalloc = ('none', 'bundled', 'system')
    if mimalloc not in supported_mimalloc:
        error("{0}={1} is not supported, must be one of {2}".format(var, mimalloc, supported_mimalloc))

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
    # cmake handles 'host' mimalloc
    if flag == 'host':
        return [], []

    mimalloc_val = get(flag)
    if mimalloc_val == 'bundled':
        bundled, system = [], []
        if flag == 'target':
            bundled, system = third_party_utils.get_bundled_compile_args(
                "mimalloc", ucp=get_uniq_cfg_path(flag)
            )
        # if flag == 'host':
        #     bundled += ["-include", "mimalloc-override.h"]
        return bundled, system
    elif mimalloc_val == 'system':
        warning("System mimalloc is not supported yet")
        pass
    return [], []

# flag is host or target
# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args(flag):
    # cmake handles 'host' mimalloc
    if flag == 'host':
        return [], []

    mimalloc_val = get(flag)
    if mimalloc_val == 'bundled':
        if flag == 'host':
            # # use the .o file for the host
            # install_dir = third_party_utils.get_bundled_install_path('mimalloc', ucp=get_uniq_cfg_path(flag))
            # lib_dir = os.path.join(install_dir, 'lib')
            # args = ([os.path.join(lib_dir, "mimalloc.o")], [])
            args = [],[]
        else:
            args = third_party_utils.pkgconfig_get_bundled_link_args(
                'mimalloc', ucp=get_uniq_cfg_path(flag)
            )
        return args
    elif mimalloc_val == 'system':
        warning("System mimalloc is not supported yet")
        pass
    return ([ ], [ ])

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
