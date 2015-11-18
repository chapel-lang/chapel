:title: Users Guide: Compiling and Executing Multi-Locale Programs

Compiling and Executing Multi-Locale Programs
=============================================

Selecting a Communication Mechanism
-----------------------------------

All multi-locale Chapel programs are implemented using runtime
communication routines which transparently move data and tasks between
the locales on which the program is running.  The choice of
communication runtime is made via the ``--comm`` compiler flag (or
equivalently, the ``$CHPL_COMM`` environment variable).  Portable
options for communication include:

* ``none``: specifies that no communication should be performed,
  restricting the program's execution to a single locale.

* ``gasnet``: specifies that communication should be implemented using
  the open-source GASNet library developed at Berkeley.

When using the Chapel module on Cray systems, a third option is
available:

* ``ugni``: specifies that communication should be implemented using
  Cray's uGNI interface.

In order to use any of these options, the Chapel runtime libraries
must have been built with the specified communication layer enabled.
See :ref:`readme-building` for more information.


Compiling Single- vs. Multi-Locale Executables
----------------------------------------------

The Chapel compiler supports distinct modes for creating single-locale
versus multi-locale executables.  This mode can be selected via the
``--[no-]local`` flag, whose default setting is based on the choice of
communication mechanism above.  Specifically, when the communication
is ``none``, the compiler defaults to ``--local`` compilation.  When
it is set to anything else, the compiler defaults to ``-no-local``
compilation.


Executing a Chapel Program with Multiple Locales
------------------------------------------------

Programs compiled in ``--no-local`` mode support distributed-memory
execution across multiple locales.  When running such programs, the
number of locales on which the program should execute is specified
using the ``--numLocales`` flag (or ``-nl`` for short).  For example,
the following command-line would attempt to launch the given Chapel
program on four locales:

    ./myChapelProgram -nl 4

Programs compiled with communication mode ``none`` can only execute on
a single shared-memory locale and therefore need not specify a number
of locales since the only valid choice is 1.  If a user tries to run
the program using more than one locale, an error message is printed.
