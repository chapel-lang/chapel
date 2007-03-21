class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

def foo(c: C(int, ?tt) ...?k) {
  var y: int;
  var yy: tt(1);
  writeln((y, yy));
  writeln(c);
}

def foo(c: C(real, ?tt) ...?k) {
  var y: real;
  var yy: tt(1);
  writeln((y, yy));
  writeln(c);
}

foo(C(int, real), C(int, real));
foo(C(real, int), C(real, int));
foo(C(real, int), C(real, real));
