.. _readme-libfabric:

============================
Using Chapel with libfabric
============================

This document describes how to run Chapel across multiple machines using
the OpenFabrics Interfaces libfabric-based ``ofi`` communication layer.
:ref:`readme-multilocale` gives general information about running Chapel
in a multilocale configuration.

.. note::
  The ofi communication layer is new as of the Chapel 1.19 release.  It is
  complete in terms of initial development and passes Chapel testing, but
  it is still something of an early-access configuration in the sense that
  users may need to supply supporting packages and performance may leave
  something to be desired.

Building Chapel with the ofi Communication Layer
++++++++++++++++++++++++++++++++++++++++++++++++

1. Make general, non-communication Chapel configuration settings as
   described in :ref:`readme-chplenv`.

#. Configure the Chapel runtime to select the ofi communication layer

   .. code-block:: bash

     export CHPL_COMM=ofi

#. Set the variable indicating the path to a libfabric installation.
   It may be possible to skip this step, if your system has libfabric
   already installed and your target compiler can find its include and
   library files itself.  But this is not common, so you will probably
   need to do:

   .. code-block:: bash

     export LIBFABRIC_DIR=<Place where libfabric is installed>

   The ``<Place where libfabric is installed>`` should be a directory with
   an ``include/rdma`` subdirectory that contains the libfabric include
   files and a ``lib`` subdirectory that contains the libfabric library
   files.  If your system does not have those installed already, you will
   need to download libfabric and possibly build it.

   Note: On a Mac OS X system, libfabric can be obtained through
   Homebrew_ with the following command.

   .. code-block:: bash

     brew install libfabric

#. Select a launcher.  On Cray XC systems you can skip this step,
   because on those systems the automatically-selected ``aprun`` or
   ``srun`` launcher settings will work with the ofi communication
   layer.  But on other systems, select the Chapel ``mpirun4ofi``
   launcher.  For more information see `The mpirun4ofi Launcher`_,
   below.


   .. code-block:: bash

     export CHPL_LAUNCHER=mpirun4ofi


#. Having done this, set the variable indicating the path to an OpenMPI
   installation.  It may be possible to skip this step, if your system
   has OpenMPI already installed and your target compiler can find its
   include and library files itself.  But this is not common, so you
   will probably need to do:

   .. code-block:: bash

     export MPI_DIR=<Place where OpenMPI is installed>

   The ``<Place where OpenMPI is installed>`` should be a directory with
   an ``include`` subdirectory that contains the OpenMPI include files
   and a ``lib`` subdirectory that contains the OpenMPI library files.
   If your system does not have those installed already, you will need
   to download OpenMPI and possibly build it.

   .. note::
     In the future we hope both to be able to support the MPICH MPI
     package in addition to supporting the OpenMPI MPI package, and to use
     the regular Chapel ``mpirun`` launcher with the ofi communication
     layer, but for now OpenMPI and mpirun4ofi are the only options on
     platforms other than Cray XC systems.

   Note: On a Mac OS X system, OpenMPI can be obtained through Homebrew_
   with the following command.

   .. code-block:: bash

     brew install open-mpi

#. Re-make the compiler and runtime from ``CHPL_HOME`` (see
   :ref:`readme-building`).

   .. code-block:: bash

     cd $CHPL_HOME
     make

#. Now you are ready to compile and run programs.
   Compile your Chapel program as usual.

   .. code-block:: bash

     chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl

#. Optionally set any environment variables necessary during execution
   (see below) and run, specifying the number of locales on the command
   line.  For example, this runs the ``hello6-taskpar-dist`` example
   program on 2 locales:

   .. code-block:: bash

     ./hello6-taskpar-dist -nl 2


Execution Environment
+++++++++++++++++++++

Libfabric Providers
___________________


Libfabric defines an abstract network and operations on it, and
so-called *providers* within libfabric define the concrete instances of
the network and operations.  The provider used by a program is selected
at execution time.  The ofi communication layer has been tested with 3
different providers:

  gni
    The ``gni`` provider works only on Cray XC systems.  It is built on
    the Cray native uGNI library and communicates over the Cray
    proprietary Aries network interface.  This is the default provider
    on Cray XC systems.  Note that the libfabric gni provider itself is
    something of a work-in-progress, and Chapel performance using
    libfabric and gni will probably never match what can be achieved
    using the native ugni communication layer.

  sockets
    The ``sockets`` provider works on all platforms.  It is built on
    POSIX sockets and communicates over any network interface on which
    the OS can provide sockets support.  This is the default provider on
    all systems other than Cray XC.  The sockets provider is fully
    functional, indeed to the extent libfabric has a reference provider
    the sockets provider is it, but its emphasis is definitely
    functionality rather than performance.

  verbs
    The ``verbs`` provider works on any system with verbs-based network
    hardware (Infiniband, iWarp, etc.).  It is built on the Linux Verbs
    API.

    (Note for libfabric devotees: when the verbs provider is specified to
    the ofi communication layer as described below, what is actually
    used is the ``verbs;ofi_rxm`` provider, which is the verbs provider
    plus a utility provider which supports reliable datagrams for remote
    memory access operations.)

The ``CHPL_RT_COMM_OFI_PROVIDER`` environment variable can be set to
force use of a provider other than the default.  In particular, it can
force use of the sockets provider on Cray XC systems, or the verbs
provider on verbs-based systems where the default would otherwise be the
sockets provider.  For example, the following would force use of the
verbs provider:

   .. code-block:: bash

     export CHPL_RT_COMM_OFI_PROVIDER=verbs

The Chapel group has done full testing both on a Cray XC system with the
gni and sockets providers, and on an InfiniBand-based system with the
sockets and verbs providers.  Some additional testing has been done with
the sockets provider on a MacBook running Mac OS X.  All of these
configurations are expected to work.  Provider settings we have not
tested with the ofi communication layer may lead to internal errors.
Settings which are at odds with the available networks, such as
specifying the gni provider on a vanilla Linux cluster, will definitely
lead to internal errors.

As the ofi communication layer evolves toward completion we expect to
move from the current name-based technique for selecting the provider to
a more capability-based one.  Users will probably still be able to force
use of a particular provider by naming it, but the need to do so for
other than curiosity's (or performance comparison's) sake should be
reduced.

The gni Provider, Memory Registration, and the Heap
___________________________________________________

(Before you get any further into this section, you should probably
re-read the note above about performance being better with the native
ugni communication layer than with the ofi communication layer and gni
provider.)

Network technologies sometimes require *memory registration*, meaning
that ranges of memory which will be the source or target of
communication operations must be made known to the network before any
such operations can occur.  When the ofi communication layer is used
with the gni provider, memory has to be registered.  This has certain
implications for users, the most notable being that the heap must have a
fixed size.

The *heap* is an area of memory used for dynamic allocation of
everything from user data to task stacks to internal management data
structures.  When memory must be registered, the ofi communication layer
needs to know the maximum size the heap will grow to during execution.
The default heap size is 16 GiB, but you can change this by setting the
``CHPL_RT_MAX_HEAP_SIZE`` environment variable.  Set it to a positive
number for the desired heap size in bytes optionally followed by ``k``
or ``K`` for KiB, ``m`` or ``M`` for MiB, ``g`` or ``G`` for GiB, or to
a positive integer followed by ``%`` to indicate a percentage of the
node real memory.  Either ``CHPL_RT_MAX_HEAP_SIZE=12g`` or ``=20%``
specifies roughly a 12 GiB heap on a 64 GiB compute node, for example.

We have not yet quantified the effects, but performance with the gni
provider may be improved if you have a ``craype-hugepages`` module
loaded both when you build your program and when you run it.  For
example::

     module load craype-hugepages16M

See :ref:`ugni-comm-on-cray` for more discussion about hugepages,
hugepage modules, and the heap size.  Note, however, that anything there
about a dynamically sized heap does not apply to the ofi communication
layer and the libfabric gni provider.

.. note::
  In the future we hope to be able to reduce the user impact of memory
  registration when using the ofi communication layer.

.. _mpirun4ofi-launcher:

The mpirun4ofi Launcher
_______________________

Programs built with the ofi communication layer on Cray XC systems can
use the existing launchers.  On other systems, for now they must use the
``mpirun4ofi`` launcher, which is a provisional, thin wrapper around
OpenMPI ``mpirun``.

The mpirun4ofi launcher can run libfabric-based Chapel programs either
with or without slurm.  Outside of a slurm job, it will run all of the
per-locale Chapel program instances directly on the launch node.  In
this situation you should be sure to follow the guidance in
:ref:`overloading-with-qthreads` if you are using Qthreads-based tasking.
Within a slurm job, the mpirun4ofi launcher will arrange for the
per-locale Chapel program instances to be distributed in a cyclic manner
across the nodes assigned to the job.  Overloading can still be an issue
if there are more Chapel locales (program instances) than nodes in the
slurm job, however.


.. _Homebrew: https://github.com/Homebrew/brew
