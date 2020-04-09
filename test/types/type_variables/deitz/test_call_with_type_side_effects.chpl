class C {
  type t;
  var x: t;
  proc postinit() {
    writeln("side effect ", x);
  }
}

proc foo(type t) {
  var x: t;
  writeln(x);
}

foo(borrowed C(real)?);
