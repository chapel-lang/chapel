#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_arch, chpl_comm, chpl_platform, overrides
from utils import memoize


@memoize
def get():
    substrate_val = overrides.get('CHPL_COMM_SUBSTRATE')
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
