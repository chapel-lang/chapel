record Foo {
  var bar: int;
  var baz: bool;

}

var foo:Foo = noinit;
foo.bar = 4;
foo.baz = true;
writeln(foo);
