#!/usr/bin/env python
import optparse, sys, os, re
from sys import stdout

import chpl_arch, chpl_compiler, chpl_locale_model, chpl_platform
import utils
from utils import memoize

@memoize
def get_uniq_cfg_path():
    target_platform = chpl_platform.get('target')
    target_compiler = chpl_compiler.get('target')
    target_arch = chpl_arch.get('target', map_to_compiler=True, get_lcd=False)
    locale_model = chpl_locale_model.get()
    uniq_cfg_path = '{0}-{1}-{2}-{3}'.format(target_platform, target_compiler,
                                             target_arch, locale_model)
    return uniq_cfg_path


def handle_la(la_path):
    args = ''
    if os.path.isfile(la_path):
        with open(la_path) as f:
            for line in f.readlines():
                if 'old_library=' in line:
                    lib_name = line.split('\'')[1]
                    p = re.compile(r'^lib([^/]+)\.a$')
                    args = args + ' ' + p.sub(r'-l\1', lib_name)
                elif 'dependency_libs=' in line:
                    for tok in line.split('\'')[1].split(' '):
                        if tok.endswith('.la'):
                            args = args + ' ' + handle_la(tok)
                        else:
                            args = args + ' ' + tok
    return args.strip()


def get_link_args():
    chpl_home = utils.get_chpl_home()
    uniq_cfg_path = get_uniq_cfg_path()
    libdir = os.path.join(chpl_home, 'third-party', 'qthread',
                          'install', uniq_cfg_path, 'lib')
    return (handle_la(os.path.join(libdir, 'libqthread_chpl.la')) + ' ' + \
            handle_la(os.path.join(libdir, 'libqthread.la')) + ' ' + '-lchpl')


def _main():
    parser = optparse.OptionParser(
        usage='usage: %prog -[l|q]')
    parser.add_option('-l',
                      action='store_true',
                      dest='link_args',
                      help='show user program link args needed')
    parser.add_option('-u',
                      action='store_true',
                      dest='uniq_cfg_path',
                      help='show unique config path for builds/installs')
    (options, args) = parser.parse_args()
    if options.link_args:
        stdout.write('{0}\n'.format(get_link_args()))
    elif options.uniq_cfg_path:
        stdout.write('{0}\n'.format(get_uniq_cfg_path()))
    else:
        parser.print_help()


if __name__ == '__main__':
    _main()
