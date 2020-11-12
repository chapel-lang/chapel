import os
import re
import sys
import importlib.util

if __name__ == '__main__':

    # leave out a python3 call if present
    starti = 1
    if (sys.argv[1] == 'python3' or sys.argv[1] == 'python' or
        sys.argv[1].endswith('/python3') or sys.argv[1].endswith('/python')):

        starti = 2

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
        sys.exit(-1)
