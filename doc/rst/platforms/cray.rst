.. _readme-cray:

================================
Using Chapel on HPE Cray Systems
================================

The following information is assembled to help Chapel users get up and
running on HPE Cray systems including the HPE Cray EX and Cray XC
series systems.

.. contents::


--------------------------------------------------------
Getting Started with Chapel on HPE Cray EX or XC Systems
--------------------------------------------------------

Chapel is available as a module for HPE Cray EX and XC systems.  When
it is installed on your system, you do not need to build Chapel from
the source release (though you can).  Using the module systems on such
platforms, you can use Chapel with the default settings and confirm it
is correctly installed, as follows:

1) Ensure this required module is loaded.  Normally it will be loaded
   for you, but under some circumstances you may need to load or
   restore it yourself:

   .. code-block:: sh

      PrgEnv-gnu or PrgEnv-cray


2) Load the Chapel module::

      module load chapel


3) Compile an example program using::

     chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl


4) Execute the resulting executable (on four locales)::

     ./hello6-taskpar-dist -nl 4


This should be all that is necessary to use Chapel on an HPE Cray EX or XC
system.  If the installation setup by your system administrator
deviates from the default settings, or you are interested in other
configuration options, see `Using Chapel on an HPE Cray System`_ below.  If
instead you wish to build Chapel from source, continue on to `Building
Chapel for an HPE Cray System from Source`_ just below.

Note that the Chapel module for HPE Cray EX systems does not currently have
the GASNet communication layer built into it.  For multilocale execution
on EX systems please use the ofi communication layer instead.

For information on obtaining and installing the Chapel module please
contact your system administrator.


-------------------------------------------------------------
Getting Started with Chapel on HPE Apollo and Cray CS Systems
-------------------------------------------------------------

On HPE Apollo and Cray CS systems, Chapel is not currently available
as a module due to the wide diversity of configurations that these
systems support.  For this reason, Chapel must be built from source on
these systems using the `Building Chapel for an HPE Cray System from
Source`_ instructions just below.


--------------------------------------------------
Building Chapel for an HPE Cray System from Source
--------------------------------------------------

1) Set ``CHPL_HOST_PLATFORM`` to the string representing your system
   type.

   For example:

    .. code-block:: sh

      export CHPL_HOST_PLATFORM=hpe-cray-ex

   The following table lists the supported systems and strings.  Note
   that on HPE Cray EX and XC systems, these values should typically
   be inferred automatically and not need to be set manually.  That said,
   there is also no downside to setting them manually.  As with most
   `CHPL_*` environment variables, the current set and inferred values
   can be determined by running ``$CHPL_HOME/util/printchplenv``.

       =============  ==================
       System         CHPL_HOST_PLATFORM
       =============  ==================
       EX series      hpe-cray-ex
       Apollo series  hpe-apollo
       XD series      hpe-cray-xd
       XC series      cray-xc
       CS series      cray-cs
       =============  ==================


2) Optionally, set the ``CHPL_LAUNCHER`` environment variable to indicate
   how Chapel should launch jobs on your system:

      ========================================  =========================
      On an HPE Cray EX system, ...             set CHPL_LAUNCHER to...
      ========================================  =========================
      ...queue jobs using SLURM (sbatch)        slurm-srun
      ...run jobs interactively using PALS      pals
      ========================================  =========================

      ========================================  =========================
      On a HPE Apollo or Cray CS system, to...  set CHPL_LAUNCHER to...
      ========================================  =========================
      ...run jobs interactively on your system  gasnetrun_ibv
      ...queue jobs using SLURM (sbatch)        slurm-gasnetrun_ibv
      ...queue jobs using LSF (bsub)            lsf-gasnetrun_ibv
      ...queue jobs using PBS (qsub)            pbs-gasnetrun_ibv
      ========================================  =========================

      ========================================  =========================
      On a Cray XC system, to...                set CHPL_LAUNCHER to...
      ========================================  =========================
      ...run jobs interactively on your system  aprun
      ...queue jobs using PBS (qsub)            pbs-aprun
      ...queue jobs using SLURM (sbatch)        slurm-srun
      ========================================  =========================

   On HPE Cray EX systems, ``CHPL_LAUNCHER`` defaults to ``slurm-srun``
   if ``srun`` is in your path and ``none`` otherwise.

   On HPE Apollo and Cray CS systems, ``CHPL_LAUNCHER`` defaults to
   ``gasnetrun_ibv``.

   On Cray XC systems, ``CHPL_LAUNCHER`` defaults to ``aprun`` if
   ``aprun`` is in your path, ``slurm-srun`` if ``srun`` is in your path
   and ``none`` otherwise.

   You can also set CHPL_LAUNCHER to ``none`` if you prefer to manually
   manage all queuing and job launch commands yourself, though the
   advantage of using a launcher is to make sure that Chapel maps
   processes and threads to the appropriate target hardware units.

   For more information on Chapel's launcher capabilities and options,
   refer to :ref:`readme-launcher`.


3) Select the target compiler that Chapel should use when compiling
   code for the compute node:

   On an HPE Apollo or Cray CS series system, set the
   ``CHPL_TARGET_COMPILER`` environment variable to indicate which
   compiler to use (and make sure that the compiler is in your path).

      ===========================  ==============================
      To request...                set CHPL_TARGET_COMPILER to...
      ===========================  ==============================
      ...the LLVM/clang backend    llvm (default)
      ...the GNU compiler (gcc)    gnu
      ...the Clang compiler        clang
      ...the Intel compiler (icc)  intel
      ===========================  ==============================

   On an HPE Cray EX or Cray XC system, when using the C back-end,
   ensure that you have one of the following Programming Environment
   modules loaded to specify your target compiler::

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


.. _readme-hpe-cray-ex:

----------------------------------
Using Chapel on an HPE Cray System
----------------------------------

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


2) On HPE Cray EX systems with ``CHPL_COMM=ofi``, optionally, load the
   Cray PMI modules::

      module load cray-pmi{,-lib}

   Often this is not required.  Usually the default PMI support has
   sufficient capabilities to support Chapel program startup.  But under
   certain circumstances it does not, and when you run a Chapel program
   that was built without these loaded you will see messages like this
   one:

   .. code-block:: sh

      [PE_0]:_pmi2_add_kvs:ERROR: The KVS data segment of <num> entries
      is not large enough.  Increase the number of KVS entries by
      setting env variable PMI_MAX_KVS_ENTRIES to a higher value.

   Having the Cray PMI modules loaded when the program is compiled
   will prevent this problem.  We expect that eventually these modules
   will be loaded by default on EX systems, but so far this has not
   consistently been the case.


3) Compile your Chapel program.  For example:

   .. code-block:: sh

      chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl

   See :ref:`readme-compiling` or  ``man chpl`` for further details.


4) If ``CHPL_LAUNCHER`` is set to anything other than ``none``, when you
   compile a Chapel program for your Cray system, you will see two
   binaries (e.g., ``hello6-taskpar-dist`` and ``hello6-taskpar-dist_real``).
   The first binary contains code to launch the Chapel program onto
   the compute nodes, as specified by your ``CHPL_LAUNCHER`` setting.  The
   second contains the program code itself; it is not intended to be
   executed directly from the shell prompt.

   You can use the ``-v`` or ``--dry-run`` flags to see the commands
   used by the launcher binary to start your program.

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


5) Execute your Chapel program.  Multi-locale executions require the
   number of locales (compute nodes) to be specified on the command
   line.  For example::

     ./hello6-taskpar-dist -nl 2

   Requests the program to be executed using two locales.


6) If your HPE Cray system has compute nodes with varying numbers of
   cores, you can request nodes with at least a certain number of
   cores using the variable ``CHPL_LAUNCHER_CORES_PER_LOCALE``.  For
   example, on a system in which some compute nodes have 24 or
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


7) If your HPE Cray system has compute nodes with varying numbers of CPUs
   per compute unit, you can request nodes with a certain number of
   CPUs per compute unit using the variable ``CHPL_LAUNCHER_CPUS_PER_CU``.
   For example, on a Cray XC system with some nodes having at
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


----------------------------------------
Special Notes for Cray XC Series Systems
----------------------------------------

.. _ugni-comm-on-cray:

Native ugni Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The native ugni communication layer is the default on Cray XC systems. See
:ref:`readme-ugni` for more information on using the ugni communication layer.


GASNet Communication Layer
~~~~~~~~~~~~~~~~~~~~~~~~~~

The GASNet-based communication layer discussed in the
:ref:`readme-gasnet` page can be used on Cray XC systems, using the 'ofi'
or 'mpi' substrates. This is not well supported or tested. The
relevant configurations are:

  CHPL_COMM=gasnet
    CHPL_COMM_SUBSTRATE=ofi or mpi
    CHPL_GASNET_SEGMENT=fast or large

In these configurations the heap is created with a fixed size at the
beginning of execution.  The default size works well in most cases but
if it doesn't a different size can be specified, as discussed in the
following section.


GASNet Communication Layer and the Heap
_______________________________________

In contrast to the dynamic heap extension available in the ugni comm
layer, when the GASNet comm layer is used with a native substrate for
higher network performance, the runtime must know up front the maximum
size the heap will grow to during execution.

In these cases the heap is used for all dynamic allocations, including
arrays.  By default it will occupy as much of the free memory on each
compute node as the runtime can acquire, less some small amount to allow
for demands from other (system) programs running there.  Advanced users
may want to make the heap smaller than the default.  Programs start more
quickly with a smaller heap, and in the unfortunate event that you need
to produce core files, those will be written more quickly if the heap is
smaller.  Specify the heap size using the :ref:`CHPL_RT_MAX_HEAP_SIZE`
environment variable.  But be aware that if you reduce the heap size
to less than the amount your program actually needs and then run it,
it will terminate prematurely due to not having enough memory.

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

* For EX and XC systems, there is a known issue with the Cray MPI
  release that causes some programs to assert and then hang during
  exit.  A workaround is to set the environment variable,
  ``MPICH_GNI_DYNAMIC_CONN`` to ``disabled``.  Setting this environment
  variable affects all MPI programs, so remember to unset it after
  running your Chapel program.


---------------
Troubleshooting
---------------

Each of the following subsection headers is an error message that you
may receive in practice, followed by steps that can be taken to work
around it.


srun: error: Unable to allocate resources: Memory required by task is not available
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See this :ref:`Slurm troubleshooting section <mem-not-avail>` for help
with this error.


.. _ex-register-too-much-mem:

OFI error: fi_mr_reg(ofi_domain, ...): Cannot allocate memory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This error suggests that the Chapel runtime is being too aggressive in
registering memory with the network, or that something else is going
wrong in the memory registration.  The typical workaround is to cap
the amount of memory registered by using :ref:`CHPL_RT_MAX_HEAP_SIZE`,
for example ``export CHPL_RT_MAX_HEAP_SIZE=50%`` will limit it to 50%
of the available memory and is generally sufficient to get programs
running.

Note that changing this environment variable, as with any
``CHPL_RT_*`` variable, does not require rebuilding Chapel or
recompiling your program.


Detected 1 oom_kill event in StepId=.... Some of the step tasks have been OOM Killed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This error is also a sign that the Chapel runtime is being too aggressive in
registering memory, or that something else is going wrong in the memory
registration.

See :ref:`the previous section <ex-register-too-much-mem>` for steps to work
around this error.

OFI error: fi_enable(tcip->txCtx): Invalid resource domain
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We have seen this error message when running on compute nodes where
the number of cores sharing a single NIC is greater than ~255.  For
example, if running on a system where each compute node has four
80-core processors, when running on more than one node (e.g., -nl 8),
by default Chapel would attempt to create 320 endpoints per locale,
all sharing a single NIC, and therefore hit this error.

Two ways to resolve this error are:

1) If the compute node has multiple NICs, run using :ref:`co-locales
   <readme-colocale>` such that each gets its own NIC and uses a
   fraction of the cores.  For example, if the node above had 4 NICs,
   switching the command line from ``-nl 8`` to ``-nl 8x4`` would run
   four co-locales per node such that each would get its own NIC and
   create 80 endpoints on it, well below the limit.  Moreover, running
   with co-locales typically has other benefits in terms of NUMA
   affinity benefits.

2) The other alternative is to cap the maximum number of endpoints
   created by setting the environment variable
   ``CHPL_RT_COMM_OFI_EP_CNT``.  For example.  ``export
   CHPL_RT_COMM_OFI_EP_CNT=254``.  The impact of this approach is that
   each core will not get its own endpoint, which could impact
   performance.


warning: The node has more locales (5) than co-locales (2)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In some cases, this warning can be indicative of an erroneous
condition in which the Chapel launcher is not correctly comprehending
the compute node structure.  This can happen on systems with multiple
partitions if no partition is explicitly specified using
``CHPL_LAUNCHER_PARTITION`` or ``--partition``.  The typical fix is to
use one of these two mechanisms to specify the partition even if it is
the default partition.

Note that changing the ``CHPL_LAUNCHER_PARTITION`` environment
variable, as with any ``CHPL_LAUNCHER_*`` variable, does not require
rebuilding Chapel or recompiling your program.


OFI error: fi_domain(ofi_fabric, ofi_info, ...): Function not implemented
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We encounter this error on some systems when trying to execute on a
single node using co-locales (e.g., `-nl 1x4`).

When this happens, our experience is that ``export
SLURM_NETWORK=single_node_vni`` can be set as a workaround with no
need to recompile.


.. |reg|    unicode:: U+000AE .. REGISTERED SIGN
.. |trade|  unicode:: U+02122 .. TRADE MARK SIGN
