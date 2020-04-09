class C {
  var x: real;
}

class D: C {
  var x: int;
}

var d = new unmanaged D();

writeln(d.x);

delete d;

