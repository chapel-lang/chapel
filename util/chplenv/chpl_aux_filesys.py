#!/usr/bin/env python3
import sys
import os
from glob import glob

import overrides
from utils import memoize


@memoize
def get():
    aux_fs = overrides.get('CHPL_AUX_FILESYS', 'none')
    return aux_fs


def _main():
    aux_fs_val = get()
    sys.stdout.write("{0}\n".format(aux_fs_val))


if __name__ == '__main__':
    _main()
