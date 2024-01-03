:title: Mason Guide: *Building Larger Packages*

Building Larger Packages
~~~~~~~~~~~~~~~~~~~~~~~~

For packages that span multiple files, the main module is designated by the module that
shares the name with the package directory and the name field in the ``Mason.toml`` file.

While not recommended with mason libraries that are going to be added to the mason registry,
passing a ``-- -M`` as outlined below can make building your mason application easier, but,
for mason libraries, submodules should be used to avoid conflicting namespaces for users
of your library (see :ref:`readme-module_include`).

For packages that span multiple sub-directories within ``src``, sub-directories must be passed
to mason with the ``-- -M  <src/subdirectory>`` flag. Note the ``--`` which tells mason to
forward all remaining flags directly to the chapel compiler. For example, lets say
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
then call ``mason build`` with the ``-- -M`` flag followed by the local dependencies.
A full command of this example would be::

  mason build -- -M src/util/MyPackageUtils.chpl

