record aRecord {
  type t;
  var namingIsHard: t;
}

record foo {
  type t;
  var baz: aRecord(t);
}
var bar: foo(int(64)) = noinit;
bar.baz = new aRecord(int(64));
writeln(bar);
