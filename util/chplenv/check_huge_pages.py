#!/usr/bin/env python
import os
from sys import stdout, stderr

import chpl_platform, chpl_comm, chpl_comm_substrate
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
        elif comm_val == 'ugni' and not is_using_hugepages:
            stderr.write("Warning: a craype-hugepages module must be loaded when using "
                         "the {0} communication layer, or link errors will "
                         "result\n".format(comm_val))


if __name__ == '__main__':
    check()
