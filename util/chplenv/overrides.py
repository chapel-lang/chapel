#!/usr/bin/env python3
"""
Provides simple 'get()' interface for accessing default value overrides
Checks environment variables first, then chplconfig file for definitions
"""
import os
import sys

from utils import memoize, warning

# List of Chapel Environment Variables
# Note: this array lists them in an order that matches
#   printchplenv --all --no-tidy
# But some variables are included here so that they can be overridden
# in a chplconfig file even though they are not in that output.
# When such variables are related to others, we include them in the same group.
chplvars = [
             'CHPL_HOME',

             'CHPL_HOST_PLATFORM',
             'CHPL_HOST_COMPILER',
             'CHPL_HOST_CC',
             'CHPL_HOST_CXX',
             'CHPL_HOST_ARCH',
             'CHPL_HOST_CPU', # note: not in printchplenv --all

             'CHPL_TARGET_PLATFORM',
             'CHPL_TARGET_COMPILER',
             'CHPL_TARGET_CC',
             'CHPL_TARGET_CXX',
             'CHPL_TARGET_LD',
             'CHPL_TARGET_ARCH',
             'CHPL_TARGET_CPU',

             'CHPL_LOCALE_MODEL',
             'CHPL_GPU',
             'CHPL_CUDA_PATH',
             'CHPL_ROCM_PATH',
             'CHPL_GPU_ARCH',

             'CHPL_COMM',
             'CHPL_COMM_SUBSTRATE',
             'CHPL_GASNET_SEGMENT',
             'CHPL_LIBFABRIC',

             'CHPL_TASKS',
             'CHPL_LAUNCHER',
             'CHPL_TIMERS',
             'CHPL_UNWIND',

             'CHPL_HOST_MEM',
             'CHPL_TARGET_MEM', # note: not in printchplenv --all
             'CHPL_MEM',
             'CHPL_JEMALLOC', # note: these 3 are not in printchplenv --all
             'CHPL_HOST_JEMALLOC',
             'CHPL_TARGET_JEMALLOC',

             'CHPL_ATOMICS',
             'CHPL_NETWORK_ATOMICS',

             'CHPL_GMP',
             'CHPL_HWLOC',
             'CHPL_RE2',

             'CHPL_LLVM',
             'CHPL_LLVM_SUPPORT',
             'CHPL_LLVM_CONFIG',
             # CHPL_LLVM_VERSION -- doesn't make sense to override it
             'CHPL_LLVM_GCC_PREFIX', # not in printchplenv --all

             'CHPL_AUX_FILESYS',
             'CHPL_LIB_PIC',
             'CHPL_SANITIZE',
             'CHPL_SANITIZE_EXE',

             # These are not in printchplenv --all
             'CHPL_MAKE',
             'CHPL_HOST_USE_SYSTEM_LIBCXX',
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
                    'No chplconfig or .chplconfig file is found in '
                    'the defined $CHPL_CONFIG'
                ))
        return False

    def parse(self):
        """ Parse chplconfig file for acceptable env var overrides """

        # Parse the chplconfig file and populate the chplconfig dict
        with open(self.chplconfigfile, 'r') as ccfile:
            for linenum, line in enumerate(ccfile.readlines()):

                # Strip comments and trailing white space from line
                line = line.split('#')[0].strip()

                if self.skip_line(line, linenum):
                    continue

                var, val = [f.strip() for f in line.split('=')]
                self.chplconfig[var] = val

    def skip_line(self, line, linenum):
        """
        Check the various conditions for skipping a line, accumulate warnings.
        """

        # Check if line is comment, by taking length of stripped line
        if len(line) == 0:
            return True

        # Check for syntax errors
        try:
            var, val = [f.strip() for f in line.split('=')]
        except ValueError:
            self.warnings.append(
            (
                'Syntax Error: {0}:line {1}\n'
                '              > {2}\n'
                '              Expected format is:\n'
                '              > CHPL_VAR = VALUE'
            ).format(self.prettypath, linenum, line.strip('\n')))
            return True

        # Check if var is in the list of approved special variables
        if var not in chplvars:
            self.warnings.append(
            (
                '{0}:line {1}: "{2}" is not an acceptable variable'
            ).format(self.prettypath, linenum, var))
            return True

        # Warn about duplicate entries, but don't skip, just overwrite
        elif var in self.chplconfig.keys():
            self.warnings.append(
            (
                '{0}:line {1}: Duplicate entry of "{2}"'
            ).format(self.prettypath, linenum, var))

        # If we reach here, this is a valid assignment, so don't skip
        return False

    def printwarnings(self):
        """ Print any warnings accumulated throughout constructor """
        sys.stderr.write('\n')
        for msg in self.warnings:
            warning(msg)
        sys.stderr.write('\n')


# Global instance that contains parsed chplconfig assignments
chplconfig = ChapelConfig()


@memoize
def get(var, default=None):
    """ Check if variable has a default defined somewhere and return value """
    return os.environ.get(var) or chplconfig.get(var) or default

@memoize
def get_environ(var, default=None):
    """ Check if variable has a default defined in the environment and return value """
    return os.environ.get(var) or default

@memoize
def get_chplconfig(var, default=None):
    """ Check if variable has a default defined in a chplconfig file and return value """
    return chplconfig.get(var) or default

""" Generate env vars currently set via environment/chplconfig """
def keys():
    for var in [v for v in chplvars if get(v)]:
        yield var

""" Generate (env var, value) pairs currently set via environment/chplconfig """
def items():
    for var in keys():
        yield (var, get(var))



def _main():
    """ Print overrides that are currently set via environment/chplconfig """
    for key, val in items():
        sys.stdout.write('{0}={1}\n'.format(key, val))


if __name__ == '__main__':
    _main()
