#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler, chpl_platform, overrides
from compiler_utils import CompVersion, get_compiler_version
from utils import memoize

@memoize
def get():
    tasks_val = overrides.get('CHPL_TASKS')
    if not tasks_val:
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')

        # CCE >= 8.4 is required to build qthreads (for gnu style inline asm.)
        # We build the module with a new enough version so we know the we can
        # use the qthreads it provides even if the user has an older CCE loaded
        using_qthreads_incompatible_cce = False
        if compiler_val == 'cray-prgenv-cray':
            if (get_compiler_version(compiler_val) < CompVersion('8.4') and
                    not using_chapel_module()):
                using_qthreads_incompatible_cce = True

        if (platform_val.startswith('cygwin') or
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
