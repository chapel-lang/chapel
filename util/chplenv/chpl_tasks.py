#!/usr/bin/env python
import sys

import chpl_compiler, chpl_platform, overrides
from chpl_home_utils import using_chapel_module
from compiler_utils import CompVersion, get_compiler_version
from utils import memoize


@memoize
def get():
    tasks_val = overrides.get('CHPL_TASKS')
    if not tasks_val:
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')

        if (platform_val.startswith('cygwin') or
                platform_val.startswith('netbsd') or
                platform_val.startswith('freebsd')):
            tasks_val = 'fifo'
        else:
            tasks_val = 'qthreads'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
