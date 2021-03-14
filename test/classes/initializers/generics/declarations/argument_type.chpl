// Checks the behavior for declaring an argument with the type desired, when
// type constructors get generated for defined initializers
class Foo {
  type t;
  var x;

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

proc takesAFoo(val: Foo(int)) {
  writeln(val);
  writeln(val.type: string);
}

var f = new Foo(10);
takesAFoo(f);
delete f;
