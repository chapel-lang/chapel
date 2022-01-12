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
    return ('12','11',)

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
    llvm_config = overrides.get('CHPL_LLVM_CONFIG', 'none')
    if llvm_config != 'none':
        return llvm_config

    homebrew_prefix = chpl_platform.get_homebrew_prefix()

    paths = [ ]
    for vers in llvm_versions():
        paths.append("llvm-config-" + vers + ".0")
        paths.append("llvm-config-" + vers)
        # this format used by freebsd
        paths.append("llvm-config" + vers)
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
def validate_llvm_config():
    llvm_val = get()
    llvm_config = get_llvm_config()

    if llvm_val == 'system':
        if llvm_config == '' or llvm_config == 'none':
            error("CHPL_LLVM=system but could not find an installed LLVM"
                  " with one of the supported versions: {0}".format(
                  llvm_versions_string()))

        else:
            bindir = get_system_llvm_config_bindir()
            if not (bindir and os.path.isdir(bindir)):
                error("llvm-config command {0} provides missing bin dir {0}"
                      .format(llvm_config, bindir))
            clang_c = get_llvm_clang('c')[0]
            clang_cxx = get_llvm_clang('c++')[0]
            if not os.path.exists(clang_c):
                error("Missing clang command at {0}".format(clang_c))
            if not os.path.exists(clang_cxx):
                error("Missing clang++ command at {0}".format(clang_cxx))

    if (llvm_val == 'system' or
        (llvm_val == 'bundled' and os.path.exists(llvm_config))):
        version, config_error = check_llvm_config(llvm_config)
        if config_error:
            error("Problem with llvm-config at {0} -- {1}"
                  .format(llvm_config, config_error))


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

def get_system_llvm_clang(lang):
    clang_name = get_llvm_clang_command_name(lang)
    bindir = get_system_llvm_config_bindir()
    clang = ''
    if bindir:
        clang = os.path.join(bindir, clang_name)

    return clang

# lang should be C or CXX
# returns [] list with the first element the clang command,
# then necessary arguments
@memoize
def get_llvm_clang(lang):

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
    result = [clang] + get_clang_basic_args()
    return result


def has_compatible_installed_llvm():
    llvm_config = find_system_llvm_config()

    if llvm_config:
        clang_c_command = get_system_llvm_clang('c')
        clang_cxx_command = get_system_llvm_clang('c++')

        if (os.path.exists(clang_c_command) and
            os.path.exists(clang_cxx_command)):
            return True

    # otherwise, something went wrong, so return False
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
        # darwin and FreeBSD default to clang
        # so shouldn't need GCC prefix
        host_platform = chpl_platform.get('host')
        if host_platform == "darwin" or host_platform == "freebsd":
            return ''

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

        gcc_prefix = gcc_prefix.strip()
        if gcc_prefix == '/usr':
            # clang will be able to figure this out so don't
            # bother with the argument here.
            gcc_prefix = ''

    return gcc_prefix

# The bundled LLVM does not currently know to look in a particular Mac OS X SDK
# so we provide a -isysroot arg to indicate which is used.
#
# Potential alternatives to -isysroot here include:
#  * using the environment variable SDKROOT
#  * providing a cmake argument to adjust the clang build
#  * using -mmacosx-version-min=11.2 e.g.
#
# Additionally the -resource-dir arg indicates where to find some clang
# dependencies.
#
# Returns a [ ] list of args
@memoize
def get_sysroot_resource_dir_args():
    args = [ ]
    target_platform = chpl_platform.get('target')
    llvm_val = get()
    if (target_platform == "darwin" and llvm_val == "bundled"):
        # Add -isysroot and -resourcedir based upon what 'clang' uses
        cfile = os.path.join(get_chpl_home(),
                             "runtime", "include", "sys_basic.h")
        if not os.path.isfile(cfile):
            error("error computing isysroot -- sys_basic.h is missing")

        (out,err) = run_command(['clang', '-###', cfile],
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
def get_clang_basic_args():
    clang_args = [ ]

    gcc_prefix = get_gcc_prefix()
    if gcc_prefix:
        clang_args.append('--gcc-toolchain=' + gcc_prefix)

    sysroot_args = get_sysroot_resource_dir_args()
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
        if 'lustre' in auxfs:
          exists, returncode, out, err = try_run_command(
              ['pkg-config', '--exists', 'cray-lustre-api-devel'])
          if exists and returncode == 0:
            ret = 'cray-lustre-api-devel:' + ret

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
            'PE_CHAPEL:' + os.environ.get('PE_PKGCONFIG_PRODUCTS',''));

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

# Filters out C++ compilation flags from llvm-config.
# The flags are passed as a list of strings.
# Returns a list of strings containing filtered flags.
def filter_llvm_config_flags(flags):
    ret = [ ]

    for flag in flags:
        if (flag == '-DNDEBUG' or
            flag == '-fPIC' or
            flag == '-gsplit-dwarf' or
            flag.startswith('-O') or
            flag == '-pedantic' or
            flag == '-Wno-class-memaccess'):
            continue # filter out these flags

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
        ret.append(flag)

    return ret

# returns (bundled, system) args for 'make'
# to compile C++ 'chpl' source code with LLVM
@memoize
def get_host_compile_args():
    bundled = [ ]
    system = [ ]

    llvm_val = get()
    llvm_config = get_llvm_config()

    if llvm_val == 'system':
        # Ubuntu 16.04 needed -fno-rtti for LLVM 3.7
        # tested on that system after installing
        #   llvm-3.7-dev llvm-3.7 clang-3.7 libclang-3.7-dev libedit-dev
        # TODO: is this still needed?
        system.append('-fno-rtti')

        # Note, the cxxflags should include the -I for the include dir
        cxxflags = run_command([llvm_config, '--cxxflags'])
        system.extend(filter_llvm_config_flags(cxxflags.split()))

    elif llvm_val == 'bundled':
        # don't try to run llvm-config if it's not built yet
        if is_included_llvm_built():
            # Note, the cxxflags should include the -I for the include dir
            cxxflags = run_command([llvm_config, '--cxxflags'])
            bundled.extend(filter_llvm_config_flags(cxxflags.split()))

        # TODO: is this still needed?
        bundled.append('-Wno-comment')

    if llvm_val == 'system' or llvm_val == 'bundled':
        bundled.append('-DHAVE_LLVM')

    return (bundled, system)

# returns (bundled, system) args for 'make'
# to link 'chpl' with LLVM
@memoize
def get_host_link_args():
    bundled = [ ]
    system = [ ]

    llvm_dynamic = True
    llvm_val = get()
    llvm_config = get_llvm_config()
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


    if llvm_val == 'system':
        # Decide whether to try to link statically or dynamically.
        # Future work: consider using 'llvm-config --shared-mode'
        # to make this choice.
        host_platform = chpl_platform.get('host')
        if host_platform == 'darwin':
            llvm_dynamic = False

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

        if llvm_dynamic:
            system.append('-lclang-cpp')
        else:
            system.extend(clang_static_libs)

    elif llvm_val == 'bundled':
        # Link statically for now for the bundled configuration
        # If this changes in the future:
        # * check for problems finding libstdc++ with different PrgEnv compilers
        # * make sure that 'make install' works correctly in terms of any
        #   rpaths embedded in the executable
        llvm_dynamic = False

        # don't try to run llvm-config if it's not built yet
        if is_included_llvm_built():

            libdir = run_command([llvm_config, '--libdir'])
            if libdir:
                libdir = libdir.strip()
                bundled.append('-L' + libdir)
                bundled.append('-Wl,-rpath,' + libdir)

            if llvm_dynamic:
                bundled.append('-lclang-cpp')
            else:
                bundled.extend(clang_static_libs)

            ldflags = run_command([llvm_config,
                                   '--ldflags', '--libs'] +
                                   llvm_components)

            bundled.extend(ldflags.split())

            system_libs = run_command([llvm_config,
                                      '--system-libs'] +
                                      llvm_components)

            system.extend(system_libs.split())


        else:
            warning("included llvm not built yet")

    return (bundled, system)

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
