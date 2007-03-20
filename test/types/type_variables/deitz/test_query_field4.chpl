class C {
  type t;
  type tt;
  var x: t;
  var xx: tt;
}

def foo(c: C(int, ?tt)) {
  var y: int;
  var yy: tt;
  writeln((y, yy));
  writeln(c);
}

def foo(c: C(real, ?tt)) {
  var y: real;
  var yy: tt;
  writeln((y, yy));
  writeln(c);
}

foo(C(int, real));
foo(C(real, int));
