.. _best-practices-memory-profiling:

Memory Profiling
================

When using a non-standard memory allocator such as jemalloc, the allocator may
provide profiling capabilities that can be useful for understanding memory
usage patterns. For Chapel programs, this can be used to augment the existing
Chapel runtime's memory tracking capabilities. For the Chapel compiler, this
can be used to understand the memory usage of the compiler itself.

Profiling the compiler with jemalloc
------------------------------------

To profile the compiler with jemalloc, make sure you build the compiler
with the following environment variables set:

.. code-block:: bash

   export CHPL_HOST_MEM=jemalloc
   export CHPL_HOST_JEMALLOC="bundled"
   export CHPL_JEMALLOC_ENABLE_PROFILING=1

Then the compiler can be run as follows:

.. code-block:: bash

   MALLOC_CONF='prof:true,prof_prefix:jeprof.out,prof_final:true' \\
     chpl myProgram.chpl

The value of ``prof_prefix`` is the prefix for the output files.

.. note::

   It may be useful to pass ``--no-compiler-driver`` to the compiler. Without this
   flag, the compiler functions as a driver and spawns several subprocesses for
   different phases of the compiler. This will results in multiple profiling
   output files, one for each subprocess. If you want to profile the compiler as a
   single process, use ``--no-compiler-driver``.

Output files can be interpreted using ``jeprof`` (which is built with
jemalloc). For example, the following will generate a simple text report,
ordered by cumulative memory usage:

.. code-block:: bash

   JEMALLOC_SUBDIR=$(./util/chplenv/chpl_jemalloc.py --host --cfg-path)
   ./third-party/jemalloc/install/$JEMALLOC_SUBDIR/bin/jeprof \\
     --text --cum $(which chpl) path/to/profile/data/jeprof.out


Profiling Chapel programs with jemalloc
---------------------------------------

.. warning::

   When writing these docs (as of Chapel 2.6), I was unable to get jemalloc's
   profiling to give good results for Chapel programs. jemalloc would always
   report 0 allocations, which is not correct or useful. I believe there are
   changes to the runtime that need to be made before this will work correctly.
   However, I am including these docs here for completeness.


Similar steps for profiling the compiler can be used for profiling Chapel
programs. The compiler/runtime must be built with the following environment
variables set:

.. code-block:: bash

   export CHPL_HOST_MEM=jemalloc
   export CHPL_HOST_JEMALLOC="bundled"
   export CHPL_JEMALLOC_ENABLE_PROFILING=1

Then, after compiling your Chapel program, you can run it with the following
command:

.. code-block:: bash

   CHPL_JE_MALLOC_CONF='prof:true,prof_prefix:jeprof.out,prof_final:true' \\
     ./myProgram

The resulting profiling data can be analyzed using ``jeprof`` in a similar way
as described above for the compiler:

.. code-block:: bash

   JEMALLOC_SUBDIR=$(./util/chplenv/chpl_jemalloc.py --target --cfg-path)
   ./third-party/jemalloc/install/$JEMALLOC_SUBDIR/bin/jeprof \\
     --text --cum ./myProgram path/to/profile/data/jeprof.out
