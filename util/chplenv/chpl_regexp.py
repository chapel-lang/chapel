#!/usr/bin/env python
import sys, os

import chpl_arch, chpl_platform, chpl_compiler, utils
from utils import memoize
import chpl_3p_re2_configs

@memoize
def get():
    regexp_val = os.environ.get('CHPL_REGEXP')
    if not regexp_val:
        chpl_home = utils.get_chpl_home()
        uniq_cfg_path = chpl_3p_re2_configs.get_uniq_cfg_path()
        regexp_subdir = os.path.join(chpl_home, 'third-party', 're2', 'install',
                                     uniq_cfg_path)
        regexp_header = os.path.join(regexp_subdir, 'include', 're2', 're2.h')
        if os.path.exists(regexp_header):
            regexp_val = 're2'
        else:
            regexp_val = 'none'
    return regexp_val


def _main():
    regexp_val = get()
    sys.stdout.write("{0}\n".format(regexp_val))


if __name__ == '__main__':
    _main()
