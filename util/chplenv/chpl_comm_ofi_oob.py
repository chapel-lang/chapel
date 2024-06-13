#!/usr/bin/env python3
import sys

import chpl_comm, chpl_launcher, chpl_platform
import overrides

from utils import error, memoize

@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val != 'ofi':
        return 'none'

    oob_val = overrides.get('CHPL_COMM_OFI_OOB')
    if oob_val:
        if oob_val not in ('mpi', 'pmi2', 'sockets'):
            error("CHPL_COMM_OFI_OOB must be 'mpi', 'pmi2', or 'sockets'")
        return oob_val

    #
    # By default, use PMI2 out-of-band support on Cray X* and HPE Cray EX
    # systems, MPI on other Cray systems or with an MPI-based launcher,
    # and "sockets" otherwise.
    #
    platform_val = chpl_platform.get('target')
    launcher_val = chpl_launcher.get()
    if platform_val in ('cray-xc', 'hpe-cray-ex'):
        oob_val = 'pmi2'
    elif 'cray-' in platform_val:
        oob_val = 'mpi'
    elif 'mpi' in launcher_val:
        oob_val = 'mpi'
    else:
        oob_val = 'sockets'

    return oob_val

def _main():
    oob_val = get()
    sys.stdout.write("{0}\n".format(oob_val))


if __name__ == '__main__':
    _main()
