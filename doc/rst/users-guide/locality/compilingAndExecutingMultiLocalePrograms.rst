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

* ``ofi``: specifies that communication should be implemented using
  the open-source libfabric library component of OpenFabrics Interfaces
  (OFI).   This configuration is preliminary; see :ref:`readme-libfabric` for
  more information.

When using the Chapel module on Cray systems, a third option is also
available:

* ``ugni``: specifies that communication should be implemented using
  Cray's uGNI interface.

In order to use any of these options, the Chapel runtime libraries
must have been built with the specified communication layer enabled.
See :ref:`readme-building` for general information on building Chapel
and :ref:`readme-multilocale` for guidance specific to building
the runtime for multi-locale execution.


Compiling for One vs. Multiple Locales
--------------------------------------

The Chapel compiler supports distinct modes for creating single-locale
versus multi-locale executables.  This mode can be selected via the
``--[no-]local`` flag, whose default setting is based on the choice of
communication mechanism above.  Specifically, when the communication
is ``none``, the compiler defaults to ``--local`` compilation.  When
it is set to anything else, the compiler defaults to ``--no-local``
compilation.  Due to this default, this flag rarely needs to be used
in practice.

Executables compiled in ``--local`` mode can only execute using a
single locale.  Executables compiled in ``--no-local`` mode can be
executed on one or more locales, but typically incur overhead when run
on a single locale as compared to those compiled in ``--local`` mode.


Executing Using Multiple Locales
--------------------------------

To specify the number of locales on which a program should run, use
the ``--numLocales`` flag (or ``-nl`` for short).

For example, the following command-line says to run the given Chapel
program on four locales::

    ./myChapelProgram -nl 4

Note that all Chapel programs begin as a single task running on locale
0.  Control over locality is explicit within Chapel, like parallelism.
In order for a program to execute using multiple locales, it must
request this either explicitly using *on-clauses* or implicitly using
abstractions that themselves use on-clauses.

.. TODO: make on-clauses above into a link once that section is written.

Programs compiled with communication mode ``none`` can only execute on
a single shared-memory locale and therefore need not specify a number
of locales since the only valid choice is 1.  If a user tries to run
the program using more than one locale, an error message will be printed.
