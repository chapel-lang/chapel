#!/usr/bin/env python3

"""Checks pip major version against the Python version.
   If there is a mismatch, returns 'pip3'.

    ./find-pip.py pip-path
"""

import optparse
import os
import sys
import subprocess
import shutil

def main():
    parser = optparse.OptionParser(usage=__doc__)

    (options, args) = parser.parse_args()

    if len(args) != 1:
        sys.stderr.write('Error: path-value must be supplied\n\n')
        parser.print_help()
        sys.exit(1)

    pip = args[0]

    p = subprocess.Popen([pip, '--version'], stdout=subprocess.PIPE)
    pip_python_major = 0
    for line_bytes in p.stdout.readlines():
        # look for (python 3 or (python 2
        line = str(line_bytes, encoding='utf-8', errors='surrogateescape')
        needle = '(python '
        at = line.find(needle)
        if at >= 0:
            after = line[at+len(needle):]
            vers = after.split('.')
            pip_python_major = int(vers[0])

    python_vers = sys.version_info
    python_major = python_vers[0]

    pip3_path = pip

    if pip_python_major != 0:
        if python_major != pip_python_major:
            sys.stderr.write('{0} uses python version {1} but have python {2}\n'
                             .format(pip, pip_python_major, python_major))
            pip3_path = shutil.which('pip3')

            sys.stderr.write('falling back on pip3 {0}\n'.format(pip3_path))

    sys.stdout.write('{0}\n'.format(pip3_path))

if __name__ == '__main__':
    main()
