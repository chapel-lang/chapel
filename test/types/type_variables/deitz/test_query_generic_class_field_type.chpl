class C {
  var x;
}

var c: unmanaged C(int) = new unmanaged C(2);

proc f(c: C(?t)) {
  var x: t;
  writeln(x);
  writeln(c);
}

f(c);

delete c;
