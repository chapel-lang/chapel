#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import overrides
from utils import memoize



@memoize
def get():
    libmode_val = overrides.get('CHPL_LIBMODE')
    if not libmode_val:
        libmode_val = 'none'
    return libmode_val


def _main():
    hwloc_val = get()
    sys.stdout.write("{0}\n".format(hwloc_val))


if __name__ == '__main__':
    _main()
