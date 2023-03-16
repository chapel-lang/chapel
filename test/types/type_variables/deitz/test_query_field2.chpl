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

var ownC = new owned C(int, real);
var c = ownC.borrow();
writeln(c);
foo(c);
