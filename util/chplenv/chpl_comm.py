#!/usr/bin/env python
import sys, os

import chpl_platform
from utils import memoize

@memoize
def get():
    comm_val = os.environ.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        # automatically uses gasnet when on a cray-x* or cray-cs machine
        if platform_val.startswith('cray-'):
            comm_val = 'gasnet'
        else:
            comm_val = 'none'
    return comm_val


def _main():
    comm_val = get()
    sys.stdout.write("{0}\n".format(comm_val))


if __name__ == '__main__':
    _main()
