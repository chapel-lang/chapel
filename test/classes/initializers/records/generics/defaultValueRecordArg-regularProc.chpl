record Foo {
  var x;

  proc init(xVal) where !xVal: Foo {
    x = xVal;
    super.init();
  }
}

proc anyFunc(yVal = new Foo(3)) {
  writeln(yVal);
}

anyFunc();
