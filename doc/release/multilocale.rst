.. _readme-multilocale:

============================
Multilocale Chapel Execution
============================

To run Chapel on multiple locales, the following steps are required:


0. The following information describes a fairly generic use of
   multi-locale Chapel, suitable for networked workstations and
   generic parallel platforms.  For other platforms, refer to the
   :ref:`platform-specific <platforms-index>` documentation for
   additional tips.


#. Set your ``CHPL_COMM`` environment variable to "gasnet".  For example:

   .. code-block:: bash

     export CHPL_COMM=gasnet

   This specifies that you wish to use the GASNet library for
   inter-locale communication.  GASNet is a one-sided communication
   and active message layer being developed by Lawrence Berkeley
   National Lab and UC Berkeley.  For details, refer to:

   * ``$CHPL_HOME/third-party/gasnet/README`` and
   * ``$CHPL_HOME/third-party/README``


#.
   .. _set-comm-conduit:

   Advanced users can set ``CHPL_COMM_SUBSTRATE`` to indicate the GASNet
   conduit that they wish to use.  Novice users can leave this unset
   and Chapel will make a choice for them.  Current defaults are:

     ====================  ===================
     CHPL_TARGET_PLATFORM  CHPL_COMM_SUBSTRATE
     ====================  ===================
     cray-cs                ibv
     cray-xc                aries
     cray-xe                gemini
     cray-xk                gemini
     cray-xt                mpi
     pwr5                   lapi
     pwr6                   ibv
     other                  udp
     ====================  ===================


#.
   .. _set-comm-segment:

   Advanced GASNet users can set ``CHPL_GASNET_SEGMENT`` to choose a
   memory segment to use with GASNet.  Current defaults are:

     ===================  ====================
     CHPL_COMM_SUBSTRATE  CHPL_GASNET_SEGMENT
     ===================  ====================
     aries                fast
     gemini               fast
     ibv                  large
     portals              fast
     other                everything
     ===================  ====================


#.
   .. _remake-the-compiler:

   Re-make the compiler and runtime from ``CHPL_HOME``:

   .. code-block:: bash

     cd $CHPL_HOME
     gmake

   Or:

   .. code-block:: bash

     cd $CHPL_HOME
     make

   Note that the runtime libraries used by the Chapel compiler are
   based on your choices of conduit, substrate, and debugging mode
   (see set-comm-conduit_, set-comm-segment_, and set-comm-debugging_),
   so the runtime directory will need to be recompiled for each
   variation in these settings that you use for a given choice of
   target platform, compiler, and launcher.


#. Set up GASNet environment variables for execution:

   a. If you are using GASNet's udp conduit, set up variables telling GASNet
      how to spawn program instances.  Most of our experience to date has been
      with two options:

      i. To run on a network of workstations to which you have
         access via ssh, configure GASNet as follows:

         .. code-block:: bash

           export GASNET_SPAWNFN=S
           export GASNET_SSH_SERVERS="host1 host2 host3 ..."
           export GASNET_SSH_CMD=ssh
           export GASNET_SSH_OPTIONS=-x

         where host1, host2, host3, ... are the names of the
         workstations that will serve as your Chapel locales.  In
         order to run your Chapel program on k locales, you must
         have k entries in the ``GASNET_SSH_SERVERS`` variable.  To avoid
         typing in passwords for each node, you will probably want
         to use normal ssh-agent/ssh-add capabilities to support
         password-less ssh-ing.

         Also note that if you are using SSH to launch jobs, you might get a
         login banner printed out along with your program's output. We have
         found the following setting useful to disable such printing:

         .. code-block:: bash

           export GASNET_SSH_OPTIONS="-o LogLevel=Error"

         It is also possible to configure GASNet/UDP to launch jobs with
         SLURM using the following command:

         .. code-block:: bash

           export GASNET_SPAWNFN=C
           export GASNET_CSPAWN_CMD="srun -N%N %C"


      #. To simulate multiple Chapel locales with one workstation,
         you can request that GASNet spawn its child functions on
         your local machine using:

         .. code-block:: bash

           export GASNET_SPAWNFN=L

         If you have problems with the local spawn, you may need to
         set ``GASNET_MASTERIP`` to your IP address.


      #. For other options, please refer to:

         ``$CHPL_HOME/third-party/gasnet/GASNet-1.*.*/udp-conduit/README``

   #. If you are using GASNet's ibv conduit, configure the environment to
      launch program instances.  We have experience with these configurations:

      i. For clusters using SLURM, enable slurm-gasnetrun_ibv:

         .. code-block:: bash

           export CHPL_LAUNCHER=slurm-gasnetrun_ibv

         See :ref:`readme-launcher` for other options available, such
         as setting the time limit or selecting the type of node.


      #. To launch InfiniBand jobs with SSH, use the following

         .. code-block:: bash

           export CHPL_LAUNCHER=gasnetrun_ibv
           export GASNET_SSH_SERVERS="host1 host2 host3 ..."
           export GASNET_IBV_SPAWNER=ssh


      #. We've observed job launch hangs in some systems with InfiniBand
         that were resolved by limiting the memory available for
         communication, for example with:

         .. code-block:: bash

           export GASNET_PHYSMEM_MAX=1G


      #. For more information on these and other available GASNet options
         other options, including configuring to launch through MPI,
         please refer to:

         ``$CHPL_HOME/third-party/gasnet/GASNet-1.*.*/ibv-conduit/README``

   #. If you are trying to debug job launch, try adding ``-v`` to your
      program's command line and set:

      .. code-block:: bash

        export GASNET_VERBOSEENV=1

   #. We've had best results with console I/O using:

      .. code-block:: bash

        export GASNET_ROUTE_OUTPUT=0

      but your mileage may vary.


#. Compile your Chapel program as usual:

   .. code-block:: bash

     chpl -o hello6-taskpar-dist $CHPL_HOME/examples/hello6-taskpar-dist.chpl


#. When you compile a Chapel program for multiple locales, you should
   typically see two binaries (e.g., hello6-taskpar-dist and
   hello6-taskpar-dist_real).  The first binary contains code to
   launch the Chapel program onto the compute nodes using a GASNet run
   command.  The second contains the program code itself.  You can use
   the ``-v`` flag to see the commands used to launch your program.  See
   :ref:`readme-launcher` for further details.


#. Multi-locale executions require the number of locales to be
   specified on the command line.  Other than this, execute your
   Chapel program as usual.  For example:

   .. code-block:: bash

     ./hello6-taskpar-dist -nl 2


#.
   .. _set-comm-debugging:

   Advanced users may want to set ``CHPL_COMM_DEBUG`` in order to enable
   GASNet's internal sanity checking. (It is off by default.)
   You need to re-make the compiler and runtime when changing
   this setting (see remake-the-compiler_).
