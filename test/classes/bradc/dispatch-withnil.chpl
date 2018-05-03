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
  var myC = new borrowed C(x=1);
  foo(myC);
  var myD = new borrowed D();
  myD.y = new borrowed C();
  myD.y.x = 2;
  foo(myD);
}
