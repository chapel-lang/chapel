#!/usr/bin/env python
import sys, os

from utils import memoize

@memoize
def get():
    timers_val = os.environ.get('CHPL_TIMERS', 'generic')
    return timers_val


def _main():
    timers_val = get()
    sys.stdout.write("{0}\n".format(timers_val))


if __name__ == '__main__':
    _main()
