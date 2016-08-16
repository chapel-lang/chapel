#!/usr/bin/env python
"""
Check .chplconfig file and environment variables for default value overrides
"""
from __future__ import print_function
import os
import sys
from sys import stderr

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

# Global dictionary that will be populated w/ .chplconfig file contents
chplconfig = {}

# Globally track if we've parsed already
parsed = False

# Valid options for override by .chplconfig file
CHPLS = [
         'CHPL_HOME',
         'CHPL_HOST_PLATFORM',
         'CHPL_HOST_COMPILER',
         'CHPL_TARGET_PLATFORM',
         'CHPL_TARGET_COMPILER',
         'CHPL_TARGET_ARCH',
         'CHPL_LOCALE_MODEL',
         'CHPL_COMM',
         'CHPL_COMM_SUBSTRATE',
         'CHPL_GASNET_SEGMENT',
         'CHPL_TASKS',
         'CHPL_LAUNCHER',
         'CHPL_TIMERS',
         'CHPL_UNWIND',
         'CHPL_MEM',
         'CHPL_MAKE',
         'CHPL_ATOMICS',
         'CHPL_NETWORK_ATOMICS',
         'CHPL_GMP',
         'CHPL_HWLOC',
         'CHPL_REGEXP',
         'CHPL_WIDE_POINTERS',
         'CHPL_LLVM',
         'CHPL_AUX_FILESYS',
         ]


def parse():
    """ Parse .chplconfig file for acceptable env var overrides """

    # Default initialized to empty dictionary
    chplconfig = {}

    # Potential paths of .chplconfig file
    chplconfigpath = os.environ.get('CHPL_CONFIG')
    home = os.path.expanduser('~')

    # Construct path to .chplconfig file
    if chplconfigpath:
        chplconfigfile = os.path.join(chplconfigpath, '.chplconfig')
        prettypath = '$CHPL_CONFIG/.chplconfig'
    else:
        chplconfigfile = os.path.join(home, '.chplconfig')
        prettypath = '~/.chplconfig'

    # Confirm that .chplconfig file exists
    if not os.path.isfile(chplconfigfile):
        if chplconfigpath:
            err = """
 Warning: $CHPL_CONFIG is defined,
 but no $CHPL_CONFIG/.chplconfig file is found"""
            stderr.write(err)
        return chplconfig

    # Parse the .chplconfig file and populate the chplconfig dict
    with open(chplconfigfile, 'r') as ccfile:
        linefields = [l.split('=') for l in ccfile.readlines()]
        for linenum, fields in enumerate([lf for lf in linefields]):

            # Check if line is a comment (has no '=')
            if len(fields) < 2:
                continue

            # Check if line is incorrectly formatted
            if len(fields) > 2:
                line = '='.join(fields)
                err = """
 Warning: {0}:line {1}:
     Received incorrect format:
         > {2}
     Expected format is:
         > CHPL_VAR = VALUE\n""".format(prettypath, linenum, line)
                stderr.write(err)
                continue

            var, val = [f.strip() for f in fields]

            # Check if var is in the list of approved special variables
            if var not in CHPLS:
                err = """
 Warning: {0}:line {1}:
     "{2}" is not an acceptable variable\n""".format(prettypath, linenum, var)
                stderr.write(err)
                continue

            # Warn about duplicate entries, but don't skip, just overwrite
            if var in chplconfig.keys():
                err = """
 Warning: {0}:line {1}:
    Duplicate entry of "{2}"\n""".format(prettypath, linenum, var)
                stderr.write(err)

            chplconfig[var] = val

    return chplconfig


def chplconfig_get(var):
    """ Wrapper for chplconfig access similar to os.environ.get() """
    global chplconfig, parsed

    if not parsed:
        chplconfig = parse()
        parsed = True

    if var in chplconfig.keys():
        return chplconfig[var]

    return None


def get(var, default=None):
    """ Check if variable has a default defined somewhere """
    # Check env var first
    value = os.environ.get(var)
    if value:
        return value

    # Check .chplconfig if it's not defined as an env var
    value = chplconfig_get(var)
    if value:
        return value

    return default


def _main():
    """ Print the default overrides that are currently set """
    for CHPL in CHPLS:
        if get(CHPL):
            print(CHPL,'=',get(CHPL))


if __name__ == '__main__':
    _main()
