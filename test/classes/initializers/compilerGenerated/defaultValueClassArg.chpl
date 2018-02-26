class Foo {
  var x: int;
}

proc anyFunc(yVal = new Foo(2)) {
  writeln(yVal);
}

anyFunc();
