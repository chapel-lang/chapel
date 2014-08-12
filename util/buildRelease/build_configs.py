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


class Chapel(object):

    comm = ['none', 'gasnet',]
    tasks = ['fifo', 'qthreads',]

    class defaults(object):

        communication = 'none'
        tasks = 'fifo'


class Config(object):

    attrs = ['comm', 'tasks']

    def __init__(self, comm=None, tasks=None):
        """Initialize new configuration value.

        :type comm: str
        :arg comm: CHPL_COMM value

        :type tasks: str
        :arg tasks: CHPL_TASKS value
        """
        self.comm = comm
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
    logging.debug('Build configs: {0}'.format(build_configs))

    for build_config in build_configs:
        build_chpl(opts.chpl_home, build_config, orig_env, verbose=opts.verbose)


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
    tasks = get_value_with_default('tasks')

    # This is a big giant nested loop...
    config_strings = itertools.product(comm, tasks)

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
    logging.info('Starting config:\n{0}'.format(build_config.verbose_str()))
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
        action='append', choices=Chapel.comm,
        help='Communication (CHPL_COMM) values to build. (default: {0})'.format(Chapel.defaults.communication)
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
