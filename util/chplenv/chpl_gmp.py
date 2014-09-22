#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_compiler, chpl_platform, utils
from utils import memoize

@memoize
def get():
    gmp_val = os.environ.get('CHPL_GMP')
    if not gmp_val:
        target_platform = chpl_platform.get('target')
        target_compiler = chpl_compiler.get('target')
        target_arch = chpl_arch.get('target', map_to_compiler=True, get_lcd=True)

        # Detect if gmp has been built for this configuration.
        chpl_home = utils.get_chpl_home()
        gmp_target_dir = '{0}-{1}-{2}'.format(target_platform, target_compiler, target_arch)
        gmp_subdir = os.path.join(chpl_home, 'third-party', 'gmp',
                                  'install', gmp_target_dir)

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
