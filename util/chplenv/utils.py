"""Utility functions for chplenv modules"""
import os
import subprocess
import sys


def error(msg, exception=Exception):
    """Exception raising wrapper that differentiates developer-mode output"""
    if os.environ.get('CHPL_DEVELOPER'):
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


def try_run_command(command, stdout=True, stderr=False, cmd_input=None):
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
        return False, 0, ''
    byte_cmd_input = str.encode(cmd_input, "utf-8") if cmd_input else None
    output = process.communicate(input=byte_cmd_input)
    output = (output[0].decode("utf-8"), output[1].decode("utf-8"))
    if stdout and stderr:
        return True, process.returncode, output
    elif stdout:
        return True, process.returncode, output[0]
    elif stderr:
        return True, process.returncode, output[1]
    else:
        return True, process.returncode, ''


def run_command(command, stdout=True, stderr=False, cmd_input=None):
    """Command subprocess wrapper.
       This is the usual way to run a command and collect its output."""
    exists, returncode, output = try_run_command(command, stdout, stderr,
                                                 cmd_input)
    if not exists:
        error("command not found: {0}".format(command[0]), OSError)
    if returncode != 0:
        error("command failed: {0}\noutput was:\n{1}".format(
            command, output[1]), CommandError)
    return output


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
