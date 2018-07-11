class Foo {
  var x: bool;

  proc init(xVal) {
    x = xVal;
    bar(x); // This should not be allowed
  }
}

proc bar(ref val) {
  val = !val;
}

var foo = new borrowed Foo(true);
writeln(foo);
