class C {
  type t = int;
  type tt = real;
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

foo(new C(int, real), new C(int, real));
foo(new C(real), new C(real));
foo(new C(real,int), new C(real,int));
