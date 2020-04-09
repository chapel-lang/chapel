.. _readme-aws:

===================================
Using Chapel on Amazon Web Services
===================================

This page contains Amazon Web Services (AWS) Elastic Cloud Compute (EC2)
virtual machine setup details specific to Chapel. For more general instance
configuration information, refer to the AWS documentation on
`launching a linux virtual machine`_.

.. _launching a linux virtual machine: https://aws.amazon.com/getting-started/tutorials/launch-a-virtual-machine/

Before getting started, you will need an AWS account, which can be created
here: https://aws.amazon.com/

Launching an EC2 instance configured for Chapel
-----------------------------------------------

From the EC2 console, do the following:

1. Begin launching an instance by clicking the **Launch Instance** button.

2. Choose an Amazon Machine Image (AMI) in the **Choose AMI** step.

   - AMI must use a base OS that supports the :ref:`readme-prereqs`, i.e.
     includes a unix-like environment.

3. For multilocale support, create or select a security group configured to
   permit incoming TCP/UDP traffic in the **Configure Security Group** step.

4. Review and launch the instance.

5. Create or select a private key.

   - If creating the key, you will need to download the ``.pem`` identity file.
     This will be used in the next step to access the instance.

6. `Access the launched instance`_ via ssh using the private key chosen before.

   - Summarizing the AWS documentation linked above, you can ssh into the
     instance using the ``.pem`` identity file downloaded in the previous step
     with the following command: ``ssh -i /path/to/key.pem username@hostname``

.. _Access the launched instance: https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/AccessingInstances.html?icmpid=docs_ec2_console


Building Chapel on an EC2 instance
----------------------------------

Once connected to the instance via ssh, do the following:

- Install the dependencies as shown on the :ref:`readme-prereqs-installation` page.
- Download a Chapel release from the `Download`_ page.
- Build the Chapel release as shown on the :ref:`readme-building` page.

  - Build with ``CHPL_COMM=gasnet`` if you plan to run multilocale programs

.. _Download: https://chapel-lang.org/download.html


Running multilocale Chapel programs
-----------------------------------

For more in-depth information about GASNet or multilocale execution with Chapel,
refer to the `GASNet documentation`_ and :ref:`readme-multilocale` page,
respectively.

On a single instance
++++++++++++++++++++

**1. Compile the program**

Compile the program with ``CHPL_COMM=gasnet``.

**2. Set up GASNet environment variables**

Set the following GASNet environment variable:

.. code-block:: sh

    # Job spawn mechanism, where 'L' means localhost spawn
    GASNET_SPAWNFN='L'

**3. Run the program**

Run the program as you would any other multilocale program:

.. code-block:: sh

    ./hello -nl 2

Over multiple instances
+++++++++++++++++++++++

To run a program across multiple EC2 instances, do the following:

**1. Enable password-less ssh between machines**

This can be done by using the existing identity file (the ``.pem``) , or by
using another authentication method, such as RSA ssh keys.

If using the identity file, copy the identity file onto each instance
into the same path, such as ``~/.ssh/foo.pem``. By default, using this file
requires passing the identity flag and the file path to `ssh`:

.. code-block:: sh

   ssh -i ~/.ssh/foo.pem ec2-11-222-33-444.us-west-2.compute.amazonaws.com

This can be made the default behavior by adding this rule to a new or existing
``~/.ssh/config``:

.. code-block:: text

   Hostname *compute.amazonaws.com
   IdentityFile ~/.ssh/foo.pem

Copy this config file into ``~/.ssh/config`` on every EC2 instance as well.
You should now be able to ssh freely between the EC2 instances.

.. tip::

    The option ``StrictHostKeyChecking no`` can be appended to the new
    ``.ssh/config`` rule to override the trusted host prompt when first
    connecting to each machine.  This can be convenient when deploying a large
    number of instances, but is only recommended if you understand the security
    implications of the change.

**2. Compile and distribute the binary**

Compile the program with ``CHPL_COMM=gasnet`` set, and copy the compiled
binary onto all of the EC2 instances, under the same path. For example:

.. code-block:: sh

    export CHPL_COMM=gasnet
    cd ~/chapel-projects
    chpl hello.chpl -o hello
    scp hello ec2-11-222-33-444.us-west-2.compute.amazonaws.com:chapel-projects/hello
    scp hello ec2-11-222-33-445.us-west-2.compute.amazonaws.com:chapel-projects/hello

**3. Set up GASNet environment variables**

There are several configuration options available for GASNet, which can be
found in the `GASNet documentation`_.
The essential configurations, with examples, are as follows:

.. code-block:: sh

    # Space-delimited list of server names
    GASNET_SSH_SERVERS='ec2-11-222-33-444.us-west-2.compute.amazonaws.com ec2-11-222-33-445.us-west-2.compute.amazonaws.com'
    # Job spawn mechanism, where 'S' means ssh/rsh-based spawn
    GASNET_SPAWNFN='S'

Some other common optional configurations are:

.. code-block:: sh

    # Defaults to current working directory
    GASNET_REMOTE_PATH='~/chapel-projects/'
    # Defaults to gethostname() of the launching node
    CHPL_RT_MASTERIP='ec2-11-222-33-444.us-west-2.compute.amazonaws.com'
    # Defaults to empty, can be used instead of copying config files onto each machine
    SSH_OPTIONS='-i ~/.ssh/foo.pem'

See :ref:`chpl-rt-masterip` for details on that environment variable.

.. _GASNet documentation: https://gasnet.lbl.gov/dist/udp-conduit/README

**4. Run the program**

Run the program as you would any other multilocale program:

.. code-block:: sh

    ./hello -nl 2

.. note::
    GASNet is not configured to oversubscribe locales by default. That is, the
    number of locales (``-nl``) provided cannot exceed the number of servers in
    ``GASNET_SSH_SERVERS``. If you wish to oversubscribe nodes, you can include
    servers in ``GASNET_SSH_SERVERS`` multiple times, to reach the desired number
    of locales.


Frequently Asked Questions
--------------------------

**How do I resolve the following error:**
``virtual memory exhausted: Cannot allocate memory``

This is a common error on systems with limited memory resources, such as the
free tier of EC2 instances. If you do not wish to launch an instance with more
memory resources, you can create a swap file or swap partition.

This can be done on Linux distributions with the following steps:

.. code-block:: sh

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

.. code-block:: sh

    /swapfile1 none swap sw 0 0

Enable the new swapfile without rebooting:

.. code-block:: sh

   swapoff -a
   swapon -a

Confirm the swapfile is working:

.. code-block:: sh

   free -m

**How can I run the testing suite in parallel over EC2 instances?**

This is a planned addition to the paratest (parallel test) functionality, but
is not yet officially supported.

