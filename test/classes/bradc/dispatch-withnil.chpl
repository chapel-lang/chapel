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
  var ownMyC = new owned C(x=1);
  var myC = ownMyC.borrow();
  foo(myC);
  var ownMyD = new owned D();
  var myD = ownMyD.borrow();
  var otherC = new C();
  myD.y = otherC.borrow();
  myD.y!.x = 2;
  foo(myD);
}
