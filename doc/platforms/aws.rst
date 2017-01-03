.. _readme-aws:

===================================
Using Chapel on Amazon Web Services
===================================

This page contains AWS virtual machine setup details specific to Chapel.
For more general instructions, see the tutorial on `launching a linux virtual machine`_

.. _launching a linux virtual machine: https://aws.amazon.com/getting-started/tutorials/launch-a-virtual-machine/

Before getting started, you will need an account on AWS: https://aws.amazon.com/

Launching an EC2 instance configured for Chapel
-----------------------------------------------

From the EC2 console, do the following:

1. Begin launching an instance.
2. Choose an Amazon Machine Image (AMI).
   - AMI must use a base OS that supports the `Chapel prerequisites`_, i.e.
   includes a unix-like environment.
3. For multilocale support, create or select a security group configured to
   permit incoming TCP/UDP traffic
4. Review and launch the instance.
5. Create or Select create a private key.
6. `Access the launched the instance`_ via ssh with the selected private key.

.. _Chapel prerequisites: TODO
.. _Access the launched the instance: TODO

[1] region-specific links via http://docs.aws.amazon.com/AWSEC2/latest/UserGuide/using-bookmarks.html
[2] https://us-west-2.console.aws.amazon.com/ec2/v2/home#LaunchInstanceWizard:

On copying across regions:
http://stackoverflow.com/questions/5402013/move-amazon-ec2-amis-between-regions-via-web-interface#14205963

Building Chapel on an EC2 instance
----------------------------------

To build Chapel on an EC2 instance, you can either launch an instance with the
provided AMI, or launch any other AMI and build Chapel yourself.

Starting from an AWS-provided AMI
+++++++++++++++++++++++++++++++++

- Mileage will vary, but in general, you need to:

.. code-block: bash

    sudo yum -y install git gcc gcc-c++ gmp-devel

.. code-block: bash

    sudo apt-get install gcc g++ libgmp3-dev python-dev python-setuptools make

        - Using EC2 as a Chapel development environment
            - Building Chapel
                - Setup using provided AMI
                    - details about the AMI
                - Setup manually (minimal instructions)
        - Using EC2 as a compute node
            - Security Groups
                - Ensure TCP ports are open!
            - Running multilocale programs over EC2 instances
            - Copy pem onto all machines, as well as config
    - Create publicly available AMI


Using the provided AMI
++++++++++++++++++++++

The provided AMI comes with the following:

Chapel 1.14.0 compiler pre-built

sudo apt-get install gcc g++ libgmp3-dev python-dev python-setuptools make libx11-dev`## Configurations:- CHPL_COMM     = {gasnet, none}- CHPL_GMP




= system- CHPL_REGEXP   = re2- CHPL_UNWIND   = {none, libunwind}

* CHPL_LLVM     = {none, llvm}Invoke `printchplenv` to see your curr


ent configuration.## Testing- Test environment built, invoke with `



start_test`## Tooling- chpldoc installed, available in `$PATH`- ch




plvis installed- vim-plugin installed- emacs-mode enabled## Docs-





restructured text documentation in `$CHPL_HOME/doc`- html documentati
on in `$CHPL_HOME/doc/html`


Running multilocale Chapel programs
-----------------------------------

On a single instance
++++++++++++++++++++


Over multiple instances
+++++++++++++++++++++++


Frequently Asked Questions
--------------------------

**How do I resolve the following error when compiling a program?
``virtual memory exhausted: Cannot allocate memory``**

This is a common error on systems with limited memory resources, such as the
free tier of EC2 instances. If you do not wish to launch an instance with more
memory resources, you can create a swap file or swap partition:

https://www.cyberciti.biz/faq/linux-add-a-swap-file-howto/


**How can I run the testing suite over AWS instances?**

This is a planned addition to the paratest (parallel test) functionality, but
is not yet officially supported.


