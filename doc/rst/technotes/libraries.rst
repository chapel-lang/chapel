.. _readme-libraries:

Exporting Chapel as a Library
=============================

.. note::

   This page discusses the implementation of libraries, which are still
   under development.  The idea is to allow components to be developed in
   Chapel and linked with other programs and languages.  It is currently fragile
   and difficult to work with.

Normally, Chapel assumes that you are building a main program and produces a
main routine whether one is explicitly coded or not.  When the ``--library``
flag is provided on the command line, it tells the Chapel compiler to produce a
library instead, by default in a sub-directory named ``lib`` (which will be
created if it does not already exist).  The user can further specify (through
the ``--static`` and ``--dynamic`` flags) which type of library to produce.  If
neither ``--static`` nor ``--dynamic`` is specified, a platform-dependent
default library type is produced.

Some platforms support linking against both static and dynamic versions of
the same library.  On those platforms, the ``--static`` or ``--dynamic``
flag can be used to select which type of library (and thus which kind of
linking) is performed by default.  Library files which are named explicitly on
the ``chpl`` command line take precedence over any found through object
library paths (``-L``).  Where there is a conflict, the last library
specified takes precedence.

The location for the generated library and associated files can be changed
using the compilation flag ``--library-dir``.

When creating a library file from Chapel code, only those symbols with
``export`` attached to them will be available from outside the library.  For
example, if one defines a Chapel file ``foo.chpl`` like this:

.. code-block:: Chapel

   // This function will be available to the library user
   export proc bar(): int {
     // Does something
     ...
   }

   // As will this one
   export proc baz(int x) {
     // Does something different
     ...
   }

   // but this function will not be
   proc gloop() {
     // Does something else
     ...
   }

A header file will be generated for the library by default, using the same base
name as the library (replacing ``.so`` or ``.a`` with ``.h``).  This name can be
changed using the flag ``--library-header`` at compilation.

When using a Chapel library file in C code, a fairly exact incantation is
required.  If compiling dynamically, update the ``$LD_LIBRARY_PATH`` environment
variable to include the directory where the new library file lives and the
directory where the Chapel build lives.  The latter can be found by looking at
the output of a ``$CHPL_HOME/util/printchplenv`` call and finding the
appropriate directory under ``$CHPL_HOME/lib``; the directory name can be found
by running ``$CHPL_HOME/util/printchplenv --runtime --path``.

.. code-block:: sh

   # Replace $PWD with the appropriate path to your library file if it isn't
   # in the current directory
   # Replace libDir with the directory we just found.
   export LD_LIBRARY_PATH=$PWD:$CHPL_HOME/lib/`$CHPL_HOME/util/printchplenv --runtime --path`:$LD_LIBRARY_PATH


When using a Chapel library from C, one must first initialize the Chapel runtime
and standard modules.  This is done by the addition of a couple of extern
declarations, calling the function ``chpl_library_init()`` before the Chapel
library function calls and by calling ``chpl_library_finalize()`` after all the
Chapel library function calls are finished.  For example:

.. code-block:: C

   #include "foo.h"

   extern void chpl_library_init(int argc, char* argv[]);
   extern void chpl_library_finalize(void);

   int main(int argc, char* argv[]) {
       chpl_library_init(argc, argv);

       baz(7); // Call into a library function

       chpl_library_finalize();

       return 0;
   }

Finally, compilation of the C program involves some additional command line
includes and links.  For your convenience, a sample Makefile can be generated
using ``--library-makefile``.  This will generate a file named
``Makefile.libname``, where libname is the name of the library.  This Makefile
can then be included and its variables referenced in your own Makefile.  An
example looks like this:

.. code-block:: make

   include lib/Makefile.foo

   myCProg: myCProg.c lib/libfoo.a
     $(CHPL_COMPILER) $(CHPL_CFLAGS) -o myCProg myCProg.c $(CHPL_LDFLAGS)


Alternatively, you can get the right combination is by using the ``compileline
--compile`` and ``compileline --libraries`` tools we provide.  The compilation
command would then look like this (replacing myprog.c with the name of your C
program):

.. code-block:: sh

   `$CHPL_HOME/util/config/compileline --compile` myprog.c -L. -lfoo `$CHPL_HOME/util/config/compileline --libraries`

Note that ``compileline --compile-c++`` is also available. It requests a
compilation command able to compile a C++ program.

Chapel library files cannot be used from Chapel code.  The library files must
include the chapel runtime and standard modules for use in a non-Chapel program
and when the library is linked to a Chapel program this leads to multiple
definitions of these functions.

As mentioned above, this feature is not very sturdy.  Please refer to
:ref:`readme-bugs` if any problems are encountered.
