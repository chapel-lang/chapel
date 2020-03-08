use UnitTest;
proc testAssert(x,y,reason="") {
  var sep = "=="*40;
  var test = new Test();
  try {
    test.assertGreaterThan(x,y);
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
  greaterOther();
  greaterRange();
  tupleOther();
  tupleList();
  NotGreaterString();
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

  var x6: [0..2] int = [2,4,8];
  var y6: [0..2] int = [1,2,4];
  testAssert(x6, y6,"0-based arrays");

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

proc greaterOther() {
  var x = [1,2,5];
  var y = [1,2,3];
  testAssert(x,y,"greaterOther 1D array");
  var x2 = (1,2,5);
  var y2 = (1,2,3);
  testAssert(x2,y2,"greaterOther tuples");
  var x3 = 2.5;
  var y3 = 2;
  testAssert(x3,y3,"greaterOther float");
  var x4 = "unequalstring";
  var y4 = "unequalString";
  testAssert(x4,y4,"greaterOther Strings");
}

proc greaterRange() {
  var x1 = 1..5;
  var y1 = 1..6;
  testAssert(x1,y1,"greater Range");
}

proc tupleOther() {
  var x1 = (1,2,3);
  var y1 = (1,2,5);
  testAssert(x1,y1,"tuple other");
}

proc tupleList() {
  var x2 = [1,2,3];
  var y2 = (1,2);
  testAssert(x2,y2,"tuple list");
}

proc NotGreaterString() {
  var x = "Just a string";
  var y = "Just a string with space";
  testAssert(x,y,"Unequal length String");

  x = "unequalString";
  y = "unequalstring";
  testAssert(x,y,"Equal Strings");
}
