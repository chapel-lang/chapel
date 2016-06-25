#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform
from utils import memoize

@memoize
def get():
    platform_val = chpl_platform.get('target')
    linux = platform_val.startswith('linux64')
    osx = platform_val.startswith('darwin')
    val = os.environ.get('CHPL_UNWIND')
    if val == 'libunwind' and linux:
        return 'libunwind'
    if val == 'system' and (osx or linux):
        return 'system'
    return 'none'


def _main():
    unwind_val = get()
    sys.stdout.write("{0}\n".format(unwind_val))


if __name__ == '__main__':
    _main()
