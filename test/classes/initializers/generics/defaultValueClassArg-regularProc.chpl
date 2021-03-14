class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

proc anyFunc(yVal = new unmanaged Foo(3)) {
  writeln(yVal);
  delete yVal;
}

anyFunc();
