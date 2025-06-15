.. _readme-gasnet:

========================
Using Chapel with GASNet
========================

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
    (see :ref:`readme-gasnet-mpi`)
smp
    Simulates multiple locales on a single shared-memory machine
    (see :ref:`readme-gasnet-smp`)
ofi
    OpenFabrics Interface (libfabric) for Slingshot and Omni-Path.

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
  This option is not supported with the ``smp`` or ``ucx`` substrates.
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

.. _readme-gasnet-smp:

Multilocale Execution with Shared-Memory
++++++++++++++++++++++++++++++++++++++++

When running Chapel programs with ``CHPL_COMM=gasnet``, you can use the
``smp`` conduit to simulate multilocale execution on a single shared-memory
machine. This allows you to run Chapel programs that are designed for
multilocale execution without needing a distributed system. To do this,
set the environment variable ``CHPL_COMM_SUBSTRATE`` to ``smp`` before
running your Chapel program:

.. code-block:: bash

    export CHPL_COMM=gasnet
    export CHPL_COMM_SUBSTRATE=smp

Using Chapel in this way will partition the machine's resources into
multiple locales.

.. _readme-gasnet-emulating-multilocale:

Emulating Multilocale Execution with the UDP Conduit
++++++++++++++++++++++++++++++++++++++++++++++++++++

While the `UDP conduit <using-udp>`_ is primarily intended for portable multilocale execution over ethernet, it can be modified to emulate multiple locales locally by `oversubscribing <oversubscribed-execution>`_ the machine's resources.

With a build of Chapel with ``CHPL_COMM=gasnet`` and
``CHPL_COMM_SUBSTRATE=udp``, you can run a Chapel program on multiple locales
on a single machine with the following environment variable settings:

.. code-block:: bash

    export GASNET_SPAWNFN=L
    export GASNET_ROUTE_OUTPUT=0
    export GASNET_MASTERIP=127.0.0.1
    export GASNET_WORKERIP=127.0.0.0
    export CHPL_RT_OVERSUBSCRIBED=yes

See the documentation for the `UDP conduit <using-udp>`_ for more details on
what these environment variables do.

.. _readme-gasnet-mpi:

Using the GASNet MPI Conduit
++++++++++++++++++++++++++++++++++++

To use the MPI directly with GASNet, you must set the
``CHPL_COMM_SUBSTRATE`` environment variable to ``mpi`` and rebuild Chapel. This will
configure Chapel to use the MPI conduit of GASNet, which allows you to
run Chapel programs using the MPI library for communication between locales.

For best practices about how to configure/use GASNet to avoid
such conflicts with MPI, please see the `GASNet docs for the MPI Conduit <https://bitbucket.org/berkeleylab/gasnet/src/master/mpi-conduit/README>`_
(also available at
``$CHPL_HOME/third-party/gasnet/gasnet-src/mpi-conduit/README``).

.. _readme-gasnet-troubleshooting:

Troubleshooting
***************

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

