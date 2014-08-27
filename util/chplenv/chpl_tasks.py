#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_platform
from utils import memoize

@memoize
def get():
    tasks_val = os.environ.get('CHPL_TASKS')
    if not tasks_val:
        arch_val = chpl_arch.get('target', get_lcd=True)
        platform_val = chpl_platform.get()
        if arch_val == 'knc' or platform_val == 'cygwin':
            tasks_val = 'fifo'
        else:
            tasks_val = 'qthreads'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
