#!/usr/bin/env python
"""
Provides simple 'get()' interface for accessing default value overrides
Checks environment variables first, then chplconfig file for definitions
"""
import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

from utils import memoize

# List of Chapel Environment Variables
chplvars = [
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


class ChapelConfig(object):
    """ Class for parsing chplconfig file and providing 'get' interface """
    def __init__(self):
        """ Find and parse chplconfig file, populating self.chplconfig """

        # Dictionary containing all of the env vars defined in chplconfig
        self.chplconfig = {}

        # List of warnings to track if a warning has occurred
        self.warnings = []

        self.chplconfigfile = None
        self.prettypath = None

        # Find and define chplconfigfile and prettypath
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
        return None

    def find(self):
        """ Find chplconfig file path"""

        # Places to look for a chplconfig file, in order of priority
        chpl_config = os.environ.get('CHPL_CONFIG')
        home = os.path.expanduser('~')
        chpl_home = os.environ.get('CHPL_HOME')

        if self.chplconfig_found(chpl_config, 'CHPL_CONFIG'):
            return
        elif self.chplconfig_found(home, '~'):
            return
        elif self.chplconfig_found(chpl_home, 'CHPL_HOME'):
            return

        # No chplconfig file was found
        self.chplconfigfile = None
        return

    def chplconfig_found(self, path, name):
        """ Check path for chplconfig and set chplconfigfile & prettypath """
        if not path:
            self.chplconfigfile = None
            return False

        # Search for both visible and hidden files
        visible = os.path.join(path, 'chplconfig')
        hidden = os.path.join(path, '.chplconfig')

        # Set self.chplconfigfile if it exists
        if os.path.isfile(visible):
            self.chplconfigfile = visible
            self.prettypath = ''.join(['$', name, '/chplconfig'])
            return True
        elif os.path.isfile(hidden):
            self.chplconfigfile = hidden
            self.prettypath = ''.join(['$', name, '/.chplconfig'])
            return True
        else:
            self.chplconfigfile = None
            if name == 'CHPL_CONFIG':
                self.warnings.append(
                (
                    'Warning: No chplconfig or .chplconfig file is found in '
                    'the defined $CHPL_CONFIG\n'
                ))
        return False

    def parse(self):
        """ Parse chplconfig file for acceptable env var overrides """

        # Parse the chplconfig file and populate the chplconfig dict
        with open(self.chplconfigfile, 'r') as ccfile:
            for linenum, line in enumerate(ccfile.readlines()):

                if not self.valid_assignment(line, linenum):
                    continue

                if not self.valid_variable(line, linenum):
                    continue

                var, val = [f.strip() for f in line.split('=')]
                self.chplconfig[var] = val

    def valid_assignment(self, line, linenum):
        """ Check if the assignment is correctly formatted, e.g. ENV = VAR """
        valid = True

        # Check if line is a comment (blank)
        if len(line.lstrip()) == 0:
            valid = False
        # Check if line is a comment (starts with '#')
        elif line.lstrip()[0] == '#':
            valid = False
        else:
            # Check if line is incorrectly formatted
            try:
                var, val = [f.strip() for f in line.split('=')]
            except ValueError:
                valid = False
                self.warnings.append(
                (
                    'Warning: {0}:line {1}: Received incorrect format:\n'
                    '         > {2}\n'
                    '         Expected format is:\n'
                    '         > CHPL_VAR = VALUE\n'
                ).format(self.prettypath, linenum, line.strip('\n')))

        return valid

    def valid_variable(self, line, linenum):
        """ Check if variable is valid and not duplicated """

        var, val = [f.strip() for f in line.split('=')]

        valid = True
        # Check if var is in the list of approved special variables
        if var not in chplvars:
            valid = False
            self.warnings.append(
            (
                'Warning: {0}:line {1}: '
                '"{2}" is not an acceptable variable\n'
            ).format(self.prettypath, linenum, var))

        # Warn about duplicate entries, but don't skip, just overwrite
        elif var in self.chplconfig.keys():
            self.warnings.append(
            (
                'Warning: {0}:line {1}: '
                'Duplicate entry of "{2}"\n'
            ).format(self.prettypath, linenum, var))

        return valid

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
    """ Check if variable has a default defined somewhere and return value """
    return os.environ.get(var) or chplconfig.get(var) or default


def allvars():
    """ Generate overrides currently set via environment/chplconfig """
    for var in [v for v in chplvars if get(v)]:
        varstring = '{0}={1}'.format(var, get(var))
        yield varstring


def _main():
    """ Print overrides that are currently set via environment/chplconfig """
    for var in allvars():
        sys.stdout.write(var)
        sys.stdout.write('\n')


if __name__ == '__main__':
    _main()
