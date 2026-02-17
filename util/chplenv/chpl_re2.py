#!/usr/bin/env python3
import os
import sys

import chpl_compiler, chpl_platform, overrides, third_party_utils
from chpl_home_utils import get_chpl_third_party
from utils import memoize, warning, error, check_valid_var


# returns True if CHPL_RE2 was set by the user
# (i.e. not inferred to be the default)
@memoize
def is_overridden():
    re2 = overrides.get('CHPL_RE2')

    if re2:
        return True

    return False

@memoize
def get():
    re2 = overrides.get('CHPL_RE2')

    if not re2:
        re2_header = os.path.join(get_chpl_third_party(), 're2',
                                  'install', get_uniq_cfg_path(),
                                  'include', 're2', 're2.h')
        re2 = 'bundled' if os.path.exists(re2_header) else 'none'

    check_valid_var("CHPL_RE2", re2, ["none", "bundled"])
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
    bundled, system = [], []
    re2_val = get()
    if re2_val == "bundled":
        bundled, system = third_party_utils.pkgconfig_get_bundled_link_args("re2")

        import chpl_llvm
        import chpl_platform
        if (
            chpl_platform.get("target") == "darwin"
            and chpl_llvm.get() != "none"
            and chpl_llvm.get_llvm_version() == "21"
        ):
            # workaround upstream LLVM/homebrew bug
            # https://github.com/llvm/llvm-project/issues/77653
            # https://github.com/Homebrew/homebrew-core/issues/235411
            libdir = chpl_llvm.get_llvm_config_libdir()
            if libdir:
                libcxxdir = os.path.join(libdir, "c++")
                if os.path.isdir(libcxxdir):
                    bundled = ["-L" + libcxxdir, "-Wl,-rpath," + libcxxdir] + bundled

    return (bundled, system)


def _main():
    import optparse
    parser = optparse.OptionParser(usage='usage: %prog [options]')
    parser.add_option('--compile', dest='which', action='store_const',
                      const='compile', default=None)
    parser.add_option('--link', dest='which', action='store_const',
                      const='link')
    options, args = parser.parse_args()
    if options.which == 'compile':
        val = get_compile_args()
    elif options.which == 'link':
        val = get_link_args()
    else:
        val = get()
    sys.stdout.write("{0}\n".format(val))

if __name__ == '__main__':
    _main()
