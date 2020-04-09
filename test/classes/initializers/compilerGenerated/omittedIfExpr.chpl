class Foo {
  var x: bool;
  var y = if x then 10 else 11;
}

var foo1 = new shared Foo(true);
var foo2 = new shared Foo(false);
writeln(foo1);
writeln(foo2);
