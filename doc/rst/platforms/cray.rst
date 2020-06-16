.. _readme-cray:

============================
Using Chapel on Cray Systems
============================

The following information is assembled to help Chapel users get up and running
on Cray\ |reg| systems including the Cray XC\ |trade| and Shasta\ |trade|
series systems.

.. contents::


----------------------------------------------------
Getting Started with Chapel on Cray X-Series Systems
----------------------------------------------------

Chapel is available as a module for Cray X-series systems.  When it is
installed on your system, you do not need to build Chapel from the
source release (though you can). To use Chapel with the default settings and
confirm it is correctly installed, do the following:

1) Load the Chapel module::

     module load chapel


2) Compile an example program using::

     chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl


3) Execute the resulting executable (on four locales)::

     ./hello6-taskpar-dist -nl 4


This may be all that is necessary to use Chapel on a Cray X-Series system.
If the installation setup by your system administrator deviates from
the default settings, or you are interested in other configuration
options, see `Using Chapel on a Cray System`_ below.  If instead you wish to
build Chapel from source, continue on to
`Building Chapel for a Cray System from Source`_ just below.

For information on obtaining and installing the Chapel module please
contact your system administrator.


--------------------------------------------------
Getting Started with Chapel on Cray Shasta Systems
--------------------------------------------------

Chapel is available as a module for Cray Shasta systems.  It should be
installed on your system already.  If it is not, contact your system
administrator for information on obtaining and installing the Chapel
module.

To use Chapel with the default settings and confirm it is correctly
installed, do the following:

1) Load the Chapel module::

     module load chapel

   Note that a side effect of loading the chapel module is that these
   other modules will either be loaded or swapped to, as needed::

     PrgEnv-gnu
     cray-mpich
     libfabric

   And this module will be unloaded, if it is loaded::

     cray-libsci


2) Compile an example program like this::

     chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl


3) Execute the resulting executable (on four locales)::

     ./hello6-taskpar-dist -nl 4


Currently the number of Chapel configurations available on
Shasta systems is quite limited.  Only the following have been built
into the module::

  CHPL_TARGET_PLATFORM: cray-shasta
  CHPL_TARGET_COMPILER: cray-prgenv-gnu
  CHPL_TARGET_ARCH: x86_64
  CHPL_TARGET_CPU: sandybridge
  CHPL_LOCALE_MODEL: flat
  CHPL_COMM: none, ofi
  CHPL_TASKS: qthreads
  CHPL_LAUNCHER: none
  CHPL_TIMERS: generic
  CHPL_UNWIND: none
  CHPL_MEM: jemalloc
  CHPL_ATOMICS: cstdlib
    CHPL_NETWORK_ATOMICS: none, ofi
  CHPL_GMP: none
  CHPL_HWLOC: hwloc
  CHPL_REGEXP: none
  CHPL_LLVM: none
  CHPL_AUX_FILESYS: none

You may be able to build Chapel from source on a Shasta system if you do
not have a module already.  Generally you should be able to follow the
instructions below for building from source, but be advised that so far
only the above configurations have been built.  Also, you'll probably
find that the module settings shown in 1) above will be required during
the build.


----------------------------------------------
Getting Started with Chapel on Cray CS Systems
----------------------------------------------

On Cray CS systems, Chapel is not currently available as a module due
to the wide diversity of software packages that Cray CS customers may
choose to install on their system.  For this reason, Chapel must be
built from source on Cray CS systems using the
`Building Chapel for a Cray System from Source`_ instructions just below.


---------------------------------------------
Building Chapel for a Cray System from Source
---------------------------------------------

1) If using an XC system, continue to step 2. If using a CS series
   system, set ``CHPL_HOST_PLATFORM`` to ``cray-cs``.

   For example:

    .. code-block:: sh

      export CHPL_HOST_PLATFORM=cray-cs

   These are the supported systems and strings.  Note that these values
   are used by default when building on the given systems.  They can
   also be set manually.

       =========  ==================
       System     CHPL_HOST_PLATFORM
       =========  ==================
       CS series  cray-cs
       XC series  cray-xc
       =========  ==================


2) Optionally, set the ``CHPL_LAUNCHER`` environment variable to indicate
   how Chapel should launch jobs on your system:

      ========================================  =========================
      On a Cray CS system, to...                set CHPL_LAUNCHER to...
      ========================================  =========================
      ...run jobs interactively on your system  gasnetrun_ibv
      ...queue jobs using SLURM (sbatch)        slurm-gasnetrun_ibv
      ========================================  =========================

      ========================================  =========================
      On a Cray X-series system, to...          set CHPL_LAUNCHER to...
      ========================================  =========================
      ...run jobs interactively on your system  aprun
      ...queue jobs using PBS (qsub)            pbs-aprun
      ...queue jobs using SLURM (sbatch)        slurm-srun
      ========================================  =========================

   You can also set CHPL_LAUNCHER to ``none`` if you prefer to manually
   manage all queuing and job launch commands yourself.

   On Cray CS systems, ``CHPL_LAUNCHER`` defaults to ``gasnetrun_ibv``.

   On Cray X-Series systems, ``CHPL_LAUNCHER`` defaults to ``aprun`` if
   ``aprun`` is in your path, ``slurm-srun`` if ``srun`` is in your path
   and ``none`` otherwise.

   For more information on Chapel's launcher capabilities and options,
   refer to :ref:`readme-launcher`.


3) Select the target compiler that Chapel should use when compiling
   code for the compute node:

   On a Cray CS series system, set the ``CHPL_TARGET_COMPILER`` environment
   variable to indicate which compiler to use (and make sure that the compiler
   is in your path).

      ===========================  ==============================
      To request...                set CHPL_TARGET_COMPILER to...
      ===========================  ==============================
      ...the GNU compiler (gcc)    gnu    (default)
      ...the Intel compiler (icc)  intel
      ===========================  ==============================

   On a Cray X-series system, ensure that you have one of the following
   Programming Environment modules loaded to specify your target compiler::

       PrgEnv-allinea (ARM only)
       PrgEnv-cray
       PrgEnv-gnu
       PrgEnv-intel


4) Make sure you're in the top-level chapel/ directory and make/re-make the
   compiler and runtime::

     gmake

   Note that a single Chapel installation can support multiple
   configurations simultaneously and that you can switch between them
   simply by changing any of the above settings.  However, each
   configuration must be built separately.  Thus, you can change any of
   the settings in the steps before this, and then re-run this step in
   order to create additional installations.  Thereafter, you can switch
   between any of these configurations without rebuilding.


-----------------------------
Using Chapel on a Cray System
-----------------------------

1) If you are working from a Chapel module:

     a) Load the module using ``module load chapel``
     b) Optionally select a launcher, as in step 2 above
     c) Select a target compiler, as in step 3 above

   If you are working from a source installation:

     a) Set your host platform as in step 1 above
     b) Optionally select a launcher, as in step 2 above
     c) Select a target compiler, as in step 3 above
     d) Set ``CHPL_HOME`` and your paths by invoking the appropriate
        ``util/setchplenv`` script for your shell.  For example:

      .. code-block:: sh

        source util/setchplenv.bash


2) Compile your Chapel program.  For example:

   .. code-block:: sh

      chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl

   See :ref:`readme-compiling` or  ``man chpl`` for further details.


3) If ``CHPL_LAUNCHER`` is set to anything other than ``none``, when you
   compile a Chapel program for your Cray system, you will see two
   binaries (e.g., ``hello6-taskpar-dist`` and ``hello6-taskpar-dist_real``).
   The first binary contains code to launch the Chapel program onto
   the compute nodes, as specified by your ``CHPL_LAUNCHER`` setting.  The
   second contains the program code itself; it is not intended to be
   executed directly from the shell prompt.

   You can use the ``-v`` flag to see the commands used by the launcher
   binary to start your program.

   If ``CHPL_LAUNCHER`` is ``pbs-aprun``:

     a) You can optionally specify a queue name using the environment
        variable ``CHPL_LAUNCHER_QUEUE``.  For example:

          .. code-block:: sh

            export CHPL_LAUNCHER_QUEUE=batch

        If this variable is left unset, no queue name will be
        specified.  Alternatively, you can set the queue name on your
        Chapel program command line using the ``--queue`` flag.

     b) You can also optionally set a wall clock time limit for the
        job using ``CHPL_LAUNCHER_WALLTIME``.  For example to specify a
        10-minute time limit, use:

          .. code-block:: sh

            export CHPL_LAUNCHER_WALLTIME=00:10:00

        Alternatively, you can set the wall clock time limit on your
        Chapel program command line using the ``--walltime`` flag.

   For further information about launchers, please refer to
   :ref:`readme-launcher`.


4) Execute your Chapel program.  Multi-locale executions require the
   number of locales (compute nodes) to be specified on the command
   line.  For example::

     ./hello6-taskpar-dist -nl 2

   Requests the program to be executed using two locales.


5) If your Cray system has compute nodes with varying numbers of
   cores, you can request nodes with at least a certain number of
   cores using the variable ``CHPL_LAUNCHER_CORES_PER_LOCALE``.  For
   example, on a Cray system in which some compute nodes have 24 or
   more cores per compute node, you could request nodes with at least
   24 cores using:

   .. code-block:: sh

      export CHPL_LAUNCHER_CORES_PER_LOCALE=24

   This variable may be needed when you are using the aprun launcher and
   running Chapel programs within batch jobs you are managing yourself.
   The aprun launcher currently creates aprun commands that request the
   maximum number of cores per locale found on any locale in the system,
   irrespective of the fact that the batch job may have a lower limit
   than that on the number of cores per locale.  If the batch job limit
   is less than the maximum number of cores per locale, you will get the
   following error message when you try to run a Chapel program::

      apsched: claim exceeds reservation's CPUs

   You can work around this by setting ``CHPL_LAUNCHER_CORES_PER_LOCALE`` to
   the same or lesser value as the number of cores per locale specified
   for the batch job (for example, the mppdepth resource for the PBS
   qsub command).  In the future we hope to achieve better integration
   between Chapel launchers and workload managers.


6) If your Cray system has compute nodes with varying numbers of CPUs
   per compute unit, you can request nodes with a certain number of
   CPUs per compute unit using the variable ``CHPL_LAUNCHER_CPUS_PER_CU``.
   For example, on a Cray XC series system with some nodes having at
   least 2 CPUs per compute unit, to request running on those nodes
   you would use:

   .. code-block:: sh

      export CHPL_LAUNCHER_CPUS_PER_CU=2

   Currently, the only legal values for ``CHPL_LAUNCHER_CPUS_PER_CU`` are
   0 (the default), 1, and 2.


========================================  =============================
For more information on...                see...
========================================  =============================
...CHPL_* environment settings            :ref:`readme-chplenv`
...Compiling Chapel programs              :ref:`readme-compiling`
...Launcher options                       :ref:`readme-launcher`
...Executing Chapel programs              :ref:`readme-executing`
...Running multi-locale Chapel programs   :ref:`readme-multilocale`
========================================  =============================


--------------------------------------
Cray File Systems and Chapel execution
--------------------------------------

For best results, it is recommended that you execute your Chapel
program by placing the binaries on a file system shared between the
login node and compute nodes (typically Lustre), as this will provide
the greatest degree of transparency when executing your program.  In
some cases, running a Chapel program from a non-shared file system
will make it impossible to launch onto the compute nodes.  In other
cases, the launch will succeed, but any files read or written by the
Chapel program will be opened relative to the compute node's file
system rather than the login node's.


----------------------------------------------------
Special Notes for Cray XC Series Systems
----------------------------------------------------

.. _ugni-comm-on-cray:

Native ugni Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :ref:`readme-multilocale` page describes the runtime communication
layer implementations that can be used by Chapel programs.  In addition
to the standard ones, Chapel supports a Cray-specific ``ugni``
communication layer.  The ugni communication layer interacts with
the system's network interface very closely through a lightweight
interface called uGNI (user Generic Network Interface).  On Cray XC
systems the ugni communication layer is the default.


Using the ugni Communications Layer
___________________________________

To use ugni communications:

1) Leave your ``CHPL_COMM`` environment variable unset or set it to
   ``ugni``:

   .. code-block:: sh

      export CHPL_COMM=ugni

   This specifies that you wish to use the Cray-specific communication
   layer.


2) *(Optional)* Load an appropriate ``craype-hugepages`` module.  For example::

     module load craype-hugepages16M

   The ugni communication layer can be used with or without so-called
   *hugepages*.  Performance for remote variable references is much
   better when hugepages are used.  The only downside of using hugepages
   is that the tasking layer may not be able to detect task stack
   overflows by means of guard pages (see below).

   To use hugepages, you must have a ``craype-hugepages`` module loaded
   both when building your program and when running it.  There are
   several hugepage modules, with suffixes indicating the page size they
   support.  For example, ``craype-hugepages16M`` supports 16 MiB
   hugepages.  It does not matter which ``craype-hugepages`` module you
   have loaded when you build your program.  Any of them will do.  Which
   one you have loaded when you run a program does matter, however.  For
   general use, the Chapel group recommends the ``craype-hugepages16M``
   module.  You can read on for more information about hugepage modules
   if you would like, but the recommended ``craype-hugepages16M`` module
   will probably give you satisfactory results.

   The Cray network interface chips (NICs) can only address memory that
   has been registered with them. In practical terms, the Aries(TM) NIC
   on Cray XC systems is not limited as to how much memory it can
   register.  However, it does have an on-board cache of 512 registered
   page table entries, and registering more than this can cause reduced
   performance if the program's memory reference pattern causes refills
   in this cache.  We have seen up to a 15% reduction from typical
   nightly XC-16 performance in an ra-rmo run using hugepages small
   enough that every reference should have missed in this cache.
   Covering an entire 128 GiB XC compute node with only 512 hugepages
   will require at least the ``craype-hugepages256M`` module's 256 MiB
   hugepages.

   Offsetting this, using larger hugepages may reduce performance because
   it can result in poorer NUMA affinity.  With the ugni communication
   layer, arrays larger than 2 hugepages are allocated separately from the
   heap, which improves NUMA affinity.  An obvious side effect of using
   larger hugepages is that an array has to be larger to qualify.  Thus,
   achieving the best performance for any given program may require
   striking a balance between using larger hugepages to reduce NIC page
   table cache refills and using smaller ones to improve NUMA locality.

   Note that when hugepages are used with the ugni comm layer, tasking
   layers cannot use guard pages for stack overflow detection.  Qthreads
   tasking cannot detect stack overflow except by means of guard pages,
   so if ugni communications is combined with qthreads tasking and a
   hugepage module is loaded, stack overflow detection is unavailable.


Network Atomics
_______________

The Aries networks on Cray XC series systems support remote atomic
memory operations (AMOs).  When the ``CHPL_NETWORK_ATOMICS`` environment
variable is set to ``ugni``, the following operations on remote atomics
are done using the network::

    32- and 64-bit signed and unsigned integer types:
    32- and 64-bit real types:
      read()
      write()
      exchange()
      compareAndSwap()
      add(), fetchAdd()
      sub(), fetchSub()

    32- and 64-bit signed and unsigned integer types:
      or(),  fetchOr()
      and(), fetchAnd()
      xor(), fetchXor()

All of the operations shown above are done natively by the network
hardware except 64-bit real add, which is disabled in hardware and thus
done using ``on`` statements.


ugni Communication Layer and the Heap
_____________________________________

The "heap" is an area of memory used for dynamic allocation of
everything from user data to internal management data structures.
When running on Cray XC systems using the default configuration
with the ugni comm layer and a ``craype-hugepages`` module loaded, the
heap is used for all dynamic allocations except data space for arrays
larger than 2 hugepages.  (See `Using the ugni Communications Layer`_,
just above, for more about hugepages.)  It is normally extended
dynamically, as needed.  But if desired, the heap can instead be created
at a specified fixed size at the beginning of execution.  In some cases
this will reduce certain internal comm layer overheads and marginally
improve performance.

The disadvantage of a fixed heap is that it usually produces worse NUMA
affinity, it limits available heap memory to the specified fixed size,
and it limits memory for arrays to whatever remains after the fixed-size
heap is created.  If either of the latter are less than what a program
needs, it will terminate prematurely with an "Out of memory" message.

To specify a fixed heap, set the ``CHPL_RT_MAX_HEAP_SIZE`` environment
variable to indicate its size.  For the value of this variable you can
use any of the following formats, where *num* is a positive integer
number:

    ======= ==========================================
    Format  Resulting Heap Size
    ======= ==========================================
    num     num bytes
    num[kK] num * 2**10 bytes
    num[mM] num * 2**20 bytes
    num[gG] num * 2**30 bytes
    num%    percentage of compute node physical memory
    ======= ==========================================

Any of the following would specify an approximately 1 GiB heap on a
128-GiB compute node, for example:

  .. code-block:: sh

    export CHPL_RT_MAX_HEAP_SIZE=1073741824
    export CHPL_RT_MAX_HEAP_SIZE=1048576k
    export CHPL_RT_MAX_HEAP_SIZE=1024m
    export CHPL_RT_MAX_HEAP_SIZE=1g
    export CHPL_RT_MAX_HEAP_SIZE=1% # 1.28 GiB, really

Note that the resulting heap size may get rounded up to match the page
alignment.  How much this will add, if any, depends on the hugepage size
in any ``craype-hugepage`` module you have loaded at the time you
execute the program.  It may also be reduced, if some resource
limitation prevents making the heap as large as requested.


ugni Communication Layer Registered Memory Regions
__________________________________________________

The ugni communication layer maintains information about every memory
region it registers with Aries NIC.  Roughly speaking there are a few
memory regions for each tasking layer thread, plus one for each array
larger than 2 hugepages allocated and registered separately from the
heap.  By default the comm layer can handle up to 16k (2**14) total
memory regions, which is plenty under normal circumstances.  In the
event a program needs more than this, a message like the following will
be printed:

  .. code-block:: sh

    warning: no more registered memory region table entries (max is 16384).
             Change using CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS.

To provide for more registered regions, set the
``CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS`` environment variable to a number
indicating how many you want to allow.  For example:

  .. code-block:: sh

    export CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS=30000

Note that there are certain comm layer overheads that are proportional to
the number of registered memory regions, so allowing a very high number of
them may lead to reduced performance.


ugni Hugepage-related Warnings
______________________________

   Communication performance with ugni is so much better when hugepages
   are used that if you do not use them, the runtime will print the
   following warning when a multilocale program starts::

      warning: without hugepages, communication performance will suffer

   If you definitely do not want to use hugepages you can quiet this
   warning by giving the ``--quiet`` or ``-q`` option when you run the
   executable.  Otherwise, load a hugepage module as described above in
   `Using the ugni Communications Layer`_ before running.

   When you are using hugepages and do not have a fixed heap (that is,
   the ``CHPL_RT_MAX_HEAP_SIZE`` environment variable is not set), the
   Chapel runtime expects certain hugepage-related environment variables
   to have been set by the Chapel launcher.  If you do not use a Chapel
   launcher you have to provide these settings yourself.  Not doing so
   will result in one or both of the following messages::

      warning: dynamic heap on hugepages needs HUGETLB_NO_RESERVE set to something
      warning: dynamic heap on hugepages needs CHPL_JE_MALLOC_CONF set properly

   To quiet these warnings, use the following settings:

    .. code-block:: sh

      export HUGETLB_NO_RESERVE=yes
      export CHPL_JE_MALLOC_CONF=purge:decay,lg_chunk:log2HPS

   where *log2HPS* is the base-2 log of the hugepage size.  For example,
   with 16 MiB hugepages you would use:

    .. code-block:: sh

      export CHPL_JE_MALLOC_CONF=purge:decay,lg_chunk:24


gasnet Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~

The GASNet-based communication layer discussed in the
:ref:`readme-multilocale` page can be used on all Cray systems.  For
best performance it should be used with native substrates and fixed
segments, though even then its performance will rarely match that of the
ugni communication layer.  The relevant configurations are::

  CHPL_COMM=gasnet
    CHPL_COMM_SUBSTRATE=aries (for XC)
    CHPL_GASNET_SEGMENT=fast or large

In these configurations the heap is created with a fixed size at the
beginning of execution.  The default size works well in most cases but
if it doesn't a different size can be specified, as discussed in the
following section.


gasnet Communication Layer and the Heap
_______________________________________

In contrast to the dynamic heap extension available in the ugni comm
layer, when the gasnet comm layer is used with a native substrate for
higher network performance, the runtime must know up front the maximum
size the heap will grow to during execution.

In these cases the heap is used for all dynamic allocations, including
arrays.  By default it will occupy as much of the free memory on each
compute node as the runtime can acquire, less some small amount to allow
for demands from other (system) programs running there.  Advanced users
may want to make the heap smaller than the default.  Programs start more
quickly with a smaller heap, and in the unfortunate event that you need
to produce core files, those will be written more quickly if the heap is
smaller.  Specify the heap size using the ``CHPL_RT_MAX_HEAP_SIZE``
environment variable, as discussed above in `ugni Communication Layer
and the Heap`_.  But be aware that just as in the ``CHPL_COMM=ugni``
case, if you reduce the heap size to less than the amount your program
actually needs and then run it, it will terminate prematurely due to not
having enough memory.

Note that for ``CHPL_COMM=gasnet``, ``CHPL_RT_MAX_HEAP_SIZE`` is
synonymous with ``GASNET_MAX_SEGSIZE``, and the former overrides the
latter if both are set.


.. _readme-cray-constraints:

--------------------------
Known Constraints and Bugs
--------------------------

* Our PBS launcher explicitly supports PBS Pro, Moab/Torque, and the
  NCCS site versions of PBS.  It may also work with other versions.
  If our PBS launcher does not work for you, you can fall back on a
  more manual launch of your program. For example, supposing the
  program is compiled to ``myprogram``:

  - Launch the ``myprogram_real`` binary manually using aprun and your own
    qsub script or command.

  - Use ``./myprogram --generate-qsub-script`` to generate a qsub script.
    Then edit the generated script and launch the ``myprogram_real`` binary
    manually as above.

* Redirecting stdin when executing a Chapel program under PBS/qsub
  may not work due to limitations of qsub.

* For X-series systems, there is a known issue with the Cray MPI
  release that causes some programs to assert and then hang during
  exit.  A workaround is to set the environment variable,
  ``MPICH_GNI_DYNAMIC_CONN`` to ``disabled``.  Setting this environment
  variable affects all MPI programs, so remember to unset it after
  running your Chapel program.

* The amount of memory available to a Chapel program running over
  GASNet with the aries conduit is allocated at program start up.  The
  default memory segment size may be too high on some platforms,
  resulting in an internal Chapel error or a GASNet initialization
  error such as::

     node 1 log gasnetc_init_segment() at $CHPL_HOME/third-party/gasnet/gasnet-src/aries-conduit/gasnet_aries.c:<line#>: MemRegister segment fault 8 at  0x2aab6ae00000 60000000, code GNI_RC_ERROR_RESOURCE

  If your Chapel program exits with such an error, try setting the
  environment variable ``CHPL_RT_MAX_HEAP_SIZE`` or ``GASNET_MAX_SEGSIZE`` to a
  lower value than the default (say 1G) and re-running your program.
  For more information, refer to the discussion of ``CHPL_RT_MAX_HEAP_SIZE``
  above and/or the discussion of ``GASNET_MAX_SEGSIZE`` here::

     $CHPL_HOME/third-party/gasnet/gasnet-src/README

.. |reg|    unicode:: U+000AE .. REGISTERED SIGN
.. |trade|  unicode:: U+02122 .. TRADE MARK SIGN
