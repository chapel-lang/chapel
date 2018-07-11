class Foo {
  type eltType;
  var args;
  var val: eltType;
  proc init(type eltType, args...) {
    this.eltType = eltType;
    this.args = args;
    this.complete();

    writeln(eltType:string);
    writeln(val);
  }
}

{
  var foo = new owned Foo(6*int, 4, 8, 15, 16, 23, 42);
}

{
  var foo = new unmanaged Foo((int, real), 42, 13.0);
  delete foo;
}
