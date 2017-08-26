
.. _readme-mason:

=====
mason
=====

Mason is Chapel's package manager




Installation Instructions
=========================

In ``$CHPL_HOME`` run the following:

.. code-block:: sh
   
   make mason


This will create the hidden ``.mason/`` directory in ``MASON_HOME``, which
defaults to your ``$HOME`` if unset. It builds the mason binary so that the 
command line interface can be used. The mason registry is also downloaded from
github so that a user can start to rely on mason to specify project dependencies.
This installs mason in the same place as the chapel compiler (``chpl``)
so that mason can be used anywhere in the user's file system.


To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean

      
To remove mason and the mason-registry change directory to ``$CHPL_HOME/tools/mason``
and run:

.. code-block:: sh

   make clobber



Setting up your Project
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
it is an acceptable tradeoff for reliability. ``MyPackage`` will be the first file listed in ``src/``.




Building and Running your project
=================================

When invoked, ``mason build [ options ]`` will do the following:

    - Run update to make sure any manual manifest edits are reflected in the dependency code.
    - Build ``MyPackage.chpl`` in the ``src/`` directory. 
    - All packages are compiled into binaries and placed into ``target/``
    - All options not recognized by ``mason`` will be forwarded to the chapel compiler(``chpl``)

``mason run [ options ]`` will, in turn:

    - Invoke build.
    - Run the resulting executable out of ``target/``, if it exists.
    - All options not recognized by ``mason`` will be forwarded to the executable.
        
For example, after ``mason run [ options ]``, the project directory appears as so::


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
    authors = ["Sam Partee <Sam@Partee.com>"]

    [dependencies]
    curl = '1.0.0'





TOML
====

TOML is the configuration language chosen by the chapel team for
configuring programs written in chapel. A TOML file contains the
necessary information to build a chapel program using mason. 
`TOML Spec <https://github.com/toml-lang/toml>`_.




Mason-Registry
==============

The initial mason registry is a GitHub repository containing a list of versioned manifest files.

`Mason-Registry <https://github.com/chapel-lang/mason-registry>`_.

The registry consists of the following hierarchy:


.. code-block:: text

 registry/
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
of the package repository, with one exception, a URL pointing to the repository and revision
in which the version is located.

Continuing the example from before, the 'registry' ``0.1.0.toml`` would include the additional source field:

.. code-block:: text

     [brick]
     name = "MyPackage"
     version = "0.1.0"
     authors = ["Sam Partee <Sam@Partee.com>"]
     source = "https://github.com/Spartee/MyPackage"

     [dependencies]
     curl = '1.0.0'





Submit a package 
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
     version = '0.1.0'
     source = 'https://github.com/username/curl'


     [root]
     name = "MyPackage"
     version = "0.1.0"
     authors = ["Sam Partee <Sam@Partee.com>"]
     source = "https://github.com/Spartee/MyPackage"
     dependencies = ['curl 1.0.0 https://github.com/username/curl']





Dependency Code
===============

The src code for every package downloaded will be in ``$MASON_HOME`` which by default is placed
under the ``$HOME`` directory of the user. The path to the versioned packages downloaded by the
user would then be under ``$MASON_HOME/.mason/src/``. In the directory adjacent to the source code
directory is the user's checkout of the mason-registry. 
