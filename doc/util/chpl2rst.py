#!/usr/bin/env python3
# encoding: utf-8

"""chpl2rst converts a chapel program to an rst file, where all comments are
rendered rst, and all code is wrapped in code blocks.

Chapel files are converted as follows:

* Line comments starting at the beginning of the line are converted to text
* Block comments starting anywhere are converted to text
* All other lines are wrapped as code-blocks

example.chpl
============

/* This is
     an example */
proc foo() {
    // this comment will be in the code block
    var bar = 1; }
// this comment will be text

example.rst
===========

This is
  an example

.. code-block:: chapel

    proc foo() {
        // this comment will be in the code block
        var bar = 1; }

this comment will be text"""

import os
import sys
from literate_chapel import to_pieces, title_comment

import argparse

def get_arguments():
    """
    Get arguments from command line
    """
    parser = argparse.ArgumentParser(
        prog='chpl2rst',
        usage='%(prog)s  file.chpl [options] ',
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument('chapelfiles', nargs='+',
                        help='Chapel files to convert to rst')
    parser.add_argument('--output', default='rst', choices=['stdout', 'rst'],
                        help='destination of output')
    parser.add_argument('--prefix', default='.',
                        help='prefix path for output')
    parser.add_argument('--codeblock', '-c', action='store_true', default=False,
                        help='convert entire document into code block')
    parser.add_argument('--verbose', '-v', action='store_true', default=False,
                        help='verbosity')
    parser.add_argument('--link', '-l', default=None)
    return parser.parse_args()


def gen_link(link, chapelfile):
    """Generate hyperlink to GitHub URL based on chapelfile path"""
    # Note - this makes the assumption that the file lives in the github repo
    abspath = os.path.realpath(chapelfile)
    filename = os.path.split(chapelfile)[1]

    chpl_home_env = os.getenv('CHPL_MAKE_HOME')
    if not chpl_home_env:
        chpl_home_env = os.getenv('CHPL_HOME')

    if not chpl_home_env:
        print('Error: --link flag only works when $CHPL_HOME is defined')
        sys.exit(1)

    chpl_home = os.path.realpath(chpl_home_env)
    if not chpl_home in abspath:
        print('Error: --link flag only work for files within $CHPL_HOME')
        print('CHPL_HOME: {0}'.format(chpl_home))
        print('file path: {0}'.format(abspath))
        sys.exit(1)

    # Get path from CHPL_HOME directory
    chplpath = abspath.replace(chpl_home, '').lstrip('/')
    hyperlink = 'https://github.com/chapel-lang/chapel/blob/{0}/{1}'.format(link, chplpath)
    rstlink = '`View {0} on GitHub <{1}>`_'.format(filename, hyperlink)
    return rstlink


def gen_title(chapelfile):
    """Generate file title, based on if title comment exists"""
    with open(chapelfile, 'r', encoding='utf-8') as handle:
        line1 = handle.readline()
        if title_comment(line1):
            title = line1.lstrip('//').strip()
        else:
            filename = os.path.split(chapelfile)[1]
            title = filename

    return title


def gen_preamble(chapelfile, link=None):
    """Generate preamble for rst file"""

    # Strip path and extension from chapelfile
    filename = os.path.split(chapelfile)[1]
    basename, _ = os.path.splitext(filename)

    domain = '.. default-domain:: chpl'
    reference = '.. _primers-{0}:'.format(basename)

    title = gen_title(chapelfile)

    output = []
    output.append(domain)
    output.append('')
    output.append(reference)
    output.append('')
    output.append(title)
    output.append('='*len(title))
    output.append('')

    # Generate dynamic links below title
    if link:
        output.append(gen_link(link, chapelfile))
        output.append('')

    return '\n'.join(output)

def gen_rst(pieces, chapelfile):
    output = []

    # Each line is rst or code-block
    for (kind, content) in pieces:
        if kind == 'title': continue
        elif kind == 'prose':
            output.extend(content)
        elif kind == 'code':
            output.append('.. code-block:: chapel')
            output.append('')
            output.extend('    ' + line for line in content)
        elif kind == 'output':
            # Don't support output
            continue
        output.append('')
    return '\n'.join(output)


def gen_codeblock(handle):
    """Wrap contents of file handle with 'code-block:: chapel' and return the
    resulting contents as a string"""

    output = []
    output.append('')
    output.append('.. code-block:: chapel')
    output.append('')

    for (i, line) in enumerate([l.strip('\n') for l in handle.readlines()]):
        if i == 0:
            if title_comment(line):
                continue

        output.append('  ' + line)

    return '\n'.join(output)

def getfname(chapelfile, output, prefix):
    """Compute filename for output"""
    if output == 'rst':
        filename = os.path.split(chapelfile)[1]
        basename, _ = os.path.splitext(filename)
        rstname = ''.join([basename, '.rst'])
        rstfile = os.path.join(prefix, rstname)
        return rstfile
    elif output == 'stdout':
        print('Warning: prefix ignored for stdout')
        return None
    else:
        print('Error: output = {0} is invalid')
        sys.exit(1)


def write(rstoutput, output):
    """Write to output"""
    if output is None:
        sys.stdout.write(rstoutput)
        return

    with open(output, 'w', encoding='utf-8') as handle:
        handle.write(rstoutput)


def main_args(**kwargs):
    """Driver function - convert each file to rst and write to output"""

    # Parse keyword arguments
    chapelfiles = kwargs['chapelfiles']
    output = kwargs['output']
    prefix = kwargs['prefix']
    codeblock = kwargs['codeblock']
    link = kwargs['link']
    verbose = kwargs['verbose']

    for chapelfile in chapelfiles:
        islearnChapelInYMinutes = (os.path.basename(chapelfile)=='learnChapelInYMinutes.chpl')
        preamble = gen_preamble(chapelfile, link=link)

        with open(chapelfile, 'r', encoding='utf-8') as handle:
            if codeblock:
                rstoutput = gen_codeblock(handle)
            else:
                pieces = to_pieces(handle, islearnChapelInYMinutes)
                rstoutput = gen_rst(pieces, chapelfile)

        rstoutput = '\n'.join([preamble, rstoutput])

        fname = getfname(chapelfile, output, prefix)

        if not os.path.exists(prefix) and output != 'stdout':
            print('Creating directory: ', prefix)
            os.makedirs(prefix)

        if verbose:
            print('writing output of {0} to {1}'.format(chapelfile, fname))

        write(rstoutput, fname)

def main():
    # Parse arguments and cast them into a dictionary
    arguments = vars(get_arguments())
    main_args(**arguments)

if __name__ == '__main__':
    main()
