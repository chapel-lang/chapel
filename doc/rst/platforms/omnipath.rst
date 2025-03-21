.. _readme-omnipath:

===========================
Using Chapel with Omni-Path 
===========================

This document describes how to run Chapel across multiple nodes of
an Intel Omni-Path cluster. :ref:`readme-multilocale` describes
general information about running Chapel in a multilocale
configuration.

.. note::
    Only ``CHPL_COMM=gasnet`` with the ofi substrate is supported for
    Omni-Path. ``CHPL_COMM=ofi`` is not supported.

.. contents::

-------------------------
Configuring for Omni-Path
-------------------------

To build Chapel with Omni-Path support, set:

.. code-block:: bash

      export CHPL_COMM=gasnet
      export CHPL_COMM_SUBSTRATE=ofi

GASNet should automatically detect the ``psm2`` provider, but to
ensure this provider is used ``FI_PROVIDER=psm2`` may optionally be
set.

----------------------
Configuring a Launcher
----------------------

A ``gasnetrun_ofi``-based launcher should be used to launch jobs and
native launchers like ``srun`` will not work. Many Omni-Path
clusters use a workload manager or queueing system such as Slurm. To
select an appropriate Chapel launcher you can set ``CHPL_LAUNCHER``
to one of the following values:

===================  ======================================
Launcher Name        Description
===================  ======================================
gasnetrun_ofi         run jobs interactively on your system
slurm-gasnetrun_ofi   queue jobs using Slurm (srun/sbatch)
===================  ======================================

.. _setting-ofi-spawner:

-------------------
Selecting a Spawner
-------------------

Under the covers, ``gasnetrun_ofi``-based launchers must figure out
how to spawn jobs and get them up and running on the compute nodes.
GASNet's three ways of doing this on Omni-Path clusters are ``ssh``,
``pmi``, and ``mpi``, described just below.  When MPI is detected at
configure time, it will be selected as the default, but we recommend
using one of the other options if possible.  This can be done by
setting the ``GASNET_OFI_SPAWNER`` environment variable, whose legal
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


Using SSH for Job Launch
------------------------

To launch Omni-Path jobs with SSH, use the following:

.. code-block:: bash

   # Specify ssh spawner
   export GASNET_OFI_SPAWNER=ssh

   # Specify the nodes to run on (only required when using plain
   # gasnetrun_ofi outside a Slurm/PBS/LSF reservation)
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


Using MPI for Job Launch
------------------------

To launch Omni-Path jobs with *mpirun*, first make sure that *mpicc* is
available and that MPI programs launch appropriately with *mpirun*. Then use
the following. You'll want to make sure that GASNet detects MPI in its
configuration output.

.. code-block:: bash

   export GASNET_OFI_SPAWNER=mpi

As mentioned above, a potential downside of using MPI for launching
Chapel programs is that the resources that it requires to get the
program up and running can interfere with those needed by GASNet.  In
some cases, this can result in negative impacts on performance.  In
others, it can prevent GASNet from accessing the network resources it
requires at all.  For example, the following error is an example of
one in which MPI is preventing GASNet from accessing what it needs:

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
than expected see :ref:`setting-ofi-spawner` and if that does not
help consider opening a bug as described in :ref:`readme-bugs`.

In some cases, we have found that setting ``HFI_NO_CPUAFFINITY=1``
may be required to get access to all cores.


--------
See Also
--------

For more information on these and other available options when
targeting Omni-Path through GASNet via OFI/libfabric, please refer to
GASNet's official `ofi conduit documentation
<https://gasnet.lbl.gov/dist/ofi-conduit/README>`_, which can also be
found in
``$CHPL_HOME/third-party/gasnet/gasnet-src/ofi-conduit/README``.
