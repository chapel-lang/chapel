// Checks the behavior for declaring an argument with the type desired
record Foo {
  type t;
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    t = xVal.type;
    x = xVal;
  }
}

proc takesAFoo(val: Foo(int, int)) {
  writeln(val);
  writeln(val.type: string);
}

var f = new Foo(10);
takesAFoo(f);
