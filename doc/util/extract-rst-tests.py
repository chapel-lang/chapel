#!/usr/bin/env python
# encoding: utf-8

"""
extract rst tests
"""

from __future__ import print_function

import os
import sys
import stat

import argparse
import re

capture_space = re.compile(r"(\s*)\S.*")
capture_chapter = re.compile(r"\s*.. _Chapter-([a-zA-Z0-9._-]+):\s*")
capture_testname = re.compile(r"\s*[*]?Example\s+\(([a-zA-Z0-9._-]+)\).*")
match_code = re.compile(r"\s*.. code-block:: chapel\s*|" +
                        r"\s*.. BLOCK-test-chapelnoprint\s*|" +
                        r"\s*.. BLOCK-test-chapelpost\s*|" +
                        r"\s*.. BLOCK-test-chapelpre\s*")
match_outputname = re.compile(r"\s*.. BLOCK-test-chapeloutputname\s*")
match_output = re.compile(r"\s*.. BLOCK-test-chapeloutput\s*|" +
                          r"\s*.. .. code-block:: printoutput")
match_compopts = re.compile(r"\s*.. BLOCK-test-chapelcompopts\s*")
match_execopts = re.compile(r"\s*.. BLOCK-test-chapelexecopts\s*")
match_prediff = re.compile(r"\s*.. BLOCK-test-chapelprediff\s*")
match_function = re.compile(r"\s*.. function::\s*")
total_tests = 0

def get_arguments():
    """
    Get arguments from command line
    """
    parser = argparse.ArgumentParser(
        prog='extract-rst-tests',
        usage='%(prog)s  file.chpl [options] ',
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument('rstfiles', nargs='+',
                        help='gather tests from these RST files')
    parser.add_argument('--output', default='test/release/examples/spec',
                        help='destination directory of output')
    parser.add_argument('--verbose', '-v', action='store_true', default=False,
                        help='verbosity')
    return parser.parse_args()


def save_to(outdir, chapter, name, data, isexe=False):
    path = outdir + "/" + chapter + "/" + name
    if not os.path.exists(os.path.dirname(path)):
        print('Adding ', os.path.dirname(path))
        os.makedirs(os.path.dirname(path))

    #print('Writing to: ', path)
    with open(path, 'w') as handle:
        handle.write(data)

    if isexe:
        os.chmod(path, stat.S_IRWXU | stat.S_IRGRP)

def read_block(lines, start):
    indent = -1
    indend_text = ""
    first_nonempty = -1
    j = start
    end = len(lines)

    # compute where to stop reading
    while j < len(lines):
        if lines[j] != "" and lines[j] != "\n":
            match = re.match(capture_space, lines[j])
            if match:
                cur = len(match.group(1))
                #print("Line ", j+1, " has indent ", cur)
                if indent == -1:
                    first_nonempty = j
                    indent = cur
                    indent_text = lines[j][:indent]
                if cur < indent:
                    end = j
                    break
        j += 1

    #print("indent_text: ", indent_text)

    # read the block
    block = ""
    j = first_nonempty
    while j < end and j < len(lines):
        # remove indent from the lines
        useline = lines[j]
        if useline.startswith(indent_text):
            useline = useline.replace(indent_text, "", 1)
        block += useline
        j += 1

    # remove multiple trailing newlines
    block = block.rstrip("\n\r")
    block += "\n"

    #print("read_block returning ", (start+1, end+1))
    #print(block)
    return (end, block)

def extract_tests(rstfile, outdir):
    print("Processing .rst file: ", rstfile)

    lines = ( )
    with open(rstfile, 'r') as handle:
        lines = handle.readlines()

    chapter = ""
    testname = ""
    
    i = 0
    while i < len(lines):
        #if i % 100 == 0:
        #    print("Processing line: ", i)

        line = lines[i]
        i += 1

        # Find lines like
        # .. _Chapter-Scope:
        # to get the chapter name
        header = ".. _Chapter-"
        match = re.match(capture_chapter, line)
        if match:
            chapter = match.group(1)

        # Find lines like
        #     *Example (enum.chpl)*.
        # to get the test name
        match = re.match(capture_testname, line)
        if match:
            testname = match.group(1)
            testname = testname.replace(".chpl", "")
            chpl = ""
            good = ""
            goodname = ""
            execopts = ""
            compopts = ""
            prediff = ""

            example_start = i-1
            example_end = -1
            (example_end, block) = read_block(lines, example_start)
            
            #print("Processing example: ", chapter + "/" + testname,
            #       (example_start+1, example_end+1))

            # Process the lines in the example
            i = example_start + 1
            while i < example_end and i < len(lines):
                line = lines[i]
                i += 1

                # Find lines like
                #     *Example (enum.chpl)*.
                if re.match(capture_testname, line):
                    # stop this loop if another example is encountered
                    #print ("Breaking for Example match on line ", i)
                    i -= 1
                    break

                # Find lines like
                #     *.. function:: proc foo int
                if re.match(match_function, line):
                    # stop this loop if a function block is encounterred
                    #print ("Breaking for Example match on line ", i)
                    i -= 1
                    break

                # Find lines like
                #   .. BLOCK-test-chapeloutputname
                elif re.match(match_outputname, line):
                    (i, block) = read_block(lines, i)
                    if block != "":
                        # First, save any old .good
                        if goodname == "":
                            goodname = testname + ".good"
                        save_to(outdir, chapter, goodname, good)
                        good = ""

                        # Then, update the .good name
                        goodname = block.strip()
                        #print ("Read output name", goodname)
 

                # Find lines like
                #   .. BLOCK-test-chapeloutput
                #   .. code-block:: printoutput
                elif re.match(match_output, line):
                    (i, block) = read_block(lines, i)
                    if block != "":
                        good += block

                # Find lines like
                #   .. BLOCK-test-chapelcompopts
                elif re.match(match_compopts, line):
                    (i, block) = read_block(lines, i)
                    if block != "":
                        compopts += block

                # Find lines like
                #   .. BLOCK-test-chapelexecopts
                elif re.match(match_execopts, line):
                    (i, block) = read_block(lines, i)
                    if block != "":
                        execopts += block


                # Find lines like
                #   .. BLOCK-test-chapelprediff
                elif re.match(match_prediff, line):
                    (i, block) = read_block(lines, i)
                    if block != "":
                        prediff += block

                # Find lines like
                #   .. code-block:: chapel
                #   .. BLOCK-test-chapelnoprint
                #   .. BLOCK-test-chapelpost
                #   .. BLOCK-test-chapelpre
                elif re.match(match_code, line):
                    (i, block) = read_block(lines, i)
                    # Filter out request for line numbers
                    if block.startswith(":linenos:"):
                        block = block.replace(":linenos:", "", 1)

                    chpl += block
                    #print ("Adding to chpl ", block)


            # Now save the data gathered
            if goodname == "":
                goodname = testname + ".good"

            # Now save the data
            save_to(outdir, chapter, testname + ".chpl", chpl)
            save_to(outdir, chapter, goodname, good)
            if execopts != "":
                save_to(outdir, chapter, testname + ".execopts", execopts)
            if compopts != "":
                save_to(outdir, chapter, testname + ".compopts", compopts)
            if prediff != "":
                isexe = prediff.startswith("#!")
                save_to(outdir, chapter, testname + ".prediff", prediff,
                        isexe)

            global total_tests
            total_tests += 1


def main(**kwargs):
    """Driver function"""

    # Parse keyword arguments
    rstfiles = kwargs['rstfiles']
    output = kwargs['output']
    verbose = kwargs['verbose']

    for rstfile in rstfiles:
        if os.path.isfile(rstfile):
            extract_tests(rstfile, output)
        elif os.path.isdir(rstfile):
          for dirpath, dirnames, filenames in os.walk(rstfile):
              for f in filenames:
                  if f.endswith('.rst'):
                      extract_tests(os.path.join(dirpath, f), output)
        else:
            print("File not found ", rstfile)

    global total_tests
    print("DONE - created", total_tests, "tests")

if __name__ == '__main__':
    # Parse arguments and cast them into a dictionary
    arguments = vars(get_arguments())
    main(**arguments)
