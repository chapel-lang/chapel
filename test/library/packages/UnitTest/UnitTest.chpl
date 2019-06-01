module UnitTest {
  use Reflection;
  /*
    Assert that a boolean condition is true.  If it is false, prints
    'assert failed' and halts the program.

    :arg test: the boolean condition
    :type test: `bool`
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assertTrue(test: bool) {
    if !test then
      __primitive("chpl_error", c"assertTrue failed. Given expression is False");
  }

  /*
    Assert that a boolean condition is false.  If it is true, prints
    'assert failed' and halts the program.

    :arg test: the boolean condition
    :type test: `bool`
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assertFalse(test: bool) {
    if test then
      __primitive("chpl_error", c"assertFalse failed. Given expression is True");
  }
  
  /*Function to call the respective method for equality checking based on the type of argument*/
  private
  proc checkAssertEquality(first, second) {
    type firstType = first.type;
    type secondType = second.type;

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

  /*
    Check that a boolean array is true.  If any element is false, returns 'false'
    else return 'true'.

    :arg it: the iterator to the array
    :type it: `iterator`
  */
  private
  proc all(it: _iteratorRecord) {
    for b in it do if b == false then return false;
    return true;
  }
  /* Method overloading for the above function. Return the argument itself
  */
  private
  proc all(check: bool) {
    return check;
  }

  /*An equality assertion for sequences (like arrays, tuples, strings, range).
    Args:
      seq1: The first sequence to compare.
      seq2: The second sequence to compare.
      seq_type_name: The name of datatype of the sequences
  */
  private
  proc assertSequenceEqual(seq1, seq2, seq_type_name) {
    var tmpString: string;
    var len1: int = seq1.size;
    var len2: int = seq2.size;
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
        var item1 = seq1[i];
        var item2 = seq2[i];
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
  
    __primitive("chpl_error", tmpString.c_str());
  }

  /*An array-specific equality assertion.
    Args:
    array1: The first array to compare.
    array2: The second array to compare.
  */
  private
  proc assertArrayEqual(array1, array2) {
    type firstType = array1.type;
    type secondType = array2.type;
    if firstType == secondType {
      if array1.rank == 1 {
        assertSequenceEqual(array1, array2, "Array");
      }
      else {
        if !array1.equals(array2) {
          var tmpString = "assert failed - \n'" + stringify(array1) +"'\n!=\n'"+stringify(array2)+"'";
          __primitive("chpl_error", tmpString.c_str());
        }
      }
    }
    else {
      var tmpString = "assert failed - \n'" + stringify(array1) +"'\nand\n'"+stringify(array2) + "'\nare not of same type";
      __primitive("chpl_error",tmpString.c_str());
    }
  }

  /*
    A tuple-specific equality assertion.
    Args:
    tuple1: The first tuple to compare.
    tuple2: The second tuple to compare.
  */
  private
  proc assertTupleEqual(tuple1, tuple2) {
    type firstType = tuple1.type;
    type secondType = tuple2.type;
    if firstType == secondType {
      assertSequenceEqual(tuple1,tuple2,"tuple("+firstType: string+")");
    }
    else {
      var tmpString = "assert failed - '" + stringify(tuple1) +"' and '"+stringify(tuple2) + "' are not of same type";
      __primitive("chpl_error",tmpString.c_str());
    }
  }

  /*
    A range-specific equality assertion.
    Args:
    range1: The first range to compare.
    range2: The second range to compare.
  */
  private
  proc assertRangeEqual(range1, range2) {
    __baseAssertEqual(range1,range2);
  }
  
  /*
    A string-specific equality assertion.
    Args:
    string1: The first string to compare.
    string2: The second string to compare.
  */
  private
  proc assertStringEqual(string1, string2) {
    assertSequenceEqual(string1,string2,"String");
  }


  /*The default assertEqual implementation, not type specific.*/
  private
  proc __baseAssertEqual(first, second) {
    if canResolve("!=",first,second) {
      if (first != second) {
        var tmpString = "assert failed - '" + stringify(first) +"' != '"+stringify(second)+"'";
        __primitive("chpl_error", tmpString.c_str());
      }
    }
    else {
      var tmpString = "assert failed - '" + stringify(first) +"' and '"+stringify(second) + "' are not of same type";
      __primitive("chpl_error",tmpString.c_str());
    }
  }
  
  /*
    Fail if the two objects are unequal as determined by the '==' operator.
  */
  proc assertEqual(first, second){
    checkAssertEquality(first, second);
  }
  /* Function that checks whether two arguments are unequal or not*/
  private
  proc checkAssertUnEquality(first,second) {
    type firstType = first.type;
    type secondType = second.type;
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
  proc assertNotEqual(first, second) {
    if canResolve("!=",first, second) {
      if !checkAssertUnEquality(first,second) {
        var tmpString = "assert failed - \n'" + stringify(first) +"'\n== \n'"+stringify(second)+"'";
        __primitive("chpl_error", tmpString.c_str());
      }
    }
  }
}
