#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_compiler, chpl_platform, utils
from utils import memoize
import chpl_3p_gmp_configs

@memoize
def get():
    gmp_val = os.environ.get('CHPL_GMP')
    if not gmp_val:
        target_platform = chpl_platform.get('target')

        # Detect if gmp has been built for this configuration.
        chpl_home = utils.get_chpl_home()
        uniq_cfg_path = chpl_3p_gmp_configs.get_uniq_cfg_path()
        gmp_subdir = os.path.join(chpl_home, 'third-party', 'gmp',
                                  'install', uniq_cfg_path)

        if os.path.exists(os.path.join(gmp_subdir, 'include', 'gmp.h')):
            gmp_val = 'gmp'
        elif target_platform.startswith('cray-x'):
            gmp_val = 'system'
        else:
            gmp_val = 'none'
    return gmp_val


def _main():
    gmp_val = get()
    sys.stdout.write("{0}\n".format(gmp_val))


if __name__ == '__main__':
    _main()
