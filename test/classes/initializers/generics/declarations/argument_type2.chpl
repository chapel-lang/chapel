// Checks the behavior for declaring an argument with the type desired
class Foo {
  type t;
  var x;

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

proc takesAFoo(val: borrowed Foo(int, int)) {
  writeln(val);
  writeln(val.type: string);
}

var f = new unmanaged Foo(10);
takesAFoo(f);
delete f;
