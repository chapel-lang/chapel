.. _readme-infiniband:

============================
Using Chapel with InfiniBand
============================

This document describes how to run Chapel across multiple nodes of
an InfiniBand system, including HPE Apollo and Cray CS systems.
:ref:`readme-multilocale` describes general information about
running Chapel in a multilocale configuration.

.. contents::

--------------------------
Configuring for InfiniBand
--------------------------

Due to the wide variety of InfiniBand systems and potential for
false-positives, Chapel does not currently auto-detect InfiniBand
configurations or platforms that commonly use InfiniBand. To build
Chapel with InfiniBand support, set:

.. code-block:: bash

      export CHPL_COMM=gasnet
      export CHPL_COMM_SUBSTRATE=ibv

Alternatively, when running on an HPE Apollo or Cray CS system
``CHPL_HOST_PLATFORM`` can be set instead, in which case the comm
and substrate settings will be inferred.

For HPE Apollo:

.. code-block:: bash

      export CHPL_HOST_PLATFORM=hpe-apollo

For Cray CS:

.. code-block:: bash

      export CHPL_HOST_PLATFORM=cray-cs


UCX alternative
---------------

In rare cases, we have had users who are unable to use the ``ibv``
conduit on their InfiniBand systems.  In such cases, the ``ucx``
conduit is an alternative to consider, which requires setting:

.. code-block:: bash

      export CHPL_COMM=gasnet
      export CHPL_COMM_SUBSTRATE=ucx

(and, most likely, ``export CHPL_LAUNCHER=gasnetrun_ucx`` when you
reach the following section).

Note that we have not had as much experience with the ``ucx`` conduit
in Chapel's development and testing as we have with ``ibv``, so it is
currently considered something of an experimental feature.  In
addition, the GASNet team also currently classifies the ``ucx``
conduit as "experimental" in the sense that it is feature complete but
has not received as much performance-oriented focus as ``ibv`` has.


----------------------
Configuring a Launcher
----------------------

A ``gasnetrun_ibv``-based launcher should be used to launch jobs and
native launchers like ``srun`` will not work. Most InfiniBand
systems use a workload manager or queueing system such as Slurm,
LSF, or PBS. To select an appropriate Chapel launcher you can set
``CHPL_LAUNCHER`` to one of the following values:

===================  ======================================
Launcher Name        Description
===================  ======================================
gasnetrun_ibv         run jobs interactively on your system
slurm-gasnetrun_ibv   queue jobs using Slurm (srun/sbatch)
pbs-gasnetrun_ibv     queue jobs using PBS (qsub)
lsf-gasnetrun_ibv     queue jobs using LSF (bsub)
===================  ======================================

``CHPL_LAUNCHER`` will typically default to ``gasnetrun_ibv`` unless
``CHPL_HOST_PLATFORM`` is ``cray-cs`` or ``hpe-apollo`` and ``srun``
is in your path, in which case it will default to
``slurm-gasnetrun_ibv``

By default Slurm, PBS, and LSF versions launch in an interactive
mode. For batch submission with Slurm ``CHPL_LAUNCHER_USE_SBATCH``
can be used as described in :ref:`using-slurm`. For other launchers
and as an alternative for Slurm, users can write batch submission
scripts and use ``gasnetrun_ibv`` to launch their jobs.


.. _setting-ibv-spawner:

-------------------
Selecting a Spawner
-------------------

Under the covers, ``gasnetrun_ibv``-based launchers must figure out
how to spawn jobs and get them up and running on the compute nodes.
GASNet's three ways of doing this on InfiniBand systems are ``ssh``,
``pmi``, and ``mpi``, described just below.  When MPI is detected at
configure time, it will be selected as the default, but we recommend
using one of the other options if possible.  This can be done by
setting the ``GASNET_IBV_SPAWNER`` environment variable, whose legal
values are:

* ``ssh``: Based on our experience, this is the preferred option, when
  possible.  This requires the ability to ``ssh`` to the system's
  compute nodes, which is not supported by all systems, depending on
  how they are configured.  See :ref:`the following
  sub-section<using-ssh>` for details on this option.
  
* ``pmi``: When GASNet's configure step detects a PMI-capable job
  scheduler like Slurm, ``pmi`` can be the next best choice because it
  often "just works" and can reduce overhead compared to ``mpi``.  For
  more information about this option, including how to configure job
  launch via ``PMIRUN_CMD``, see the `GASNet README for the PMI-based
  spawner
  <https://bitbucket.org/berkeleylab/gasnet/src/master/other/pmi-spawner/README>`_
  (also available at
  ``$CHPL_HOME/third-party/gasnet/gasnet-src/other/pmi-spawner/README``).
  
* ``mpi``: When the previous cases are not options, ``mpi`` serves as
  a reasonable last resort.  Note that it may, depending on its
  configuration, incur a performance penalty due to competition
  between MPI and GASNet for limited communication resources.  See the
  :ref:`using-mpi` section below for best practices when using this
  option.

.. _using-ssh:

Using SSH for Job Launch
------------------------

To launch InfiniBand jobs with SSH, use the following:

.. code-block:: bash

   # Specify ssh spawner
   export GASNET_IBV_SPAWNER=ssh

   # Specify the nodes to run on (only required when using plain
   # gasnetrun_ibv outside a Slurm/PBS/LSF reservation)
   export GASNET_SSH_SERVERS="nid00001 nid00002 nid00003 ..."

If you receive an error message like:

.. code-block:: printoutput

      *** Failed to start processes on nid00001, possibly due to an inability to establish an ssh connection from login-node without interactive authentication.

This indicates passwordless SSH is not set up. You can try copying
existing SSH keys or generating new ones with the following:

.. code-block:: bash

      ssh-keygen -t rsa # use default location and empty passphrase
      cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

If you see the same error message this may indicate ssh connections
to compute nodes are not allowed, in which case using the MPI
spawner may be your only option.

For further information about environment variables that can be used
to control how `ssh` is used to launch your Chapel program, see the
descriptions of ``GASNET_SSH_CMD`` and ``GASNET_SSH_OPTIONS`` in the
`GASNet README for the ssh spawner
<https://bitbucket.org/berkeleylab/gasnet/src/master/other/ssh-spawner/README>`_
(also available at
``$CHPL_HOME/third-party/gasnet/gasnet-src/other/ssh-spawner/README``).

.. _using-mpi:

Using MPI for Job Launch
------------------------

To launch InfiniBand jobs with *mpirun*, first make sure that *mpicc* is
available and that MPI programs launch appropriately with *mpirun*. Then use
the following. You'll want to make sure that GASNet detects MPI in its
configuration output.

.. code-block:: bash

   export GASNET_IBV_SPAWNER=mpi

As mentioned above, a potential downside of using MPI for launching
Chapel programs is that the resources that it requires to get the
program up and running can interfere with those needed by GASNet.  In
some cases, this can result in negative impacts on performance.  In
others, it can prevent GASNet from accessing the network resources it
requires at all.  For example, the following error is an example of
one in which MPI is preventing GASNet from accessing what it needs
(albeit using the ``ofi`` conduit rather than the ``ibv`` conduit
preferred for InfiniBand):

.. code-block:: bash

   *** FATAL ERROR (proc 0): in gasnetc_ofi_init() at /third-party/gasnet/gasnet-src/ofi-conduit/gasnet_ofi.c:1336: fi_endpoint for rdma failed: -22(Invalid argument)


For tips and best practices about how to configure/use GASNet to avoid
such conflicts with MPI, please refer to the section "Build-time
Configuration" in the `GASNet README for the MPI spawner
<https://bitbucket.org/berkeleylab/gasnet/src/master/other/mpi-spawner/README>`_
(also available at
``$CHPL_HOME/third-party/gasnet/gasnet-src/other/mpi-spawner/README``).
Within this README, see also the description of the ``MPIRUN_CMD``
environment variable as a means of configuring how jobs are started.
   

--------------------
Verifying Job Launch
--------------------

Once the above configuration has been done, checking that jobs are
launching properly is recommended. The following Chapel program will
print out the locale names and how much parallelism is available per
locale. Ideally each locale is running on a unique node (not
oversubscribed) and the amount of parallelism matches the number of
physical cores on each node.

.. code-block:: chapel

      for loc in Locales do on loc do
        writeln((here.name, here.maxTaskPar));

An example run may look something like the following:

.. code-block:: printoutput

      (nid00001, 28)
      (nid00002, 28)

If nodes are oversubscribed or the amount of parallelism is far less
than expected see :ref:`setting-ibv-spawner` and if that does not
help consider opening a bug as described in :ref:`readme-bugs`.


----------------------------------
Setting Memory Registration Limits
----------------------------------

On most high-performance networks, including InfiniBand, memory has
to be registered with the network in order for Chapel to take
advantage of fast one-sided communication. On InfiniBand networks
there may be limits placed on how much memory can be registered so
GASNet will probe at startup to detect this value. This probing can
be slow, so GASNet will recommend setting ``GASNET_PHYSMEM_MAX`` to
avoid probing every time. On nodes with homogeneous amounts of
memory this message usually looks something like:

.. code-block:: printoutput

      WARNING: Beginning a potentially slow probe of max pinnable memory...
      WARNING: Probe of max pinnable memory completed in 45s.
      WARNING:   Probe of max pinnable memory has yielded '335 GB'.
      WARNING:   If you have the same memory configuration on all nodes, then
      WARNING:   to avoid this probe in the future either reconfigure using
      WARNING:      --with-ibv-physmem-max='335 GB'
      WARNING:   or run with environment variable
      WARNING:      GASNET_PHYSMEM_MAX='335 GB'.

Where setting ``GASNET_PHYSMEM_MAX='335 GB'`` will quiet the warning
and skip the startup probe.  On nodes with non-homogeneous amounts
of memory GASNet may recommend using a fraction of memory instead of
an absolute value with something like
``GASNET_PHYSMEM_MAX='0.667'``.

Setting ``GASNET_PHYSMEM_MAX`` to a small value can limit
communication performance so it is highly recommended to use the value
GASNet suggests.

----------
Co-locales
----------

By default, Chapel disables GASNet's PSHM (Process SHared-Memory bypass)
feature when running on InfiniBand.  This means that by default on-node
communication between co-locales traverses the loopback IB network interface,
incurring overheads associated with the IB verbs networking layer that are high
relative to the latencies one might expect for on-node communication.  Using
co-locales in this configuration will generate a startup warning from GASNet
that looks like this:

.. code-block:: printoutput

      *** WARNING (proc 0): Running with multiple processes per host without shared-memory communication support (PSHM).  This can significantly reduce performance.  Please re-configure GASNet using `--enable-pshm` to enable fast intra-host comms.

This (somewhat confusingly worded) message accurately reflects the fact that
Chapel's co-locale behavior with the InfiniBand backend has not yet been tuned
and may provide sub-optimal performance for on-node communication.

Users wishing to experiment with enabling the shared-memory bypass support for
the InfiniBand backend can do so by adding the following environment variable
setting when building Chapel:

.. code-block:: bash

      export CHPL_GASNET_MORE_CFG_OPTIONS=--enable-pshm

This will enable the PSHM support when co-locales are in-use, such that on-node
communication between co-locales will occur in memory and not through the
network.  It's worth noting that Chapel's integration of the PSHM feature
currently requires an extra "progress" thread that defaults to running on the
same cores as the Chapel tasks, and will compete with those tasks for cycles.
For computation-bound applications the overhead incurred by this progress thread
may outweigh the benefits of PSHM.  You can optionally disable PSHM at
application run-time by setting ``export GASNET_SUPERNODE_MAXSIZE=1``, although
this won't exactly match the behavior of building without PSHM support.

Another alternative is to dedicate a core for the progress thread, preventing
it from running on the same cores as the Chapel tasks. This is accomplished
by setting ``CHPL_RT_COMM_GASNET_DEDICATED_PROGRESS_CORE=true``. Note that
this means there will be one fewer core to run Chapel tasks, which may not be
advantageous on machines with relatively few cores. Also note that this
variable will dedicate a core whether or not PSHM is in-use, so you should
probably only set this variable if you are using co-locales and PSHM is enabled.

----------------
Progress Threads
----------------

In addition to the "busy" polling-based progress thread described above that
is enabled when co-locales are combined with GASNet's (default disabled) PSHM
support, the InfiniBand backend also optionally includes GASNet-level blocking
progress threads used to help retire incoming and outgoing network communication
operations (named the blocking "receive" and "send" progress threads, respectively). 
These threads are "blocking" in that they are awakened on-demand when the network
adapter reports there is communication work to be done, and otherwise generally
remain parked in a kernel call where they do not consume any core cycles.
By default the blocking receive progress thread is enabled and the blocking
send progress thread is disabled.

The blocking send progress thread may be enabled at application run-time by setting:

.. code-block:: bash

      export GASNET_SND_THREAD=1

This enables a helper thread that has been shown to accelerate injection of
communication in some cases, but in other cases may degrade communication
throughput (notably on NUMA systems where the locale straddles the NUMA boundary).
GASNet also provides additional environment variable settings that can
optionally be used to control the detailed behavior of these threads, see the
GASNet documentation referenced in :ref:`ibv-conduit-docs`.

By default, the blocking progress threads are created by GASNet and do not
have any thread-specific core binding.  An experimental Chapel feature
allows more control over the behavior of the blocking progress threads:

.. code-block:: bash

      export CHPL_RT_COMM_GASNET_DEFER_PROGRESS_THREADS=true

Specifically, this setting enables the
``CHPL_RT_COMM_GASNET_DEDICATED_PROGRESS_CORE`` setting described in the
earlier section to additionally control the placement and binding of GASNet's
blocking send and receive progress threads.

.. _ibv-conduit-docs:

--------
See Also
--------

For more information on these and other available GASNet options when
targeting InfiniBand, please refer to GASNet's official `InfiniBand
conduit documentation
<https://gasnet.lbl.gov/dist/ibv-conduit/README>`_, which can also be
found in
``$CHPL_HOME/third-party/gasnet/gasnet-src/ibv-conduit/README``.
