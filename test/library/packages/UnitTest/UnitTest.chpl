/*
Module UnitTest provides support for automated testing in Chapel .
Any function of the form

.. code-block:: chapel
  
  proc funcName(test: Test) throws {}

is treated as a test function.
*/
module UnitTest {
  use Reflection;
  use TestError;
  pragma "no doc"
  config const skipId: int = 0;
  config const testNames: string = "None";
  // This is a dummy test to capture the function signature
  private
  proc testSignature(test: Test) throws { }
  pragma "no doc"
  var tempFcf = testSignature;
  pragma "no doc"
  type argType = tempFcf.type;  //Type of First Class Test Functions

  class Test {
    var numMaxLocales: int,
        numMinLocales: int;
    var dictDomain: domain(int);
    var numLocalesList: [dictDomain] int;
    /* Unconditionally skip a test. */
    proc skip(reason: string = "") throws {
      throw new owned TestSkipped(reason);
    }

    /*
    Skip a test if the condition is true.
   */
    proc skipIf(condition: bool, reason: string = "") throws {
      if condition then
        skip(reason);
    }

    /*
      Assert that a boolean condition is true.  If it is false, prints
      'assert failed' and rasies AssertionError. 

      :arg test: the boolean condition
      :type test: `bool`
    */
    pragma "insert line file info"
    pragma "always propagate line file info"
    proc assertTrue(test: bool) throws {
      if !test then
        throw new owned AssertionError("assertTrue failed. Given expression is False");
    }

    /*
      Assert that a boolean condition is false.  If it is true, prints
      'assert failed' and raises AssertionError.

      :arg test: the boolean condition
      :type test: `bool`
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
            if i != seq1.size then tmpString+= seq1[i]+", ";
            else tmpString += seq1[i]+"]' != '[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]+", ";
            else tmpString += seq2[i]+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"' != '"+stringify(seq2)+"'" ;
        }
        for i in 1..min(len1,len2) {
          var item1 = seq1[i],
              item2 = seq2[i];
          if item1 != item2 {
            tmpString += "\nFirst differing element at index "+i +":\n'"+item1+"'\n'"+item2+"'\n";
            break;
          }
        }
        if len1 > len2 {
          var size_diff = len1 - len2;
          tmpString += "\nFirst "+seq_type_name+" contains "+ size_diff +" additionl elements.\n";
          tmpString += "First extra element is at index "+(len2+1)+"\n'"+seq1[len2+1]+"'\n";
        }
        else if len1 < len2 {
          var size_diff = len2 - len1;
          tmpString += "\nSecond "+seq_type_name+" contains "+ size_diff +" additionl elements.\n";
          tmpString += "First extra element is at index "+(len1+1)+"\n'"+seq2[len1+1]+"'\n";
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
    proc assertArrayEqual(array1, array2) throws {
      type firstType = array1.type,
          secondType = array2.type;
      if firstType == secondType {
        if array1.rank == 1 {
          assertSequenceEqual(array1, array2, "Array");
        }
        else {
          if !array1.equals(array2) {
            var tmpString = "assert failed - \n'" + stringify(array1) +"'\n!=\n'"+stringify(array2)+"'";
            throw new owned AssertionError(tmpString);
          }
        }
      }
      else {
        var tmpString = "assert failed - \n'" + stringify(array1) +"'\nand\n'"+stringify(array2) + "'\nare not of same type";
        throw new owned AssertionError(tmpString);
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
      Fail if the two objects are unequal as determined by the '==' operator.
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
      Fail if the two objects are equal as determined by the '==' operator and type.
    */
    proc assertNotEqual(first, second) throws {
      if canResolve("!=",first, second) {
        if !checkAssertInequality(first,second) {
          var tmpString = "assert failed - \n'" + stringify(first) +"'\n== \n'"+stringify(second)+"'";
          throw new owned AssertionError(tmpString);
        }
      }
    }

    /*
      Assert that a first argument is greater than second argument.  If it is false, prints
      'assert failed' and rasies AssertionError. 
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
    /*checks the type of the arguments and then do greater than comaprison */
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
            tmplarge += "\nFirst larger element in second "+seq_type_name+" is at index "+i +":\n'"+item1+"'\n'"+item2+"'\n";
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
            if i != seq1.size then tmpString+= seq1[i]+", ";
            else tmpString += seq1[i]+"]' "+symbol+ " '[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]+", ";
            else tmpString += seq2[i]+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"' "+symbol+" '"+stringify(seq2)+"'" ;
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
              var tmpString = "assert failed - \n'" + stringify(array1) +"'\n<=\n'"+stringify(array2)+"'";
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
      Assert that a first argument is less than second argument.  If it is false, prints
      'assert failed' and rasies AssertionError. 
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
    /*checks the type of the arguments and then do less than comaprison */
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
            tmplarge += "\nFirst larger element in first "+seq_type_name+" is at index "+i +":\n'"+item1+"'\n'"+item2+"'\n";
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
            if i != seq1.size then tmpString+= seq1[i]+", ";
            else tmpString += seq1[i]+"]' "+symbol+ " '[";
          }
          for i in 1..seq2.size {
            if i != seq2.size then tmpString+= seq2[i]+", ";
            else tmpString += seq2[i]+"]'";
          }
        }
        else {
          tmpString += "'"+stringify(seq1)+"' "+symbol+" '"+stringify(seq2)+"'" ;
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
        var tmpString = "assert failed - '" + stringify(first) +"' >= '"+stringify(second)+"'";
        throw new owned AssertionError(tmpString);
      }
    }

    /*Specify Max Number of Locales required to run the test*/
    proc maxLocales(value: int) throws {
      this.numMaxLocales = value;
      if value < numLocales {
        throw new owned TestIncorrectNumLocales("Required Locales ="+value);
      }
    }

    /*Specify Min Number of Locales required to run the test*/
    proc minLocales(value: int) throws {
      this.numMinLocales = value;
      if value > numLocales {
        throw new owned TestIncorrectNumLocales("Required Locales ="+value);
      }
    }

    /*To add how many locales this test requires*/
    proc addNumLocales(locales: int ...?n) throws {
      var canRun =  false;
      for numLocale in locales {
        this.numLocalesList[numLocale]=1;
        if numLocale == numLocales {
          canRun = true;
        }
      }
      var tempList: [1..0] int;
      for key in dictDomain do tempList.push_back(key);
      if !canRun then 
        throw new owned TestIncorrectNumLocales("Required Locales ="+tempList:string);
    }
  }

  pragma "no doc"
  /*A test result class that can print formatted text results to a stream.*/
  class TextTestResult {
    var separator1 = "="* 70,
        separator2 = "-"* 70;
    
    proc startTest(test, indx) throws {
      stdout.write(test: string,"[",indx: string,"]: ");
    }

    proc addError(test, errMsg) throws {
      stdout.writeln("ERROR");
      PrintError(errMsg);
    }

    proc addFailure(test, errMsg) throws {
      stdout.writeln("FAIL");
      PrintError(errMsg);
    }

    proc addSuccess(test) throws {
      stdout.writeln("OK");
    }

    proc addSkip(test, reason) throws {
      stdout.writeln("SKIPPED");
      PrintError(reason);
    }

    proc addIncorrectNumLocales(test, reason) throws {
      stdout.writeln("IncorrectNumLocales");
      PrintError(reason);
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
    var _tests: [1..0] argType;
    
    proc addTest(test) lifetime this < test {
      // var tempTest = new Test();
      // param test_name = test: string;
      // if !canResolve(test_name,tempTest) then
      //   compilerError(test + " is not callable");
      this._tests.push_back(test);
      this.testCount += 1;
    }

    proc addTests(tests) lifetime this < tests {
      /*if isString(tests) then
        compilerError("tests must be an iterable, not a string");*/
      for test in tests do
        this.addTest(test);
    }

    proc this(i : int) ref: argType {
      return this._tests[i];
    }

    iter these() {
      for i in this._tests do
        yield i;
    }
  }

  /*Runs the tests*/
  proc runTest(tests: argType ...?n) throws {

    var runAllTests = true;
    var testNameList: [1..0] string;
    if testNames != "None" {
      runAllTests = false;
      for test in testNames.split(" ") do testNameList.push_back(test.strip());
    }
    // Assuming 1 global test suite for now
    // Per-module or per-class is possible too
    var testSuite = new TestSuite();
    testSuite.addTests(tests);
    var testResult = new TextTestResult();
    // if skipId == 0 then
    //   stdout.writeln("Found "+testSuite.testCount+" "+printTest(testSuite.testCount));
    for indx in (skipId+1)..testSuite.testCount {
      var test = testSuite[indx];
      try {
        if runAllTests {
          // Create a test object per test
          var testObject = new Test();
          //test is a FCF:
          testResult.startTest(test: string, indx);
          test(testObject);
          testResult.addSuccess(test: string);
        }
        else {
          var checkStatus = testNameList.find(test:string);
          if checkStatus[1] {
            // Create a test object per test
            var testObject = new Test();
            //test is a FCF:
            testResult.startTest(test: string, indx);
            test(testObject);
            testResult.addSuccess(test: string);
          }
        }
        
      }
      // A variety of catch statements will handle errors thrown
      catch e: AssertionError {
        testResult.addFailure(test:string, e:string);
        // print info of the assertion error
      }
      catch e: TestSkipped {
        testResult.addSkip(test:string, e:string);
        // Print info on test skipped
      }
      catch e: TestDependencyNotMet {
        // Pop test out of array and append to end
      }
      catch e: TestIncorrectNumLocales {
        testResult.addIncorrectNumLocales(test:string,e: string);
      }
      catch e { 
        testResult.addError(test:string, e:string);
      }
    }
    // testResult.printErrors();
    // stdout.writeln(testResult.separator2);
    // testResult.PrintResult();
  }
}
