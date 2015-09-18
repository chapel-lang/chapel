#!/usr/bin/env python
# encoding: utf-8

from __future__ import print_function

import os
import sys
import shlex
import shutil

from subprocess32 import Popen, PIPE
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter


def get_arguments(args=None):
    """
    Get arguments from command line
    :args: Arguments, if predefined
    :returns: Opts, the arguments parsed
    """
    parser = ArgumentParser(prog='build_webdocs.py',
                            usage='%(prog)s [options] ',
                            description=''' %(prog)s builds the documentation pages for Chapel''',
                            formatter_class=ArgumentDefaultsHelpFormatter
                            )

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='see input and output of subprocess calls')
    parser.add_argument('--push', '-p', default=None, type=str,
                        help='rsync html contents to a destination, \
                              e.g. `--push chapcs01:public_html/`')

    opts = parser.parse_args(args)
    return opts


def copytree(src, dst, symlinks=False, ignore=None, rmprefix=''):
    """Copy trees recursively

       This is a modified version of shutil.copytree created because
       shutil.copytree arbitrarily doesn't let you copy into already existing
       directories.
    """
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            copytree(s, d, symlinks, ignore)
        else:
            dpath = os.path.dirname(d)
            if not os.path.exists(dpath):
                os.makedirs(dpath)
            shutil.copy2(s, d)


def makewebdocs(modulespath, verbose=False):
    """Call `make webdocs` in directory $CHPL_HOME/modules"""

    originalpath = os.getcwd()

    os.chdir(modulespath)

    makewebdocs_command = 'make webdocs'

    if verbose:
        print(makewebdocs_command)

    p = Popen(shlex.split(makewebdocs_command), stdout=PIPE, stderr=PIPE)
    stdout, stderr = p.communicate()

    if verbose:
        print(stdout)
        print(stderr)

    os.chdir(originalpath)

    return p.returncode


def makehtml(webdocspath, verbose=False):
    """Call `make html` in directory $CHPL_HOME/modules/webdocs"""

    makehtml_command = 'make -C {0} html'.format(webdocspath)

    if verbose:
        print(makehtml_command)

    p = Popen(shlex.split(makehtml_command), stdout=PIPE, stderr=PIPE)

    stdout, stderr = p.communicate()

    if verbose:
        print(stdout)
        print(stderr)

    return p.returncode


def rsync(src, dst, flags='avP', verbose=False):
    """Rsync webdocs onto server"""
    rsync_command = 'rsync -{0} {1} {2}'.format(flags, src, dst)

    if verbose:
        print(rsync_command)

    p = Popen(shlex.split(rsync_command), stdout=PIPE, stderr=PIPE)

    stdout, stderr = p.communicate()

    if verbose:
        print(stdout)
        print(stderr)

    return p.returncode


def extfilter(directory, extension='.rst'):
    """Filter out files without extension '.rst'"""
    return [os.path.join(directory, f) for f in os.listdir(directory) if os.path.splitext(f)[1] == extension]


def build_webdocs(dst=None, verbose=False):
    """ Build the web docs
        This script builds the web documentation for Chapel.
        It first builds the module documents with chpldoc via the modules/Makefile.
        The contents of both doc/release and ./webdocs are then copied into the sphinx
        project ($CHPL_HOME/modules/webdocs) and the html docs are built with sphinx.
    """

    # Check that $CHPL_HOME is defined, and set the path to a variable
    chpl_home = os.getenv('CHPL_HOME')
    if not chpl_home:
        print('$CHPL_HOME is not set. Unable to build webdocs')
        return 1

    # Get tree of 'index.rst's to copy over
    filedir = os.path.dirname(os.path.realpath(__file__))
    webdocs = os.path.join(filedir, 'webdocs')

    chpl_modules = os.path.join(chpl_home, 'modules')

    # Build webdocs via chpldoc
    makewebdocscode= makewebdocs(chpl_modules, verbose=verbose)

    # Check that make webdocs succeeded
    if makewebdocscode != 0:
        print('An error has occured when calling `make webdocs`')
        return 1

    modules_webdocs = os.path.join(chpl_modules, 'webdocs')

    # Copy index.rst's over
    copytree(webdocs, modules_webdocs)

    # Create list of READMEs converted to rst
    docs = {}
    docs['source'] = extfilter(os.path.join(chpl_home, 'doc/release'))
    docs['source/platforms'] = extfilter(os.path.join(chpl_home, 'doc/release/platforms'))
    docs['source/technotes'] = extfilter(os.path.join(chpl_home, 'doc/release/technotes'))

    # Copy READMEs into webdocs/source/{directory} with new names
    for directory, rstfiles in docs.items():
        source_directory = os.path.join(modules_webdocs, '{0}'.format(directory))
        for rstfile in rstfiles:
            # strip off README. prefix
            newrstfilename = os.path.split(rstfile)[1].replace('README.', '')
            newpath = os.path.join(source_directory, newrstfilename)
            shutil.copy(rstfile, newpath)

    # Build the html docs
    makehtmlcode = makehtml(modules_webdocs, verbose=verbose)

    # Check that make html succeeded
    if makehtmlcode!= 0:
        print('An error has occured when calling `make html`')
        return 1

    # Push build to provided destination
    if dst:
        src = os.path.join(modules_webdocs, 'build/html')
        rsynccode = rsync(src, dst, verbose=verbose)
        if rsynccode != 0:
            print('An error has occured when calling `rsync`')
            return 1

    return 0


def main():
    """Call build_webdocs and report success / errors based on return"""
    opts = get_arguments()

    build_webdocscode = build_webdocs(dst=opts.push, verbose=opts.verbose)

    if build_webdocscode == 0:
        print('Web docs built successfully')
    else:
        print('An error has occurred while building the web docs')
        if not opts.verbose:
            print('Try running with `--verbose` for more output')

    return build_webdocscode


if __name__ == '__main__':
    main()
