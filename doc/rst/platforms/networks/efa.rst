
.. _readme-efa:

==============================================
Using Chapel with Elastic Fabric Adapter (EFA)
==============================================

The Elastic Fabric Adapter (EFA) is a network interface designed to provide
high throughput and low latency for HPC applications on Amazon Web Services
(AWS). Chapel supports EFA through the ``ofi`` communication layer. These docs
do not cover how to set up EFA on AWS, but you can find more information in the
AWS documentation.

At a minimum, to use EFA with Chapel, you will need to set the following
environment variables before building Chapel:

.. code-block:: bash

   export CHPL_COMM=ofi
   export CHPL_COMM_OFI_OOB=pmi2

The current best practice for using Chapel on AWS with EFA is to use
``pcluster`` to create a cluster for you. See :ref:`readme-aws` for more
information.
