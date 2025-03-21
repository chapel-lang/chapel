.. _readme-multilocale:

============================
Multilocale Chapel Execution
============================

This document outlines the steps to get started with multilocale Chapel using
GASNet-based communication.  This configuration is fully functional on every
platform that supports multilocale Chapel.  However, there are also other
communication configurations that work in specific situations.  For
instructions on using the OpenFabrics Interface's libfabric-based
``ofi`` communication layer---the preferred option on HPE Cray EX
systems---see :ref:`readme-libfabric`.  On Cray XC systems, using
native ``ugni`` communication as described in :ref:`Using Chapel on
Cray Systems <readme-cray>` typically gives the best performance.

Steps 2-3 describe how to build a multilocale Chapel, and steps 4-6 cover
compiling and running multilocale Chapel programs.

0. Check for instructions more relevant to your platform in
   :ref:`platform-specific <platforms-index>` documentation.

#. Inspect your current communication configuration:

   .. code-block:: bash

     $CHPL_HOME/util/printchplenv

#. Configure the Chapel runtime appropriately. For most configurations, set:

   .. code-block:: bash

     export CHPL_COMM=gasnet

   Then, consider changing these variables:

     * :ref:`CHPL_LAUNCHER <readme-launcher>` controls job launch
     * :ref:`CHPL_COMM_SUBSTRATE <set-comm-conduit>` selects a GASNet conduit
     * :ref:`CHPL_GASNET_SEGMENT <set-comm-segment>` indicates a memory segment

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

.. _what-is-gasnet:

What is GASNet?
+++++++++++++++

GASNet is a one-sided communication and active message library being
developed by Lawrence Berkeley National Laboratory and UC Berkeley.  For
details, refer to the `GASNet website <https://gasnet.lbl.gov/>`_.

.. _set-comm-conduit:

Setting CHPL_COMM_SUBSTRATE
+++++++++++++++++++++++++++

Users can set ``CHPL_COMM_SUBSTRATE`` to indicate the GASNet conduit that
they wish to use. Conduits are alternative implementations of the GASNet
library. GASNet uses different conduits to support different networks.
Novice users can leave this unset and Chapel will make a choice for them.

Most settings for ``CHPL_COMM_SUBSTRATE`` rely on the particular network
hardware. The options include:

ibv
    OpenIB/OpenFabrics Verbs for InfiniBand
    (see :ref:`Using Chapel with InfiniBand <readme-infiniband>`)
udp
    UDP - portable conduit, works on any network with a TCP/IP stack
    (see :ref:`Using the Portable UDP Conduit <using-udp>`)
aries
    Aries for Cray XC series
    (see :ref:`Using Chapel on Cray Systems <readme-cray>`)
mpi
    MPI - portable conduit, works on any network with MPI 1.1 or newer
smp
    Simulates multiple locales on a single shared-memory machine

See the `GASNet website <https://gasnet.lbl.gov/>`_ for more
information on each of these conduits.

Current defaults are:

====================  ===================
CHPL_TARGET_PLATFORM  CHPL_COMM_SUBSTRATE
====================  ===================
cray-cs                ibv
cray-xc                aries
pwr6                   ibv
other                  udp
====================  ===================

.. _set-comm-segment:

Setting CHPL_GASNET_SEGMENT
+++++++++++++++++++++++++++

Users can set ``CHPL_GASNET_SEGMENT`` to choose a memory segment to use
with GASNet. A GASNet segment is a region of memory that is expected to
be used for remote memory access. The GASNet library works to make memory
in this segment available for accelerated memory access supported
directly by network hardware.  The options are:

everything
  All memory is available for remote memory access.
fast
  A limited portion of memory is available and optimized for fastest remote
  memory access
large
  As with fast, but a larger amount of memory is available for communication

Each choice of segment has different tradeoffs. For the ``fast`` segment,
the Chapel heap is entirely in memory that can be directly accessed over
the network, but the drawback is that the size of the heap must be
specified at program start-up. For ``everything``, accessing memory over
the network is generally slower, but there is no need to worry about the
size of the heap or whether some memory is registered with the network.
The ``large`` segment offers a compromise between these two options,
where some portion of the heap will be available for faster network
access.

Current defaults are:

===================  ====================
CHPL_COMM_SUBSTRATE  CHPL_GASNET_SEGMENT
===================  ====================
aries                fast
ibv                  large
smp                  fast
other                everything
===================  ====================

.. _readme-colocale:

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
+++++++++++++++

If you are trying to debug job launch, try adding ``-v`` or
``--dry-run`` to your program's command-line to see the command(s)
that the Chapel launcher is executing to get things running.

For ``CHPL_COMM=gasnet`` runs, you may also want to consider setting
one or both of:

  .. code-block:: bash

    export GASNET_SPAWN_VERBOSE=1
    export GASNET_VERBOSEENV=1

where the first prints more information about GASNet's job launch
actions, and the second is used to audit environment variable
settings.

When running ``CHPL_COMM=gasnet`` programs using the ``udp`` conduit,
we've had best results with console I/O using:

  .. code-block:: bash

    export GASNET_ROUTE_OUTPUT=0

.. _set-comm-debugging:

Advanced users may also want to set ``CHPL_COMM_DEBUG`` in order to
enable GASNet's internal sanity checking (it is off by default).  Note
that you'll need to re-build GASNet and runtime when changing this
setting (i.e., re-run the ``make`` command you used for your initial
Chapel install).

