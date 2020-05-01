#!/usr/bin/env python
import sys

import overrides
from utils import memoize


@memoize
def get():
    debug_val = overrides.get('CHPL_COMM_DEBUG')
    if not debug_val:
        debug_val = 'nodbg'
    else:
        debug_val = 'debug'
    return debug_val


def _main():
    debug_val = get()
    sys.stdout.write("{0}\n".format(debug_val))


if __name__ == '__main__':
    _main()
