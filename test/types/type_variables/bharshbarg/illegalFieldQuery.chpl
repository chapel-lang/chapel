
class Foo {
  param foo : bool;
  param bar : int;
  var   baz : string;
}

proc foo(x: Foo(?a, ?b, ?c)) {
  writeln(c);
}

var f = new Foo(true, 5);
foo(f);
