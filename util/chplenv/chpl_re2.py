#!/usr/bin/env python3
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize


@memoize
def get():
    re2 = overrides.get('CHPL_RE2')
    regexp = overrides.get('CHPL_REGEXP')
    if regexp:
        if not re2:
            re2 = 'bundled' if regexp == 're2' else 'none'
        sys.stderr.write("Warning: CHPL_REGEXP is deprecated.\n"
                         "Set CHPL_RE2 to 'bundled' or 'none'.\n"
                         "Assuming CHPL_RE2={}.\n".format(re2))
    elif not re2:
        re2_header = os.path.join(get_chpl_third_party(), 're2',
                                  'install', get_uniq_cfg_path(),
                                  'include', 're2', 're2.h')
        re2 = 'bundled' if os.path.exists(re2_header) else 'none'
    return re2


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()


@memoize
def get_link_args():
    return third_party_utils.default_get_link_args('re2',
                                                   libs=['-lre2', '-lpthread'])


def _main():
    sys.stdout.write("{0}\n".format(get()))


if __name__ == '__main__':
    _main()
