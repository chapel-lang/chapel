Basic Usage
-----------

Starting a New Package
~~~~~~~~~~~~~~~~~~~~~~

To initialize a new Mason package, run ``mason new MyPackage``. This creates a
directory and sets it up to have the structure of a Mason package called
MyPackage. Particularly, it creates a `Mason.toml` file and the `src` folder
with the package main module. This creates a git repository by default, unless
``--no-vcs`` is used.

Mason packages can also be initialized using ``mason init``. For example, for
an existing directory named MyPackage:

  .. code-block:: bash

    mason init MyPackage

    # OR

    cd MyPackage
    mason init

Package Layout
~~~~~~~~~~~~~~

Freshly created packages will have the following hierarchy:

.. code-block:: text

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
As you might expect, the ``test/`` folder and the ``example/`` folder hold
tests and examples for your package, respectively. We will get to the
additional functionality that comes with these folders later.

Mason enforces that the main file be named after the package to enforce
namespacing. ``MyPackage.chpl`` will be the first file listed in ``src/``.

Package Naming
~~~~~~~~~~~~~~

Mason packages by default have the same name as the directory in which they are
created. This name must be a legal Chapel identifier. However, you can have the
Mason package name differ from the directory name by creating the package with
the ``--name`` flag. This may be useful when creating a package in a directory
that is an illegal Mason package name, such as names with dashes. For example,

  .. code-block:: bash

    mason new illegal-module-name --name LegalModuleName

    # OR

    mkdir illegal-module-name
    mason init illegal-module-name --name LegalModuleName
