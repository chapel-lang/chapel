#!/usr/bin/env python
"""
Provides simple 'get()' interface for accessing default value overrides
Checks environment variables first, then chplconfig file for definitions
"""
from __future__ import print_function
import os, sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

from utils import chplvars, memoize


class ChapelConfig(object):
    """ Class for parsing chplconfig file and providing 'get' interface """
    def __init__(self):
        """ Find and parse chplconfig file, populating self.chplconfig """

        # Dictionary containing all of the env vars defined in chplconfig
        self.chplconfig = {}

        # List of warnings to track if a warning has occurred
        self.warnings = []

        # Find chplconfigfile and prettypath
        self.find()

        # Populate chplconfig
        if self.chplconfigfile:
            self.parse()

        # Print any warnings accumulated throughout constructor
        if self.warnings:
            self.printwarnings()

    def get(self, var):
        """ Wrapper for chplconfig[var] intended to mirror os.environ.get() """
        if var in self.chplconfig.keys():
            return self.chplconfig[var]

    def find(self):
        """ Find chplconfig file path"""

        # Places to look for a chplconfig file
        chplconfigpath = os.environ.get('CHPL_CONFIG')
        home = os.path.expanduser('~')

        # Construct path to chplconfig file, ccpath
        if chplconfigpath:
            ccpath = chplconfigpath
            self.prettypath = '$CHPL_CONFIG/'
        else:
            ccpath = home
            self.prettypath = '~/'

        # Search for both visible and hidden files
        visible = os.path.join(ccpath, 'chplconfig')
        hidden = os.path.join(ccpath, '.chplconfig')

        # Set self.chplconfigpath if it exists
        if os.path.isfile(visible):
            self.chplconfigfile = visible
            self.prettypath += 'chplconfig'
        elif os.path.isfile(hidden):
            self.chplconfigfile = hidden
            self.prettypath += '.chplconfig'
        else:
            self.chplconfigfile = None
            if chplconfigpath:
                self.warnings.append(
                (
                    'Warning: No chplconfig or .chplconfig file is found in '
                    'the defined $CHPL_CONFIG\n'
                ))

    def parse(self):
        """ Parse chplconfig file for acceptable env var overrides """

        # Parse the chplconfig file and populate the chplconfig dict
        with open(self.chplconfigfile, 'r') as ccfile:

            # Split lines into fields delimited by '='
            linefields = [l.split('=') for l in ccfile.readlines()]

            for linenum, fields in enumerate([lf for lf in linefields]):


                if self.invalid_assignment(fields, linenum):
                    continue

                var, val = [f.strip() for f in fields]

                if self.invalid_entry(var, val, linenum):
                    continue

                self.chplconfig[var] = val

    def invalid_assignment(self, fields, linenum):
        """ Check if formatting of fields is formatted like 'ENV = VAR' """

        # Check if line is a comment (has no '=')
        if len(fields) < 2:
            return True

        # Check if line is incorrectly formatted
        elif len(fields) > 2:
            line = '='.join(fields).strip('\n')
            self.warnings.append(
            (
                'Warning: {0}:line {1}: Received incorrect format:\n'
                '         > {2}\n'
                '         Expected format is:\n'
                '         > CHPL_VAR = VALUE\n'
            ).format(self.prettypath, linenum, line))
            return True

        return False


    def invalid_entry(self, var, val, linenum):
        """ Check if entry for variable assignment is valid """

        # Check if var is in the list of approved special variables
        if var not in chplvars:
            self.warnings.append(
            (
                'Warning: {0}:line {1}: '
                '"{2}" is not an acceptable variable\n'
            ).format(self.prettypath, linenum, var))
            return True

        # Warn about duplicate entries, but don't skip, just overwrite
        elif var in self.chplconfig.keys():
            self.warnings.append(
            (
                'Warning: {0}:line {1}: '
                'Duplicate entry of "{2}"\n'
            ).format(self.prettypath, linenum, var))
            return False

        return False

    def printwarnings(self):
        """ Print any warnings accumulated throughout constructor """
        sys.stderr.write('\n')
        for warning in self.warnings:
            sys.stderr.write(warning)
        sys.stderr.write('\n')


# Global instance that contains parsed chplconfig assignments
chplconfig = ChapelConfig()


@memoize
def get(var, default=None):
    """ Check if variable has a default defined somewhere """
    # Check environment variable
    value = os.environ.get(var)
    if value:
        return value

    # Check chplconfig file
    value = chplconfig.get(var)
    if value:
        return value

    return default


def _main():
    """ Print the default overrides that are currently set """
    for var in chplvars:
        if get(var):
            print(var,'=',get(var))


if __name__ == '__main__':
    _main()
