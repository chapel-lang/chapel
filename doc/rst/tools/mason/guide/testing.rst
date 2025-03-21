:title: Mason Guide: *Testing*

.. _testing-with-mason:
        
Testing your Package
~~~~~~~~~~~~~~~~~~~~

Mason provides the functionality to test packages through the ``mason test``
subcommand. There are two styles of writing mason tests:

1. Tests that utilize the ``UnitTest`` module to determine pass/fail status
2. Tests that rely on the exit code to determine pass/fail status

Here is an example of a ``UnitTest``-based test:

.. code-block:: chpl

  use UnitTest;

  config const testParam: bool = true;

  proc myTest(test: borrowed Test) throws{
    test.assertTrue(testParam);
  }

  UnitTest.main();

Mason testing that uses ``UnitTest`` will treat each individual function as a
test, and the test will be considered successful if no assertions failed and no
halts were reached within the function body.

See the :chpl:mod:`UnitTest` documentation to learn more about writing unit tests in
Chapel.

Here is an example of an exit-code-based tests:

.. code-block:: chpl

   config const testParam: bool = true;

   if testParam {
     writeln("Test Passed!");
   }
   else {
     exit(1);
   }

Mason testing that relies on exit code tests each file as a test, and the test
will be considered successful if the program compiled and exited with an exit
code of 0.

These tests should be configured such that a failure produces an exit code other than 0.
Returning a non-zero exit code can be accomplished by calling ``exit()`` or
throwing an uncaught error.

Both exit-code and ``UnitTest`` style tests can be used within a single mason
package.

After adding our test, the package structure will be as follows::

  MyPackage/
   │
   ├── Mason.lock
   ├── Mason.toml
   ├── example/
   ├── src/
   │   └── myPackage.chpl
   ├── target/
   │   ├── debug/
   │   │   └── myPackage/
   │   ├── example/
   │   ├── release/
   │   │   └── myPackage
   │   └── test/
   └── test/
        └── myPackageTest.chpl


Use ``mason test`` to run the test(s). If tests are not explicitly
specified in ``Mason.toml``, Mason will gather all the tests found in ``test/``,
compile them with the dependencies listed in your ``Mason.toml`` and run them
producing the following output::

  --- Results ---
  Test: myPackageTest Passed

  --- Summary:  1 tests run ---
  -----> 1 Passed
  -----> 0 Failed

Specific tests can be run by listing their names or substrings of their names as command line arguments:

.. code-block:: sh

    # Run these specific tests:
    mason test test/test1.chpl test/test2.chpl
    # Run any test file with 'test1' or 'test2' in the name
    mason test test1 test2
    # Run any test file with the '1' in the name
    mason test 1

Specifying tests to run in the command line ignores the list of tests in `Mason.toml`, and searches all files in `test/`.

Additional output can be displayed by throwing the ``--show`` flag.

.. note::

    ``mason test`` can also be used outside of a mason package as a
    ``UnitTest`` test runner. See :chpl:mod:`UnitTest` for more information.

Tests can be listed in the ``Mason.toml`` as a TOML array of strings for the
``tests`` field:

.. code-block:: text

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   tests = ["test1.chpl",
            "test2.chpl",
            "test3.chpl"]
   version = "0.1.0"

A user may also set the ``CHPL_COMM`` value for running the tests, e.g. ``none``, ``gasnet``, ``ugni`` using ``mason test --setComm``.

