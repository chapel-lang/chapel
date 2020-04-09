# General things to do (in no particular order):

- Rework Chapel FEB support to use new task-queue system.

- Rework most qutil/qloop functions to deal with deactivated shepherds.

- Implement periodic task system.

- Implement direct thread swapping, esp. for sinc's or other synchronization operations where the next thread to execute is obvious.

- Add a `qthread_replace(me, func, arg, argsize)` function to enable convenient tail-recursion algorithms.

- Implement Qthreads with in/out vectors for cross-node workstealing.

- Implement 128-bit syncvars.

- Implement cross-node synchronization (i.e. fill remote FEB).

- Implement hierarchical shepherds (need to rename shepherds).

- Add support for "spawn_to" information in the precondition record.

# General things to think about:

- Refactor code base to provide a proper `qthread.hpp` for including in C++ applications, in place of the (clever) C++ support in `qthread.h`.

- Implementing MAMA malloc.

- Coalescing system calls.

- Non-executing shepherds to allow for memory locales without associated computing resources.

- Porting to XMT.
