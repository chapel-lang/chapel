#!/usr/bin/env python3
import sys

import chpl_comm, chpl_platform, overrides
from utils import memoize, check_valid_var


@memoize
def get():
    substrate_val = overrides.get('CHPL_COMM_SUBSTRATE')
    if not substrate_val:
        comm_val = chpl_comm.get()
        platform_val = chpl_platform.get('target')

        if comm_val == 'gasnet':
            if platform_val == 'cray-xc':
                substrate_val = 'aries'
            elif platform_val == 'hpe-cray-ex':
                substrate_val = 'ofi'
            elif platform_val in ('cray-cs', 'hpe-apollo'):
                substrate_val = 'ibv'
            elif platform_val == 'pwr6':
                substrate_val = 'ibv'
            else:
                substrate_val = 'udp'
        else:
            substrate_val = 'none'

    check_valid_var("CHPL_COMM_SUBSTRATE", substrate_val, ("none", "aries", "ofi", "ibv", "udp", "mpi"))
    return substrate_val


def _main():
    substrate_val = get()
    sys.stdout.write("{0}\n".format(substrate_val))


if __name__ == '__main__':
    _main()
