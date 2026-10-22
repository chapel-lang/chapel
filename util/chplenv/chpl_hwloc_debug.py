#!/usr/bin/env python3
import sys

import overrides
from utils import memoize, per_script_main
import chpl_hwloc


@memoize
def get():
    hwloc_val = chpl_hwloc.get()
    debug_val = overrides.get("CHPL_HWLOC_DEBUG")
    if not debug_val or hwloc_val != "bundled":
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
