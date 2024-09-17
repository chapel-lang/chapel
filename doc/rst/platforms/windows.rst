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

This example shows how to install WSL and Ubuntu on Windows 10/11::

    # From PowerShell, run the following command to enable WSL
    wsl --install -d Ubuntu


There are two main approaches for using Chapel on WSL:

1) Install via a prebuilt Chapel package. This is the quickest way to get up
   and running, but it results in a copy of Chapel without GPU support and that
   only supports shared-memory (single-locale) executions. See the list of available
   packages released on the `Chapel GitHub page <https://github.com/chapel-lang/chapel/releases>`_.

2) Build Chapel from source, as with any other UNIX system. This is slightly
   more involved, but supports Chapel's full feature set. See the list of prerequisites
   for your distribution from :ref:`readme-prereqs`


For option 1, see the following example of how to install Chapel on WSL::

    # From the Ubuntu terminal, use wget to download the Chapel package,
    # replacing <chapel_version> and <package_file_name> with the appropriate values
    wget https://github.com/chapel-lang/chapel/releases/download/<chapel_version>/<package_file_name>

    # Verify the package by checking the shasum 256 against the known good value:
    # see https://github.com/chapel-lang/chapel/releases for a list of packages and known good sha256 values
    # if you get the following error:
    # `shasum: standard input: no properly formatted SHA checksum lines found`
    # check that you have 2 spaces between the sha256 value and the filename

    # Install Chapel, replacing <package_file_name> with the appropriate value
    sudo apt-get update
    sudo apt-get install <package_file_name>

    # Test that chpl is available
    chpl --version

There are no platform-specific settings for Chapel on WSL at this time.

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
