class Foo {
  type eltType;
  var args;
  var val: eltType;
  proc Foo(args..., type eltType = args.type) {
    writeln(eltType:string);
    writeln(val);
  }
}

{
  var foo = new Foo(4, 8, 15, 16, 23, 42);
  delete foo;
}

{
  var foo = new Foo(42, 13.0);
  delete foo;
}
