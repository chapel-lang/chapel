#!/usr/bin/env python3

import optparse
import os
import sys
import re

import chpl_bin_subdir, chpl_arch, chpl_compiler, chpl_platform, overrides
from chpl_home_utils import get_chpl_third_party, get_chpl_home
import chpl_gpu
import homebrew_utils
from utils import which, memoize, error, run_command, try_run_command, warning
from collections import defaultdict

# returns a tuple of supported major LLVM versions as strings
def llvm_versions():
    # Which major release - only need one number for that with current
    # llvm (since LLVM 4.0).
    # These will be tried in order.
    return ('19','18','17','16','15','14','13','12','11',)

@memoize
def get_uniq_cfg_path_for(llvm_val, llvm_support_val):
    if llvm_val == "bundled" or llvm_support_val == "bundled":
        # put platform-arch-compiler for included llvm
        llvm_target_dir = chpl_bin_subdir.get('host')
    else:
        # just put 'system' for system llvm
        llvm_target_dir = llvm_val

    return llvm_target_dir

@memoize
def get_uniq_cfg_path():
    llvm_val = get()
    llvm_support_val = get_llvm_support()
    return get_uniq_cfg_path_for(llvm_val, llvm_support_val)

def get_bundled_llvm_dir():
    chpl_third_party = get_chpl_third_party()
    llvm_target_dir = get_uniq_cfg_path_for('bundled', 'bundled')
    llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                               llvm_target_dir)
    return llvm_subdir

def get_bundled_llvm_support_only_dir():
    chpl_third_party = get_chpl_third_party()
    llvm_target_dir = get_uniq_cfg_path_for('none', 'bundled')
    llvm_subdir = os.path.join(chpl_third_party, 'llvm', 'install',
                               'support-only-' + llvm_target_dir)
    return llvm_subdir

def is_included_llvm_built(llvm_val):
    llvm_subdir = None
    if llvm_val == 'bundled':
        llvm_subdir = get_bundled_llvm_dir()
    else:
        llvm_subdir = get_bundled_llvm_support_only_dir()

    llvm_header = os.path.join(llvm_subdir, 'include', 'llvm',
                               'PassSupport.h')
    if os.path.exists(llvm_header):
        return True
    else:
        return False

def compatible_platform_for_llvm():
    target_arch = chpl_arch.get('target')
    target_platform = chpl_platform.get('target')

    is32bit = target_platform == "linux32" or target_arch == "i368"

    return not is32bit

# returns a string of the supported llvm versions suitable for error msgs
def llvm_versions_string():
    return ', '.join(llvm_versions())


# returns the full output of llvm-config --version for the passed llvm-config
# path or command name. Returns None if something went wrong.
@memoize
def get_llvm_config_version(llvm_config):
    got_version = None

    if llvm_config != 'none' and llvm_config != None:
        exists, returncode, got_out, got_err = try_run_command([llvm_config,
                                                                '--version'])
        if exists and returncode == 0:
            got_version = got_out

        if got_version != None and chpl_gpu.get() == 'amd':
            # strip the "git" suffix. This is a TODO. We want to be able to
            # detect LLVM "nightly" versions because ROCm seems to ship with
            # those. A sign for that is the `git` suffix at the end of the
            # version string. As of today 15.0.0git works for us as, I believe,
            # it is pretty close to 15.0.0 proper.
            got_version = got_version.strip()
            if got_version[-3:] == 'git':
                got_version = got_version[:-3]
    return got_version

# Returns the full output of clang --version for the passed clang command.
# Returns None if something went wrong.
@memoize
def get_clang_version(clang_command, short=False):
    got_version = None

    if clang_command != 'none' and clang_command != None:
        version = '--version' if not short else '-dumpversion'
        exists, returncode, got_out, _ = try_run_command([clang_command, version])

        if exists and returncode == 0:
            got_version = got_out

    return got_version

@memoize
def get_clang_cfg_file(clang_command):
    """
    Returns the clang config file for the given clang command.
    """

    if clang_command == 'none' or clang_command is None:
        return None

    got_file = None
    exists, ret, got_out, _ = try_run_command([clang_command, "--version"])
    if exists and ret == 0 and got_out:
        # fine the line that starts with "Configuration file:"
        for line in got_out.splitlines():
            if line.startswith("Configuration file:"):
                got_file = line.split(":", maxsplit=1)[1].strip()
                break

    return got_file

# llvm_config is the llvm-config command we want to check out.
# returns (version_number, config_error_message)
@memoize
def check_llvm_config(llvm_config):
    if llvm_config == 'none':
        return (0, "no llvm-config detected")

    got_version = 0
    version_ok = False

    s = ''

    version_string = get_llvm_config_version(llvm_config)
    if version_string != None:
        got_version = version_string.strip().split('.')[0]
        version_ok = got_version in llvm_versions()
    else:
        s = "could not run llvm-config at {0}".format(llvm_config)
        return (0, s)

    if not version_ok:
        s = ("LLVM version {0} is not one of the supported versions: {1}"
             .format(got_version, llvm_versions_string()))

    return (got_version, s)

# Ensure that relevant LLVM-related header files and libraries have been
# installed. If these are missing it usually indicates the user failed to
# install some necessary package.
#
# Returns a tuple like (isOk, errorMessage)
def check_llvm_packages(llvm_config):
    llvm_header = ''
    llvm_include_ok = False
    clang_header = ''
    clang_include_ok = False
    clang_cpp_lib = ''
    clang_cpp_lib_ok = False

    usr_include_clang_ok = False

    include_dir = run_command([llvm_config, '--includedir']).strip()
    if include_dir.startswith("/nix/store"):
        # Dependencies are managed by Nix, and llvm, clang, etc. are all
        # different packages. Hence, we cannot rely on clang/Basic/Version.h
        # residing in the include_dir returned by llvm-config.
        return (True, '')

    if os.path.isdir(include_dir):
        llvm_header = os.path.join(include_dir,
                                   'llvm', 'Config', 'llvm-config.h')
        llvm_include_ok = os.path.exists(llvm_header)

        clang_header = os.path.join(include_dir, 'clang', 'Basic', 'Version.h')
        clang_include_ok = os.path.exists(clang_header)
        if not clang_include_ok:
            usr_include = "/usr/include"
            clang_header2 = os.path.join(usr_include, 'clang', 'Basic', 'Version.h')
            ver_inc = os.path.join(usr_include, 'clang', 'Basic', 'Version.inc')
            if os.path.exists(clang_header2) and os.path.exists(ver_inc):

                llvm_version, ignored_err = check_llvm_config(llvm_config)
                llvm_version = llvm_version.strip()
                with open(ver_inc) as f:
                    contents = f.read()
                    if llvm_version in contents:
                        usr_include_clang_ok = True
                        clang_include_ok = True

    llvm_lib_dir = run_command([llvm_config, '--libdir']).strip()

    clang_lib_name = None
    if sys.platform == "darwin":
        clang_lib_name = 'libclang-cpp.dylib'
    elif sys.platform == "win32":
        clang_lib_name = 'libclang-cpp.dll'
    else:
        clang_lib_name = 'libclang-cpp.so'

    if os.path.isdir(llvm_lib_dir):
        clang_cpp_lib = os.path.join(llvm_lib_dir, clang_lib_name)
        clang_cpp_lib_ok = os.path.exists(clang_cpp_lib)
        if usr_include_clang_ok and not clang_cpp_lib_ok:
            # use e.g. /usr/lib/libclang-cpp.so
            clang_cpp_lib = os.path.join("/usr/lib", clang_lib_name)
            clang_cpp_lib_ok = os.path.exists(clang_cpp_lib)

        if usr_include_clang_ok and not clang_cpp_lib_ok:
            # use e.g. /usr/lib64/libclang-cpp.so
            clang_cpp_lib = os.path.join("/usr/lib64", clang_lib_name)
            clang_cpp_lib_ok = os.path.exists(clang_cpp_lib)

    s = ''
    if not llvm_include_ok:
        s = "Could not find the LLVM header {0}".format(llvm_header)
        s += "\nPerhaps you need to install clang and llvm dev packages"
    elif not clang_include_ok:
        s = "Could not find the clang header {0}".format(clang_header)
        s += "\nPerhaps you need to install clang and llvm dev packages"
    elif not clang_cpp_lib_ok:
        s = "Could not find the clang library {0}".format(clang_cpp_lib)
        s += "\nPerhaps you need to install the libclang-cpp-dev package"

    host_platform = chpl_platform.get('host')
    if host_platform == "darwin":
        # on Mac OS X with Homebrew, require LLVM 14 or newer
        # because these LLVM versions fix a problem with
        # mixing libc++ versions
        llvm_version, ignored_err = check_llvm_config(llvm_config)
        llvm_version = llvm_version.strip()
        bad_vers = ('11', '12', '13')
        if llvm_version in bad_vers:
            # compute the set subtraction:
            #    llvm_versions() - bad_vers
            # for use in the error message
            ok_vers = [ ]
            vers = llvm_versions()
            for v in vers:
                if not v in bad_vers:
                    ok_vers.append(v)

            s = ("LLVM version {0} is not supported on Mac OS X. "
                 "Please use one of these versions: {1}"
                 .format(llvm_version, ', '.join(ok_vers)))


    return (s == '', s)


@memoize
def find_system_llvm_config():
    llvm_config = overrides.get('CHPL_LLVM_CONFIG', 'none')
    if llvm_config != 'none':
        return llvm_config

    llvm_config = chpl_gpu.get_llvm_override()
    if llvm_config != 'none':
        return llvm_config


    homebrew_prefix = homebrew_utils.get_homebrew_prefix()

    paths = [ ]
    for vers in llvm_versions():
        paths.append("llvm-config-" + vers + ".0")
        paths.append("llvm-config-" + vers)
        # this format used by freebsd
        paths.append("llvm-config" + vers)
        # this format is used by Alpine Linux
        paths.append("llvm" + vers + "-config")
        if homebrew_prefix:
            # look for homebrew install of LLVM
            paths.append(homebrew_prefix +
                         "/opt/llvm@" + vers + ".0/bin/llvm-config")
            paths.append(homebrew_prefix +
                         "/opt/llvm@" + vers + "/bin/llvm-config")

    # check also unversioned commands
    paths.append("llvm-config")
    if homebrew_prefix:
        paths.append(homebrew_prefix + "/opt/llvm/bin/llvm-config")

    by_version = defaultdict(list)
    errs = []

    for command in paths:
        version, config_err = check_llvm_config(command)
        if not config_err:
            by_version[version].append(command)
        else:
            errs.append((command, config_err))

    for version in llvm_versions():
        commands = by_version[version]
        if commands:
            return commands[0]

    return ''


# Returns whether to use the bundled or system LLVM for the
# LLVM support module.
# This corresponds to the CHPL_LLVM_SUPPORT variable.
# Since the LLVM support module is now required to build Chapel,
# this returns "bundled" or "system" even if CHPL_LLVM=none.
@memoize
def get_llvm_support():
    llvm_val = get()

    # If using CHPL_LLVM=bundled or CHPL_LLVM=system
    # then CHPL_LLVM_SUPPORT needs to match
    if llvm_val == 'bundled':
        return 'bundled'
    if llvm_val == 'system':
        return 'system'

    llvm_support_val = overrides.get('CHPL_LLVM_SUPPORT')
    if not llvm_support_val:
        if has_compatible_installed_llvm():
            llvm_support_val = 'system'
        else:
            llvm_support_val = 'bundled'

    return llvm_support_val

@memoize
def get_llvm_config():
    llvm_val = get()
    llvm_support_val = get_llvm_support()
    llvm_config = overrides.get('CHPL_LLVM_CONFIG', 'none')

    if llvm_val == 'bundled':
        llvm_subdir = get_bundled_llvm_dir()
        bundled_config = os.path.join(llvm_subdir, 'bin', 'llvm-config')
        if llvm_config != 'none' and llvm_config != bundled_config:
            warning("CHPL_LLVM_CONFIG is ignored for CHPL_LLVM=bundled")
        llvm_config = bundled_config

    elif llvm_support_val == 'bundled':
        llvm_subdir = get_bundled_llvm_support_only_dir()
        bundled_config = os.path.join(llvm_subdir, 'bin', 'llvm-config')
        if llvm_config != 'none' and llvm_config != bundled_config:
            warning("CHPL_LLVM_CONFIG is ignored for CHPL_LLVM_SUPPORT=bundled")
        llvm_config = bundled_config

    elif llvm_config == 'none':
        if llvm_val == 'system' or llvm_support_val == 'system':
            llvm_config = find_system_llvm_config()

    return llvm_config


@memoize
def get_llvm_version():
    if os.environ.get('CHPL_LLVM_VERSION') is not None:
        warning("Setting CHPL_LLVM_VERSION has no effect. Unset CHPL_LLVM_VERSION and set CHPL_LLVM_CONFIG to the desired 'llvm-config' path instead.")
    (llvm_version, _) = check_llvm_config(get_llvm_config())
    return llvm_version

@memoize
def validate_llvm_config():
    llvm_val = get()
    llvm_support_val = get_llvm_support()
    llvm_config = get_llvm_config()

    if llvm_val == 'system':
        if llvm_config == '' or llvm_config == 'none':
            error("CHPL_LLVM=system but could not find an installed LLVM"
                  " with one of the supported versions: {0}".format(
                  llvm_versions_string()))

    if llvm_support_val == 'system':
        if llvm_config == '' or llvm_config == 'none':
            error("CHPL_LLVM_SUPPORT=system but could not find an installed "
                  "LLVM with one of the supported versions: {0}".format(
                  llvm_versions_string()))

    if (llvm_val == 'system' or
        (llvm_val == 'bundled' and os.path.exists(llvm_config))):
        version, config_error = check_llvm_config(llvm_config)
        if config_error:
            error("Problem with llvm-config at {0} -- {1}"
                  .format(llvm_config, config_error))

    if llvm_val == 'system':
        bindir = get_system_llvm_config_bindir()
        if not bindir.startswith("/nix/store"):
            if not (bindir and os.path.isdir(bindir)):
                error("llvm-config command {0} provides missing bin dir {1}"
                      .format(llvm_config, bindir))

        (noPackageErrors, package_err) = check_llvm_packages(llvm_config)
        if not noPackageErrors:
            error(package_err)

        clang_c = get_llvm_clang('c')[0]
        clang_cxx = get_llvm_clang('c++')[0]
        if clang_c == '':
            error("Could not find clang with the same version as "
                  "CHPL_LLVM_CONFIG={}. Please try setting CHPL_TARGET_CC.".format(llvm_config))
        if clang_cxx == '':
            error("Could not find clang++ with the same version as "
                  "CHPL_LLVM_CONFIG={}. Please try setting CHPL_TARGET_CXX.".format(llvm_config))

        def print_clang_version_error(clang, name="clang"):
            clang_version = get_clang_version(clang, short=True)
            llvm_version = str(get_llvm_config_version(llvm_config)).strip()
            if clang_version is None:
                error(
                    "Could not determine version of {} at '{}'".format(
                        name, clang
                    )
                )
            elif llvm_version not in clang_version:
                error(
                    "Version of {} at '{}' does not".format(name, clang) +
                    " match version of LLVM at '{}'\n".format(llvm_config) +
                    "LLVM version: {}\n".format(llvm_version) +
                    "{} version: {}".format(name, clang_version)
                )
            else:
                error(
                    "Missing or wrong version for {} at '{}'".format(
                        name, clang
                    )
                    + " (using LLVM at {})".format(llvm_config)
                )

        if not is_system_clang_version_ok(clang_c):
            print_clang_version_error(clang_c)
        if not is_system_clang_version_ok(clang_cxx):
            print_clang_version_error(clang_cxx, name="clang++")


@memoize
def get_system_llvm_config_bindir():
    llvm_config = find_system_llvm_config()
    found_version, found_config_err = check_llvm_config(llvm_config)

    bindir = None

    if llvm_config and found_version and not found_config_err:
        bindir = run_command([llvm_config, '--bindir']).strip()

    return bindir

def get_llvm_clang_command_name(lang):
    lang_upper = lang.upper()

    if lang_upper == 'C++':
        lang_upper = 'CXX'

    if lang_upper == 'CXX':
        return 'clang++'
    else:
        return 'clang'

# checks that the clang version matches the llvm-config version.
# Returns True if it's compatible and False if not.
@memoize
def is_system_clang_version_ok(clang_command):
    llvm_config = find_system_llvm_config()
    llvm_version_string = get_llvm_config_version(llvm_config)
    llvm_version = llvm_version_string.strip()
    clang_version_out = get_clang_version(clang_command)
    return clang_version_out != None and llvm_version in clang_version_out

# Given a lang argument of 'c' or 'c++'/'cxx', returns the value to use for
# CHPL_LLVM_CLANG_C / CHPL_LLVM_CLANG_CXX when considering overrides for these
# variables as well as for CHPL_TARGET_CC / CHPL_TARGET_CXX.
# Returns a list where the 1st value is the command and additional values
# are arguments to use.
# Returns None if no override was present.
@memoize
def get_overriden_llvm_clang(lang):
    lang_upper = lang.upper()
    if lang_upper == 'C++':
        lang_upper = 'CXX'

    # Compute it based on setting CHPL_LLVM_CLANG_C/CXX
    # or, if CHPL_TARGET_COMPILER=llvm, CHPL_TARGET_CC/CXX.
    # These use split in order to separate the command out from
    # any arguments passed to it.

    # The proper check here is `chpl_compiler.get('target') == 'llvm'`.
    # However, that will not work when trying to infer `CHPL_LLVM`, because
    # `chpl_compiler.get('target')` will call this code and infinite recursion
    # will occur.
    # Instead we will check the target compiler solely based on the
    # values of `CHPL_TARGET_COMPILER` and `CHPL_LLVM`
    llvm_val = overrides.get('CHPL_LLVM', 'unset')
    tgt_llvm = (
        overrides.get("CHPL_TARGET_COMPILER", "llvm") == "llvm"
        and llvm_val != "unset"
        and llvm_val != "none"
    )
    res = None
    if lang_upper == 'C':
        llvm_clang_c = overrides.get('CHPL_LLVM_CLANG_C')
        if llvm_clang_c:
            res = llvm_clang_c.split()
        elif tgt_llvm:
            target_cc = overrides.get('CHPL_TARGET_CC')
            if target_cc:
                res = target_cc.split()
    elif lang_upper == 'CXX':
        llvm_clang_cxx = overrides.get('CHPL_LLVM_CLANG_CXX')
        if llvm_clang_cxx:
            res = llvm_clang_cxx.split()
        elif tgt_llvm:
            target_cc = overrides.get('CHPL_TARGET_CXX')
            if target_cc:
                res = target_cc.split()
    else:
        error('unknown lang value {}'.format(lang))

    if res is not None:
        # validate that the command is clang by checking a preprocessor macro
        macro = "__clang_major__"
        exists, returncode, out, _ = try_run_command([res[0], "-E", "-x", "c", "-"], macro)
        if exists and returncode == 0 and out:
            # check that the preprocess output does not contains the macro
            if macro in out:
                error("'{}' is not clang and cannot be used with LLVM".format(res[0]))
        else:
            warning("unable to execute '{}' to validate it".format(res[0]))

    return res

# given a lang argument of 'c' or 'c++'/'cxx', return the system clang command
# to use. Checks that the clang version matches the version of llvm-config in
# use. Returns '' if no acceptable system clang was found.
@memoize
def get_system_llvm_clang(lang):
    provided = get_overriden_llvm_clang(lang)
    if provided:
        return provided[0]

    # Otherwise, look for an acceptable clang in the
    # llvm-config --bindir and in PATH.
    llvm_config = find_system_llvm_config()
    llvm_version_string = get_llvm_config_version(llvm_config)

    if llvm_version_string == None:
        return ''

    llvm_version = llvm_version_string.strip()

    # We expect the executable to be called either clang or clang-<version>
    llvm_major = llvm_version.split(".")[0]
    clang_name = get_llvm_clang_command_name(lang)
    clang_suffixes = ["", "-" + llvm_major]

    # We expect to find clang either in `llvm-config --bindir` or on PATH
    bindir = get_system_llvm_config_bindir()
    clang_prefixes = []
    if bindir is not None:
        clang_prefixes.append(bindir)
    clang_prefixes.append(None)

    for prefix in clang_prefixes:
        for suffix in clang_suffixes:
            clang_path = clang_name + suffix
            if prefix is not None:
                clang_path = os.path.join(prefix, clang_path)
            # Use the full path to clang. For some reason, this is important
            # on Alpine Linux for clang to find its own standard lib headers.
            if '/' not in clang_path:
                clang_path = which(clang_path)
            if is_system_clang_version_ok(clang_path):
                return clang_path
    return ''

# lang should be C or CXX
# returns [] list with the first element the clang command,
# then necessary arguments
@memoize
def get_llvm_clang(lang):

    # if it was provided by a user setting, just use that
    provided = get_overriden_llvm_clang(lang)
    if provided:
        return provided

    clang = None
    llvm_val = get()
    if llvm_val == 'system':
        clang = get_system_llvm_clang(lang)
    elif llvm_val == 'bundled':
        clang_name = get_llvm_clang_command_name(lang)
        llvm_subdir = get_bundled_llvm_dir()
        clang = os.path.join(llvm_subdir, 'bin', clang_name)

    if not clang:
        return ['']

    # tack on arguments that control clang's function
    result = [clang] + get_clang_basic_args(clang)
    return result


def has_compatible_installed_llvm():
    llvm_config = find_system_llvm_config()

    if llvm_config:
        (ok, errMsg) = check_llvm_packages(llvm_config)

        if ok:
            clang_c_command = get_system_llvm_clang('c')
            clang_cxx_command = get_system_llvm_clang('c++')

            if (is_system_clang_version_ok(clang_c_command) and
                is_system_clang_version_ok(clang_cxx_command)):
                return True

    # otherwise, something went wrong, so return False
    return False

# Returns the setting to use for CHPL_LLVM
#  CHPL_LLVM=none means build the compiler without enabling LLVM and clang
#                 integration (but note the LLVM support library is still used)
#  CHPL_LLVM=system means to use a system install of LLVM and clang
#  CHPL_LLVM=bundled means to use the bundled version of LLVM and clang
@memoize
def get():
    llvm_val = overrides.get('CHPL_LLVM')
    if not llvm_val:
        llvm_val = 'unset'

        if compatible_platform_for_llvm():
            if is_included_llvm_built('bundled'):
                llvm_val = 'bundled'
            elif has_compatible_installed_llvm():
                llvm_val = 'system'
            # otherwise, the default remains 'unset' -- ask user

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
def _get_gcc_prefix_dir_inner():
    # helper memoization for get_gcc_prefix_dir, only should be called from there

    gcc_prefix = ''

    # darwin and FreeBSD default to clang
    # so shouldn't need GCC prefix
    host_platform = chpl_platform.get('host')
    if host_platform == "darwin" or host_platform == "freebsd":
        return ''

    # When 'gcc' is a command other than '/usr/bin/gcc',
    # compute the 'gcc' prefix that LLVM should use.
    gcc_path = which('gcc')
    if gcc_path == '/usr/bin/gcc':
        # In this common case, nothing else needs to be done,
        # because we can assume that clang can find this gcc.
        pass
    elif gcc_path is None:
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

    gcc_prefix = gcc_prefix.strip()
    if gcc_prefix == '/usr':
        # clang will be able to figure this out so don't
        # bother with the argument here.
        gcc_prefix = ''

    return gcc_prefix

def get_gcc_prefix_dir(clang_cfg_args):
    gcc_prefix = overrides.get('CHPL_LLVM_GCC_PREFIX', '')

    # allow CHPL_LLVM_GCC_PREFIX=none to disable inferring it
    if gcc_prefix == 'none':
        return ''

    # return the value if it was set, regardless of whether it is valid
    if gcc_prefix:
        return gcc_prefix

    # if gcc-install-dir is in the config, don't try and infer gcc-toolchain
    if any(arg.startswith("--gcc-install-dir=") for arg in clang_cfg_args):
        return ''

    # this is in a separate function so the logic can be memoized
    # this function can't be memoized because 'clang_cfg_args' is a list and cannot be hashed.
    return _get_gcc_prefix_dir_inner()

@memoize
def is_gcc_install_dir_supported():
    llvm_version = get_llvm_version()
    return llvm_version not in ('11', '12', '13', '14', '15')

@memoize
def get_gcc_install_dir():
    gcc_dir = overrides.get('CHPL_LLVM_GCC_INSTALL_DIR', '')

    flag_supported = is_gcc_install_dir_supported()

    if gcc_dir:
        if not flag_supported:
            llvm_version = get_llvm_version()
            warning("This LLVM / clang version {0} is too old to use "
                    "CHPL_LLVM_GCC_INSTALL_DIR -- "
                    "it will be ignored".format(llvm_version))
            gcc_dir = ''

        # allow CHPL_LLVM_GCC_INSTALL_DIR=none to disable inferring it
        if gcc_dir == 'none':
            gcc_dir = ''

    elif flag_supported:
        # compute the GCC that LLVM should use.
        # this logic is based on the same logic in the LLVM spack package
        _, _, stdout, _ = try_run_command(
            ["gcc", "-print-file-name=libgcc.a"], combine_output=True)
        if stdout:
            gcc_dir_path = stdout.strip()
            if os.path.abspath(gcc_dir_path):
                dirname = os.path.dirname(gcc_dir_path)
                gcc_dir = dirname if os.path.exists(dirname) else gcc_dir

    return gcc_dir


# The bundled LLVM does not currently know to look in a particular Mac OS X SDK
# so we provide a -isysroot arg to indicate which is used.
#
# Potential alternatives to -isysroot here include:
#  * using the environment variable SDKROOT
#  * providing a cmake argument to adjust the clang build
#    e.g.
#      DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
#  * using -mmacosx-version-min=11.2 e.g.
#
# Additionally the -resource-dir arg indicates where to find some clang
# dependencies.
#
# Returns a [ ] list of args
@memoize
def get_sysroot_resource_dir_args():
    args = [ ]
    llvm_val = get()
    if llvm_val == "bundled":
        # Add -isysroot and -resourcedir based upon what 'clang' uses
        cfile = os.path.join(get_chpl_home(),
                             "runtime", "include", "sys_basic.h")
        if not os.path.isfile(cfile):
            error("error computing isysroot -- sys_basic.h is missing")

        (out, err) = run_command(['clang', '-###', cfile],
                                 stdout=True, stderr=True)
        out += err

        found = re.search('"-isysroot" "([^"]+)"', out)
        if found:
            args.append('-isysroot')
            args.append(found.group(1).strip())

        found = re.search('"-resource-dir" "([^"]+)"', out)
        if found:
            args.append('-resource-dir')
            args.append(found.group(1).strip())

    return args

# On some systems, it may be necessary to provide a --sysroot and/or a explicit dynamic linker
@memoize
def get_sysroot_linux_args():
    args = [ ]
    # try invoking the system gcc to see if it needs various extra flags
    dummy_main = '#include <stdio.h>\nint main() { return 0; }\n'
    _, _, stdout, _ = try_run_command(
        ["gcc", "-v", "-x", "c", "-", "-o", "/dev/null"],
        combine_output=True, cmd_input=dummy_main
    )
    if stdout:
        # on some platforms, it may be necessary to provide a --sysroot.
        # note: this regex does not handle paths with spaces
        found = re.search(r'--(?:with-)sysroot(?:=|\s+)([^ ]+)', stdout)
        if found:
            args.append('--sysroot')
            args.append(found.group(1).strip())

        # on some platforms, it may be necessary to provide -dynamic-linker explicitly
        # note: this regex does not handle paths with spaces
        found = re.search(r'-dynamic-linker(?:=|\s+)([^ ]+)', stdout)
        if found:
            dyn_linker = found.group(1).strip()
            # if this is path starts with '/lib/' or '/lib64/' then its the
            # default and we don't need to override it
            if not (
                dyn_linker.startswith('/lib/') or dyn_linker.startswith('/lib64/')
            ):
                # this linker flags needs some tricks to be passed to the compiler
                # wrap in '--[start|end]-no-unused-arguments' to prevent compiler warnings
                # pass as '-Wl,-dynamic-linker,<path>'
                args.append('--start-no-unused-arguments')
                args.append('-Wl,-dynamic-linker,' + dyn_linker)
                args.append('--end-no-unused-arguments')
    return args

# When a system LLVM is installed with Homebrew, it's very important
# to use the same Mac OS X libraries as what the Homebrew LLVM used.
# This function helps us to do that.
# Note that the goal here is to find the Mac OS X system libraries
# version that the system LLVM was built with, rather than
# to find out what it is currently configured to compile for;
# so e.g. setting SDKROOT should not change the value returned here.
@memoize
def get_system_llvm_built_sdkroot():
    # Homebrew installs of clang configure it with DEFAULT_SYSROOT
    # set to the SDKROOT being used when building.
    #
    # We could alternatively run
    #   dyld_info -platform /usr/local/opt/llvm@12/lib/libLLVM.dylib
    #     (on Monterey or later), or
    #   otool -lL /usr/local/opt/llvm@12/lib/libLLVM.dylib | grep sdk
    #
    # and then give that version to xcrun (say it is 12.1):
    #   xcrun --sdk macosx12.1 --show-sdk-path
    # and this should be the same as the DEFAULT_SYSROOT but
    # there might be a symlink pointing to the same place.

    llvm_val = get()
    host_platform = chpl_platform.get('host')
    if llvm_val == 'system' and host_platform == 'darwin':
        llvm_config = get_llvm_config()
        include_dir = run_command([llvm_config, '--includedir']).strip()
        if os.path.isdir(include_dir):
            clang_config = os.path.join(include_dir,
                                       'clang', 'Config', 'config.h')
            if os.path.exists(clang_config):
                with open(clang_config) as f:
                    for line in f.readlines():
                        # Looking for /some/path in #define DEFAULT_SYSROOT "/some/path"
                        if 'DEFAULT_SYSROOT' in line:
                            path = line.split('DEFAULT_SYSROOT')[1].strip()
                            # remove quotes around it
                            path = path.strip('"')
                            return path
    return None


@memoize
def _determine_gcc_flag_to_use():
    # note: this is a helper for get_clang_basic_args, its declared here so the
    # memoization can be shared and possible warnings only occur once
    flag_to_use = None
    if (overrides.get('CHPL_LLVM_GCC_INSTALL_DIR') is not None and
        overrides.get('CHPL_LLVM_GCC_PREFIX') is not None):
        # the user set both, warn
        base_msg = "Both CHPL_LLVM_GCC_INSTALL_DIR and CHPL_LLVM_GCC_PREFIX are set."
        if is_gcc_install_dir_supported():
            flag_to_use = 'CHPL_LLVM_GCC_INSTALL_DIR'
            warning("{} CHPL_LLVM_GCC_INSTALL_DIR will be used.".format(base_msg))
        else:
            flag_to_use = 'CHPL_LLVM_GCC_PREFIX'
            warning("{} CHPL_LLVM_GCC_PREFIX will be used.".format(base_msg))

    elif overrides.get('CHPL_LLVM_GCC_INSTALL_DIR') is not None:
        # the user set CHPL_LLVM_GCC_INSTALL_DIR, use it
        flag_to_use = 'CHPL_LLVM_GCC_INSTALL_DIR'
    elif overrides.get('CHPL_LLVM_GCC_PREFIX') is not None:
        # the user set CHPL_LLVM_GCC_PREFIX, use it
        flag_to_use = 'CHPL_LLVM_GCC_PREFIX'

    return flag_to_use

@memoize
def _get_clang_cfg_args(clang_command):
    clang_cfg = get_clang_cfg_file(clang_command)
    return parse_clang_cfg_file(clang_cfg)

# On some systems, we need to give clang some arguments for it to
# find the correct system headers.
#  * when PrgEnv-gnu is loaded on an XC, we should provide
#    a --gcc-toolchain argument indicating the prefix of that GCC installation.
#
#  * on Mac OS X, we might need to provide -mlinker-version=450
#    on 10.14 and in some cases -isysroot and -resource-dir.
#
# Returns a [ ] list of args
@memoize
def get_clang_basic_args(clang_command):
    clang_args = [ ]

    # Check that the clang configure file doesn't already supply
    # info about the sysroot or gcc install so we don't end up overriding it.
    clang_cfg_args = _get_clang_cfg_args(clang_command)
    if (any(arg.startswith("--sysroot") for arg in clang_cfg_args) or
        any(arg.startswith("--gcc-install-dir") for arg in clang_cfg_args) or
        any(arg.startswith("--gcc-toolchain") for arg in clang_cfg_args)):
        return clang_args

    @memoize
    def _get_gcc_prefix_dir():
        # read the args that clang will use by default from the config file
        return get_gcc_prefix_dir(clang_cfg_args)

    gcc_prefix_flags = {
        "CHPL_LLVM_GCC_INSTALL_DIR": ('--gcc-install-dir=', get_gcc_install_dir),
        "CHPL_LLVM_GCC_PREFIX": ('--gcc-toolchain=', _get_gcc_prefix_dir),
    }
    def use_flag(flag_to_use):
        flag, func = gcc_prefix_flags[flag_to_use]
        val = func()
        if val:
            clang_args.append(flag + val)
            return True
        return False

    # if the user set one of the flags, use it
    flag_to_use = _determine_gcc_flag_to_use()
    if flag_to_use:
        use_flag(flag_to_use)
    else:
        # we should try and infer them, preferring GCC_INSTALL_DIR
        for try_flag in ['CHPL_LLVM_GCC_INSTALL_DIR', 'CHPL_LLVM_GCC_PREFIX']:
            if use_flag(try_flag):
                break

    target_platform = chpl_platform.get('target')
    sysroot_args = []
    if target_platform == "darwin":
        sysroot_args = get_sysroot_resource_dir_args()
    else:
        sysroot_args = get_sysroot_linux_args()
    if sysroot_args:
        clang_args.extend(sysroot_args)

    # This is a workaround for problems with Homebrew llvm@11 on 10.14
    # which avoids errors like
    #  ld: unknown option: -platform_version
    target_platform = chpl_platform.get('target')
    if target_platform == "darwin":
        os_ver = chpl_platform.get_mac_os_version()
        if os_ver.startswith('10.14'):
            clang_args.append('-mlinker-version=450')

    return clang_args

def parse_clang_cfg_file(file):
    """
    Parse the clang config file and return a list of the arguments
    """
    args = []
    if file is None or not os.path.exists(file):
        return args

    with open(file) as f:
        cur_line = ""
        for line in f:
            if line.startswith('#'):
                continue
            cur_line += line.strip()
            if cur_line.endswith('\\'):
                # remove the trailing backslash and continue
                cur_line = cur_line.removesuffix('\\')
            else:
                args.append(cur_line)
                cur_line = ""
    return args

@memoize
def gather_pe_chpl_pkgconfig_libs():
    # Don't do anything if we aren't using a PrgEnv compiler
    if chpl_compiler.get_prgenv_compiler() == 'none':
        return ""

    import chpl_comm, chpl_comm_substrate, chpl_aux_filesys, chpl_libfabric

    platform = chpl_platform.get('target')
    comm = chpl_comm.get()
    substrate = chpl_comm_substrate.get()
    auxfs = chpl_aux_filesys.get()

    ret = os.environ.get('PE_CHAPEL_PKGCONFIG_LIBS', '')
    if comm != 'none':
        if platform != 'hpe-cray-ex':
            # Adding -lhugetlbfs gets the PrgEnv driver to add the appropriate
            # linker option for static linking with it. While it's not always
            # used with Chapel programs, it is expected to be the common case
            # when running on a Cray X*, so just always linking it is ok.
            # (We don't add it for HPE Cray EX systems, where it's not needed.)
            ret = 'craype-hugetlbfs:cray-pmi:' + ret

        if platform.startswith('cray-x'):
            ret = 'cray-ugni:' + ret

        if comm == 'gasnet' and substrate == 'aries':
            ret = 'cray-udreg:' + ret

        if comm == 'ofi' and chpl_libfabric.get() == 'system':
            ret = 'libfabric:' + ret

        # on login/compute nodes, lustre requires the devel api to make
        # lustre/lustreapi.h available (it's implicitly available on esl nodes)
        if "lustre" in auxfs:
            import third_party_utils
            pkg = "cray-lustre-api-devel"
            if third_party_utils.pkgconfig_system_has_package(pkg):
                ret = pkg + ":" + ret

    return ret

# Gather the compiler arguments that are automatically provided
# to a PrgEnv compiler.
# Returns (compileArgsList, linkArgsList)
@memoize
def get_clang_prgenv_args():

    platform = chpl_platform.get('target')
    comp_args = [ ]
    link_args = [ ]

    if chpl_compiler.get_prgenv_compiler() != 'none':
        # When running on a PrgEnv system, gather the PrgEnv arguments

        # Set up the environment to make the proper libraries and include
        # files available.
        os.environ['PE_PKGCONFIG_PRODUCTS'] = (
            'PE_CHAPEL:' + os.environ.get('PE_PKGCONFIG_PRODUCTS', ''))

        os.environ['PE_CHAPEL_MODULE_NAME'] = 'chapel'
        os.environ['PE_CHAPEL_PKGCONFIG_LIBS'] = gather_pe_chpl_pkgconfig_libs()

        # Use cc --cray-print-opts=... to get arguments from compiler driver

        # Get compilation arguments
        opts = run_command(['cc', '--cray-print-opts=cflags'])
        comp_args.extend(opts.split())

        # Get link arguments
        opts = run_command(['cc', '--cray-print-opts=libs'])
        link_args.extend(opts.split())

    return (comp_args, link_args)

@memoize
def get_clang_gcc_system_include_directories(flag, lang):
    """
    Runs 'COMPILER -E -Wp,-v -' to determine the system include directories
    """
    directories = []
    compiler = chpl_compiler.get_compiler_command(flag, lang)
    _, _, stdout, _ = try_run_command(
        compiler + ["-E", "-Wp,-v", "-", "-o", "/dev/null"],
        combine_output=True, cmd_input=""
    )
    if stdout:
        lines = stdout.splitlines()
        start_includes_regex = re.compile(r"#include.+search starts here")
        end_includes_regex = re.compile(r"End of search list")
        ignoring_regex = re.compile(r"(?:ignoring nonexistent)|(?:as it is a non-system)")
        collecting = False
        for line in lines:
            if start_includes_regex.search(line):
                collecting = True
            elif end_includes_regex.search(line):
                collecting = False
                break
            elif collecting and not ignoring_regex.search(line):
                directories.append(line.strip())
    return directories

@memoize
def is_path_clang_gcc_system_include_directory(path, flag, lang):
    """
    Returns True if the given path is a system include directory
    """
    directories = get_clang_gcc_system_include_directories(flag, lang)
    p = os.path.realpath(os.path.abspath(path))
    return any([os.path.realpath(os.path.abspath(d)) == p for d in directories])

@memoize
def can_infer_system_include_directories(flag, lang):
    """
    Returns True if the system include directories can be inferred
    """
    compiler = chpl_compiler.get_compiler_command(flag, lang)
    inferred = chpl_compiler.get_compiler_from_command(compiler[0])
    return inferred in ('clang', 'gnu')


# Filters out C++ compilation flags from llvm-config.
# The flags are passed as a list of strings.
# Returns a list of strings containing the kept flags.
def filter_llvm_config_flags(llvm_val, flags):
    ret = [ ]

    platform_val = chpl_platform.get('host')
    cygwin = platform_val.startswith('cygwin')
    darwin = platform_val.startswith('darwin')
    gnu = chpl_compiler.get('host') == 'gnu'

    for flag in flags:
        if (flag == '-DNDEBUG' or
            flag == '-fPIC' or
            flag == '-gsplit-dwarf' or
            flag.startswith('-O') or
            flag == '-pedantic' or
            flag == '-Wno-class-memaccess' or
            (darwin and gnu and flag.startswith('-stdlib=')) or
            (cygwin and flag == '-std=c++17') or
            flag == '-std=c++14'):
            continue # filter out these flags

        #
        # include LLVM headers as system headers using -isystem
        # this avoids warnings inside of LLVM headers by treating LLVM headers
        #
        # If the header is already a system header, using -isystem will break
        # the include search. In that case, just use -I (which technically wont do anythings)
        #
        if flag.startswith('-I'):
            directory = flag[2:]
            if llvm_val == "system":
                can_infer = can_infer_system_include_directories("host", "c++")
                is_system_inc = is_path_clang_gcc_system_include_directory(
                    directory, "host", "c++"
                )
                if can_infer and not is_system_inc:
                    # the directory not included by the compiler,
                    # so its safe to use -isystem
                    ret.append("-isystem" + directory)
                else:
                    # technically don't need to explicitly add the directory to
                    # the include path if it's a system header. but it is
                    # harmless to use just '-I'
                    # if we can't determine the system include directories,
                    # we have to add it anyways
                    ret.append("-I" + directory)
            else:
                # for the bundled LLVM it is always safe to use -isystem
                ret.append("-isystem" + directory)
            continue

        if flag.startswith('-W'):
            if flag.startswith('-Wno-'):
                ret.append(flag) # include -Wno- flags
            else:
                continue # filter out other -W flags
        else:
            ret.append(flag)

    return ret

# Filters out link flags from llvm-config.
# The flags are passed as a list of strings.
# Returns a list of strings containing filtered flags.
def filter_llvm_link_flags(flags):
    ret = [ ]
    for flag in flags:
        # remove -llibxml2.tbd which seems to appear on some Mac OS X versions
        # with LLVM 11.
        # TODO: can we remove this workaround?
        if flag == '-llibxml2.tbd':
            continue

        # LLVM 15 detects libzstd on some systems but doesn't include
        # the -L path from pkg-config (this can happen in a Spack configuration)
        # So, if we have '-lzstd', use pkg-config to get the link flags.
        if flag == '-lzstd':
          import third_party_utils
          if third_party_utils.has_pkgconfig():
              link_bundled_args, link_system_args = (
                  third_party_utils.pkgconfig_get_system_link_args('libzstd'))
              if link_system_args:
                  # found something with pkg-config, so use that instead
                  ret.extend(link_system_args)
                  continue
        # otherwise, append -lzstd as usual

        ret.append(flag)

    return ret

# returns (bundled, system) args for 'make'
# to compile C++ 'chpl' source code with LLVM
@memoize
def get_host_compile_args():
    bundled = [ ]
    system = [ ]

    llvm_val = get()
    llvm_support_val = get_llvm_support()
    llvm_config = get_llvm_config()

    # quit early if the llvm value is unset
    if llvm_val == 'unset':
        return (bundled, system)

    if llvm_support_val == 'system':
        # Ubuntu 16.04 needed -fno-rtti for LLVM 3.7
        # tested on that system after installing
        #   llvm-3.7-dev llvm-3.7 clang-3.7 libclang-3.7-dev libedit-dev
        # TODO: is this still needed?
        system.append('-fno-rtti')

        # Note, the cxxflags should include the -I for the include dir
        cxxflags = run_command([llvm_config, '--cxxflags'])
        system.extend(filter_llvm_config_flags('system', cxxflags.split()))

    elif llvm_support_val == 'bundled':
        # don't try to run llvm-config if it's not built yet
        if is_included_llvm_built(llvm_val):
            # Note, the cxxflags should include the -I for the include dir
            cxxflags = run_command([llvm_config, '--cxxflags'])
            bundled.extend(filter_llvm_config_flags('bundled', cxxflags.split()))

        # TODO: is this still needed?
        bundled.append('-Wno-comment')

    if llvm_val == 'system' or llvm_val == 'bundled':
        bundled.append('-DHAVE_LLVM')

    return (bundled, system)

# returns (bundled, system, static_or_dynamic)
#  * bundled and system are lists of args for 'make' to link 'chpl' with LLVM
#  * static_or_dynamic is "static" for static linking or "dynamic"
#    for dynamic linking or None for unknown
@memoize
def compute_host_link_settings():
    bundled = [ ]
    system = [ ]

    llvm_dynamic = True
    llvm_val = get()
    llvm_support_val = get_llvm_support()
    llvm_config = get_llvm_config()

    # If llvm_config is not discoverable (e.g. we're configured to use a system
    # LLVM and LLVM is not installed or we have an incompatible version
    # installed) return a dummy value. If necessary, elsewhere in printchplenv
    # (validate_llvm_config) we will recognize this and present an error to the
    # user.
    if llvm_config == "" or llvm_config is None:
        return ('', '', None)

    clang_static_libs = ['-lclangFrontend',
                         '-lclangSerialization',
                         '-lclangDriver',
                         '-lclangCodeGen',
                         '-lclangParse',
                         '-lclangSema',
                         '-lclangAnalysis',
                         '-lclangEdit',
                         '-lclangASTMatchers',
                         '-lclangAST',
                         '-lclangLex',
                         '-lclangBasic']

    llvm_components = ['bitreader',
                       'bitwriter',
                       'ipo',
                       'instrumentation',
                       'option',
                       'objcarcopts',
                       'profiledata',
                       'all-targets',
                       'coverage',
                       'coroutines',
                       'lto']

    if llvm_val == 'system' or llvm_val == 'bundled':
        llvm_version = get_llvm_version()
        # Starting with clang 15, clang needs additional libraries
        if llvm_version not in ('11', '12', '13', '14'):
            clang_static_libs.append('-lclangSupport')
            llvm_components.append('windowsdriver')
        # Starting with clang 16, clang needs additional libraries
        if llvm_version not in ('11', '12', '13', '14', '15'):
            llvm_components.append('frontendhlsl')
        # Starting with clang 18, clang needs additional libraries
        if llvm_version not in ('11', '12', '13', '14', '15', '16', '17'):
            llvm_components.append('frontenddriver')
            # clangAPINotes must go immediately after clangSema
            idx = clang_static_libs.index('-lclangSema') + 1
            clang_static_libs.insert(idx, '-lclangAPINotes')

    # quit early if the llvm value is unset
    if llvm_val == 'unset':
        return (bundled, system, None)

    # only use LLVMSupport for CHPL_LLVM=none
    if llvm_val == 'none':
        clang_static_libs = [ ]
        llvm_components = ['support']

    if llvm_support_val == 'system':
        # For LLVM version 11 and older, there was a problem where
        # 'llvm-config' did not work properly on Mac OS X, so work around
        # that by using static linking.
        llvm_version = get_llvm_version()
        host_platform = chpl_platform.get('host')
        if host_platform == 'darwin' and llvm_version == '11':
            llvm_dynamic = False

        # Also, change to static, if llvm-config indicates static linking
        shared_mode = run_command([llvm_config, '--shared-mode'])
        if shared_mode.strip() == 'static':
            llvm_dynamic = False

        # Make sure to put clang first on the link line
        # because depends on LLVM libraries
        if llvm_dynamic:
            system.append('-lclang-cpp')
        else:
            system.extend(clang_static_libs)

        libdir = run_command([llvm_config, '--libdir'])
        if libdir:
            libdir = libdir.strip()
            system.append('-L' + libdir)
            system.append('-Wl,-rpath,' + libdir)

        ldflags = run_command([llvm_config,
                               '--ldflags', '--system-libs', '--libs'] +
                              llvm_components)
        if ldflags:
            system.extend(filter_llvm_link_flags(ldflags.split()))

    elif llvm_support_val == 'bundled':
        # Link statically for now for the bundled configuration
        # If this changes in the future:
        # * check for problems finding libstdc++ with different PrgEnv compilers
        # * make sure that 'make install' works correctly in terms of any
        #   rpaths embedded in the executable
        llvm_dynamic = False

        # don't try to run llvm-config if it's not built yet
        if is_included_llvm_built(llvm_val):

            libdir = run_command([llvm_config, '--libdir'])
            if libdir:
                libdir = libdir.strip()
                bundled.append('-L' + libdir)
                bundled.append('-Wl,-rpath,' + libdir)

            if llvm_val != 'none':
                if llvm_dynamic:
                    bundled.append('-lclang-cpp')
                else:
                    bundled.extend(clang_static_libs)

            ldflags = run_command([llvm_config,
                                   '--ldflags', '--libs'] +
                                  llvm_components)

            bundled.extend(ldflags.split())

            ldflags = run_command([llvm_config,
                                   '--system-libs'] +
                                   llvm_components)

            if ldflags:
                system.extend(filter_llvm_link_flags(ldflags.split()))

        else:
            warning("included llvm not built yet")

    static_dynamic = "static"
    if llvm_dynamic:
        static_dynamic = "dynamic"

    return (bundled, system, static_dynamic)

# returns whether LLVM and clang will be linked dynamically, statically,
# or it is unknown, by returning "dynamic" or "static" or None.
@memoize
def get_static_dynamic():
    bundled, system, static_dynamic = compute_host_link_settings()
    return static_dynamic

# returns (bundled, system) args for 'make' to link 'chpl' with LLVM
@memoize
def get_host_link_args():
    bundled, system, static_dynamic = compute_host_link_settings()
    return (bundled, system)

# Return the isysroot argument provided by get_clang_basic_args, if any
def get_clang_args_sdkroot():
    args = get_clang_basic_args(None)
    bundled, system = get_host_compile_args()
    args += bundled
    args += system

    return_next_arg = False
    for arg in args:
        if return_next_arg:
            return arg

        if arg == "-isysroot":
            return_next_arg = True

    return ''


def _main():
    llvm_val = get()
    llvm_config = get_llvm_config()
    llvm_versions = llvm_versions_string()

    parser = optparse.OptionParser(usage='usage: %prog [--needs-llvm-runtime]')
    parser.add_option('--needs-llvm-runtime', dest='action',
                      action='store_const',
                      const='needsllvm', default='')
    parser.add_option('--llvm-config', dest='action',
                      action='store_const',
                      const='llvmconfig', default='')
    parser.add_option('--llvm-vesion', dest='action',
                      action='store_const',
                      const='llvmversion', default='')
    parser.add_option('--supported-versions', dest='action',
                      action='store_const',
                      const='llvmversions', default='')
    parser.add_option('--sdkroot', dest='action',
                      action='store_const',
                      const='sdkroot', default='')
    parser.add_option('--quickstart', dest='action',
                      action='store_const',
                      const='quickstart', default='')



    (options, args) = parser.parse_args()

    # if --needs-llvm-runtime is set, print out llvm if runtime is needed,
    # and print out nothing if it is not.
    if options.action == 'needsllvm':
        if llvm_val == 'system' or llvm_val == 'bundled':
            sys.stdout.write("llvm\n")
    elif options.action == 'llvmconfig':
        sys.stdout.write("{0}\n".format(llvm_config))
        validate_llvm_config()
    elif options.action == 'llvmversion':
        llvm_version = get_llvm_version()
        sys.stdout.write("{0}\n".format(llvm_version))
    elif options.action == 'llvmversions':
        sys.stdout.write("{0}\n".format(llvm_versions))
    elif options.action == 'sdkroot':
        sys.stdout.write("{0}\n".format(get_clang_args_sdkroot()))
    elif options.action == 'quickstart':
        if has_compatible_installed_llvm() and compatible_platform_for_llvm():
            sys.stdout.write("system\n")
        else:
            sys.stdout.write("none\n")
    else:
        sys.stdout.write("{0}\n".format(llvm_val))


if __name__ == '__main__':
    _main()
