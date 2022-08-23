class C {
  type t;
  type tt;
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

foo((new owned C(int, real)).borrow(), (new owned C(int, real)).borrow());
foo((new owned C(real, int)).borrow(), (new owned C(real, int)).borrow());
foo((new owned C(real, int)).borrow(), (new owned C(real, real)).borrow());
