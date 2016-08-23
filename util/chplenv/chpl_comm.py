#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler, chpl_platform, overrides
from chpl_home_utils import using_chapel_module
from utils import memoize


@memoize
def get():
    comm_val = overrides.get('CHPL_COMM')
    if not comm_val:
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')

        # use ugni on cray-x* machines using the module and supported compiler
        ugni_comp = ('cray-prgenv-gnu', 'cray-prgenv-intel', 'cray-prgenv-cray')
        if (platform_val.startswith('cray-x') and
                using_chapel_module() and
                compiler_val in ugni_comp):
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
