#!/usr/bin/env python
import optparse, sys, os, re
from sys import stdout

import utils
from utils import memoize
import chpl_locale_model

import third_party_utils


@memoize
def get_uniq_cfg_path():
    return '{0}-{1}'.format(third_party_utils.default_uniq_cfg_path(),
                            chpl_locale_model.get())


def get_link_args():
    libdir = os.path.join(utils.get_chpl_home(),
                          'third-party',
                          'qthread',
                          'install',
                          get_uniq_cfg_path(),
                          'lib')
    link_args = third_party_utils.handle_la(os.path.join(libdir,
                                                         'libqthread_chpl.la'))
    link_args.extend(third_party_utils.handle_la(os.path.join(libdir,
                                                              'libqthread.la')))
    link_args.append('-lchpl')
    return link_args


def _main():
    parser = optparse.OptionParser(
        usage='usage: %prog -[l|u]')
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
