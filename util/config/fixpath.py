#!/usr/bin/env python

from __future__ import print_function

import os
import sys


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

    if len(sys.argv) < 2:
        # Return empty string if len(path) == 0
        newpath = ""
    else:
        path = sys.argv[1]
        if ":" in path:
            # bash/csh/tcsh-like path
            newpath = main(path, sep=":")
        elif " " in path:
            # fish-like path
            newpath = main(path, sep=" ")
        else:
            # return unmodified path if len(path) == 1
            newpath = sys.argv[1]

    print(newpath)
