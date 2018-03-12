class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

proc anyFunc(yVal = new Foo(3)) {
  writeln(yVal);
}

anyFunc();
