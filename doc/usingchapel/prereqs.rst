.. _readme-prereqs:

====================
Chapel Prerequisites
====================

If you have a standard UNIX environment, a C/C++ compiler, some basic
scripting languages, a GNU-compatible make, and awk installed you should
have no problems getting started with Chapel.


Prerequisites
-------------

In slightly more detail, the following are prerequisites and assumptions
about your environment for using Chapel:

  * You are using an environment that supports standard UNIX commands
    such as: ``cd, mkdir, rm, echo``

  * You have the Bourne shell available at ``/bin/sh``, the C-shell
    available at ``/bin/csh``, 'env' available at ``/usr/bin/env``, and
    that 'env' can locate Perl and Python on your system.

  * You have access to gmake or a GNU-compatible version of make.

  * You have access to standard C and C++ compilers. We test our code
    using a range of compilers on a nightly basis; these include
    relatively recent versions of gcc/g++, clang, and compilers from
    Cray, Intel, and PGI.

  * If you wish to use Chapel's test system, python-setuptools and
    python-devel (or equivalent packages for your platform) are required.

.. _readme-prereqs-installation:

Installation
------------

We have used the following commands to install the above prerequisites:

  * CentOS, Fedora 21::

      sudo yum install gcc gcc-c++ perl python python-devel python-setuptools bash make gawk

  * Fedora 22::

      sudo dnf install gcc gcc-c++ perl python python-devel python-setuptools bash make gawk

  * SLES, openSUSE::

      sudo zypper install gcc gcc-c++ perl python python-devel python-setuptools bash make gawk

  * Debian, Ubuntu::

      sudo apt-get install gcc g++ perl python python-dev python-setuptools bash make mawk

  * FreeBSD::

     sudo pkg install bash python py27-setuptools gmake gawk
