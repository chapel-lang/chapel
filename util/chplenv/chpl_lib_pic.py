#!/usr/bin/env python3
import sys
import platform

import overrides
from utils import memoize


@memoize
def get():
    lib_pic_val = overrides.get('CHPL_LIB_PIC')
    if not lib_pic_val:
        # Some platforms have 'gcc' and/or standard libraries
        # default to PIC. For those platforms, arrange for CHPL_LIB_PIC
        # to defaurt to 'pic' instead of 'none' to avoid problems.
        #
        # Another option would be to check for --enable-default-pie in
        #   gcc -v -E
        # However, that would assume that 'gcc' is related to the link command,
        # which is not necessarily the case.
        release = platform.uname()[2]
        if 'arch' in release:
            lib_pic_val = 'pic'
        else:
            lib_pic_val = 'none'

    return lib_pic_val


def _main():
    lib_pic_val = get()
    sys.stdout.write("{0}\n".format(lib_pic_val))


if __name__ == '__main__':
    _main()
