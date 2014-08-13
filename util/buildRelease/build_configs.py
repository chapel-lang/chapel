#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Interactive CLI for building one or more Chapel configurations.

"""

from __future__ import absolute_import, print_function, unicode_literals

import contextlib
import itertools
import logging
import optparse
import os
import shlex
import subprocess
import time


class Dimension(object):
    """Encapsulate information about a single dimension."""

    def __init__(self, name, var_name, values=None, default=None, help_text=None):
        """Initialize a new dimension."""
        self.name = name
        self.var_name = var_name
        self.values = values
        self.default = default
        self.help_text = help_text.format(**locals())

        self.validate()

    def __repr__(self):
        """Stringify this dimsion."""
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


class _Dimensions(object):
    """Dimensions that this script knows about for configuring chapel compiler and
    runtime.

    FIXME: Add details about creating new dimensions in this class!
    """

    comm = Dimension(
        'comm', 'CHPL_COMM',
        values=['none', 'gasnet'],
        default='none',
        help_text='Chapel communcation ({var_name}) value to build. (default: {default})',
    )

    gmp = Dimension(
        'gmp', 'CHPL_GMP',
        values=['none', 'gmp', 'system'],
        default='none',
        help_text='GMP ({var_name}) values to build. (default: {default})',
    )

    tasks = Dimension(
        'task', 'CHPL_TASKS',
        values=['fifo', 'qthreads'],
        default='fifo',
        help_text='Tasks ({var_name}) values to build. (default: {default})',
    )

    @classmethod
    def get_dims(cls):
        """Returns list of the class attributes (aka the dimensions)."""
        dims = []
        for attr, value in cls.__dict__.iteritems():
            if not attr.startswith('_') and isinstance(value, Dimension):
                dims.append(value)
        return dims

    def __iter__(self):
        self._iter_index = 0
        self._dims = self.get_dims()
        return self

    def next(self):
        if self._iter_index < len(self._dims):
            i = self._iter_index
            self._iter_index += 1
            return self._dims[i]
        else:
            raise StopIteration


# Create instance of _Dimensions.
Dimensions = _Dimensions()


class Chapel(object):

    communication = ['none', 'gasnet',]
    gmp = ['none', 'gmp', 'system',]
    tasks = ['fifo', 'qthreads',]

    class defaults(object):

        communication = 'none'
        gmp = 'none'
        tasks = 'fifo'


class Config(object):

    attrs = ['comm', 'gmp', 'tasks']

    def __init__(self, comm=None, gmp=None, tasks=None):
        """Initialize new configuration value.

        :type comm: str
        :arg comm: CHPL_COMM value

        :type gmp: str
        :arg gmp: CHPL_GMP value

        :type tasks: str
        :arg tasks: CHPL_TASKS value
        """
        self.comm = comm
        self.gmp = gmp
        self.tasks = tasks

    def __repr__(self):
        """Return stringified version of Configuration."""
        cls_name = self.__class__.__name__
        attr_str = ', '.join(
            map(lambda x: '{0}={1}'.format(x, getattr(self, x, None)), self.attrs)
        )
        return '{0}({1})'.format(cls_name, attr_str)

    def __str__(self):
        """Return name string for this configuration."""
        return ' '.join(
            map(lambda x: '{0}={1}'.format(x, getattr(self, x, None)), self.attrs)
        )

    def verbose_str(self):
        """Return verbose string of configs - one per line indented."""
        return '\n'.join(
            map(lambda x: '    {0}={1}'.format(x, getattr(self, x, None)), self.attrs)
        )

    def get_env(self, orig_env):
        """Update and return an existing configuration with this configuration's
        values.

        :type orig_env: dict
        :arg orig_env: environment variables dictionary

        :rtype: dict
        :returns: updated dict to be used as the environment
        """
        new_env = orig_env.copy()

        new_env['CHPL_COMM'] = self.comm
        new_env['CHPL_TASKS'] = self.tasks

        return new_env


def main():
    """Main CLI function."""
    opts, args = parse_args()
    setup_logging(opts.verbose)

    orig_env = os.environ.copy()

    # TODO: Clean env... remove any existing chapel vars?

    build_configs = get_configs(opts)
    config_count_str = '{0} configuration{1}'.format(
        len(build_configs),
        's' if len(build_configs) > 1 else '')
    logging.info('Building {0}.'.format(config_count_str))
    logging.debug('Build configs: {0}'.format(build_configs))

    with elapsed_time('All {0}'.format(config_count_str)):
        for build_config in build_configs:
            build_chpl(
                opts.chpl_home,
                build_config,
                orig_env,
                verbose=opts.verbose
            )


def get_configs(opts):
    """Compile list of configurations to build based on command line options.

    :type opts: optparse.Values
    :arg opts: options parsed from command line

    :rtype: list of Config instances
    :returns: list of configurations to build
    """
    logging.debug('opts is type: {0}'.format(type(opts)))
    logging.debug('Compiling configs from: {0}'.format(opts))

    def get_value_with_default(config_str):
        """Returns the option value. If value is None, returns the default."""
        opt_value = getattr(opts, config_str, None)
        if opt_value is None:
            return [getattr(Chapel.defaults, config_str)]
        else:
            return opt_value

    comm = get_value_with_default('communication')
    gmp = get_value_with_default('gmp')
    tasks = get_value_with_default('tasks')

    # This is a big giant nested loop...
    config_strings = itertools.product(comm, gmp, tasks)

    configs = []
    for config_tuple in config_strings:
        configs.append(Config(*config_tuple))

    return configs


def build_chpl(chpl_home, build_config, env, verbose=False):
    """Build Chapel with the provided environment.

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type build_config: Config
    :arg build_config: build configuration to build

    :type env: dict
    :arg env: Dictionary of key/value pairs to set as the environment.

    :type verbose: bool
    :arg verbose: if True, increase output

    :rtype: FIXME
    :returns: FIXME
    """
    build_env = build_config.get_env(env)
    logging.info('Building config: {0}'.format(build_config))
    with elapsed_time(build_config):
        result = check_output('make', chpl_home, build_env, verbose=verbose)
    logging.info('Finished config:\n{0}'.format(build_config.verbose_str()))

    # FIXME: handle non-success results...


def check_output(command, chpl_home, env, stdin=None, verbose=False):
    """Runs command in subprocess and returns result.

    :type command: str or list
    :arg command: command to run

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type env: dict
    :arg env: environment dictionary to pass to subprocess

    :type stdin: str
    :arg stdin: string to pass as stdin to process

    :type verbose: bool
    :arg verbose: if True, let stdout/stderr stream

    :rtype: FIXME
    :returns: FIXME
    """
    if isinstance(command, basestring):
        command = shlex.split(command)

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
        env=env,
    )
    out, _ = p.communicate(input=stdin)

    # FIXME: handle non-zero exit statuses.

    return out


@contextlib.contextmanager
def elapsed_time(timer_name):
    start_time = time.time()
    yield
    elapsed = time.time() - start_time
    logging.info('{0} finished in {1:.3f} seconds.'.format(
        timer_name, elapsed))


def parse_args():
    """Parse and return command line arguments."""
    parser = optparse.OptionParser(
        usage='usage: %prog [options] args',
        description=__doc__
    )

    parser.set_defaults(**{
        # Default to CHPL_HOME from environ or current chapel source tree.
        'chpl_home': (os.environ.get('CHPL_HOME') or
                      os.path.abspath(os.path.join(os.getcwd(), '../..'))),
    })


    parser.add_option(
        '-v', '--verbose',
        action='store_true',
        help='Verbose output.'
    )
    parser.add_option(
        '--chpl-home',
        help='CHPL_HOME setting. (default: %default)'
    )

    config_group = optparse.OptionGroup(
        parser, 'Configuration Options',
        'Configuration options for building the Chapel compiler and runtime.')
    config_group.add_option(
        '--communication',
        action='append', choices=Chapel.communication,
        help='Communication (CHPL_COMM) values to build. (default: {0})'.format(Chapel.defaults.communication)
    )
    config_group.add_option(
        '--gmp',
        action='append', choices=Chapel.gmp,
        help='GMP (CHPL_GMP) values to build. (default: {0})'.format(Chapel.defaults.gmp)
    )
    config_group.add_option(
        '--tasks',
        action='append', choices=Chapel.tasks,
        help='Tasks (CHPL_TASKS) values to build. (default: {0})'.format(Chapel.defaults.tasks)
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
    main()
