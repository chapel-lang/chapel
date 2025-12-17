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

To use AddressSanitizer with Chapel-generated executables only:

.. code-block:: bash

     export CHPL_TARGET_MEM=cstdlib
     export CHPL_SANITIZE_EXE=address
     export ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"

     cd $CHPL_HOME
     make
     chpl <program.chpl>
     ./<program>

To use it with both the Chapel compiler and its generated executables:

.. code-block:: bash

     export CHPL_TARGET_MEM=cstdlib
     export CHPL_HOST_MEM=cstdlib
     export CHPL_SANITIZE=address
     export ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"

     cd $CHPL_HOME
     make
     chpl <program.chpl>
     ./<program>

To get better stack traces when optimizations are enabled:

.. code-block:: bash

     export DEBUG=1


.. note::

     Non-chplenv environment variables aren't propagated by paratest. So,
     to turn off leak checking, it is necessary to either pass
     ``-env ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"`` or to include
     ``export ASAN_OPTIONS="use_sigaltstack=0,detect_leaks=0"`` in .bashrc or
     the equivalent.

See :ref:`readme-sanitizers` for more information on using AddressSanitizer with
Chapel programs. That page also lists various limitations of AddressSanitizer
with Chapel programs, which also apply to the use of AddressSanitizer for
debugging the Chapel compiler itself.


UBSan
-----

We have also find utility in using an undefined behavior sanitizer (UBSan) to
help catch certain classes of bugs in the runtime resulting from undefined
behavior in C/C++. This works best with the C backend.

For example, the following can catch many kinds of integer overflows and
invalid pointers.

.. code-block:: bash

    export CHPL_TARGET_COMPILER=gnu
    export CHPL_TARGET_MEM=cstdlib
    export CHPL_SANITIZE_EXE=undefined
    export UBSAN_OPTIONS="print_stacktrace=1"

    cd $CHPL_HOME
    make
    chpl <program.chpl>
    ./<program>


You can also make use of this without needing to rebuild the runtime. This will
not detect errors in the runtime library itself, but can catch errors in Chapel
code (including internal modules).

.. code-block:: bash

    export CHPL_TARGET_COMPILER=gnu
    export UBSAN_OPTIONS="print_stacktrace=1"

    chpl <program.chpl> --ccflags=-fsanitize=undefined --ldflags=-fsanitize=undefined
    ./<program>
