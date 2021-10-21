#!/usr/bin/env python3

# Parses a build path and outputs the values of the environment variables
# that correspond to that path. 

import os
import utils
import printchplenv
import sys
import stat
from collections import OrderedDict
import optparse
from pprint import pprint
import datetime

# Parsing the build directory path is implemented as a state machine. Some
# of the components start with a prefix, some do not. The state machine
# determines the current environment variable being parsed for those that
# do not have a prefix. The state is STATE_PREFIX for those that do. This
# state is exited when a component is encountered that doesn't have a
# prefix. 

STATE_NONE = 0
STATE_TARGET_PLATFORM = 1
STATE_TARGET_COMPILER = 2
STATE_PREFIX = 3
STATE_HWLOC = 4
STATE_RE2 = 5
STATE_LIBFABRIC = 6
STATE_TARGET_ARCH = 7
STATE_COMM_SUBSTRATE = 8
STATE_GASNET_SEGMENT = 9

# Maps component prefix to corresponding environment variable.

prefixes = {}
prefixes['arch'] = 'CHPL_TARGET_CPU' # obsolete
prefixes['cpu'] = 'CHPL_TARGET_CPU'
prefixes['loc'] = 'CHPL_LOCALE_MODEL'
prefixes['comm'] = 'CHPL_COMM'
prefixes['tasks'] = 'CHPL_TASKS'
prefixes['launch'] = 'CHPL_LAUNCHER'
prefixes['tmr'] = 'CHPL_TIMERS'
prefixes['unwind'] = 'CHPL_UNWIND'
prefixes['mem'] = 'CHPL_MEM'
prefixes['atomics'] = 'CHPL_ATOMICS'
prefixes['gmp'] = 'CHPL_GMP'
prefixes['llvm'] = 'CHPL_LLVM'
prefixes['fs'] = 'CHPL_AUX_FILESYS'
prefixes['sanitizers'] = 'CHPL_SANITIZE_EXE' # obsolete
prefixes['san'] = 'CHPL_SANITIZE_EXE'
prefixes['lib_pic'] = 'CHPL_LIB_PIC'
prefixes['hwloc'] = 'CHPL_HWLOC'
prefixes['re2'] = 'CHPL_RE2'

# Maps state to corresponding environment variable.

varNames = {}
varNames[STATE_TARGET_PLATFORM] = 'CHPL_TARGET_PLATFORM'
varNames[STATE_TARGET_COMPILER] = 'CHPL_TARGET_COMPILER'
varNames[STATE_HWLOC] = 'CHPL_HWLOC'
varNames[STATE_RE2] = 'CHPL_RE2'
varNames[STATE_LIBFABRIC] = 'CHPL_LIBFABRIC'
varNames[STATE_PREFIX] = None
varNames[STATE_TARGET_ARCH] = 'CHPL_TARGET_ARCH'
varNames[STATE_COMM_SUBSTRATE] = 'CHPL_COMM_SUBSTRATE'
varNames[STATE_GASNET_SEGMENT] = 'CHPL_GASNET_SEGMENT'

# State transitions. This isn't a true state machine because some of the transitions are
# encoded in the actions below.

nextStates = {}
nextStates[STATE_TARGET_PLATFORM] = STATE_TARGET_COMPILER
nextStates[STATE_TARGET_COMPILER] = STATE_TARGET_ARCH
nextStates[STATE_TARGET_ARCH] = STATE_PREFIX
nextStates[STATE_PREFIX] = STATE_PREFIX
nextStates[STATE_LIBFABRIC] = STATE_PREFIX
nextStates[STATE_HWLOC] = STATE_RE2
nextStates[STATE_RE2] = STATE_PREFIX
nextStates[STATE_COMM_SUBSTRATE] = STATE_GASNET_SEGMENT
nextStates[STATE_GASNET_SEGMENT] = STATE_PREFIX

# Parse the given path and return a configuration based on the components of the path.
# The configuration is a dictionary where the key is the environment variable name and the
# value is its value. As a side effect the variables are added to the "used" set.

def Parse(path):
    global used

    state = STATE_TARGET_PLATFORM

    config = {}
    i = 0
    while i < len(path):
        component = path[i]
        # Get environment variable that corresponds to the current state.
        var = varNames[state]
        # Get default next state. Note that it might be changed below.
        nextState = nextStates[state]
        j = i + 1
        if state == STATE_PREFIX and '-' in component:
            fields = component.split('-')
            (prefix, value) = fields[0:2]
            var = prefixes[prefix]
            config[var] = value
            used.add(var)
            if var == 'CHPL_COMM':

                if value != "none":
                    # The value of CHPL_COMM_DEBUG is determined by a suffix of the component name.
                    # It's a bit odd in that the build is based on whether or not it is set, 
                    # not its value. "+" denotes that is it set, "-" that it is not set.

                    config['CHPL_COMM_DEBUG'] = "+" if fields[-1] == 'debug' else "-"
                    used.add('CHPL_COMM_DEBUG')
                if value == 'ofi':
                    nextState = STATE_LIBFABRIC
                elif value == 'gasnet':
                    nextState = STATE_COMM_SUBSTRATE
        else:
            # Component doesn't have a prefix. If we were expecting a prefix then go to the
            # next state and reprocess the current component. Components without prefixes
            # have been deprecated so this should only happen with old builds.
            if state == STATE_PREFIX:
                nextState = STATE_HWLOC
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
    filters = ['no-tidy'] if all else ['tidy']
    lines = printchplenv.printchplenv(['runtime'], print_filters=filters, 
                                      print_format='simple').split('\n')
    for line in lines:
        if len(line) == 0:
            continue
        (var, value) = line.split('=', 1)
        if var in ['CHPL_HOME']:
            continue
        config[var] = value
        if var == 'CHPL_COMM' and value != "none":
            # CHPL_COMM_DEBUG isn't returned by printchplenv
            value = '+' if os.getenv('CHPL_COMM_DEBUG') else '-'
            config['CHPL_COMM_DEBUG'] = value
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

def main(args):
    global configs
    global used
    global usedvars
    usage = "Usage %prog [options]"
    description = "Displays the available Chapel runtime builds. Values that differ from" \
    " the current configuration have a '*' suffix. '+' denotes a variable that is set but" \
    " whose value doesn't matter, '-' denotes a variable that is not set. 'NA' denotes a" \
    " variable that is not applicable to the build." 

    parser = optparse.OptionParser(version = "1.0", usage=usage, description=description)
    group = optparse.OptionGroup(parser, "Sorting")
    group.add_option("-n", "--newest", action="store_true", dest="newest", default=False,
                      help="Sort builds with newest first. [default]")
    group.add_option("-o", "--oldest", action="store_true", dest="oldest", default=False,
                      help="Sort builds with oldest first.")
    group.add_option("-f", "--fewest", action="store_true", dest="fewest", default=False,
                      help="Sort builds with fewest differences first.")
    group.add_option("-m", "--most", action="store_true", dest="most", default=False,
                      help="Sort builds with most differences first.")
    parser.add_option_group(group)

    group = optparse.OptionGroup(parser, "Display")
    group.add_option("-C", "--no-current", action="store_false", dest="current", default=True,
                      help="Do not display the current configuration.")
    group.add_option("-s", "--summary", action="store_true", dest="summary", default=False,
                      help="Summarize the differences between the builds.")
    group.add_option("-w", "--width", action="store", type="int", dest="width", default=6,
                      metavar="NUM",
                      help="Number of columns in the output [default: %default].")
    parser.add_option_group(group)

    group = optparse.OptionGroup(parser, "Shell Commands")
    group.add_option("-b", "--bash", action="store", type="int", dest="bash", default=None,
                      metavar="NUM",
                      help="Output bash commands to match specified build.")
    group.add_option("-c", "--csh", action="store", type="int", dest="csh", default=None,
                      metavar="NUM",
                      help="Output csh commands to match specified build.")
    parser.add_option_group(group)

    group = optparse.OptionGroup(parser, "Misc.")
    group.add_option("-p", "--path", action="store", dest="path", default=None,
                      help="Parse the given runtime build path.")
    group.add_option("-k", "--check", action="store_true", dest="check", default=False,
                      help="Check that configuration matches a build.")
    parser.add_option_group(group)

    (options, args) = parser.parse_args(args[1:])
    if len(args) > 0:
        parser.error("Extra arguments.")

    sorting = 0
    if options.newest:
        sorting += 1
    if options.oldest:
        sorting += 1
    if options.fewest:
        sorting += 1
    if options.most:
        sorting += 1

    if sorting > 1:
        parser.error("You can only specify one of --newest, --oldest, --fewest, and --most.")

    if sorting == 0:
        options.newest = True

    if options.csh and options.bash:
        parser.error("You can only specify one of --csh and --bash.")

    if options.width < 1:
        parser.error("Number of columns must be >= 1.")

    if options.summary:
        if sorting > 0:
            parser.error("Sort option cannot be used with --summary.")
        if options.csh:
            parser.error("--summary cannot be used with --csh.")
        if options.bash:
            parser.error("--summary cannot be used with --bash.")
        if options.check:
            parser.error("--summary cannot be used with --check.")

    if options.check:
        if sorting > 0:
            parser.error("Sort option cannot be used with --check.")
        if options.csh:
            parser.error("--check cannot be used with --csh.")
        if options.bash:
            parser.error("--check cannot be used with --bash.")


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

    if options.path is not None:
        builds = [Parse(options.path.split(os.sep))]
    else:
        # gather the paths for all builds
        home = os.getenv('CHPL_HOME')
        if home is None:
            print("Error: CHPL_HOME is not set", file=sys.stderr)
            sys.exit(1)

        base = os.path.join(home, "lib")
        # "targets" is a list of all paths that contain a "main.o" file
        targets = []
        for root, dirs, files in os.walk(base):
            for target in filter(lambda x: x == "main.o", files):
                targets.append(os.path.join(root, target))

        if options.oldest:
            targets.sort(key= lambda x: os.stat(x)[stat.ST_MTIME])

        if options.newest:
            targets.sort(key= lambda x: os.stat(x)[stat.ST_MTIME], reverse=True)

        # Convert the targets into a list of paths in which each path is a list of
        # components, excluding the final "main.o"
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

    if options.summary:
        # summarize which variables differ between the configs and their values
        diffvars = set()
        for i in range(len(builds)):
            diffvars |= set(FindDiffVars(usedvars, current, builds[i]))
        values = {}
        for config in [current] + builds:
            for var in usedvars:
                if var in diffvars:
                    value = config.get(var, 'NA')
                    if var not in values.keys():
                        values[var] = set([value])
                    else:
                        values[var].add(value)
                    #print(var, value, values[var])

        for var in usedvars:
            if var in diffvars:
                 print("%25s: %-20s" % (var, list(values[var])))
        sys.exit(0)

    if options.fewest or options.most or options.check:
        # Sort based on # of differences from current config
        for i in range(len(builds)):
            builds[i]['diffs'] = len(FindDiffVars(usedvars, current, builds[i]))
        reverse = False if (options.fewest or options.check) else True
        builds.sort(key= lambda x: x['diffs'], reverse=reverse)

    if options.csh is not None:
        if options.csh > len(builds):
            parser.error("Invalid build ", options.csh)
        PrintShellCommands(current, builds[options.csh], shell="csh")
        sys.exit(0)

    if options.bash is not None:
        if options.bash > len(builds):
            parser.error("Invalid build ", options.bash)
        PrintShellCommands(current, builds[options.bash],shell="bash")
        sys.exit(0)

    if options.check:
        if len(builds) > 0:
            if builds[0]['diffs'] == 0:
                print("There is a build for the current configuration.")
                sys.exit(0)
        print("No build for the current configuration.")
        sys.exit(1)

    if options.current:
        # Print current configuration too
        configs = [current] + builds
    else:
        configs = builds

    extras = ['MTIME']
    done = 0
    headers = 0
    offset = 0
    # print the output options.width columns at a time
    while done < len(configs):
        columns = min(options.width, len(configs) - done)
        # print column headers
        output = "%26s " % ''
        if options.current and done == 0:
            output += "%-20s" % '<Current>'
            headers = columns - 1
        else:
            headers = columns
        for i in range(headers):
            output += " %3d%17s" % (i + done - offset, '')
        print(output)
        if options.current and done == 0:
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
        print
        done += columns


if __name__ == '__main__':
    main(sys.argv)
