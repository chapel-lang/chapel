#!/usr/bin/env python

from __future__ import print_function

from os import getenv
from sys import argv
import re

def escape_path(p):
    if len(argv) >=3 and argv[2] == 'fish':
        # Suppress splitting of fish so that the path will be treated as a whole
        return '"{}"'.format(p)
    return p

def main(env='PATH', delim=':'):
    """
    Removes path components that begin with $CHPL_HOME, to reduce
    $PATH & $MANPATH pollution

    :env: path environment variable ('PATH' or 'MANPATH')
    :delim: path delimiter (':' or ' ')
    :returns: new path with $CHPL_HOME components removed
    """

    # Get environment variables, $(MAN)PATH will be always be ':' delimited
    path = getenv(env, default=' ')
    chpl_home = getenv('CHPL_HOME', default=' ')

    # Find ':'s that are not escaped
    pattern = r'(?<!\\)\:'

    # Split path into list separated by non-escaped ':'s, and sieve chpl_home
    newpath = [escape_path(p) for p in re.split(pattern, path) if chpl_home not in p]

    # Return path delimited by shell-type (':' vs. ' ')
    return delim.join(newpath)


if __name__ == '__main__':

    if len(argv) < 2:
        newpath = main()
    elif len(argv) < 3:
        newpath = main(env=argv[1])
    else:
        if argv[2] == 'fish':
            # fishy path (space-separated)
            newpath = main(env=argv[1], delim=' ')
        else:
            # Default: bash/sh/csh/tcsh-like path
            newpath = main(env=argv[1], delim=':')

    print(newpath)
