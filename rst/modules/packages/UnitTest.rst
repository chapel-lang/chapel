.. default-domain:: chpl

.. module:: UnitTest
   :synopsis: Module UnitTest provides support for automated testing in Chapel.

UnitTest
========
**Usage**

.. code-block:: chapel

   use UnitTest;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   UnitTest/*


Module UnitTest provides support for automated testing in Chapel.
Any function of the form

.. code-block:: chapel

  proc funcName(test: borrowed Test) throws {}

is treated as a test function. These functions must accept an object of Test
Class. We use :proc:`~UnitTest.main()` to run the tests.

.. note::

  It is intended to be used in concert with the `mason test <https://chapel-lang.org/docs/tools/mason/mason.html>`_ command, which automates execution of any test function.

Assert Functions
----------------

Here are the assert functions available in the UnitTest module:

- :proc:`~Test.assertTrue`
- :proc:`~Test.assertFalse`
- :proc:`~Test.assertEqual`
- :proc:`~Test.assertNotEqual`
- :proc:`~Test.assertGreaterThan`
- :proc:`~Test.assertLessThan`

Test Metadata Functions
-----------------------

UnitTest module also provides multiple methods for specifying test Metadata:

- :proc:`~Test.skip`
- :proc:`~Test.skipIf`
- :proc:`~Test.addNumLocales`
- :proc:`~Test.maxLocales`
- :proc:`~Test.minLocales`
- :proc:`~Test.dependsOn`


Examples
--------

Basic Usage
^^^^^^^^^^^

Here is a minimal example demonstrating how to use the UnitTest module:

.. code-block:: chapel

   use UnitTest;

   proc celsius2fahrenheit(x) {
     // we should be returning "(x: real * 9/5)+32"
     return (x * 9/5)+32;
   }

   proc test_temperature(test: borrowed Test) throws {
     // we were expecting 98.6 but since we missed typecasting
     // the above function returned 98.
     test.assertFalse(celsius2fahrenheit(37) == 98);
   }

   UnitTest.main();

Output:

.. code-block:: bash

  ======================================================================
  FAIL xyz.chpl: test_temperature()
  ----------------------------------------------------------------------
  AssertionError: assertFalse failed. Given expression is True

  ----------------------------------------------------------------------
  Run 1 test

  FAILED failures = 1


Skipping Tests
^^^^^^^^^^^^^^^

You can skip tests unconditionally with :proc:`~Test.skip` and
conditionally with :proc:`~Test.skipIf`:

.. code-block:: chapel

   use UnitTest;

   /* calculates factorial */
   proc factorial(x: int): int {
     return if x == 0 then 1 else x * factorial(x-1);
   }

   /*Conditional skip*/
   proc test1(test: borrowed Test) throws {
     test.skipIf(factorial(0) != 1,"Base condition is wrong in factorial");
     test.assertTrue(factorial(5) == 120);
   }

   /*Unconditional skip*/
   proc test2(test: borrowed Test) throws {
     test.skip("Skipping the test directly");
   }

   UnitTest.main();


Output:

.. code-block:: bash

  ======================================================================
  SKIPPED xyz.chpl: test2()
  ----------------------------------------------------------------------
  TestSkipped: Skipping the test directly

  ----------------------------------------------------------------------
  Run 1 test

  OK skipped = 1


Specifying locales
^^^^^^^^^^^^^^^^^^

You can specify the num of locales of a test using these method.

:proc:`~Test.addNumLocales`
:proc:`~Test.maxLocales`
:proc:`~Test.minLocales`

Here is an example demonstrating how to use the :proc:`~Test.addNumLocales`

.. code-block:: chapel

  proc test_square(test: borrowed Test) throws {
    test.addNumLocales(5);
    var A: [Locales.domain] int;
    coforall i in 0..numLocales-1 with (ref A) {
      on Locales(i) {
        A[i+1] = (i+1)*(i+1);
      }
    }
    test.assertTrue(A[5]==25);
  }

Output:

.. code-block:: bash

  ----------------------------------------------------------------------
  Run 1 test

  OK

You can also specify multiple locales on which your code can run.

.. code-block:: chapel

  proc test3(test: borrowed Test) throws {
    test.addNumLocales(16,8);
  }

You can mention the range of locales using :proc:`~Test.maxLocales` and
:proc:`~Test.minLocales`

.. code-block:: chapel

  proc test4(test: borrowed Test) throws {
    test.maxLocales(4);
    test.minLocales(2);
  }

Specifying Dependencies
^^^^^^^^^^^^^^^^^^^^^^^

:proc:`~Test.dependsOn`

Here is an example demonstrating how to use the :proc:`~Test.dependsOn`

.. code-block:: chapel

   use UnitTest;

   var factorials: list(int);

   // calculates factorial
   proc factorial(x: int): int {
     return if x == 0 then 1 else x * factorial(x-1);
   }

   proc testFillFact(test: borrowed Test) throws {
     test.skipIf(factorial(0) != 1,"Base condition is wrong in factorial");
     for i in 1..10 do
       factorials.append(factorial(i));
   }

   proc testSumFact(test: borrowed Test) throws {
     test.dependsOn(testFillFact);
     var s = 0;
     for i in factorials.indices do
       s += factorials[i];
     test.assertGreaterThan(s,0);
   }

   UnitTest.main();

Output:

.. code-block:: bash

  ----------------------------------------------------------------------
  Run 2 tests

  OK


.. class:: Test

   .. method:: proc skip(reason: string = "") throws

      Unconditionally skip a test.
      
      :arg reason: the reason for skipping
      :type reason: `string`
      :throws TestSkipped: Always
      
      

   .. method:: proc skipIf(condition: bool, reason: string = "") throws

      
      Skip a test if the condition is true.
      
      :arg condition: the boolean condition
      :type condition: `bool`
      
      :arg reason: the reason for skipping
      :type reason: `string`
      :throws TestSkipped: If the `condition` is true.
      

   .. method:: proc assertTrue(test: bool) throws

      
      Assert that a boolean condition is true.  If it is false, prints
      ``assert failed``.
      
      :arg test: the boolean condition
      :type test: `bool`
      :throws AssertionError: If the assertion is false.
      

   .. method:: proc assertFalse(test: bool) throws

      
      Assert that a boolean condition is false.  If it is true, prints
      ``assert failed``.
      
      :arg test: the boolean condition
      :type test: `bool`
      :throws AssertionError: If the assertion is true.
      

   .. method:: proc assertEqual(first, second) throws

      
      Fail if the two objects are unequal as determined by the ``==`` operator.
      
      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If both the arguments are not equal.
      

   .. method:: proc assertNotEqual(first, second) throws

      
      Assert that a first argument is not equal to second argument.
      Uses ``==`` operator and type to determine if both are equal
      or not.
      
      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If both the arguments are equal.
      

   .. method:: proc assertGreaterThan(first, second) throws

      
      Assert that a first argument is greater than second argument.  If it is false, prints
      ``assert failed`` and raises AssertionError.
      
      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If the first argument is not greater than second argument.
      

   .. method:: proc assertLessThan(first, second) throws

      
      Assert that a first argument is less than second argument.  If it is false, raises AssertionError.
      
      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If the first argument is not less than the second argument.
      

   .. method:: proc maxLocales(value: int) throws

      
      Specify Max Number of Locales required to run the test
      
      :arg value: Maximum number of locales with which the test can be ran.
      :type value: `int`.
      
      :throws UnexpectedLocalesError: If `value` is less than 1 or `minNumLocales`
      

   .. method:: proc minLocales(value: int) throws

      
      Specify Min Number of Locales required to run the test
      
      :arg value: Minimum number of locales with which the test can be ran.
      :type value: `int`.
      
      :throws UnexpectedLocalesError: If `value` is more than `maxNumLocales`
      

   .. method:: proc addNumLocales(locales: int ...?n) throws

      
      To add locales in which test can be run.
      
      :arg locales: Multiple ``,`` separated locale values
      
      :throws UnexpectedLocalesError: If `locales` are already added.
      
      

   .. method:: proc dependsOn(tests: argType ...?n) throws

      Adds the tests in which the given test is depending.
      
      :arg tests: Multiple ``,`` separated First Class Test Functions.
      :throws DependencyFound: If Called for the first time in a function.
      
      

.. function:: proc main() throws

   Runs the tests
   
   Call this as
   
   .. code-block:: chapel
   
     UnitTest.main();
   

