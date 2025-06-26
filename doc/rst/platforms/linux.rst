.. _readme-linux:

=====================
Using Chapel on Linux
=====================

There are two main approaches for using Chapel on Linux:

1) Install via a Linux package manager. If you are using a Linux
   distribution that provides a Chapel package, this is the easiest way
   to get started.

2) Build Chapel from source.

---------------
Package Install
---------------

Chapel provides prebuilt apt/rpm packages for several Linux distributions.
These packages can be downloaded and installed using the package manager for
your distribution. For example, on x86 Ubuntu 24, you can install Chapel 2.4.0
with the following commands:

.. code-block:: bash

   wget https://github.com/chapel-lang/chapel/releases/download/2.4.0/chapel-2.4.0-1.ubuntu24.amd64.deb
   sudo apt-get install ./chapel-2.4.0-1.ubuntu24.amd64.deb

See the full list of supported packages `here
<https://chapel-lang.org/download/#linux>`__. In addition to these apt/apm
packages, there is also a Chapel `AUR
<https://aur.archlinux.org/packages/chapel>`__.

---------
Linuxbrew
---------

For users of Linuxbrew_, Chapel can be installed with the following commands:

.. code-block:: bash

   brew update
   brew install chapel

For more information on using Chapel with Linuxbrew, see the
:ref:`docs on using Homebrew <macosx-homebrew>` with Chapel.

--------------------
Building from Source
--------------------

Chapel can be built from source on Linux systems using the normal build steps.
See :ref:`readme-prereqs` for a list of prerequisites and follow
:ref:`readme-building` for the build instructions.


.. _Linuxbrew: https://docs.brew.sh/Homebrew-on-Linux
