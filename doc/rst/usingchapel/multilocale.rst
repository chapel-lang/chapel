.. _readme-multilocale:

============================
Multilocale Chapel Execution
============================

This document outlines the steps to get started with multilocale Chapel. Chapel provides several multilocale configurations. See :ref:`platforms-comm-layers` for more information on the different communication layers available in Chapel.

Steps 2-3 describe how to build a multilocale Chapel, and steps 4-6 cover
compiling and running multilocale Chapel programs.

0. Check for instructions more relevant to your platform in
   :ref:`platform-specific <platforms-index>` documentation.

#. Inspect your current communication configuration:

   .. code-block:: bash

     $CHPL_HOME/util/printchplenv

#. Configure the Chapel runtime appropriately.

   For GASNet:

   .. code-block:: bash

     export CHPL_COMM=gasnet

   Then, consider changing these variables:

     * :ref:`CHPL_LAUNCHER <readme-launcher>` controls job launch
     * :ref:`CHPL_COMM_SUBSTRATE <set-comm-conduit>` selects a GASNet conduit
     * :ref:`CHPL_GASNET_SEGMENT <set-comm-segment>` indicates a memory segment

   For libfabric:

   .. code-block:: bash

     export CHPL_COMM=ofi

   Then, consider changing these variables:

     * :ref:`CHPL_LAUNCHER <readme-launcher>` controls job launch
     * :ref:`CHPL_RT_COMM_OFI_PROVIDER <readme-libfabric-providers>` forces a specific libfabric provider
     * :ref:`CHPL_COMM_OFI_OOB <set-comm-ofi-oob>` selects the out-of-band communication method

   Note that the runtime libraries used by the Chapel compiler are
   based on these settings.

#.
   .. _remake-the-compiler:

   Re-make the compiler and runtime from ``CHPL_HOME`` (see :ref:`readme-building`):

   .. code-block:: bash

     cd $CHPL_HOME
     make

#. Compile your Chapel program as usual:

   .. code-block:: bash

     chpl -o hello $CHPL_HOME/examples/hello6-taskpar-dist.chpl

#. Set any environment variables necessary to control the launcher.
   See :ref:`readme-launcher` or documentation for your platform.

   For example, to specify which servers the UDP conduit should use as
   described in :ref:`using-udp`:

   .. code-block:: bash

     export GASNET_SPAWNFN=S
     export GASNET_SSH_SERVERS="host1 host2 host3 ..."  # or SSH_SERVERS

#. Specify the number of locales on the command line. For example:

   .. code-block:: bash

     ./hello -nl 2

   runs our Hello World program on 2 locales.

What is this _real program?
+++++++++++++++++++++++++++

When you compile a Chapel program for multiple locales, you should
typically see two binaries (e.g., hello and hello_real).  The first
binary contains code to launch the Chapel program onto the compute nodes
as specified by the :ref:`CHPL_LAUNCHER <readme-launcher>` variable. The
second contains the program code itself. The ``-v`` and ``--dry-run``
command line options are a good way to learn about what the launcher is
doing.

Co-locales
++++++++++

On some platforms Chapel can run multiple locales on the same node without
oversubscription (i.e., without sharing cores). For example, on a node with
multiple sockets performance may be improved by running one locale in each
socket to avoid inter-socket memory latencies. We refer to this functionality
as *co-locales*. Chapel supports co-locales in the
following configurations:

=========   =============
CHPL_COMM   CHPL_LAUNCHER
=========   =============
gasnet      gasnetrun_*
gasnet      pbs-gasnetrun_ibv
gasnet      slurm-gasnetrun_*
gasnet      slurm-srun
gasnet      smp
ofi         slurm-srun
=========   =============

There are two ways to create co-locales. The first is to set the
``CHPL_RT_LOCALES_PER_NODE`` environment variable. If set, Chapel will run
the specified number of locales per node. The second way to create co-locales
is to use the command-line argument ``-nl NxLt``, where ``N`` is the number
of nodes, ``L`` is the number of locales per node, and ``t`` is an optional
suffix indicating the architectural feature to which the co-locales should be
bound. The ``L`` is optional; if it isn't specified then Chapel will run
the "ideal" number of locales based on the node architecture. Currently this
is limited to the value of ``CHPL_RT_LOCALES_PER_NODE``; in future releases
we plan to include more sophisticated heuristics such as automatically
running one locale per socket on nodes with multiple sockets.

.. note::

   The ``-nl NxLt`` syntax is considered unstable and may change in the
   future.

By default, Chapel will try to bind co-locales to an architectural feature.
For example, launching a Chapel program with the argument ``-nl 1x2`` on a
node with two sockets will bind each co-locale to its own socket. Chapel
looks at the number of sockets, NUMA domains, caches, and cores on the node,
in that order, to determine if the co-locales can be bound to an
architectural feature. If the number of co-locales requested does not match
the number of any feature then Chapel simply assigns an equal number of cores
to each co-locale. Any remaining cores are unused.

You can force Chapel to bind co-locales to an architectural feature with a
suffix to the ``-nl`` argument. The valid suffixes and their bindings are:

===========   =============
Suffix        Binding
===========   =============
s or socket   socket
numa          NUMA domain
llc           last-level cache
c or core     core
===========   =============

It is an error to specify a number of co-locales greater than the number of
the specified architectural feature. For example, specifying ``-nl 1x2s`` on a
node with a single socket is an error. Any remaining cores are
unused; for example, specifying ``-nl 1x1s`` on a node with two sockets
will leave the cores in one socket unused.

Troubleshooting
***************

If you are trying to debug job launch, try adding ``-v`` or
``--dry-run`` to your program's command-line to see the command(s)
that the Chapel launcher is executing to get things running.

See :ref:`readme-gasnet-troubleshooting` for additional tips on debugging GASNet.

.. _set-comm-debugging:

Advanced users may also want to set ``CHPL_COMM_DEBUG`` in order to
enable GASNet's internal sanity checking (it is off by default).  Note
that you'll need to re-build GASNet and runtime when changing this
setting (i.e., re-run the ``make`` command you used for your initial
Chapel install).

