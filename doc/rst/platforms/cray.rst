.. _readme-cray:

============================
Using Chapel on Cray Systems
============================

The following information is assembled to help Chapel users get up and running
on Cray\ |reg| systems including the Cray XC\ |trade|, XE\ |trade|, XK\
|trade|, and CS\ |trade| series systems.

Support has been added for the Cray XC50\ |trade| system with ARM
processors. This works the same as other Cray XC\ |trade| systems in
the instructions below, except that there is no Intel compiler.

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

     chpl -o hello6-taskpar-dist $CHPL_HOME/examples/hello6-taskpar-dist.chpl


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

1) If using a XC, XE, or XK system, continue to step 2. If using a
   CS series system, set ``CHPL_HOST_PLATFORM`` to ``cray-cs``.

   For example:

    .. code-block:: sh

      export CHPL_HOST_PLATFORM=cray-cs

   These are the supported systems and strings.  Note that these values
   are used by default when building on the given systems.  They can
   also be set manually.  Also note that the ``cray-xe`` configuration
   covers Cray XK systems as well as Cray XE systems.

       =========  ==================
       System     CHPL_HOST_PLATFORM
       =========  ==================
       CS series  cray-cs
       XC series  cray-xc
       XE series  cray-xe
       XK series  cray-xe
       =========  ==================


2) Optionally, set the ``CHPL_LAUNCHER`` environment variable to indicate
   how Chapel should launch jobs on your system:

      ========================================  =========================
      On a Cray CS system, to...                set CHPL_LAUNCHER to...
      ========================================  =========================
      ...run jobs interactively on your system  gasnetrun_ibv
      ...queue jobs using PBSPro (qsub)         pbs-gasnetrun_ibv
      ...queue jobs using SLURM (sbatch)        slurm-gasnetrun_ibv
      ...queue jobs using LSF (bsub)            lsf-gasnetrun_ibv
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

      chpl -o hello6-taskpar-dist $CHPL_HOME/examples/hello6-taskpar-dist.chpl

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

   If ``CHPL_LAUNCHER`` is ``pbs-aprun`` or ``pbs-gasnetrun_ibv``:

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

   If ``CHPL_LAUNCHER`` is ``slurm-gasnetrun_ibv``:

     You must set the amount of time to request from SLURM.
     For example, the following requests 15 minutes:

      .. code-block:: sh

        export CHPL_LAUNCHER_WALLTIME=00:15:00

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
Special Notes for Cray XC, XE, and XK Series Systems
----------------------------------------------------

Native ugni Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :ref:`readme-multilocale` page describes the runtime communication
layer implementations that can be used by Chapel programs.  In addition
to the standard ones, Chapel supports a Cray-specific ``ugni``
communication layer.  The ugni communication layer interacts with
the system's network interface very closely through a lightweight
interface called uGNI (user Generic Network Interface).  On Cray XC, XK,
and XE systems the ugni communication layer is the default.


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
   has been registered with them, and there are limits on how many pages
   of memory can be registered.  The Gemini(TM) NIC used on Cray XE and XK
   systems can register no more than 16k (2**14) pages of memory.  The
   Aries(TM) NIC used on Cray XC systems can register more, but it has an
   on-board cache of registered page information with 16k entries and
   performance will be reduced if the number of registered pages exceeds
   the 16k entries in that cache.  Thus for any kind of Cray X* system,
   you should choose a hugepage module whose page size is large enough
   that 16k of its hugepages will cover the program's per-node memory
   requirement or, if that is not known, the compute node memory size.
   For example, the 2 MiB hugepages in the ``craype-hugepages2M`` module
   will cover a 32 GiB Cray XE compute node, but on a Cray XC system
   with 128 GiB compute nodes at least 8 MiB hugepages will be needed to
   achieve full coverage.

   Note that when hugepages are used with the ugni comm layer, tasking
   layers cannot use guard pages for stack overflow detection.  Qthreads
   tasking cannot detect stack overflow except by means of guard pages,
   so if ugni communications is combined with qthreads tasking and a
   hugepage module is loaded, stack overflow detection is unavailable.


Network Atomics
_______________

The Gemini and Aries networks on Cray XE, XK, and XC series systems
support remote atomic memory operations (AMOs).  When the
``CHPL_NETWORK_ATOMICS`` environment variable is set to ``ugni``, the
following operations on remote atomics are done using the network::

    32- and 64-bit signed and unsigned integer types:
    32- and 64-bit real types:
      read()
      write()
      exchange()
      compareExchange()
      add(), fetchAdd()
      sub(), fetchSub()

    32- and 64-bit signed and unsigned integer types:
      or(),  fetchOr()
      and(), fetchAnd()
      xor(), fetchXor()

Note that on XE and XK systems, which have Gemini networks, out of the
above list only the 64-bit integer operations are done natively by the
network hardware.  32-bit integer and all real operations are
done using implicit ``on`` statements inside the ugni communication
layer, accelerated by Gemini hardware capabilities.

On XC systems, which have Aries networks, all of the operations shown
above are done natively by the network hardware except 64-bit real add,
which is disabled in hardware and thus done using ``on`` statements.


ugni Communication Layer and the Heap
_____________________________________

The "heap" is an area of memory used for dynamic allocation of
everything from user data to internal management data structures.
When running on Cray XC/XE/XK systems using the default configuration
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
variable to indicate its size.  Set this to just a number to specify the
size of the heap in bytes, or to a number with a ``k`` or ``K``, ``m``
or ``M``, or ``g`` or ``G`` suffix with no intervening spaces to specify
the heap size in KiB (2**10 bytes), MiB (2**20 bytes), or GiB (2**30
bytes), respectively.  Any of the following would set the heap size to 1
GiB, for example:

  .. code-block:: sh

    export CHPL_RT_MAX_HEAP_SIZE=1073741824
    export CHPL_RT_MAX_HEAP_SIZE=1048576k
    export CHPL_RT_MAX_HEAP_SIZE=1024m
    export CHPL_RT_MAX_HEAP_SIZE=1g

Note that the value you set in ``CHPL_RT_MAX_HEAP_SIZE`` may get rounded up
internally to match the page alignment.  How much, if any, this will add
depends on the hugepage size in any ``craype-hugepage`` module you have
loaded at the time you execute the program.


Communication Layer Concurrency
_______________________________

The ``CHPL_RT_COMM_CONCURRENCY`` environment variable tells the ugni
communication layer how much program concurrency it should try to
support.  Basically, this controls how much of the communication
resources on the NIC will be used by the program.  The default value is
the number of hardware processor cores the program will use for Chapel
tasks.  Usually this is enough, but for highly parallel codes that do a
lot of remote references, increasing it may help the performance.
Useful values for ``CHPL_RT_COMM_CONCURRENCY`` are in the range 1 to 30
on the Gemini-based Cray XE and XK systems, and 1 to 120 on the
Aries-based Cray XC systems.  Values specified outside this range are
silently increased or reduced so as to fall within it.


gasnet Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~

The GASNet-based communication layer discussed in the
:ref:`readme-multilocale` page can be used on all Cray systems.  For
best performance it should be used with native substrates and fixed
segments, though even then its performance will rarely match that of the
ugni communication layer.  The relevant configurations are::

  CHPL_COMM=gasnet
    CHPL_COMM_SUBSTRATE=gemini (for XE or XK) or aries (for XC)
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

* GASNet targets multiple network *conduits* as the underlying
  communication mechanism.  On certain platforms, the Chapel build
  will use the ``mpi`` conduit as the default.  As a result of using the
  mpi conduit, you may see a GASNet warning message at program start
  up.  To squelch this message, you can set the environment variable
  ``GASNET_QUIET=yes``.

* For X-series systems, there is a known issue with the Cray MPI
  release that causes some programs to assert and then hang during
  exit.  A workaround is to set the environment variable,
  ``MPICH_GNI_DYNAMIC_CONN`` to ``disabled``.  Setting this environment
  variable affects all MPI programs, so remember to unset it after
  running your Chapel program.

* The amount of memory available to a Chapel program running over
  GASNet with the gemini and aries conduits is allocated at program
  start up.  The default memory segment size may be too high on some
  platforms, resulting in an internal Chapel error or a GASNet
  initialization error such as::

     node 1 log gasnetc_init_segment() at $CHPL_HOME/third-party/gasnet/gasnet-src/gemini-conduit/gasnet_gemini.c:<line#>: MemRegister segment fault 8 at  0x2aab6ae00000 60000000, code GNI_RC_ERROR_RESOURCE

  If your Chapel program exits with such an error, try setting the
  environment variable ``CHPL_RT_MAX_HEAP_SIZE`` or ``GASNET_MAX_SEGSIZE`` to a
  lower value than the default (say 1G) and re-running your program.
  For more information, refer to the discussion of ``CHPL_RT_MAX_HEAP_SIZE``
  above and/or the discussion of ``GASNET_MAX_SEGSIZE`` here::

     $CHPL_HOME/third-party/gasnet/gasnet-src/README


---------------
NCCS user notes
---------------

* NCCS Cray systems use a different qsub mechanism in order to
  enforce their queuing policies.  We have attempted to make our
  pbs-aprun launch code work with this version of qsub, but require a
  ``CHPL_LAUNCHER_ACCOUNT`` environment variable to be set to specify your
  NCCS account name.  For example:

  .. code-block:: sh

    export CHPL_LAUNCHER_ACCOUNT=MYACCOUNTID

* NCCS users either need to specify ``debug`` as their queue or set an
  explicit wall clock time limit using the mechanisms described above.


.. |reg|    unicode:: U+000AE .. REGISTERED SIGN
.. |trade|  unicode:: U+02122 .. TRADE MARK SIGN
