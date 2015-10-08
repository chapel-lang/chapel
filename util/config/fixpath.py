#!/usr/bin/env python

from __future__ import print_function

import os
from sys import argv


def splitescaped(s, delim=":", escape='\\'):
    """
    Python builtin split extended to account for escaped delimiters

    This should work for all cases except when s begins with '\:',
    the '\:' will be excluded from the resulting split.
    It is safe to assume no path will ever begin with '\:'

    """

    # Prepend dummy character so that we loop through all characters with c2
    s = ' '+s

    words = []
    word = []
    for c1, c2 in zip(s[:-1:], s[1::]):
        if c2 not in delim or c1 in escape:
            word.append(c2)
        else:
            if word:
                words.append(''.join(word))
                word = []
    if word:
        words.append(''.join(word))
    return words


def main(env='PATH', delim=':'):
    """
    Removes path components that begin with $CHPL_HOME, to reduce
    $PATH & $MANPATH pollution

    :env: PATH or MANPATH
    :delim: path delimiter
    :returns: new path with $CHPL_HOME components stripped
    """

    path = os.getenv(env, default=' ')
    chpl_home = os.getenv('CHPL_HOME', default=' ')

    newpath = [p for p in splitescaped(path) if chpl_home not in p]

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
