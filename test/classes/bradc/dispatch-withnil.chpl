class C {
  var x: int;
}

class D {
  var y: borrowed C?;
}

proc foo(c: borrowed C) {
  writeln("x is: ", c.x);
}

proc foo(d: borrowed D) {
  foo(d.y!);
}

proc foo(n: _nilType) {
  writeln("foo() was passed a nil instance");
}

proc main() {
  var myC = new borrowed C(x=1);
  foo(myC);
  var myD = new borrowed D();
  var otherC = new C();
  myD.y = otherC.borrow();
  myD.y!.x = 2;
  foo(myD);
}
