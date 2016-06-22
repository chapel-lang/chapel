#!/usr/bin/env python

from __future__ import print_function

import os
import sys

# Remove the virtualenv pythons as well as references to them in
# scripts. This effectively forces the virtualenv scripts to use the
# system python.
def remove_venv_python_references(venv_bin_dir):
    for f in os.listdir(venv_bin_dir):
        abs_f = os.path.join(venv_bin_dir, f)
        if f.startswith("python"):
            print('Removing {0}'.format(abs_f))
            os.remove(abs_f)
        else:
            with open(abs_f) as fin:
                lines = fin.readlines()

            python_shebang = '#!/usr/bin/env python'
            if len(lines) > 0  and lines[0].startswith (python_shebang):
                print('Updating shebang for {0}'.format(abs_f))
                lines[0] = '{0}\n'.format(python_shebang)

            with open (abs_f, 'w') as fout:
               for line in lines:
                   fout.write(line)

def main():
    venv_bin_dir = sys.argv[1]
    remove_venv_python_references(venv_bin_dir)


if __name__ == '__main__':
    main()
