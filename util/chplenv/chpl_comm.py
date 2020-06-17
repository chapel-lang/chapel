#!/usr/bin/env python
import sys

import chpl_platform, overrides
from utils import memoize


@memoize
def get():
    comm_val = overrides.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        # Use ugni on cray-xc series
        if platform_val == 'cray-xc':
            comm_val = 'ugni'
        # Use ofi on cray-shasta
        elif platform_val == 'cray-shasta':
            comm_val = 'ofi'
        # Use gasnet on cray-cs
        elif platform_val.startswith('cray-'):
            comm_val = 'gasnet'
        else:
            comm_val = 'none'
    return comm_val


def _main():
    comm_val = get()
    sys.stdout.write("{0}\n".format(comm_val))


if __name__ == '__main__':
    _main()
