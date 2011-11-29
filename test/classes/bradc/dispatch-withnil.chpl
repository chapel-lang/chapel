class C {
  var x: int;
}

class D {
  var y: C;
}

proc foo(c: C) {
  writeln("x is: ", c.x);
}

proc foo(d: D) {
  foo(d.y);
}

proc foo(n: _nilType) {
  writeln("foo() was passed a nil instance");
}

proc main() {
  var myC = new C(x=1);
  foo(myC);
  var myD = new D();
  myD.y = new C();
  myD.y.x = 2;
  foo(myD);
  delete myC;
  delete myD.y;
  delete myD;
}
