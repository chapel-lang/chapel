.. _readme-debugging:

=========================
Debugging Chapel Programs
=========================

This document discusses support for debugging your Chapel program and a set of
experimental flags and configuration constants to enable task monitoring and
memory tracking.

.. contents::

--------------
Running in gdb
--------------

The compiler-generated executable has a ``--gdb`` flag that can be used
to launch the program within a ``gdb`` session.  For best results, make
sure that your program has been compiled using the chpl compiler's
``-g`` flag.

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
  gasnetrun_psm
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

The utility of this feature depends greatly on your familiarity with
the Chapel generated code.  However, if your program is crashing or
running into a runtime error, you can often determine where that is
taking place by looking at a stack trace within ``gdb``.

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

Over time, we plan to improve our ability to debug the generated C
code for a Chapel program.  If you find yourself debugging the
generated code a lot and need help or have requests for better
support, please let us know so that we can prioritize accordingly.


------------------------
Flags for Tracking Tasks
------------------------

For certain tasking layers, Chapel supports a few experimental
capabilities for tracking the status of tasks, primarily designed for
use in a single-locale execution.  To enable this capability, your
program must be compiled with the ``--task-tracking`` flag.  These flags
add a fair amount of runtime overhead to task-parallel programs. The
flags are as follows:

  -b, --blockreport  When ``<CTRL-C>`` is entered during a program
                     executing under this flag, it will display a list
                     of where tasks are blocked on a synchronization
                     variable.  Running with this flag will also cause
                     the executable to attempt to automatically detect
                     deadlock for single-locale executions.  This is
                     only supported with ``CHPL_TASKS=fifo``.

  -t, --taskreport   When ``<CTRL-C>`` is entered during a program
                     executing under this flag, a list of pending and
                     executing tasks will be printed to the console,
                     giving an indication of which tasks are at which
                     source locations.  This is only supported with
                     ``CHPL_TASKS=fifo``.


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
