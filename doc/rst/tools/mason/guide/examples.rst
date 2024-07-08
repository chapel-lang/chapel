:title: Mason Guide: *Examples*

.. _mason-examples:
        
Creating and Running Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Mason supports examples as a way to demonstrate typical usage of a package.
The following example adds an example to ``MyPackage`` and runs it.
The example below prints a message a number of times based on the ``config const count``:


.. code-block:: chpl

   config const count: int = 10;

   for i in 1..count {
     writeln("This is an example!!");
   }


To build the example without running it, use the command ``mason build --example``.
This command will build ALL examples found either in the ``example/`` directory or
listed in the ``Mason.toml``

.. note:: If examples or tests are listed in the Mason.toml, Mason will not search
          for any examples or tests not listed.

To view what examples are available, enter ``mason run --example`` without any other
arguments. This will produce the names of all examples that are currently available
to Mason::

  --- available examples ---
  --- myPackageExample.chpl
  --------------------------

To run the example, use the command ``mason run --example myPackageExample.chpl``.

After the program is run via the command above, the package structure will look as
follows::


  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   │   └── myPackageExample.chpl
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage
   │   ├── example/
   │   │   └── myPackageExample
   │   ├── release/
   │   │   └── myPackage
   │   └── test/
   └── test/
        └── myPackageTest.chpl


Examples can either be specified in the Mason.toml, or found automatically by Mason. However,
to include compile time or runtime arguments for examples, users must explicitly declare them
in their ``Mason.toml`` as follows:

.. code-block:: text

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [dependencies]

   [examples]
   examples = ["myPackageExample.chpl"]

   [examples.myPackageExample]
   compopts = "--savec tmp"
   execopts = "--count=20"

 
