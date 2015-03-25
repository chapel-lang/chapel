record Foo {
  var bar: int;
  var baz: bool;

}

proc _defaultOf(type t) where t == Foo {
  writeln("I default initialized!");
  var res:t = noinit;
  // The following could be replaced with a call to the _defaultOf for those
  // respective types.  But it isn't important.
  res.bar = 0;
  res.baz = false;
  return res;
}

var foo:Foo = noinit; // Should not print out the message
foo.bar = 4;
foo.baz = true;
writeln(foo);
var bar:Foo;          // Should print out the message
writeln(bar);
