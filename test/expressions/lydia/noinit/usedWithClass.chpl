class Foo {
  var bar: int;
  var baz: bool;

}

inline proc _defaultOf(type t:unmanaged Foo) {
  writeln("I default initialized!");
  return nil:t;
}



var foo:unmanaged Foo = noinit; // Should not print out message

foo = new unmanaged Foo(4, true);

writeln(foo);

delete foo;



var bam:unmanaged Foo;          // Should print out message

bam = new unmanaged Foo(3, false);

writeln(bam);

delete bam;
