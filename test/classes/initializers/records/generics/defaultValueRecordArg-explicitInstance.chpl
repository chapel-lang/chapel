record Foo {
  type T;
  var x : T;

  proc init(type T, xVal : T) where !isSubtype(xVal.type, Foo) {
    this.T = T;
    x = xVal;
  }
}

type t = Foo(int);

proc anyFunc(yVal = new t(3)) {
  writeln(yVal);
}

anyFunc();
