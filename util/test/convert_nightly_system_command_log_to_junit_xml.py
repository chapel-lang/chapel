#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import xml.etree.ElementTree as XML

parser = argparse.ArgumentParser(description='Convert nightly system command logs to JUnit XML.')
parser.add_argument('cleanlog', type=str, help='Log listing all commands that were executed')
parser.add_argument('errorlog', type=str, help='Log listing errors in commands that were executed')
parser.add_argument('output', type=str, help='Output JUnit XML file')

args = parser.parse_args()

with open(args.cleanlog, 'r') as f:
    cleanlog = f.readlines()

with open(args.errorlog, 'r') as f:
    errorlog = f.readlines()

test_suites = XML.Element('testsuites')
test_suite = XML.SubElement(test_suites, 'testsuite')

# Error lines are empty when no error occurred.
num_errors = 0
for (cleanline, errline) in zip(cleanlog, errorlog):
    cleanline = cleanline.strip()
    errline = errline.strip()

    test_case = XML.SubElement(test_suite, 'testcase')
    test_case.set('name', cleanline)
    test_case.set('classname', 'nightly system command')
    test_case.set('time', '0')

    if errline:
        failure = XML.SubElement(test_case, 'failure')
        failure.set('message', errline)
        num_errors += 1

test_suite.set('name', 'nightly system commands')
test_suite.set('errors', str(num_errors))
test_suite.set('failures', str(num_errors))
test_suite.set('tests', str(len(cleanlog)))

tree = XML.ElementTree(test_suites)
tree.write(args.output)
