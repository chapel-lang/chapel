record Foo {
  var x;

  proc init(xVal) where !xVal: Foo {
    x = xVal;
  }
}

type t = Foo(int);

proc anyFunc(yVal = new t(3)) {
  writeln(yVal);
}

anyFunc();
