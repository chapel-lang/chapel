.. index::
    single: debug
    single: debugging
    single: debugging; gdb
    single: debugging; lldb
.. _readme-debugging:

=========================
Debugging Chapel Programs
=========================

Chapel compiler-generated executables have a flag to launch the program
in a debug session. ``--gdb`` launches the program in a ``gdb`` session, while
``--lldb`` launches it in an ``lldb`` session. For best results, you
should read :ref:`this section <readme-debugging-bkc>` to build Chapel and
build your application.

You can also just run the program with ``gdb`` or ``lldb`` directly. This
may be useful, for example if you want to use a graphical debugger like VSCode.
``--gdb`` and ``--lldb`` are just convenience flags that do the following:

* Source the command files to setup the debugger for Chapel

  * For GDB: ``source $CHPL_HOME/runtime/etc/debug/gdb.commands``

  * For LLDB: ``command source $CHPL_HOME/runtime/etc/debug/lldb.commands``

* LLDB only: Enable the Chapel pretty-printer for LLDB, i.e.
  ``command script import $CHPL_HOME/runtime/etc/debug/chpl_lldb_pretty_print.py``

* If specified, execute additional debugger commands from a file whose path is
  set in ``CHPL_RT_DEBUGGER_CMD_FILE``

If you are not using ``--gdb`` or ``--lldb``, make sure to replicate the
above steps in your debugger of choice for the best experience.

.. note::

   GDB/LLDB will not know about ``$CHPL_HOME``, you will need to expand that
   variable to the actual path.

The following sections provide more information on debugging Chapel programs:

.. toctree::
   :maxdepth: 1
   :glob:

   debugging/bkc
   debugging/launchers
   debugging/multilocale
   debugging/memory
   debugging/tasks
   debugging/sanitizers

Useful Debug Features
---------------------

Pretty Printing (LLDB only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Chapel pretty-printer for LLDB is automatically loaded when using the
``--lldb`` flag. If you are using LLDB without this flag, you can load the
pretty-printer manually by running the following command in LLDB:

.. code-block:: bash

    command script import $CHPL_HOME/runtime/etc/debug/chpl_lldb_pretty_print.py

This pretty-printer understands a number of builtin Chapel types. This
overrides the default printing for many types when using ``p`` (``print``) or
``v`` (``frame variable``). To circumvent this, you can use ``v -R`` (``frame
variable -R``) to print the raw value of a variable without the pretty-printer.

Using a Graphical Debugger
~~~~~~~~~~~~~~~~~~~~~~~~~~

The Chapel VSCode extension provides a graphical debugging experience with
either ``gdb`` or ``lldb``. See :ref:`vscode-debugging` for more information.

The `Debugger.breakpoint` statement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :chpl:mod:`Debugger` module provides a parenless function called
:chpl:proc:`~Debugger.breakpoint`. This will cause an attached debugger to
automatically stop at calls to this function as a breakpoint.

.. note::

   This requires ``b debuggerBreakHere`` to be set in the debugger, which is
   done automatically by the ``--gdb`` and ``--lldb`` flags. If you need a true
   debug trap, see the :chpl:proc:`~Debugger.debugTrap` function.


----

We continue to improve our debugging support for Chapel programs. If you
find cases where the debugging experience could be improved,
`please let us <https://github.com/chapel-lang/chapel/issues/new/choose>`_
know so that we can prioritize accordingly.
