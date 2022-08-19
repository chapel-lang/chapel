class C {
  type t = int;
  type tt = real;
  var x: t;
  var xx: tt;
}

proc foo(c: borrowed C(int, ?tt) ...?k) {
  var y: int;
  var yy: tt;
  writeln((y, yy));
  writeln(c);
}

proc foo(c: borrowed C(real, ?tt) ...?k) {
  var y: real;
  var yy: tt;
  writeln((y, yy));
  writeln(c);
}

foo((new owned C(int, real), new borrowed C(int, real))).borrow();
foo((new owned C(real), new borrowed C(real))).borrow();
foo((new owned C(real,int), new borrowed C(real,int))).borrow();
