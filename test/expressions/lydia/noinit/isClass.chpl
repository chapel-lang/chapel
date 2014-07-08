class Foo {
  var bar: int;
  var baz: bool;

}

var foo:Foo = noinit;
foo = new Foo(4, true);
writeln(foo);
