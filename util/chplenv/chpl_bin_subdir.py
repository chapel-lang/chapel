#!/usr/bin/env python

import os
from sys import stderr, stdout
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_machine, chpl_platform

from utils import memoize

@memoize
def get(flag='host'):
    platform = chpl_platform.get(flag)
    machine = chpl_machine.get(flag)

    return "{0}-{1}".format(platform,machine)


def _main():
    bin_subdir = get("host")

    stdout.write("{0}\n".format(bin_subdir))

if __name__ == '__main__':
    _main()
