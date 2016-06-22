#!/usr/bin/env python
import os
import sys
from sys import stderr, stdout

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_comm, chpl_comm_substrate, chpl_platform
from utils import memoize


# this one doesnt really need to cache anything, but it doesnt need to run more
# than once to print out any warnings
@memoize
def check():
    platform_val = chpl_platform.get('target')
    if platform_val.startswith('cray-x'):
        comm_val = chpl_comm.get()
        substrate_val = chpl_comm_substrate.get()
        is_using_hugepages = 'HUGETLB' in os.environ.get('PE_PRODUCT_LIST', '')

        if comm_val == 'gasnet' and is_using_hugepages:
            if substrate_val == 'gemini' or substrate_val == 'aries':
                stderr.write("Warning: compiling with the craype-hugepages module while "
                             "using the GASNet {0} conduit is not supported and "
                             "will lead to link errors\n".format(substrate_val))


if __name__ == '__main__':
    check()
