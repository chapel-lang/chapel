#!/usr/bin/env python3
import sys

import overrides
from utils import memoize


@memoize
def get():
    lib_pic_val = overrides.get('CHPL_LIB_PIC')
    if not lib_pic_val:
        lib_pic_val = 'none'
    return lib_pic_val


def _main():
    lib_pic_val = get()
    sys.stdout.write("{0}\n".format(lib_pic_val))


if __name__ == '__main__':
    _main()
