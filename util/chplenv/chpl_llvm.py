#!/usr/bin/env python3
import optparse
import os
import sys

import chpl_bin_subdir, chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party
from utils import memoize, run_command

@memoize
def get_uniq_cfg_path_for(llvm_val):
    if llvm_val == "bundled":
      # put platform-arch-compiler for included llvm
      host_bin_subdir = chpl_bin_subdir.get('host')
      host_compiler = chpl_compiler.get('host')
      llvm_target_dir = '{0}-{1}'.format(host_bin_subdir, host_compiler)
    else:
      # just put 'system' for system llvm
      llvm_target_dir = llvm_val

    return llvm_target_dir

@memoize
def get_uniq_cfg_path():
    llvm_val = get()
    return get_uniq_cfg_path_for(llvm_val)

def is_included_llvm_built():
    chpl_third_party = get_chpl_third_party()
    llvm_target_dir = get_uniq_cfg_path_for('bundled')
    llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                               llvm_target_dir)
    llvm_header = os.path.join(llvm_subdir, 'include', 'llvm',
                               'PassSupport.h')
    if os.path.exists(llvm_header):
        return True
    else:
        return False

def compatible_platform_for_llvm():
  target_arch = chpl_arch.get('target')
  target_platform = chpl_platform.get('target')
  return (target_arch != "i368" and target_platform != "linux32")

def has_compatible_installed_llvm():
    preferred_vers_file = os.path.join(get_chpl_third_party(),
                                       'llvm', 'LLVM_VERSION')
    preferred_vers = ""
    with open(preferred_vers_file, 'r') as file:
        preferred_vers = file.read().strip()

    find_llvm_config = os.path.join(get_chpl_third_party(),
                                    'llvm', 'find-llvm-config.sh')

    got = run_command([find_llvm_config, preferred_vers])
    got = got.strip()
    platform = chpl_platform.get('target')
    if got and got != "missing-llvm-config":
        return True
    else:
        return False

@memoize
def get():
    llvm_val = overrides.get('CHPL_LLVM')
    if not llvm_val:
        llvm_val = 'unset'

        if compatible_platform_for_llvm():
            if is_included_llvm_built():
                llvm_val = 'bundled'
            elif has_compatible_installed_llvm():
                llvm_val = 'system'

        else:
            # This platform doesn't work with the LLVM backend
            # for one reason or another. So default to CHPL_LLVM=none.
            llvm_val = 'none'

    if llvm_val == 'llvm':
        sys.stderr.write("Warning: CHPL_LLVM=llvm is deprecated. "
                         "Use CHPL_LLVM=bundled instead\n")
        llvm_val = 'bundled'

    if not compatible_platform_for_llvm():
        if llvm_val != 'none' and llvm_val != 'unset':
            sys.stderr.write("Warning: CHPL_LLVM={0} is not compatible "
                             "with this platform".format(llvm_val))

    return llvm_val

def get_clang_bin_dir():
    llvm_val = get()
    if llvm_val == 'bundled':
        chpl_third_party = get_chpl_third_party()
        llvm_target_dir = get_uniq_cfg_path_for('bundled')
        llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                                   llvm_target_dir)
        clang = os.path.join(llvm_subdir, 'bin', 'clang')
        return clang
    else:
        return None

@memoize
def get_clang_cc():
    bin_dir = get_clang_bin_dir()
    if bin_dir:
        return os.path.join(bin_dir, 'clang');
    else:
        return 'clang'

@memoize
def get_clang_cpp():
    bin_dir = get_clang_bin_dir()
    if bin_dir:
        return os.path.join(bin_dir, 'clang++');
    else:
        return 'clang++'

def _main():
    llvm_val = get()

    parser = optparse.OptionParser(usage='usage: %prog [--needs-llvm-runtime]')
    parser.add_option('--needs-llvm-runtime', dest='needsllvm',
                      action='store_const',
                      const='needsllvm', default='')
    (options, args) = parser.parse_args()

    #if --needs-llvm-runtime is set, print out llvm if runtime is needed,
    # and print out nothing if it is not.
    if options.needsllvm:
      if llvm_val == 'system' or llvm_val == 'bundled':
        sys.stdout.write("llvm\n");
    else:
      sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
