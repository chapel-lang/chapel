#!/usr/bin/env python3
import sys
import optparse

import chpl_platform, overrides, third_party_utils
from utils import error, memoize, warning, check_valid_var


@memoize
def system_library_available():
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    if osx:
        return True
    else:
        return third_party_utils.pkgconfig_system_has_package('libunwind')

@memoize
def get():
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')
    val = overrides.get('CHPL_UNWIND', None)

    if val is None:
        if system_library_available():
            val = 'system'
        else:
            val = 'bundled'

    if osx and val == 'bundled':
        error("Using CHPL_UNWIND=bundled is not supported on Mac OS X."
              "\nUse CHPL_UNWIND=system instead.", ValueError)

    check_valid_var('CHPL_UNWIND', val, ['none', 'bundled', 'system'])
    return val

@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    unwind_val = get()
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    args = ([ ], [ ])

    # Mac OS X supports libunwind in the C library
    # it's not actually a special library.
    if osx:
      return args

    if unwind_val == 'bundled':
        return third_party_utils.get_bundled_compile_args('libunwind')
    elif unwind_val == 'system' :
        args = third_party_utils.pkgconfig_get_system_compile_args('libunwind')
    return args

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
        args = third_party_utils.pkgconfig_get_system_link_args('libunwind', static=True)
    return args


def _main():
    parser = optparse.OptionParser()
    parser.add_option('--quickstart', dest='action',
                      action='store_const',
                      const='quickstart', default='')
    (options, args) = parser.parse_args()

    if options.action == 'quickstart':
        if system_library_available():
            sys.stdout.write("system\n")
        else:
            sys.stdout.write("none\n")
    else:
        unwind_val = get()
        sys.stdout.write("{0}\n".format(unwind_val))


if __name__ == '__main__':
    _main()
