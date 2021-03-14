// Exercises using a where clause when the signature is otherwise identical
record Foo {
  var x: int;

  proc init(param xVal: int) where (xVal > 10) {
    x = xVal;
  }

  proc init(param xVal: int) where (xVal <= 10) {
    x = abs(xVal) + 10;
  }
}

var f1 = new Foo(16);
var f2 = new Foo(3);
writeln(f1);
writeln(f2);
