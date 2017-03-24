import os
import re
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_arch, chpl_compiler, chpl_locale_model, chpl_platform
from chpl_home_utils import get_chpl_home, using_chapel_module
from utils import error, memoize, run_command


#
# This is the default unique configuration path.
#
@memoize
def default_uniq_cfg_path():
    return '{0}-{1}-{2}'.format(chpl_platform.get('target'),
                                chpl_compiler.get('target'),
                                chpl_arch.get('target', map_to_compiler=True,
                                              get_lcd=using_chapel_module()))

#
# Returns the path to the packages install directory
#
@memoize
def get_cfg_install_path(pkg, ucp=default_uniq_cfg_path()):
    return os.path.join(get_chpl_home(), 'third-party', pkg, 'install', ucp)

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
                        repl = r'\1' + get_chpl_home() + r'\3'
                        tok = pat.sub(repl, tok)
                        if tok.endswith('.la'):
                            args.extend(handle_la(tok))
                        else:
                            args.append(tok)
    return args

#
# Return linker arguments required to link with a library
# known to pkgconfig. The pkg can be a path to a .pc file or
# the name of a system-installed package or the name of
# a third-party package.
#
# if system=True, searches for a system-instaled package.
# if static=True, uses --static (suitable for static linking)
@memoize
def pkgconfig_get_link_args(pkg, ucp='', system=True, static=True):
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

#
# This returns the default link args for the given third-party package.
#
def default_get_link_args(pkg, ucp='', libs=[]):
    if ucp == '':
        ucp = default_uniq_cfg_path()
    if libs == []:
        libs = [ 'lib' + pkg + '.la' ]
    all_args = []
    for lib_arg in libs:
        if lib_arg.endswith('.la'):
            la = os.path.join(get_cfg_install_path(pkg, ucp), 'lib', lib_arg)
            all_args.extend(handle_la(la))
        else:
            all_args.append(lib_arg)
    return all_args
