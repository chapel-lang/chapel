record C {
  type t;
  var x: t;
}

class D {
  var c: C(borrowed D?);
}

var ownD = new owned D();
var d = ownD.borrow();

writeln(d);
