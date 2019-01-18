record Foo {
  var x: int;

  proc init(xVal) where xVal.type != Foo {
    x = xVal;
  }
}

proc anyFunc(yVal = new Foo(3)) {
  writeln(yVal);
}

anyFunc();
