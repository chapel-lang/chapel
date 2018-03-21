
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
