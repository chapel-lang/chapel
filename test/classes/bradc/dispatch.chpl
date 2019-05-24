class C {
  var x: int;
}

class D {
  var y: unmanaged C?;
}

proc foo(c: borrowed C) {
  writeln("x is: ", c.x);
}

proc foo(d: borrowed D) {
  foo(d.y!);
}

proc main() {
  var myC = new unmanaged C(x=1);
  foo(myC);
  var myD = new unmanaged D();
  myD.y = new unmanaged C();
  myD.y!.x = 2;
  foo(myD);
  delete myC;
  delete myD.y;
  delete myD;
}
