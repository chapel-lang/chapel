Building Larger Packages
~~~~~~~~~~~~~~~~~~~~~~~~

For packages that span multiple files, the main module is designated by the
module that shares the name with the package directory and the name field in
the ``Mason.toml`` file.

If a ``main`` procedure is used as the program's entry point, it should be
located in the main module. If no main procedure is present in the main module,
program execution will begin at the first line of the main module. Additional
source files can be added to the package as needed. By default, Mason will look
for source files in the ``src/`` directory.

Alternatively, additional sources can be added as subdirectories in the
``src/`` directory. This may be desirable as your project grows. Submodules
should be used to automatically work with Mason and avoid conflicting
namespaces for users of your library. A submodule can be included in a Mason
project by adding a directory with the same name as the parent module, and
placing source files for submodules in that directory. For example, in the
directory structure below, the submodule ``subMod`` can be included in the main
module ``myPackage`` by using ``include module SubMod;`` in the main module's
source file (see :ref:`readme-module_include` for more):

.. code-block:: text

    MyPackage/
    │
    ├── Mason.lock
    ├── Mason.toml
    ├── example/
    ├── src/
    │   ├── myPackage.chpl
    │   └── myPackage/
    │       └── SubMod.chpl
    ├── target/
    │   ├── debug/
    │   │   └── myPackage
    │   ├── example/
    │   └── test/
    └── test/

To use modules located in sub-directories (without using submodules), all
sub-directory paths must be added to the compiler's search path. This can be
done by editing the ``compopts`` field in the ``Mason.toml`` file to include
the necessary ``-M`` flags. For example, lets say MyPackage's structure is as
follows:

.. code-block:: text

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

You can edit the ``compopts`` field in the ``Mason.toml`` file to include the
necessary ``-M`` flag to add the ``src/util/`` directory to the compiler's
search path as follows:

.. code-block:: toml

    compopts = ["-M", "src/util/"]
