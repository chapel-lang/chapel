class Foo {
  var x: bool;
  var y = if x then 10 else 11;

  proc init(xVal: bool) {
    x = xVal;
  }
}

var foo1 = new unmanaged Foo(true);
var foo2 = new unmanaged Foo(false);
writeln(foo1);
writeln(foo2);
delete foo1;
delete foo2;
