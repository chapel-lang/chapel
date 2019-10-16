record C {
  type t;
  var x: t;
}

class D {
  var c: C(borrowed D?);
}

var d = new borrowed D();

writeln(d);
