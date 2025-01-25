#!/usr/bin/env python3
import sys

import overrides
from utils import memoize, check_valid_var


@memoize
def get():
    timers_val = overrides.get('CHPL_TIMERS', 'generic')
    check_valid_var("CHPL_TIMERS", timers_val, ["generic"])
    return timers_val


def _main():
    timers_val = get()
    sys.stdout.write("{0}\n".format(timers_val))


if __name__ == '__main__':
    _main()
