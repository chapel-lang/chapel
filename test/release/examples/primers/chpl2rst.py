#!/usr/bin/env python
# encoding: utf-8

"""
Convert Chapel file into restructured text

TODO -- clean this up
"""

from __future__ import print_function

import os
import sys
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter



def get_arguments():
    """
    Get arguments from command line
    """
    parser = ArgumentParser(prog='chpl2rst',
                            usage='%(prog)s  foo.chpl [options] ',
                            description=''' %(prog)s converts a chapel program
                            to an rst file, where all comments are rendered rst,
                            and all code is wrapped in code blocks''',
                            formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument('chapelfiles', nargs='+', help='Chapel files')
    parser.add_argument('--output', default='rst', choices=['stdout', 'rst'],
                        help='Direct output of conversion')
    parser.add_argument('--prefix', default='.', help='prefix path for output')
    return parser.parse_args()


def chpl2rst(chapelfile):
    """Convert Chapel program to restructured text"""

    # Title
    output = []
    output.append(chapelfile)
    output.append('='*len(chapelfile))

    with open(chapelfile, 'r') as handle:
        commentdepth = 0
        codeblock = False
        # Each line is rst or code-block
        for line in [l.strip('\n') for l in handle.readlines()]:
            if len(line.strip()) == 0:
                output.append('')
                continue

            # Detection of comment
            # TODO -- support escaped characters (got to make mppf proud)
            commentstarts = line.count('/*')
            commentends = line.count('*/')
            commentdepth += commentstarts - commentends

            # Identification of comment type
            blockcomment = commentdepth > 0 or commentends > 0
            # Comments that don't begin on the first character aren't rendered
            linecomment = line.startswith('//')
            comment = blockcomment or linecomment

            if comment:
                codeblock = False
                rstline = line
                if linecomment:
                    # Strip white space for line comments
                    rstline = rstline.replace('//', '')
                    rstline = rstline.strip()
                    output.append('')
                else:
                    # Preserve white space for block comments
                    if commentstarts:
                        rstline = rstline.replace('/*', '')
                    if commentends > 0:
                        rstline = rstline.replace('*/', '')
                    if '.. code-block::' in rstline:
                        rstline = rstline.strip()

                output.append(rstline)
            else:
                # Write code block
                if not codeblock:
                    output.append('')
                    output.append('.. code-block:: chapel')
                    output.append('')
                    codeblock = True
                codeline = ''.join(['    ', line])
                output.append(codeline)

    return '\n'.join(output)


def getfname(chapelfile, output, prefix):
    """Compute filename for output"""
    if output == 'rst':
        basename, _ = os.path.splitext(chapelfile)
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


def main(chapelfiles, output='rst', prefix='.'):
    """Driver function"""
    for chapelfile in chapelfiles:
        rstoutput = chpl2rst(chapelfile)
        fname = getfname(chapelfile, output, prefix)
        write(rstoutput, fname)


if __name__ == '__main__':
    args = get_arguments()
    main(args.chapelfiles, args.output, args.prefix)
