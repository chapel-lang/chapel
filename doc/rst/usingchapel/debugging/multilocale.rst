==========================
Debugging Multilocale Code
==========================

Debugging multilocale programs is more complex than debugging
single-locale programs. Most techniques for doing this end up boiling down to
running a debugger instance for each locale. See :ref:`these docs
<debugging-with-launchers>` which describe how to achieve this very explicitly
with multiple windows.

The rest of this page describes some other tools that can help with debugging
multilocale code.


chpl-parallel-dbg
-----------------

chpl-parallel-dbg is a tool that is bundled with Chapel to debug multilocale
programs. This tool serves as a wrapper around ``lldb``.

Requirements
~~~~~~~~~~~~

To use ``chpl-parallel-dbg``, all you need to do is run ``make
chpl-parallel-dbg`` in the Chapel source directory. This will build the tool
and place it in the ``bin`` directory next to the Chapel compiler. You can then
use it to debug your multilocale programs.

Usage
~~~~~

After having built your Chapel multilocale Chapel program (see
:ref:`chpl-parallel-dbg-limitations`), you just need to run the program as
follows:

.. code-block:: bash

    ./<program> -nl <numLocales> --lldb

This will launch the program, start up a few ``lldb-server`` instances, and
then wait.

From another window, you can then run the following command to connect to the
``lldb-server`` instances:

.. code-block:: bash

    chpl-parallel-dbg <program> <numLocales>

This will create setup a ``lldb`` session, and create a target per locale. You
can then use ``lldb`` commands to debug the program as you would normally.
However, its important to remember that you are essentially debugging multiple
processes at once (however many locales you specified). This means that when you
step through the code, set breakpoints, print variables, you will only be doing
so for the current locale. To switch to another locale, you can use the
``on <localeNum>`` (an alias for ``target select `<localeNum>``) command in
``lldb`` to switch to a different target.

For example, to switch to locale 1 and print the value of a variable called
``x``, you would run:

.. code-block:: bash

    (lldb) on 1
    (lldb) print x

.. _chpl-parallel-dbg-limitations:

Limitations
~~~~~~~~~~~

* ``chpl-parallel-dbg`` currently only works with ``CHPL_COMM=gasnet``.

* ``chpl-parallel-dbg`` has only been tested with ``CHPL_COMM_SUBSTRATE=smp`` and
  ``CHPL_COMM_SUBSTRATE=udp`` (running oversubscribed on a single node).

* Running large numbers of locales can become unwieldy, so it is recommended to
  use this tool with a small number of locales.

* ``chpl-parallel-dbg`` currently only supports ``CHPL_TARGET_PLATFORM=linux64``.

gdb4hpc
-------

gdb4hpc_ is a proprietary tool offered by HPE for debugging distributed
programs. It is mostly used for debugging MPI programs, but can in theory be
used for debugging Chapel programs as well.

.. _gdb4hpc: https://cpe.ext.hpe.com/docs/latest/debugging-tools/gdb4hpc/man/gdb4hpc.1.html
