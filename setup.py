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
    repo_root = os.path.abspath(os.path.dirname(__file__))
    sphinx_dir = os.path.join(repo_root, 'docs')

    assert not os.path.exists(sphinx_dir), (
        'Sphinx output dir already exists: {0}'.format(sphinx_dir))


    # Create a faux SysBasic for now.
    # FIXME: Update makeSysBasicTypes to not rely on bc.
    #        (thomasvandoren, 2015-03-24)
    modules_gen_dir = os.path.join(repo_root, 'modules', 'standard', 'gen')
    sys_docs_dir = os.path.join(modules_gen_dir, 'doc')
    sys_docs_chpl = os.path.join(sys_docs_dir, 'SysCTypes.chpl')
    if not os.path.exists(sys_docs_dir):
        os.makedirs(sys_docs_dir)
    with open(sys_docs_chpl, 'w') as fp:
        fp.write(sys_basic_for_docs)

    # Add the chplenv dir to the python path.
    chplenv_dir = os.path.join(repo_root, 'util', 'chplenv')
    sys.path.insert(0, os.path.abspath(chplenv_dir))
    import chpl_platform
    import chpl_compiler
    cfg = '{0}-{1}'.format(chpl_platform.get('target'),
                           chpl_compiler.get('target'))

    # Just touch the other SysCTypes.chpl so the make target won't be invoked.
    sys_c_dir = os.path.join(modules_gen_dir, cfg)
    sys_c_chpl = os.path.join(sys_c_dir, 'SysCTypes.chpl')
    if not os.path.exists(sys_c_dir):
        os.makedirs(sys_c_dir)
    with open(sys_c_chpl, 'w') as fp:
        fp.write('')

    cmd = ['make', 'docs',
           'CHPLDOC_OPTS=--save-sphinx {0}'.format(sphinx_dir)]
    print('Running: {0}'.format(' '.join(cmd)))
    proc = subprocess.Popen(cmd, cwd=repo_root)
    proc.communicate()

    assert proc.returncode == 0, (
        '"make docs" failed with exit code: {0}'.format(proc.returncode))


sys_basic_for_docs = """
/* The type corresponding to the C int type */
extern type c_int= integral;
/* The type corresponding to the C uint type */
extern type c_uint= integral;
/* The type corresponding to the C long type */
extern type c_long= integral;
/* The type corresponding to the C unsigned long type */
extern type c_ulong= integral;
/* The type corresponding to the C long long type */
extern type c_longlong= integral;
/* The type corresponding to the C unsigned long long type */
extern type c_ulonglong= integral;
/* The type corresponding to the C char type */
extern type c_char= integral;
/* The type corresponding to the C signed char type */
extern type c_schar= integral;
/* The type corresponding to the C unsigned char type */
extern type c_uchar= integral;
/* The type corresponding to the C short type */
extern type c_short= integral;
/* The type corresponding to the C unsigned short type */
extern type c_ushort= integral;
/* The type corresponding to the C intptr_t type */
extern type c_intptr= integral;
/* The type corresponding to the C uintptr_t type */
extern type c_uintptr= integral;
/* The type corresponding to the C ptrdiff_t type */
extern type c_ptrdiff= integral;
/* The type corresponding to the C ssize_t type */
extern type ssize_t= integral;
/* The type corresponding to the C size_t type */
extern type size_t= integral;
extern type c_void_ptr; // opaque; no ptr arithmetic in Chapel code!

{
  pragma "no prototype"
  extern proc sizeof(type t): size_t;

  assert(sizeof(c_int) == sizeof(int(32)));
  assert(sizeof(c_uint) == sizeof(uint(32)));
  assert(sizeof(c_long) == sizeof(int(64)));
  assert(sizeof(c_ulong) == sizeof(uint(64)));
  assert(sizeof(c_longlong) == sizeof(int(64)));
  assert(sizeof(c_ulonglong) == sizeof(uint(64)));
  assert(sizeof(c_char) == sizeof(int(8)));
  assert(sizeof(c_schar) == sizeof(int(8)));
  assert(sizeof(c_uchar) == sizeof(uint(8)));
  assert(sizeof(c_short) == sizeof(int(16)));
  assert(sizeof(c_ushort) == sizeof(uint(16)));
  assert(sizeof(c_intptr) == sizeof(int(64)));
  assert(sizeof(c_uintptr) == sizeof(uint(64)));
  assert(sizeof(c_ptrdiff) == sizeof(int(64)));
  assert(sizeof(ssize_t) == sizeof(int(64)));
  assert(sizeof(size_t) == sizeof(uint(64)));
}
"""


if __name__ == '__main__':
    main()
