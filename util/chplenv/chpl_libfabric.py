#!/usr/bin/env python
import sys

import chpl_comm, overrides
from utils import error, memoize


@memoize
def get():
    libfabric_val = overrides.get('CHPL_LIBFABRIC')
    comm_val = chpl_comm.get()
    if not libfabric_val:
        if comm_val == 'ofi':
            libfabric_val = 'system'
        else:
            libfabric_val = 'none'

    if comm_val == 'ofi' and libfabric_val == 'none':
      error("CHPL_LIBFABRIC must not be 'none' when CHPL_COMM is ofi")

    if comm_val != 'ofi' and libfabric_val != 'none':
      error("CHPL_LIBFABRIC must be 'none' when CHPL_COMM is not ofi")

    if libfabric_val == 'libfabric':
        error("Using CHPL_LIBFABRIC=libfabric is not yet supported."
              "\nUse CHPL_LIBFABRIC=system instead.", ValueError)
    return libfabric_val


def _main():
    libfabric_val = get()
    sys.stdout.write("{0}\n".format(libfabric_val))


if __name__ == '__main__':
    _main()
