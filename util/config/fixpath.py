#!/usr/bin/env python

from __future__ import print_function

import os
from sys import argv


def main(path, sep=":"):
    """
    Removes path components that begin with $CHPL_HOME, to reduce
    $PATH & $MANPATH pollution

    :path: $PATH or $MANPATH
    :sep: Expected path separator
    :returns: new path with $CHPL_HOME components stripped
    """
    chpl_home = os.getenv("CHPL_HOME")
    newpath = [p for p in path.split(sep) if chpl_home not in p]
    return sep.join(newpath)


if __name__ == '__main__':

    if len(argv) < 2:
        # Return empty string if len(path) == 0
        newpath = ""
    elif len(argv) < 3:
        # Default: bash/sh/csh/tcsh-like path
        newpath = main(argv[1], sep=":")
    else:
        if argv[2] == "fish":
            # fishy path (space-separated)
            newpath = main(argv[1], sep=" ")
        else:
            # Default: bash/sh/csh/tcsh-like path
            newpath = main(argv[1], sep=":")

    print(newpath)
