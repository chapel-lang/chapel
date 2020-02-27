/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
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
    var A: [1..numLocales] int;
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

   var factArray: [1..0] int;

   // calculates factorial
   proc factorial(x: int): int {
     return if x == 0 then 1 else x * factorial(x-1);
   }

   proc testFillFact(test: borrowed Test) throws {
     test.skipIf(factorial(0) != 1,"Base condition is wrong in factorial");
     for i in 1..10 do
       factArray.push_back(factorial(i));
   }

   proc testSumFact(test: borrowed Test) throws {
     test.dependsOn(testFillFact);
     var s = 0;
     for i in 1..10 do
       s += factArray[i];
     test.assertGreaterThan(s,0);
   }

   UnitTest.main();

Output:

.. code-block:: bash

  ----------------------------------------------------------------------
  Run 2 tests

  OK

*/
module UnitTest {
  use Reflection;
  use TestError;
  use List, Map;
  private use IO;

  pragma "no doc"
  config const testNames: string = "None";
  pragma "no doc"
  config const failedTestNames: string = "None";
  pragma "no doc"
  config const errorTestNames: string = "None";
  pragma "no doc"
  config const skippedTestNames: string = "None";
  pragma "no doc"
  config const ranTests: string = "None";
  // This is a dummy test to capture the function signature
  private
  proc testSignature(test: borrowed Test) throws { }
  pragma "no doc"
  var tempFcf = testSignature;
  pragma "no doc"
  type argType = tempFcf.type;  //Type of First Class Test Functions

  class Test {
    pragma "no doc"
    var numMaxLocales = max(int),
        numMinLocales = min(int);
    pragma "no doc"
    var dictDomain: domain(int);
    pragma "no doc"
    var testDependsOn: list(argType);

    /* Unconditionally skip a test.

      :arg reason: the reason for skipping
      :type reason: `string`
      :throws TestSkipped: Always

    */
    proc skip(reason: string = "") throws {
      throw new owned TestSkipped(reason);
    }

    /*
    Skip a test if the condition is true.

    :arg condition: the boolean condition
    :type condition: `bool`

    :arg reason: the reason for skipping
    :type reason: `string`
    :throws TestSkipped: If the `condition` is true.
   */
    proc skipIf(condition: bool, reason: string = "") throws {
      if condition then
        skip(reason);
    }

    /*
      Assert that a boolean condition is true.  If it is false, prints
      ``assert failed``.

      :arg test: the boolean condition
      :type test: `bool`
      :throws AssertionError: If the assertion is false.
    */
    pragma "insert line file info"
    pragma "always propagate line file info"
    proc assertTrue(test: bool) throws {
      if !test then
        throw new owned AssertionError("assertTrue failed. Given expression is False");
    }

    /*
      Assert that a boolean condition is false.  If it is true, prints
      ``assert failed``.

      :arg test: the boolean condition
      :type test: `bool`
      :throws AssertionError: If the assertion is true.
    */
    pragma "insert line file info"
    pragma "always propagate line file info"
    proc assertFalse(test: bool) throws {
      if test then
        throw new owned AssertionError("assertFalse failed. Given expression is True");
    }

    pragma "no doc"
    /*Function to call the respective method for equality checking based on the type of argument*/
    proc checkAssertEquality(first, second) throws {
      type firstType = first.type,
          secondType = second.type;

      if isTupleType(firstType) && isTupleType(secondType) {
        // both are tuples.
        assertTupleEqual(first, second);
        return;
      }
      else if isArrayType(firstType) && isArrayType(secondType) {
        // both are arrays
        assertArrayEqual(first, second);
        return;
      }
      else if isRangeType(firstType) && isRangeType(secondType) {
        // both are Range
        assertRangeEqual(first, second);
      }
      else if isString(first) && isString(second) {
        // both are Strings
        assertStringEqual(first, second);
      }
      else {
        __baseAssertEqual(first, second);
      }
    }

    pragma "no doc"
    /*
      Check that a boolean array is true.  If any element is false, returns 'false'
      else return 'true'.

      :arg it: the iterator to the array
      :type it: `iterator`
    */
    proc all(it: _iteratorRecord) {
      for b in it do if b == false then return false;
      return true;
    }

    pragma "no doc"
    /* Method overloading for the above function. Return the argument itself
    */
    proc all(check: bool) {
      return check;
    }

    pragma "no doc"
    /*An equality assertion for sequences (like arrays, tuples, strings, range).
      Args:
      seq1: The first sequence to compare.
      seq2: The second sequence to compare.
      seq_type_name: The name of datatype of the sequences
    */
    proc assertSequenceEqual(seq1, seq2, seq_type_name) throws {
      var tmpString: string;
      const len1 = seq1.size,
            len2 = seq2.size;
      if len1 == 0 && len2 == 0 then return;
      if len1 == 0 {
        tmpString = "First "+seq_type_name+" has no length.";
      }
      if tmpString == "" {
        if len2 == 0 {
          tmpString = "Second "+seq_type_name+" has no length.";
        }
      }
      if tmpString == "" {
        if len1 == len2 {
          if all(seq1 == seq2) then return;
        }
        tmpString = seq_type_name+"s differ: ";
        if seq_type_name == "Array" {
          tmpString += "'[";
          for i in 1..seq1.size {
            if i != seq1.size then tmpString+= seq1[i]:string+", ";
            else tmpString += seq1[i]:string+"]' != '[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]:string+", ";
            else tmpString += seq2[i]:string+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"' != '"+stringify(seq2)+"'" ;
        }
        for i in 1..min(len1,len2) {
          var item1 = seq1[i],
              item2 = seq2[i];
          if item1 != item2 {
            tmpString += "\nFirst differing element at index "+i:string +":\n'"+item1:string+"'\n'"+item2:string+"'\n";
            break;
          }
        }
        if len1 > len2 {
          var size_diff = len1 - len2;
          tmpString += "\nFirst "+seq_type_name+" contains "+ size_diff:string +" additional elements.\n";
          tmpString += "First extra element is at index "+(len2+1):string+"\n'"+seq1[len2+1]:string+"'\n";
        }
        else if len1 < len2 {
          var size_diff = len2 - len1;
          tmpString += "\nSecond "+seq_type_name+" contains "+ size_diff:string +" additional elements.\n";
          tmpString += "First extra element is at index "+(len1+1):string+"\n'"+seq2[len1+1]:string+"'\n";
        }
      }
      throw new owned AssertionError(tmpString);
    }

    pragma "no doc"
    /*An array-specific equality assertion.
      Args:
      array1: The first array to compare.
      array2: The second array to compare.
    */
    proc assertArrayEqual(array1: [], array2: []) throws {
      const genericErrorMsg = "assert failed -\n'" + stringify(array1) +
                              "'\nand\n'"+stringify(array2) + "'\n";

      // Compare array types, size, and shape
      if array1.type != array2.type {
        const errorMsg = genericErrorMsg + "are not of same type";
        throw new owned AssertionError(errorMsg);
      } else if array1.size != array2.size {
        const errorMsg = genericErrorMsg + "are not of same size";
        throw new owned AssertionError(errorMsg);
      } else if array1.shape != array2.shape {
        const errorMsg = genericErrorMsg + "are not of same shape";
        throw new owned AssertionError(errorMsg);
      }

      // Compare array values
      const arraysEqual = && reduce (array1 == array2);
      if !arraysEqual {
        const errorMsg = "assert failed -\n'" + stringify(array1) +
                         "'\n!=\n'"+stringify(array2)+"'";
        throw new owned AssertionError(errorMsg);
      }
    }

    pragma "no doc"
    /*
      A tuple-specific equality assertion.
      Args:
      tuple1: The first tuple to compare.
      tuple2: The second tuple to compare.
    */
    proc assertTupleEqual(tuple1, tuple2) throws {
      type firstType = tuple1.type,
          secondType = tuple2.type;
      if firstType == secondType {
        assertSequenceEqual(tuple1,tuple2,"tuple("+firstType: string+")");
      }
      else {
        var tmpString = "assert failed - '" + stringify(tuple1) +"' and '"+stringify(tuple2) + "' are not of same type";
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A range-specific equality assertion.
      Args:
      range1: The first range to compare.
      range2: The second range to compare.
    */
    proc assertRangeEqual(range1, range2) throws {
      __baseAssertEqual(range1,range2);
    }

    pragma "no doc"
    /*
      A string-specific equality assertion.
      Args:
      string1: The first string to compare.
      string2: The second string to compare.
    */
    proc assertStringEqual(string1, string2) throws {
      assertSequenceEqual(string1,string2,"String");
    }

    pragma "no doc"
    /*The default assertEqual implementation, not type specific.*/
    proc __baseAssertEqual(first, second) throws {
      if canResolve("!=",first,second) {
        if (first != second) {
          var tmpString = "assert failed - '" + stringify(first) +"' != '"+stringify(second)+"'";
          throw new owned AssertionError(tmpString);
        }
      }
      else {
        var tmpString = "assert failed - '" + stringify(first) +"' and '"+stringify(second) + "' are not of same type";
        throw new owned AssertionError(tmpString);
      }
    }

    /*
      Fail if the two objects are unequal as determined by the ``==`` operator.

      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If both the arguments are not equal.
    */
    proc assertEqual(first, second) throws {
      checkAssertEquality(first, second);
    }

    pragma "no doc"
    /* Function that checks whether two arguments are unequal or not*/
    proc checkAssertInequality(first,second) throws {
      type firstType = first.type,
          secondType = second.type;
      if isTupleType(firstType) && isTupleType(secondType) {
        if firstType == secondType {
          if first == second then return false;
        }
      }
      else if isArrayType(firstType) && isArrayType(secondType) {
        if (firstType == secondType) && (first.size == second.size) {
          if first.equals(second) then return false;
        }
      }
      else {
        if first == second then return false;
      }
      return true;
    }


    /*
      Assert that a first argument is not equal to second argument.
      Uses ``==`` operator and type to determine if both are equal
      or not.

      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If both the arguments are equal.
    */
    proc assertNotEqual(first, second) throws {
      if canResolve("!=",first, second) {
        if !checkAssertInequality(first,second) {
          var tmpString = "assert failed -\n'" + stringify(first) +"'\n==\n'"+stringify(second)+"'";
          throw new owned AssertionError(tmpString);
        }
      }
    }

    /*
      Assert that a first argument is greater than second argument.  If it is false, prints
      ``assert failed`` and raises AssertionError.

      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If the first argument is not greater than second argument.
    */
    proc assertGreaterThan(first, second) throws {
      if canResolve(">=",first, second) {
        checkGreater(first, second);
      }
      else {
        var tmpString = "assert failed - First element is of type " + first.type:string +" and Second is of type "+second.type:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*checks the type of the arguments and then do greater than comparison */
    proc checkGreater(first, second) throws {
      type firstType = first.type,
          secondType = second.type;

      if isTupleType(firstType) && isTupleType(secondType) {
        // both are tuples.
        assertTupleGreater(first, second);
        return;
      }
      else if isArrayType(firstType) && isArrayType(secondType) {
        // both are arrays
        assertArrayGreater(first, second);
        return;
      }
      else if isRangeType(firstType) && isRangeType(secondType) {
        // both are Range
        assertRangeGreater(first, second);
      }
      else if isString(first) && isString(second) {
        // both are Strings
        assertStringGreater(first, second);
      }
      else {
        __baseAssertGreater(first, second);
      }
    }

    pragma "no doc"
    /*An greater assertion for sequences (like arrays, tuples, strings).
      Args:
      seq1: The first sequence to compare.
      seq2: The second sequence to compare.
      seq_type_name: The name of datatype of the sequences
    */
    proc assertSequenceGreater(seq1, seq2, seq_type_name) throws {
      var checkgreater: bool = false,
          checkequal: bool = false;
      const len1 = seq1.size,
            len2 = seq2.size;
      var symbol: string,
          tmpString: string,
          tmplarge: string;

      if len1 == 0 {
        tmpString = "First "+seq_type_name+" has no length.";
      }
      if tmpString == "" {
        if len2 == 0 {
          tmpString = "Second "+seq_type_name+" has no length.";
        }
      }
      if tmpString == "" {
        for i in 1..len1 {
          var item1 = seq1[i],
              item2 = seq2[i];
          if item1 == item2 then checkequal = true;
          else if item1 < item2 {
            tmpString += "First "+seq_type_name+" < Second "+seq_type_name+" :\n";
            tmplarge += "\nFirst larger element in second "+seq_type_name+" is at index "+i:string +":\n'"+item1:string+"'\n'"+item2:string+"'\n";
            checkgreater = true;
            checkequal = false;
            symbol = "<";
            break;
          }
          else {
            checkequal = false;
            break;
          }
        }
        if !checkgreater && !checkequal then return;
        else if checkequal {
          tmpString += "Both "+seq_type_name+" are equal\n";
          symbol = "==";
        }
        if seq_type_name == "Array" {
          tmpString += "'[";
          for i in 1..seq1.size {
            if i != seq1.size then tmpString+= seq1[i]:string+", ";
            else tmpString += seq1[i]:string+"]'"+symbol+ "'[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]:string+", ";
            else tmpString += seq2[i]:string+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"'"+symbol+"'"+stringify(seq2)+"'" ;
        }
        tmpString+=tmplarge;
      }
      throw new owned AssertionError(tmpString);
    }

    pragma "no doc"
    /*An array-specific greater assertion.
      Args:
      array1: The first array to compare.
      array2: The second array to compare.
    */
    proc assertArrayGreater(array1, array2) throws {
      if array1.rank == array2.rank {
        if array1.shape == array2.shape {
          if array1.rank == 1 {
            assertSequenceGreater(array1, array2, "Array");
          }
          else { // can be reimplemented using `reduce`
            if all(array1 <= array2) {
              var tmpString = "assert failed -\n'" + stringify(array1) +"'\n<=\n'"+stringify(array2)+"'";
              throw new owned AssertionError(tmpString);
            }
        }
        }
        else {
          var tmpString = "assert failed - First element is of shape " + stringify(array1.shape) +" and Second is of shape "+stringify(array2.shape);
          throw new owned AssertionError(tmpString);
        }
      }
      else {
        var tmpString = "assert failed - First element is of type " + array1.type:string +" and Second is of type "+array2.type:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A tuple-specific greater assertion.
      Args:
      tuple1: The first tuple to compare.
      tuple2: The second tuple to compare.
    */
    proc assertTupleGreater(tuple1, tuple2) throws {
      type firstType = tuple1.type,
          secondType = tuple2.type;
      if firstType == secondType {
        assertSequenceGreater(tuple1,tuple2,"tuple("+firstType: string+")");
      }
      else {
        var tmpString = "assert failed - First element is of type " + firstType:string +" and Second is of type "+secondType:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A range-specific greater assertion.
      Args:
      range1: The first range to compare.
      range2: The second range to compare.
    */
    proc assertRangeGreater(range1, range2) throws {
      if range1.size == range2.size {
        __baseAssertGreater(range1,range2);
      }
      else {
        var tmpString = "assert failed - Ranges are not of same length";
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A string-specific Greater assertion.
      Args:
      string1: The first string to compare.
      string2: The second string to compare.
    */
    proc assertStringGreater(string1, string2) throws {
      if string1.size == string2.size {
        assertSequenceGreater(string1,string2,"String");
      }
      else {
        var tmpString = "assert failed - Strings are not of same length";
        throw new owned AssertionError(tmpString);
      }
    }

     pragma "no doc"
    /*The default assertGreater implementation, not type specific.*/
    proc __baseAssertGreater(first, second) throws {
      if all(first <= second) {
        var tmpString = "assert failed - '" + stringify(first) +"' <= '"+stringify(second)+"'";
        throw new owned AssertionError(tmpString);
      }
    }

    /*
      Assert that a first argument is less than second argument.  If it is false, raises AssertionError.

      :arg first: The first object to compare.
      :arg second: The second object to compare.
      :throws AssertionError: If the first argument is not less than the second argument.
    */
    proc assertLessThan(first, second) throws {
      if canResolve("<=",first, second) {
        checkLessThan(first, second);
      }
      else {
        var tmpString = "assert failed - First element is of type " + first.type:string +" and Second is of type "+second.type:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*checks the type of the arguments and then do less than comparison */
    proc checkLessThan(first, second) throws {
      type firstType = first.type,
          secondType = second.type;

      if isTupleType(firstType) && isTupleType(secondType) {
        // both are tuples.
        assertTupleLess(first, second);
        return;
      }
      else if isArrayType(firstType) && isArrayType(secondType) {
        // both are arrays
        assertArrayLess(first, second);
        return;
      }
      else if isRangeType(firstType) && isRangeType(secondType) {
        // both are Range
        assertRangeLess(first, second);
      }
      else if isString(first) && isString(second) {
        // both are Strings
        assertStringLess(first, second);
      }
      else {
        __baseAssertLess(first, second);
      }
    }

    pragma "no doc"
    /*An less than assertion for sequences (like arrays, tuples, strings).
      Args:
      seq1: The first sequence to compare.
      seq2: The second sequence to compare.
      seq_type_name: The name of datatype of the sequences
    */
    proc assertSequenceLess(seq1, seq2, seq_type_name) throws {
      var checkless: bool = false,
          checkequal: bool = false;
      const len1 = seq1.size,
            len2 = seq2.size;
      var symbol: string,
          tmpString: string,
          tmplarge: string;

      if len1 == 0 {
        tmpString = "First "+seq_type_name+" has no length.";
      }
      if tmpString == "" {
        if len2 == 0 {
          tmpString = "Second "+seq_type_name+" has no length.";
        }
      }
      if tmpString == "" {
        for i in 1..len1 {
          var item1 = seq1[i],
              item2 = seq2[i];
          if item1 == item2 then checkequal = true;
          else if item1 > item2 {
            tmpString += "First "+seq_type_name+" > Second "+seq_type_name+" :\n";
            tmplarge += "\nFirst larger element in first "+seq_type_name+" is at index "+i:string +":\n'"+item1:string+"'\n'"+item2:string+"'\n";
            checkless = true;
            checkequal = false;
            symbol = ">";
            break;
          }
          else {
            checkequal = false;
            break;
          }
        }
        if !checkless && !checkequal then return;
        else if checkequal {
          tmpString += "Both "+seq_type_name+" are equal\n";
          symbol = "==";
        }
        if seq_type_name == "Array" {
          tmpString += "'[";
          for i in 1..seq1.size {
            if i != seq1.size then tmpString+= seq1[i]:string+", ";
            else tmpString += seq1[i]:string+"]'"+symbol+ "'[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]:string+", ";
            else tmpString += seq2[i]:string+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"'"+symbol+"'"+stringify(seq2)+"'" ;
        }
        tmpString+=tmplarge;
      }
      throw new owned AssertionError(tmpString);
    }

    pragma "no doc"
    /*An array-specific less than assertion.
      Args:
      array1: The first array to compare.
      array2: The second array to compare.
    */
    proc assertArrayLess(array1, array2) throws {
      if array1.rank == array2.rank {
        if array1.shape == array2.shape {
          if array1.rank == 1 {
            assertSequenceLess(array1, array2, "Array");
          }
          else {
            if all(array1 >= array2) {
              var tmpString = "assert failed - \n'" + stringify(array1) +"'\n>=\n'"+stringify(array2)+"'";
              throw new owned AssertionError(tmpString);
            }
        }
        }
        else {
          var tmpString = "assert failed - First element is of shape " + stringify(array1.shape) +" and Second is of shape "+stringify(array2.shape);
          throw new owned AssertionError(tmpString);
        }
      }
      else {
        var tmpString = "assert failed - First element is of type " + array1.type:string +" and Second is of type "+array2.type:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A tuple-specific less than assertion.
      Args:
      tuple1: The first tuple to compare.
      tuple2: The second tuple to compare.
    */
    proc assertTupleLess(tuple1, tuple2) throws {
      type firstType = tuple1.type,
          secondType = tuple2.type;
      if firstType == secondType {
        assertSequenceLess(tuple1,tuple2,"tuple("+firstType: string+")");
      }
      else {
        var tmpString = "assert failed - First element is of type " + firstType:string +" and Second is of type "+secondType:string;
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A range-specific Less than assertion.
      Args:
      range1: The first range to compare.
      range2: The second range to compare.
    */
    proc assertRangeLess(range1, range2) throws {
      if range1.size == range2.size {
        __baseAssertLess(range1,range2);
      }
      else {
        var tmpString = "assert failed - Ranges are not of same length";
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*
      A string-specific Less than assertion.
      Args:
      string1: The first string to compare.
      string2: The second string to compare.
    */
    proc assertStringLess(string1, string2) throws {
      if string1.size == string2.size {
        assertSequenceLess(string1,string2,"String");
      }
      else {
        var tmpString = "assert failed - Strings are not of same length";
        throw new owned AssertionError(tmpString);
      }
    }

    pragma "no doc"
    /*The default assertGreater implementation, not type specific.*/
    proc __baseAssertLess(first, second) throws {
      if all(first >= second) {
        var tmpString = "assert failed - '" + stringify(first) +"'>='"+stringify(second)+"'";
        throw new owned AssertionError(tmpString);
      }
    }

    /*
      Specify Max Number of Locales required to run the test

      :arg value: Maximum number of locales with which the test can be ran.
      :type value: `int`.

      :throws UnexpectedLocalesError: If `value` is less than 1 or `minNumLocales`
    */
    proc maxLocales(value: int) throws {
      this.numMaxLocales = value;
      if this.numMaxLocales < 1 {
        throw new owned UnexpectedLocales("Max Locales is less than 1");
      }
      if this.numMaxLocales < this.numMinLocales {
        throw new owned UnexpectedLocales("Max Locales is less than Min Locales");
      }
      if value < numLocales {
        throw new owned TestIncorrectNumLocales("Required Locales = "+value:string);
      }
    }

    /*
      Specify Min Number of Locales required to run the test

      :arg value: Minimum number of locales with which the test can be ran.
      :type value: `int`.

      :throws UnexpectedLocalesError: If `value` is more than `maxNumLocales`
    */
    proc minLocales(value: int) throws {
      this.numMinLocales = value;
      if this.numMaxLocales < this.numMinLocales {
        throw new owned UnexpectedLocales("Max Locales is less than Min Locales");
      }
      if value > numLocales {
        throw new owned TestIncorrectNumLocales("Required Locales = "+value:string);
      }
    }

    /*
      To add locales in which test can be run.

      :arg locales: Multiple ``,`` separated locale values

      :throws UnexpectedLocalesError: If `locales` are already added.

    */
    proc addNumLocales(locales: int ...?n) throws {
      var canRun =  false;
      if this.dictDomain.size > 0 {
        throw new owned UnexpectedLocales("Locales already added.");
      }
      for curLocale in locales {
        this.dictDomain.add(curLocale);
        if curLocale == numLocales {
          canRun = true;
        }
      }
      if !canRun {
        var localesErrorStr= "Required Locales = ";
        localesErrorStr += ",".join(this.dictDomain:string);
        throw new owned TestIncorrectNumLocales(localesErrorStr);
      }
    }

    /*Adds the tests in which the given test is depending.

      :arg tests: Multiple ``,`` separated First Class Test Functions.
      :throws DependencyFound: If Called for the first time in a function.

    */
    proc dependsOn(tests: argType ...?n) throws {
      if testDependsOn.size == 0 {
        for eachSuperTest in tests {
          this.testDependsOn.append(eachSuperTest);
        }
        throw new owned DependencyFound();
      }
    }
  }

  pragma "no doc"
  /*A test result class that can print formatted text results to a stream.*/
  class TextTestResult {
    var separator1 = "="* 70,
        separator2 = "-"* 70;

    proc startTest(test) throws {
      stdout.writeln(test: string);
    }

    proc addError(test, errMsg) throws {
      stdout.writeln("Flavour: ERROR");
      PrintError(errMsg);
    }

    proc addFailure(test, errMsg) throws {
      stdout.writeln("Flavour: FAIL");
      PrintError(errMsg);
    }

    proc addSuccess(test) throws {
      stdout.writeln("Flavour: OK");
      stdout.writeln(this.separator1);
      stdout.writeln(this.separator2);
    }

    proc addSkip(test, reason) throws {
      stdout.writeln("Flavour: SKIPPED");
      PrintError(reason);
    }

    proc addIncorrectNumLocales(test, reason) throws {
      stdout.writeln("Flavour: IncorrectNumLocales");
      PrintError(reason);
    }

    proc dependencyNotMet(test) throws {
      stdout.writeln("Flavour: Dependence");
      stdout.writeln(this.separator1);
      stdout.writeln(this.separator2);
    }

    proc PrintError(err) throws {
      stdout.writeln(this.separator1);
      stdout.writeln(err);
      stdout.writeln(this.separator2);
    }

  }

  pragma "no doc"
  class TestSuite {
    var testCount = 0;
    var _tests: list(argType);

    // TODO: Get lifetime checking working in this case and remove pragma unsafe.
    // Pragma "unsafe" disables the lifetime checker here.
    pragma "unsafe"
    proc addTest(test) {
      this._tests.append(test);
      this.testCount += 1;
    }

    // TODO: Get lifetime checking working in this case and remove pragma unsafe.
    // Pragma "unsafe" disables the lifetime checker here.
    pragma "unsafe"
    proc addTests(tests) {
      for test in tests do
        this.addTest(test);
    }

    proc this(i: int) ref: argType {
      return this._tests[i];
    }

    iter these() {
      for i in this._tests do
        yield i;
    }
  }

  /*Runs the tests

    Call this as

    .. code-block:: chapel

      UnitTest.main();
  */
  proc main() throws {

    var testStatus: map(string, bool, parSafe=true),
        testsFailed: map(string, bool, parSafe=true),
        testsErrored: map(string, bool, parSafe=true),
        testsLocalFails: map(string, bool, parSafe=true),
        testsPassed: map(string, bool, parSafe=true),
        testsSkipped: map(string, bool, parSafe=true);
    // Assuming 1 global test suite for now
    // Per-module or per-class is possible too
    var testSuite = new TestSuite();
    //Variable for passing to the primitive
    var testObjGather: owned Test = new Test();
    // gather all the tests
    param n = __primitive("gather tests", testObjGather.borrow());

    for param i in 1..n {
      var test_FCF = __primitive("get test by index",i);
      if (test_FCF: string != tempFcf: string) {
        testSuite.addTest(test_FCF);
      }
    }

    for test in testSuite {
      const testName = test: string;
      testStatus[testName] = false;
      testsFailed[testName] = false;
      testsErrored[testName] = false;
      testsLocalFails[testName] = false;
      testsPassed[testName] = false;
      testsSkipped[testName] = false;
    }
    if testNames != "None" {
      for test in testNames.split(" ") {
        testsLocalFails[test.strip()] = true;
      }
    }
    if failedTestNames != "None" {
      for test in failedTestNames.split(" ") {
        testsFailed[test.strip()] = true; // these tests failed or skipped
        testStatus[test.strip()] = true;
      }
    }
    if errorTestNames != "None" {
      for test in errorTestNames.split(" ") {
        testsErrored[test.strip()] = true; // these tests failed or skipped
        testStatus[test.strip()] = true;
      }
    }
    if skippedTestNames != "None" {
      for test in skippedTestNames.split(" ") {
        testsSkipped[test.strip()] = true; // these tests failed or skipped
        testStatus[test.strip()] = true;
      }
    }
    if ranTests != "None" {
      for test in ranTests.split(" ") {
        testsPassed[test.strip()] = true; // these tests failed or skipped
        testStatus[test.strip()] = true;
      }
    }

    for test in testSuite {
      if !testStatus[test: string] {
        // Create a test object per test
        var checkCircle: list(string);
        var circleFound = false;
        var testObject = new Test();
        runTestMethod(testStatus, testObject, testsFailed, testsErrored, testsSkipped,
                      testsLocalFails, test, checkCircle, circleFound);
      }
    }
  }

  private
  proc runTestMethod(ref testStatus, ref testObject, ref testsFailed, ref testsErrored,
                      ref testsSkipped, ref testsLocalFails, test, ref checkCircle,
                      ref circleFound) throws {
    var testResult = new TextTestResult();
    var testName = test: string; //test is a FCF:
    checkCircle.append(testName);
    try {
      testResult.startTest(testName);
      test(testObject);
      testResult.addSuccess(testName);
      testsLocalFails[testName] = false;
    }
    // A variety of catch statements will handle errors thrown
    catch e: AssertionError {
      testResult.addFailure(testName, e: string);
      testsFailed[testName] = true;
      // print info of the assertion error
    }
    catch e: DependencyFound {
      var allTestsRan = true;
      for superTest in testObject.testDependsOn {
        var checkCircleCount = checkCircle.count(superTest: string);
        // cycle is checked
        if checkCircleCount > 0 {
          testsSkipped[testName] = true;
          circleFound = true;
          var failReason = testName + " skipped because circular dependency found";
          testResult.addSkip(testName, failReason);
          testStatus[testName] = true;
          return;
        }
        // if super test didn't Error or Failed or skipped
        if !testsErrored[superTest: string] && !testsFailed[superTest: string] && !testsSkipped[superTest: string]
        {
          // checking if super test ran or not.
          if !testStatus[superTest: string] {
            // Create a test object per test
            var superTestObject = new Test();
            // running the super test
            runTestMethod(testStatus, superTestObject, testsFailed, testsErrored,
                          testsSkipped, testsLocalFails, superTest, checkCircle,
                          circleFound);
            var removeSuperTestCount = checkCircle.count(superTest: string);
            if removeSuperTestCount > 0 {
              checkCircle.remove(superTest: string);
            }
            // if super test failed
            if testsFailed[superTest: string] {
              testsSkipped[testName] = true; // current test have failed or skipped
              var skipReason = testName + " skipped because " + superTest: string +" failed";
              testResult.addSkip(testName, skipReason);
              break;
            }
            // if super test failed
            if testsSkipped[superTest: string] {
              testsSkipped[testName] = true; // current test have failed or skipped
              var skipReason = testName + " skipped because " + superTest: string +" skipped";
              testResult.addSkip(testName, skipReason);
              break;
            }
            // this superTest has not yet finished.
            if testsLocalFails[superTest: string] {
              allTestsRan = false;
            }

            // if Circle Found running the superTests
            if circleFound then break;

            // if superTest error then
            if testsErrored[superTest: string] {
              testsSkipped[testName] = true;
              var skipReason = testName + " skipped because " + superTest: string +" gave an Error";
              testResult.addSkip(testName, skipReason);
              break;
            }
          }
        }
        // super test Errored
        else if testsErrored[superTest: string] {
          testsSkipped[testName] = true;
          var skipReason = testName + " skipped because " + superTest: string +" gave an Error";
          testResult.addSkip(testName, skipReason);
          break;
        }
        // super test Skipped
        else if testsSkipped[superTest: string] {
          testsSkipped[testName] = true;
          var skipReason = testName + " skipped because " + superTest: string +" Skipped";
          testResult.addSkip(testName, skipReason);
          break;
        }
        //super test failed
        else {
          testsSkipped[testName] = true; // current test have failed or skipped
          var skipReason = testName + " skipped because " + superTest: string +" failed";
          testResult.addSkip(testName, skipReason);
        }
      }
      if circleFound {
        testsSkipped[testName] = true;
        var skipReason = testName + " skipped because circular dependency found";
        testResult.addSkip(testName, skipReason);
      }
      // Test is not having error or failures or dependency or skipped
      else if !testsErrored[testName] && allTestsRan && !testsFailed[testName] && !testsSkipped[testName]
      {
        testObject.dictDomain.clear(); // clearing so that we don't get Locales already added
        runTestMethod(testStatus, testObject, testsFailed, testsErrored, testsSkipped,
                      testsLocalFails, test, checkCircle, circleFound);
      }
      else if !testsErrored[testName] && !allTestsRan && !testsFailed[testName] && !testsSkipped[testName]
      {
        testResult.dependencyNotMet(testName);
      }
    }
    catch e: TestSkipped {
      testResult.addSkip(testName, e: string);
      testsSkipped[testName] = true ;
      // Print info on test skipped
    }
    catch e: TestIncorrectNumLocales {
      testResult.addIncorrectNumLocales(testName, e: string);
      testsLocalFails[testName] = true;;
    }
    catch e: UnexpectedLocales {
      testResult.addFailure(testName, e: string);
      testsFailed[testName] = true ;
    }
    catch e {
      testResult.addError(testName, e:string);
      testsErrored[testName] = true ;
    }
    testStatus[testName] = true;
  }

  module TestError {
    /*
    :class:`TestError` is a base class.
    */
    class TestError: Error {
      var details: string;

      proc init(details: string = "") {
        this.details = details;
      }

      // Message function overridden here
      override proc message() {
        return this.details;
      }
    }

    /*Assertion Error class. Raised when assert Function Failed*/
    class AssertionError: TestError {
      proc init(details: string = "") {
        super.init(details);
      }
    }

    /* TestSkipped Error Class. Raised when a test is skipped.*/
    class TestSkipped: TestError {
      proc init(details: string = "") {
        super.init(details);
      }
    }

    /* DependencyFound Error Class. Raised when a all dependency
      of a test are not met.
    */
    class DependencyFound: TestError {
      proc init(details: string = "") {
        super.init(details);
      }
    }

    /* TestIncorrectNumLocales Error Class. Raised when test is not run with
      expected number of Locales.
    */
    class TestIncorrectNumLocales: TestError {
      proc init(details: string = "") {
        super.init(details);
      }
    }

    /* UnexpectedLocales Error Class. Raised when test has
        locales with which it can't be run.
        Eg: MaxLocales < MinLocales
    */
    class UnexpectedLocales: TestError {
      proc init(details: string = "") {
        super.init(details);
      }
    }
  }
}
