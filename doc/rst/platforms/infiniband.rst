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
``pmi``, and ``mpi``.  When MPI is detected at configure time, it will
be selected as the default, but we recommend using one of the other
options if possible.

* ``ssh``: Based on our experience, we recommend launching with the
  ``ssh`` option, if possible.  This requires the ability to ``ssh``
  to the system's compute nodes, which is not supported by all
  systems, depending on how they are configured.  See the following
  sub-section for details on this option.
  
* ``pmi``: When GASNet's configure step detects a PMI-capable job
  scheduler like Slurm, ``pmi`` can be the next best choice because it
  often "just works" and can reduce overhead compared to ``mpi``.

* ``mpi``: When the previous cases are not options, ``mpi`` serves as
  a reasonable last resort.  Note that it can incur a performance
  penalty because MPI will be running concurrently with GASNet.  See
  the second subsection below for tips on this option.


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


Using MPI for Job Launch
------------------------

To launch InfiniBand jobs with *mpirun*, first make sure that *mpicc* is
available and that MPI programs launch appropriately with *mpirun*. Then use
the following. You'll want to make sure that GASNet detects MPI in its
configuration output.

.. code-block:: bash

   export GASNET_IBV_SPAWNER=mpi


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

On most high performance networks, including InfiniBand, memory has
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
communication performance so it is highly recommend to use the value
GASNet suggests.


--------
See Also
--------

For more information on these and other available GASNet options,
including configuring to launch through MPI, please refer to
GASNet's official `InfiniBand conduit documentation
<https://gasnet.lbl.gov/dist/ibv-conduit/README>`_, which can also be found
in ``$CHPL_HOME/third-party/gasnet/gasnet-src/ibv-conduit/README``.
