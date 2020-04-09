proc foo(type t) {
  var x : t;
  writeln(x);
}

class C {
  var a : int = 2;
}

record R {
  var b : int = 2;
}

foo(borrowed C?);
foo(R);
