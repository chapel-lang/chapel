#!/usr/bin/env python3
import optparse
import os
import sys

import chpl_bin_subdir, chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party
from utils import memoize, error, run_command, try_run_command

# returns a tuple of supported major LLVM versions as strings
def llvm_versions():
    # Which major release - only need one number for that with current
    # llvm (since LLVM 4.0).
    # These will be tried in order.
    return ('11',)

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

def get_bundled_llvm_dir():
    chpl_third_party = get_chpl_third_party()
    llvm_target_dir = get_uniq_cfg_path_for('bundled')
    llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                               llvm_target_dir)
    return llvm_subdir

def is_included_llvm_built():
    llvm_subdir = get_bundled_llvm_dir()
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

# returns a string of the supported llvm versions suitable for error msgs
def llvm_versions_string():
    return ', '.join(llvm_versions())

# llvm_config is the llvm-config command we want to check out.
# returns (version_number, config_error_message)
@memoize
def check_llvm_config(llvm_config):
    got_version = 0
    version_ok = False
    llvm_header = ''
    llvm_include_ok = False
    clang_header = ''
    clang_include_ok = False
    exists, returncode, my_stdout, my_stderr = try_run_command([llvm_config,
                                                                '--version'])
    if exists and returncode == 0:
        version_string = my_stdout.strip()
        got_version = version_string.split('.')[0]
        version_ok = got_version in llvm_versions()
    else:
        s = "could not run llvm-config at {0}".format(llvm_config)
        return (0, s)

    include_dir = run_command([llvm_config, '--includedir']).strip()
    if os.path.isdir(include_dir):
        llvm_header = os.path.join(include_dir,
                                   'llvm', 'Config', 'llvm-config.h')
        llvm_include_ok = os.path.exists(llvm_header)
        clang_header = os.path.join(include_dir, 'clang', 'Basic', 'Version.h')
        clang_include_ok = os.path.exists(clang_header)

    s = ''
    if not version_ok:
        s = ("LLVM version {0} is not one of the supported versions: {1}"
             .format(got_version, llvm_versions_string()))
        return (got_version, s)

    if not llvm_include_ok:
       s = "Could not find the LLVM header {0}".format(llvm_header)
       s += "\nPerhaps you need to install clang and llvm dev packages"
       return (got_version, s)
    elif not clang_include_ok:
       s = "Could not find the clang header {0}".format(clang_header)
       s += "\nPerhaps you need to install clang and llvm dev packages"
       return (got_version, s)

    return (got_version, '')


@memoize
def find_system_llvm_config():
    paths = [ ]
    for vers in llvm_versions():
        paths.append("llvm-config-" + vers + ".0")
        paths.append("llvm-config-" + vers)
        # next ones are for Homebrew
        paths.append("/usr/local/opt/llvm@" + vers + ".0/bin/llvm-config")
        paths.append("/usr/local/opt/llvm@" + vers + "/bin/llvm-config")

    # check also unversioned commands
    paths.append("llvm-config")
    # next for Homebrew
    paths.append("/usr/local/opt/llvm/bin/llvm-config")

    all_found = [ ]

    for command in paths:
        found_version, found_config_err = check_llvm_config(command)
        all_found.append( (command, found_version, found_config_err) )

    found = ('', '', '')
    for vers in llvm_versions():
        for entry in all_found:
            if entry[1] == vers:
                found = entry
                break

    # command set, version > 0, no error
    command = found[0]
    version = found[1]
    config_err = found[2]

    if command and version and not config_err:
        return found[0]

    return ''


@memoize
def get_llvm_config():
    llvm_val = get()
    llvm_config = overrides.get('CHPL_LLVM_CONFIG', 'none')

    if llvm_config and llvm_val == 'bundled':
        error("cannot set CHPL_LLVM_CONFIG along with CHPL_LLVM=bundled")

    if not llvm_config:
        if llvm_val == 'bundled':
            llvm_subdir = get_bundled_llvm_dir()
            llvm_config = os.path.join(llvm_subdir, 'bin', 'llvm-config')
        elif llvm_val == 'system':
            llvm_config = find_system_llvm_config()
            if not llvm_config:
                error("CHPL_LLVM=system but could not find an installed LLVM"
                       " with one of the supported versions: {0}".format(
                       llvm_versions_string()))

    return llvm_config

@memoize
def validate_llvm_config():
    llvm_val = get()
    llvm_config = get_llvm_config()
    if llvm_val == 'system' and llvm_config:
        version, config_error = check_llvm_config(llvm_config)
        if config_error:
            error("Problem with llvm-config at {0} -- {1}"
                  .format(llvm_config, config_error))


@memoize
def get_system_llvm_config_bindir():
    llvm_config = get_llvm_config()
    bindir = run_command([llvm_config, '--bindir']).strip()

    if os.path.isdir(bindir):
        pass
    else:
        error("llvm-config command {0} provides missing bin directory {0}"
              .format(llvm_config, bindir))
    return bindir

def get_llvm_clang_command_name(lang):
    lang_upper = lang.upper()

    if lang_upper == 'C++':
        lang_upper = 'CXX'

    if lang_upper == 'CXX':
        return 'clang++'
    else:
        return 'clang'

# lang should be C or CXX
@memoize
def get_llvm_clang(lang):
    clang_name = get_llvm_clang_command_name(lang)

    llvm_val = get()
    if llvm_val == 'system':
        bindir = get_system_llvm_config_bindir()
        return os.path.join(bindir, clang_name)
    elif llvm_val == 'bundled':
        llvm_subdir = get_bundled_llvm_dir()
        return os.path.join(llvm_subdir, 'bin', clang_name)
    else:
        return ''

def has_compatible_installed_llvm():
    llvm_config = find_system_llvm_config()
    if llvm_config:
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

def llvm_enabled():
    llvm_val = get()
    if llvm_val == 'bundled' or llvm_val == 'system':
        return True

    return False

def _main():
    llvm_val = get()
    llvm_config = get_llvm_config()

    parser = optparse.OptionParser(usage='usage: %prog [--needs-llvm-runtime]')
    parser.add_option('--needs-llvm-runtime', dest='action',
                      action='store_const',
                      const='needsllvm', default='')
    parser.add_option('--llvm-config', dest='action',
                      action='store_const',
                      const='llvmconfig', default='')

    (options, args) = parser.parse_args()

    #if --needs-llvm-runtime is set, print out llvm if runtime is needed,
    # and print out nothing if it is not.
    if options.action == 'needsllvm':
        if llvm_val == 'system' or llvm_val == 'bundled':
            sys.stdout.write("llvm\n");
    elif options.action == 'llvmconfig':
        sys.stdout.write("{0}\n".format(llvm_config))
        validate_llvm_config()
    else:
      sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
