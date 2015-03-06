#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Interactive CLI for building one or more Chapel configurations."""

# TODO: Add flag to ignore env when picking defaults, maybe --ignore-environment.
# TODO: Add additional configuration flags.
# TODO: Split up compile process into stages (compile, runtime, then third-party, etc).
# TODO: Parallelize build stages that are amenable (e.g. runtime, third-party, etc).
# TODO: Add --all-configs (?) flag that will build all configurations.
# TODO: Figure out how to support compiler configs. It is a bit challenging because the default should almost certainly come from chplenv (otherwise that logic will be duplicated here).
# TODO: Figure out how to best support complex configs, like comm with substrate and segment values.
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

# Add the chplenv dir to the python path.
chplenv_dir = os.path.join(os.path.dirname(__file__), 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

# Import chplenv modules here.
import chpl_comm
import chpl_gmp
import chpl_launcher
import chpl_llvm
import chpl_locale_model
import chpl_make
import chpl_mem
import chpl_regexp
import chpl_tasks


class Dimension(object):
    """Encapsulate information about a single dimension."""

    def __init__(self, name, var_name, values=None, default=None, help_text=None):
        """Initialize a new dimension."""
        self.name = name
        self.var_name = var_name
        self.values = values
        self.default = default
        self.help_text = self._get_help_text(help_text.format(**locals()))

        self.validate()

    def _get_help_text(self, help_text):
        """Returns help text with some additional content.

        :type help_text: str
        :arg help_text: original help text string (unformatted)

        :rtype: str
        :returns: formatted help text
        """
        return '{0} (default: {1})'.format(help_text, self.default)

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
        if not self.values or not isinstance(self.values, list):
            raise ValueError('values must be a non-empty list: {0}'.format(
                self.values))
        if not self.default or not isinstance(self.default, basestring):
            raise ValueError('default must be a non-empty string: {0}'.format(
                self.default))
        if not self.help_text or not isinstance(self.help_text, basestring):
            raise ValueError('help_text must be a non-empty string: {0}'.format(
                self.help_text))


"""Dimensions this script knows about when compiling chapel. Order determines
how they will show up in the usage and what order is used in interactive mode.
"""
Dimensions = [
    Dimension(
        'comm', 'CHPL_COMM',
        values=['none', 'gasnet'],
        default=chpl_comm.get(),
        help_text='Chapel communcation ({var_name}) value to build.',
    ),
    Dimension(
        'task', 'CHPL_TASKS',
        values=['fifo', 'qthreads'],
        default=chpl_tasks.get(),
        help_text='Tasks ({var_name}) values to build.',
    ),
    Dimension(
        'mem', 'CHPL_MEM',
        values=['cstdlib', 'tcmalloc', 'dlmalloc'],
        default=chpl_mem.get('target'),
        help_text='Memory allocator ({var_name}) values to build.',
    ),
    Dimension(
        'launcher', 'CHPL_LAUNCHER',
        values=['none', 'pbs-aprun', 'aprun', 'slurm-srun'],
        default=chpl_launcher.get(),
        help_text='Launcher ({var_name}) to build.',
    ),
    Dimension(
        'gmp', 'CHPL_GMP',
        values=['none', 'gmp', 'system'],
        default=chpl_gmp.get(),
        help_text='GMP ({var_name}) values to build.',
    ),
    Dimension(
        'regexp', 'CHPL_REGEXP',
        values=['none', 're2'],
        default=chpl_regexp.get(),
        help_text='Regular expression ({var_name}) values to build.',
    ),
    Dimension(
        'localeModel', 'CHPL_LOCALE_MODEL',
        values=['flat', 'numa'],
        default=chpl_locale_model.get(),
        help_text='Locale model ({var_name}) values to build.',
    ),
    Dimension(
        'llvm', 'CHPL_LLVM',
        values=['none', 'llvm'],
        default=chpl_llvm.get(),
        help_text='LLVM ({var_name}) values to build.',
    ),
]


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
        f = lambda x: '{0}={1}'.format(x, getattr(self, x))
        return ' '.join(map(lambda dim: f(dim.name), Dimensions))

    def verbose_str(self):
        """Return verbose string of configs - one per line indented."""
        f = lambda x: '    {0}={1}'.format(x, getattr(self, x))
        return '\n'.join(map(lambda dim: f(dim.name), Dimensions))

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
            new_env[dim.var_name] = getattr(self, dim.name)

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

    if opts.parallel:
        logging.info('Using parallel execution for build.')

    build_env = os.environ.copy()

    build_configs = get_configs(opts)
    config_count_str = '{0} configuration{1}'.format(
        len(build_configs),
        's' if len(build_configs) > 1 else '')
    logging.info('Building {0}.'.format(config_count_str))
    logging.debug('Build configs: {0}'.format(build_configs))

    statuses = []
    with elapsed_time('All {0}'.format(config_count_str)):
        for build_config in build_configs:
            result = build_chpl(
                opts.chpl_home,
                build_config,
                build_env,
                parallel=opts.parallel,
                verbose=opts.verbose
            )
            statuses.append((build_config, result))

    # Sum of all exit codes is the exit code from this program.
    exit_code = reduce(
        lambda a, b: a+b,
        map(operator.itemgetter(1), statuses),
        0
    )
    return exit_code


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
            logging.debug(
                'No value specified for {0}, defaulting to: {1}'.format(
                    dim.name, dim.default))
            dimension_values.append([dim.default])
        else:
            dimension_values.append(values)

    # Create the cross product of all dimension values.
    config_strings = itertools.product(*dimension_values)

    configs = []
    for config_tuple in config_strings:
        configs.append(Config(*config_tuple))

    return configs


def build_chpl(chpl_home, build_config, env, parallel=False, verbose=False):
    """Build Chapel with the provided environment.

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type build_config: Config
    :arg build_config: build configuration to build

    :type env: dict
    :arg env: Dictionary of key/value pairs to set as the environment.

    :type parallel: bool
    :arg parallel: enable parallel execution for build

    :type verbose: bool
    :arg verbose: if True, increase output

    :rtype: int
    :returns: exit code from building configuration
    """
    build_env = build_config.get_env(env)
    logging.info('Building config: {0}'.format(build_config))

    make_cmd = chpl_make.get()
    if parallel:
        make_cmd += ' --jobs={0}'.format(multiprocessing.cpu_count())
    logging.debug('Using make command: {0}'.format(make_cmd))

    with elapsed_time(build_config):
        result, output, error = check_output(
            make_cmd, chpl_home, build_env, verbose=verbose)
        logging.debug('Exit code for config {0}: {1}'.format(
            build_config, result))
    logging.info('Finished config:\n{0}'.format(build_config.verbose_str()))

    if result != 0:
        if output is not None:
            logging.error('stdout:\n{0}'.format(output))
        if error is not None:
            logging.error('stderr:\n{0}'.format(error))
        logging.error('Non-zero exit code when building config {0}: {1}'.format(
            build_config, result))

    return result


def check_output(command, chpl_home, env, stdin=None, verbose=False):
    """Runs command in subprocess and returns result.

    :type command: str
    :arg command: command to run

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type env: dict
    :arg env: environment dictionary to pass to subprocess

    :type stdin: str
    :arg stdin: string to pass as stdin to process

    :type verbose: bool
    :arg verbose: if True, let stdout/stderr stream

    :rtype: tuple(int, str, str)
    :returns: exit code/status from subcommand, stdout string, stderr string
    """
    command = shlex.split(str(command))

    stdout = None
    stderr = None
    if not verbose:
        stdout = subprocess.PIPE
        stderr = subprocess.STDOUT

    p = subprocess.Popen(
        command,
        stdout=stdout,
        stderr=stderr,
        cwd=chpl_home,
        env=env
    )
    out, err = p.communicate(input=stdin)
    retcode = p.returncode

    return retcode, out, err


@contextlib.contextmanager
def elapsed_time(timer_name):
    start_time = time.time()
    yield
    elapsed = time.time() - start_time
    logging.info('{0} finished in {1:.3f} seconds.'.format(
        timer_name, elapsed))


def print_configs():
    """Print each configuration dimension and its possible values."""
    for dim in Dimensions:
        print('{name} - {help_text}'.format(name=dim.name, var_name=dim.var_name, help_text=dim.help_text))
        print('    Default: {0}'.format(dim.default))
        print('    Values:')
        for value in dim.values:
            print('      {0}'.format(value))
        print()


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
        if v not in choices:
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
        usage='usage: %prog [options] args',
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

    config_group = optparse.OptionGroup(
        parser, 'Configuration Options',
        'Configuration options for building the Chapel compiler and runtime.')

    for dim in Dimensions:
        config_group.add_option(
            '--{0}'.format(dim.name),
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
