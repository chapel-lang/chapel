#!/usr/bin/env python
import sys, os, optparse

import chpl_comm, chpl_comm_segment
from utils import memoize

@memoize
def get(flag='host'):
    if flag == 'host':
        mem_val = 'default'
    elif flag == 'target':
        mem_val = os.environ.get('CHPL_MEM')
        if not mem_val:
            comm_val = chpl_comm.get()
            if comm_val == 'gasnet':
                segment_val = chpl_comm_segment.get()
                if segment_val == 'fast' or segment_val == 'large':
                    mem_val = 'dlmalloc'
                else:
                    mem_val = 'default'
            elif comm_val == 'ugni':
                mem_val = 'tcmalloc'
            else:
                mem_val = 'default'
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))
    return mem_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    (options, args) = parser.parse_args()

    mem_val = get(options.flag)
    sys.stdout.write("{0}\n".format(mem_val))


if __name__ == '__main__':
    _main()
