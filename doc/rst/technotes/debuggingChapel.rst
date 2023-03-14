.. _readme-debugging-chapel:

======================================================
Debugging Chapel Source Code: Best Known Configuration
======================================================

The Chapel compiler provides a number of mechanisms to make the generated code
more readable in a debugger. There is limited support available for the LLVM
backend so it is recommended to use the C backend. 

In Summary
==========

1) Build the compiler with ``CHPL_TARGET_COMPILER`` set to ``gnu``:

   .. code-block:: bash

        CHPL_TARGET_COMPILER=gnu make

2) Build the executable from Chapel source code:

   .. code-block:: bash

        chpl -g --target-compiler=gnu --savec <dir> --preserve-inlined-line-numbers --no-munge-user-idents <source_file>

3) Debug the executable:

   .. code-block:: bash

        ./<executable> --gdb

Full Details
============

Building the Compiler
~~~~~~~~~~~~~~~~~~~~~

We recommend building the compiler with ``CHPL_TARGET_COMPILER`` set to ``gnu``
(or ``clang`` if on Mac). See :ref:`readme-chplenv` for more information on
building the compiler.

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

Debugging the Application
~~~~~~~~~~~~~~~~~~~~~~~~~

After building the executable, the best practice is to launch the debugger using
``--gdb`` (or ``--lldb``). See :ref:`readme-debugging` for more information.
