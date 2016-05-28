class Foo {
  type eltType;
  var args;
  var val: eltType;
  proc Foo(type eltType, args...) {
    writeln(eltType:string);
    writeln(val);
  }
}

{
  var foo = new Foo(6*int, 4, 8, 15, 16, 23, 42);
  delete foo;
}

{
  var foo = new Foo((int, real), 42, 13.0);
  delete foo;
}
