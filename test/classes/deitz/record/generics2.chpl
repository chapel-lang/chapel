record C {
  type t;
  var x: t;
}

class D {
  var c: C(D);
}

var d = new borrowed D();

writeln(d);
