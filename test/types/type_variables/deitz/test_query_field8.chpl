class C {
  type t = int;
  type tt = real;
  var x: t;
  var xx: tt;
}

proc foo(c: C(int, ?tt) ...?k) {
  var y: int;
  var yy: tt(1);
  writeln((y, yy));
  writeln(c);
  delete c(1);
  delete c(2);
}

proc foo(c: C(real, ?tt) ...?k) {
  var y: real;
  var yy: tt(1);
  writeln((y, yy));
  writeln(c);
  delete c(1);
  delete c(2);
}

foo(new C(int, real), new C(int, real));
foo(new C(real), new C(real));
foo(new C(real,int), new C(real,int));
