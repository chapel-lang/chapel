#!/usr/bin/env python
import os, re

import utils
from utils import memoize

import chpl_arch, chpl_compiler, chpl_platform
import chpl_locale_model


#
# This is the default unique configuration path.
#
@memoize
def default_uniq_cfg_path():
    return '{0}-{1}-{2}'.format(chpl_platform.get('target'),
                                chpl_compiler.get('target'),
                                chpl_arch.get('target', map_to_compiler=True,
                                              get_lcd=utils.using_chapel_module()))

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
                elif 'dependency_libs=' in line:
                    for tok in line.split('\'')[1].split():
                        # paths reflect built env; replace with $CHPL_HOME
                        pat = re.compile(r'^((-L\s*)?).*(/third-party/)')
                        repl = r'\1' + utils.get_chpl_home() + r'\3'
                        tok = pat.sub(repl, tok)
                        if tok.endswith('.la'):
                            args.extend(handle_la(tok))
                        else:
                            args.append(tok)
    return args


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
            all_args.extend(handle_la(os.path.join(utils.get_chpl_home(),
                                                   'third-party',
                                                   pkg,
                                                   'install',
                                                   ucp,
                                                   'lib',
                                                   lib_arg)))
        else:
            all_args.append(lib_arg)
    return all_args
