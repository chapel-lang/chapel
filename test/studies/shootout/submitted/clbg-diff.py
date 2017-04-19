#!/usr/bin/env python
# encoding: utf-8

"""
clbg-diff.py

Check out a copy of CLBG benchmarks and diff the CLBG's shootouts against
chapel-lang's shootouts.
"""

from __future__ import print_function

import difflib
import glob
import os
import re
import shlex
import subprocess
import sys


def main():
    print('------------------------')
    print('Updating CLBG repository')
    print('------------------------')
    print()
    CLBG_repo = ':pserver:anonymous@cvs.debian.org:/cvs/benchmarksgame'
    CLBG_dir = 'benchmarksgame/bench/'
    cvs_checkout(CLBG_repo, CLBG_dir)

    print()
    print('---------------')
    print('Computing diffs')
    print('---------------')
    print()

    # List of Chapel-implementations
    submitted = glob.glob('*.chpl')
    # List of CLBG-implementations
    clbg = glob.glob('benchmarksgame/bench/*/*.chapel')
    # List of CLBG-implementations without path
    clbgbase = [os.path.basename(c) for c in clbg]

    for i in range(len(submitted)):
        s = submitted[i]
        c = clbgify_filename(s)
        if c in clbgbase:
            c_chapel = clbg[clbgbase.index(c)]
            diff(s, c_chapel)
        else:
            '{0} not found'.format(s)

    print()
    print('------------------------------')
    print('Checking for missing shootouts')
    print('------------------------------')
    print()
    clbg_set = set(clbgbase)
    submitted_set = {clbgify_filename(s) for s in submitted}

    clbg_missing = submitted_set.difference(clbg_set)
    if clbg_missing:
        print('Extra shootouts found:')
        for c in clbg_missing:
            print(c)

    submitted_missing = clbg_set.difference(submitted_set)
    if submitted_missing:
        print('Missing shootouts')
        for s in submitted_missing:
            print(s)

    print()
    print('----')
    print('Done')
    print('----')
    print()


def cvs_checkout(r, d):
    """Checkout cvs repository"""
    cmd = 'cvs -d {0} checkout {1}'.format(r, d)
    stdout, stderr = run_command(cmd, True)


def clbgify_filename(f):
    """Convert our filenames to clbg-style filenames"""
    r = f
    r = r.replace('chpl', 'chapel')
    r = re.sub(r'([0-9])', r'.chapel-\1', r)
    return r


def diff(f1, f2):
    """Take the diff of 2 files"""
    file1 = open(f1, 'r')
    file2 = open(f2, 'r')

    diff = difflib.ndiff(file1.readlines(), file2.readlines())
    delta = ''.join(x for x in diff if (x.startswith('- ') or x.startswith('+ ')))

    if delta:
        print('diff ', f1, ' ', f2)
        print(delta)


def run_command(cmd, verbose=False):
    """Run a command and return stdout, stderr"""
    if verbose:
        print(cmd)
    p = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print(p.stdout)
        print(p.stderr)
        print("Command failed: {0}".format(cmd))
        sys.exit(p.returncode)
    return (stdout, stderr)


if __name__ == '__main__':
    if '-h' in sys.argv or '--help' in sys.argv:
        print(__doc__)
        sys.exit(0)
    main()
