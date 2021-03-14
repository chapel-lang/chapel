class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

proc foo(c: borrowed C(int, ?tt)) {
  var y: int;
  var yy: tt;
  writeln((y, yy));
  writeln(c);

}

proc foo(c: borrowed C(real, ?tt)) {
  var y: real;
  var yy: tt;
  writeln((y, yy));
  writeln(c);

}

foo(new borrowed C(int, real));
foo(new borrowed C(real, int));
