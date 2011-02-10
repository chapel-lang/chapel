class C {
  type t;
  var x: t;
  proc initialize() {
    writeln("side effect ", x);
  }
}

proc foo(type t) {
  var x: t;
  writeln(x);
}

foo(C(real));
