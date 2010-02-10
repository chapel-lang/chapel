class C {
  var x;
}

var c: C(int) = new C(2);

def f(c: C(?t)) {
  var x: t;
  writeln(x);
  writeln(c);
}

f(c);
