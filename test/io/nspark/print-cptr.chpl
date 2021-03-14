use CPtr;
record Foo {
  var mem = c_nil:c_ptr(uint(64));
}

record Bar {
  var mem:c_void_ptr = c_nil;
}

writeln("c_nil = ", c_nil:c_void_ptr);

var foo: Foo;
writeln("foo = ", foo);
foo = new Foo(mem=c_malloc(uint(64), 256));
writeln("foo = ", foo);

var bar: Bar;
writeln("bar = ", bar);
bar = new Bar(mem=c_malloc(uint(64), 256):c_void_ptr);
writeln("bar = ", bar);

c_free(foo.mem);
c_free(bar.mem);
