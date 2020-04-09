.. _readme-omnipath:

==========================
Using Chapel with OmniPath
==========================

.. warning::
    PSM is not supported by the current GASNet release. If this is an important
    configuration for you, please contact the Chapel team.

This document describes how to run Chapel across multiple machines in an
OmniPath cluster. The :ref:`readme-multilocale` describes general information
about running Chapel in a multilocale configuration. The OmniPath drivers come
with Gasnet, which is what has been used to launch jobs. Tested using Slurm
16.05 and Gasnet supplied with OPA Basic 10.3.1.

Compiling Chapel
++++++++++++++++

Used the following environment variables:

.. code-block:: bash

    export CHPL_COMM='gasnet'
    export CHPL_LAUNCHER='gasnetrun_psm'
    export CHPL_COMM_SUBSTRATE='psm'
    export CHPL_GASNET_SEGMENT='everything'
    export CHPL_TARGET_CPU='native'

Using Slurm for Job Launch
++++++++++++++++++++++++++

1. Source the Chapel environment script

2. Define the variables given in the above section

3. Set ``HFI_NO_CPUAFFINITY=1`` otherwise no parallelism on each locale will be
   used.
