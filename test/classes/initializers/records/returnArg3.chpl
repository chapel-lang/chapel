record Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
  }
}

proc ident(x: Foo) return x;

var foo1 = new Foo(13);
var foo2 = ident(foo1);
writeln(foo1);
writeln(foo2);
