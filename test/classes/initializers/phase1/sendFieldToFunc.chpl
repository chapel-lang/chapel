class Foo {
  var x: bool;

  proc init(xVal) {
    x = xVal;
    bar(x); // This should be allowed
  }
}

proc bar(val) {
  writeln(val);
  return !val;
}

var foo = new borrowed Foo(true);
writeln(foo);
