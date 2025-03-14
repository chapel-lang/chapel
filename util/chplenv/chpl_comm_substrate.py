#!/usr/bin/env python3
import sys
import shutil

import chpl_comm, overrides
from utils import memoize


@memoize
def get():
    substrate_val = overrides.get('CHPL_COMM_SUBSTRATE')
    if not substrate_val:
        comm_val = chpl_comm.get()

        if comm_val == 'gasnet':
            network = chpl_comm.get_network()
            if network == 'aries':
                substrate_val = 'aries'
            elif network == 'ibv':
                substrate_val = 'ibv'
            elif network == 'cxi':
                substrate_val = 'ofi'
            else:
                import chpl_platform
                platform_val = chpl_platform.get('target')
                if platform_val == 'cray-xc':
                    substrate_val = 'aries'
                elif platform_val == 'hpe-cray-ex':
                    substrate_val = 'ofi'
                elif platform_val == 'hpe-cray-xd':
                    # default to ofi on XD when we can't determine ib vs cxi
                    substrate_val = 'ofi'
                elif platform_val in ('cray-cs', 'hpe-apollo'):
                    substrate_val = 'ibv'
                elif platform_val == 'pwr6':
                    substrate_val = 'ibv'
                else:
                    substrate_val = 'udp'
        else:
            substrate_val = 'none'
    return substrate_val


def _main():
    substrate_val = get()
    sys.stdout.write("{0}\n".format(substrate_val))


if __name__ == '__main__':
    _main()
