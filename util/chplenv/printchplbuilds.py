#!/usr/bin/env python3

# Parses a build path and outputs the values of the environment variables
# that correspond to that path.

import os
import utils
import printchplenv
import sys
import stat
from collections import OrderedDict
import argparse
from pprint import pprint
import datetime
from enum import Enum, unique
from collections import defaultdict

# Parsing the build directory path is implemented as a state machine. Some
# of the components start with a prefix, some do not. The state machine
# determines the current environment variable being parsed for those that
# do not have a prefix. The state is State.PREFIX for those that do. This
# state is exited when a component is encountered that doesn't have a
# prefix.

@unique
class State(Enum):
    NONE =              0
    TARGET_PLATFORM =   1
    TARGET_COMPILER =   2
    PREFIX =            3
    HWLOC =             4
    RE2 =               5
    LIBFABRIC =         6
    TARGET_ARCH =       7
    COMM_SUBSTRATE =    8
    GASNET_SEGMENT =    9

# Maps component prefix to corresponding environment variable.

prefixes = {
    'arch':         'CHPL_TARGET_CPU', # obsolete
    'cpu':          'CHPL_TARGET_CPU',
    'loc':          'CHPL_LOCALE_MODEL',
    'comm':         'CHPL_COMM',
    'tasks':        'CHPL_TASKS',
    'launch':       'CHPL_LAUNCHER',
    'tmr':          'CHPL_TIMERS',
    'unwind':       'CHPL_UNWIND',
    'mem':          'CHPL_MEM',
    'atomics':      'CHPL_ATOMICS',
    'gmp':          'CHPL_GMP',
    'llvm':         'CHPL_LLVM',
    'fs':           'CHPL_AUX_FILESYS',
    'sanitizers':   'CHPL_SANITIZE_EXE', # obsolete
    'san':          'CHPL_SANITIZE_EXE',
    'lib_pic':      'CHPL_LIB_PIC',
    'hwloc':        'CHPL_HWLOC',
    're2':          'CHPL_RE2'
}

# Maps state to corresponding environment variable.

varNames = {
    State.TARGET_PLATFORM:  'CHPL_TARGET_PLATFORM',
    State.TARGET_COMPILER:  'CHPL_TARGET_COMPILER',
    State.HWLOC:            'CHPL_HWLOC',
    State.RE2:              'CHPL_RE2',
    State.LIBFABRIC:        'CHPL_LIBFABRIC',
    State.PREFIX:           None,
    State.TARGET_ARCH:      'CHPL_TARGET_ARCH',
    State.COMM_SUBSTRATE:   'CHPL_COMM_SUBSTRATE',
    State.GASNET_SEGMENT:   'CHPL_GASNET_SEGMENT'
}

# State transitions. This isn't a true state machine because some of the transitions are
# encoded in the actions below.

nextStates = {
    State.TARGET_PLATFORM:  State.TARGET_COMPILER,
    State.TARGET_COMPILER:  State.TARGET_ARCH,
    State.TARGET_ARCH:      State.PREFIX,
    State.PREFIX:           State.PREFIX,
    State.LIBFABRIC:        State.PREFIX,
    State.HWLOC:            State.RE2,
    State.RE2:              State.PREFIX,
    State.COMM_SUBSTRATE:   State.GASNET_SEGMENT,
    State.GASNET_SEGMENT:   State.PREFIX
}

# Some of the CHPL_*_DEBUG variables add a "-debug" suffix to the component
# name depending on whether or not they are set. "+" denotes that the debug
# variable was set, and  "-" that was not. Returns the value of the original
# variable with the "-debug" suffix removed.

def ProcessDebug(fields, config):
    global used
    print(fields)
    var = prefixes[fields[0]]
    print(var)
    value = '-'.join(fields[1:])
    if var in ("CHPL_COMM", "CHPL_HWLOC", "CHPL_TASKS"):
        debug = var + "_DEBUG"
        if fields[-1] == 'debug':
            config[debug] = "+"
            value = '-'.join(fields[1:-1]) # drop the suffix
        else:
            config[debug] = "-"
        used.add(debug)
    return value

# Parse the given path and return a configuration based on the components of the path.
# The configuration is a dictionary where the key is the environment variable name and the
# value is its value. As a side effect the variables are added to the "used" set.

def Parse(path):
    global used

    state = State.TARGET_PLATFORM

    config = {}
    i = 0
    while i < len(path):
        component = path[i]
        # Get environment variable that corresponds to the current state.
        var = varNames[state]
        # Get default next state. Note that it might be changed below.
        nextState = nextStates[state]
        j = i + 1
        if state == State.PREFIX and '-' in component:
            fields = component.split('-')
            var = prefixes[fields[0]]
            value = ProcessDebug(fields, config)
            if var == 'CHPL_COMM':
                if value == 'ofi':
                    nextState = State.LIBFABRIC
                elif value == 'gasnet':
                    nextState = State.COMM_SUBSTRATE
            config[var] = value
            used.add(var)
        else:
            # Component doesn't have a prefix. If we were expecting a prefix then go to the
            # next state and reprocess the current component. Components without prefixes
            # have been deprecated so this should only happen with old builds.
            if state == State.PREFIX:
                nextState = State.HWLOC
                j = i
            else:
                # Set the environment variable based solely on which state we are in.
                value = component
                config[var] = value
                used.add(var)
        state = nextState
        i = j
    return config

# Uses printchplenv to get the current configuration based on the environment variables.
# If 'all' is True then returns all environment variables, otherwise omits variables not
# relevant to the current configuration.

def GetConfig(all=False):
    config = OrderedDict()
    excludes = ['CHPL_HOME']
    filters = ['no-tidy'] if all else ['tidy']
    filters.append('only-path')
    lines = printchplenv.printchplenv(['runtime'], print_filters=filters,
                                      print_format='simple').split('\n')
    for line in lines:
        if len(line) == 0:
            continue
        (var, value) = line.split('=', 1)
        if var in excludes:
            continue
        config[var] = value
        # CHPL_*_DEBUG variables aren't returned by printchplenv
        if var == 'CHPL_COMM' and value != "none":
            value = '+' if os.getenv('CHPL_COMM_DEBUG') else '-'
            config['CHPL_COMM_DEBUG'] = value
        elif var == 'CHPL_HWLOC' and value == "bundled":
            value = '+' if os.getenv('CHPL_HWLOC_DEBUG') else '-'
            config['CHPL_HWLOC_DEBUG'] = value
        elif var == 'CHPL_TASKS':
            value = '+' if os.getenv('CHPL_TASKS_DEBUG') else '-'
            config['CHPL_TASKS_DEBUG'] = value
    return config

# Returns a list of variables that differ between two configurations

def FindDiffVars(usedvars, a, b):
    result = []
    for var in usedvars:
        value = a.get(var, 'NA')
        if a.get(var, 'NA') != b.get(var, 'NA'):
            result.append(var)
    return result

# print shell commands to change the environment variables from current to config

def PrintShellCommands(current, config, shell="bash"):
    global usedvars
    if shell == "csh":
        print("set noglob;")
    for var in usedvars:
        newValue = config.get(var, None)
        oldValue = current.get(var, None)
        if oldValue != newValue:
            if newValue is None or newValue == '-':
                if shell == "csh":
                    print('unsetenv %s;' % (var))
                elif shell == "bash":
                    print('unset %s;' % (var))
            else:
                if newValue == '+':
                    newValue = 1
                if shell == "csh":
                    print('setenv %s "%s";' %(var, newValue))
                elif shell == "bash":
                    print('export %s="%s";' %(var, newValue))
    if shell == "csh":
        print("unset noglob;")

# Prints the configurations in columns. If a variable's value differs from the
# current configuration it has a '*' suffix. "extras" are variables that shouldn't
# get a prefix, e.g. MTIME.

def Output(current, variables, extras, configs):
    for var in variables + extras:
        output = "%25s:" % var
        for config in configs:
            value = config.get(var, 'NA')
            if var not in extras:
                tag = "" if current.get(var, 'NA') == value else "*"
                value += tag
            if var == 'MTIME' and value != 'NA':
                value = datetime.datetime.fromtimestamp(value).strftime("%b %d %H:%M")
            output += " %-20s" % value
        print(output)

# For argument parsing
@unique
class Sort(Enum):
    NEWEST =    1
    OLDEST =    2
    FEWEST =    3
    MOST =      4

def main(argv):
    global configs
    global used
    global usedvars
    description = "Displays the available Chapel runtime builds. Values that differ from" \
    " the current configuration have a '*' suffix. '+' denotes a variable that is set but" \
    " whose value doesn't matter, '-' denotes a variable that is not set. 'NA' denotes a" \
    " variable that is not applicable to the build."

    # Note: add_mutually_exclusive_group doesn't currently support a title which would make
    # the help output easier to read. Also help doesn't appear in the same order as below.

    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("--version", action="version", version="%(prog)s 1.0",
                      help="Print version information.")

    group = parser.add_mutually_exclusive_group()
    group.add_argument("--newest", "-n", action="store_const", dest="sort", const=Sort.NEWEST,
                      help="Sort builds with newest first. [default]")
    group.add_argument("--oldest", "-o", action="store_const", dest="sort", const=Sort.OLDEST,
                      help="Sort builds with oldest first.")
    group.add_argument("--fewest", "-f", action="store_const", dest="sort", const=Sort.FEWEST,
                      help="Sort builds with fewest differences first.")
    group.add_argument("--most", "-m", action="store_const", dest="sort", const=Sort.MOST,
                      help="Sort builds with most differences first.")

    group = parser.add_argument_group("Display")
    group.add_argument("--no-current", "-C", action="store_false", dest="current", default=True,
                      help="Do not display the current configuration.")
    group.add_argument("--summary", "-s", action="store_true", dest="summary", default=False,
                      help="Summarize the differences between the builds.")
    group.add_argument("--width", "-w", action="store", type=int, dest="width", default=6,
                      metavar="NUM",
                      help="Number of columns in the output [default: %(default)s].")

    group = parser.add_mutually_exclusive_group()
    group.add_argument("--bash", "-b", action="store", type=int, dest="bash", default=None,
                      metavar="NUM",
                      help="Output bash commands to match specified build.")
    group.add_argument("--csh", "-c", action="store", type=int, dest="csh", default=None,
                      metavar="NUM",
                      help="Output csh commands to match specified build.")

    group = parser.add_argument_group("Misc.")
    group.add_argument("--path", "-p", action="store", dest="path", default=None,
                      help="Parse the given runtime build path.")
    group.add_argument("--check", "-k", action="store_true", dest="check", default=False,
                      help="Check that configuration matches a build.")

    (args) = parser.parse_args(argv[1:])

    if args.width < 1:
        parser.error("Number of columns must be >= 1.")

    if args.summary:
        if args.sort != None:
            parser.error("Sort option cannot be used with --summary.")
        if args.csh:
            parser.error("--summary cannot be used with --csh.")
        if args.bash:
            parser.error("--summary cannot be used with --bash.")
        if args.check:
            parser.error("--summary cannot be used with --check.")

    if args.check:
        if args.sort != None:
            parser.error("Sort option cannot be used with --check.")
        if args.csh:
            parser.error("--check cannot be used with --csh.")
        if args.bash:
            parser.error("--check cannot be used with --bash.")

    if args.sort == None:
        args.sort = Sort.NEWEST

    # Note: "allvars" is a list of all possible variables in the order in
    # which they are displayed by printchplenv so we can print them in the
    # same order. "usedvars" is a list of the variables actually used in the
    # configurations, in the same order. "used" is a set of used variables to
    # make it easy to determine whether a variable has been used or
    # not. "usedvars" is created from "allvars" and "used".

    # get the names of all variables
    printchplenv.compute_all_values()
    allvars = GetConfig(True).keys()
    used = set()

    # get the current configuration
    current = GetConfig()
    for var in current:
        used.add(var)

    if args.path is not None:
        builds = [Parse(args.path.split(os.sep))]
    else:
        # gather the paths for all builds
        home = os.getenv('CHPL_HOME')
        if home is None:
            print("Error: CHPL_HOME is not set", file=sys.stderr)
            sys.exit(1)

        base = os.path.join(home, "lib")
        # "targets" is a list of all paths that contain a "libchpl.a" file
        targets = []
        for root, dirs, files in os.walk(base):
            for target in filter(lambda x: x == "libchpl.a", files):
                targets.append(os.path.join(root, target))

        if args.sort == Sort.OLDEST:
            targets.sort(key= lambda x: os.stat(x)[stat.ST_MTIME])

        if args.sort == Sort.NEWEST:
            targets.sort(key= lambda x: os.stat(x)[stat.ST_MTIME], reverse=True)

        # Convert the targets into a list of paths in which each path is a list of
        # components, excluding the final "libchpl.a"
        paths = []
        for target in targets:
            relpath = os.path.relpath(target, base)
            paths.append(relpath.split(os.sep)[:-1])

        builds = []
        for (target, path) in zip(targets, paths):
            build = Parse(path)
            build['MTIME'] = os.stat(target)[stat.ST_MTIME]
            builds.append(build)

    # make a list of variables that were actually used by the configurations
    usedvars = [x for x in allvars if x in used]

    if args.summary:
        # summarize which variables differ between the configs and their values
        diffvars = set()
        for i in range(len(builds)):
            diffvars |= set(FindDiffVars(usedvars, current, builds[i]))
        values = defaultdict(set)
        for config in [current] + builds:
            for var in usedvars:
                if var in diffvars:
                    value = config.get(var, 'NA')
                    values[var].add(value)

        for var in usedvars:
            if var in diffvars:
                 print("%25s: %-20s" % (var, list(values[var])))
        sys.exit(0)

    if args.sort == Sort.FEWEST or args.sort == Sort.MOST or args.check:
        # Sort based on # of differences from current config
        for i in range(len(builds)):
            builds[i]['diffs'] = len(FindDiffVars(usedvars, current, builds[i]))
        reverse = False if (args.sort == Sort.FEWEST or args.check) else True
        builds.sort(key= lambda x: x['diffs'], reverse=reverse)

    if args.csh is not None:
        if args.csh > len(builds):
            parser.error("Invalid build %d" % args.csh)
        PrintShellCommands(current, builds[args.csh], shell="csh")
        sys.exit(0)

    if args.bash is not None:
        if args.bash > len(builds):
            parser.error("Invalid build %d" % args.bash)
        PrintShellCommands(current, builds[args.bash],shell="bash")
        sys.exit(0)

    if args.check:
        if len(builds) > 0:
            if builds[0]['diffs'] == 0:
                print("There is a build for the current configuration.")
                sys.exit(0)
        print("No build for the current configuration.")
        sys.exit(1)

    if args.current:
        # Print current configuration too
        configs = [current] + builds
    else:
        configs = builds

    extras = ['MTIME']
    done = 0
    headers = 0
    offset = 0
    # print the output args.width columns at a time
    while done < len(configs):
        columns = min(args.width, len(configs) - done)
        # print column headers
        output = "%26s " % ''
        if args.current and done == 0:
            output += "%-20s" % '<Current>'
            headers = columns - 1
        else:
            headers = columns
        for i in range(headers):
            output += " %3d%17s" % (i + done - offset, '')
        print(output)
        if args.current and done == 0:
            offset = 1

        # print variable names and their values
        for var in usedvars + extras:
            output = "%25s:" % var
            for config in configs[done:done+columns]:
                value = config.get(var, 'NA')
                if var not in extras:
                    tag = "" if current.get(var, 'NA') == value else "*"
                    value += tag
                if var == 'MTIME' and value != 'NA':
                    value = datetime.datetime.fromtimestamp(value).strftime("%b %d %H:%M")
                output += " %-20s" % value
            print(output)
        done += columns


if __name__ == '__main__':
    main(sys.argv)
