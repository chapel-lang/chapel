#!/usr/bin/env python

from __future__ import print_function

import os
import sys
import re

""" Fix embedded Python refs in chpl-venv install, for Cray Chapel modules only.

This fixes up the chpl-venv installation in a Cray Chapel module that was built using a non-standard
Python installation (ie, the Python used was not installed as "/usr/bin/python*").

1. Remove the python executables from the virtualenv.
   This effectively forces the virtualenv scripts to use the system python.
   Without this, "start_test" will not work if the Cray Chapel module is installed on a different
   machine (ie, if no Python is installed in the same location as when the module was built).

2. Rewrite shebang lines (in scripts) in the virtualenv that point to the non-standard Python exe.
   This change is probably not needed for most situations.

As noted, these fixes are only applicable when building a Cray Chapel module under certain conditions.
This has been tested only for the cray-xc platform, where the Cray Chapel module was:
 - built on a cray-xc CLE 5 (system Python version 2.6, non-standard user-installed Python 2.7), and
 - installed on a cray-xc CLE 6 (system Python version 2.7).
"""


def rm_python_bins(bindir):

    """ This removes any file objects named "python", "python2", etc found in bindir.

    bindir is the abs path to ./third-party/chpl-venv/$(bin_subdir)/$(python_version)/chpl-virtualenv/bin
    """

    if os.path.isdir(bindir) and not os.path.islink(bindir):
        for f in os.listdir(bindir):
            abs_f = os.path.join(bindir, f)
            # abs_f is the abs path to some file system object under bindir
            if f in [ 'python', 'python2', 'python2.6', 'python2.7' ]:
                print('Removing {0}'.format(abs_f))
                os.remove(abs_f)
    else:
        print('error, rm_python_bins(): {0} not found'.format(bindir))
        sys.exit(2)


def rw_shebangs(root):

    """ This finds and rewrites shebang lines that point to a non-standard Python executable.
    The new shebang points to the equivalent Python executable from a "standard" system Python installation
      (ie, "/usr/bin/python*").

    root is the abs path to $(chpl_home)/third-party/chpl-venv/$(bin_subdir)/$(python_version)
    """

    if os.path.isdir(root) and not os.path.islink(root):
        for f in os.listdir(root):
            rw = False

            abs_f = os.path.join(root, f)
            # abs_f is the abs path to some file system object under root
            if os.path.isfile(abs_f) and not os.path.islink(abs_f):
                # abs_f is a file
                with open(abs_f) as fin:
                    # read file abs_f and examine the first line
                    lines = fin.readlines()
                    if len(lines) > 0:
                        firstline = lines[0]
                        if re.match(r'#!/.*/bin/python', firstline):
                            # first line is a shebang that points to python
                            if not re.match(r'#!/usr/bin/python', firstline):
                                # shebang points to some non-standard location, so generate a new shebang
                                # the new shebang should be #!/usr/bin/python, #!/usr/bin/python2, etc
                                new_firstline = re.sub(r'^.*/bin/python', '#!/usr/bin/python', firstline)
                                lines[0] = new_firstline
                                rw = True

                if rw:
                    # rewrite the file
                    print('Rewriting shebang for {0}'.format(abs_f))
                    print(' - from\t{0}'.format(firstline.rstrip()))
                    print(' -   to\t{0}'.format(new_firstline.rstrip()))

                    with open (abs_f, 'w') as fout:
                        for line in lines:
                            fout.write(line)

    else:
        print('error, rw_shebangs(): {0} not found'.format(root))
        sys.exit(2)


def main(argv=None):
    chpl_venv_install_dir = argv[0]
    rm_python_bins(os.path.join(chpl_venv_install_dir, 'chpl-virtualenv', 'bin'))
    rw_shebangs(chpl_venv_install_dir)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1:])
    else:
        print('error, no argument found')
        sys.exit(2)
