#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_platform, chpl_compiler, chpl_comm
from utils import memoize, CompVersion
import utils

@memoize
def get():
    tasks_val = os.environ.get('CHPL_TASKS')
    if not tasks_val:
        arch_val = chpl_arch.get('target', get_lcd=True)
        platform_val = chpl_platform.get()
        compiler_val = chpl_compiler.get('target')

        # CCE 8.4 or newer is required to build qthreads. We build the module
        # with a new enough version so we know the we can use the qthreads it
        # provides even if the user has an older CCE loaded
        using_qthreads_incompatible_cce = False
        if compiler_val == 'cray-prgenv-cray':
            if (utils.get_compiler_version(compiler_val) < CompVersion('8.4') and
                    not using_chapel_module()):
                using_qthreads_incompatible_cce = True

        if (arch_val == 'knc' or
                platform_val.startswith('cygwin') or
                platform_val.startswith('netbsd') or
                using_qthreads_incompatible_cce):
            tasks_val = 'fifo'
        else:
            tasks_val = 'qthreads'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
