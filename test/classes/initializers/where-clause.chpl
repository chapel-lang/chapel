// Exercises using a where clause when the signature is otherwise identical
class Foo {
  var x: int;

  proc init(param xVal: int) where (xVal > 10) {
    x = xVal;
  }

  proc init(param xVal: int) where (xVal <= 10) {
    x = abs(xVal) + 10;
  }
}

var f1 = new unmanaged Foo(16);
var f2 = new unmanaged Foo(3);
writeln(f1);
writeln(f2);
delete f1;
delete f2;
