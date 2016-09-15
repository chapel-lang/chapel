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
    return parser.parse_args()


def chpl2rst(chapelfile):
    """Convert Chapel program to restructured text, using the rules described
    in the module doc string"""

    filename = os.path.split(chapelfile)[1]
    basename, _ = os.path.splitext(filename)
    # Title
    output = []
    output.append(basename)
    output.append('='*len(basename))

    with open(chapelfile, 'r') as handle:
        commentdepth = 0
        state = ''
        indentation = -1

        # Each line is rst or code-block
        for line in [l.strip('\n') for l in handle.readlines()]:

            # Skip empty lines
            if len(line.strip()) == 0:
                output.append('')
                continue

            # Comment canaries
            # TODO -- support escaped characters (to make mppf proud)
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


def chpl2codeblock(chapelfile):
    """Wrap contents of chapelfile with code-block:: chapel and return the
    resulting contents as a string"""

    filename = os.path.split(chapelfile)[1]
    basename, _ = os.path.splitext(filename)
    # Title
    output = []
    output.append(basename)
    output.append('='*len(basename))
    output.append('')
    output.append('.. code-block:: chapel')
    output.append('')

    with open(chapelfile, 'r') as handle:
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


def main(chapelfiles, output='rst', prefix='.', codeblock=False, verbose=False):
    """Driver function - convert each file to rst and write to output"""
    for chapelfile in chapelfiles:

        if codeblock:
            rstoutput = chpl2codeblock(chapelfile)
        else:
            rstoutput = chpl2rst(chapelfile)

        fname = getfname(chapelfile, output, prefix)
        if verbose:
            print('writing output of {0} to {1}'.format(chapelfile, fname))
        write(rstoutput, fname)

if __name__ == '__main__':
    ARGS = get_arguments()
    main(ARGS.chapelfiles, ARGS.output, ARGS.prefix, ARGS.codeblock, ARGS.verbose)
