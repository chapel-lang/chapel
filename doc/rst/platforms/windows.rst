.. _readme-windows:

=======================
Using Chapel on Windows
=======================

Chapel can be used on Windows systems in two ways: via the Windows Subsystem
for Linux (WSL) or via Cygwin. The WSL approach is the preferred method for
running Chapel on Windows, as it provides a more native Linux environment and
better performance. The Cygwin approach is supported as a portability option
and should not be expected to result in good performance.


-------------------
Using Chapel on WSL
-------------------

WSL is a compatibility layer for running Linux binary executables natively on
Windows. It provides a Linux-compatible kernel interface, and it can run a
variety of Linux distributions. For more information on WSL, see the
`WSL documentation <https://docs.microsoft.com/en-us/windows/wsl/about>`_.

To use Chapel on WSL, you will need to install a Linux distribution from the
Microsoft Store. We recommend using Ubuntu, but others will likely work.
Once you have installed your distribution, get the list of prerequisites
from :ref:`readme-prereqs` and install Chapel as you would on a native Linux
system. There are no platform-specific settings for Chapel on WSL at this time.

  .. note::

    This configuration is not currently tested nightly. Please report any issues
    you encounter when using Chapel on WSL by `filing a bug report
    <https://github.com/chapel-lang/chapel/issues/new>`_

----------------------
Using Chapel on Cygwin
----------------------

Chapel can be used on both 32-bit and 64-bit installations of Cygwin.
Chapel can be sensitive to directories with spaces, but otherwise it
will work on Cygwin just like any other platform.

   .. note::

     Please note that running Chapel on Cygwin is supported only as a
     portability option and should not be expected to result in good
     performance.  For users wanting to run Chapel on Windows systems,
     the preferred approach is to use the Windows Subsystem for Linux (WSL)
     / Linux Bash Shell.


Unpacking Chapel
----------------

For best results, avoid using WinZip to unpack the release, or make
sure that it is running in a mode that will not convert textfiles to
use Windows-style carriage-return/linefeed end-of-lines -- doing so
can change Makefiles such that they break the build process. We
recommend using Cygwin's gunzip and tar utilities to unpack the
release::

    gunzip chapel.tar.gz
    tar xvf chapel.tar

It is best to unpack the release in a directory whose path contains no
spaces. For example, unpacking the release in the root directory of your
hard-drive (``/cygdrive/c/``) to create ``/cygdrive/c/chapel`` tends to
work well. Currently, our compiler-generated Makefiles break when the
compiler or runtime use an absolute path that contains spaces. If any
Cygwin experts have tips on addressing this issue in a portable way,
please let us know.


Required Packages
-----------------

In order to get the required tools listed in :ref:`readme-prereqs`,
Cygwin users will need to ensure that the following packages are
installed in addition to the standard set of Cygwin packages:

    Devel
      + gcc and gcc-g++
      + make
    Perl
      + perl
    Python
      + python
    Utils
      + diffutils


Additional Information
----------------------

For more information on Cygwin, please refer to:

    http://www.cygwin.com
