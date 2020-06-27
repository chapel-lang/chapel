#!/usr/bin/env python
import os
import sys

import chpl_mem, overrides, third_party_utils
from utils import error, memoize, run_command


@memoize
def get():
    jemalloc_val = overrides.get('CHPL_JEMALLOC')
    mem_val = chpl_mem.get('target')
    if not jemalloc_val:
        if mem_val == 'jemalloc':
            jemalloc_val = 'jemalloc'
        else:
            jemalloc_val = 'none'

    if mem_val == 'jemalloc' and jemalloc_val == 'none':
      error("CHPL_JEMALLOC must not be 'none' when CHPL_MEM is jemalloc")

    if mem_val != 'jemalloc' and jemalloc_val != 'none':
      error("CHPL_JEMALLOC must be 'none' when CHPL_MEM is not jemalloc")

    return jemalloc_val


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()


# Instead of libtool or pkg-config, jemalloc uses a jemalloc-config script to
# determine dependencies/link args . It's located in the bin directory
@memoize
def get_jemalloc_config_file():
    install_path = third_party_utils.get_cfg_install_path('jemalloc')
    config_file = os.path.join(install_path, 'bin', 'jemalloc-config')
    return config_file


@memoize
def get_link_args(target_mem):
    if target_mem == 'jemalloc':
        jemalloc_config = get_jemalloc_config_file()
        libs = ['-ljemalloc']
        if os.access(jemalloc_config, os.X_OK):
            jemalloc_libs = run_command([jemalloc_config, '--libs'])
            libs += jemalloc_libs.split()
        return libs
    elif target_mem == 'system':
        return ['-ljemalloc']
    else:
        return []


def _main():
    jemalloc_val = get()
    sys.stdout.write("{0}\n".format(jemalloc_val))


if __name__ == '__main__':
    _main()
