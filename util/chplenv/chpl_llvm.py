#!/usr/bin/env python3
import optparse
import os
import sys
from distutils.spawn import find_executable
import re

import chpl_bin_subdir, chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party, get_chpl_home
from utils import memoize, error, run_command, try_run_command, warning

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
    if llvm_config == 'none':
        return (0, "no llvm-config detected")

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

    if llvm_val == 'bundled':
        llvm_subdir = get_bundled_llvm_dir()
        bundled_config = os.path.join(llvm_subdir, 'bin', 'llvm-config')
        if llvm_config != 'none' and llvm_config != bundled_config:
            warning("CHPL_LLVM_CONFIG is ignored for CHPL_LLVM=bundled");
        llvm_config = bundled_config

    elif llvm_config == 'none' and llvm_val == 'system':
        llvm_config = find_system_llvm_config()

    return llvm_config

@memoize
def validate_llvm_config(llvm_config=None):
    llvm_val = get()
    # We pass in llvm_config if has already been computed (so we don't
    # end up in an infinite loop).
    if llvm_config is None:
      llvm_config = get_llvm_config()
    if llvm_val == 'system':
        if llvm_config == '' or llvm_config == 'none':
            error("CHPL_LLVM=system but could not find an installed LLVM"
                  " with one of the supported versions: {0}".format(
                  llvm_versions_string()))

    if (llvm_val == 'system' or
        (llvm_val == 'bundled' and os.path.exists(llvm_config))):
        version, config_error = check_llvm_config(llvm_config)
        if config_error:
            error("Problem with llvm-config at {0} -- {1}"
                  .format(llvm_config, config_error))


@memoize
def get_system_llvm_config_bindir():
    llvm_config = get_llvm_config()
    validate_llvm_config(llvm_config)
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
        clang = os.path.join(bindir, clang_name)
    elif llvm_val == 'bundled':
        llvm_subdir = get_bundled_llvm_dir()
        clang = os.path.join(llvm_subdir, 'bin', clang_name)
    else:
        return ''

    # tack on arguments that control clang's function
    clang_args = get_clang_args()
    if clang_args:
        clang += " " + clang_args

    return clang


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

    if not compatible_platform_for_llvm():
        if llvm_val != 'none' and llvm_val != 'unset':
            warning("CHPL_LLVM={0} is not compatible with this "
                    "platform".format(llvm_val))

    return llvm_val

def llvm_enabled():
    llvm_val = get()
    if llvm_val == 'bundled' or llvm_val == 'system':
        return True

    return False

@memoize
def get_gcc_prefix():
    gcc_prefix = overrides.get('CHPL_LLVM_GCC_PREFIX', '')

    if not gcc_prefix:
        # When 'gcc' is a command other than '/usr/bin/gcc',
        # compute the 'gcc' prefix that LLVM should use.
        gcc_path = find_executable('gcc')
        if gcc_path == '/usr/bin/gcc' :
            # In this common case, nothing else needs to be done,
            # because we can assume that clang can find this gcc.
            pass
        elif gcc_path == None:
            # Nothing else we can do here
            pass
        else:
            # Try to figure out the GCC prefix by running gcc
            out, err = run_command(['gcc', '-v'], stdout=True, stderr=True)
            out = out + err

            # look for the --prefix= specified when GCC was configured
            words = out.split()
            for word in words:
                if word.startswith('--prefix='):
                    gcc_prefix = word[len('--prefix='):]
                    break
            # check that directory exists.
            if gcc_prefix and os.path.isdir(gcc_prefix):
                # if so, we are done.
                pass
            else:
                # We didn't find a --prefix= flag, so fall back on a heuristic.
                # try removing bin/gcc from the end
                mydir = os.path.dirname(os.path.dirname(gcc_path))
                if mydir and os.path.isdir(mydir):
                    # then check for mydir/include
                    inc = os.path.join(mydir, "include")
                    if os.path.isdir(inc):
                        gcc_prefix = mydir
                    else:
                        inc = os.path.join(mydir, "snos", "include")
                        if os.path.isdir(inc):
                            gcc_prefix = mydir

    return gcc_prefix

@memoize
def get_sysroot_resource_dir_args():
    args = ''
    target_platform = chpl_platform.get('target')
    llvm_val = get()
    if target_platform == "darwin" and llvm_val == "bundled":
        # Add -isysroot and -resourcedir based upon what 'clang' uses
        cfile = get_chpl_home() + "/runtime/include/sys_basic.h"
        (out,err) = run_command(['clang', '-###', cfile],
                                stdout=True, stderr=True)
        out += err

        found = re.search('"-isysroot" "([^"]+)"', out)
        if found:
            args += ' -isysroot ' + found.group(1)

        found = re.search('"-resource-dir" "([^"]+)"', out)
        if found:
            args += ' -resource-dir ' + found.group(1)

        if args:
            # add -L/usr/local/lib
            # since some versions of clang forget to search it with -isysroot
            args += ' -L/usr/local/lib'

    return args.strip()

# On some systems, we need to give clang some arguments for it to
# find the correct system headers.
#  * when PrgEnv-gnu is loaded on an XC, we should provide
#    a --gcc-toolchain argument indicating the prefix of that GCC installation.
#
#  * on Mac OS X, we might need to provide -mlinker-version=450
#    on 10.14 and in some cases -isysroot and -resource-dir.
@memoize
def get_clang_args():
    clang_args = ''

    gcc_prefix = get_gcc_prefix()
    if gcc_prefix:
        clang_args += ' --gcc-toolchain=' + gcc_prefix

    sysroot_args = get_sysroot_resource_dir_args()
    if sysroot_args:
        clang_args += ' ' + sysroot_args

    # This is a workaround for problems with Homebrew llvm@11 on 10.14
    # which avoids errors like
    #  ld: unknown option: -platform_version
    target_platform = chpl_platform.get('target')
    if target_platform == "darwin":
        os_ver = run_command(['sw_vers', '-productVersion'])
        if os_ver.startswith('10.14'):
            clang_args += ' -mlinker-version=450'

    return clang_args.strip()

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
