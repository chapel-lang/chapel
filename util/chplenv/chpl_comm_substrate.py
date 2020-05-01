#!/usr/bin/env python
import sys

import chpl_comm, chpl_platform, overrides
from utils import memoize


@memoize
def get():
    substrate_val = overrides.get('CHPL_COMM_SUBSTRATE')
    if not substrate_val:
        comm_val = chpl_comm.get()
        platform_val = chpl_platform.get('target')

        if comm_val == 'gasnet':
            if platform_val == 'cray-xc':
                substrate_val = 'aries'
            elif platform_val == 'cray-cs':
                substrate_val = 'ibv'
            elif platform_val == 'pwr6':
                substrate_val = 'ibv'
            else:
                substrate_val = 'udp'
        elif comm_val == 'ofi':
            if platform_val == 'cray-xc':
                substrate_val = 'sockets'
                # substrate_val = 'gni'
            else:
                substrate_val = 'sockets'
        else:
            substrate_val = 'none'
    return substrate_val


def _main():
    substrate_val = get()
    sys.stdout.write("{0}\n".format(substrate_val))


if __name__ == '__main__':
    _main()
