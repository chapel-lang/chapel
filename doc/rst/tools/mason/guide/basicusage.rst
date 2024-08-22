:title: Mason Guide: *Basic Usage*

Basic Usage
===========

Starting a New Package
~~~~~~~~~~~~~~~~~~~~~~

To initialize a new mason package, run ``mason new``. The same can also be done using ``mason init`` as follows:

  .. code-block:: sh

    mkdir newPackage
    cd newPackage
    mason init

This sets up a folder to have the structure of a mason package. Particularly, it creates a `Mason.toml` file and the `src` folder with the package main module.

A more advanced user may use the ``mason new [ options ] <project name>`` command, for example::

    mason new MyPackage

This creates a git repository by default, unless ``--no-vcs`` is included.

Mason packages can also be initialized using the ``mason init [options] [directory path]``.


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
As you might expect, the ``test/`` folder and the ``example/`` folder hold
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
