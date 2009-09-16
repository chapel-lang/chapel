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
  delete c;
}

def foo(c: C(real, ?tt)) {
  var y: real;
  var yy: tt;
  writeln((y, yy));
  writeln(c);
  delete c;
}

foo(new C(int, real));
foo(new C(real, int));
