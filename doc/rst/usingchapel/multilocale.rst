.. _readme-multilocale:

============================
Multilocale Chapel Execution
============================

This document outlines the steps to get started with multilocale Chapel using
GASNet-based communication.  This configuration is fully functional on every
platform that supports multilocale Chapel.  However, there are also other
communication configurations that work in specific situations.  On Cray
XC systems, using native communication as described in :ref:`Using
Chapel on Cray Systems <readme-cray>` will probably give the best performance.
For instructions on using the preliminary OpenFabrics Interfaces
libfabric-based ``ofi`` communication layer, see :ref:`readme-libfabric`.

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
     export GASNET_SSH_SERVERS="host1 host2 host3 ..."

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
second contains the program code itself. The ``-v`` command line option
is a good way to learn about what the launcher is doing.

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

Troubleshooting
+++++++++++++++

If you are trying to debug job launch, try adding ``-v`` to your
program's command line and set:

  .. code-block:: bash

    export GASNET_VERBOSEENV=1

We've had best results with console I/O using:

  .. code-block:: bash

    export GASNET_ROUTE_OUTPUT=0

but your mileage may vary.

.. _set-comm-debugging:

Advanced users may want to set ``CHPL_COMM_DEBUG`` in order to enable
GASNet's internal sanity checking. (It is off by default.)
Note that you'll need to re-make GASNet and runtime when changing
this setting.


