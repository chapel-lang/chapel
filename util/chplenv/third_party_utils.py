import os
import re

import chpl_cpu, chpl_arch, chpl_compiler
import chpl_lib_pic, chpl_locale_model, chpl_platform
import chpl_home_utils
from chpl_home_utils import get_chpl_home, get_chpl_third_party, using_chapel_module
from utils import error, memoize, run_command


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
def get_cfg_install_path(pkg, ucp=default_uniq_cfg_path()):
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

#
# Return compiler arguments required to use a library known to
# pkgconfig. The pkg can be a path to a .pc file or the name of a
# system-installed package or the name of a third-party package.
#
# Note that pkg-config is not available by default on Mac OS X.
#
# if system=True, searches for a system-installed package.
@memoize
def pkgconfig_get_compile_args(pkg, ucp='', system=True):
  havePcFile = pkg.endswith('.pc')
  pcArg = pkg
  if not havePcFile:
    if system:
      # check that pkg-config knows about the package in question
      run_command(['pkg-config', '--exists', pkg])
    else:
      # look for a .pc file
      if ucp == '':
        ucp = default_uniq_cfg_path()
      pcfile = pkg + '.pc' # maybe needs to be an argument later?

      pcArg = os.path.join(get_cfg_install_path(pkg, ucp), 'lib',
                           'pkgconfig', pcfile)

      if not os.access(pcArg, os.R_OK):
        error("Could not find '{0}'".format(pcArg), ValueError)

  cflags_line = run_command(['pkg-config', '--cflags'] + [pcArg]);
  cflags = cflags_line.split()
  return cflags

#
# Return linker arguments required to link with a library
# known to pkgconfig. The pkg can be a path to a .pc file or
# the name of a system-installed package or the name of
# a third-party package.
#
# if system=True, searches for a system-installed package.
# if static=True, uses --static (suitable for static linking)
@memoize
def pkgconfig_get_link_args(pkg, ucp='', system=True, static=(chpl_platform.get('target')!='hpe-cray-ex')):
  havePcFile = pkg.endswith('.pc')
  pcArg = pkg
  if not havePcFile:
    if system:
      # check that pkg-config knows about the package in question
      run_command(['pkg-config', '--exists', pkg])
    else:
      # look for a .pc file
      if ucp == '':
        ucp = default_uniq_cfg_path()
      pcfile = pkg + '.pc' # maybe needs to be an argument later?

      pcArg = os.path.join(get_cfg_install_path(pkg, ucp), 'lib',
                           'pkgconfig', pcfile)

      if not os.access(pcArg, os.R_OK):
        error("Could not find '{0}'".format(pcArg), ValueError)

  static_arg = [ ]
  if static:
    static_arg = ['--static']

  libs_line = run_command(['pkg-config', '--libs'] + static_arg + [pcArg]);
  libs = libs_line.split()
  return libs

# Get the version number for a system-wide installed package.
# Presumably we update the bundled packages to compatible versions,
# so this routine doesn't handle ucp and other bundled version concerns.
@memoize
def pkgconfig_get_system_version(pkg):
  # check that pkg-config knows about the package in question
  run_command(['pkg-config', '--exists', pkg])
  # run pkg-config to get the version
  version = run_command(['pkg-config', '--modversion', pkg])
  return version.strip()

#
# This returns the default link args for the given third-party package
# assuming that the bundled version is used.
#
# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
def get_bundled_compile_args(pkg, ucp=''):
    if ucp == '':
        ucp = default_uniq_cfg_path()
    inc_dir = os.path.join(get_cfg_install_path(pkg, ucp), 'include')
    return (['-I' + inc_dir], [ ])


#
# This returns the default link args for the given third-party package
# assuming that the bundled version is used.
#
# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
def get_bundled_link_args(pkg, ucp='', libs=[], add_L_opt=True):
    if ucp == '':
        ucp = default_uniq_cfg_path()
    if libs == []:
        libs = [ 'lib' + pkg + '.la' ]
    all_args = []
    lib_dir = os.path.join(get_cfg_install_path(pkg, ucp), 'lib')
    if add_L_opt:
        all_args.append('-L' + lib_dir)
        all_args.append('-Wl,-rpath,' + lib_dir)
    for lib_arg in libs:
        if lib_arg.endswith('.la'):
            la = os.path.join(lib_dir, lib_arg)
            all_args.extend(handle_la(la))
        else:
            all_args.append(lib_arg)
    if all_args == []:
        all_args.append('-l' + pkg)

    bundled_args = [ ]
    system_args = [ ]
    for arg in all_args:
        # put some of the usual suspects into the system args
        if (arg == '-ldl' or
            arg == '-lm' or
            arg == '-lnuma' or
            arg == '-lpthread'):
            system_args.append(arg)
        else:
            bundled_args.append(arg)

    return (bundled_args, system_args)

# apply substitutions like ${VARNAME} within string
# using the supplied dictionary d
def apply_subs(s, d):
    # look for ${BLA} but not $${BLA}
    # since $${BLA} is pkg-config's escape form
    pattern = r'(?<!\$)(\${([^}]+)})'
    for m in re.findall(pattern, s):
        sub, key = m
        if key in d:
            s = s.replace(sub, d[key])

    return s

# Read a pkg-config .pc file at path pcfile into a dictionary
# Handles comments, variable escapes with ${variable},
# and literal $${variable} (which is not an escape).
# Assumes that the variables are defined before they are used
#
# If find_third_party and replace_third_party are provided,
# they should be e.g.
#   find_third_party=third-party/gasnet/install/some-ucp
#   replace_third_party=/path/to/chpl-home/third-party/gasnet/install/some-ucp
# and this code will replace
#   /*/$find_third_party
# with
#  $replace_third_party
#
def read_pkg_config_file(pcfile,
                         find_third_party=None,
                         replace_third_party=None):
    ret = { }

    pattern = None
    if find_third_party and replace_third_party:
        pattern = re.compile(r'/[^ ]*/' + find_third_party)

    with open(pcfile) as file:
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
                val = apply_subs(val, ret)
                # fix up paths to third-party/ subdirs
                if pattern:
                    val = re.sub(pattern, replace_third_party, val)

                #val = chpl_home_utils.add_vars_to_paths(val)
                ret[key] = val
            else:
                # look for a line like KEY: VALUE
                key, sep, val = line.partition(':')
                key = key.strip()
                val = val.strip()
                if sep == ':' and not " " in key:
                    # substitute pkg-config file variables
                    val = apply_subs(val, ret)
                    # fix up paths to third-party/ subdirs
                    if pattern:
                        val = re.sub(pattern, replace_third_party, val)
                    ret[key] = val

    return ret
