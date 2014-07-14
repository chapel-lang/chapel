record foo {
  type t;
  var i: t;
}
var bar: foo(int) = noinit;
var baz: foo(int);
baz.i = 7;
bar = baz;
writeln(bar);
