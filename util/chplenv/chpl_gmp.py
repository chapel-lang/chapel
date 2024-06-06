#!/usr/bin/env python3
import os
import sys
import optparse

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize, warning, error

# returns True if CHPL_GMP was set by the user
# (i.e. not inferred to be the default)
@memoize
def is_overridden():
    gmp_val = overrides.get('CHPL_GMP')

    if gmp_val:
        return True

    return False

@memoize
def get():
    gmp_val = overrides.get('CHPL_GMP')
    if not gmp_val:
        target_platform = chpl_platform.get('target')

        # Detect if gmp has been built for this configuration.
        third_party = get_chpl_third_party()
        uniq_cfg_path = get_uniq_cfg_path()
        gmp_subdir = os.path.join(third_party, 'gmp', 'install', uniq_cfg_path)

        if os.path.exists(os.path.join(gmp_subdir, 'include', 'gmp.h')):
            gmp_val = 'bundled'
        elif target_platform.startswith('cray-x'):
            gmp_val = 'system'
        else:
            gmp_val = 'none'

    return gmp_val


@memoize
def has_system_gmp_install():
    # try pkg-config
    if third_party_utils.pkgconfig_has_system_package('gmp'):
        return True
    # try homebrew
    if chpl_platform.get_homebrew_prefix('gmp'):
        return True
    return False

@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    gmp_val = get()
    if gmp_val == 'bundled':
         return third_party_utils.get_bundled_compile_args('gmp')
    elif gmp_val == 'system':
        # try pkg-config
        args = third_party_utils.pkgconfig_get_system_compile_args('gmp')
        if args != (None, None):
            return args
        # try homebrew
        gmp_prefix = chpl_platform.get_homebrew_prefix('gmp')
        if gmp_prefix:
            return ([], ['-I{0}'.format(os.path.join(gmp_prefix, 'include'))])

        error("Could not find a suitable GMP installation.  Please install GMP or set CHPL_GMP=bundled or CHPL_GMP=none.")

    return ([ ], [ ])

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    gmp_val = get()
    if gmp_val == 'bundled':
         return third_party_utils.pkgconfig_get_bundled_link_args('gmp')

    elif gmp_val == 'system':
        # try pkg-config
        args = third_party_utils.pkgconfig_get_system_link_args('gmp')
        if args != (None, None):
            return args
        # try homebrew
        gmp_prefix = chpl_platform.get_homebrew_prefix('gmp')
        if gmp_prefix:
            return ([], ['-L{0}'.format(os.path.join(gmp_prefix, 'lib')),
                         '-lgmp'])

        error("Could not find a suitable GMP installation. Please install GMP or set CHPL_GMP=bundled or CHPL_GMP=none.")

    return ([ ], [ ])


def _main():
    gmp_val = get()

    parser = optparse.OptionParser(usage='usage: %prog [--prefix] [--compile] [--link]')
    parser.add_option('--compile', dest='action',
                      action='store_const',
                      const='compile', default='')
    parser.add_option('--link', dest='action',
                      action='store_const',
                      const='link', default='')

    (options, args) = parser.parse_args()

    if options.action == 'compile':
        sys.stdout.write("{0}\n".format(get_compile_args()))
    elif options.action == 'link':
        sys.stdout.write("{0}\n".format(get_link_args()))
    else:
        sys.stdout.write("{0}\n".format(gmp_val))


if __name__ == '__main__':
    _main()
