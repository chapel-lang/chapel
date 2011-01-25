class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

proc foo(c: C(tt=?tt, t=?t)) {
  var y: t;
  var yy: tt;
  writeln((y, yy));
}

var c = new C(int, real);
writeln(c);
foo(c);
delete c;
