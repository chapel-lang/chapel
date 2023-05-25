#!/usr/bin/env python3
import sys

import chpl_platform, overrides, third_party_utils
from utils import error, memoize, warning


@memoize
def get():
    platform_val = chpl_platform.get('target')
    linux = platform_val.startswith('linux64')
    osx = platform_val.startswith('darwin')
    val = overrides.get('CHPL_UNWIND')

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

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    unwind_val = get()
    if unwind_val == 'bundled':
         return third_party_utils.get_bundled_compile_args('libunwind')

    return ([ ], [ ])

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    unwind_val = get()
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    args = ([ ], [ ])

    # Mac OS X supports libunwind in the C library
    # it's not actually a special library.
    if osx:
      return args

    # Get the link arguments (e.g. -lunwind)
    if unwind_val == 'bundled':
      args = third_party_utils.pkgconfig_get_bundled_link_args('libunwind')

    elif unwind_val == 'system':
      # Try using pkg-config to get the libraries to link
      # libunwind with.
      args = third_party_utils.pkgconfig_get_system_link_args('libunwind',
                                                              static=True)

    if unwind_val == 'system' or unwind_val == 'bundled':
        # add -ldl so that we can call dladdr
        if "-ldl" not in args[1]:
            args[1].append("-ldl")

    return args


def _main():
    unwind_val = get()
    sys.stdout.write("{0}\n".format(unwind_val))


if __name__ == '__main__':
    _main()
