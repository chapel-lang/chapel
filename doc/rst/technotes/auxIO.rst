.. _readme-auxIO:

.. default-domain:: chpl

=====================
Auxiliary I/O Systems
=====================

This document describes Chapel support for Auxiliary I/O (AIO) systems. It also
provides instructions on how to set Chapel up to support multiple Auxiliary I/O
systems simultaneously.

I/O Systems Supported
---------------------

Currently, the I/O systems supported are:

 - Lustre
 - :mod:`HDFS`
 - :mod:`Curl`


.. _auxIO-HDFS-deps:

Setting up HDFS
---------------

HDFS is the Hadoop Distributed Filesystem. This section demonstrates how to set
up a Hadoop installation. If you already have access to an HDFS filesystem, you
can skip ahead to :ref:`enabling-hdfs-support`.

The :mod:`HDFS` functionality in Chapel is dependent Hadoop being
installed.  The ``HADOOP_INSTALL``, ``JAVA_INSTALL`` and ``CLASSPATH``
environment variables must be set as described below.
Without this it will not compile with :mod:`HDFS`, even if
the flags are set. As well, the :mod:`HDFS` functionality is also dependent upon the
``CHPL_AUXIO_INCLUDE`` and ``CHPL_AUXIO_LIBS`` environment variables being set
properly.

If you have a working installation of Hadoop already, you can skip
this section, other than to set up your ``CLASSPATH`` environment
variable.  This section is written so that people without sudo
permission can install and use :mod:`HDFS`.  If you do have sudo permissions,
you can usually install all of these via a package manager.

The general outline for these instructions are:

  * :ref:`Step 1 <setup-hadoop-1>`:  Install and point to the jdk to provide code Chapel needs to
    compile against libhdfs

  * :ref:`Step 2 <setup-hadoop-2>`: Install Hadoop

  * :ref:`Step 3 <setup-hadoop-3>`: Set up Hadoop on (a) the local host or (b) a cluster of hosts

  * :ref:`Step 4 <setup-hadoop-4>`: Start up HDFS

  * :ref:`Step 5 <setup-hadoop-5>`: Stop HDFS when you're done

  * :ref:`Step 6 <setup-hadoop-6>`: Set up Chapel to run in distributed mode

First reflect your directory structure and version numbers (etc) in the
:ref:`sample .bashrc <setup-hadoop-bashrc>` and put it in your .bashrc (or
other shell rc file of your choice) and source whichever one you put it into.

.. _setup-hadoop-1:

1. Make sure you have a SERVER edition of the jdk installed and
   point ``JAVA_INSTALL`` to it (see the
   :ref:`sample .bashrc <setup-hadoop-bashrc>` below)

.. _setup-hadoop-2:

2. Install Hadoop

   * Download the latest version of Hadoop and unpack it

   * Now in the unpacked directory, open ``conf/hadoop-env.sh`` and edit:

     * set ``JAVA_INSTALL`` to be the part before ``bin/`` when you do:

        .. code-block:: sh

          which java

     * set ``HADOOP_CLASSPATH=$HADOOP_HOME/""*:$HADOOP_HOME/lib/""*:``

   * Now in conf/hdfs-site.xml put the replication number that you
     want for the field ``dfs.replication`` (this will set the
     replication of blocks of the files in HDFS)

   * Now set up passwordless ssh, if you haven't yet:

     .. code-block:: sh

       ssh-keygen -t dsa -P '' -f ~/.ssh/id_dsa
       cat ~/.ssh/id_dsa.pub >> ~/.ssh/authorized_keys

.. _setup-hadoop-3:

3. Set up Hadoop

   a. For the local host - See the
      `Hadoop website <http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/SingleCluster.html>`_
      for good documentation on how to do this.

   b. For a cluster of hosts. If you want to run Hadoop over a cluster, there
      are good tutorials online. Although it is usually as easy as making
      edits to the following files in ``$HADOOP_HOME/conf``:

      * adding the name of the nodes to ``slaves``
      * putting what you want to be the namenode in ``masters``
      * putting the master node in ``core-site.xml`` and ``mapred-site.xml``
      * running:

        .. code-block:: sh

         hadoop-daemon.sh start datanode
         hadoop-daemon.sh start tasktracker

      After this go to your datanode site and you should see a new
      datanode.

      A good online tutorial for this as well can be found here on the
      `Hadoop Cluster Setup Documentation <http://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/ClusterSetup.html>`_

.. _setup-hadoop-4:

4. Start HDFS

   * Now all we need to do is format the namenode and start things up:

     .. code-block:: sh

       hadoop namenode -format
       start-all.sh  # (This will start hdfs and the tasktracker/jobtracker)

   * In general, hadoop has the same type of commands as bash,
     usually in the form:

     .. code-block:: sh

         hadoop dfs -<command> <regular args to that command>

   * At this point, you can compile and run Chapel programs using HDFS

   * You can check the status of Hadoop via http, for example on a local
     host (e.g., for :ref:`3a above <setup-hadoop-3>`), using:

       *  ``http://localhost:50070/``
       *  ``http://localhost:50030/``

     For cluster mode (:ref:`3b <setup-hadoop-3>`), you'll use the name of the
     master host in the URL and its port (see the web for details).

.. _setup-hadoop-5:

5. Shut things down:

   .. code-block:: sh

     stop-all.sh   # (This will stop hdfs and mapreduce)

.. _setup-hadoop-6:

6. Set up Chapel to run in distributed mode:

   * You'll need to set up your Chapel environment to target multiple
     locales in the standard way (see :ref:`readme-multilocale` and the
     "Settings to run Chapel on multiple nodes" section of the
     :ref:`setup-hadoop-bashrc` below).

   * After this you should be able to run Chapel code with HDFS over
     a cluster of computers the same way as you normally would.


.. _setup-hadoop-bashrc:

Sample .bashrc
**************

Here is a sample .bashrc for using Hadoop within Chapel:


.. code-block:: sh

  #
  # For Hadoop
  #
  export HADOOP_INSTALL=<Place where you have Hadoop installed>
  export HADOOP_HOME=$HADOOP_INSTALL
  export HADOOP_VERSION=<Your Hadoop version number>
  #
  # Note that the following environment variables might contain more paths than
  # those listed below if you have more than one IO system enabled. These are all
  # that you will need in order to use HDFS (only)
  #
  export CHPL_AUXIO_INCLUDE="-I$JAVA_INSTALL/include -I$JAVA_INSTALL/include/linux  -I$HADOOP_INSTALL/src/c++/libhdfs"
  export CHPL_AUXIO_LIBS="-L$JAVA_INSTALL/jre/lib/amd64/server -L$HADOOP_INSTALL/c++/Linux-amd64-64/lib"

  #
  # So we can run things such as start-all.sh etc. from anywhere and
  # don't need to be in $HADOOP_INSTALL
  #
  export PATH=$PATH:$HADOOP_INSTALL/bin

  #
  # Point to the JDK installation
  #
  export JAVA_INSTALL=<Place where you have the jdk installed>

  #
  # Add Hadoop directories to the Java class path
  #
  export CLASSPATH=$CLASSPATH:$HADOOP_HOME/""*:$HADOOP_HOME/lib/""*:$HADOOP_HOME/conf/""*:$(hadoop classpath):

  #
  # So we don't have to "install" these things
  #
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADOOP_HOME/c++/Linux-amd64-64/lib:$HADOOP_HOME/src/c++/libhdfs:$JAVA_INSTALL/jre/lib/amd64/server:$JAVA_INSTALL:$HADOOP_HOME/lib:$JAVA_INSTALL/jre/lib/amd64:$CLASSPATH

  #
  # Settings to run Chapel on multiple nodes
  #
  export GASNET_SPAWNFN=S
  export SSH_SERVERS=<the names of the computers in your cluster>
  export SSH_CMD=ssh
  export SSH_OPTIONS=-x
  export GASNET_ROUTE_OUTPUT=0


.. _enabling-hdfs-support:

Enabling HDFS Support
*********************


There are two ways to configure Chapel to work with HDFS: using the Java
implementation with libhdfs; or using a C/C++ implementation with libhdfs3.

The user should set their ``CHPL_AUX_FILESYS`` accordingly:

.. code-block:: sh

    # C/C++ implementation
    export CHPL_AUX_FILESYS=hdfs3

.. code-block:: sh

    # Java implementation. Also set environment variables noted above.
    export CHPL_AUX_FILESYS=hdfs

Then, rebuild Chapel by executing ``make`` from ``$CHPL_HOME``.

.. code-block:: sh

  make

.. note::

  If HDFS support is not enabled (which is the default), all
  features described in :mod:`HDFS` will compile successfully but will result
  in an error at runtime such as: "No HDFS Support".

Installing Curl Dependencies
----------------------------

The :mod:`Curl` functionality in Chapel is dependent on libcurl. For
information on how to install libcurl, see the
`curl installation instructions <https://curl.haxx.se/docs/install.html>`_

The environment variables ``CHPL_AUXIO_INCLUDE`` and ``CHPL_AUXIO_LIBS`` must
be set to point to the include and lib directories for libcurl respectively.

.. note::

  If libcurl is installed system-wide you should not need to set these
  variables.


Enabling Curl Support
*********************

Once you have ensured that libcurl is installed, and have the two variables
above defined, set the environment variable ``CHPL_AUX_FILESYS`` to 'curl' to
enable :mod:`Curl` support:

.. code-block:: sh

  export CHPL_AUX_FILESYS=curl

Then, rebuild Chapel by executing ``make`' from ``$CHPL_HOME``:

.. code-block:: sh

  make

.. note::

  If Curl support is not enabled (which is the default), all features
  described below will compile successfully but will result in an error at
  runtime, saying: "No Curl Support".


The AIO system depends upon three environment variables:

    ``CHPL_AUX_FILESYS``
    ``CHPL_AUXIO_INCLUDE``
    ``CHPL_AUXIO_LIBS``

In the following sections, we will explain what they should be set to, and give
the general idea of what they do.


CHPL_AUXIO_INCLUDE & CHPL_AUXIO_LIBS
------------------------------------

These paths are for the extra libraries that will be linked in with the runtime
when it is compiled. For instance, if I installed libcurl, and had it install in
``~/include`` and ``~/lib`` you would set them to be:


.. code-block:: sh

    export CHPL_AUXIO_LIBS="-L~/include"
    export CHPL_AUXIO_INCLUDE="-I~/lib"

In general, you want it so that if you had a .c file that used the libraries
that you wish to compile Chapel with, all you would need to do to compile this
file would be:

``cc $CHPL_AUXIO_LIBS $CHPL_AUXIO_INCLUDE <any libraries> <the .c file>``

where <any libraries> might be ``-lcurl``, ``-lhdfs``, ``-lhdfs3``, ``-ljvm`` etc.

.. note::

  It is not necessary to pass these library flags, or library/include paths
  to the Chapel compiler invocations (chpl) as the values in ``CHPL_AUXIO_LIBS``
  and ``CHPL_AUXIO_INCLUDE`` will be used there as well as in building the
  Chapel runtime

CHPL_AUX_FILESYS
----------------

This is a space delimited string detailing what AIO systems we wish to compile
Chapel with (and use). For example if we wanted to enable :mod:`Curl` and
:mod:`HDFS` support simultaneously we would set:

    ``CHPL_AUX_FILESYS="hdfs curl"``

Assuming that you have correctly defined ``CHPL_AUXIO_INCLUDE`` and
``CHPL_AUXIO_LIBS`` as detailed above, and have the correct libraries
installed.

If you only have one AIO system that you wish to use, you may simply set
``CHPL_AUX_FILESYS=<system>``. For example, if we only wanted Apache Hadoop
HDFS support, we would set:

    ``CHPL_AUX_FILESYS=hdfs``


Parallel and Distributed I/O Features
-------------------------------------

We support two functions for Parallel and Distributed file systems (these also
work on "standard" file systems as well).

``file.getchunk(start:int(64), end:int(64)):(int(64), int(64))``

 - This returns the first logical *chunk* of the file that is inside this
   section. If no *chunk* can be found inside this region, (0,0) is returned.
   If no arguments are provided, we return the start and end of the first
   logical chunk for this file.

     - On Lustre, this returns the first stripe for the file that is inside
       this region.

     - On HDFS, this returns the first block for the file that is inside this
       region.

     - On local file systems, it returns the first *optimal transfer block*
       (from fstatfs) inside this section of the file.

``file.localesForRegion(start:int(64), end:int(64)):domain(locale)``

 - This returns the *best locales* for a given chunk of the file. If no
   individual or set of locales are *best* (i.e., there is some sort of data
   affinity that we can exploit), we return all locales.

     - On Lustre, no locale are *best*, so we return all locales

     - On HDFS, we return the block owners for that specific block

     - On local file systems, we return all locales, since no individual
       locale is best.


