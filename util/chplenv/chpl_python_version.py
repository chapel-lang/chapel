#!/usr/bin/env python
import optparse
import sys

from utils import memoize


@memoize
def get(major_only=False):
    ver = sys.version_info
    if not major_only:
        return '{0}.{1}'.format(ver[0], ver[1])
    else:
        return '{0}'.format(ver[0])

def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--major-only]")
    parser.add_option("--major-only", default=False, action='store_true')
    (options, args) = parser.parse_args()

    python_version_val = get(options.major_only)
    sys.stdout.write("{0}\n".format(python_version_val))


if __name__ == '__main__':
    _main()
