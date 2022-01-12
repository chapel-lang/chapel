#!/usr/bin/env python
# encoding: utf-8

"""
parse_results -- script for parsing results from known
    logfile types to junit xml format

@author:     jswaro
@copyright:  Copyright 2018 Cray Inc. All Rights Reserved.
@deffield    updated: 2018-09-26
"""

from junit_xml import TestSuite, TestCase
import sys
import os
from functools import wraps
from argparse import ArgumentParser, RawDescriptionHelpFormatter

__all__ = []
__version__ = 1.0
__date__ = '2018-09-26'
__updated__ = '2018-09-26'

VERSION = '1.0.0'
DEBUG = False

known_formats = dict()

def generate_generic_test_case(name, classname, time, message, result):
    default_pass_message = 'Test passed, check build log for additional details'
    default_skip_message = 'Test skipped, check build log for additional details'
    tc = TestCase(name, classname, time,
                  default_pass_message if result == 'pass' else '')
    if result == 'fail':
        tc.add_failure_info(message=message)
    if result == 'skip':
        tc.add_skipped_info(message=default_skip_message)

    return tc

def tap_delimiter(line):
    return line.startswith('ok') or line.startswith('not ok')

def tap_parser(log, classname_prefix):
    name = ""
    testname = ""
    classname = ""
    result = ""
    time = 0.0
    message = ""
    for line in log:
        data = line.strip().split()
        if tap_delimiter(line):
            result = 'pass'
            if data[0] == 'ok' and '# skip' in line:
                result = 'skip'
            elif data[0] == 'not':
                result = 'fail'
            name_index = 3 if result == 'fail' else 2
            testname = " ".join(data[name_index:]) if result != 'skip' else " ".join(data[name_index:data.index('#')])
            classname = classname_prefix
        elif line.startswith('#'):
            message += " ".join(data) + '\n'

    tc = generate_generic_test_case(testname, classname, 1.00, message, result)
    return tc

def fabtests_test_delimiter(line):
    return line.startswith('- name: ')

def fabtests_testcase_parser(log, classname_prefix):
    name = ""
    testname = ""
    classname = ""
    result = ""
    time = ""
    message = ""
    for line in log:
        data = line.strip().split()
        if fabtests_test_delimiter(line):
            name = " ".join(data[2:])
            testname = data[2]
            classname = "{0}.{1}".format(classname_prefix, testname)
        elif line.startswith('  result:'):
            if data[1] == 'Pass':
                result = 'pass'
            elif data[1] == 'Notrun':
                result = 'skip'
            elif data[1] == 'Excluded':
                result = 'skip'
            else:
                result = 'fail'
        elif line.startswith('  time:'):
            time = float(data[1])
        else: #recording stdout/stderr
            message += " ".join(data) + '\n'

    tc = generate_generic_test_case(name, classname, time, message, result)
    return tc

def parse(infile, outfile, format_type, classname, suitename):
    testcases = list()
    testcase_logs = list()
    current = None
    test_block_delimiter = known_formats[format_type]['tb_delimiter']

    # separate log file into test blocks by test block delimiter
    for line in infile:
        if test_block_delimiter(line):
            if current: # non-empty list
                testcase_logs.append(current)
            current = list()
        if current is not None:
            current.append(line)

    # add last record if present
    if current not in testcase_logs:
        testcase_logs.append(current)

    # create test cases from test blocks
    for entry in testcase_logs:
        testcases.append(known_formats[format_type]['test_parser'](entry, classname))

    # generate test suite result using provided test cases
    test_suite = TestSuite(suitename, testcases)

    # get rid of unnecessary 'disabled' strings in formatted xml string
    s = TestSuite.to_xml_string([test_suite])
    s = s.replace(' disabled=\"0\"', '')

    # write xml to outfile
    outfile.write(s)

def register_parser(label, func):
    global known_formats
    known_formats[label] = func


def main(argv=None):  #IGNORE:C0111
    if argv is None:
        argv = sys.argv
    else:
        sys.argv.extend(argv)
    register_parser('fabtests',
                    {'tb_delimiter': fabtests_test_delimiter,
                     'test_parser': fabtests_testcase_parser})
    register_parser('tap',
                    {'tb_delimiter': tap_delimiter,
                     'test_parser': tap_parser})

    program_name = os.path.basename(sys.argv[0])
    program_version = "v%s" % (VERSION)
    program_build_date = str(__updated__)
    program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
    program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
    program_license = '''%s

  Copyright 2018 Cray Inc. All rights reserved.

  Distributed on an "AS IS" basis without warranties
  or conditions of any kind, either express or implied.

USAGE
''' % program_shortdesc
    # Setup argument parser
    parser = ArgumentParser(description=program_license,
                            formatter_class=RawDescriptionHelpFormatter)

    parser.add_argument("-V", "--version", action="version", version=program_version_message)
    parser.add_argument("-r", "--infile", action="store", default=None, dest='infile',
                        help="file to read in, defaults to stdin")
    parser.add_argument("-w", "--outfile", action="store", default=None, dest='outfile',
                        help="junit xml file to write out, defaults to stdout")
    parser.add_argument("format",
                        help='test format to expect (available: {0})'.format(", ".join(known_formats.keys())))
    parser.add_argument("classname",
                        help='class name to associate with generated junit xml tests')
    parser.add_argument("suitename",
                        help='suite name to associate with generated junit xml tests')
    # Process arguments
    args = parser.parse_args()

    infile = sys.stdin
    outfile = sys.stdout
    if args.infile is not None:
        infile = open(args.infile, 'r')
    if args.outfile is not None:
        outfile = open(args.outfile, 'w')

    parse(infile, outfile, args.format, args.classname, args.suitename)

    if args.infile is not None:
        infile.close()
    if args.outfile is not None:
        outfile.close()

if __name__ == "__main__":
    if DEBUG:
        sys.argv.append("-h")
        sys.argv.append("-v")

    sys.exit(main())
