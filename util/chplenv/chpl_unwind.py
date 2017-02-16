#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform, overrides
from utils import error, memoize

@memoize
def get():
    platform_val = chpl_platform.get('target')
    linux = platform_val.startswith('linux64')
    osx = platform_val.startswith('darwin')
    val = overrides.get('CHPL_UNWIND')

    if linux:
        if val == 'libunwind':
            return 'libunwind'
        elif val == 'system':
            return 'system'
    if osx:
        if val == 'libunwind':
            error("Using CHPL_UNWIND=libunwind is not supported on Mac OS X."
                  "\nUse CHPL_UNWIND=system instead.", ValueError)
        elif val == 'system':
            return 'system'
    return 'none'

def _main():
    unwind_val = get()
    sys.stdout.write("{0}\n".format(unwind_val))


if __name__ == '__main__':
    _main()
