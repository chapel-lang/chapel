#!/usr/bin/env python3
# encoding: utf-8

"""chpl2md converts a chapel program to an md file, where all comments are
rendered md, and all code is wrapped in code blocks.

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

example.md
===========

This is
  an example

```chapel
proc foo() {
    // this comment will be in the code block
    var bar = 1; }
```

this comment will be text"""

import os
import sys
from itertools import product
from literate_chapel import to_pieces, title_comment

import argparse

def get_arguments():
    """
    Get arguments from command line
    """
    parser = argparse.ArgumentParser(
        prog='chpl2md',
        usage='%(prog)s  file.chpl [options] ',
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument('chapelfiles', nargs='+',
                        help='Chapel files to convert to md')
    parser.add_argument('--output', default='md', choices=['stdout', 'md'],
                        help='destination of output')
    parser.add_argument('--prefix', default='.',
                        help='prefix path for output')
    parser.add_argument('--codeblock', '-c', action='store_true', default=False,
                        help='convert entire document into code block')
    parser.add_argument('--verbose', '-v', action='store_true', default=False,
                        help='verbosity')
    return parser.parse_args()


def print_good(output, good_options, chunk_index):
    """Print the output from .good files"""

    output.append('')
    if len(good_options) == 1:
        # Just one good option means no need to generate menu
        (basename, _, _), = good_options
        output.append('{{{{< goodChunk name="{0}" chunk="{1}">}}}}'
                        .format(basename, chunk_index))
    else:
        output.append('{{{{< goodMenu name="list_{0}" >}}}}'.format(chunk_index))
        for (basename, version, options) in good_options:
            output.append('  {{{{< goodOption chunk="{0}" file="{1}" cnfg="{2}" txt="{3}" >}}}}'
                          .format(chunk_index, basename, version, options))
        output.append('{{< /goodMenu >}}')

def gen_md(pieces, chapelfile):
    output = []
    good_options = list(goodfiles(chapelfile))

    # Each line is md or code-block
    for (kind, content) in pieces:
        if kind == 'title':
            output.append('---')
            output.append('title: "{0}"'.format(content[0]))
            output.append('draft: true')
            output.append('---')
        elif kind == 'prose':
            output.extend(content)
        elif kind == 'code':
            output.append('```Chapel')
            output.extend(content)
            output.append('```')
        elif kind == 'output':
            print_good(output, good_options, content)
        output.append('')
    return '\n'.join(output)

def variant_list(file):
    """Create a list of an compopts / execopts file's lines if it
    exists, and return None otherwise."""

    if not os.path.isfile(file): return None
    with open(file, 'r', encoding='utf-8') as handle:
        return [l.strip("\n") for l in handle]

def get_combinations(iterables):
    """Take iterables of compopts, execopts, etc and yield tuples of
    (file_modifier, option_tuple) corresponding to each combination of these
    options. File modifier is, e.g., "1-1" for 'first compile option, first
    exec option'."""

    if len(iterables) == 0:
        yield (None, ())
        return
    for combination in product(*[enumerate(it, 1) for it in iterables]):
        indices = []
        opts = []
        for (idx, opt) in combination:
            indices.append(str(idx))
            opts.append(opt)
        yield ('-'.join(indices), tuple(opts))

def goodfiles(chapelfile):
    """Yield (file_name, options_string) for .good files corresponding to
    each combination of compile and execution options."""

    filedir, filename = os.path.split(chapelfile)
    basename, _ = os.path.splitext(filename)
    compoptsFile = os.path.join(filedir, ''.join([basename, '.compopts']))
    execoptsFile = os.path.join(filedir, ''.join([basename, '.execopts']))

    iters = []
    compoptsList = variant_list(compoptsFile)
    if compoptsList is not None: iters.append(compoptsList)
    execoptsList = variant_list(execoptsFile)
    if execoptsList is not None: iters.append(execoptsList)

    for (name, opts) in get_combinations(iters):
        yield (basename, name, ' '.join(opts))

def getfname(chapelfile, output, prefix):
    """Compute filename for output"""
    if output == 'md':
        filename = os.path.split(chapelfile)[1]
        basename, _ = os.path.splitext(filename)
        mdname = ''.join([basename, '.md'])
        mdfile = os.path.join(prefix, mdname)
        print("Writing to", mdfile, os.path.split(chapelfile))
        return mdfile
    elif output == 'stdout':
        return None
    else:
        print('Error: output = {0} is invalid')
        sys.exit(1)


def write(mdoutput, output):
    """Write to output"""
    if output is None:
        sys.stdout.write(mdoutput)
        return

    with open(output, 'w', encoding='utf-8') as handle:
        handle.write(mdoutput)


def main_args(**kwargs):
    """Driver function - convert each file to md and write to output"""

    # Parse keyword arguments
    chapelfiles = kwargs['chapelfiles']
    output = kwargs['output']
    prefix = kwargs['prefix']
    verbose = kwargs['verbose']

    for chapelfile in chapelfiles:
        islearnChapelInYMinutes = (os.path.basename(chapelfile)=='learnChapelInYMinutes.chpl')

        with open(chapelfile, 'r', encoding='utf-8') as handle:
            pieces = to_pieces(handle, islearnChapelInYMinutes)
            mdoutput = gen_md(pieces, chapelfile)

        fname = getfname(chapelfile, output, prefix)

        if not os.path.exists(prefix) and output != 'stdout':
            print('Creating directory: ', prefix)
            os.makedirs(prefix)

        if verbose:
            print('writing output of {0} to {1}'.format(chapelfile, fname))

        write(mdoutput, fname)

def main():
    # Parse arguments and cast them into a dictionary
    arguments = vars(get_arguments())
    main_args(**arguments)

if __name__ == '__main__':
    main()
