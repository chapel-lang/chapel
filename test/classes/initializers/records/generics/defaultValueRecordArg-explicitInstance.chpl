record Foo {
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    x = xVal;
  }
}

type t = Foo(int);

proc anyFunc(yVal = new t(3)) {
  writeln(yVal);
}

anyFunc();
