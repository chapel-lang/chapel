#!/usr/bin/env python
import optparse
import os
import sys

import chpl_bin_subdir, chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party
from utils import memoize, run_command

@memoize
def get_uniq_cfg_path_for(llvm_val):
    if llvm_val == "llvm":
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
    llvm_target_dir = get_uniq_cfg_path_for('llvm')
    llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                               llvm_target_dir)
    llvm_header = os.path.join(llvm_subdir, 'include', 'llvm',
                               'PassSupport.h')
    if os.path.exists(llvm_header):
        return True
    else:
        return False

def compatible_platform_for_llvm_default():
  target_arch = chpl_arch.get('target')
  return (target_arch != "i368")

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
    if got and got != "missing-llvm-config":
        return True
    else:
        return False

@memoize
def get():
    llvm_val = overrides.get('CHPL_LLVM')
    if not llvm_val:
        llvm_val = 'none'

        if is_included_llvm_built():
            llvm_val = 'llvm'
        elif ("CHPL_LLVM_BY_DEFAULT" in os.environ and
               os.environ["CHPL_LLVM_BY_DEFAULT"] != "0" and
               # CHPL_LLVM_BY_DEFAULT is an enviro var to help us transition
               compatible_platform_for_llvm_default()):
            if has_compatible_installed_llvm():
                llvm_val = 'system'
            else:
                llvm_val = 'llvm'

    return llvm_val


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
      if llvm_val == 'system' or llvm_val == 'llvm':
        sys.stdout.write("llvm\n");
    else:
      sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
