:title: Mason Guide: *Registry*

Mason-Registry
==============

The default mason registry is a GitHub repository containing a list of versioned manifest files.

View the official mason registry here: `Mason-Registry <https://github.com/chapel-lang/mason-registry>`_.

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
     authors = ["Sam Partee <Sam@Partee.com>"]
     chplVersion = "1.16.0"
     license = "None"
     name = "MyPackage"
     source = "https://github.com/Spartee/MyPackage"
     version = "0.1.0"

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


Alternatively, you may automatically create a local registry by running ``mason publish --create-registry path/to/local/registry``.
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
