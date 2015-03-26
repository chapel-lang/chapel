#!/usr/bin/env python

"""Build the module docs and leave the sphinx project in $CHPL_HOME/docs/.

This allows the Chapel module documentation to build and publish on
readthedocs.org. It expects a standard python project setup.py, but this seems
to work, too.

This script basically calls:

    cd ../.. && make docs

"""

import os
import os.path
import subprocess
import sys


def main():
    on_rtd = os.environ.get('READTHEDOCS', None) == 'True'
    repo_root = os.path.abspath(os.path.dirname(__file__))
    sphinx_dir = os.path.join(repo_root, 'docs')

    if on_rtd and os.path.exists(sphinx_dir):
        print('Deleting pre-existing sphinx_dir at: {0}'.format(
            sphinx_dir))
        shutil.rmtree(sphinx_dir)
    assert not os.path.exists(sphinx_dir), (
        'Sphinx output dir already exists: {0}'.format(sphinx_dir))

    cmd = ['make', 'docs',
           'CHPLDOC_OPTS=--save-sphinx {0}'.format(sphinx_dir)]
    print('Running: {0}'.format(' '.join(cmd)))
    proc = subprocess.Popen(cmd, cwd=repo_root)
    proc.communicate()

    assert proc.returncode == 0, (
        '"make docs" failed with exit code: {0}'.format(proc.returncode))


if __name__ == '__main__':
    main()
