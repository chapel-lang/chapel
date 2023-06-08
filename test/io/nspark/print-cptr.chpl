use CTypes;
record Foo {
  var mem = c_nil:c_ptr(uint(64));
}

record Bar {
  var mem:c_void_ptr = c_nil;
}

writeln("c_nil = ", c_nil:c_void_ptr);

var foo: Foo;
writeln("foo = ", foo);
foo = new Foo(mem=allocate(uint(64), 256));
writeln("foo = ", foo);

var bar: Bar;
writeln("bar = ", bar);
bar = new Bar(mem=allocate(uint(64), 256):c_void_ptr);
writeln("bar = ", bar);

deallocate(foo.mem);
deallocate(bar.mem);
