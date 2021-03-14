"""
Python 2/3 compatibility shim. We use subprocess.Popen.communicate() all over.
Instead of updating every call site to do the right thing with regard to byte
to str conversions, we have opted to use this wrapper which assumes our
input/output to subprocesses will always be utf-8 strings. There are some cases
where that's not true (e.g. catfiles for Mandelbrot are binary), so we allow
the conversion to silently fail and we handle any issues at the call site.

This is probably one of the worst ways to get python 2 and 3 compatibility, but
it is pretty straightforward. A better solution might be to use six or future.
"""

import subprocess
import sys
import os
import errno

def instance_or_none(val, val_type):
    return val is None or isinstance(val, val_type)

def bytes_to_str(byte_string):
    if sys.version_info[0] >= 3 and not instance_or_none(byte_string, str):
        try:
            return str(byte_string, 'utf-8')
        except UnicodeDecodeError as e:
            pass
        return byte_string
    else:
        return byte_string

def str_to_bytes(str_string):
    if sys.version_info[0] >= 3 and not instance_or_none(str_string, bytes):
        return bytes(str_string, 'utf-8')
    else:
        return str_string

def concat_streams(stdout, stderr):
    """ Concats stderr to stdout. Returns str if both are utf-8 strings, bytes otherwise """
    if sys.version_info[0] >= 3:
        str_stdout = bytes_to_str(stdout)
        str_stderr = bytes_to_str(stderr)
        if isinstance(str_stdout, str) and isinstance(str_stderr, str):
            return str_stdout + str_stderr
        else:
            return str_to_bytes(stdout) + str_to_bytes(stderr)

    return stdout + stderr

class Py3CompatPopen(object):
    """ Popen wrapper where communicate will convert input to bytes, and try to
        convert output to str. Note that communicate() will try to stderr and
        strerr to str independently (so you could end up with 1 being str and
        the other being bytes)"""

    def __init__(self, popen):
        self._popen = popen

    def communicate(self, input=None):
        bytes_input = str_to_bytes(input)
        stdout, stderr = self._popen.communicate(bytes_input)
        str_stdout = bytes_to_str(stdout)
        str_stderr = bytes_to_str(stderr)
        return (str_stdout, str_stderr)

    def __getattr__(self, name):
        return getattr(self._popen, name)


def Popen(*args, **kwargs):
    p = subprocess.Popen(*args, **kwargs)
    return Py3CompatPopen(p)

def makedirs(path, mode=0o777, exist_ok=False):
    if sys.version_info[0] >= 3:
        os.makedirs(path, mode, exist_ok)
    else:
        try:
            os.makedirs(path, mode)
        except OSError as e:
            if exist_ok and e.errno == errno.EEXIST:
                pass # Ignore directory already existing error
            else:
                raise
