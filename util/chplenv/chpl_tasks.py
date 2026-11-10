#!/usr/bin/env python3
import sys

import chpl_platform, overrides
from utils import memoize, check_valid_var


@memoize
def get():
    tasks_val = overrides.get("CHPL_TASKS")
    if not tasks_val:
        platform_val = chpl_platform.get("target")

        cygwin = platform_val.startswith("cygwin")
        bsd = platform_val.startswith("netbsd") or platform_val.startswith(
            "freebsd"
        )

        if cygwin or bsd:
            tasks_val = "fifo"
        else:
            tasks_val = "qthreads"

    check_valid_var("CHPL_TASKS", tasks_val, ("fifo", "qthreads"))
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == "__main__":
    _main()
