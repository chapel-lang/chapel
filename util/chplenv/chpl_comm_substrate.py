#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_platform, chpl_comm
from utils import memoize

@memoize
def get():
    substrate_val = os.environ.get('CHPL_COMM_SUBSTRATE')
    if not substrate_val:
        comm_val = chpl_comm.get()
        platform_val = chpl_platform.get('target')
        arch_val = chpl_arch.get('target', get_lcd=True)

        if comm_val == 'gasnet':
            if platform_val == 'cray-xt':
                substrate_val = 'mpi'
            elif platform_val == 'cray-xe':
                substrate_val = 'gemini'
            elif platform_val == 'cray-xk':
                substrate_val = 'gemini'
            elif platform_val == 'cray-xc':
                if arch_val == 'knc':
                    substrate_val = 'mpi'
                else:
                    substrate_val = 'aries'
            elif platform_val == 'marenostrum':
                substrate_val = 'udp'
            elif platform_val == 'pwr5':
                substrate_val = 'lapi'
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
