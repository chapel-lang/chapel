
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
