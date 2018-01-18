class Foo {
  var x: bool;
  var y = if x then 10 else 11;
}

var foo1 = new Foo(true);
var foo2 = new Foo(false);
writeln(foo1);
writeln(foo2);
delete foo1;
delete foo2;
