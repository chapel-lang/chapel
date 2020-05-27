.. _readme-mason:

=====
Mason
=====

Mason is Chapel's package manager

.. contents::


Installation Instructions
=========================

In ``$CHPL_HOME`` run the following:

.. code-block:: sh

   make mason

It builds the mason binary so that the command line interface can be used.
This installs mason in the same place as the chapel compiler (``chpl``) so that
mason can be used anywhere in the user's file system.

To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean


Basic Usage
===========

Starting a New Package
~~~~~~~~~~~~~~~~~~~~~~

To initialize a new mason package, run the ``mason new [ package name ] [ options ]`` command, for example::

    mason new MyPackage

This creates a git repository by default, unless ``--no-vcs`` is included.

Mason packages can also be initialized using the ``mason init [options] [directory path]`` or 
``mason init [options]`` command outside or inside the project directory respectively. 

For example, for an existing directory named MyPackage, 
    
  .. code-block:: sh
    
    mason init MyPackage 

    # OR 

    cd MyPackage
    mason init  


The package will have the following hierarchy::

  MyPackage/
   │
   ├── Mason.toml
   ├── example/
   ├── src/
   │   └── MyPackage.chpl
   └── test/


The first file listed is the ``Mason.toml``. This is the manifest file
for the package. All dependencies for the package are listed in this file
as well as additional metadata about the package.

The ``src/`` folder is where the source code of the package should reside.
As you might expect, the ``test/`` folder and the ``example`` folder hold
tests and examples for your package, respectively. We will get to the
additional functionality that comes with these folders later.

Mason enforces that the main file be named after the package to enforce namespacing.
``MyPackage.chpl`` will be the first file listed in ``src/``.

You can create a package in a directory that differs from the mason 
package name with the `mason {new,init} --name` flag.
This may be useful when creating a package in a directory that 
is an illegal Mason package name, such as names with dashes. For example, 

  .. code-block:: sh

    mason new illegal-module-name --name LegalModuleName

    # OR

    mkdir illegal-module-name
    mason init illegal-module-name --name LegalModuleName


Building and Running
~~~~~~~~~~~~~~~~~~~~

When invoked, ``mason build [ options ]`` will do the following:

    - Run update to make sure any manual manifest edits are reflected in the dependency code.
    - Build ``MyPackage.chpl`` in the ``src/`` directory.
    - All packages are compiled into binaries and placed into ``target/``
    - All options not recognized by ``mason`` will be forwarded to the chapel compiler(``chpl``)

``mason run [ options ]`` will, in turn:

    - Run the executable built above out of ``target/``, if it exists.
    - All options not recognized by ``mason`` will be forwarded to the executable.

For example, after ``mason run --build [ options ]``, the package directory appears as follows::

  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage
   │   ├── example/
   │   └── test/
   └── test/


As you can see, new files have been added to the package, the first of which
is the ``Mason.lock``. You can think of this file as a snapshot of a single
run of the program. This file "locks" in the settings in which the program
ran upon invocation of ``mason run``. This file can be generated manually
with the ``mason update`` command. ``mason update`` will read the ``Mason.toml``,
resolve dependencies, and generate the ``Mason.lock`` based on it's contents.

The ``target/`` directory is where Mason stores all the binaries related to your package.
These could be binaries for the main source code as well as examples and tests. There are two types of
targets for building. The default location of a package binary is ``target/debug/``, as shown in the
example above. However, if a final version of an application or library is
being produced, the ``--release`` flag can be thrown as follows:

.. code-block:: sh

   mason run --build --release --force

The ``--release`` option adds the ``--fast`` argument to the compilation step.

The argument ``--force`` is included as Mason will only build the package if
the package has been modified. Throwing the ``--release`` flag will result in
the following package structure::

  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage
   │   ├── example/
   │   ├── release/
   │   │   └── myPackage
   │   └── test/
   └── test/

As you can see there are now two binaries of ``MyPackage``, one under ``debug/`` and
one under ``release``.  To remove the ``target/`` directory along with all of the binaries
for your package, use the ``mason clean`` command.


Building Larger Packages
~~~~~~~~~~~~~~~~~~~~~~~~

For packages that span multiple files, the main module is designated by the module that
shares the name with the package directory and the name field in the ``Mason.toml``.


For packages that span multiple sub-directories within ``src``, sub-directories must be passed
to Mason with the ``-M  <src/subdirectory>`` flag which is forwarded to the chapel compiler. For example, lets say
MyPackage's structure is as follows::

  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   ├── src/
   │   ├── myPackage.chpl
   │   └── util/
   │       └── myPackageUtils.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage
   │   ├── example/
   │   └── test/
   └── test/


If MyPackage needs multiple files in different directories like the example above,
then call ``mason build`` with the ``-M`` flag followed by the local dependencies.
A full command of this example would be::

  mason build -M src/util/MyPackageUtils.chpl


Runtime/Compilation Arguments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For an example of forwarding arguments in a call to ``mason run``, a chapel program built in
mason might have a ``config const number`` that corresponds to a value used in ``MyPackage.chpl``.
To try out different values at runtime, pass the values for ``number`` to ``mason run`` as follows::

      mason run --number=100
      mason run --number=1000


.. note::

   For the case when a flag intended for the ``chpl`` compiler or executable is recognized by
   ``mason build`` or ``mason run``, respectively, the flag can be thrown after ``--``
   to override this conflict. For example, ``mason run -- -nl 4``. Instead of mason recognizing
   this argument, this command will run the executable over 4 locales.


Testing your Package
~~~~~~~~~~~~~~~~~~~~

Mason provides the functionality to test packages through the ``mason test``
subcommand. There are two styles of writing mason tests:

1. Tests that utilize the `UnitTest`` module to determine pass/fail status
2. Tests that rely on the exit code to determine pass/fail status

Here is an example of a ``UnitTest``-based tests:

.. code-block:: chpl

  use UnitTest;

  config const testParam: bool = true;

  proc myTest(test: borrowed Test) throws{
    test.assertTrue(testParam);
  }

  UnitTest.main();

Mason testing that uses ``UnitTest`` will treat each individual function as a
test, and the test will be considered successful if no assertions failed and no
halts were reached within the function body.

See the :chpl:mod:`UnitTest` documentation to learn more about writing unit tests in
Chapel.

Here is an example of an exit-code-based tests:

.. code-block:: chpl

   config const testParam: bool = true;

   if testParam {
     writeln("Test Passed!");
   }
   else {
     exit(1);
   }

Mason testing that relies on exit code tests each file as a test, and the test
will be considered successful if the program compiled and exited with an exit
code of 0.

These tests should be configured such that a failure produces an exit code other than 0.
Returning a non-zero exit code can be accomplished by calling ``exit()`` or
throwing an uncaught error.

Both exit-code and ``UnitTest`` style tests can be used within a single mason
package.

After adding our test, the package structure will be as follows::

  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage/
   │   ├── example/
   │   ├── release/
   │   │   └── myPackage
   │   └── test/
   └── test/
        └── myPackageTest.chpl


Use ``mason test`` to run the test(s). If tests are not explicitly
specified in ``Mason.toml``, Mason will gather all the tests found in ``test/``,
compile them with the dependencies listed in your ``Mason.toml`` and run them
producing the following output::

  --- Results ---
  Test: myPackageTest Passed

  --- Summary:  1 tests run ---
  -----> 1 Passed
  -----> 0 Failed

Additional output can be displayed by throwing the ``--show flag``.

.. note::

    ``mason test`` can also be used outside of a mason package as a
    ``UnitTest`` test runner. See :chpl:mod:`UnitTest` for more information.

Tests can be listed in the ``Mason.toml`` as a TOML array of strings for the
``tests`` field:

.. code-block:: text

   [brick]
   name = "myPackage"
   version = "0.1.0"
   chplVersion = "1.18.0"
   tests = ["test1.chpl",
            "test2.chpl",
            "test3.chpl"]



Creating and Running Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Mason supports examples as a way to demonstrate typical usage of a package.
The following example adds an example to ``MyPackage`` and runs it.
The example below prints a message a number of times based on the ``config const count``:


.. code-block:: chpl

   config const count: int = 10;

   for i in 1..count {
     writeln("This is an example!!");
   }


To build the example without running it, use the command ``mason build --example``.
This command will build ALL examples found either in the ``example/`` directory or
listed in the ``Mason.toml``

.. note:: If examples or tests are listed in the Mason.toml, Mason will not search
          for any examples or tests not listed.

To view what examples are available, enter ``mason run --example`` without any other
arguments. This will produce the names of all examples that are currently available
to Mason::

  --- available examples ---
  --- myPackageExample.chpl
  --------------------------

To run the example, use the command ``mason run --example myPackageExample.chpl``.

After the program is run via the command above, the package structure will look as
follows::


  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   │   └── myPackageExample.chpl
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage
   │   ├── example/
   │   │   └── myPackageExample
   │   ├── release/
   │   │   └── myPackage
   │   └── test/
   └── test/
        └── myPackageTest.chpl


Examples can either be specified in the Mason.toml, or found automatically by Mason. However,
to include compile time or runtime arguments for examples, users must explicitly declare them
in their ``Mason.toml`` as follows:

.. code-block:: text

   [brick]
   name = "myPackage"
   version = "0.1.0"
   chplVersion = "1.18.0"

   [dependencies]

   [examples]
   examples = ["myPackageExample.chpl"]

   [examples.myPackageExample]
   execopts = "--count=20"
   compopts = "--savec tmp"


Documenting a Package
~~~~~~~~~~~~~~~~~~~~~

Creating a website for package documentation is a breeze with Mason. Mason uses ``chpldoc`` which turns any ``.chpl`` file
into ``Sphinx`` documentation. To document a package, run the command ``mason doc`` while inside of a package. The
documentation will be automatically generated as long as ``chpldoc`` has been set up. For instructions on how to set up
``chpldoc``, view its documentation. Documentation will be built into the ``doc/`` folder that will be created upon
the first call of ``mason doc``.




Using Chapel Dependencies
=========================

There are multiple types of dependencies in Mason. Chapel or "Mason" dependencies are other
Mason packages that you want to use in your Mason package.

To search through all the current available Mason packages, use ``mason search``.

Chapel Dependencies are listed under the ``[dependencies]`` table in the ``Mason.toml``
file of the package as follows:

.. code-block:: text


   [brick]
   name = "myPackage"
   version = "0.1.0"
   chplVersion = "1.18.0"

   [dependencies]
   MatrixMarket = 0.1.0

To add a Chapel dependency without editing the ``Mason.toml`` manually, use the ``mason add``
command as follows::

  mason add MatrixMarket@0.1.0


Using Non-Chapel Dependencies
=============================
Mason allows for specification of external, non-Chapel dependencies through two
mediums, ``Spack`` and ``pkg-config``. The following two sections document how to
use ``mason external`` and ``mason system`` to interface with ``Spack`` and ``pkg-config``
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
   name = "myPackage"
   version = "0.1.0"
   chplVersion = "1.18.0"

   [system]
   openSSL = "0.9.8zh"

Now, upon calling ``mason build`` or ``mason run --build``, Mason will go get ``openssl`` and include it
in the package so that it can be used as a dependency.


Using Spack Dependencies
~~~~~~~~~~~~~~~~~~~~~~~~

Mason users can interface with ``Spack``, a package manager geared towards high performance
computing. Through this integration, Mason user's now have
access to a large ecosystem of `packages <https://spack.readthedocs.io/en/latest/package_list.html#package-list>`_.
Non-destructive installs, custom version and configurations, and simple package installation
and uninstallation are a few of the features Mason gains through this integration.

Mason users can access Spack through the ``mason external`` command. Spack provides Mason users with the ability
to install and use any package in the `Spack registry <https://spack.readthedocs.io/en/latest/package_list.html#package-list>`_.
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

Obviously there are two types of the package listed, so we need to figure out which one to use.
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
For example, other ways to install openSSL would be::

  mason external install openssl@1.0.2k

which simply specifies the exact version that we want.
If we wanted to specify which compiler the package was built with::

  mason external install openssl@1.0.2k%gcc

Mason will infer which compiler, in the case that the compiler is left out of the spec,
by looking at the environment variable ``CHPL_TARGET_COMPILER``. For more information on
how to use spec expressions, use the command ``mason external --spec`` which would output the following:

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
This way, multiple versions and builds of a package can be installed on a system and used without breaking anything.

**Specifying Spack packages in the manifest file**

Now that the correct package is installed, add it to the ``Mason.toml`` as follows::

  $ mason add --external openssl@1.0.2k
  Adding external dependency with spec openssl@1.0.2k



The ``Mason.toml`` now looks like:

.. code-block:: text

   [brick]
   name = "myPackage"
   version = "0.1.0"
   chplVersion = "1.18.0"

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


Mason-Registry
==============

The default mason registry is a GitHub repository containing a list of versioned manifest files.

`Mason-Registry <https://github.com/chapel-lang/mason-registry>`_.

A registry will be downloaded to ``$MASON_HOME/<name>`` by ``mason update``
for each registry named in ``$MASON_REGISTRY`` if a registry at that location
does not already exist.

The registry consists of a hierarchy like the following:

.. code-block:: text

 mason-registry/
    Bricks/
       Curl/
          1.0.0.toml
          2.0.0.toml
       RecordParser/
          1.0.0.toml
          1.1.0.toml
          1.2.0.toml
       VisualDebug/
          2.2.0.toml
          2.2.1.toml

Each versioned manifest file is identical to the manifest file in the top-level directory
of the package repository, with the exception of a file path or URL pointing to the repository and revision
in which the version is located.

Continuing the example from before, the 'registry' ``0.1.0.toml`` would include the additional source field:

.. code-block:: text

     [brick]
     name = "MyPackage"
     version = "0.1.0"
     chplVersion = "1.16.0"
     authors = ["Sam Partee <Sam@Partee.com>"]
     source = "https://github.com/Spartee/MyPackage"

     [dependencies]
     curl = '1.0.0'

Search the registry with ``mason search <query>``, which will list all packages
(and their latest version) that contain ``<query>`` in their names (case-insensitive).
If no query is provided, all packages in the registry will be listed.

Searching with the ``--show`` flag will output the toml file of a package if the search
returns a single package. If the package has multiple versions it will return the most
recent.

.. note::

    Packages will be listed regardless of their ``chplVersion`` compatibility.


Submit a Package
================

The mason registry will hold the manifest files for packages submitted by developers.
To contribute a package to the mason-registry a chapel developer will need to host their
package and submit a pull request to the mason-registry with the toml file pointing
to their package. For a more detailed description follow the steps below. Publishing
can be done with ``mason publish`` or manually.

``mason publish`` Steps:
      1) Write a library or binary package in chapel using mason
      2) Host the package in a git repository. (e.g. GitHub)
      3) Fork the mason-registry on GitHub
      4) Ensure your package has a remote origin.
      5) Run ``mason publish`` in your package
      6) Go to the link provided to open a pull request to the mason registry.
      7) Wait for mason-registry gatekeepers to approve PR.

Manual Steps:
      1) Write a library or binary package in chapel using mason
      2) Host that package in a git repository. (e.g. GitHub)
      3) Create a tag of your package that corresponds to the version number prefixed with a 'v'. (e.g. v0.1.0)
      4) Fork the mason-registry on GitHub
      5) Create a branch of the mason-registry and add your package's ``Mason.toml`` under ``Bricks/<package_name>/<version>.toml``
      6) Add a source field to your ``<version>.toml`` pointing to your package's repository.
      7) Open a PR in the mason-registry for your newly created branch containing just your <version>.toml.
      8) Wait for mason-registry gatekeepers to approve the PR.

Once your package is uploaded, maintain the integrity of your package, and please notify the
chapel team if your package should be taken down.



If you have a personal remote registry, ``mason publish <path-to-registry>``  also accepts
a remote path to a git repository. This will create a branch to your registry that adds
your package, and you can approve the PR to merge your new package into your registry.
Must ensure your package has a remote origin in order to publish remotely.

Publishing to a personal remote registry

.. code-block:: sh

   cd PackageA
   mason publish <remote-path-to-registry>

To assess the ability of your package to be published to the mason-registry or
a personal registry, run ``mason publish --dry-run <path-to-registry>`` for a
series of quick checks or ``mason publish --check <path-to-registry`` for a more
in depth check that will build your packages and run the full test suite.

Local Registries
================

It is sometimes desirable to use a local registry, for example with libraries
you don't intend to distribute. The following steps create a local registry
starting with Bricks for ``PackageA`` which was created with ``mason new PackageA``.
Once you have successfully created a local registry, ``mason publish <path-to-local-registry>``
can be used to publish automatically.

First create, commit, and tag the packages that will be in the registry:


Create a local registry:

.. code-block:: sh

   # Create the local registry
   mkdir /path/to/local/registry
   cd /path/to/local/registry
   # Create /Bricks/DummyPackage/0.1.0.toml
   touch README.md

   # Initialize and check everything in to the git repository
   git init
   git add README.md /Bricks/DummyPackage/0.1.0.toml
   git commit -m 'First Commit'

Now ``MASON_REGISTRY`` can be set to point at both the local registry and the
default registry.

.. code-block:: sh

   export MASON_REGISTRY="local-registry|/path/to/local/registry,mason-registry|https://github.com/chapel-lang/mason-registry"


Adding a local package to the local registry

.. code-block:: sh

   mason new PackageA
   cd PackageA
   git add .
   git commit -m "First Commit"
   mason publish <path-to-local-registry>

The ``MyPackage`` package is now free to include ``PackageA`` as dependency by adding
the it as a dependency with ``mason add package@version``

.. code-block:: sh

   cd MyPackage
   mason add PackageA@0.1.0


The Manifest File
=================

The ``Mason.toml`` manifest file is written in TOML(for more information see TOML section below).
Each time a new package is created in Mason a standard TOML file is included in the top-level
directory of the package.

For example, ``Mason.toml``:

.. code-block:: text

    [brick]
    name = "MyPackage"
    version = "0.1.0"
    chplVersion = "1.16.0"
    authors = ["Sam Partee <Sam@Partee.com>"]

    [dependencies]
    curl = '1.0.0'

The ``chplVersion`` field indicates Chapel releases compatible with this
package. There are a number of accepted formats:

.. code-block:: text

    "1.16.0"         # 1.16.0 or later
    "1.16"           # 1.16.0 or later
    "1.16.0..1.19.0" # 1.16 through 1.19, inclusive

By default, ``chplVersion`` is set to represent the current Chapel release or
later. For example, if you are using the 1.16 release, chplVersion will be
``1.16.0``.


Environment Variables
=====================

Mason can be configured by setting the following environment variables:

- ``MASON_HOME`` : Path to a directory where mason will store cached registry
  and package data. Defaults to ``$HOME/.mason``.
- ``MASON_REGISTRY`` : A comma separated list of ``name|location`` pairs, where
  ``name`` is a local name for the registry at ``location``. Defaults to
  ``mason-registry|https://github.com/chapel-lang/mason-registry``. If the
  ``name|`` part of a pair is omitted it is inferred to be the word following
  the final slash in ``location`` with any ``.git`` suffix removed.
- ``MASON_OFFLINE`` : A boolean value that prevents mason from making calls that
  require internet access when set to ``true``. Defaults to ``false``. Mason command
  that support a ``--[no-]update`` flag can override the ``MASON_OFFLINE`` setting
  when ``--update`` is explicitly passed.

The ``mason env`` command will print the inferred or set values of these
environment variables. If a variable was set by the user, an asterisk will be
printed at the end of the line. For example, if ``$MASON_HOME`` was set:

.. code-block:: text

   > mason env
   MASON_HOME: /path/to/something *
   MASON_REGISTRY: mason-registry|https://github.com/chapel-lang/mason-registry
   MASON_OFFLINE: false

.. warning::

   If ``MASON_REGISTRY`` changes after invoking a mason command that updates the
   local copy of the registry (e.g. ``mason update``), the local copies of the
   registry and dependency sources will be removed.


TOML
====

TOML is the configuration language chosen by the chapel team for
configuring programs written in chapel. A TOML file contains the
necessary information to build a chapel program using mason.
`TOML Spec <https://github.com/toml-lang/toml>`_.


Namespacing
===========

All packages will exist in a single common namespace with a first-come, first-served policy.
It is easier to go to separate namespaces than to roll them back, so this position affords
flexibility.


Semantic Versioning
===================

To assist version resolution, the mason registry will enforce the following conventions:

The format for all versions will be a.b.c.
   Major versions are denoted by a.
   Minor versions are denoted by b.
   Bug fixes are denoted by c.

- If the major version is 0, no further conventions will be enforced.

- The major version must be advanced if and only if the update causes breaking API changes,
  such as updated data structures or removed methods and procedures. The minor and bug fix
  versions will be zeroed out. (ex. 1.13.1 -> 2.0.0)

- The minor version must be advanced if and only if the update adds functionality to the API
  while maintaining backward compatibility with the current major version. The bug fix
  version will be zeroed out. (ex. 1.13.1 -> 1.14.0)

- The bug fix must be advanced for any update correcting functionality within a minor revision.
  (ex. 1.13.1 -> 1.13.2)


Incompatible Version Resolution Strategy
========================================

The current resolution strategy for Mason 0.1.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the package,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the package,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)


The Lock File
=============

The lock file ``Mason.lock`` is generated after running a ``mason update`` command. The user should
never manually edit the lock file as it is intended to "lock" in the settings of a certain
package build iteration. ``Mason.lock`` is added by default to the .gitignore when a new package
is created. If your intention is to create a binary application package that does not need to
be re-compiled by mason then take the ``Mason.lock`` out of your .gitignore. An example of
a lock file is written below as if generated from the earlier example of a ``Mason.toml``:


.. code-block:: text

     [curl]
     name = 'curl'
     version = '1.0.0'
     chplVersion = "1.16.0..1.16.0"
     source = 'https://github.com/username/curl'


     [root]
     name = "MyPackage"
     version = "0.1.0"
     chplVersion = "1.16.0..1.16.0"
     authors = ["Sam Partee <Sam@Partee.com>"]
     source = "https://github.com/Spartee/MyPackage"
     dependencies = ['curl 1.0.0 https://github.com/username/curl']


Dependency Code
===============

The source code for every package will be downloaded to ``$MASON_HOME/src``.
