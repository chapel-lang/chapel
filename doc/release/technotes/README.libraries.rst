.. _readme-libraries:

Chapel Libraries
================

[This readme discusses the implementation of libraries, which are still under
development.  The idea is to allow components to be developed in Chapel and
linked with other programs and languages.]

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

When Chapel code is accessed as library code, it is the user's responsibility to
ensure that modules are initialized before they are used.  For a module
named ``<myModule>``, the corresponding initialization routine is
named ``chpl__init_<myModule>``.  Therefore, for example, a C program that
makes use of the standard Chapel module ``Random`` should
call ``chpl__init_Random`` at some point before the Random interface is first
used.

.. code-block:: C

    // C code using the Chapel Random module.
    int main()
    {
      _int64 seed = 0;
      double rand;

      chpl__init_Random();
      RandomStream* rs = RandomStream_RandomStream(seed);

      // ...

      rand = RandomStream_getNext(rs);
    }

The interface to the Random module has not yet been exported, so the code above
is provided as an example only.  It is not expected to be functional.

Presumably, a C library defining an initialization routine and perhaps meeting
other criteria could be mentioned in a "use" statement within a Chapel
program.  As long as this module was also declared as "extern", the Chapel
compiler could arrange to link against it and call its initialization routine.
In that way, external code could be made to behave exactly like a native Chapel module.

