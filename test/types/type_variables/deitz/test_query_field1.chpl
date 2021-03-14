class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

proc foo(c: unmanaged C(?t, ?tt)) {
  var y: t;
  var yy: tt;
  writeln((y, yy));
}

var c = new unmanaged C(int, real);
writeln(c);
foo(c);
delete c;
