import os
import re

import chpl_cpu, chpl_arch, chpl_compiler
import chpl_lib_pic, chpl_locale_model, chpl_platform
from chpl_home_utils import get_chpl_home, get_chpl_third_party, using_chapel_module
from utils import error, memoize, run_command, warning, try_run_command
import homebrew_utils

#
# This is the default unique configuration path which
# is only appropriate for target.
#
@memoize
def default_uniq_cfg_path():
    cpu_val = chpl_cpu.get('target', map_to_compiler=True,
                           get_lcd=using_chapel_module()).cpu
    compiler_val = chpl_compiler.get_path_component('target')
    return '{0}-{1}-{2}-{3}-{4}'.format(chpl_platform.get('target'),
                                        chpl_arch.get('target'),
                                        cpu_val,
                                        compiler_val,
                                        chpl_lib_pic.get())

#
# Returns the path to the packages install directory
#
@memoize
def get_bundled_install_path(pkg, ucp=default_uniq_cfg_path()):
    return os.path.join(get_chpl_third_party(), pkg, 'install', ucp)

#
# Return libraries and other options mentioned in the old_library and
# dependency_libs entries in a libtool .la file, recursively searching
# other .la files encountered there.
#
@memoize
def handle_la(la_path):
    args = []
    if os.path.isfile(la_path):
        with open(la_path) as f:
            for line in f.readlines():
                if 'old_library=' in line:
                    lib_name = line.split('\'')[1]
                    p = re.compile(r'^lib([^/]+)\.a$')
                    args.append(p.sub(r'-l\1', lib_name))
                elif 'inherited_linker_flags=' in line:
                    for tok in line.split('\'')[1].split():
                        args.append(tok)
                elif 'dependency_libs=' in line:
                    for tok in line.split('\'')[1].split():
                        # paths reflect built env; replace with $CHPL_HOME
                        pat = re.compile(r'^((-L\s*)?).*(/third-party/)')
                        repl = r'\1' + get_chpl_third_party() + '/'
                        tok = pat.sub(repl, tok)
                        if tok.endswith('.la'):
                            args.extend(handle_la(tok))
                        else:
                            args.append(tok)
    return args

def filter_libs_skip_arg(arg):
    if arg == '-pthread':
        # ignore this flag since it causes problems
        # if ld is used as the linker (vs clang/gcc/etc),
        # and since Chapel programs always build with pthreads anyway
        return True

    if arg == '-L/usr/lib':
        # Ignore this flag since on some systems /usr/lib is 32-bit
        # and /usr/lib64 is 64-bit, so we would normally want /usr/lib64.
        # This is a workaround for building qthreads with CHPL_HWLOC=system
        # on Gentoo systems.
        return True

    return False

# Given bundled_libs and system_libs lists, filters some
# usual suspects into system_libs and
# returns (bundled_args, system_args)
def filter_libs(bundled_libs, system_libs):
    bundled_ret = [ ]
    system_ret = [ ]

    for arg in bundled_libs:
        if filter_libs_skip_arg(arg):
            # ignore any args we need to skip
            pass
        elif (arg == '-ldl' or
              arg == '-lm' or
              arg == '-lnuma' or
              arg == '-lpthread'):
            # put some of the usual suspects into the system args
            system_ret.append(arg)
        else:
            # otherwise include the flag in bundled
            # TODO: this put something like -L/usr/lib into system_ret instead.
            bundled_ret.append(arg)

    for arg in system_libs:
        if filter_libs_skip_arg(arg):
            # ignore any args we need to skip
            pass
        else:
            # otherwise include the flag in system
            system_ret.append(arg)

    return (bundled_ret, system_ret)

@memoize
def pkgconfig_system_has_package(pkg):
    exists, returncode, _, _ = try_run_command(['pkg-config', '--exists', pkg])
    return exists and not returncode

#
# Return compiler arguments required to use a system library known to
# pkg-config. The pkg argument should be the name of a system-installed
# package.
#
# Returns a 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def pkgconfig_get_system_compile_args(pkg):
    if not pkgconfig_system_has_package(pkg):
        return (None, None)
    # run pkg-config to get the cflags
    cflags_line = run_command(['pkg-config', '--cflags'] + [pkg])
    cflags = cflags_line.split()
    return ([ ], cflags)

# helper function to determine if we need to warn about 'Requires'/'Requires.private'
# these fields list other packages that are required to link with this package
# however, this only matters if the required package is not listed in
# 'Libs'/'Libs.private' already
# see https://people.freedesktop.org/~dbn/pkg-config-guide.html
def _pkgconfig_should_warn_for_requires(d, private=False):
    libs = d['Libs' if not private else 'Libs.private'].split()
    requires = d['Requires' if not private else 'Requires.private'].split()
    for req in requires:
        lib_name = '-l' + req
        if lib_name not in libs:
            return True
    return False

#
# Return compiler arguments required to use a bundled library
# based on a pkg-config .pc file.
#
# Even though pkg-config is not available by default on Mac OS X,
# many third-party packages create .pc files. This function can
# parse them. Additionally, it adjusts the output to replace
# paths to a previous third-party install directory with the current one.
#
# pkg is the name of the subdir in third-party
# ucp is the unique config path (defaults to default_uniq_cfg_path())
# pcfile is the name of the .pc file (defaults to <pkg>.pc)
#
# This function looks for
#   third-party/<pkg>/install/<ucp>/lib/pkgconfig/<pcfile>
#
# Returns a 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
def pkgconfig_get_bundled_compile_args(pkg, ucp='', pcfile=''):
    (d, pcpath) = read_bundled_pkg_config_file(pkg, ucp, pcfile)

    # Return empty tuple if no .pc file was found (e.g. pkg not built yet)
    if d == None:
        return ([ ], [ ])

    if 'Requires' in d and d['Requires']:
        warn = False
        if 'Libs' in d:
            warn = _pkgconfig_should_warn_for_requires(d)
        else:
            # no Libs, so no way to check if the required package is already included
            warn = True

        if warn:
            warning("Simple pkg-config parser does not handle Requires")
            warning("in {0}".format(pcpath))

    cflags = [ ]

    if 'Cflags' in d:
      cflags = d['Cflags'].split()

    return (cflags, [ ])

# default static value for pkgconfig_get_system_link_args
# and pkgconfig_get_bundled_link_args
def pkgconfig_default_static():
    static = chpl_platform.get('target')!='hpe-cray-ex'
    return static

#
# Return linker arguments required to link with a system library
# known to pkg-config. The pkg argument should be the name of a
# system-installed package.
#
# if static=True, uses --static (suitable for static linking)
#
# Returns a 2-tuple of lists
#  (link_bundled_args, link_system_args)
@memoize
def pkgconfig_get_system_link_args(pkg, static=pkgconfig_default_static()):
    if not pkgconfig_system_has_package(pkg):
        return (None, None)
    # run pkg-config to get the link flags
    static_arg = [ ]
    if static:
      static_arg = ['--static']

    libs_line = run_command(['pkg-config', '--libs'] + static_arg + [pkg])
    libs = libs_line.split()
    return ([ ], libs)

#
# Return linker arguments required to use a bundled library
# based on a pkg-config .pc file.
#
# Even though pkg-config is not available by default on Mac OS X,
# many third-party packages create .pc files. This function can
# parse them. Additionally, it adjusts the output to replace
# paths to a previous third-party install directory with the current one.
#
# pkg is the name of the subdir in third-party
# ucp is the unique config path
# pcfile is the name of the .pc file
# static indicates if Libs.private should be included for static linking.
#
# This function looks for
#   third-party/<pkg>/install/<ucp>/lib/pkgconfig/<pcfile>
#   where pcfile defaults to pkg.pc
#
# If add_rpath=True (the default) it will add an rpath linker flag for
#   third-party/<pkg>/install/<ucp>/lib/
#
# Returns a 2-tuple of lists
#  (link_bundled_args, link_system_args)
@memoize
def pkgconfig_get_bundled_link_args(pkg, ucp='', pcfile='',
                                    static=pkgconfig_default_static(),
                                    add_rpath=True):

    # compute the default ucp
    if ucp == '':
        ucp = default_uniq_cfg_path()
    # compute the default pcfile name
    if pcfile == '':
        pcfile = pkg + '.pc'

    install_path = get_bundled_install_path(pkg, ucp)
    lib_dir = os.path.join(install_path, 'lib')

    (d, pcpath) = read_bundled_pkg_config_file(pkg, ucp, pcfile)

    # Return empty tuple if no .pc file was found (e.g. pkg not built yet)
    if d == None:
        return ([ ], [ ])

    if d.get('Requires') and _pkgconfig_should_warn_for_requires(d):
        warning("Simple pkg-config parser does not handle Requires")
        warning("in {0}".format(pcpath))

    if (
        static
        and d.get("Requires.private")
        and _pkgconfig_should_warn_for_requires(d, private=True)
    ):
        warning("Simple pkg-config parser does not handle Requires.private")
        warning("in {0}".format(pcpath))

    libs = [ ]
    libs_private = [ ]

    if 'Libs' in d:
        libs = d['Libs'].split()

    if 'Libs.private' in d:
        libs_private = d['Libs.private'].split()

    # add the -rpath option if it was enabled by the caller
    if add_rpath:
        libs.append('-Wl,-rpath,' + lib_dir)

    # assuming libs_private stores system libs, like -lpthread
    return filter_libs(libs, libs_private)

@memoize
def has_pkgconfig():
    (exists, code, _stdout, _stderr) = try_run_command(['pkg-config',
                                                       '--version'])
    if exists and code == 0:
        return True
    return False

#
# This returns the default link args for the given third-party package
# assuming that the bundled version is used and uses a .la file
# to do so.
#
# Note that some systems remove .la files on installation of a package,
# and as a result, using this function can lead to portability problems.
#
# TODO: remove this function when it is no longer needed
#
# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
def libtool_get_bundled_link_args(pkg, ucp='', libs=[], add_L_opt=True):
    if ucp == '':
        ucp = default_uniq_cfg_path()
    if libs == []:
        libs = [ 'lib' + pkg + '.la' ]
    all_args = []
    lib_dir = os.path.join(get_bundled_install_path(pkg, ucp), 'lib')
    if add_L_opt:
        all_args.append('-L' + lib_dir)
        all_args.append('-Wl,-rpath,' + lib_dir)

    # gather the args from the .la, or fallback on just -lpkg
    for lib_arg in libs:
        if lib_arg.endswith('.la'):
            la = os.path.join(lib_dir, lib_arg)
            if os.path.isfile(la):
                all_args.extend(handle_la(la))
            else:
                # if we can't find 'libBLA.la' then add '-lBLA'.
                # this happens for some package installations
                x = lib_arg
                if x.startswith('lib'):
                    x = x[len('lib'):]
                if x.endswith('.la'):
                    x = x[:-len('.la')]
                all_args.append('-l' + x)
        else:
            all_args.append(lib_arg)

    return filter_libs(all_args, [ ])

#
# This returns the default compile args for the given third-party package
# assuming that the bundled version is used.
#
# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
def get_bundled_compile_args(pkg, ucp=''):
    if ucp == '':
        ucp = default_uniq_cfg_path()
    inc_dir = os.path.join(get_bundled_install_path(pkg, ucp), 'include')
    return (['-I' + inc_dir], [ ])

# apply substitutions like ${VARNAME} within string
# using the supplied dictionary d
def apply_pkgconfig_subs(s, d):
    # look for ${BLA} but not $${BLA}
    # since $${BLA} is pkg-config's escape form
    pattern = r'(?<!\$)(\${([^}]+)})'
    for m in re.findall(pattern, s):
        sub, key = m
        if key in d:
            s = s.replace(sub, d[key])

    return s

# Read a pkg-config .pc file at path pcpath into a dictionary
# Handles comments, variable escapes with ${variable},
# and literal $${variable} (which is not an escape).
# Assumes that the variables are defined before they are used
#
# Does not handle dependencies on other pkg-config files
# (i.e. Requires: lines). However this version is intended to
# be sufficient for the third-party packages we do have.
#
# If find_third_party and replace_third_party not None,
# they should be e.g.
#   find_third_party=third-party/gasnet/install/some-ucp
#   replace_third_party=/path/to/chpl-home/third-party/gasnet/install/some-ucp
# and this code will replace
#   /*/$find_third_party
# with
#  $replace_third_party
#
@memoize
def read_pkg_config_file(pcpath, find_third_party, replace_third_party):
    ret = { }

    pattern = None
    if find_third_party and replace_third_party:
        pattern = re.compile(r'/[^ ]*/' + find_third_party)

    with open(pcpath) as file:
        for line in file:
            line = line.strip()

            # pkg-config files can contain comments starting with #
            if line.startswith('#'):
                continue

            # look for a line like KEY=VALUE
            key, sep, val = line.partition('=')
            key = key.strip()
            val = val.strip()
            if sep == '=' and not " " in key:
                # substitute pkg-config file variables
                val = apply_pkgconfig_subs(val, ret)
                # fix up paths to third-party/ subdirs
                if pattern:
                    val = re.sub(pattern, replace_third_party, val)

                ret[key] = val
            else:
                # look for a line like KEY: VALUE
                key, sep, val = line.partition(':')
                key = key.strip()
                val = val.strip()
                if sep == ':' and not " " in key:
                    # substitute pkg-config file variables
                    val = apply_pkgconfig_subs(val, ret)
                    # fix up paths to third-party/ subdirs
                    if pattern:
                        val = re.sub(pattern, replace_third_party, val)
                    ret[key] = val

    return ret

# Helper function to call read_pkg_config_file for a bundled
# pkg-config file.
#
# pkg should be the bundled package name (e.g. gasnet)
# ucp is the unique config path (defaults to default_uniq_cfg_path())
# pcfile is the .pc file name (defaults to <pkg>.pc)
#
# This function looks for
#   third-party/<pkg>/install/<ucp>/lib/pkgconfig/<pcfile>
#   where pcfile defaults to pkg.pc
#
# Returns a tuple. The first member is None if the .pc file could not be read,
# otherwise it's a dictionary of values read from the .pc file. The second
# member is the name of the pc file, if it could be determined.
def read_bundled_pkg_config_file(pkg, ucp='', pcfile=''):
    # compute the default ucp
    if ucp == '':
        ucp = default_uniq_cfg_path()
    # compute the default pcfile name
    if pcfile == '':
        pcfile = pkg + '.pc'

    install_path = get_bundled_install_path(pkg, ucp)

    # give up early if the 3rd party package hasn't been built
    if not os.path.exists(install_path):
        return (None, None)

    pcpath = os.path.join(install_path, 'lib', 'pkgconfig', pcfile)

    # if we get this far, we should have a .pc file. check that it exists.
    if not os.access(pcpath, os.R_OK):
        error("Could not find '{0}'".format(pcpath), ValueError)
        return (None, pcpath)

    find_path = os.path.join('third-party', pkg, 'install', ucp)
    replace_path = install_path

    return (read_pkg_config_file(pcpath, find_path, replace_path), pcpath)


def could_not_find_pkgconfig_pkg(pkg, envname):
    if homebrew_utils.homebrew_exists() and homebrew_utils.homebrew_pkg_exists(pkg):
        # tell user to install pkg-config as well
        error("{0} is installed via homebrew, but pkg-config is not installed. Please install pkg-config with `brew install pkg-config`.".format(pkg))
    else:
        install_str = " with `brew install {0}`".format(pkg) if homebrew_utils.homebrew_exists() else ""
        error("Could not find a suitable {0} installation. Please install {0}{1} or set {2}=bundled.".format(pkg, install_str, envname))
