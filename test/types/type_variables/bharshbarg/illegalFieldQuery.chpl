
class Foo {
  param foo : bool;
  param bar : int;
  var   baz : string;
}

proc foo(x: borrowed Foo(?a, ?b, ?c)) {
  writeln(c);
}
var fObj = new Foo(true, 5);
var f = fObj.borrow();
foo(f);
