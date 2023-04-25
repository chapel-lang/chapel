.. _readme-raspberrypi:

============================
Using Chapel on Raspberry Pi
============================

This document describes how to run Chapel on a standalone Raspberry Pi, as well
as multi-locale on a Raspberry Pi cluster.

.. note::

    As the Chapel compiler has modernized, the memory requirements to build it
    have grown. If you find you can’t build Chapel in the memory you have
    available on your system, please let us know.


.. contents::

----------------------------
Configuring for Raspberry Pi
----------------------------

To build Chapel with support for Raspberry Pi, set:

.. code-block:: bash

    export CHPL_ATOMICS=locks

------------------
Clustering the Pis
------------------

If you have a working cluster already, you can forgo this section of the guide.

This section describes just one of several ways to set up a Pi cluster. This
section assumes that the user has already equipped each Pi with an SD card
flashed with the operating system of the users' choice -- for example,
`Raspberry Pi OS Lite (Legacy) <https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2022-09-26/2022-09-22-raspios-buster-armhf-lite.img.xz>`_.

In order to cluster all of the nodes, we must set up password-less SSH. This
guide borrows from `MagPi <https://magpi.raspberrypi.com/articles/build-a-raspberry-pi-cluster-computer>`_.

1. Connect all nodes via Ethernet

Connect each node to a network switch via Ethernet cables. For a cluster with
only two nodes, you can connect the two nodes directly without the need for a
network switch.

2. For each node, set a static IP address in the same subnet.

In this example, we will use ``10.0.0.0`` as our subnet. If your regular network
uses this, choose something different like ``192.168.10.0``.

.. code-block:: bash

    sudo nano /etc/dhcpcd.conf

At the end of the file, add the lines:

.. code-block:: bash

    interface eth0
    static ip_address=10.0.0.1/24

For each node, replace the last digit of the subnet with a new unique value,
such as 2, 3, and so on. You should be able to ping each node -- for example,
from 10.0.0.1:

.. code-block:: bash

    ping 10.0.0.2

1. Generate SSH keys

On each node, generate an SSH key by running the command:

.. code-block:: bash

    ssh-keygen -t rsa

Press RETURN for each prompt and do not create a passphrase when asked.

4. Copy SSH keys to all nodes in the cluster

For each non-master node, run:

.. code-block:: bash

    ssh-copy-id 10.0.0.1

to copy that node's SSH key to the master node. Once that has been done on all
non-master nodes, copy the master node's SSH key to all other nodes in the
cluster by running the same command from the master node, using the IP addresses
of the other nodes as the command argument.

5. Set up a shared filesystem

In order for each node to reference the same Chapel executable, the executable
must be stored in a shared filesystem that all of the nodes in the cluster have
simultaneous access to. There are several ways to do this.

One way is to mount a flash drive and export it as a network file system to the
other nodes in the cluster. The steps to achieve this are described in sections
4.1.1 to 4.1.2 from this `Medium article <https://glmdev.medium.com/building-a-raspberry-pi-cluster-784f0df9afbd>`_.

--------------------------------------
Configuring for Multi-Locale Execution
--------------------------------------

For more in-depth information about GASNet or multilocale execution with Chapel,
refer to the `GASNet documentation`_ and :ref:`readme-multilocale` page,
respectively.

Ensure that the Chapel tree is installed in the shared filesystem (or that the
Chapel tree is built on each node in your cluster), each node is running the
same version of Chapel, and that all nodes in the cluster have SSH access to
each other.

Complete these steps on the master node of your cluster.

1. Set Chapel to use GASNet for multilocale execution

Set the following environment variable:

.. code-block:: bash

    CHPL_COMM=gasnet

2. Set up GASNet environment variables

Set the following GASNet environment variables:

.. code-block:: bash

    GASNET_SPAWNFN=S # Job spawn mechanism, where 'S' means ssh/rsh-based spawn
    GASNET_MASTERIP= # IP address of master node in your cluster, eg. 10.0.0.1
    GASNET_SSH_SERVERS= # Space-delimited list of server names, eg. 10.0.0.1 10.0.0.2 10.0.0.3


-----------------------------------
Running Multilocale Chapel Programs
-----------------------------------

1. Move Chapel executable to shared directory

This is necessary if your Chapel executables are not output to the shared filesystem by default.

eg.

.. code-block:: bash

    mv hello /clusterfs/
    mv hello_real /clusterfs/

2. Run the program

Run the program as you would any other multilocale program:

.. code-block:: bash

    /clusterfs/hello -nl 2


.. _GASNet documentation: https://gasnet.lbl.gov/dist/udp-conduit/README