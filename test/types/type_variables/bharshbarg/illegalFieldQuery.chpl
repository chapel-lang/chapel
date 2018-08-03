
class Foo {
  param foo : bool;
  param bar : int;
  var   baz : string;
}

proc foo(x: borrowed Foo(?a, ?b, ?c)) {
  writeln(c);
}

var f = new borrowed Foo(true, 5);
foo(f);
