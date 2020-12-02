# This script ends up in chpldeps/__main__.py

"""
chpldeps
======

This Python module contains the dependencies used by the Chapel project.
Dependencies can be invoked as subcommands in the following ways:

  python3 path/to/chpldeps sphinx-build <sphinx options>
  python3 path/to/chpldeps rst2man <rst2man options>
  python3 path/to/chpldeps register-python-argcomplete
  python3 path/to/chpldeps path/to/something.py <something.py options>

For the last case, this program assumes that something.py has a
'main' function that can be called with no arguments. The arguments
to it will be communicated in sys.argv.

"""

import os
import re
import sys
import importlib.util

def missing_subcommand():
    print("chpldeps requires a subcommand", file=sys.stderr)
    print(__doc__, file=sys.stderr)
    sys.exit(-1)

if __name__ == '__main__':

    # leave out a python3 call if present
    starti = 1
    if starti >= len(sys.argv):
        missing_subcommand()

    if (sys.argv[1] == 'python3' or sys.argv[1] == 'python' or
        sys.argv[1].endswith('/python3') or sys.argv[1].endswith('/python')):

        starti = 2

    if starti >= len(sys.argv):
        missing_subcommand()

    # compute the subcommand we are running
    subcommand = sys.argv[starti]
    starti += 1

    # set the new arguments
    argv = [subcommand]
    argv.extend(sys.argv[starti:])
    sys.argv = argv

    if subcommand == 'sphinx-build':
        # copied from sphinx-build's body
        from sphinx.cmd.build import main
        sys.exit(main())

    elif subcommand == 'rst2man.py':
        # copied from rst2man.py's body
        import locale
        try:
            locale.setlocale(locale.LC_ALL, '')
        except:
            pass

        from docutils.core import publish_cmdline, default_description
        from docutils.writers import manpage

        description = ("Generates plain unix manual documents.  " + default_description)

        publish_cmdline(writer=manpage.Writer(), description=description)

    elif subcommand == 'register-python-argcomplete':

        # Run the argument as a program within bin/
        # just by exec'ing it
        # (can't import something with dashes)
        my_bin_dir = os.path.join(os.path.dirname(__file__), 'bin')
        subcommand_path = os.path.join(my_bin_dir, subcommand)

        exec(open(subcommand_path).read())

    elif os.path.isfile(subcommand):
        # Run the argument as a python script with the dependencies.
        # Assumes that the script uses the convention of having a
        # 'def main()' that we can call.
        dirname, basename = os.path.split(subcommand)
        base, ext = os.path.splitext(basename)
        sys.path.insert(0, os.path.abspath(dirname))
        spec = importlib.util.spec_from_file_location(base, subcommand)
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)
        sys.exit(mod.main())

    else:
        print("Unknown chpldeps subcommand {0}".format(subcommand),
              file=sys.stderr)
        print(__doc__, file=sys.stderr)
        sys.exit(-1)
