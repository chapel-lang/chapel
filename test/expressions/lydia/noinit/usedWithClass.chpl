class Foo {
  var bar: int;
  var baz: bool;

}

inline proc _defaultOf(type t) where t == Foo {
  writeln("I default initialized!");
  return nil:t;
}

var foo:Foo = noinit; // Should not print out message
foo = new Foo(4, true);
writeln(foo);
var bam:Foo;          // Should print out message
bam = new Foo(3, false);
writeln(bam);
