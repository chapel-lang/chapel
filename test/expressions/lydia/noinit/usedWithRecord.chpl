record Foo {
  var bar: int;
  var baz: bool;
  proc init() {
    writeln("I default initialized!");
  }
}

var foo:Foo = noinit; // Should not print out the message
foo.bar = 4;
foo.baz = true;
writeln(foo);
var bar:Foo;          // Should print out the message
writeln(bar);
