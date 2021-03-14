record Foo {
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    x = xVal;
  }
}

proc anyFunc(yVal = new Foo(3)) {
  writeln(yVal);
}

anyFunc();
