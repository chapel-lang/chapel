#!/usr/bin/env python3
import sys

import chpl_platform, overrides, third_party_utils
from utils import error, memoize


@memoize
def get():
    platform_val = chpl_platform.get('target')
    linux = platform_val.startswith('linux64')
    osx = platform_val.startswith('darwin')
    val = overrides.get('CHPL_UNWIND')

    if val == 'libunwind':
        sys.stderr.write("Warning: CHPL_UNWIND=libunwind is deprecated. "
                         "Use CHPL_UNWIND=bundled instead\n")
        val = 'bundled'

    if linux:
        if val == 'bundled':
            return 'bundled'
        elif val == 'system':
            return 'system'
    if osx:
        if val == 'bundled':
            error("Using CHPL_UNWIND=bundled is not supported on Mac OS X."
                  "\nUse CHPL_UNWIND=system instead.", ValueError)
        elif val == 'system':
            return 'system'
    return 'none'


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()


@memoize
def get_link_args(unwind):
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    # Mac OS X supports libunwind in the C library
    # it's not actually a special library.
    if osx:
      return []

    libs = []
    # Get the link arguments (e.g. -lunwind)
    if unwind == 'system':
      # Try using pkg-config to get the libraries to link
      # libunwind with.
      libs = third_party_utils.pkgconfig_get_link_args(
                       'libunwind', system=True, static=True)
    elif unwind == 'bundled':
      # the pkg-config file for libunwind is nice, but as of 1.1
      # it doesn't include -lzma when it probably should.
      # So try to get the libraries out of libunwind.la.
      libs = third_party_utils.default_get_link_args(
                       'libunwind', libs=['libunwind.la', 'libunwind-x86_64.la'])

    # add -ldl so that we can call dladdr
    if "-ldl" not in libs:
        libs.append("-ldl")

    return libs


def _main():
    unwind_val = get()
    sys.stdout.write("{0}\n".format(unwind_val))


if __name__ == '__main__':
    _main()
