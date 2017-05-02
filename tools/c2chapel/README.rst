-----------------------------------------------------
c2chapel -- A tool to generate  C bindings for Chapel
-----------------------------------------------------

``c2chapel`` is a tool to help the Chapel programmer generate extern types,
records, and procedures when given a C99 file. For example, given the following
function declaration in a header file:

.. code-block:: c

  void foo(char* str, int n);

``c2chapel`` will generate Chapel code similar to:

.. code-block:: chapel

  extern proc foo(str: c_string, n : c_int) : void;


=============
Prerequisites
=============

This tool has the following prerequisites:
- A Unix-like environment (something with commands like cd, mkdir, rm, etc.)
- A GNU-compatible version of 'make'
- A C preprocessor
- Python 2.7 or Python 3.5

Other versions of Python may work, but only 2.7 and 3.5 have been tested at
this time.

========
Building
========

``c2chapel`` is built on top of `pycparser <https://github.com/eliben/pycparser>`_.
Building ``c2chapel`` will install pycparser into a local virtualenv, leaving
the user's python environment untouched. A symbolic link will be created in the
appropriate directory under ``$CHPL_HOME/bin``.

Simply run the following from the same directory as this README:

.. code-block:: sh

  make
  make install

Once installed, ``c2chapel`` should be visible in your ``$PATH`` provided you
have sourced a script like ``$CHPL_HOME/util/setchplenv.bash`` (also used to
make the ``chpl`` compiler available to your path). To test the ``c2chapel``
installation, you can run ``make check`` to automatically run some tests.

To remove ``c2chapel`` and files generated during the build process, execute
one of the following commands:

.. code-block:: sh

  make clean
  make cleanall
  make clobber

=====
Usage
=====

``c2chapel`` requires the name of a C99-compliant file, and emits the generated
Chapel code to stdout. For example, once installed you can run the following
command from within this directory:

.. code-block:: sh

  c2chapel test/fnints.h

The resulting output should be identical to ``test/fnints.chpl``. Run
``c2chapel`` with the ``--help`` flag for more information.

``c2chapel`` by default uses the fake standard headers included with pycparser.
These are headers used to work around compiler-specific macros or attributes
often found in C standard headers. Without these fake headers, pycparser will
probably not be able to parse the given C99 file. Usage of these fake headers
can be disabled with the ``--no-fake-headers`` flag.

===========
Future Work
===========

``c2chapel`` does not currently handle the entirety of C99, so some human
intervention may be required (e.g. commenting out unhandled portions of the
file). There are also some limitations based on Chapel's extern capability.
See http://chapel.cray.com/bugs.html for instructions on reporting bugs.

Known issues:
- enums may cause fatal errors
- fake standard headers are incomplete

============
Contributors
============
| Ben Harshbarger [Cray Inc.]
| Nikhil Padmanabhan [Yale University]
