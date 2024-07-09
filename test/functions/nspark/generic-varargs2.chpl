class Foo {
  type eltType;
  var args;
  var val: eltType;
  proc init(args..., type eltType = args.type) {
    this.eltType = eltType;
    this.args = args;
    init this;
    writeln(eltType:string);
    writeln(val);
  }
}

{
  var foo = new unmanaged Foo(4, 8, 15, 16, 23, 42);
  delete foo;
}

{
  var foo = new unmanaged Foo(42, 13.0);
  delete foo;
}
