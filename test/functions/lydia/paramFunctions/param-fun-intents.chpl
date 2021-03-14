proc foo (out x: int, out y: real) {
  x = 7;
  y = 3.14;
  return true;
}

proc foo2 (out x: int, out y: real) param {
  x = 7;
  y = 3.14;
  return true;
}

proc foo3 (inout x: int) param {
  return true;
}

proc foo4 (out x: int) type {
  return true;
}

proc foo5 (in x: int) type {
  return true;
}


var testFooX: int;
var testFooY: real;
foo(testFooX, testFooY);
writeln(testFooX+testFooY);
var testFoo2X: int;
var testFoo2Y: real;
foo2(testFoo2X, testFoo2Y);
writeln(testFoo2X+testFoo2Y);

var x = 1;
foo3(x);
foo4(x);
foo5(x);
