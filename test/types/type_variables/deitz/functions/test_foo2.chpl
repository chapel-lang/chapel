function foo(type t) {
  var x : t;
  writeln(x);
}

class C {
  var a : integer = 2;
}

record R {
  var b : integer = 2;
}

foo(C);
foo(R);
