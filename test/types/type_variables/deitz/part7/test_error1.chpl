class C {
  type t;
  var x : t;
}

var c = new C(int);

writeln(c);

proc foo(i : int) {
  writeln(i);
}

foo(c);
