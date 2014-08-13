#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Interactive CLI for building one or more Chapel configurations.

TODO: Handle errors in build process better...
TODO: Update defaults to take into account current env (i.e. if CHPL_REGEXP is set to re2 in environment, use that as default).
TODO: Add flag to ignore env when picking defaults, maybe --ignore-environment.
TODO: Add additional configuration flags.
TODO: Split up compile process into stages (compile, runtime, then third-party, etc).
TODO: Parallelize build stages that are ammenable (e.g. runtime, third-party, etc).
TODO: Add --all-configs (?) flag that will build all configurations.
TODO: Figure out how to best support complex configs, like comm with substrate and segment values.
TODO: Add interactive mode where user is asked what configs they want.
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


"""Dimensions this script know about when compiling chapel. Order determines
how they will show up in the usage and what order is used in iteractive mode.
"""
Dimensions = [
    Dimension(
        'comm', 'CHPL_COMM',
        values=['none', 'gasnet'],
        default='none',
        help_text='Chapel communcation ({var_name}) value to build. (default: {default})',
    ),
    Dimension(
        'gmp', 'CHPL_GMP',
        values=['none', 'gmp', 'system'],
        default='none',
        help_text='GMP ({var_name}) values to build. (default: {default})',
    ),
    Dimension(
        'regexp', 'CHPL_REGEXP',
        values=['none', 're2'],
        default='none',
        help_text='Regular expression ({var_name}) values to buid. (default: {default})',
    ),
    Dimension(
        'task', 'CHPL_TASKS',
        values=['fifo', 'qthreads'],
        default='fifo',
        help_text='Tasks ({var_name}) values to build. (default: {default})',
    ),
]


class Chapel(object):

    communication = ['none', 'gasnet',]
    gmp = ['none', 'gmp', 'system',]
    tasks = ['fifo', 'qthreads',]

    class defaults(object):

        communication = 'none'
        gmp = 'none'
        tasks = 'fifo'


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
    logging.debug('Compiling configs from: {0}'.format(opts))

    # Create a list of lists. The inner lists are the values to build for each
    # dimension (i.e. communcation). The outer list encapsulates all the
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

    for dim in Dimensions:
        config_group.add_option(
            '--{0}'.format(dim.name),
            action='append', choices=dim.values,
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
    main()
