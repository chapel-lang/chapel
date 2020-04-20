use UnitTest;

proc testAssert(x,y,reason="") {
  var sep = "=="*40;
  var test = new Test();
  try {
    test.assertNotEqual(x,y);
  } catch e {
    writeln("Error Caught in "+reason);
    writeln(e);
    writeln(sep);
  }
}


proc main() {
  equalArray();
  equalIntegers();
  equalRanges();
  equalStrings();
  unequalHighArraySameType();
  equalHighArrayDiffType();
  Unequal1DArray();
  UnequalComplex();
  UnequalPrimitive();
  UnequalRanges();
  UnequalTuples();
}

proc equalArray() {
  var x1D = [1,2,5];
  var y1D = [1,2,5];
  var xND : [1..5,1..7] real;
  var yND : [{1..5,1..7}] real;
  testAssert(x1D,y1D,"Equal 1D Arrays");
  testAssert(xND,yND,"Equal High Dim Arrays");

  var x0: [0..2] int = [1,2,4];
  var y0: [0..2] int = [1,2,4];
  testAssert(y0, x0,"Equal 0-based arrays");
}

proc equalIntegers() {
  var x1 = 1.0;
  var y1 = 1;
  testAssert(x1,y1,"Equal Integers");
}

proc equalRanges() {
  var x2 = 1..6;
  var y2 = 1..6;
  testAssert(x2,y2,"Equal Ranges");
}

proc equalStrings() {
  var x3 = "equal string";
  var y3 = "equal string";
  testAssert(x3,y3,"Equal Strings");
}

proc unequalHighArraySameType() {
  var x1 : [1..5,1..7,1..6] real;
  var y1 : [{1..5,1..7}] real;
  testAssert(x1,y1,"Unequal Array Same Type");
}

proc equalHighArrayDiffType() {
  var x2 : [1..5,1..7] real;
  var y2 : [{1..5,1..7}] int;
  testAssert(x2,y2,"Equal High Array Diff Type");
}

proc Unequal1DArray() {
  // Type 1
  var x1 = [1,5,7];
  var y1 = [1,7,6];
  testAssert(x1,y1,"Unequal 1D Array Type 1");
  // Type 2
  var x2 = [1,7,8,5,8];
  var y2 = [1,7,6,4];
  testAssert(x2,y2,"Unequal 1D Array Type 2");
  //Type 3
  var x3 = [1,7,6,5,8];
  var y3 = [1,7,6];
  testAssert(x3,y3,"Unequal 1D Array Type 3");

  var x0: [0..2] int = [1,2,8];
  var y0: [0..2] int = [1,2,4];
  testAssert(y0, x0,"Unequal 0-based arrays");
}

proc UnequalComplex() {
  // Type 1
  var x1 = 5;
  var y1 = 7i;
  testAssert(x1,y1,"Unequal Complex Type 1");
  //Type 2
  var x2 = 5 + 6i;
  var y2 = 7i;
  testAssert(x2,y2,"Unequal Complex Type 2");
}

proc UnequalPrimitive() {
  //Type 1
  var x1 = true;
  var y1 = false;
  testAssert(x1,y1,"Unequal Bool Types");
  //Type 2
  var x2 = 1.5;
  var y2 = 1.55;
  testAssert(x2,y2,"Unequal Float");
  //Type 3
  var x = "unequalstring";
  var y = "unequalString";
  testAssert(x,y,"Unequal String");
}

proc UnequalRanges() {
  //Type 1
  var x1 = ..6;
  var y1 = ..7;
  testAssert(x1,y1,"Unequal Ranges Type 1");
  //Type 2
  var x2 = 1..6;
  var y2 = 2..7;
  testAssert(x2,y2,"Unequal Ranges Type 2");
}

proc UnequalTuples() {
  var x1 = (1,2,3);
  var y1 = (1,2);
  testAssert(x1,y1,"Unequal Tuples Type 1");
  //Type 2
  var x2 = (1,2,3);
  var y2 = (1,2,5);
  testAssert(x2,y2,"Unequal Tuples Type 2");

}
