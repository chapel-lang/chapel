#!/usr/bin/env python
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize


@memoize
def get():
    regexp_val = overrides.get('CHPL_REGEXP')
    if not regexp_val:
        third_party = get_chpl_third_party()
        uniq_cfg_path = get_uniq_cfg_path()
        regexp_subdir = os.path.join(third_party, 're2', 'install',
                                     uniq_cfg_path)
        regexp_header = os.path.join(regexp_subdir, 'include', 're2', 're2.h')
        if os.path.exists(regexp_header):
            regexp_val = 're2'
        else:
            regexp_val = 'none'
    return regexp_val


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()


@memoize
def get_link_args():
    return third_party_utils.default_get_link_args('re2',
                                                   libs=['-lre2', '-lpthread'])


def _main():
    regexp_val = get()
    sys.stdout.write("{0}\n".format(regexp_val))


if __name__ == '__main__':
    _main()
