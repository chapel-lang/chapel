#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Interactive CLI for building one or more Chapel configurations.

"""

from __future__ import absolute_import, print_function, unicode_literals

import contextlib
import logging
import optparse
import os
import shlex
import subprocess
import time


def main():
    """Main CLI function."""
    opts, args = parse_args()
    setup_logging(opts.verbose)

    orig_env = os.environ.copy()

    # TODO: Clean env... remove any existing chapel vars?
    # TODO: Set new chpl vars

    build_chpl(opts.chpl_home, orig_env)


def build_chpl(chpl_home, env):
    """Build Chapel with the provided environment.

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type env: dict
    :arg env: Dictionary of key/value pairs to set as the environment.

    :rtype: FIXME
    :returns: FIXME
    """
    config_name = 'default chapel build'
    logging.info('Starting {0} config FIXME...'.format(config_name))
    with elapsed_time('default chapel build'):
        result = check_output('make', chpl_home, env)
    logging.debug('Result: {0}'.format(result))
    logging.info('Finished {0} config FIXME...'.format(config_name))


def check_output(command, chpl_home, env, stdin=None):
    """Runs command in subprocess and returns result.

    :type command: str or list
    :arg command: command to run

    :type chpl_home: str
    :arg chpl_home: CHPL_HOME env var

    :type env: dict
    :arg env: environment dictionary to pass to subprocess

    :type stdin: str
    :arg stdin: string to pass as stdin to process

    :rtype: FIXME
    :returns: FIXME
    """
    if isinstance(command, basestring):
        command = shlex.split(command)
    p = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=chpl_home,
        env=env,
    )
    out, _ = p.communicate(input=stdin)
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


    parser.add_option('-v', '--verbose', action='store_true',
                      help='Verbose output.')
    parser.add_option('--chpl-home',
                      help='CHPL_HOME setting. (default: %default)')

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
