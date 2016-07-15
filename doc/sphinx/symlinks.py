#!/usr/bin/env python
# encoding: utf-8

from __future__ import print_function

import os
"""
Symlink some files from doc/* to the sphinx project for generating
the web documentation
"""


def extfilter(directory, extension='.rst'):
    """Filter out files without extension '.rst'"""
    return [os.path.join(directory, f) for f in os.listdir(directory) if os.path.splitext(f)[1] == extension]


def main():
    """symlinks doc/release/* to doc/sphinx/source/ for *rst files"""

    # Check that $CHPL_HOME is defined, and set the path to a variable
    chpl_home = os.getenv('CHPL_HOME')
    if not chpl_home:
        print('$CHPL_HOME is not set. Unable to build webdocs')
        return 1

    # Get tree of 'index.rst's to copy over
    filedir = os.path.dirname(os.path.realpath(__file__))
    sourcedir = os.path.join(filedir, 'source')

    # Create list of READMEs converted to rst
    docs = {}
    docs['source/usingchapel'] = extfilter(os.path.join(chpl_home, 'doc/release'))
    docs['source/platforms'] = extfilter(os.path.join(chpl_home, 'doc/release/platforms'))
    docs['source/technotes'] = extfilter(os.path.join(chpl_home, 'doc/release/technotes'))

    # symlink READMEs into doc/sphinx
    for directory, rstfiles in docs.items():
        newdir = os.path.join(filedir, directory)
        for rstfile in rstfiles:
            # strip off README. prefix
            newrstfilename = os.path.split(rstfile)[1].replace('README.', '')
            newpath = os.path.join(newdir, newrstfilename)
            if os.path.exists(newpath):
                os.remove(newpath)
            os.symlink(rstfile, newpath)

    # CHPL_HOME README
    chplhomereadme = os.path.join(chpl_home, 'QUICKSTART.rst')
    os.symlink(chplhomereadme, 'source/usingchapel/QUICKSTART.rst')

    # Man pages
    chplmanpage = os.path.join(chpl_home, 'man', 'chpl.rst')
    os.symlink(chplmanpage, 'source/usingchapel/man.rst')

    chpldocmanpage = os.path.join(chpl_home, 'man', 'chpldoc.rst')
    os.symlink(chpldocmanpage, 'source/tools/chpldoc/man.rst')

    chpldocexamples = os.path.join(chpl_home, 'test', 'release', 'examples')
    os.symlink(chpldocexamples, 'source/users-guide/base/examples')

    chpldocexamples = os.path.join(chpl_home, 'test', 'release', 'examples')
    os.symlink(chpldocexamples, 'source/users-guide/taskpar/examples')

    chpldocexamples = os.path.join(chpl_home, 'test', 'release', 'examples')
    os.symlink(chpldocexamples, 'source/users-guide/datapar/examples')

    chpldocexamples = os.path.join(chpl_home, 'test', 'release', 'examples')
    os.symlink(chpldocexamples, 'source/users-guide/locality/examples')


if __name__ == '__main__':
    main()
