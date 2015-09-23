.. _readme-libraries:

Chapel Libraries
================

.. note::

   This page discusses the implementation of libraries, which are still
   under development.  The idea is to allow components to be developed in
   Chapel and linked with other programs and languages.  It is very fragile
   and very difficult to work with, so user beware.

Normally, Chapel assumes that you are building a main program, and produces a
main routine whether one is explicitly coded or not.  When the ``--library``
flag is provided on the command line, it tells the Chapel compiler to produce a
library instead.  The user can further specify (through the ``--static``
and ``--dynamic`` flags) which type of library to produce.  If
neither ``--static`` nor ``--dynamic`` is specified, a platform-dependent
default library type is produced.

Some platforms support linking against both static and dynamic versions of
the same library.  On those platforms, the ``--static`` or ``--dynamic``
flag can be used to select which type of library (and thus which kind of
linking) is performed by default.  Library files which are named explicitly on
the ``chpl`` command line take precedence over any found through object
library paths (``-L``).  Where there is a conflict, the last library
specified takes precedence.

When creating a library file from Chapel code, only those symbols with
``export`` attached to them will be available from outside the library.  For
example:

.. code-block:: Chapel

   // This function will be available to the library user
   export proc foo() {
     // Does something
     ...
   }

   // but this function will not be
   proc bar() {
     // Does something else
     ...
   }

When using a Chapel library file in C code, a fairly exact incantation is
required.  First, a header file must be created for the library.  This does not
happen through use of the Chapel ``--library`` flag during compilation!  If
during compilation of the library the ``--savec <dir>`` flag is also used, the C
definitions for the exported functions should be visible in a C file under
*dir* of the same name as the library Chapel file.  A declaration version of
these definitions should go into a ``.h`` file.  For instance, if one compiled
a Chapel file ``foo.chpl`` like this:

.. code-block:: sh

   chpl --library --savec cDir --static -o libfoo foo.chpl


then under ``cDir/foo.c`` one might see something resembling:

.. code-block:: C

   int64_t bar(void) {
     ...
   }

   void baz(int64_t x) {
     ...
   }

from which one would create a ``.h`` file:

.. code-block:: C

   int64_t bar(void);
   void baz(int64_t x);


Next, update the ``$LD_LIBRARY_PATH`` environment variable to include the
directory where the new library file lives and the directory where the Chapel
build lives.  The latter can be found by looking at the output of a
``$CHPL_HOME/util/printchplenv`` call and finding the appropriate directory
under ``$CHPL_HOME/lib``; the directory name is of the form
``$CHPL_HOST_PLATFORM.$CHPL_HOST_COMPILER.(<env variable suffix>-<corresponding environment variable.)+``.
For instance, if the output of a printchplenv call was::

   CHPL_HOST_PLATFORM: linux64
   CHPL_HOST_COMPILER: gnu
   CHPL_TARGET_PLATFORM: linux64
   CHPL_TARGET_COMPILER: gnu
   CHPL_TARGET_ARCH: native
   CHPL_LOCALE_MODEL: flat
   CHPL_COMM: none
   CHPL_TASKS: fifo
   CHPL_LAUNCHER: none
   CHPL_TIMERS: generic
   CHPL_MEM: cstdlib
   CHPL_MAKE: gmake
   CHPL_ATOMICS: intrinsics
   CHPL_GMP: gmp
   CHPL_HWLOC: none
   CHPL_REGEXP: re2
   CHPL_WIDE_POINTERS: struct
   CHPL_LLVM: none
   CHPL_AUX_FILESYS: none

then the ``$CHPL_HOME/lib/`` subdirectory is::

     linux64.gnu.arch-native.loc-flat.comm-none.tasks-fifo.tmr-generic.mem-cstdlib.atomics-intrinsics.gmp.hwloc-none.re2.wide-struct.fs-none/

.. code-block:: sh

   # Replace $PWD with the appropriate path to your library file if it isn't
   # in the current directory
   # Replace libDir with the directory we found from the previous step.
   export LD_LIBRARY_PATH=$PWD:<libDir>:$LD_LIBRARY_PATH


When using a Chapel library from C, one must first initialize the Chapel runtime
and standard modules.  This is done by the addition of a couple of includes and
by calling the function ``chpl_library_init()`` before the Chapel library
function calls and by calling ``chpl_library_finalize()`` after all the Chapel
library function calls are finished.  For example:

.. code-block:: C

   #include "chpl-string.h"
   #include "chpl-init.h"
   #include "testing.h"

   int main(int argc, char* argv[]) {
       chpl_library_init(argc, argv);

       baz(7); // Call into a library function

       chpl_library_finalize();

       return 0;
   }

Finally, compilation of the C program involves some additional command line
includes and links.  The easiest way to get the right combination is from
the compilation output of another Chapel program.  If one compiles with
``--print-commands`` and uses all the ``-I`` output and ``-std=c99`` during
compilation of the C program, one would be most of the way towards a successful
compile.  All that is missing is the addition of
``$CHPL_HOME/runtime/src/chpl-init.c`` before the C program file, and
``-L<libDir> -l<nameOfLib> -L<Chapel Lib location from LD step> -lchpl -lpthread`` in the linking stage.

.. note::

   When compiling with ``CHPL_TASKS=qthreads``, ``CHPL_GMP=gmp``,
   ``CHPL_HWLOC=hwloc``, ``CHPL_COMM=gasnet``, or ``CHPL_REGEXP=re2``,
   the appropriate third-party ``install/<CHPL_SETTING>/lib`` folder will
   need to be added to ``$LD_LIBRARY_PATH`` and the ``-L`` step here, as well
   as a ``-l`` for the library name.  ``CHPL_TASKS=fifo`` does require this.

Chapel library files cannot be used from Chapel code.  The library files must
include the chapel runtime and standard modules for use in a non-Chapel program
and when the library is linked to a Chapel program this leads to multiple
definitions of these functions.

As mentioned above, this feature is not very sturdy.  Please refer to
:ref:`readme-bugs` if any problems are encountered.
