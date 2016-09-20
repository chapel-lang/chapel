#!/usr/bin/env python
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

from __future__ import print_function

import os
import sys

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


def gen_preamble(chapelfile, link=None):
    """Generate preamble for rst file"""

    # Strip path and extension from chapelfile
    filename = os.path.split(chapelfile)[1]
    basename, _ = os.path.splitext(filename)

    domain = '.. default-domain:: chpl'
    reference = '.. _primers-{0}:'.format(basename)
    title = filename

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
        # Note - this makes the assumption that the file lives in the test dir
        abspath = os.path.abspath(chapelfile)

        if 'chapel/test/' not in abspath:
            print('Error: --link flag only works for files in chapel/test/')
            sys.exit(1)

        # Get path from CHPL_HOME directory
        chplpath = abspath[abspath.find('chapel/test/'):]
        testpath = chplpath.lstrip('chapel/')
        hyperlink = 'https://github.com/chapel-lang/chapel/blob/{0}/{1}'.format(link, testpath)
        rstlink = '`View {0} on GitHub <{1}>`_'.format(filename, hyperlink)
        output.append(rstlink)
        output.append('')

    return '\n'.join(output)


def gen_rst(handle):
    """Convert contents of file handle to restructured text, using the rules
    described in the module doc string (__doc__)"""

    output = []
    commentdepth = 0
    state = ''
    indentation = -1

    # Each line is rst or code-block
    for line in [l.strip('\n') for l in handle.readlines()]:

        # Skip empty lines
        if len(line.strip()) == 0:
            output.append('')
            continue

        # Comment canaries - note: we don't support escaped comments: \/*
        commentstarts = line.count('/*')
        commentends = line.count('*/')
        commentdepth += commentstarts - commentends

        # State tracking
        laststate = state
        state = ''

        # Identification of line
        if commentdepth > 0 or commentends > 0:
            state = 'blockcomment'
        elif line.startswith('//'):
            state = 'linecomment'
        elif 'code' in laststate:
            state = 'code'
        else:
            state = 'codeblock'

        if 'comment' in state:

            if 'comment' not in laststate:
                output.append('')

            rstline = line
            if state == 'linecomment':
                # Strip white space for line comments
                rstline = rstline.replace('//', '  ')
                rstline = rstline.strip()
            else:
                # Preserve white space for block comments
                if commentstarts:
                    rstline = rstline.replace('/*', '  ')
                if commentends > 0:
                    rstline = rstline.replace('*/', '  ')
                if '.. code-block::' in rstline or len(rstline.strip()) == 0:
                    rstline = rstline.strip()

            # Strip indentation
            if indentation == -1:
                lineindentation = len(rstline) - len(rstline.lstrip(' '))
                if lineindentation > 0:
                    indentation = lineindentation
                    rstline = rstline.lstrip(' ')
            else:
                if rstline.startswith(' '*indentation):
                    rstline = rstline[indentation:]
                else:
                    rstline = rstline.lstrip(' ')

            output.append(rstline)
        else:
            # Reset indentation
            indentation = -1

            # Write code block
            if state == 'codeblock':
                output.append('')
                output.append('.. code-block:: chapel')
                output.append('')
            codeline = ''.join(['    ', line])
            output.append(codeline)

    return '\n'.join(output)


def gen_codeblock(handle):
    """Wrap contents of file handle with 'code-block:: chapel' and return the
    resulting contents as a string"""

    output = []
    output.append('')
    output.append('.. code-block:: chapel')
    output.append('')

    for line in [l.strip('\n') for l in handle.readlines()]:
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

    with open(output, 'w') as handle:
        handle.write(rstoutput)


def main(**kwargs):
    """Driver function - convert each file to rst and write to output"""

    # Parse keyword arguments
    chapelfiles = kwargs['chapelfiles']
    output = kwargs['output']
    prefix = kwargs['prefix']
    codeblock = kwargs['codeblock']
    link = kwargs['link']
    verbose = kwargs['verbose']

    for chapelfile in chapelfiles:

        preamble = gen_preamble(chapelfile, link=link)

        with open(chapelfile, 'r') as handle:
            if codeblock:
                rstoutput = gen_codeblock(handle)
            else:
                rstoutput = gen_rst(handle)

        rstoutput = '\n'.join([preamble, rstoutput])

        fname = getfname(chapelfile, output, prefix)

        if verbose:
            print('writing output of {0} to {1}'.format(chapelfile, fname))

        write(rstoutput, fname)

if __name__ == '__main__':
    # Parse arguments and cast them into a dictionary
    arguments = vars(get_arguments())
    main(**arguments)
