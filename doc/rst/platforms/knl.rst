.. _readme-knl:

=======================================
Using Chapel on Intel "Knights Landing"
=======================================

The following information is assembled to help Chapel users get up and
running on Intel Xeon Phi, Knights Landing (KNL).

The initial implementation has only been tested on Cray machines where
the KNL chip is used as a self-hosted processor. While we have not
explicitly tested on other platforms, or with KNL as a Coprocessor, we
don't know of any reason an advanced user couldn't run on such a
platform.

---------------
Getting started
---------------

By and large running on KNL on a Cray XC will be the same as running on
a Xeon based Cray XC. See :ref:`readme-cray` for more info. 

In order to take better advantage of the AVX-512 micro-architecture
you'll want to have a KNL targeting module loaded. For example::
    
  module load craype-mic-knl

We provide a KNL locale model for making use of the MCDRAM (High
Bandwidth Memory/HBM).  Please see :ref:`readme-KNLlm` for details.

KNL provides many memory configurations and clustering modes.  If a
program will manage HBM explicitly using the KNL locale model, the
``flat`` memory configuration is a good place to start.  If not, the
``cache`` configuration will use the HBM as a level 3 cache.  Two
in-between configurations are available, ``equal`` for 50% cache/50%
explicitly managed, and ``split`` for 25% cache/75% explicitly
managed.  It is likely that the highest performing configuration is
different for different programs, so it pays to experiment.

So far, the clustering modes that seem to have the most promise for
Chapel programs are ``quad`` and ``snc4``.  The ``quad`` mode,
contrary to its name, places all the KNL cores in one NUMA node.  The
``snc4`` mode splits the cores equally into four NUMA nodes.  Again,
experimentation will tell which works best for a given program, and
other clustering modes may be worth trying as well.

The method of choosing the clustering mode and memory configuration is
different for different machine installations.  Please see your system
administrator for more information on this.  When using Slurm, a
common method is to use the ``--constraint=`` flag to ``srun``.  For
Chapel, this flag can be set using the ``CHPL_LAUNCHER_CONSTRAINT``
environment variable.  For example, to use the ``snc4`` clustering
mode and the ``flat`` memory model, the following would be used.

.. code-block:: sh

    export CHPL_LAUNCHER_CONSTRAINT=snc4,flat
