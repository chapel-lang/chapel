#!/usr/bin/env python3
import sys
import shutil

import chpl_platform, overrides
from utils import memoize


@memoize
def get():
    comm_val = overrides.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        # use gasnet for ibv
        if shutil.which('ibstat'):
            comm_val = 'gasnet'
        # use ofi for slingshot
        elif shutil.which('cxi_stat'):
            comm_val = 'ofi'
        # Use ugni on cray-xc series
        elif platform_val == 'cray-xc':
            comm_val = 'ugni'
        # Use ofi on hpe-cray-ex
        elif platform_val == 'hpe-cray-ex':
            comm_val = 'ofi'
        # Use gasnet on cray-cs and hpe-apollo
        elif platform_val in ('cray-cs', 'hpe-apollo'):
            comm_val = 'gasnet'
        # default to ofi on cray-xd when we can't determine ib vs cxi
        elif platform_val == 'hpe-cray-xd':
            comm_val = 'ofi'
        else:
            comm_val = 'none'

    return comm_val


def _main():
    comm_val = get()
    sys.stdout.write("{0}\n".format(comm_val))


if __name__ == '__main__':
    _main()
