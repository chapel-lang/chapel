class C {
  param x: int;
}

class D {
  var c: unmanaged C = new unmanaged C();
  param y: int = c.x;
}

var c = new unmanaged C(2);

var d = new unmanaged D(c);

writeln("c is: ", c);
writeln("d is: ", d);
