"""
Python 2/3 compatibility shim. We use subprocess.Popen.communicate() all over
sub_test. Instead of updating every call site to do the right thing with regard
to byte to str conversions for 3 compatibility, we have opted to add this
wrapper. Not all things we run will produce utf-8 (i.e. catfiles for thing like
mandelbrot will return binary data) so we allow the conversion to utf-8 to
silently fail and we handle any issues at those call sites.
"""

import subprocess
import sys

def bytes_to_str(byte_string):
    if sys.version_info[0] >= 3 and not isinstance(byte_string, str):
        try:
            return str(byte_string, 'utf-8')
        except UnicodeDecodeError as e:
            pass
        return byte_string
    else:
        return byte_string

def str_to_bytes(str_string):
    if sys.version_info[0] >= 3 and not isinstance(str_string, bytes):
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

    def __init__(self, popen):
        self._popen = popen

    def communicate(self):
        c = self._popen.communicate()
        return Py3CompatCommGetter(c)

    def __getattr__(self, name):
        return getattr(self._popen, name)


class Py3CompatCommGetter(object):

    def __init__(self, comm_result):
        self._c = comm_result

    def __getitem__(self, key):
        val = self._c[key]
        if key == 0 or key == 1:
            return bytes_to_str(val)
        else:
            return val

def Popen(*args, **kwargs):
    p = subprocess.Popen(*args, **kwargs)
    return Py3CompatPopen(p)
