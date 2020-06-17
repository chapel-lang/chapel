#!/usr/bin/env python
import sys

import chpl_comm, overrides
from utils import error, memoize


@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val == 'ofi':
        libfabric_val = overrides.get('CHPL_LIBFABRIC')
        if not libfabric_val:
            libfabric_val = 'libfabric'
        if libfabric_val == 'none':
          error("CHPL_LIBFABRIC must not be 'none' when CHPL_COMM is ofi")
    else:
        libfabric_val = 'none'

    return libfabric_val


def _main():
    libfabric_val = get()
    sys.stdout.write("{0}\n".format(libfabric_val))


if __name__ == '__main__':
    _main()
