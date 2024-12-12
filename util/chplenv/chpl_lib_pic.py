#!/usr/bin/env python3
import sys

import overrides
import chpl_platform
from utils import memoize, check_valid_var

@memoize
def get():
    lib_pic_val = overrides.get('CHPL_LIB_PIC', 'none')
    check_valid_var("CHPL_LIB_PIC", lib_pic_val, ["none", "pic"])
    return lib_pic_val


def _main():
    lib_pic_val = get()
    sys.stdout.write("{0}\n".format(lib_pic_val))


if __name__ == '__main__':
    _main()
