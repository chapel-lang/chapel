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
executables (including ``gcc`` and ``clang``) are actually `compiler drivers`.
A compiler driver is a smaller program responsible for processing arguments and
invoking separate processes for the different stages of compilation required.

With release 1.32, the Chapel compiler provides an opt-in compiler driver mode
that can be used via the ``--compiler-driver`` flag. This mode will become the
default at some point in the future. The driver currently splits work into two
phases: phase one, which is responsible for everything through code generation
(C code or LLVM bitcode), and phase two, which is responsible for binary
generation (including linking).

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
  debugger: '1', '2', or 'all'. If debugging just phase one, phase two will be
  skipped entirely as it is unlikely to be useful.
- ``--driver-phase-one``: Internal flag. Causes the execution of phase one.
  The driver re-invokes itself with this flag to run phase one.
- ``--driver-phase-two``: Internal flag. Causes the execution of phase two.
  The driver re-invokes itself with this flag to run phase two.
- ``--driver-tmp-dir``: Internal flag, specifying where the current phase will
  look for temporary files that must be carried between phases. The driver sets
  this flag during re-invocations for the different phases, and will provide the
  same value to all phases.

-----------
Future Work
-----------

- Fix driver behavior for GPU compilation, which currently performs all work
  in phase one and skips phase two.
- Reduce work re-done between driver and phase invocations.
- Enable our usual performance and correctness testing for driver mode, and
  reach parity with monolithic mode.
- Measure performance of compiler itself in driver mode, including change in
  memory pressure.
- Eventually, make compiler driver mode the default, with an opt-out flag.
