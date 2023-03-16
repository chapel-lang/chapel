class C {
  type t;
  var x : t;
}

class D : C {
  var y : int;
}

var ownD = new owned D(real);
var d = ownD.borrow();

writeln(d);
