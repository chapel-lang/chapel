#!/usr/bin/env python
import optparse
import os
import re
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import overrides
from utils import memoize


@memoize
def get(flag='wide'):
    wide_val = overrides.get('CHPL_WIDE_POINTERS', 'struct')
    define = ''

    if wide_val == 'struct':
        define = '-DCHPL_WIDE_POINTER_STRUCT'
    else:
        match = re.match(r'node(\d+)', wide_val)
        if match:
            node_bits = int(match.group(1))
            if node_bits < 2 or node_bits > 60:
                sys.stderr.write("Error: Bad wide pointer node bit width: {0}\n".format(node_bits))
            else:
                define = "-DCHPL_WIDE_POINTER_PACKED " \
                         "-DCHPL_WIDE_POINTER_NODE_BITS={0}".format(node_bits)
        else:
            sys.stderr.write("Error: Unknown wide pointer format: {0}\n".format(wide_val))

    if flag == 'wide':
        return wide_val
    elif flag == 'define':
        return define
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--wide|define])')
    parser.add_option('--wide', dest='flag', action='store_const',
                      const='wide', default='wide')
    parser.add_option('--define', dest='flag', action='store_const',
                      const='define')
    (options, args) = parser.parse_args()

    wide_val = get(options.flag)
    sys.stdout.write("{0}\n".format(wide_val))


if __name__ == '__main__':
    _main()
