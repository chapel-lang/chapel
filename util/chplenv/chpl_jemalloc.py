#!/usr/bin/env python
import sys

import chpl_mem, overrides
from utils import error, memoize


@memoize
def get():
    jemalloc_val = overrides.get('CHPL_JEMALLOC')
    mem_val = chpl_mem.get('target')
    if not jemalloc_val:
        if mem_val == 'jemalloc':
            jemalloc_val = 'jemalloc'
        else:
            jemalloc_val = 'none'

    if mem_val == 'jemalloc' and jemalloc_val == 'none':
      error("CHPL_JEMALLOC must not be 'none' when CHPL_MEM is jemalloc")

    if mem_val != 'jemalloc' and jemalloc_val != 'none':
      error("CHPL_JEMALLOC must be 'none' when CHPL_MEM is not jemalloc")

    return jemalloc_val


def _main():
    jemalloc_val = get()
    sys.stdout.write("{0}\n".format(jemalloc_val))


if __name__ == '__main__':
    _main()
