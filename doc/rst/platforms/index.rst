.. _platforms-index:

Platform-Specific Notes
=======================

Major Platforms
---------------

.. toctree::
   :maxdepth: 1

   macosx
   cray
   windows
   aws
   raspberrypi


.. _platforms-networks:

Networks
--------

Chapel is portable across many different networks. The following sections
provide information on the different networks available in Chapel and how to
target them.

.. toctree::
   :maxdepth: 1

   networks/slingshot
   networks/infiniband
   networks/efa
   networks/ethernet
   networks/aries
   networks/omnipath


.. _platforms-comm-layers:

Communication Layers
--------------------

The Chapel runtime can be built with different communication layers, controlled
with ``CHPL_COMM``. Different layers are preferred depending on the
platform/network. The following sections provide information on the different
communication layers available in Chapel.

.. toctree::
   :maxdepth: 1

   comm-layers/gasnet
   comm-layers/libfabric
   comm-layers/ugni


Older Platforms
---------------

.. toctree::
   :maxdepth: 1

   ibm
