.. _readme-debugging:

=========================
Debugging Chapel Programs
=========================

This document discusses support for debugging your Chapel program and a set of
experimental settings to enable task monitoring and memory tracking.

.. contents::

-------------------
Running in gdb/lldb
-------------------

The compiler-generated executable has a ``--gdb`` flag that can be used to
launch the program within a ``gdb`` session.  A similar flag, ``--lldb``,
exists to launch the program within a ``lldb`` session. For best results, you
should follow `Best Known Configuration`_ to build Chapel and build your
application.

Running in gdb/lldb with a launcher
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When using almost any launcher, you can launch ``gdb`` by setting the
environment variable ``CHPL_COMM_USE_GDB`` when running the program.
This will open up a separate terminal emulator window for each locale,
each running the debugger on that locale's program instance.  On the Mac
OS X (darwin) platform, you can launch ``lldb`` instead, by setting the
``CHPL_COMM_USE_LLDB`` environment variable.  This works in all of these
launchers::

  amudprun
  aprun
  gasnetrun_ibv
  gasnetrun_mpi
  gasnetrun_ofi
  mpirun4ofi
  pbs-aprun
  smp

The default terminal emulator program is ``xterm``,
but by setting the environment variable ``CHPL_COMM_DBG_TERM=urxvt``
you can force use of ``urxvt`` instead.
Whichever terminal emulator is used must be in your ``PATH``
on the compute node or an error will result.
Note that it is the user's responsibility to make sure things are set up
so the terminal emulator run in the target environment can open its
display window in the launch environment.

------------------------
Best Known Configuration
------------------------

The current best practice for debugging Chapel source code is to use the C
backend and use a series of flags to improve the debuggability of the generated
executable. This can be done in two steps.

1) Build the compiler with ``CHPL_TARGET_COMPILER`` set to ``gnu``:

   .. code-block:: bash

        CHPL_TARGET_COMPILER=gnu make

2) Build the executable from Chapel source code:

   .. code-block:: bash

        chpl -g --target-compiler=gnu --savec <dir> --preserve-inlined-line-numbers --no-munge-user-idents <source_file>

For more details on these settings, read the rest of this section.

Building the Compiler
~~~~~~~~~~~~~~~~~~~~~

For best results while debugging, we recommend building the compiler with
``CHPL_TARGET_COMPILER`` set to ``gnu`` (or ``clang`` if on Mac). See
:ref:`readme-chplenv` for more information on building the compiler.

With two invocations of the build command, both backends can be built. First
execute ``make`` (which uses the LLVM backend by default) and then execute
``CHPL_TARGET_COMPILER=gnu make``. This will keep the default as LLVM and allow
switching to the C backend as needed for debugging. This can be done for a
particular invocation of the compiler with ``chpl --target-compiler=gnu ...``.

Building the Application
~~~~~~~~~~~~~~~~~~~~~~~~

The following flags can be useful for making the generated C more amenable to
debugging.

  ===================================  =========================================
  Flag                                 Description
  ===================================  =========================================
  ``-g``                               Generate debug symbols in the executable
  ``--target-compiler=gnu``            Target the C backend
  ``--savec <dir>``                    Write out the generated C to a given
                                       directory
  ``--preserve-inlined-line-numbers``  When code gets inlined (e.g. replacing a
                                       function call with the function body)
                                       maintain the filename and line number
                                       information of the original function
                                       call.
  ``--no-munge-user-idents``           Don't munge user identifiers (e.g.
                                       variable or function names). Munging
                                       typically prevents conflicts with
                                       identifiers in external code but makes
                                       debugging harder.
  ===================================  =========================================

Notes on munging
''''''''''''''''

The utility of using a debugger with Chapel depends greatly on your familiarity
with the Chapel generated code.  However, if your program is crashing or running
into a runtime error, you can often determine where that is taking place by
looking at a stack trace within ``gdb``.

When debugging Chapel, it is useful to know that in generating its code,
the Chapel compiler renames user identifiers.  By default, the Chapel
compiler munges all user identifiers, such that a variable named ``x``
would be code generated as ``x_chpl``.  This munging can be controlled
using the ``--[no-]munge-user-idents`` flag (see the ``chpl`` man page
for more information).  In some cases, additional munging may be
required or applied that cannot be turned off.

The net effect of this is that Chapel variables can often be inspected
using ``p`` *name*\ ``_chpl`` (or ``p`` *name*\ ``_chpl<TAB>`` in cases
where the compiler has further renamed the variable).  If the
``--no-munge-user-idents`` flag is used, ``p`` *name* or
``p`` *name*\ ``<TAB>`` should work in most cases.

See :ref:`more-munging-info` for more information on munging.

Over time, we plan to improve our ability to debug the generated C
code for a Chapel program.  If you find yourself debugging the
generated code a lot and need help or have requests for better
support, please let us know so that we can prioritize accordingly.

-------------------------------
Tracking and Reporting on Tasks
-------------------------------

For certain tasking layers, Chapel supports an experimental
capability for tracking the status of tasks, primarily designed for
use in a single-locale execution.  To enable this capability, your
program must be compiled with the ``--task-tracking`` flag.

The feature itself is enabled at execution time by setting the boolean
environment variable ``CHPL_RT_ENABLE_TASK_REPORTING`` to any of the
values "1", "yes", or "true".  If this is done, then when ``<CTRL-C>``
is entered while a program is executing, a list of pending and executing
tasks will be printed to the console, giving an indication of which
tasks are at which source locations.  This is only supported with
``CHPL_TASKS=fifo``.

Note that task tracking adds a fair amount of runtime overhead to
task-parallel programs.

-------------------------------------------
Configuration Constants for Tracking Memory
-------------------------------------------

Chapel supports a number of configuration constants related to dynamic
memory allocation for the compiler-generated executable, currently
designed for use primarily by the development team to track memory
usage in tests.  Please note that our generated code currently
contains memory leaks, so you should not be surprised if your program
requires more memory than it seems it should.

For full information on these configuration constants consult
:chpl:mod:`Memory`.

A brief synopsis of these configuration constants is as follows:

  --memTrack            turn on memory tracking and enable reporting
  --memStats            call ``printMemAllocStats()`` on normal termination
  --memLeaksByType      call ``printMemAllocsByType()`` on normal termination
  --memLeaks            call ``printMemAllocs()`` on normal termination
  --memMax=int          set maximum level of allocatable memory
  --memThreshold=int    set minimum threshold for memory tracking
  --memLog=string       file to contain all memory reporting
  --memLeaksLog=string  if set, append final stats and leaks-by-type here
