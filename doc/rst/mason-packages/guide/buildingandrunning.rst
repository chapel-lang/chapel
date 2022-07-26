:title: Mason Guide: *Building and Running*

.. _building-and-running:
        
Building and Running
~~~~~~~~~~~~~~~~~~~~

When invoked, ``mason build [ options ]`` will do the following:

    - Run update to make sure any manual manifest edits are reflected in the dependency code.
    - Build ``MyPackage.chpl`` in the ``src/`` directory.
    - All packages are compiled into binaries and placed into ``target/``
    - To forward options to the Chapel compiler(``chpl``), seperate them with a double dash
       - e.g., mason build --force -- --savec tmpdir

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
was built upon invocation of ``mason build``. This file can be generated manually
with the ``mason update`` command. ``mason update`` will read the ``Mason.toml``,
resolve dependencies, and generate the ``Mason.lock`` based on it's contents.

The ``target/`` directory is where Mason stores all the binaries related to your package.
These could be binaries for the main source code as well as examples and tests. There are two types of
targets for building. The default location of a package binary is ``target/debug/``, as shown in the
example above. However, if a final version of an application or library is
being produced, the ``--release`` flag can be thrown as follows:

.. code-block:: sh

   mason build --release --force
   
   OR
   
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

