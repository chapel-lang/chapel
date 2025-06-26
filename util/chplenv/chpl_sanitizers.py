#!/usr/bin/env python3
import optparse
import os
import sys

import overrides
from utils import memoize, error
import chpl_tasks, chpl_mem, chpl_compiler

@memoize
def get(flag=''):
    sanitizers_val = None
    if flag == 'exe':
        sanitizers_val = overrides.get('CHPL_SANITIZE_EXE', get(''))
    if not sanitizers_val:
        sanitizers_val = overrides.get('CHPL_SANITIZE', 'none')

    if sanitizers_val != 'none':
        if chpl_tasks.get() != 'fifo':
            error("CHPL_TASKS=fifo is required for sanitizers")
        if flag == 'exe' and chpl_mem.get('target') != 'cstdlib':
            error("CHPL_TARGET_MEM=cstdlib is required for sanitizers")
        if flag == 'exe' and chpl_compiler.get() == 'llvm':
            error("CHPL_TARGET_COMPILER=llvm is not supported with sanitizers")
        if flag == '' and chpl_mem.get('host') != 'cstdlib':
            error("CHPL_HOST_MEM=cstdlib is required for sanitizers")
    return sanitizers_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--exe])')
    parser.add_option('--exe', dest='flag', action='store_const',
                      const='exe', default='')
    (options, args) = parser.parse_args()

    sanitizers_val = get(options.flag)
    sys.stdout.write("{0}\n".format(sanitizers_val))

if __name__ == '__main__':
    _main()
