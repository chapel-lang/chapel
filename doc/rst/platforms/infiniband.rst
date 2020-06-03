.. _readme-infiniband:

============================
Using Chapel with InfiniBand
============================

This document describes how to run Chapel across multiple machines in an
InfiniBand cluster.  :ref:`readme-multilocale` describes general
information about running Chapel in a multilocale configuration.

Avoiding Slow Job Launch
++++++++++++++++++++++++

We've observed very slow job launch on some systems with InfiniBand
that were resolved by limiting the memory available for
communication, for example with:

.. code-block:: bash

   export GASNET_PHYSMEM_MAX=1G

Note that setting ``GASNET_PHYSMEM_MAX`` will limit amount of memory
available to Chapel programs if ``CHPL_GASNET_SEGMENT=fast``.

It might be necessary to also set ``GASNET_PHYSMEM_NOPROBE=1`` -
especially if you increase the size of ``GASNET_PHYSMEM_MAX``.

It's probably a good idea to start with this variable set
and to try removing it once everything else is working.

Using Slurm for Job Launch
++++++++++++++++++++++++++

For clusters using Slurm, there are a few options:

a. The current best option for InfiniBand+Slurm is
   ``CHPL_LAUNCHER=slurm-gasnetrun_ibv``:

   .. code-block:: bash

     export CHPL_COMM=gasnet
     export CHPL_COMM_SUBSTRATE=ibv

     export CHPL_LAUNCHER=slurm-gasnetrun_ibv

     # Rebuild the Chapel runtime for these settings
     cd $CHPL_HOME
     make

     # Compile a sample program
     chpl -o hello6-taskpar-dist examples/hello6-taskpar-dist.chpl

   See :ref:`using-slurm` for other options available, such
   as setting the time limit or selecting the type of node.
   Some settings might be required by your Slurm configuration.
   Setting these variables are typically necessary:

   .. code-block:: bash

      # Specify the Slurm partition to use
      export CHPL_LAUNCHER_PARTITION=debug

      # Run the sample program
      ./hello6-taskpar-dist -nl 2

#. An alternative is to use an ssh spawner and configure it to use the
   nodes allocated by Slurm.

   .. code-block:: bash

      export CHPL_COMM=gasnet
      export CHPL_COMM_SUBSTRATE=ibv

      export CHPL_LAUNCHER=gasnetrun_ibv
      
      # Rebuild the Chapel runtime for these settings
      cd $CHPL_HOME
      make

      # Compile a sample program
      chpl -o hello6-taskpar-dist examples/hello6-taskpar-dist.chpl

   Now, to run a program, reserve some nodes with `salloc` and then
   within the resulting shell, configure the servers to SSH and run
   the program:

   .. code-block:: bash

      # Reserve 2 nodes for an interactive run
      salloc -N 2
      # Then, within the salloc shell

        # Specify that ssh should be used
        export GASNET_IBV_SPAWNER=ssh
        # Run the program on the 2 reserved nodes.
        # gasnetrun_ibv will use the nodes Slurm allocated above.
        ./hello6-taskpar-dist -nl 2

   This technique is also possible when using `sbatch`. In that case,
   make sure your `sbatch` script includes the line:

   .. code-block:: bash

      export GASNET_IBV_SPAWNER=ssh

   See :ref:`ssh-launchers-with-slurm` for more information on these
   techniques.

#. A further alternative is to configure GASNet to use *mpirun* to launch your
   program. *mpirun* might already be configured to work with Slurm. See
   using-mpi-for-job-launch_.

Using SSH for Job Launch
++++++++++++++++++++++++

To launch InfiniBand jobs with SSH, use the following

.. code-block:: bash

   export CHPL_COMM=gasnet
   export CHPL_COMM_SUBSTRATE=ibv

   export CHPL_LAUNCHER=gasnetrun_ibv
  
   # Rebuild the Chapel runtime for these settings
   cd $CHPL_HOME
   make

   # Compile a sample program
   chpl -o hello6-taskpar-dist examples/hello6-taskpar-dist.chpl

   # Specify that ssh should be used
   export GASNET_IBV_SPAWNER=ssh
   # Specify the nodes to run on
   export GASNET_SSH_SERVERS="host1 host2 host3 ..."

.. _using-mpi-for-job-launch:

Using MPI for Job Launch
++++++++++++++++++++++++

To launch InfiniBand jobs with *mpirun*, first make sure that *mpicc* is
available and that MPI programs launch appropriately with *mpirun*. Then use
the following. You'll want to make sure that GASNet detects MPI in its
configuration output.

.. code-block:: bash

   export CHPL_COMM=gasnet
   export CHPL_COMM_SUBSTRATE=ibv

   export CHPL_LAUNCHER=gasnetrun_ibv
  
   # Rebuild the Chapel runtime for these settings
   cd $CHPL_HOME
   make

   # Compile a sample program
   chpl -o hello6-taskpar-dist examples/hello6-taskpar-dist.chpl

   # Specify that ssh should be used
   export GASNET_IBV_SPAWNER=mpi



See Also
++++++++

For more information on these and other available GASNet options,
including configuring to launch through MPI, please refer to
GASNet's official `InfiniBand conduit documentation
<https://gasnet.lbl.gov/dist/ibv-conduit/README>`_, which can also be found
in ``$CHPL_HOME/third-party/gasnet/gasnet-src/ibv-conduit/README``.

