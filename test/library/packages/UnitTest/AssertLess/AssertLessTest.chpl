use UnitTest;
proc testAssert(x,y,reason="") {
  var sep = "=="*40;
  var test = new Test();
  try {
    test.assertLessThan(x,y);
  } catch e {
    writeln("Error Caught in "+reason);
    writeln(e);
    writeln(sep);
  }
}

proc main() {
  ArrayOthers();
  UnequalComplex();
  equalStrings();
  equalRanges();
  equalIntegers();
  lessOthers();
  LessRange();
  tupleOther();
  tupleList();
  NotLessString();
  complextuple();
}

proc ArrayOthers() {

  var x1 : [1..5,1..7,1..6] real;
  var y1 : [{1..5,1..7}] real;
  testAssert(x1,y1,"Array Others Diff Dim");

  var x2 = [1,2,5];
  var y2 = [1,2,5];
  testAssert(x2,y2,"Array Others Equal 1D");

  var x3 : [1..5,1..7] real;
  var y3 : [{1..5,1..7}] real;
  testAssert(x3,y3,"Array Others Equal High Dim");

  var x4 : [1..5,1..7] real;
  var y4 : [{1..7,1..5}] int;
  testAssert(x4,y4,"Array Others Diff Shape High Dim");

  var x5 = [1,7,6,5,8];
  var y5 = [1,7,6];
  testAssert(x5,y5,"Array Others Diff Shape 1D");

  var x0: [0..2] int = [2,4,8];
  var y0: [0..2] int = [1,2,4];
  testAssert(y0, x0,"Array 0-based");
}

proc UnequalComplex() {
  // Type 1
  var x1 = 5;
  var y1 = 7i;
  testAssert(x1,y1,"Unequal Complex Type 1");
  //Type 2
  var x2 = 1 + 5i;
  var y2 = 5i;
  testAssert(x2,y2,"Unequal Complex Type 2");
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

proc lessOthers() {
  var x = [1,2,3];
  var y = [1,2,5];
  testAssert(x,y,"lessOther 1D array");
  var x2 = (1,2,3);
  var y2 = (1,2,5);
  testAssert(x2,y2,"lessOther tuples");
  var x3 = 2;
  var y3 = 2.5;
  testAssert(x3,y3,"lessOther float");
  var x4 = "unequalString";
  var y4 = "unequalstring";
  testAssert(x4,y4,"lessOther Strings");
}

proc LessRange() {
  var x1 = 1..5;
  var y1 = 1..6;
  testAssert(x1,y1,"greater Range");
}

proc tupleOther() {
  var x1 = (1,2,5);
  var y1 = (1,2,3);
  testAssert(x1,y1,"tuple other");
}

proc complextuple() {
  var x1 = (1,2,3);
  var y1 = 1 + 5i;;
  testAssert(x1,y1,"tuple complex");
}

proc tupleList() {
  var x2 = [1,2,3];
  var y2 = (1,2);
  testAssert(x2,y2,"tuple list different");
  var x1 = (1,2);
  var y1 = [1,2];
  testAssert(x1,y1,"tuple list same length");
}

proc NotLessString() {
  var x = "Just a string";
  var y = "Just a string with space";
  testAssert(x,y,"Unequal length String");

  x = "unequalstring";
  y = "unequalString";
  testAssert(x,y,"Equal Strings");
}
