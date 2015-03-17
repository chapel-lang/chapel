#!/usr/bin/env python
import sys, os

import chpl_compiler
import chpl_platform
from utils import memoize
import utils

@memoize
def get():
    comm_val = os.environ.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')

        # use ugni on cray-x* machines using the module and supported compiler
        #
        # Check that target arch is not knc. Don't use chpl_arch.get(), though,
        # since it already calls into this get() function. This check only
        # happens for X* systems using the Cray programming environment, so it
        # is safe to assume the relevant craype module will be used that sets
        # CRAY_CPU_TARGET.
        if (platform_val.startswith('cray-x') and
                utils.using_chapel_module() and
                compiler_val in ('cray-prgenv-gnu', 'cray-prgenv-intel') and
                os.getenv('CRAY_CPU_TARGET', '') != 'knc'):
            comm_val = 'ugni'
        # automatically uses gasnet when on a cray-x* or cray-cs machine
        elif platform_val.startswith('cray-'):
            comm_val = 'gasnet'
        else:
            comm_val = 'none'
    return comm_val


def _main():
    comm_val = get()
    sys.stdout.write("{0}\n".format(comm_val))


if __name__ == '__main__':
    _main()
