#!/usr/bin/env python3
import sys

import chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import using_chapel_module
from compiler_utils import CompVersion
from utils import memoize


@memoize
def get():
    tasks_val = overrides.get('CHPL_TASKS')
    if not tasks_val:
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')
        arch_val = chpl_arch.get('target')

        cygwin = platform_val.startswith('cygwin')
        bsd = (platform_val.startswith('netbsd') or
               platform_val.startswith('freebsd'))
        mac_arm = platform_val.startswith('darwin') and arch_val == 'arm64'

        if cygwin or bsd or mac_arm:
            tasks_val = 'fifo'
        else:
            tasks_val = 'qthreads'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
