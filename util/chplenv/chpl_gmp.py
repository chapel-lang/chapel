#!/usr/bin/env python
import os
import sys

import chpl_3p_gmp_configs, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party
from utils import memoize


@memoize
def get():
    gmp_val = overrides.get('CHPL_GMP')
    if not gmp_val:
        target_compiler = chpl_compiler.get('target')
        if target_compiler == 'cray-prgenv-cray':
            gmp_val = 'system'
        else:
            target_platform = chpl_platform.get('target')

            # Detect if gmp has been built for this configuration.
            third_party = get_chpl_third_party()
            uniq_cfg_path = chpl_3p_gmp_configs.get_uniq_cfg_path()
            gmp_subdir = os.path.join(third_party, 'gmp', 'install', uniq_cfg_path)

            if os.path.exists(os.path.join(gmp_subdir, 'include', 'gmp.h')):
                gmp_val = 'gmp'
            elif target_platform.startswith('cray-x'):
                gmp_val = 'system'
            elif target_platform == 'aarch64':
                gmp_val = 'system'
            else:
                gmp_val = 'none'
    return gmp_val


def _main():
    gmp_val = get()
    sys.stdout.write("{0}\n".format(gmp_val))


if __name__ == '__main__':
    _main()
