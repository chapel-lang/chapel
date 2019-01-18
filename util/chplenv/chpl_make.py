#!/usr/bin/env python
from distutils.spawn import find_executable
import sys

import chpl_platform, overrides
from utils import memoize


@memoize
def get():
    make_val = overrides.get('CHPL_MAKE')
    if not make_val:
        platform_val = chpl_platform.get()
        if platform_val.startswith('cygwin') or platform_val == 'darwin':
            make_val = 'make'
        elif platform_val.startswith('linux'):
            if find_executable('gmake'):
                make_val = 'gmake'
            else:
                make_val = 'make'
        else:
            make_val = 'gmake'
    return make_val


def _main():
    make_val = get()
    sys.stdout.write("{0}\n".format(make_val))


if __name__ == '__main__':
    _main()
