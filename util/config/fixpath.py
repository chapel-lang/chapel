#!/usr/bin/env python3

"""Removes path components that begin with $CHPL_HOME from the given path.

    ./fixpath.py path-value [--shell shell]

Example:

    ./fixpath.py "$PATH"
    ./fixpath.py \\" $PATH \\" --shell=fish

This is used by the setchplenv.* scripts to reduce PATH/MANPATH pollution. It
may be called in several situations:

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
"""

import optparse
import os
import re
import sys

def escape_path(p, delim):
    """Wrap fish paths in quotes to prevent splitting on spaces in paths"""
    if delim == ' ':
        return '"{}"'.format(p)
    return p


def remove_chpl_from_path(path_val, delim):
    """
    :path_val: path environment variable value ('$PATH' or '$MANPATH')
    :delim: path delimiter (':' or ' ')
    :returns: new path with $CHPL_HOME components removed
    """

    chpl_home = os.getenv('CHPL_HOME')

    if not chpl_home or chpl_home not in path_val:
        return path_val

    # Find delims that are not escaped
    pattern = r'(?<!\\)\{0}'.format(delim)

    # Split path by non-escaped delims, and sieve chpl_home
    # Fish input includes hanging quotation marks, so we drop those here
    newpath = [escape_path(p, delim) for p in re.split(pattern, path_val) if p != '"']
    newpath = [p for p in newpath if chpl_home not in p]

    return delim.join(newpath)


def update_path_env():
    os.environ["PATH"] = remove_chpl_from_path(os.environ["PATH"], ":")

def main():
    parser = optparse.OptionParser(usage=__doc__)
    parser.add_option('--shell', dest='shell', default='bash',
                      help='shell being used')

    (options, args) = parser.parse_args()

    if options.shell == 'fish':
        delim = ' '
    else:
        delim = ':'

    if len(args) == 0:
        sys.stderr.write('Error: path-value must be supplied\n\n')
        parser.print_help()
        sys.exit(1)

    path = delim.join(args)

    newpath = remove_chpl_from_path(path, delim)
    sys.stdout.write('{0}'.format(newpath))


if __name__ == '__main__':
    main()
