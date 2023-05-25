class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

proc foo(c: borrowed C(tt=?tt, t=?t)) {
  var y: t;
  var yy: tt;
  writeln((y, yy));
}

var c = (new owned C(int, real)).borrow();
writeln(c);
foo(c);
