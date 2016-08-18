#!/usr/bin/env python
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import overrides
from utils import memoize


@memoize
def get():
    timers_val = overrides.get('CHPL_TIMERS', 'generic')
    return timers_val


def _main():
    timers_val = get()
    sys.stdout.write("{0}\n".format(timers_val))


if __name__ == '__main__':
    _main()
