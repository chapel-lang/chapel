#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_platform, chpl_compiler, chpl_comm
from utils import memoize
import utils

@memoize
def get():
    tasks_val = os.environ.get('CHPL_TASKS')
    if not tasks_val:
        arch_val = chpl_arch.get('target', get_lcd=True)
        platform_val = chpl_platform.get()
        compiler_val = chpl_compiler.get('target')
        comm_val = chpl_comm.get()

        # use muxed on cray-x* machines using the module and supported compiler
        if (comm_val == 'ugni' and
                platform_val.startswith('cray-x') and
                utils.using_chapel_module() and
                compiler_val in ('cray-prgenv-gnu', 'cray-prgenv-intel') and
                arch_val != 'knc'):
            tasks_val = 'muxed'
        elif (arch_val == 'knc' or
                platform_val.startswith('cygwin') or
                platform_val.startswith('netbsd') or
                compiler_val == 'pgi'             or
                compiler_val == 'cray-prgenv-pgi' or
                compiler_val == 'cray-prgenv-cray'):
            tasks_val = 'fifo'
        else:
            tasks_val = 'qthreads'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
