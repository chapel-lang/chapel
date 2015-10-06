#!/usr/bin/env python

from __future__ import print_function

import os
import sys


def main(path, sep=":"):
    """
    This script takes in two arguments: $PATH and the separator for it
    (since fish uses ' ' and everybody else uses ':')
    Then, we remove path components that begin with $CHPL_HOME
    """
    chpl_home = os.getenv("CHPL_HOME")
    newpath = [p for p in path.split(sep) if chpl_home not in p]
    return sep.join(newpath)


if __name__ == '__main__':

    if len(sys.argv) < 2:
        print("fixpath.py requires $PATH as an argument")
        exit(1)

    path = sys.argv[1]
    if " " in path:
        sep = " "
    elif ":" in path:
        sep = ":"
    else:
        print("Invalid $PATH passed to fixpath.py")
        print(sys.argv[1])
        exit(1)

    print(main(path, sep=sep))
