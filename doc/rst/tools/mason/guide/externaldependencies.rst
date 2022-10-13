:title: Mason Guide: *External Dependencies*

Using Non-Chapel Dependencies
=============================
Mason allows for specification of external, non-Chapel dependencies through two
mediums, ``Spack`` and ``pkg-config``. The following two sections document how to
use ``mason external`` and ``mason system`` to interface with the ``Spack`` and ``pkg-config``
packages respectively.



Using System Dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~

System dependencies are packages that are found on your system through ``pkg-config``. To use
this functionality of Mason, users must have ``pkg-config`` installed.

Mason interfaces with ``pkg-config`` through the ``mason system`` command.

``mason system search`` will print all the current packages installed and available for use in a Mason package. To examine
the ``.pc`` file of a particular package, use ``mason system pc <package>`` where ``<package>``
is replaced with the particular package you are looking for. Here is an example of a workflow
for creating a Mason package with ``openssl`` which has already been installed.

First, search to see that it is installed with ``mason system search openSSl`` which outputs:

.. code-block:: sh

   $ mason system search openssl
   openssl               OpenSSL - Secure Sockets Layer and cryptography libraries and tools

To find out more about the package, since it is in fact installed on my system, use the
``mason system pc`` command as follows

.. code-block:: sh

   $ mason system pc openssl

   ------- openSSL.pc -------

   prefix=/usr
   exec_prefix=${prefix}
   libdir=${exec_prefix}/lib
   includedir=${prefix}/include

   Name: OpenSSL
   Description: Secure Sockets Layer and cryptography libraries and tools
   Version: 0.9.8zh
   Requires:
   Libs: -L${libdir} -lssl -lcrypto -lz
   Cflags: -I${includedir}

   -------------------


Use the ``mason add --system`` command to add the dependency to the Mason.toml of the package.

.. code-block:: sh

   $ mason add --system openSSL@0.9.8zh
   Adding system dependency openSSL version 0.9.8zh

The ``Mason.toml`` now looks like:

.. code-block:: text

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [system]
   openSSL = "0.9.8zh"

Now, upon calling ``mason build`` or ``mason run --build``, Mason will go get ``openssl`` and include it
in the package so that it can be used as a dependency.


Using Spack Dependencies
~~~~~~~~~~~~~~~~~~~~~~~~

Mason users can interface with ``Spack``, a package manager geared towards high performance
computing, through the ``mason external`` command. For a list of available ``Spack`` packages,
see: `packages <https://spack.readthedocs.io/en/latest/package_list.html#package-list>`_.

This interface is analogous to the previous example except when a package is missing, user's can download that package
through the Spack integration. The following is a workflow of finding, installing, and adding a Spack dependency to a Mason Package.

**Setting up Spack backend**

First, the Spack backend must be installed. Users can have mason install Spack
or point mason to an existing spack installation.

This command will install Spack into ``$MASON_HOME/spack`` and set it up so that it
can be used by Mason. It should be noted that this command pulls from the `master` branch of spack
for setting up the spack registry at ``$MASON_HOME/spack-registry``::

  mason external --setup

Alternatively, users can set ``$SPACK_ROOT`` to their own spack installation::

  export SPACK_ROOT=/path/to/spack

**Searching Spack packages**

Let's use ``openSSL`` as an example since we used it in the system example. ``mason external search openSSL``
will search for the package and produce the following output::

  $ mason external search openSSL
  ==> 2 packages.
  openssl  r-openssl

Since there are two types of the package listed, we need to figure out which one to use.
To find out more about a package, use ``mason external info <package>`` as follows::

  $ mason external info openssl
  Package:   openssl

  Description:
  OpenSSL is an open source package that provides a robust, commercial-
  grade, and full-featured toolkit for the Transport Layer Security (TLS)
  and Secure Sockets Layer (SSL) protocols. It is also a general-purpose
  cryptography library.

  Homepage: http://www.openssl.org

  Tags:
    None

  Preferred version:
    1.0.2k    http://www.openssl.org/source/openssl-1.0.2k.tar.gz

  Safe versions:
    1.1.0e    http://www.openssl.org/source/openssl-1.1.0e.tar.gz
    1.1.0d    http://www.openssl.org/source/openssl-1.1.0d.tar.gz
    1.1.0c    http://www.openssl.org/source/openssl-1.1.0c.tar.gz
    1.0.2k    http://www.openssl.org/source/openssl-1.0.2k.tar.gz
    1.0.2j    http://www.openssl.org/source/openssl-1.0.2j.tar.gz

  Variants:
   None

  Installation Phases:
   install

  Build Dependencies:
   zlib

  Link Dependencies:
   zlib

  Run Dependencies:
   None

  Virtual Packages:
   None


**Installing Spack packages**

The correct package has been found, but not yet installed. Let's fix that.
We know that we want the preferred version which is ``1.0.2k``.
The command to install ``openssl`` version ``1.0.2k`` would be::

  mason external install openssl

Since the version was left out, version ``1.0.2k`` is used because Mason
will always take the preferred version. This is a case where Spack's
spec expression syntax can be used to specify exactly which package is desired.
For example, another way to install openSSL would be::

  mason external install openssl@1.0.2k

which simply specifies the exact version that we want.
If we wanted to specify which compiler the package was built with::

  mason external install openssl@1.0.2k%gcc

Mason will infer which compiler, in the case that the compiler is left out of the spec,
by looking at the environment variable ``CHPL_TARGET_COMPILER``. For more information on
how to use spec expressions, use the command ``mason external --spec`` which will output the following:

.. code-block:: text

   spec expression syntax:

  package [constraints] [^dependency [constraints] ...]

  package                           any package from 'spack list'

  constraints:
    versions:
      @version                      single version
      @min:max                      version range (inclusive)
      @min:                         version <min> or higher
      @:max                         up to version <max> (inclusive)

    compilers:
      %compiler                     build with <compiler>
      %compiler@version             build with specific compiler version
      %compiler@min:max             specific version range (see above)

    variants:
      +variant                      enable <variant>
      -variant or ~variant          disable <variant>
      variant=value                 set non-boolean <variant> to <value>
      variant=value1,value2,value3  set multi-value <variant> values

    architecture variants:
      target=target                 specific <target> processor
      os=operating_system           specific <operating_system>
      platform=platform             linux, darwin, cray, bgq, etc.
      arch=platform-os-target       shortcut for all three above

    cross-compiling:
      os=backend or os=be           build for compute node (backend)
      os=frontend or os=fe          build for login node (frontend)

    dependencies:
      ^dependency [constraints]     specify constraints on dependencies

  examples:
      hdf5                          any hdf5 configuration
      hdf5 @1.10.1                  hdf5 version 1.10.1
      hdf5 @1.8:                    hdf5 1.8 or higher
      hdf5 @1.8: %gcc               hdf5 1.8 or higher built with gcc
      hdf5 +mpi                     hdf5 with mpi enabled
      hdf5 ~mpi                     hdf5 with mpi disabled
      hdf5 +mpi ^mpich              hdf5 with mpi, using mpich
      hdf5 +mpi ^openmpi@1.7        hdf5 with mpi, using openmpi 1.7
      boxlib dim=2                  boxlib built for 2 dimensions
      libdwarf %intel ^libelf%gcc
          libdwarf, built with intel compiler, linked to libelf built with gcc
      mvapich2 %pgi fabrics=psm,mrail,sock
          mvapich2, built with pgi compiler, with support for multiple fabrics


Resuming the example, the result of the install given ``openssl`` as the sole argument would output the following::

  $ mason external install openssl
  ==> Installing zlib
  ==> Fetching http://zlib.net/fossils/zlib-1.2.11.tar.gz
  ==> Staging archive: /$HOME/.mason/spack/var/spack/stage/zlib-1.2.11-cpdvq4e7otjepbwdtxmgk5bzszze27fj/zlib-1.2.11.tar.gz
  ==> Created stage in /$HOME/.mason/spack/var/spack/stage/zlib-1.2.11-cpdvq4e7otjepbwdtxmgk5bzszze27fj
  ==> No patches needed for zlib
  ==> Building zlib [Package]
  ==> Executing phase: 'install'
  ==> Successfully installed zlib
  Fetch: 4.84s.  Build: 4.24s.  Total: 9.08s.

  ==> Installing openssl
  ==> Fetching http://www.openssl.org/source/openssl-1.0.2k.tar.gz
  ==> Staging archive: /$HOME/.mason/spack/var/spack/stage/openssl-1.0.2k-fwnsee6qcvbbgvmgp3f5hio6dwg6nh2d/openssl-1.0.2k.tar.gz
  ==> Created stage in /$HOME/.mason/spack/var/spack/stage/openssl-1.0.2k-fwnsee6qcvbbgvmgp3f5hio6dwg6nh2d
  ==> No patches needed for openssl
  ==> Building openssl [Package]
  ==> Executing phase: 'install'
  ==> Successfully installed openssl
  Fetch: 3.37s.  Build: 3m 11.76s.  Total: 3m 15.13s.
  ######################################################################## 100.0%
  ######################################################################## 100.0%

As shown, Mason not only goes and gets the package specified, but also all of the dependencies
of the package specified. Packages are installed into unique directories such that it is impossible for package namespaces to collide.
Each dependency is downloaded distinctly for a package so no previous installs will be broken by installing new packages.
This way, multiple versions and builds of a package can be installed on a system and used without clashing with each other.

**Specifying Spack packages in the manifest file**

Now that the correct package is installed, add it to the ``Mason.toml`` as follows::

  $ mason add --external openssl@1.0.2k
  Adding external dependency with spec openssl@1.0.2k



The ``Mason.toml`` now looks like:

.. code-block:: text

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [external]
   openSSL = "1.0.2k"

To ensure the package is installed on the system, run ``mason external find``
which will list all of the current Spack packages installed on system. For example::


  ==> 2 installed packages.
  -- darwin-sierra-x86_64 / clang@9.0.0-apple ---------------------
  openssl@1.0.2k  zlib@1.2.11


Now, everything necessary to use ``openssl`` in a Mason package has been done.
Upon building, Mason will retrieve the necessary files and file locations
for building ``myPackage`` with ``openssl``.
