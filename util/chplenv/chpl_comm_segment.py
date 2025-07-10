#!/usr/bin/env python3
import sys

import chpl_comm, chpl_comm_substrate, chpl_platform, overrides
from utils import memoize, check_valid_var, error


@memoize
def get():
    comm_val = chpl_comm.get()
    if comm_val == 'gasnet':
        segment_val = overrides.get('CHPL_GASNET_SEGMENT')
        if not segment_val:
            substrate_val = chpl_comm_substrate.get()
            if substrate_val in ('smp', 'ucx'):
                segment_val = 'fast'
            elif substrate_val == 'ofi' and chpl_platform.is_hpe_cray('target'):
                segment_val = 'fast'
            elif substrate_val == 'ibv':
                segment_val = 'large'
            else:
                segment_val = 'everything'
        check_valid_var("CHPL_GASNET_SEGMENT", segment_val, ("fast", "large", "everything"))
    else:
        segment_val = 'none'

    substrate_val = chpl_comm_substrate.get()
    if segment_val == "everything" and substrate_val in ("smp", "ucx"):
        error(
            "CHPL_GASNET_SEGMENT=everything is not supported with "
            + "CHPL_COMM_SUBSTRATE={0}. Please use ".format(substrate_val)
            + "CHPL_GASNET_SEGMENT=fast or CHPL_GASNET_SEGMENT=large instead"
        )

    return segment_val


def _main():
    segment_val = get()
    sys.stdout.write("{0}\n".format(segment_val))


if __name__ == '__main__':
    _main()
