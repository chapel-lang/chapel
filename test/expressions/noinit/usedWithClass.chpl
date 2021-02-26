class Foo {
  var bar: int;
  var baz: bool;

}

var foo:unmanaged Foo? = noinit;

foo = new unmanaged Foo(4, true);

writeln(foo);

delete foo;



var bam:unmanaged Foo?;

bam = new unmanaged Foo(3, false);

writeln(bam);

delete bam;
