.. _readme-driver:

====================
Compiler Driver Mode
====================

.. note::

   The compiler driver mode is relatively new. If you use it and encounter a bug
   or limitation not yet documented as a
   `Github issue <https://github.com/chapel-lang/chapel/issues>`_, consider
   filing an issue as described in :ref:`readme-bugs`.

The Chapel compiler ``chpl`` previously ran as a single executable responsible
for compilation, assembly, and linking, with some components done in
subprocesses depending on which backend is used. By contrast, many compiler
executables (including ``gcc`` and ``clang``) are actually `compiler drivers`.
A compiler driver is a smaller program responsible for processing arguments and
invoking separate processes for the different stages of compilation required. As
of release 1.33, ``chpl`` runs as a compiler driver by default.

---------------------
Motivation for Driver
---------------------

There are a few potential advantages provided by the compiler driver approach,
which are not necessarily accomplished by the present implementation yet.

- Reduced memory pressure. Different driver phases phases are separate
  subprocesses, and the memory used by each can be completely reclaimed after
  it is done.
- Ability to perform some parts of compilation but not others, to avoid
  unnecessary work. For example, some compilers provide ways to check if code
  is valid without generating a binary, supporting a faster edit-recompile
  cycle; the driver provides one way of accomplishing this effect.
- Better debuggability. A developer could debug just the phase(s) of the driver
  relevant to what they're investigating, simplifying and saving time.
- Better compiler code organization via looser coupling between components.

--------------------
Driver Control Flags
--------------------

Several flags are available to control the actions performed by the driver. Some
are intended for the user, and others are considered internal and likely only
to be useful to compiler developers. Both are documented here.

- ``--compiler-driver``: Enable compiler driver mode. This flag mut be present
  to set any other driver flags. Without this flag, the compiler will run
  monolithically as usual.
- ``--driver-debug-phase``: Set which phase of compilation to run in the
  debugger: 'compilation', 'makeBinary', or 'all'. If debugging just
  compilation, makeBinary will be skipped entirely as it is unlikely to be
  useful.
- ``--driver-compilation-phase``: Internal flag. The driver re-invokes itself
  with this flag to to trigger execution of the compilation phase.
- ``--driver-makebinary-phase``: Internal flag. The driver re-invokes itself
  with this flag to to trigger execution of the makeBinary phase.
- ``--driver-tmp-dir``: Internal flag, specifying where the current phase will
  look for temporary files that must be carried between phases. The driver sets
  this flag during re-invocations for the different phases, and will provide the
  same value to all phases.

-------------------
Development History
-------------------

- With release 1.32, the Chapel compiler provides an opt-in compiler driver mode
  that can be used via the ``--compiler-driver`` flag. This mode will become the
  default at some point in the future. The driver currently splits work into two
  phases: `compilation`, which is responsible for everything through code
  generation (C code or LLVM bitcode), and `makeBinary`, which is responsible
  for binary generation (including linking).
- In release 1.33, the driver mode is on by default, and the
  ``--compiler-driver`` flag is deprecated. The new ``--no-compiler-driver``
  flag allows opting out of the driver mode and will cause the compiler to
  run monolithically as before.
