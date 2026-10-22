#!/usr/bin/env python3
import sys

import overrides
from utils import memoize, per_script_main


@memoize
def get():
    debug_val = overrides.get("CHPL_TASKS_DEBUG")
    if not debug_val:
        debug_val = "nodbg"
    else:
        debug_val = "debug"
    return debug_val


@per_script_main
def _main():
    debug_val = get()
    sys.stdout.write("{0}\n".format(debug_val))


if __name__ == "__main__":
    _main()
