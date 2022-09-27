
class Foo {
  param foo : bool;
  param bar : int;
  var   baz : string;
}

proc foo(x: borrowed Foo(?a, ?b, ?c)) {
  writeln(c);
}

var f = (new owned Foo(true, 5)).borrow();
foo(f);
