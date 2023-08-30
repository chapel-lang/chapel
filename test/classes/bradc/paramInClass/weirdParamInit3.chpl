class C {
  param x: int;
}

class D {
  var cc = new owned C(2);
  var c: borrowed C(?) = cc.borrow();
  param y: int = c.x;
}

var c = new C(2);

var d = new D(c=c);

writeln("c is: ", c);
writeln("d is: ", d);
