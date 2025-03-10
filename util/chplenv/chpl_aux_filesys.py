#!/usr/bin/env python3
import sys
import os
from glob import glob

import overrides
from utils import memoize, check_valid_var


@memoize
def get():
    aux_fs = overrides.get('CHPL_AUX_FILESYS', 'none')
    check_valid_var('CHPL_AUX_FILESYS', aux_fs, ['none', 'lustre'])
    return aux_fs


def _main():
    aux_fs_val = get()
    sys.stdout.write("{0}\n".format(aux_fs_val))


if __name__ == '__main__':
    _main()
