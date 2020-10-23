#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""CLI for building multiple Chapel configurations."""

# TODO: Add flag to ignore env when picking defaults, maybe --ignore-environment.
# TODO: Add interactive mode where user is asked what configs they want.

from __future__ import absolute_import, print_function, unicode_literals

import contextlib
import itertools
import logging
import multiprocessing
import operator
import optparse
import os
import shlex
import subprocess
import sys
import time

try:
    basestring
except NameError:
    basestring = str

class Dimension(object):
    """Encapsulate information about a single dimension."""

    def __init__(self, name, var_name, values=None, default=None, help_text=None):
        """Initialize a new dimension."""
        self.name = name
        self.var_name = var_name
        self.values = values
        self.default = default
        if help_text:
            self.help_text = self._get_help_text(help_text.format(**locals()))
        else:
            self.help_text = self._get_help_text('')

        self.validate()

    def _get_help_text(self, help_text):
        """Returns help text with some additional content.

        :type help_text: str
        :arg help_text: original help text string (unformatted)

        :rtype: str
        :returns: formatted help text
        """
        if self.default:
            return '{0} (default: {1})'.format(help_text, self.default)
        else:
            return help_text

    def __repr__(self):
        """Stringify this dimension."""
        cls_name = self.__class__.__name__
        attrs = ['name', 'var_name', 'values', 'default', 'help_text']
        f = lambda x: '{0}={1!r}'.format(x, getattr(self, x, None))
        attr_list = ', '.join(map(f, attrs))
        return '{0}({1})'.format(cls_name, attr_list)

    def validate(self):
        """Validate instance parameters. Raises ValueError if dimension values are not
        valid."""
        if not self.name and self.name != self.name.lower():
            raise ValueError('name must be a lower case string: {0}'.format(
                self.name))
        if not self.var_name or self.var_name != self.var_name.upper():
            raise ValueError('var_name must an upper case string: {0}'.format(
                self.var_name))
        if not isinstance(self.help_text, basestring):
            raise ValueError('help_text must be a string: {0}'.format(
                self.help_text))


"""Dimensions this script knows about when compiling chapel. Order determines
how they will show up in the usage and what order is used in interactive mode.
"""

known_dimensions = [
    ( 'host_platform',      'CHPL_HOST_PLATFORM', ),
    ( 'host_compiler',      'CHPL_HOST_COMPILER', ),
    ( 'host_machine',       'CHPL_HOST_ARCH', ),
    ( 'target_platform',    'CHPL_TARGET_PLATFORM', ),
    ( 'target_compiler',    'CHPL_TARGET_COMPILER', ),
    ( 'target_machine',     'CHPL_TARGET_ARCH', ),
    ( 'cpu',                'CHPL_TARGET_CPU', ),
    ( 'locale_model',       'CHPL_LOCALE_MODEL', ),
    ( 'comm',               'CHPL_COMM', ),
    ( 'substrate',          'CHPL_COMM_SUBSTRATE', ),
    ( 'segment',            'CHPL_GASNET_SEGMENT', ),
    ( 'tasks',              'CHPL_TASKS', ),
    ( 'launcher',           'CHPL_LAUNCHER', ),
    ( 'timers',             'CHPL_TIMERS', ),
    ( 'unwind',             'CHPL_UNWIND', ),
    ( 'mem',                'CHPL_MEM', ),
    ( 'atomics',            'CHPL_ATOMICS', ),
    ( 'network_atomics',    'CHPL_NETWORK_ATOMICS', ),
    ( 'gmp',                'CHPL_GMP', ),
    ( 'hwloc',              'CHPL_HWLOC', ),
    ( 'regexp',             'CHPL_REGEXP', ),
    ( 'llvm',               'CHPL_LLVM', ),
    ( 'auxfs',              'CHPL_AUX_FILESYS', ),
    ( 'lib_pic',            'CHPL_LIB_PIC', ),
]
Dimensions = []
for (name, var_name) in known_dimensions:
    value = os.environ.get(var_name)
    if isinstance(value, basestring) and not value:
        value = None
    Dimensions.append(Dimension(name, var_name, default=value))

class Config(object):

    def __init__(self, *args, **kwargs):
        """Initialize new configuration value. Arguments are based on the dimensions
        object. Order is important and should also be based on the iteration
        order of dimensions.
        """
        for i, dim in enumerate(Dimensions):
            setattr(self, dim.name, args[i])

    def __repr__(self):
        """Return stringified version of Configuration."""
        cls_name = self.__class__.__name__
        f = lambda x: '{0}={1!r}'.format(x, getattr(self, x))
        attr_list = ', '.join(map(lambda dim: f(dim.name), Dimensions))
        return '{0}({1})'.format(cls_name, attr_list)

    def __str__(self):
        """Return name string for this configuration."""
        values = []
        for dim in Dimensions:
            value = getattr(self, dim.name)
            if value:
                values.append('{0}={1}'.format(dim.name, value))
        return ','.join(values)

    def verbose_str(self):
        """Return verbose string of configs - one per line indented."""
        values = []
        for dim in Dimensions:
            value = getattr(self, dim.name)
            if value:
                values.append('\t{0}={1}'.format(dim.name, value))
        return '\n'.join(values)

    def pretty_str(self,header=False):
        """Return pretty string of configs - one line of a formatted table."""
        values = []
        for dim in Dimensions:
            value = getattr(self, dim.name)
            if value:
                if header:
                    # for a header line at the top with the names of the config variables
                    values.append('%10.10s' % (dim.name))
                else:
                    # a normal line with just the config values
                    values.append('%10.10s' % (value))
        return ' '.join(values)

    def get_env(self, orig_env):
        """Update and return an existing configuration with this configuration's
        values.

        :type orig_env: dict
        :arg orig_env: environment variables dictionary

        :rtype: dict
        :returns: updated dict to be used as the environment
        """
        new_env = orig_env.copy()

        for dim in Dimensions:
            new_env_var = getattr(self, dim.name)
            if not new_env_var or not isinstance(new_env_var, basestring) or new_env_var == 'UNSET':
                if new_env_var and dim.var_name in new_env:
                    del new_env[dim.var_name]
            else:
                new_env[dim.var_name] = new_env_var

        return new_env


def main():
    """Main CLI function.

    :rtype: int
    :returns: exit code, which is sum of all exit codes
    """
    opts, args = parse_args()
    setup_logging(opts.verbose)

    if opts.show_configs:
        print_configs()
        return 0

    build_env = os.environ.copy()
    chpl_misc = get_chpl_misc(opts, args, build_env)

    build_configs = get_configs(opts)
    config_count_str = '{0} configuration{1}'.format(
        len(build_configs),
        's' if len(build_configs) > 1 else '')
    logging.info('Building {0}.'.format(config_count_str))

    def list_config_names():
        """Return a complete formatted table showing all the chapel configs in this build."""
        names = []
        for i, build_config in enumerate(build_configs):
            if i == 0:
                # prepend header row
                build_config_name = build_config.pretty_str(header=True)
                if not build_config_name:
                    build_config_name = 'None'
                names.append('')
                names.append('           ' + build_config_name)
            # normal table row
            build_config_name = build_config.pretty_str()
            if not build_config_name:
                build_config_name = 'None'
            names.append('%3d / %3d  %s' % (i+1, len(build_configs), build_config_name))
        return names

    logging.info('\n'.join(list_config_names()))

    make_logfile = chpl_misc['make_logfile']
    chpl_home = chpl_misc['chpl_home']
    if make_logfile:
        print('\n[BUILD_CONFIGS] CHPL_HOME={0}'.format(chpl_home), file=make_logfile)
        print('\n[BUILD_CONFIGS] Building {0}'.format(config_count_str), file=make_logfile)
        print('\n'.join(list_config_names()), file=make_logfile)

    statuses = [0,]
    with elapsed_time('All {0}'.format(config_count_str)):
        for i, build_config in enumerate(build_configs):
            result = build_chpl(
                '{0} / {1}'.format(i+1, len(build_configs)),
                build_config,
                build_env,
                chpl_misc,
                parallel=opts.parallel,
                verbose=opts.verbose,
                dry_run=opts.dry_run,
            )
            statuses.append(result)

    # exit from this program.
    exit_code = max(statuses)
    return exit_code

def get_chpl_misc(opts, args, build_env):
    """Miscellaneous chplenv-related paths, settings, and options

    :type opts: optparse.Values
    :arg opts: Options parsed from command line

    :type args: list
    :arg args: Additional command line parameters, not found in opts

    :type build_env: dict
    :arg build_env: Dictionary of key/value pairs to set as the environment.

    :rtype dict
    :returns: Miscellaneous chplenv-related paths, settings, and options
    """

    # full path to CHPL_HOME
    chpl_home_opt = os.path.abspath(opts.chpl_home)
    chpl_home_env = build_env.get('CHPL_HOME')
    if chpl_home_env:
        chpl_home_env = os.path.abspath(chpl_home_env)
        if chpl_home_env != chpl_home_opt:
            logging.warn('Resetting build_env[CHPL_HOME],\nfrom\t{0}\nto\t{1}'.format(chpl_home_env, chpl_home_opt))

    chpl_home = chpl_home_opt
    logging.info('Using CHPL_HOME={0}'.format(chpl_home))
    build_env['CHPL_HOME'] = chpl_home
    if not os.path.isdir(chpl_home):
        logging.error('CHPL_HOME={0} not found'.format(chpl_home))
        sys.exit(2)

    # path to "CHPL_HOME/util/chplenv" dir
    chplenv_dir = os.path.join(chpl_home, 'util', 'chplenv')
    if not os.path.isdir(chplenv_dir):
        logging.error('chplenv_dir={0} not found'.format(chplenv_dir))
        sys.exit(2)

    # path to chpl_make.py
    chpl_make_py = os.path.join(chplenv_dir, 'chpl_make.py')
    if not os.path.isfile(chpl_make_py):
        logging.error('chpl_make_py={0} not found'.format(chpl_make_py))
        sys.exit(2)

    # path to printchplenv
    chplenv_exe = os.path.join(chpl_home, 'util', 'printchplenv')
    if not os.path.isfile(chplenv_exe):
        logging.error('printchplenv={0} not found'.format(chplenv_exe))
        sys.exit(2)

    # run "printchplenv" without any commandline config options or setenv, check for errors
    chplenv_cmd = chplenv_exe + ' --all --no-tidy --anonymize'
    result, output, error = check_output(chplenv_cmd, chpl_home, build_env)
    if result:
        logging.error('From {0}\n{1}\n{2}\nexit code {3}'.format(chplenv_cmd, error, output, result))
        sys.exit(2)
    if error:
        logging.warn('From {0}\n{1}\n{2}'.format(chplenv_cmd, error, output))
    elif not output:
        logging.error('{0}\nreturns no output'.format(chplenv_cmd))
        sys.exit(2)

    # Chapel's preferred make command on this platform ("make", "gmake")
    result, output, error = check_output(chpl_make_py, chpl_home, build_env)
    if result or error:
        logging.error('From {0}\n{1}\n{2}\nexit code {3}'.format(chpl_make_py, error, output, result))
        sys.exit(2)
    if output:
        chpl_make = output
        logging.debug('Chapel make is {0}'.format(output))
    else:
        logging.error('{0}\nreturns no output'.format(chpl_make_py))
        sys.exit(2)

    # construct the Chapel make command line, including all options and extras
    make_cmd = chpl_make
    dryrun_cmd = chplenv_cmd

    if opts.parallel:
        def _cpu_count():
            """ return Python cpu_count(), optionally capped by env var CHPL_MAKE_MAX_CPU_COUNT
            """
            cpus = multiprocessing.cpu_count()
            try:
                max = os.getenv('CHPL_MAKE_MAX_CPU_COUNT', '0')
                if int(max) > 0:
                    cpus = min(int(max), cpus)
            except:
                pass
            return cpus

        logging.info('Using parallel make with {0} CPUs'.format(_cpu_count()))
        make_cmd += ' --jobs={0}'.format(_cpu_count())

    # extra options appended to Chapel "make", if any, not checked
    if args and isinstance(args, basestring):
        make_targets = args
    elif args:
        make_targets = ' '.join(args)
    else:
        make_targets = ''
    if make_targets:
        logging.info('Using make_targets={0}'.format(make_targets))

    make_cmd += ' {0}'.format(make_targets)

    # setenv file, if any
    if opts.setenv:
        setenv = os.path.abspath(opts.setenv)
        if not os.path.isfile(setenv):
            logging.error('setenv={0} not found'.format(setenv))
            sys.exit(2)
        logging.info('Using setenv={0}'.format(setenv))
        if opts.verbose:
            x = '-x'
        else:
            x = '+x'
        make_cmd = 'bash {0} -c "source {1} && {2}"'.format(x, setenv, make_cmd)
        dryrun_cmd = 'bash {0} -c "source {1} 2>&1 && {2}"'.format(x, setenv, dryrun_cmd)
    else:
        setenv = None

    if opts.dry_run:
        logging.debug('Using dryrun_cmd={0}'.format(dryrun_cmd))
    else:
        logging.debug('Using make_cmd={0}'.format(make_cmd))

    if opts.make_log:
        logging.info('Using Chapel make log file={0}'.format(opts.make_log))
        make_logfile = open(opts.make_log, 'w')
    else:
        make_logfile = None

    return {
        'chpl_home'     : chpl_home,
        'make_cmd'      : make_cmd,
        'dryrun_cmd'    : dryrun_cmd,
        'chplenv_exe'   : chplenv_exe,
        'chplenv_cmd'   : chplenv_cmd,
        'chpl_make'     : chpl_make,
        'setenv'        : setenv,
        'make_targets'  : make_targets,
        'make_logfile'  : make_logfile,
    }

def get_configs(opts):
    """Compile list of configurations to build based on command line options.

    :type opts: optparse.Values
    :arg opts: options parsed from command line

    :rtype: list of Config instances
    :returns: list of configurations to build
    """
    logging.debug('Compiling configs from: {0}'.format(opts))

    # Create a list of lists. The inner lists are the values to build for each
    # dimension (i.e. communication). The outer list encapsulates all the
    # dimensions.
    dimension_values = []
    for dim in Dimensions:
        values = getattr(opts, dim.name)
        if values is None:
            if dim.default:
                logging.debug('No value specified for {0}, defaulting to: {1}'.format(dim.name, dim.default))
            dimension_values.append([dim.default])
        else:
            dimension_values.append(values)

    # Create the cross product of all dimension values.
    config_strings = itertools.product(*dimension_values)

    configs = []
    for config_tuple in config_strings:
        configs.append(Config(*config_tuple))

    return configs


def build_chpl(counter, build_config, env, chpl_misc, parallel=False, verbose=False, dry_run=False):
    """Build Chapel with the provided environment.

    :type counter: string
    :arg counter: current config number / total number of configs, for edits

    :type build_config: Config
    :arg build_config: build configuration to build

    :type env: dict
    :arg env: Dictionary of key/value pairs to set as the environment.

    :type chpl_misc: dict
    :arg chpl_misc: miscellaneous chplenv-related paths, settings, and options

    :type verbose: bool
    :arg verbose: if True, increase output

    :type dry_run: bool
    :arg dry_run: if True, do not actually run Chapel make

    :rtype: int
    :returns: exit code from building configuration
    """

    build_env = build_config.get_env(env)

    chpl_home       = chpl_misc['chpl_home']
    make_cmd        = chpl_misc['make_cmd']
    dryrun_cmd      = chpl_misc['dryrun_cmd']
    make_logfile    = chpl_misc['make_logfile']
    make_targets    = chpl_misc['make_targets']

    build_config_name = '{0}'.format(build_config)
    if not build_config_name:
        build_config_name = 'None'

    logging.info('Building config number {0}:\n\t{1}'.format(counter, build_config_name))
    if make_logfile:
        print('\n[BUILD_CONFIGS] config number {0}:\n\t{1}'.format(counter, build_config_name), file=make_logfile)
        print('\n[BUILD_CONFIGS] make_targets:\n\t{0}'.format(make_targets), file=make_logfile)

    build_env['BUILD_CONFIGS_CALLBACK'] = '{0}'.format(build_config_name)
    build_env['BUILD_CONFIGS_VERBOSE'] = '{0}'.format(verbose)
    build_env['BUILD_CONFIGS_DRY_RUN'] = '{0}'.format(dry_run)
    build_env['BUILD_CONFIGS_CHPLENV_EXE'] = chpl_misc['chplenv_exe']
    build_env['BUILD_CONFIGS_CHPL_MAKE'] = chpl_misc['chpl_make']
    build_env['BUILD_CONFIGS_MAKE_TARGETS'] = chpl_misc['make_targets']

    if dry_run:
        build_env['BUILD_CONFIGS_DRYRUN_CMD']  = dryrun_cmd

        # dry-run: basically, dry-run runs printchplenv instead of Chapel make.
        #   Non-zero printchplenv exit status means "error", this Chapel config failed
        #   Anything on stderr provokes a "warning" message but is not an error
        #   Printchplenv is run twice if logfile is used, to see Chapel env on both console and logfile

        logging.info('DRY-RUN: printchplenv command:\n\t{0}'.format(dryrun_cmd))
        with elapsed_time('Config number {0}'.format(counter)):
            result, output, error = check_output(dryrun_cmd, chpl_home, build_env)
        if result or error:
            logging.warn('Errors/Warnings from printchplenv, config {0}\n{1}\n{2}\nExit code {3}'.format(
                build_config_name, output, error, result))
        elif verbose:
            logging.debug('Results from printchplenv, config {0}\n{1}\n{2}\nExit code {3}'.format(
                build_config_name, output, error, result))
        make_result = result

        if make_logfile:
            logging.debug('Printchplenv for logfile:\n\t{0}'.format(dryrun_cmd))
            print('[BUILD_CONFIGS] DRY-RUN: printchplenv command:\n\t{0}\n'.format(dryrun_cmd), file=make_logfile)
            result, output, error = check_output(dryrun_cmd, chpl_home, build_env, file=make_logfile)
    else:
        build_env['BUILD_CONFIGS_MAKE_CMD'] = make_cmd

        # default: Chapel make. If no logfile is used then Chapel make output is lost!
        #   Non-zero make exit status means "error", this Chapel config failed
        #   Printchplenv may be run after the make, to add Chapel env to the logfile

        logging.info('Chapel make command:\n\t{0}'.format(make_cmd))
        if make_logfile:
            print('[BUILD_CONFIGS] Chapel make command:\n\t{0}\n'.format(make_cmd), file=make_logfile)
        with elapsed_time('Config number {0}'.format(counter)):
            result, output, error = check_output(make_cmd, chpl_home, build_env, file=make_logfile)
        if result:
            logging.error('Non-zero exit code {0}'.format(result))
        make_result = result

        if make_logfile:
            logging.info('Printchplenv for logfile:\n\t{0}'.format(dryrun_cmd))
            print('[BUILD_CONFIGS] printchplenv command:\n\t{0}\n'.format(dryrun_cmd), file=make_logfile)
            result, output, error = check_output(dryrun_cmd, chpl_home, build_env, file=make_logfile)

    return make_result


def check_output(command, chpl_home, env, stdin=None, file=None):
    """Runs command in subprocess and returns result.

    :type command: str
    :arg command: command to run

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type env: dict
    :arg env: environment dictionary to pass to subprocess

    :type stdin: str
    :arg stdin: string to pass as stdin to process

    :type file: File
    :arg file: if it exists, redirect stdout/stderr stream

    :rtype: tuple(int, str, str)
    :returns: exit code/status from subcommand, stdout string, stderr string
    """
    command = shlex.split(str(command))

    if file:
        stdout = file
        stderr = subprocess.STDOUT
        file.flush()
    else:
        stdout = subprocess.PIPE
        stderr = subprocess.PIPE

    p = subprocess.Popen(
        command,
        stdout=stdout,
        stderr=stderr,
        cwd=chpl_home,
        env=env,
        encoding='utf-8'
    )
    out, err = p.communicate(input=stdin)
    retcode = p.returncode

    if file:
        if out:
            raise ValueError('if file exists then "out" should be empty')
        if err:
            raise ValueError('if file exists then "err" should be empty')
    else:
        if out:
            out=out.strip()
        else:
            out=''
        if err:
            err=err.strip()
        else:
            err=''

    return retcode, out, err


@contextlib.contextmanager
def elapsed_time(timer_name):
    start_time = time.time()
    yield
    elapsed = time.time() - start_time
    logging.info('{0} finished in {1:.3f} seconds.'.format(
        timer_name, elapsed))


def print_configs():
    """Print each configuration dimension and its allowed values (if any)"""
    for dim in Dimensions:
        print('{name}\t{help_text}\n\t{var_name}'.format(name=dim.name, var_name=dim.var_name, help_text=dim.help_text))
        if dim.values:
            print('\t\tValues:')
            for value in dim.values:
                print('\t\t{0}'.format(value))


def parse_config_value_callback(option, opt, value, parser, choices, **kwargs):
    """OptionParser callback function for parsing the configuration value passed by user.

    It takes values that have commas (e.g. the user specified
    --comm=none,gasnet), breaks them apart and adds the individual values to
    list of values.

    :type option: optparse.Option
    :arg option: Option instance that's calling the callback

    :type opt_str: str
    :arg opt_str: option string seen on the command line that's triggering the
        callback

    :type value: str
    :arg value: value for this option passed by user

    :type parser: optparse.OptionParser
    :arg parser: instance of option parser that is doing work

    :type choices: list
    :arg choices: list of valid choices for this option
    """
    # Get the existing values the parser knows about for this particular
    # option.
    value_list = getattr(parser.values, option.dest, None) or []

    # Split the value provided. Validate each value parsed here.
    parsed_vals = value.split(',')
    for v in parsed_vals:
        if choices and v not in choices:
          parser.error('option {0}: invalid choice: {1} (choose from {2})'.format(
              opt, v, choices))

    # Add the new vals to the existing list of values, and set them on option.
    value_list += parsed_vals
    setattr(parser.values, option.dest, value_list)


def parse_args():
    """Parse and return command line arguments."""
    usage_description = __doc__ + """

To select multiple values for a particular setting, use a comma separated list
or pass the argument multiple times. For example, to build with comm=none and
comm=gasnet either of these will work:

  %prog --comm=none,gasnet
  %prog --comm=none --comm=gasnet"""

    class NoWrapHelpFormatter(optparse.IndentedHelpFormatter):
        """Help formatter that does not wrap the description text."""

        def _format_text(self, text):
            return text

    parser = optparse.OptionParser(
        usage='usage: %prog [options] [extra options appended to Chapel "make" commands]',
        description=usage_description,
        formatter=NoWrapHelpFormatter()
    )

    parser.set_defaults(**{
        # Default to CHPL_HOME from environ or current chapel source tree.
        'chpl_home': (os.environ.get('CHPL_HOME') or
                      os.path.abspath(
                          os.path.join(os.path.dirname(__file__), '../..'))),
    })

    parser.add_option(
        '-v', '--verbose',
        action='store_true',
        help='Verbose output.'
    )
    parser.add_option(
        '-n', '--dry-run',
        action='store_true',
        help='Dry run only. Do not run Chapel make.'
    )
    parser.add_option(
        '-l', '--make-log',
        help='Output file to receive Chapel make output (default: output not saved)'
    )
    parser.add_option(
        '--show-configs',
        action='store_true',
        help='Print all possible values for all configurations.'
    )
    parser.add_option(
        '--chpl-home',
        help='CHPL_HOME setting. (default: %default)'
    )
    parser.add_option(
        '-p', '--parallel',
        action='store_true',
        help='Enable parallel execution for build.'
    )
    parser.add_option(
        '-s', '--setenv',
        help='Input file with bash-compatible env setup commands (default: None).',
    )

    config_group = optparse.OptionGroup(
        parser, 'Configuration Options',
        'Configuration options for building the Chapel compiler and runtime.')

    for dim in Dimensions:
        config_group.add_option(
            '--{0}'.format(dim.name.replace('_','-')),
            metavar=dim.var_name,
            action='callback', type="string",
            callback=parse_config_value_callback, callback_args=(dim.values,),
            help=dim.help_text
        )

    parser.add_option_group(config_group)

    opts, args = parser.parse_args()
    return opts, args


def setup_logging(verbose=False):
    """Initialize logging and set level based on verbose.

    :type verbose: bool
    :arg verbose: When True, set log level to DEBUG.
    """
    log_level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(format='%(asctime)s [%(levelname)s] %(message)s',
                        level=log_level)
    logging.debug('Verbose output enabled.')


if __name__ == '__main__':
    exit(main())
