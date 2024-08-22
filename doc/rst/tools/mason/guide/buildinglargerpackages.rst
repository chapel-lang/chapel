:title: Mason Guide: *Building Larger Packages*

Building Larger Packages
~~~~~~~~~~~~~~~~~~~~~~~~

For packages that span multiple files, the main module is designated by the module that
shares the name with the package directory and the name field in the ``Mason.toml`` file.

If a ``main`` procedure is used as the program's entry point, it should be located in the
main module. If no main procedure is present in the main module, program execution will
begin at the first line of the main module.

While not recommended with mason libraries that are going to be added to the mason registry,
passing an ``-- -M`` compilation argument, as outlined below, can make building your mason
application easier.

Alternatively, for mason libraries, submodules should be used to avoid conflicting namespaces
for users of your library. A submodule can be included in a mason project by adding a directory
with the same name as the parent module, and placing source files for submodules in that
directory. For example, in the directory structure below, the submodule ``subMod`` can be
included in the main module ``myPackage`` by using ``include module SubMod;`` in the main
module's source file (see :ref:`readme-module_include` for more)::

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

To use modules located in sub-directories (without using submodules), all sub-directory paths
must be passed to mason with the ``-- -M <src/subdirectory>`` flag. Note the ``--`` which tells
mason to forward all remaining flags directly to the Chapel Compiler. For example, lets say
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

The following command would be used to build MyPackage with the myPackageUtils module::

  mason build -- -M src/util/MyPackageUtils.chpl
