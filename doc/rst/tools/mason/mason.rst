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


Environment Variables
=====================

Mason can be configured by setting the following environment variables:

- ``MASON_HOME`` : Path to a directory where mason will store cached registry
  and package data. Defaults to ``$HOME/.mason``.
- ``MASON_REGISTRY`` : A comma separated list of ``name|location`` pairs, where
  ``name`` is a local name for the registry at ``location``. Defaults to
  ``mason-registry|https://github.com/chapel-lang/mason-registry``. If the
  ``name|`` part of a pair is omitted it is inferred to be the word following
  the final slash in ``location`` with any ".git" suffix removed.

The ``mason env`` command will print the inferred or set values of these
environment variables. If a variable was set by the user, an asterisk will be
printed at the end of the line. For example, if ``$MASON_HOME`` was set:

.. code-block:: text

   > mason env
   MASON_HOME: /path/to/something *
   MASON_REGISTRY: mason-registry|https://github.com/chapel-lang/mason-registry

.. warning::

   If MASON_REGISTRY changes after invoking a mason command that updates the
   local copy of the registry (e.g. ``mason update``), the local copies of the
   registry and dependency sources will be removed.


Setting up Your Project
=======================

``mason new [ project name ] [ options ]`` is the command that initializes
a new project. It also creates a git repository unless ``--no-vcs`` is included.

For example, after ``mason new MyPackage`` is run in an empty directory it will have the
following hierarchy::

  MyPackage/
      Mason.toml
      test/
      src/
          MyPackage.chpl


Mason will ensure that the main file be named after the package to enforce namespacing.
While it is common practice for package names to be PascalCase and chpl files to be lowercase,
it is an acceptable tradeoff for reliability. ``MyPackage.chpl`` will be the first file listed in ``src/``.


Building and Running Your Project
=================================

When invoked, ``mason build [ options ]`` will do the following:

    - Run update to make sure any manual manifest edits are reflected in the dependency code.
    - Build ``MyPackage.chpl`` in the ``src/`` directory.
    - All packages are compiled into binaries and placed into ``target/``
    - All options not recognized by ``mason`` will be forwarded to the chapel compiler(``chpl``)

``mason run [ options ]`` will, in turn:

    - Run the executable built above out of ``target/``, if it exists.
    - All options not recognized by ``mason`` will be forwarded to the executable.

For example, after ``mason build && mason run [ options ]``, the project directory appears as so::


    MyPackage/
      Mason.toml
      Mason.lock
      src/
        MyPackage.chpl
      test/
      target/
        debug/
        (release/)
           MyPackage


For projects that span multiple files, the main module is designated by the module that
shares the name with the package directory and the name field in the ``Mason.toml``.


For projects that span multiple sub-directories within ``src``, sub-directories must be passed
to Mason with the ``-M  <src/subdirectory>`` flag which is forwarded to the chapel compiler. For example, lets say
MyPackage's structure is as follows::


    MyPackage/
      Mason.toml
      Mason.lock
      src/
        MyPackage.chpl
        MySubPackage.chpl
      util/
        MyPackageUtils.chpl
      test/
      target/
        debug/
          MyPackage



If MyPackage needs multiple files in different directories like the example above,
then call ``mason build`` with the ``-M`` flag followed by the local dependencies.
A full command of this example would be::

  mason build -M src/util/MyPackageUtils.chpl



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


Testing your Project
====================

Testing a Mason project is easy! Start by adding test files to the ``test/`` directory and specifying
them in your Mason.toml as follows:

.. code-block:: text

    [brick]
    name = "MyPackage"
    version = "0.1.0"
    chplVersion = "1.16.0"
    authors = ["Sam Partee <Sam@Partee.com>"]
    tests = ["sampleTest.chpl"]

    [dependencies]
    curl = '1.0.0'


When the ``mason test [options]`` command is invoked, mason will find and download the necessary dependencies
for your project that you listed in your Mason.toml and compile them with your main module found in
``src/``. For example, after listing ``sampleTest.chpl`` as a test, the project structure would be
as follows::

    MyPackage/
      Mason.toml
      Mason.lock
      src/
        MyPackage.chpl
      test/
        sampleTest.chpl
      target/
        debug/
        (release/)
           MyPackage

To test the project, run ``mason test [options]``, which will update the lock file, compile the tests,
and produce the binary within ``target/test/`` as follows::

    MyPackage/
      Mason.toml
      Mason.lock
      src/
        MyPackage.chpl
      test/
        sampleTest.chpl
      target/
        test/
          sampleTest
        debug/
        (release/)
          MyPackage



The Manifest File
=================

The ``Mason.toml`` manifest file is written in TOML(for more information see TOML section below).
Each time a new project is created in Mason a standard TOML file is included in the top-level
directory of the project.

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


TOML
====

TOML is the configuration language chosen by the chapel team for
configuring programs written in chapel. A TOML file contains the
necessary information to build a chapel program using mason.
`TOML Spec <https://github.com/toml-lang/toml>`_.


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
of the package repository, with one exception, a file path or URL pointing to the repository and revision
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

.. note::

    Packages will be listed regardless of their chplVersion compatibility.

Non-Chapel Dependencies
=======================
Mason allows for specification of external, non-Chapel dependencies through two
mediums, ``Spack`` and ``pkg-config``. The following two sections document how to
use ``mason external`` and ``mason system`` to interface with ``Spack`` and ``pkg-config``
packages respectively.


Using Spack Packages
~~~~~~~~~~~~~~~~~~~~
Mason users can interface with ``Spack``, a package manager geared towards high performance
computing, through the ``mason external`` command. Though this integration, Mason users have
access to over 2800 `packages <https://spack.readthedocs.io/en/latest/package_list.html#package-list>`_.
Non-destructive installs, custom version and configurations, and simple package installation
and uninstallation are a few of the features Mason gains through this integration.

The following example shows how to create a Chapel library from a C library. This example uses the ``HDF5``
library which is a C library wrapped with extern procedures using :mod:`C2Chapel`. By wrapping
these calls and including the original C library, Chapel can harness all of the functionality
of the ``HDF5`` library without ever having to write library code. Before Mason, Chapel users
were required to download and install the libraries themselves and use Make to include and use them
in their projects. These requirements have been removed and now only a few Mason commands
are required to include external dependencies such as ``HDF5``.


First a Mason project is created using ``mason new masonHDF5`` and the ``HDF5.chpl`` library
is placed within the ``src/`` folder of the project and renamed ``masonHDF5.chpl``.
A few examples with ``.h5`` data are placed in the ``examples/`` directory so that
we can check out our new Mason package in action. After these steps, the project directory
hierarchy is as follows::


    masonHDF5/
      Mason.toml
      Mason.lock
      src/
        masonHDF5.chpl
      test/
      example/
        data/
          sample_data.h5
        hdf5Example.chpl

However, at this point, we have the wrapped function calls to the ``HDF5`` library included in
the project but not the ``HDF5`` actual library itself. To search for and install packages such
as ``HDF5``, simply use ``mason external search <package>`` and ``mason external install <package>``.
Mason uses the ``Spack`` package specification.

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


So, to install ``HDF5`` version 1.10.1 built with gcc including a high level interface and without
MPI, the following spec expression would be used::

  mason external install hdf5@1.10.1%gcc+hl~mpi

After the download, to ensure the package was downloaded, use ``mason external find`` which
will list all installed external packages.

The next step in creating the library is to add ``HDF5`` to our ``Mason.toml`` to inform Mason that
we want the library included when our project is compiled. Use ``mason add --external hdf5@1.10.1%gcc``
to add the external dependency to your ``Mason.toml`` without ever having to open up your editor.

.. note:: Currently the name of the package, version, and compiler must all be included
          in the value of the ``Mason.toml`` for external dependencies only. See ``Mason.toml``
          below for details.

``Mason.toml``

.. code-block:: text

   [brick]
   name = "masonHDF5"
   version = "0.1.0"
   chplVersion = "1.18.0"
   # linker flags for the library
   compopts = "-lhdf5 -lhdf5_hl"

   # must give name, version, compiler
   [external]
   hdf5 = "hdf5@1.10.1%clang"

Now lets run our example. To check the name of all the examples available in your project use
``mason run --example``. The output will represent whatever ``.chpl`` files are located in your
``example/`` folder as follows::

  $ mason run --example
  --- available examples ---
  --- hdf5Example.chpl
  --------------------------

Calling ``mason run --build --example hdf5Example.chpl`` will update the project's ``Mason.lock``
and build and run ``hdf5Example.chpl``.

When constructing the lock file (see below section), Mason will work with Spack to gather dependencies.
In the case of ``HDF5``, at the time of this writing, ``zlib`` will be added to the lock file since the
``HDF5`` Spack package depends on it. Any dependency a Spack package has will be handled in this manner.
This highlights another great feature of the integartion. Mason can install and retrieve all dependencies
necessary for any package from Spack without ever interfering with a previous package installation.

Using System Packages
~~~~~~~~~~~~~~~~~~~~~

To use this feature of Mason users must have access to a ``pkg-config`` installation.
The following is an example of using the :mod:`LinearAlgebra` package that requires
both BLAS and LAPACK:

``Mason.toml``

.. code-block:: text

    [brick]
    name = "cholesky"
    version = "0.1.0"
    chplVersion = "1.18.0"
    compopts = "--ccflags -Wno-enum-conversion --ccflags -Wno-strict-prototypes"

    [dependencies]

    [system]
    lapack = "3.8.0"
    openblas = "*"


This ``Mason.toml`` is updated to reflect the necessary dependency information for Mason
to build the package in the ``Mason.lock``. Mason gets external dependency information,
in this case, from the ``.pc`` files found via the ``PKG_CONFIG_PATH``. The ``*`` means
that Mason will grab the latest package available known to ``pkg-config`` in your system.


``Mason.lock``

.. code-block:: text

   [root]
   name = "cholesky"
   compopts = "--ccflags -Wno-enum-conversion --ccflags -Wno-strict-prototypes"
   version = "0.1.0"
   chplVersion = "1.18.0..1.18.0"

   [system]
        [system.lapack]
        name = "lapack"
        version = "3.8.0"
        libs = "-L/usr/local/Cellar/lapack/3.8.0_1/lib -lapack"
        include = "/usr/local/Cellar/lapack/3.8.0_1/include"

        [system.openblas]
        name = "openblas"
        version = "0.3.1"
        libs = "-L/usr/local/Cellar/openblas/0.3.1/lib -openblas"
        include = "/usr/local/Cellar/openblas/0.3.1/include"


Use the ``mason system`` command for help with developing Mason packages with system
package dependencies. Use ``mason system search <package>`` to lookup packages that are available
on your system via ``pkg-config``. To view a package's ``.pc`` file, use the ``mason system pc <package>``
command.


Submit a Package
================

The mason registry will hold the manifest files for packages submitted by developers.
To contribute a package to the mason-registry a chapel developer will need to host their
project and submit a pull request to the mason-registry with the toml file pointing
to their project. For a more detailed description follow the steps below.

Steps:
      1) Write a library or binary project in chapel using mason
      2) Host that project in a git repository. (e.g. GitHub)
      3) Create a tag of your package that corresponds to the version number prefixed with a 'v'. (e.g. v0.1.0)
      4) Fork the mason-registry on GitHub
      5) Create a branch of the mason-registry and add your project's ``Mason.toml`` under ``Bricks/<project_name>/<version>.toml``
      6) Add a source field to your ``<version>.toml`` pointing to your project's repository.
      7) Open a PR in the mason-registry for your newly created branch containing just your <version>.toml.
      8) Wait for mason-registry gatekeepers to approve the PR.

Once your package is uploaded, maintain the integrity of your package, and please notify the
chapel team if your package should be taken down.


Local Registries
================

It is sometimes desirable to use a local registry, for example with libraries
you don't intend to distribute. The following steps create a local registry
starting with Bricks for ``ProjectA`` and ``ProjectB`` which were created with
``mason new ProjectA`` and ``mason new ProjectB``, and are located at
``/path/to/my/projects/Project[AB]``. It is expected that mason will be
extended to simplify and handle more of this process.

First create, commit, and tag the projects that will be in the registry:

.. code-block:: sh

   # Create ProjectA
   cd /path/to/my/projects
   mason new ProjectA
   cd ProjectA
   git add Mason.toml src/ProjectA.chpl
   git commit
   git tag -a v0.1.0 -m "Tag version 0.1.0"

   # Create ProjectB
   cd ..
   mason new ProjectB
   cd ProjectB
   git add Mason.toml src/ProjectB.chpl
   git commit
   git tag -a v0.1.0 -m "Tag version 0.1.0"

Next, create a local registry:

.. code-block:: sh

   # Create the local registry
   mkdir /path/to/local/registry
   cd /path/to/local/registry
   mkdir -p Bricks/ProjectA Bricks/ProjectB

   # Add bricks for ProjectA and ProjectB
   cp /path/to/my/projects/ProjectA/Mason.toml Bricks/ProjectA/0.1.0.toml
   cp /path/to/my/projects/ProjectB/Mason.toml Bricks/ProjectB/0.1.0.toml

   # Edit Bricks/ProjectA/0.1.0.toml to add:
   source = "/path/to/my/projects/ProjectA"

   # Edit Bricks/ProjectB/0.1.0.toml to add:
   source = "/path/to/my/projects/ProjectB"

   # Initialize and check everything in to the git repository
   git init
   git add Bricks/ProjectA/0.1.0.toml Bricks/ProjectB/0.1.0.toml
   git commit

Now ``MASON_REGISTRY`` can be set to point at both the local registry and the
default registry.

.. code-block:: sh

   export MASON_REGISTRY="local-registry|/path/to/local/registry,mason-registry|https://github.com/chapel-lang/mason-registry"

The ``MyPackage`` package is now free to include ``ProjectA`` and ``ProjectB``
as dependencies by adding the following lines to the ``[dependencies]`` section
of its .toml file.

.. code-block:: text

   ProjectA = "0.1.0"
   ProjectB = "0.1.0"


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
    1. If multiple bug fixes of a package are present in the project,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the project,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)


The Lock File
=============

The lock file ``Mason.lock`` is generated after running a ``mason update`` command. The user should
never manually edit the lock file as it is intended to "lock" in the settings of a certain
project build iteration. ``Mason.lock`` is added by default to the .gitignore when a new project
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
