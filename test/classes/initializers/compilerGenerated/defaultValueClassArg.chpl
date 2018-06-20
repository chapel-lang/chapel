class Foo {
  var x: int;
}

proc anyFunc(yVal = new owned Foo(2)) {
  writeln(yVal);
}

anyFunc();
