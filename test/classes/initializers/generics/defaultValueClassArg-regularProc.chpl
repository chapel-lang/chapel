class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
    super.init();
  }
}

proc anyFunc(yVal = new Foo(3)) {
  writeln(yVal);
}

anyFunc();
