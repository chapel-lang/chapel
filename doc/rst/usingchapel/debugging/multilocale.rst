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

:ref:`chpl-parallel-dbg <readme-chpl-parallel-dbg>` is a wrapper around ``lldb``
that makes it easier to debug multilocale code. Check the documentation page of
the tool for more information on its usage and best practices.

gdb4hpc
-------

gdb4hpc_ is a proprietary tool offered by HPE for debugging distributed
programs. It is mostly used for debugging MPI programs, but can in theory be
used for debugging Chapel programs as well.

.. _gdb4hpc: https://cpe.ext.hpe.com/docs/latest/debugging-tools/gdb4hpc/man/gdb4hpc.1.html
