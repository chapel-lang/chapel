#!/usr/bin/env python3
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize


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
    elif gmp_val == 'gmp':
        sys.stderr.write("Warning: CHPL_GMP=gmp is deprecated. "
                         "Use CHPL_GMP=bundled instead.\n")
        gmp_val = 'bundled'

    return gmp_val


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()


@memoize
def get_link_args(gmp):
    if gmp == 'bundled':
        return third_party_utils.default_get_link_args('gmp')
    elif gmp == 'system':
        return ['-lgmp']
    return []


def _main():
    gmp_val = get()
    sys.stdout.write("{0}\n".format(gmp_val))


if __name__ == '__main__':
    _main()
