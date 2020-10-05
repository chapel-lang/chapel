#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Convert start_test log to jUnit XML report."""

from __future__ import print_function, unicode_literals

import codecs
import getpass
import logging
import optparse
import os.path
import re
import sys
import xml.etree.ElementTree as XML

# Add the chplenv dir to the python path.
chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform


DEBUG = False


def main():
    """Parse cli arguments and convert a start_test log file to jUnit xml
    format.
    """
    args = _parse_args()
    _setup_logging(args.verbose)

    global DEBUG
    if args.debug:
        DEBUG = True

    test_cases = _parse_start_test_log(args.start_test_log)
    _remove_prefixes(test_cases, args.remove_prefix)
    _create_junit_report(test_cases, args.junit_xml)


def _create_junit_report(test_cases, junit_file):
    """Create jUnit XML report from test info.

    :type test_cases: list of dicts
    :arg test_cases: list of dicts; each dict contains info about a single test case

    :type junit_file: str
    :arg junit_file: filename to write the jUnit XML report
    """
    logging.debug('Creating jUnit XML report at: {0}'.format(junit_file))
    test_suite = XML.Element('testsuite')

    for test_case in test_cases:
        case_elem = XML.SubElement(test_suite, 'testcase')
        case_elem.set('name', test_case['name'])
        case_elem.set('classname', test_case['classname'])
        case_elem.set('time', str(test_case['time']))

        test_error = test_case['error']

        if test_case['skipped']:
            skip_elem = XML.SubElement(case_elem, 'skipped')
        elif test_error is not None:
            error_elem = XML.SubElement(case_elem, 'error')
            error_elem.set('message', test_error['message'])
            error_elem.text = test_error['content']

        system_out = XML.SubElement(case_elem, 'system-out')
        system_out.text = test_case['system-out']

    encoding = "unicode" if sys.version_info[0] >= 3 else "us-ascii"
    xml_content = XML.tostring(test_suite, encoding=encoding)
    xml_content = _clean_xml(xml_content)
    with open(junit_file, 'w') as fp:
        fp.write(xml_content)
    logging.info('Wrote jUnit report to: {0}'.format(junit_file))


def _parse_start_test_log(start_test_log):
    """Parse start_test logfile and return results in python data structure.

    :type start_test_log: str
    :arg start_test_log: start_test log filename

    :rtype: list of dicts
    :returns: list of dicts; each dict contains info about a single test case
    """
    logging.debug('Parsing start_test log: {0}'.format(start_test_log))
    with codecs.open(start_test_log, 'r', encoding='utf-8', errors='ignore') as fp:
        start_test_lines = fp.readlines()
    logging.debug('Read {0} lines from "{1}".'.format(
        len(start_test_lines), start_test_log))

    test_cases = []
    while len(start_test_lines) > 0:
        subtest_start, subtest_end = _get_block(
            start_test_lines, '[Starting subtest - ', '[Finished subtest ')

        # No more sub_tests; delete the remaining lines and finish up.
        if subtest_start == -1:
            del start_test_lines[:]
            continue

        # Copy subtest lines into new list for further processing and delete
        # them from start_test_lines.
        sub_test_lines = start_test_lines[subtest_start:subtest_end+1:1]
        del start_test_lines[:subtest_end+1]

        while len(sub_test_lines) > 0:
            test_start, test_end = _get_block(
                sub_test_lines,
                '[test: ',
                '[Elapsed time to compile and execute all versions of "')
            test_start_skip, test_end_skip = _get_block(
                sub_test_lines,
                '[test: ',
                '[Skipping')

            test_skipped = False
            if test_end_skip != -1 and (test_end == -1 or test_end_skip < test_end):
                test_start, test_end = test_start_skip, test_end_skip
                test_skipped = True

                noperf_start, noperf_end = _get_block(
                    sub_test_lines,
                    '[test: ',
                    '[Skipping noperf test:')

                # If the test was skipped because it did not have performance
                # configuration files, drop the lines and continue. We don't
                # care about these for performance tests (as opposed to real
                # perf tests that are skipped due to environment/etc).
                if noperf_end != -1:
                    del sub_test_lines[noperf_start:noperf_end+1]
                    continue

            # If test_end is still -1 (i.e. not found), look for end of subtest
            # call (usually means subtest failed and did not tests).
            if test_start != -1 and test_end == -1:
                test_start, test_end = _get_block(
                    sub_test_lines,
                    '[test: ',
                    '[Finished subtest "')
                if test_end == -1:
                    raise ValueError('Failed to parse test case from: {0}'.format(
                        sub_test_lines))

            # No more test cases; delete remaining lines and finish up.
            if test_start == -1:
                del sub_test_lines[:]
                continue

            # Copy test lines into new list for further processing and delete
            # from sub_test_lines.
            test_case_lines = sub_test_lines[test_start:test_end+1:1]
            del sub_test_lines[:test_end+1]

            # Extract test name from "[test: <path to .chpl file>]" line.
            classname, test_name = _get_test_name(test_case_lines)
            if test_skipped:
                test_time = 0.0
                error = None
            else:
                test_time = _get_test_time(test_case_lines)
                error = _get_test_error(test_case_lines)
            test_content = ''.join(test_case_lines)

            test_case = {
                'name': test_name,
                'classname': classname,
                'time': test_time,
                'error': error,
                'skipped': test_skipped,
                'system-out': test_content,
            }

            test_cases.append(test_case)

    logging.info('Parsed {0} test cases from "{1}".'.format(
        len(test_cases), start_test_log))
    return test_cases


def _remove_prefixes(test_cases, prefix):
    """Remove the prefix from class names in all test cases. This is helpful for
    removing the $CHPL_HOME/test prefixes when sub_test used absolute file
    paths, instead of relative paths, in the logs. If a class name does not
    have the prefix, it is not changed.

    :type test_cases: list of dicts
    :arg test_cases: list of dicts; each dict contains info about a single test case

    :type prefix: str
    :arg prefix: prefix to remove from all class names
    """
    if prefix is None:
        return

    logging.debug('Removing prefix "{0}" from {1} test cases.'.format(
        prefix, len(test_cases)))

    # Remove the prefix, including a trailing slash.
    prefix_len = len(prefix.rstrip('/')) + 1
    def remove_prefix(class_name):
        if class_name.startswith(prefix):
            return class_name[prefix_len:]
        else:
            return class_name

    for i, test_case in enumerate(test_cases):
        classname = test_case['classname']
        updated_classname = remove_prefix(classname)
        test_cases[i]['classname'] = updated_classname


def _find_line(lines, prefix):
    """Find a line that starts with prefix in lines list.

    :type lines: list
    :arg lines: list of strings

    :type prefix: str
    :arg prefix: search prefix

    :rtype: int
    :returns: index of string that starts with 'prefix'; -1 if not found
    """
    for i, line in enumerate(lines):
        if line.startswith(prefix):
            return i
    return -1


def _get_block(lines, start_prefix, end_prefix):
    """Find the lines in the list between a given start point and end point.

    :type lines: list
    :arg lines: list of lines from start_test logfile.

    :rtype: (int, int)
    :returns: tuple for range of line numbers
    """
    start = _find_line(lines, start_prefix)

    # No more subtests in log file.
    if start == -1:
        return -1, -1

    end = _find_line(lines[start:], end_prefix)
    if end == -1:
        return start, end
    else:
        return start, start + end


def _get_test_name(test_case_lines):
    """Return test name and classname from single test case lines. Extract from
    "[test: <path to .chpl file>]" line. Use the directory path as the
    classname and the filename, without extension, as the test name. For
    example, in the example below, "release/examples" would be the classname
    and "hello" would be the test name.

    :type test_case_lines: list
    :arg test_case_lines: lines for a single test case from start_test logfile

    :rtype: (str, str)
    :returns: tuple with classname and test name
    """
    pattern = re.compile('\[test: (?P<test_name>[^\]]+)\]')
    match = pattern.search(test_case_lines[0])
    if match is None:
        raise ValueError('Could not find test name in: {0}'.format(
            test_case_lines[0].strip()))

    test_file = match.group('test_name')
    classname = os.path.dirname(test_file)
    base_file = os.path.basename(test_file)
    test_name, _ = os.path.splitext(base_file)
    return classname, test_name


def _get_test_time(test_case_lines):

    """Return the total compile and execution time in seconds for single test
    case. Finds the "[Elapsed time to compile and execute all versions of ..." line
    and extracts the time from it.

    :type test_case_lines: list
    :arg test_case_lines: lines for a single test case from start_test logfile

    :rtype: float
    :returns: time to run test
    """
    time_line_idx = _find_line(
        test_case_lines,
        '[Elapsed time to compile and execute all versions of "')
    if time_line_idx == -1:
        msg = 'Could not find elapsed time line in: {0}'.format(
            test_case_lines)
        logging.warn(msg)
        if DEBUG:
            raise ValueError(msg)
    time_line = test_case_lines[time_line_idx]

    pattern = re.compile(' - (?P<time>-?\d+\.\d+) seconds\]$')
    match = pattern.search(time_line)
    if match:
        time = match.group('time')
    else:
        time = '0.0'
        msg = 'Could not find time in: {0}'.format(time_line)
        logging.warn(msg)
        if DEBUG:
            raise ValueError(msg)
    return max(float(time),0.0)


def _get_test_error(test_case_lines):
    """Find errors and warnings. If any exist, return error dict with message and
    content. If no errors or warnings, return None.

    :type test_case_lines: list
    :arg test_case_lines: lines for a single test case from start_test logfile

    :rtype: dict
    :returns: error dict with 'message' and 'content' keys, None if no errors
    """
    error_pattern = re.compile('^\[Error.*$', re.IGNORECASE | re.MULTILINE)
    warn_pattern = re.compile('^\[Warning.*$', re.IGNORECASE | re.MULTILINE)

    whole_test = ''.join(test_case_lines)
    errors = error_pattern.findall(whole_test)
    warnings = warn_pattern.findall(whole_test)

    # No errors or warnings!
    if not (errors or warnings):
        return None

    # Use the first error or warning line as the message. The full test output
    # will be included in system-out.
    if errors:
        message = errors[0].strip().strip('[]')
    else:
        message = warnings[0].strip().strip('[]')
    content = '\n'.join(errors) + '\n'.join(warnings)

    return {
        'message': message,
        'content': content,
    }


def _clean_xml(xml_string):
    """Replace invalid characters with "?".

    :type xml_string: str
    :arg xml_string: XML string

    :rtype: str
    :returns: valid XML string
    """
    invalid_chars = re.compile(r'[\000-\010\013\014\016-\037]')
    return invalid_chars.sub('?', xml_string)


def _parse_args():
    """Parse and return cli arguments."""
    parser = optparse.OptionParser(
        usage='usage: %prog [options] args',
        description=__doc__)
    parser.add_option('-v', '--verbose', action='store_true',
                      help='Enable verbose output. (default: %default)')
    parser.add_option('-l', '--start-test-log',
                      default=_start_test_default_log(),
                      help='start_test log file. (default: %default)')
    parser.add_option('-o', '--junit-xml', default=_junit_xml_default(),
                      help='jUnit XML output file. (default: %default)')
    parser.add_option('-p', '--remove-prefix',
                      help=('Remove this prefix from all tests '
                            '(default: %default)'))
    parser.add_option('--debug', action='store_true',
                      help=('Throw exceptions when invalid data is '
                            'encountered. (default: %default)'))
    opts, _ = parser.parse_args()
    return opts


def _chpl_home():
    """Calculate and return $CHPL_HOME defaulting to root of repo/install."""
    repo_root = os.path.abspath(os.path.join(
        os.path.dirname(__file__), '../..'))
    return os.environ.get('CHPL_HOME', repo_root)


def _start_test_default_log():
    """Calculate default Logs/ dir (just like start_test)."""
    test_dir = os.path.join(_chpl_home(), 'test')
    logs_dir = os.path.join(test_dir, 'Logs')
    log_filename = '{user}.{target_platform}.log'.format(
        user=getpass.getuser(), target_platform=chpl_platform.get('target'))
    log_file = os.path.join(logs_dir, log_filename)
    return log_file


def _junit_xml_default():
    """Calculate default jUnit XML output file as $CHPL_HOME/chapel-tests.xml."""
    return os.path.join(_chpl_home(), 'chapel-tests.xml')


def _setup_logging(verbose):
    """Initialize logging and set level based on verbose.

    :type verbose: bool
    :arg verbose: When True, set log level to DEBUG.
    """
    log_level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(format='%(asctime)s [%(levelname)s] %(message)s',
                        level=log_level)
    logging.debug('Verbose output enabled.')


if __name__ == '__main__':
    main()
