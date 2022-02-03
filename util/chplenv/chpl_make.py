#!/usr/bin/env python3

import sys

try:
    # Module `distutils` is deprecated in Python 3.10 and will be removed in Python 3.12
    # Prefer `shutil.which` in Python 3.2+
    from shutil import which
except ImportError:
    # Backport for pre Python 3.2
    from distutils.spawn import find_executable as which

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
            if which('gmake'):
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
