record C {
  type t;
  var x: t;
}

class D {
  var c: C(borrowed D?);
}

var d = (new owned D()).borrow();

writeln(d);
