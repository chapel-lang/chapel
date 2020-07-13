#!/usr/bin/env python
import optparse
import os
import sys

import overrides
from utils import memoize

@memoize
def get(flag=''):
    sanitizers_val = None
    if flag == 'exe':
        sanitizers_val = overrides.get('CHPL_SANITIZE_EXE', get(''))
    if not sanitizers_val:
        sanitizers_val = overrides.get('CHPL_SANITIZE', 'none')
    return sanitizers_val


def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--exe])')
    parser.add_option('--exe', dest='flag', action='store_const',
                      const='exe', default='')
    (options, args) = parser.parse_args()

    sanitizers_val = get(options.flag)
    sys.stdout.write("{0}\n".format(sanitizers_val))

if __name__ == '__main__':
    _main()
