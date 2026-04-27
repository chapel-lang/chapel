
.. _readme-slingshot:

===========================
Using Chapel with Slingshot
===========================

HPE Slingshot is a high-performance network technology designed for large-scale
supercomputing systems. Chapel supports targeting Slingshot networks using the
:ref:`readme-libfabric` communication layer.

The most common platform with Slingshot is the HPE Cray EX. See
:ref:`readme-hpe-cray-ex` for more information on using Chapel with HPE Cray EX
systems.

To target Slingshot with Chapel, you should build Chapel with the
``CHPL_COMM=ofi`` environment variable set, which enables the use of the
libfabric library. See :ref:`readme-libfabric` for more information on building
Chapel with libfabric.

------------------------------
Cross-compiling for Slingshot
------------------------------

If you are building Chapel on one system for execution on a different
Slingshot-based system (cross-compiling), set
:ref:`readme-chplenv.CHPL_TARGET_PLATFORM` to the target platform in addition
to ``CHPL_COMM=ofi``. In most cases, the target platform for a Slingshot system
will be ``hpe-cray-ex``.

.. code-block:: bash

   export CHPL_TARGET_PLATFORM=hpe-cray-ex
   export CHPL_COMM=ofi
   make

If Chapel does not correctly infer the build system, also set
:ref:`readme-chplenv.CHPL_HOST_PLATFORM`.

When cross-compiling to an HPE Cray EX system, see
:ref:`readme-hpe-cray-ex` for target-compiler, launcher, and PMI-related
settings. Provider selection happens at run time, so no additional Chapel
build-time option is required to request the ``cxi`` provider specifically.

If the build host is not itself a Slingshot system, also make sure that the
libfabric installation used for the build has ``cxi`` support. See
:ref:`readme-libfabric` for more information, including the public HPE CXI
inputs required by this build path.

After building Chapel, compile your Chapel program with the same target
settings and run the resulting executable on the target Slingshot system.

