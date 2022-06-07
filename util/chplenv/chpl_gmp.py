#!/usr/bin/env python3
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize, warning

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
        elif target_platform == 'aarch64':
            gmp_val = 'system'
        else:
            gmp_val = 'none'

    return gmp_val


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

    return ([ ], [ ])

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    gmp_val = get()
    if gmp_val == 'bundled':
         return third_party_utils.pkgconfig_get_bundled_link_args('gmp')

    elif gmp_val == 'system':
        return ([ ], ['-lgmp'])

    return ([ ], [ ])

def _main():
    gmp_val = get()
    sys.stdout.write("{0}\n".format(gmp_val))


if __name__ == '__main__':
    _main()
