.. _readme-knc:

==============================
Using Chapel on Intel Xeon Phi
==============================

The following information is assembled to help Chapel users get up and
running on Intel Xeon Phi, Knights Corner (KNC).

The initial implementation runs in self-hosted mode only.  That is,
the entire Chapel program runs on the KNC co-processor.  For
multilocale, GASNet must use the MPI conduit (segment everything) as
all communication must bounce through the host processor.

-------------------
Current Limitations
-------------------

There are currently a number of limitations for KNC builds:

- Only the Intel compiler supports compilation for KNC:

  ``CHPL_TARGET_COMPILER=intel`` or ``CHPL_TARGET_COMPILER=cray-prgenv-intel``

- Only non-native comm substrates can be used with KNC:

  ``CHPL_COMM_SUBSTRATE=mpi`` or ``CHPL_COMM_SUBSTRATE=udp``

- Additionally the following configurations are not supported:

  =================  ================================
  Configuration      Reason
  =================  ================================
  CHPL_MEM=tcmalloc  lfence not supported
  CHPL_REGEXP=re2    sfence not supported
  CHPL_HWLOC=hwloc   undiagnosed build issues
  CHPL_GMP=system    system version not built for KNC
  =================  ================================


---------------
Getting started
---------------

For vanilla Intel compiler:
---------------------------

- Set ``CHPL_TARGET_COMPILER=intel`` and ``CHPL_TARGET_ARCH=knc``.

- Build Chapel as normal.  Running ``printchplenv`` should look something
  like this::

    CHPL_HOST_PLATFORM: linux64
    CHPL_HOST_COMPILER: gnu
    CHPL_TARGET_PLATFORM: linux64
    CHPL_TARGET_COMPILER: intel
    CHPL_TARGET_ARCH: knc
    CHPL_LOCALE_MODEL: flat
    CHPL_COMM: none
    CHPL_TASKS: fifo
    CHPL_LAUNCHER: none
    CHPL_TIMERS: generic
    CHPL_MEM: cstdlib
    CHPL_MAKE: gmake
    CHPL_ATOMICS: intrinsics
    CHPL_GMP: none
    CHPL_HWLOC: none
    CHPL_REGEXP: none
    CHPL_WIDE_POINTERS: struct
    CHPL_LLVM: none
    CHPL_AUX_FILESYS: none

- Build your Chapel program as normal::

    chpl hello.chpl

For Cray XC:
------------

For Cray machines, only the ``aprun`` launcher is supported.  In addition,
``CHPL_TASKS=muxed`` and ``CHPL_COMM=ugni`` are not supported.

- Load the proper modules. The order in which you do these module
  commands matters.

  - First, swap in the Intel PE module.  For example, if you have the
    Cray PE module loaded::

      module swap PrgEnv-cray PrgEnv-intel

  - Unload ``libsci``, ``atp`` (unsupported) and any ``craype`` processor type modules::

      module unload cray-libsci atp craype-ivybridge

  - Load the KNC ``craype`` module::

      module load craype-intel-knc

  - Unload any ``craype-hugepages`` module you have loaded, for example::

      module unload craype-hugepages2M

- Build Chapel as normal.  Running ``printchplenv`` should look something
  like this::

    CHPL_HOST_PLATFORM: cray-xc
    CHPL_HOST_COMPILER: gnu
    CHPL_TARGET_PLATFORM: cray-xc
    CHPL_TARGET_COMPILER: cray-prgenv-intel
    CHPL_TARGET_ARCH: knc
    CHPL_LOCALE_MODEL: flat
    CHPL_COMM: gasnet
      CHPL_COMM_SUBSTRATE: mpi
      CHPL_GASNET_SEGMENT: everything
    CHPL_TASKS: fifo
    CHPL_LAUNCHER: aprun
    CHPL_TIMERS: generic
    CHPL_MEM: cstdlib
    CHPL_MAKE: gmake
    CHPL_ATOMICS: intrinsics
      CHPL_NETWORK_ATOMICS: none
    CHPL_GMP: gmp
    CHPL_HWLOC: none
    CHPL_REGEXP: none
    CHPL_WIDE_POINTERS: struct
    CHPL_LLVM: none
    CHPL_AUX_FILESYS: none

- Build your Chapel program as normal::

    chpl hello.chpl

- Request some nodes with KNCs.  You can get the node numbers using
  cnselect:

    .. code-block:: sh

      cnselect -e name.eq.Xeon_Phi

  or:

    .. code-block:: sh

      cnselect -e name.eq.MIC

- Next, use qsub to request the nodes.  For example, to request nodes
  12-16 for interactive using PBSPro:

  .. code-block:: sh

    qsub -I -lmppwidth=5,mppnppn=1,mppnodes=\"12-16\"

- Change to your working directory:

  .. code-block:: sh

    cd $PBS_O_WORKDIR

- Run your program as you would normally.

  .. code-block:: sh

    ./a.out -nl 5

