.. _readme-aws:

===================================
Using Chapel on Amazon Web Services
===================================

This page contains information on how to use `AWS ParallelCluster
<https://aws.amazon.com/hpc/parallelcluster>`_ to run Chapel in the cloud.
ParallelCluster is a commandline tool that helps you create and manage High
Performance Computing (HPC) clusters in the AWS cloud. It uses a simple
configuration file to create a cluster that can be customized to your needs.
The following steps will guide you through the process of setting up a cluster
and running Chapel programs on it.

Before getting started, you will need an AWS account, which can be created
here: https://aws.amazon.com/

This guide assumes you have the ParallelCluster CLI installed and configured.
If you do not, follow the steps `here
<https://docs.aws.amazon.com/parallelcluster/latest/ug/install-v3-parallelcluster.html>`_.

.. note::

   This document was last updated for ParallelCluster v3.8.0. Other versions may not have the same features and adjustments may be necessary.

Configuring a ParallelCluster
-----------------------------

ParallelCluster uses a configuration file to define the cluster. Running the
``pcluster configure`` command walks through the process of creating a
configuration file for a cluster. The command requires the flag ``-c
CONFIG_NAME``, where ``CONFIG_NAME`` is the path to the YAML file where the
generated config file will go. Listed below are a few key options to consider
when configuring a cluster for Chapel.

* EC2 Key Pair Name
   Make sure to have an EC2 key pair created in the same region you are creating
   the cluster. This key pair will be used to access the instances in the
   cluster.

   .. note::

      If you plan to use ``ubuntu``, make sure the key pair type is ``ED25519``.

* Scheduler
   The default scheduler is ``slurm``. AWS Batch is also available, but not
   currently supported by Chapel.
* Operating System
   The default operating system is ``alinux2``. We recommend using either
   ``alinux2`` or ``ubuntu2204``.

   .. note::

      The default AMI for ``alinux2`` does not have the necessary drivers for
      GPUs. If you plan to use GPUs, we recommend using ``ubuntu2204``.

   .. note::

      ``Amazon Linux 2023`` is preferred, but is not yet available in
      ParallelCluster. If a later version of ParallelCluster includes ``Amazon
      Linux 2023``, we recommend using it.

* Head node instance type
   The default instance type is ``t2.micro``. This is the node that will be
   compiling Chapel programs. We recommend using an instance type with more
   memory, such as ``t2.medium`` or ``m5.large``.
* Number of queues
   This is number of slurm queues that will be created. The default is 1 and
   most users should not need to change this. After selecting the number of
   queues, you will be prompted to enter the name of each queue.
* Number of compute resources for queue
   This is the number of different types of nodes that will be created for this
   queue. The default is 1 and most users should not need to change this.
* Compute instance type for compute resource in queue
   This is the type of instance that will be created for this queue. It is
   recommended to use the same architecture as the head node instance type (i.e.
   don't use an x86 head node and an ARM compute node).
* Maximum instance count
   This is the maximum number of nodes that will be created for this queue. It
   also determines the maximum number of locales that can be used in a Chapel
   program. Four compute nodes is the minimum needed to successfully run
   ``make check``.
* Automate VPC creation? and Automate Subnet creation?
   ParallelCluster runs in a Virtual Private Network (VPC). If you do not have a
   VPC created, you can have ParallelCluster create one for you. If you already
   have a VPC, you can choose to use it. ParallelCluster can also configure the
   head node and compute nodes in several subnet configurations. Chapel works
   well with many of these configurations, but we recommend using a public
   subnet for the head node and a private subnet for the compute nodes.

Following ``pcluster configure``, the generated configuration file will look
something like this:

.. code-block:: yaml

   Region: us-west-1
   Image:
     Os: alinux2
   HeadNode:
     InstanceType: t2.medium
     Networking:
       SubnetId: SUBNETID
     Ssh:
       KeyName: KEYNAME
   Scheduling:
     Scheduler: slurm
     SlurmQueues:
     - Name: queue1
       ComputeResources:
       - Name: c5n18xlarge
         Instances:
         - InstanceType: c5n.18xlarge
         MinCount: 0
         MaxCount: 4
       Networking:
         SubnetIds:
         - SUBNETID

.. note::

  ParallelCluster will use whatever the default region is for the AWS CLI. If
  that is not set or not the desired region, you can set the region at the
  command line as ``pcluster configure -r REGION``. Note that all cluster
  resources will be created in the region specified in the configuration file,
  and that other resources (such as the key pair or additional volumes) must be
  in the same region.

Performance Notes
^^^^^^^^^^^^^^^^^

For best performance, we recommend the following:

* Use a placement group for the compute nodes.
   This will reduce the latency between the nodes and improve performance. This
   requires using an instance type that supports ``cluster`` placement, such as
   ``c5n.18xlarge``.
* Set ``MinCount`` to some non-zero value.
   This will create the compute nodes when the cluster is created, rather than
   waiting for them to be created when the first job is submitted. Using a
   ``MinCount`` of 0 results in significant overhead when running programs. For
   best performance, we recommend setting ``MinCount`` to the same value as
   ``MaxCount``, however this will result in AWS charges for the compute nodes
   even when they are not being used.
* Enable EFA (Elastic Fabric Adapter) for the compute nodes.
   EFA is a network interface for HPC applications that require low-latency and
   high-bandwidth communications between nodes. This requires using an instance
   type that supports EFA, such as ``c5n.18xlarge``.

These additional options can be added to the configuration file:

.. code-block:: yaml

   Region: us-west-1
   Image:
     Os: alinux2
   HeadNode:
     InstanceType: t2.medium
     Networking:
       SubnetId: SUBNETID
     Ssh:
       KeyName: KEYNAME
   Scheduling:
     Scheduler: slurm
     SlurmQueues:
     - Name: queue1
       ComputeResources:
       - Name: c5n18xlarge
         Instances:
         - InstanceType: c5n.18xlarge
         MinCount: 4
         MaxCount: 4
         Efa:
           Enabled: true
       Networking:
         PlacementGroup:
           Enabled: true
         SubnetIds:
         - SUBNETID

It also possible to use instances with GPUs. We recommend using ``G4dn``,
``G5``, ``P3``, or ``P4`` instances. We also recommend not using ``alinux2``
with these instances, as it does not have the necessary drivers for the GPUs.
Instead, use ``ubuntu2204``.

Launching and Connecting
------------------------

To launch the cluster, run the following command:

.. code-block:: bash

   pcluster create-cluster -c CONFIG_NAME -n mycluster

This will start the process of allocating the AWS resources required. To check
the process of the cluster creation, run the following command:

.. code-block:: bash

   pcluster describe-cluster -n mycluster

This will report various details about the cluster, including the status of the
cluster. Once the cluster is in the ``CREATE_COMPLETE`` state, you can access
the head node. To query just the status of the cluster, use ``pcluster
describe-cluster -n mycluster --query clusterStatus``.

Connecting to the head node depends on how the VPC was set up. If the head node
exists in a public subnet, you can connect to it using the public IP address.
If the head node exists in a private subnet, you will need to connect to it
using the AWS session manager.

* Connecting via a public subnet:

   .. code-block:: bash

      ssh -i /path/to/key.pem ec2-user@`pcluster describe-cluster -n mycluster --query headNode.publicIpAddress | tr -d '"'`

   .. note::

      The username may be different depending on the AMI used. The default
      username for Amazon Linux 2 is ``ec2-user``. The default username for
      Ubuntu 22.04 is ``ubuntu``.

   .. note::

      ``key.pem`` is the private key that corresponds to the public key used
      when creating the EC2 key pair, specified in the configuration file.

* Connecting via the AWS session manager:

   Query the instance ID of the head node:

   .. code-block:: bash

      pcluster describe-cluster -n mycluster --query headNode.instanceId

   Open the AWS console and navigate to the EC2 Instances view. Select the head
   node instance (with an ID matching the one queried above) and click the
   "Connect" button. This will open a new window with a list of connection
   options. Select "Session Manager" and click the "Connect" button. This will
   open a new window with a terminal that is connected to the head node. After
   connecting to the node, run ``sudo su ec2-user`` to switch to the default
   user (for Ubuntu, use ``sudo su ubuntu``). Then run ``cd`` to go to the home
   directory.

Building Chapel
---------------

Once connected to the instance via ssh, do the following:

* Install the dependencies as shown on the :ref:`readme-prereqs-installation` page.

    If using a GPU instance, install the CUDA toolkit from the `NVIDIA website <https://developer.nvidia.com/cuda-downloads>`_.

* Download a Chapel release from the `Download <https://chapel-lang.org/download.html>`_ page.
* Build the Chapel release with ``CHPL_COMM=ofi`` as shown on the :ref:`readme-building` page.

   For best results, we recommend running the following prior to building
   Chapel. Users may wish to add this to their ``.bashrc``:

   .. code-block:: bash

      # this path may need to be adjusted, depending on where the Chapel release was downloaded
      . ~/chapel/util/setchplenv.bash

      export CHPL_COMM=ofi
      # if using a cluster without EFA, use FI_PROVIDER=tcp instead
      export FI_PROVIDER=efa

      export CHPL_LAUNCHER=slurm-srun
      export CHPL_LIBFABRIC=system
      export PKG_CONFIG_PATH=/opt/amazon/efa/lib64/pkgconfig/
      export CHPL_COMM_OFI_OOB=pmi2
      PMI2_DIR=/opt/slurm/lib/
      export CHPL_LD_FLAGS="-L$PMI2_DIR -Wl,-rpath,$PMI2_DIR"
      export SLURM_MPI_TYPE=pmi2
      export CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES=true
      export CHPL_RT_COMM_OFI_CONNECT_EAGERLY=true

      # Set this based on the max amount of memory available per-instance
      # Note that EFA currently prevents the use of a heap larger than 96G
      export CHPL_RT_MAX_HEAP_SIZE=75%

   For best performance, users should also set ``export
   FI_EFA_USE_DEVICE_RDMA=1``. This enables higher network performance by using
   the RDMA capabilities of EFA, but it is only available on newer instances.
   If you are unsure if your instance supports this, try setting it and running
   a Chapel program. If the program fails with an error about
   ``FI_EFA_USE_DEVICE_RDMA``, then your instance does not support this
   feature.

   If using a GPU instance, use the following in addition to the above:

   .. code-block:: bash

      export CHPL_LOCALE_MODEL=gpu
      export CHPL_LLVM=bundled
      export CHPL_GPU=nvidia

Running Chapel Programs
-----------------------

If all of the above steps have been completed successfully, you should be able
to use your cluster to run Chapel programs. If you have a cluster with 4 or
more compute nodes, you can run ``make check`` from ``CHPL_HOME`` to test the
Chapel installation. If you have a cluster with less than 4 nodes, you can test
your configuration compile and run the ``hello`` program as shown below:

.. code-block:: bash

   chpl ~/chapel/examples/hello.chpl
   ./hello -nl 2


Cleanup
-------

When you are done with the cluster, you can delete it with the following command:

.. code-block:: bash

   pcluster delete-cluster -n mycluster

This will delete all of the resources associated with the cluster, including
the storage. If you have data on the cluster that you want to keep, you should
back it up before deleting the cluster.

.. note::

   If desired, users can create their own storage volumes and attach them to
   the cluster at configure time. For example, users can add the following to
   their configuration file prior to running ``pcluster create-cluster``:

    .. code-block:: yaml

       SharedStorage:
         - MountDir: /scratch
           Name: scratch
           StorageType: Ebs
           EbsSettings:
             VolumeId: VOLUMEID

    Replace ``VOLUMEID`` with the ID of the volume you want to attach. After
    the cluster is created, the volume will be mounted at ``/scratch`` on both
    the head node and the compute nodes. Users can then use the volume as they
    see fit. When the cluster is deleted, the volume will be detached but not
    deleted. Make sure when creating the volume that it is in the same region
    as the cluster.

    For more information on attaching volumes to a cluster, see the `ParallelCluster documentation <https://docs.aws.amazon.com/parallelcluster/latest/ug/shared-storage-quotas-integration-v3.html>`_.

Frequently Asked Questions
--------------------------

**How do I resolve the following error:**
``virtual memory exhausted: Cannot allocate memory``

This is a common error on systems with limited memory resources, such as the
free tier of EC2 instances. If you do not wish to launch an instance with more
memory resources, you can create a swap file or swap partition.

This can be done on Linux distributions with the following steps:

.. code-block:: bash

    # Log in as root
    sudo -s

    # Create a 512MB swap file (1024 * 512MB = 524288 block size)
    dd if=/dev/zero of=/swapfile1 bs=1024 count=524288

    # Secure swap file
    chown root:root /swapfile1
    chmod 0600 /swapfile1

    # Set up linux swap file
    mkswap /swapfile1

    # Enable swap file
    swapon /swapfile1

Then edit ``/etc/fstab`` to include:

.. code-block:: bash

    /swapfile1 none swap sw 0 0

Enable the new swapfile without rebooting:

.. code-block:: bash

   swapoff -a
   swapon -a

Confirm the swapfile is working:

.. code-block:: bash

   free -m

