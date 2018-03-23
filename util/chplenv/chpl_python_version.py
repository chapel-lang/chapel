#!/usr/bin/env python
import sys

from utils import memoize


@memoize
def get():
    ver = sys.version_info
    return '{0}.{1}'.format(ver[0], ver[1])


def _main():
    python_version_val = get()
    sys.stdout.write("{0}\n".format(python_version_val))


if __name__ == '__main__':
    _main()
