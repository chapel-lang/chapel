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

    This is used by the setchplenv.* scripts, and may be called in several
    situations:
    1. No Chapel environment settings (new shell)
    2. Same $CHPL_HOME as last time (re-running setchplenv in same dir)
    3. Different $CHPL_HOME (cd ../other-chapel-dir).
    4. $CHPL_HOME is set, but path doesn't include an old one.
       ($CHPL_HOME was hand-set, now setchplenv is run)

    For case 1, just return the existing environment variable.

    For case 2, return the environment variable without the components
    that begin with $CHPL_HOME.

    For case 3, setchplenv invokes this script before setting the new
    $CHPL_HOME.  We still have the old $CHPL_HOME set, so we can remove
    the old $PATH and $MANPATH entries.  The upshot is we do the same thing
    as in case 2.

    Case 4 should also be the same as case 2, but we won't remove any
    components since there should be no components starting with $CHPL_HOME.
    Mentioned only to avoid reintroducing #10196 when this function is modified.

    :env: path environment variable ('PATH' or 'MANPATH')
    :delim: path delimiter (':' or ' ')
    :returns: new path with $CHPL_HOME components removed
    """

    # Get environment variables, $(MAN)PATH will be always be ':' delimited
    path = getenv(env)
    if path is None:
        return ''

    chpl_home = getenv('CHPL_HOME')

    # Find ':'s that are not escaped
    pattern = r'(?<!\\)\:'

    # Split path into list separated by non-escaped ':'s, and sieve chpl_home
    newpath = [escape_path(p)
                 for p in re.split(pattern, path)
                     if chpl_home is None or chpl_home not in p]

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
