#!/usr/bin/env python

"""Usage: printchplenv [options]

Print the current Chapel configuration. Arguments allow selection of
what gets printed [content], how it gets printed [format], and what gets
filtered [filter].

The default [content] provides user-facing variables.

Options:
  -h, --help    Show this help message and exit

  [shortcut]
  --all         Shortcut for --compiler --runtime --launcher, includes defaults

  [content]
  --compiler    Select variables describing the configuration of the compiler
  --runtime     Select variables describing the configuration of the runtime
  --launcher    Select variables describing the configuration of the launcher
  --internal    Select additional variables used during builds
                 this flag is incompatible with [format]: --path

  [filter]
  --[no-]tidy   (default) [don't] Omit sub-variables irrelevant to the current
                 configuration
  --anonymize   Omit machine specific details, script location, and CHPL_HOME
  --overrides   Omit variables that have not been user supplied via environment
                 or chplconfig

  [format]
  --pretty      (default) Print variables in format: CHPL_KEY: VALUE
                 indicating which options are set by environment variables (*)
                 and which are set by configuration files (+)
  --simple      Print variables in format: CHPL_KEY=VALUE
                 output is compatible with chplconfig format
  --make        Print variables in format: CHPL_MAKE_KEY=VALUE
  --path        Print variables in format: VALUE1/VALUE2/...
                 this flag always excludes CHPL_HOME and CHPL_MAKE
"""

from collections import namedtuple
from functools import partial
import optparse
import os
from sys import stdout, path

from chplenv import *

ChapelEnv = namedtuple('ChapelEnv', ['name', 'content', 'shortname'])

# Set default argument of 'shortname' to None
ChapelEnv.__new__.__defaults__ = (None,)

# Define base sets
COMPILER = set(['compiler'])
LAUNCHER = set(['launcher'])
RUNTIME = set(['runtime'])
INTERNAL = set(['internal'])
DEFAULT = set(['default'])

# Global ordered list that stores names, content-categories, and shortnames
CHPL_ENVS = [
    ChapelEnv('CHPL_HOST_PLATFORM', COMPILER | LAUNCHER),
    ChapelEnv('CHPL_HOST_COMPILER', COMPILER | LAUNCHER),
    ChapelEnv('CHPL_HOST_ARCH', COMPILER | LAUNCHER),
    ChapelEnv('CHPL_HOST_CPU', INTERNAL),
    ChapelEnv('CHPL_TARGET_PLATFORM', RUNTIME | DEFAULT),
    ChapelEnv('CHPL_TARGET_COMPILER', RUNTIME | DEFAULT),
    ChapelEnv('CHPL_TARGET_ARCH', RUNTIME | DEFAULT),
    ChapelEnv('CHPL_ORIG_TARGET_COMPILER', INTERNAL),
    ChapelEnv('CHPL_TARGET_CPU', RUNTIME | DEFAULT, 'arch'),
    ChapelEnv('CHPL_RUNTIME_CPU', INTERNAL),
    ChapelEnv('CHPL_TARGET_CPU_FLAG', INTERNAL),
    ChapelEnv('CHPL_TARGET_BACKEND_CPU', INTERNAL),
    ChapelEnv('CHPL_LOCALE_MODEL', RUNTIME | LAUNCHER | DEFAULT, 'loc'),
    ChapelEnv('CHPL_COMM', RUNTIME | LAUNCHER | DEFAULT, 'comm'),
    ChapelEnv('  CHPL_COMM_SUBSTRATE', RUNTIME | LAUNCHER | DEFAULT),
    ChapelEnv('  CHPL_GASNET_SEGMENT', RUNTIME | LAUNCHER | DEFAULT),
    ChapelEnv('  CHPL_LIBFABRIC', RUNTIME | INTERNAL | DEFAULT),
    ChapelEnv('CHPL_TASKS', RUNTIME | LAUNCHER | DEFAULT, 'tasks'),
    ChapelEnv('CHPL_LAUNCHER', LAUNCHER | DEFAULT, 'launch'),
    ChapelEnv('CHPL_TIMERS', RUNTIME | LAUNCHER | DEFAULT, 'tmr'),
    ChapelEnv('CHPL_UNWIND', RUNTIME | LAUNCHER | DEFAULT, 'unwind'),
    ChapelEnv('CHPL_HOST_MEM', INTERNAL, 'hostmem'),
    ChapelEnv('CHPL_TARGET_MEM', INTERNAL, 'tgtmem'),
    ChapelEnv('CHPL_MEM', RUNTIME | LAUNCHER | DEFAULT, 'mem'),
    ChapelEnv('  CHPL_JEMALLOC', INTERNAL, 'jemalloc'),
    ChapelEnv('CHPL_MAKE', INTERNAL, 'make'),
    ChapelEnv('CHPL_ATOMICS', RUNTIME | LAUNCHER | DEFAULT, 'atomics'),
    ChapelEnv('  CHPL_NETWORK_ATOMICS', INTERNAL | DEFAULT),
    ChapelEnv('CHPL_GMP', INTERNAL | DEFAULT, 'gmp'),
    ChapelEnv('CHPL_HWLOC', RUNTIME | DEFAULT),
    ChapelEnv('CHPL_REGEXP', RUNTIME | DEFAULT),
    ChapelEnv('CHPL_LLVM', COMPILER | DEFAULT, 'llvm'),
    ChapelEnv('CHPL_AUX_FILESYS', RUNTIME | DEFAULT, 'fs'),
    ChapelEnv('CHPL_LIB_PIC', RUNTIME | LAUNCHER, 'lib_pic'),
    ChapelEnv('CHPL_RUNTIME_SUBDIR', INTERNAL),
    ChapelEnv('CHPL_LAUNCHER_SUBDIR', INTERNAL),
    ChapelEnv('CHPL_COMPILER_SUBDIR', INTERNAL),
    ChapelEnv('CHPL_HOST_BIN_SUBDIR', INTERNAL),
    ChapelEnv('CHPL_TARGET_BIN_SUBDIR', INTERNAL),
    ChapelEnv('  CHPL_LLVM_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_GASNET_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_GMP_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_HWLOC_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_JEMALLOC_UNIQ_CFG_PATH',INTERNAL),
    ChapelEnv('  CHPL_LIBFABRIC_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_LIBUNWIND_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_QTHREAD_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_RE2_UNIQ_CFG_PATH', INTERNAL),
    ChapelEnv('  CHPL_THIRD_PARTY_COMPILE_ARGS', INTERNAL),
    ChapelEnv('  CHPL_THIRD_PARTY_LINK_ARGS', INTERNAL),
]

# Global map of environment variable names to values
ENV_VALS = {}

"""Compute '--all' env var values and populate global dict, ENV_VALS"""
def compute_all_values():
    global ENV_VALS

    # If we're doing an LLVM build, set CHPL_TARGET_COMPILER to clang-included
    # and set CHPL_ORIG_TARGET_COMPILER to whatever we would have used
    # otherwise.
    # This happens early because it modifies the environment and other
    # defaults might depend on this setting.
    llvm_codegen = ("CHPL_LLVM_CODEGEN" in os.environ and
                    os.environ["CHPL_LLVM_CODEGEN"] != "0")
    if llvm_codegen:
        new_target_compiler = chpl_compiler.get('target')
        if new_target_compiler != 'clang-included':
            # This error indicates something about chplenv (probably
            # chpl_compiler.py) is not working correctly with LLVM mode.
            raise ValueError('LLVM mode but target compiler is set incorrectly')

    ENV_VALS['CHPL_HOME'] = chpl_home_utils.get_chpl_home()
    ENV_VALS["CHPL_ORIG_TARGET_COMPILER"] = chpl_compiler.get('target', llvm_mode='orig')
    ENV_VALS['CHPL_HOST_PLATFORM'] = chpl_platform.get('host')
    ENV_VALS['CHPL_HOST_COMPILER'] = chpl_compiler.get('host')
    ENV_VALS['CHPL_HOST_ARCH'] = chpl_arch.get('host')
    ENV_VALS['CHPL_HOST_CPU'] = chpl_cpu.get('host').cpu
    ENV_VALS['CHPL_TARGET_PLATFORM'] = chpl_platform.get('target')
    ENV_VALS['CHPL_TARGET_COMPILER'] = chpl_compiler.get('target')
    ENV_VALS['CHPL_TARGET_ARCH'] = chpl_arch.get('target')
    ENV_VALS['CHPL_TARGET_CPU'] = chpl_cpu.get('target').cpu

    # Use module's LCD architecture in case it was built before
    # Internal, but this value is used in place of CHPL_TARGET_CPU for --path
    ENV_VALS['CHPL_RUNTIME_CPU'] = chpl_cpu.get('target',
            get_lcd=chpl_home_utils.using_chapel_module()).cpu

    ENV_VALS['CHPL_LOCALE_MODEL'] = chpl_locale_model.get()
    ENV_VALS['CHPL_COMM'] = chpl_comm.get()
    ENV_VALS['  CHPL_COMM_SUBSTRATE'] = chpl_comm_substrate.get()
    ENV_VALS['  CHPL_GASNET_SEGMENT'] = chpl_comm_segment.get()
    ENV_VALS['  CHPL_LIBFABRIC'] = chpl_libfabric.get()
    ENV_VALS['CHPL_TASKS'] = chpl_tasks.get()
    ENV_VALS['CHPL_LAUNCHER'] = chpl_launcher.get()
    ENV_VALS['CHPL_TIMERS'] = chpl_timers.get()
    ENV_VALS['CHPL_UNWIND'] = chpl_unwind.get()
    ENV_VALS['CHPL_MEM'] = chpl_mem.get('target')
    ENV_VALS['  CHPL_JEMALLOC'] = chpl_jemalloc.get()
    ENV_VALS['CHPL_MAKE'] = chpl_make.get()
    ENV_VALS['CHPL_ATOMICS'] = chpl_atomics.get()
    ENV_VALS['  CHPL_NETWORK_ATOMICS'] = chpl_atomics.get('network')
    ENV_VALS['CHPL_GMP'] = chpl_gmp.get()
    ENV_VALS['CHPL_HWLOC'] = chpl_hwloc.get()
    ENV_VALS['CHPL_REGEXP'] = chpl_regexp.get()
    ENV_VALS['CHPL_LLVM'] = chpl_llvm.get()
    aux_filesys = chpl_aux_filesys.get()
    ENV_VALS['CHPL_AUX_FILESYS'] = '_'.join(sorted(aux_filesys.split(' ')))
    ENV_VALS['CHPL_LIB_PIC'] = chpl_lib_pic.get()


"""Compute '--internal' env var values and populate global dict, ENV_VALS"""
def compute_internal_values():
    global ENV_VALS

    # Maps architecture name that Chapel uses to the name that can be included
    # in an argument like -march e.g. for gcc-4.7: 'ivybridge' -> 'core-avx-i'
    backend_info = chpl_cpu.get('target', map_to_compiler=True)
    ENV_VALS['CHPL_TARGET_CPU_FLAG'] = backend_info.flag
    ENV_VALS['CHPL_TARGET_BACKEND_CPU'] = backend_info.cpu

    ENV_VALS['CHPL_HOST_MEM'] = chpl_mem.get('host')
    ENV_VALS['CHPL_TARGET_MEM'] = chpl_mem.get('target')
    ENV_VALS['CHPL_RUNTIME_SUBDIR'] = printchplenv(set(['runtime']), print_format='path').rstrip('\n')
    ENV_VALS['CHPL_LAUNCHER_SUBDIR'] = printchplenv(set(['launcher']), print_format='path').rstrip('\n')
    ENV_VALS['CHPL_COMPILER_SUBDIR'] = printchplenv(set(['compiler']), print_format='path').rstrip('\n')
    ENV_VALS['CHPL_HOST_BIN_SUBDIR'] = chpl_bin_subdir.get('host')
    ENV_VALS['CHPL_TARGET_BIN_SUBDIR'] = chpl_bin_subdir.get('target')
    ENV_VALS['  CHPL_LLVM_UNIQ_CFG_PATH'] = chpl_llvm.get_uniq_cfg_path()

    compile_args_3p = []
    link_args_3p = []

    ENV_VALS['  CHPL_GASNET_UNIQ_CFG_PATH'] = chpl_gasnet.get_uniq_cfg_path()

    ENV_VALS['  CHPL_GMP_UNIQ_CFG_PATH'] = chpl_gmp.get_uniq_cfg_path()
    link_args_3p.extend(chpl_gmp.get_link_args(chpl_gmp.get()))

    ENV_VALS['  CHPL_HWLOC_UNIQ_CFG_PATH'] = chpl_hwloc.get_uniq_cfg_path()
    link_args_3p.extend(chpl_hwloc.get_link_args(chpl_hwloc.get()))

    ENV_VALS['  CHPL_JEMALLOC_UNIQ_CFG_PATH'] = chpl_jemalloc.get_uniq_cfg_path()
    link_args_3p.extend(chpl_jemalloc.get_link_args(chpl_jemalloc.get()))

    ENV_VALS['  CHPL_LIBFABRIC_UNIQ_CFG_PATH'] = chpl_libfabric.get_uniq_cfg_path()
    if chpl_comm.get() == 'ofi':
      compile_args_3p.extend(chpl_libfabric.get_compile_args())
      link_args_3p.extend(chpl_libfabric.get_link_args())

    ENV_VALS['  CHPL_LIBUNWIND_UNIQ_CFG_PATH'] = chpl_unwind.get_uniq_cfg_path()
    link_args_3p.extend(chpl_unwind.get_link_args(chpl_unwind.get()))

    ENV_VALS['  CHPL_QTHREAD_UNIQ_CFG_PATH'] = chpl_qthreads.get_uniq_cfg_path()
    if chpl_tasks.get() == 'qthreads':
        link_args_3p.extend(chpl_qthreads.get_link_args())

    ENV_VALS['  CHPL_RE2_UNIQ_CFG_PATH'] = chpl_regexp.get_uniq_cfg_path()
    if chpl_regexp.get() == 're2':
        link_args_3p.extend(chpl_regexp.get_link_args())

    # Remove duplicates, keeping last occurrence and preserving order
    # e.g. "-lhwloc -lqthread -lhwloc ..." -> "-lqthread -lhwloc ..."
    seen = set()
    compile_args_3p_dedup = [arg for arg in reversed(compile_args_3p)
                             if not (arg in seen or seen.add(arg))]
    ENV_VALS['  CHPL_THIRD_PARTY_COMPILE_ARGS'] = ' '.join(reversed(compile_args_3p_dedup))

    seen = set()
    link_args_3p_dedup = [arg for arg in reversed(link_args_3p)
                          if not (arg in seen or seen.add(arg))]
    ENV_VALS['  CHPL_THIRD_PARTY_LINK_ARGS'] = ' '.join(reversed(link_args_3p_dedup))


"""Return non-empty string if var is set via environment or chplconfig"""
def user_set(env):
    env_stripped = env.strip()
    env_set = overrides.get_environ(env_stripped, '')
    config_set = overrides.get_chplconfig(env_stripped, '')
    if env_set:
        return ' *'
    elif config_set:
        return ' +'
    return ''


"""Filter variables that are not user set"""
def filter_overrides(chpl_env):
    return bool(user_set(chpl_env.name))


"""Filter variables irrelevant to configuration for --tidy flag"""
def filter_tidy(chpl_env):
    comm = ENV_VALS['CHPL_COMM']
    if chpl_env.name == '  CHPL_COMM_SUBSTRATE':
        return comm == 'gasnet'
    elif chpl_env.name == '  CHPL_GASNET_SEGMENT':
        return comm == 'gasnet'
    elif chpl_env.name == '  CHPL_LIBFABRIC':
        return comm == 'ofi'
    elif chpl_env.name == '  CHPL_NETWORK_ATOMICS':
        return comm != 'none'
    return True


"""Filter variables that are not selected in contents
Requires a content argument via functools.partial
"""
def _filter_content(chpl_env, contents=None):
    return chpl_env.content.intersection(contents)


"""Return string to be printed for a given variable and print_format
Requires a print_format argument
"""
def _print_var(key, value, print_format=None, shortname=None):
    key_stripped = key.strip()
    if print_format == 'pretty':
        user_set_symbol = user_set(key_stripped)
        return "{0}: {1}{2}\n".format(key, value, user_set_symbol)
    elif print_format == 'simple':
        return "{0}={1}\n".format(key_stripped, value)
    elif print_format == 'make':
        make_key = key_stripped.replace("CHPL_", "CHPL_MAKE_", 1)
        return "{0}={1}\n".format(make_key, value)
    elif print_format == 'path':
        if shortname:
            ret = "{0}-{1}".format(shortname, value)
        else:
            ret = "{0}".format(value)
        return ret + '/'
    else:
        raise ValueError("Invalid format '{0}'".format(print_format))


"""Return a string that contains the Chapel configuration variable info"""
def printchplenv(contents, print_filters=None, print_format='pretty'):
    global CHPL_ENVS

    if print_filters is None:
        print_filters = ['tidy']

    # Error checking for external python codes calling printchplenv function
    if not ENV_VALS.items:
        raise KeyError('ENV_VALS must be populated with compute_*_values()'
                       'before printchplenv is called')

    # Specialize _filter_content to use contents as default arg
    filter_content = partial(_filter_content, contents=contents)

    envs = filter(filter_content, CHPL_ENVS)

    # --overrides
    if 'overrides' in print_filters:
        envs = filter(filter_overrides, envs)

    # --tidy
    if 'tidy' in print_filters:
        envs = filter(filter_tidy, envs)

    # Specialize _print_var to use print_format as default arg
    print_var = partial(_print_var, print_format=print_format)

    # List of strings that will be concatenated and returned
    ret = []

    # Print header
    if 'anonymize' not in print_filters:
        if print_format == 'pretty':
            ret.append("machine info: {0} {1} {2} {3} {4}\n".format(*os.uname()))
            ret.append(print_var('CHPL_HOME', ENV_VALS['CHPL_HOME']))
            this_dir = os.path.realpath(os.path.dirname(__file__))
            ret.append("script location: {0}\n".format(this_dir))
        elif print_format == 'simple':
            ret.append(print_var('CHPL_HOME', ENV_VALS['CHPL_HOME']))

    # Print environment variables and their values
    for env in envs:
        value = ENV_VALS[env.name]
        if print_format == 'path':
            if env.name == 'CHPL_TARGET_CPU':
                value = ENV_VALS['CHPL_RUNTIME_CPU']
            elif env.name == 'CHPL_COMM' and chpl_comm_debug.get() == 'debug':
                value += '-debug'
        ret.append(print_var(env.name, value, shortname=env.shortname))

    # Handle special formatting case for --path
    if print_format == 'path':
        # Remove trailing '/' and add a newline
        ret[-1] = ret[-1].rstrip('/')
        ret.append('\n')

    return ''.join(ret)


"""Define argument to parse"""
def parse_args():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        description = 'Print the current Chapel configuration. '
                      '[content] arguments determine what gets printed. '
                      '[filter] arguments determine what gets omitted. '
                      '[format] arguments determine how it gets printed. '
                      '[shortcut] arguments are for convenience.')

    #[shortcut]
    parser.add_option('--all', action='store_true', dest='all')

    #[content]
    parser.set_defaults(content=[])
    parser.add_option('--compiler', action='append_const', dest='content', const='compiler')
    parser.add_option('--runtime', action='append_const', dest='content', const='runtime')
    parser.add_option('--launcher', action='append_const', dest='content', const='launcher')
    parser.add_option('--internal', action='append_const', dest='content', const='internal')

    #[filter]
    parser.set_defaults(tidy=True)
    parser.set_defaults(filter=[])
    parser.add_option('--tidy', action='store_true', dest='tidy')
    parser.add_option('--no-tidy', action='store_false', dest='tidy')
    parser.add_option('--anonymize', action='append_const', dest='filter', const='anonymize')
    parser.add_option('--overrides', action='append_const', dest='filter', const='overrides')

    #[format]
    parser.set_defaults(format='pretty')
    parser.add_option('--pretty', action='store_const', dest='format', const='pretty')
    parser.add_option('--simple', action='store_const', dest='format', const='simple')
    parser.add_option('--make',   action='store_const', dest='format', const='make')
    parser.add_option('--path',   action='store_const', dest='format', const='path')

    # Hijack the help message to use the module docstring
    # optparse is not robust enough to support help msg sections for args.
    parser.print_help = lambda: stdout.write(__doc__)

    return parser.parse_args()


def main():
    (options, args) = parse_args()

    # Handle --all flag
    if options.all:
        options.content.extend(['runtime', 'launcher', 'compiler', 'default'])

    # Handle --tidy / --no-tidy flags
    if options.tidy:
        options.filter.append('tidy')

    # Set default [content]
    if not options.content:
        options.content = ['default']

    # Convert lists to sets to pass to printchplenv
    contents = set(options.content)
    filters = set(options.filter)

    # Prevent --internal --path, because it's useless
    if options.format == 'path' and 'internal' in contents:
        stdout.write('--path and --internal are incompatible flags\n')
        exit(1)

    # Populate ENV_VALS
    compute_all_values()

    # Don't populate internal ENV_VALS unless specified
    if 'internal' in contents:
        compute_internal_values()

    ret = printchplenv(contents, filters, options.format)
    stdout.write(ret)


if __name__ == '__main__':
    main()
