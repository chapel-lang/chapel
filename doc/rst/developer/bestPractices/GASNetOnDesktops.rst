===================================================
Running Chapel Programs with GASNet on your Desktop
===================================================

This document give a few pointers on using Chapel with the **GASNet**
communication layer on your desktop.  For more general information
about using the GASNet communication layer, see ``multilocale.rst`` in
the release docs.


RUNNING
=======

Set **GASNet** environment variables for simulating multiple Chapel
locales on a single workstation as described in ``multilocale.rst``.

GASNet may detect the existence of an Infiniband network even if one
does not exist (due to the inclusion of Infiniband in the Linux
kernel).  If you do not have an Infiniband NIC in your workstation (or
don't want to use it), set ``CHPL_GASNET_CFG_OPTIONS=--disable-ibv``
when you build the GASNet library for your Chapel installation.
Alternatively, you can set ``GASNET_QUIET=yes`` to avoid the GASNet
warning messages from GASNet regarding using the Infiniband conduit.

DEBUGGING
=========
See ``debugging.rst`` regarding debugging with ``CHPL_COMM_USE_GDB`` or
``CHPL_COMM_USE_LLDB``.

Sometimes GASNet issues are general multi-locale issues that can be
recreated by compiling the program with the ``--no-local`` flag and using
``CHPL_COMM=none``.  The ``--no-local`` flag assumes that all variables are
non-local, and thus the compiler inserts communication and
communication-related calls into the code.  During program execution,
the runtime checks to see if the reference is remote (which it will
never be), and if the generated code is correct, the program will
execute without issue.  If the program does not complete as expected,
you can use the ``--gdb`` flags to debug in **gdb** (see ``executing.rst``).

DEBUGGING WITH VALGRIND
=======================

To debug using valgrind, use the following command

.. code-block:: bash

    % valgrind --trace-children=yes --suppressions=$CHPL_HOME/third-party/gasnet/gasnet-src/other/contrib/gasnet.supp

