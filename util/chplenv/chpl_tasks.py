#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_arch, chpl_comm, chpl_compiler, chpl_platform, utils
from utils import memoize


@memoize
def get():
    tasks_val = os.environ.get('CHPL_TASKS')
    if not tasks_val:
        arch_val = chpl_arch.get('target', get_lcd=True)
        platform_val = chpl_platform.get()
        compiler_val = chpl_compiler.get('target')
        comm_val = chpl_comm.get()

        if (arch_val == 'knc' or
                platform_val.startswith('cygwin') or
                platform_val.startswith('netbsd') or
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
