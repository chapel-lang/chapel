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

foo(new borrowed C(int, real), new borrowed C(int, real));
foo(new borrowed C(real), new borrowed C(real));
foo(new borrowed C(real,int), new borrowed C(real,int));
