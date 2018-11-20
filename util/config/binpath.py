#!/usr/bin/env python

from collections import namedtuple
from functools import partial
import optparse
import os
from sys import stdout, path

CHPLENV_DIR = os.path.dirname(os.path.dirname(__file__))
path.insert(0, os.path.abspath(CHPLENV_DIR))

from chplenv import chpl_machine, chpl_platform

def _main():
    platform = chpl_platform.get("host")
    machine = chpl_machine.get("host")

    stdout.write("{0}-{1}\n".format(platform,machine))

if __name__ == '__main__':
    _main()
