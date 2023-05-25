.. _best-practices-sanitizers:

Sanitizers
==========

Sanitizers are a compiler feature that support instrumenting programs to do
dynamic analysis to catch many classes of bugs at runtime.

AddressSanitizer (ASan) is a fast memory error detector. It consists of a
compiler instrumentation module and a run-time library. ASan is similar
to valgrind (:ref:`best-practices-valgrind`) in that it can help
identify memory errors. ASan is much faster than valgrind, but does
require recompilation. Note that only GCC and Clang support sanitizers.

How-to
------

To use AddressSanitizer with Chapel (compiler and executables):

.. code-block:: bash

     export CHPL_MEM=cstdlib
     export CHPL_HOST_MEM=cstdlib
     export CHPL_TASKS=fifo
     export CHPL_LLVM=none
     export CHPL_SANITIZE=address
     export ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"

     cd $CHPL_HOME
     make
     chpl <program.chpl>
     ./<program>

To only sanitize executables and not the compiler:

.. code-block:: bash

     unset CHPL_SANITIZE
     export CHPL_SANITIZE_EXE=address

To get better stack traces when optimizations are enabled:

.. code-block:: bash

     export DEBUG=1


.. note::

     Non-chplenv environment variables aren't propagated by paratest. So,
     to turn off leak checking, it is necessary to either pass
     ``-env ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"`` or to include
     ``export ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"`` in .bashrc or
     the equivalent.

Limitations
-----------

AddressSanitizer should be able to detect almost all memory errors for single
locale configurations using ``CHPL_COMM=none``. However, ASan cannot detect all
classes of memory errors in multilocale configurations and the extent of the
support depends on the ``CHPL_COMM`` setting. As a specific example some
configurations will not be able to detect invalid remote reads/writes since
ASan only knows about local memory. In the following program the invalid write
to the array may not be detected for configurations that use RDMA to perform
writes.

.. code-block:: chapel

   var A: [1..10] int;
   on Locales[numLocales-1] {
     A[11] = 11; // Out of bounds write
   }

This particular example would be caught by Chapel bounds checking, which are
enabled by default but disabled with ``--fast``. To get bounds checking and
optimizations you can use ``--fast --bounds-checks`` or ``--fast --checks``.

``CHPL_COMM=gasnet`` with ``CHPL_COMM_SUBSTRATE=udp`` should be able to detect
most memory errors since remote reads/writes are performed with active messages
instead of RDMA.

``CHPL_COMM=ugni`` without hugepages can detect some, but not all invalid
remote reads/writes, but comes with a large performance cost. Hugepages can be
unloaded with ``module unload $(module -t list 2>&1 | grep craype-hugepages)``


Configuration Limitations
-------------------------

The above options are needed because not all third-party libraries support
sanitizers. In particular:

- Sanitizer instrumentation is added by the C compiler, so LLVM
  compilations don't currently work
- Sanitizers hook into the system allocator, so using ``jemalloc`` is not
  supported
- ``qthreads`` performs task-switching in user-space, which throws off
  stack frame tracking. We expect to be able to resolve this in the
  future.
- By default the gcc address sanitizer will enable leak checking, but
  Chapel intentionally leaks some memory in the runtime, so we disable
  that tracking for now. See :ref:`readme-debugging` for more info about
  debugging memory leaks in Chapel.
- An upstream bug_ can result in false-positives for some gcc versions.
  ``use_sigaltstack=0`` works around this.

  .. _bug: https://gcc.gnu.org/bugzilla//show_bug.cgi?id=101476


Other Sanitizers
----------------

Currently, only AddressSanitizer has been tested, but the options passed to
``CHPL_SANITIZE`` are passed directly to the backend compiler's ``-fsanitize=``
option, so other sanitizers can be enabled. e.g. to use memory sanitizer and
address sanitizer set ``CHPL_SANITIZE=address,memory``
