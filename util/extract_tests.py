#! /usr/bin/env python
#
# Extract test programs from the Chapel spec
#
# All tests are place in a subdirectory of the output directory
# (default '.') named after the LaTeX file.  Multiple LaTex files may
# be specified on the command line.
#
# See spec/chapel_testing.tex and the LaTex source for the spec itself
#  for more info.
#
# SUPPORTED ENVIROMENTS:
# - chapelpre (REQUIRED)
#   If a filename is not given on the first line, the filename is
#   generated from the input filename and the line number.
#
# - chapel|chapelcode
# - chapelpost
# - chapelfuture
# - chapelcompopts
# - chapelexecopts
# - chapeloutput|chapelprintoutput (REQUIRED)
#
# GENERAL CAVEATS:
# - All environments may contain special formatting due the fact that
#   everything must be able to be parsed by LaTeX.  Currently, the
#   following substitutions are made:
#
#      '//' becomes '/'
#      '/#' becomes '#'
#      '/_' becomes '_'
#
#   If this becomes too unwieldy, we'll probably have to find a better
#   way to do it.

#
# - The 'chapelprintoutput' enviroment is special since the output is
#   also in the spec document itself and must contain LaTeX formatting.
#   The formatting must be removed for the .good file.   Currently the
#   only additional LaTex formatting that is removed from the output is
#   new line ('//').


import os, sys

#
# Helper functions
#

# Generate the test name prefix
def GenTestName(lineno):
    infileSplit = curinfile.rsplit('/', 1)[1]
    newof = infileSplit+'.'+str(lineno)
    curTestNames.append(newof)
    return outdir+'/'+curdir+'/'+newof

# Extract the test name from a source line
def GetTestName(line):
    outfile = line.strip().strip('%').strip().rstrip('.chpl')
    if options.debug:
        print outfile
    curTestNames.append(outfile)
    if curTestNames.count(outfile) > 1:
        sys.stdout.write("Warning: Test name '%s' already specified\n"%
                         (outfile));
        newof = outfile + "." + str(curTestNames.count(outfile))
        curTestNames.append(newof)
        sys.stdout.write("         Renaming it to %s\n"%(newof));
        outfile = newof
    return outdir+'/'+curdir+'/'+outfile

# Helper functions for de-LaTeX-ifying output line
def RemoveLaTeXFormatting(line, nlrepl):
    dline = line.replace('\\#', '#')
    dline = dline.replace('\\_', '_')
    dline = dline.replace('\\\\', nlrepl)
    return dline

# Helper for writing test files
def WriteTestFile(fh, output):
    for line in output:
        if line[0] != '%':
            fh.write(RemoveLaTeXFormatting(line, '\\'))

# Helper for writing test .good files
def WriteGoodFile(fh, output, testName):
    goodTestName = testName+'.good'
    opened = False
    for line in output:
        if line[0] == '%':
            # new .good file
            if opened:
                fh.close()
            opened = True
            goodTestName = GetTestName(line)
            if options.debug:
                sys.stdout.write('Opening %s\n'%(goodTestName))
            fh = open(goodTestName, 'w')
        else:
            if not opened:
                if options.debug:
                    sys.stdout.write('Opening %s\n'%(goodTestName))
                fh = open(goodTestName, 'w')
                opened = True
            fh.write(RemoveLaTeXFormatting(line, ''))

    if opened:
        fh.close()


# Write out the test files
def OutputTest(lineno, seenChapelPre, seenChapelOutput):
    if seenChapelPre and seenChapelOutput:
        # writeout the actual test program
        if len(chapelpre) > 0 and chapelpre[0][0] == '%':
            testName = GetTestName(chapelpre[0])
        else:
            testName = GenTestName(lineno)
        if options.verbose:
            sys.stdout.write('Writing %s (line %d)\n'
                             %(testName.rsplit('/', 1)[1], lineno))

        fh = open(testName+'.chpl', 'w')
        if options.debug:
            sys.stdout.write('Opening %s\n'%(testName+'.chpl'))

        WriteTestFile(fh, chapelpre)
        WriteTestFile(fh, chapelcode)
        WriteTestFile(fh, chapelpost)

        fh.close()

        # write out the future file
        if len(chapelfuture) > 0:
            if options.debug:
                sys.stdout.write('Opening %s\n'%(testName+'.future'))
            fh = open(testName+'.future', 'w')
            WriteTestFile(fh, chapelfuture)

        # write out the .compopts file
        if len(chapelcompopts) > 0:
            if options.debug:
                sys.stdout.write('Opening %s\n'%(testName+'.compopts'))
            fh = open(testName+'.compopts', 'w')
            WriteTestFile(fh, chapelcompopts)

        # write out the .execopts files
        if len(chapelexecopts) > 0:
            if options.debug:
                sys.stdout.write('Opening %s\n'%(testName+'.execopts'))
            fh = open(testName+'.execopts', 'w')
            WriteTestFile(fh, chapelexecopts)

        # write out .good files
        if len(chapeloutput) > 0 or len(chapelprintoutput) > 0:
            WriteGoodFile(fh, chapeloutput, testName)
            WriteGoodFile(fh, chapelprintoutput, testName)
        else:
            # write an empty .good file
            fh = open(testName+'.good', 'w')
            fh.close()

    else:
    # invalid format or ignored test
        if lineno != 0:
            if options.verbose:
                sys.stdout.write('Ignoring test on line %d\n'%(lineno))

    seenChapelPre = False
    seenChapelOutput = False

    # clear lists
    del chapelpre[:]
    del chapelcode[:]
    del chapelpost[:]
    del chapelfuture[:]
    del chapelcompopts[:]
    del chapelexecopts[:]
    del chapeloutput[:]
    del chapelprintoutput[:]





####################
# extract_tests.py #
####################

#
# Initialize global vars
#
chapelpre = list()
chapelcode = list()
chapelpost = list()
chapelfuture = list()
chapelcompopts = list()
chapelexecopts = list()
chapeloutput = list()
chapelprintoutput = list()
current = None
curTestNames = list()

#
# Parse command line args
#
from optparse import OptionParser

parser = OptionParser("usage: %prog [options] file1.tex file2.tex ...")
parser.add_option('-o', '--outdir',
                  action='store', type='string', dest='outdir',
                  default='.', help="output directory (default '.')",
                  metavar='DIR')
parser.add_option('-q', '--quiet',
                  action='store_false', dest='verbose', default=True,
                  help="don't print status messages to stdout")
parser.add_option('-d', '--debug',
                  action='store_true', dest='debug', default=False)

(options, args) = parser.parse_args()

if len(args) < 1:
    parser.error("incorrect number of arguments")

#
# Create output directory
#
outdir = os.path.realpath(options.outdir)
if not os.path.isdir(outdir):
    os.mkdir(outdir)

for infile in args:
    #
    # Open the input file
    #
    realinfile = os.path.realpath(infile)
    fh = open(realinfile, 'r')
    myLines = fh.readlines()
    fh.close()

    # Clean up table of testname
    del curTestNames[:]

    curinfile = realinfile
    curdir = realinfile.rsplit('/', 1)[1]+'.tests'
    if not os.path.isdir(outdir+'/'+curdir):
        os.mkdir(outdir+'/'+curdir)
    if options.verbose:
        sys.stdout.write('***\n')
        sys.stdout.write('*** Generating tests from from %s\n'%(infile))
        sys.stdout.write('***\tOutput directory: %s/%s\n'%
                         (options.outdir,curdir))
        sys.stdout.write('***\n')

    lineno = 0
    testlineno = lineno
    printToOutfile = False
    seenChapelPre = False
    seenChapelOutput = False

    for line in myLines:
        lineno += 1
        pos = line.find('\\end{chapel')
        if pos > -1:
            pos += 11
            printToOutfile = False
            current = None
            continue

        if printToOutfile:
            current += [line]
            continue

        pos = line.find('\\begin{chapel')
        if pos > -1:
            pos += 13
            if options.debug:
                print line
            if (line.find('pre}', pos) == pos):
                OutputTest(testlineno, seenChapelPre, seenChapelOutput)
                testlineno = lineno
                printToOutfile = True
                seenChapelPre = True
                seenChapelOutput = False
                current = chapelpre;
                continue

            elif ((line.find('}', pos) == pos) or
                  (line.find('code}', pos) == pos)):
                if ((seenChapelPre and seenChapelOutput) or
                    not (seenChapelPre or seenChapelOutput)):
                    # output the previous test, but ignore this one
                    #  (no \begin{chapelpre} encountered)
                    OutputTest(testlineno, seenChapelPre, seenChapelOutput)
                    testlineno = lineno
                    seenChapelPre = False
                    seenChapelOutput = False
                else:
                    printToOutfile = True
                    current = chapelcode;

            elif line.find('post}', pos) == pos:
                printToOutfile = True
                current = chapelpost;

            elif line.find('future}', pos) == pos:
                printToOutfile = True
                current = chapelfuture;

            elif line.find('compopts}', pos) == pos:
                printToOutfile = True
                current = chapelcompopts;
            elif line.find('execopts}', pos) == pos:
                printToOutfile = True
                current = chapelexecopts;

            elif line.find('output}', pos) == pos:
                printToOutfile = True
                seenChapelOutput = True
                current = chapeloutput;
            elif line.find('printoutput}', pos) == pos:
                printToOutfile = True
                seenChapelOutput = True
                current = chapelprintoutput;

    OutputTest(testlineno, seenChapelPre, seenChapelOutput)
