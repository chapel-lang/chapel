#!/usr/bin/env python3
import sys

import chpl_comm, chpl_comm_substrate, overrides
from utils import memoize


@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val == 'gasnet':
        segment_val = overrides.get('CHPL_GASNET_SEGMENT')
        if not segment_val:
            substrate_val = chpl_comm_substrate.get()
            if substrate_val in ('aries', 'smp', 'ucx'):
                segment_val = 'fast'
            elif substrate_val == 'ibv':
                segment_val = 'large'
            else:
                segment_val = 'everything'
    else:
        segment_val = 'none'
    return segment_val


def _main():
    segment_val = get()
    sys.stdout.write("{0}\n".format(segment_val))


if __name__ == '__main__':
    _main()
