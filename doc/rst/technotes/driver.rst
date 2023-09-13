.. _readme-driver:

====================
Compiler Driver Mode
====================

.. note::

   The compiler driver mode is considered experimental, and is not yet
   well-tested for correctness or performance.
   If you use it and encounter a bug or limitation not yet documented as a
   `Github issue <https://github.com/chapel-lang/chapel/issues>`_, consider
   filing an issue as described in :ref:`readme-bugs`.

The Chapel compiler ``chpl`` currently runs as a single executable responsible
for compilation, assembly, and linking, with some components done in
subprocesses depending on which backend is used. By contrast, many compiler
executables (including ``gcc`` and ``clang``) are actually compiler drivers, a
smaller driver program which is responsible for processing arguments and
invoking separate processes for the different components of compilation
required.

With release 1.32, the Chapel compiler provides an opt-in experimental compiler
driver mode that can be used via the ``--compiler-driver`` flag; at some point
in the future this will become the default mode of operation.

---------------------
Motivation for Driver
---------------------

placeholder

--------------------
Driver Control Flags
--------------------

placeholder

-----------------
Known Limitations
-----------------

placeholder

-----------
Future Work
-----------

placeholder
