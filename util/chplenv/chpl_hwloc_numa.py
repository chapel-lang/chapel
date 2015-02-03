#!/usr/bin/env python
import sys, os

import chpl_hwloc, chpl_arch, chpl_compiler, chpl_platform, utils
from utils import memoize

@memoize
def get():
    numa_val = 'none'
    hwloc_val = chpl_hwloc.get()
    if hwloc_val == 'hwloc':
        target_platform = chpl_platform.get('target')
        target_compiler = chpl_compiler.get('target')
        target_arch = chpl_arch.get('target', map_to_compiler=True, get_lcd=True)

        chpl_home = utils.get_chpl_home()
        hwloc_target_dir = '{0}-{1}-{2}'.format(target_platform,
                                                target_compiler, target_arch)
        hwloc_subdir = os.path.join(chpl_home, 'third-party', 'hwloc',
                                    'install', hwloc_target_dir)
        libhwloc_la = os.path.join(hwloc_subdir, 'lib', 'libhwloc.la')

        if os.path.isfile(libhwloc_la):
            with open(libhwloc_la) as f:
                la_info = f.readlines()
            for line in la_info:
                if 'dependency_libs=' in line and '-lnuma' in line:
                    numa_val = 'numa'
                    break
    return numa_val


def _main():
    numa_val = get()
    sys.stdout.write("{0}\n".format(numa_val))


if __name__ == '__main__':
    _main()
