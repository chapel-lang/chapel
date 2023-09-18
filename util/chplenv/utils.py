"""Utility functions for chplenv modules"""

import os
import subprocess
import sys
import unittest

try:
    # Module `distutils` is deprecated in Python 3.10 and will be removed in Python 3.12
    # Prefer `shutil.which` in Python 3.2+
    from shutil import which
except ImportError:
    # Backport for pre Python 3.2
    from distutils.spawn import find_executable as which


def warning(msg):
    if not os.environ.get('CHPLENV_SUPPRESS_WARNINGS'):
        sys.stderr.write('Warning: ')
        sys.stderr.write(msg)
        sys.stderr.write('\n')


def error(msg, exception=Exception):
    """Exception raising wrapper that differentiates developer-mode output"""
    developer = os.environ.get('CHPL_DEVELOPER')
    if developer and developer != "0":
        raise exception(msg)
    else:
        sys.stderr.write('\nError: ')
        sys.stderr.write(msg)
        sys.stderr.write('\n')
        sys.exit(1)


def memoize(func):
    """Function memoizing decorator"""
    cache = func.cache = {}

    def memoize_wrapper(*args, **kwargs):
        if kwargs:
            return func(*args, **kwargs)
        if args not in cache:
            cache[args] = func(*args)
        return cache[args]
    return memoize_wrapper


class CommandError(Exception):
    """Custom exception for run_command errors"""
    pass


def try_run_command(command, cmd_input=None):
    """Command subprocess wrapper tolerating failure to find or run the cmd.
       For normal usage the vanilla run_command() may be simpler to use.
       This should be the only invocation of subprocess in all chplenv scripts.
       This could be replaced by subprocess.check_output, but that
       is only available after Python 2.7, and we still support 2.6 :("""
    try:
        process = subprocess.Popen(command,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE,
                                   stdin=subprocess.PIPE)
    except OSError:
        return (False, 0, None, None)
    byte_cmd_input = str.encode(cmd_input, "utf-8") if cmd_input else None
    output = process.communicate(input=byte_cmd_input)
    return (True, process.returncode, output[0].decode("utf-8"),
            output[1].decode("utf-8"))


def run_command(command, stdout=True, stderr=False, cmd_input=None):
    """Command subprocess wrapper.
       This is the usual way to run a command and collect its output."""
    exists, returncode, my_stdout, my_stderr = try_run_command(command,
                                                               cmd_input)
    if not exists:
        error("command not found: {0}".format(command[0]), OSError)
    if returncode != 0:
        error("command failed: {0}\noutput was:\n{1}".format(command,
                                                             my_stderr),
              CommandError)
    if stdout and stderr:
        return my_stdout, my_stderr
    elif stdout:
        return my_stdout
    elif stderr:
        return my_stderr
    else:
        return None


def run_live_command(command):
    """Run a command, yielding the merged output (stdout/stderr) as the process
       runs rather than returning the output after the process finishes"""
    try:
        process = subprocess.Popen(command,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT)
    except OSError:
        error("command not found: {0}".format(command[0]), OSError)

    for stdout_char in iter(lambda: process.stdout.read(1), str.encode("", "utf-8")):
        yield stdout_char.decode("utf-8")
    process.stdout.close()
    returncode = process.wait()

    if returncode != 0:
        error("command failed: {0}".format(command), CommandError)


def _split_ver_str(ver_str):
    """Split a version string into an array of integers"""

    # Semantic versioning strings can have prelease information
    # after a hypen and/or build metadata after a plus. For
    # the purpose of our versioning checks we strip these
    ver_str = ver_str.rsplit('-',1)[0]
    ver_str = ver_str.rsplit('+',1)[0]

    return [int(x) for x in ver_str.split('.')]


def is_ver_in_range(versionStr, minimumStr, maximumStr):
    """Assume that version, minimum, and maximum are version strings consisting
    of integers separated by periods. Ensure that version is within the
    bounds: [minimumStr, maximumStr); non upper-bound is non inclusive."""

    version = _split_ver_str(versionStr)
    minimum = _split_ver_str(minimumStr)
    maximum = _split_ver_str(maximumStr)

    # Pad version, minimum, and maximum with zeros so they're the same length
    max_len = max(len(version), len(minimum), len(maximum))
    version = version + [0] * (max_len - len(version))
    minimum = minimum + [0] * (max_len - len(minimum))
    maximum = maximum + [0] * (max_len - len(maximum))

    for i in range(max_len):
        if version[i] < minimum[i]:
            return False
        elif version[i] > minimum[i]:
            break

    for i in range(max_len):
        if version[i] < maximum[i]:
            return True
        elif version[i] > maximum[i]:
            return False

    return False


class _UtilsTests(unittest.TestCase):
    def test_is_ver_in_range(self):
        # Various tests for versions in range: [2, 4)
        for min_ver in ["2", "2.0", "2.0.0"]:
          for max_ver in ["4", "4.0", "4.0.0"]:
            # Versions too low
            for ver in ["0", "0.0", "1", "1.0", "1.0-alpha"]:
                self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

            # Versions in bound
            for ver in ["2", "2.0", "2.5", "3", "3-alpha", "3.1", "3.1.1", "3.9", "3.9.9.9.9"]:
              self.assertTrue(is_ver_in_range(ver, min_ver, max_ver))

            # Versions too high
            for ver in ["4", "4.0", "4.1", "4.0.1", "5", "5-alpha"]:
              self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

        # Various tests for versions in range: [2.5, 4.5)
        for min_ver in ["2.5", "2.5.0"]:
          for max_ver in ["4.5", "4.5.0"]:
            # Versions too low
            for ver in ["2", "2.4", "2.4.0", "2.4.0.0", "2.4.9.9.9.9"]:
              self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

            # Versions in bound
            for ver in ["2.5", "2.5.0", "2.6", "4.4", "4.4.0", "4.4.9"]:
              self.assertTrue(is_ver_in_range(ver, min_ver, max_ver))

            # Versions too high
            for ver in ["4.5", "4.5.0", "4.5.9", "4.6", "5", "5.0"]:
              self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))
