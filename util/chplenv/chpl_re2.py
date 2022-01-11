#!/usr/bin/env python3
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize, warning


# returns True if CHPL_RE2 was set by the user
# (i.e. not inferred to be the default)
@memoize
def is_overridden():
    re2 = overrides.get('CHPL_RE2')
    regexp = overrides.get('CHPL_REGEXP')

    if re2 or regexp:
        return True

    return False

@memoize
def get():
    re2 = overrides.get('CHPL_RE2')
    regexp = overrides.get('CHPL_REGEXP')
    if regexp:
        if not re2:
            re2 = 'bundled' if regexp == 're2' else 'none'

        warning("CHPL_REGEXP is deprecated. Use CHPL_RE2.")
    elif not re2:
        re2_header = os.path.join(get_chpl_third_party(), 're2',
                                  'install', get_uniq_cfg_path(),
                                  'include', 're2', 're2.h')
        re2 = 'bundled' if os.path.exists(re2_header) else 'none'
    return re2


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    re2_val = get()
    if re2_val == 'bundled':
        return third_party_utils.get_bundled_compile_args('re2')

    return ([ ], [ ])

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    re2_val = get()
    if re2_val == 'bundled':
        return third_party_utils.get_bundled_link_args('re2',
                                                       libs=['-lre2',
                                                             '-lpthread'])
    return ([ ], [ ])


def _main():
    sys.stdout.write("{0}\n".format(get()))


if __name__ == '__main__':
    _main()
