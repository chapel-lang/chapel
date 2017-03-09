#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform, overrides
from chpl_home_utils import using_chapel_module
from utils import memoize


@memoize
def get():
    comm_val = overrides.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        # Use ugni on cray-x* series
        if platform_val.startswith('cray-x'):
            comm_val = 'ugni'
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
