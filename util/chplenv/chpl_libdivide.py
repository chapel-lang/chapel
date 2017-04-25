#!/usr/bin/env python
import sys
import os
from sys import stdout

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import overrides
from utils import memoize

@memoize
def get():
    libdivide = overrides.get('CHPL_LIBDIVIDE', 'none')
    return libdivide


def _main():
    libdivide_val = get()
    stdout.write("{0}\n".format(libdivide_val))


if __name__ == '__main__':
    _main()
