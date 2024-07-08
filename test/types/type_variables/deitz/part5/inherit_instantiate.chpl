class C {
  var x : int;
}

class D : C {
  type t;
  var y : t;
}

var dOwn = new owned D(t=real);
var d = dOwn.borrow();

writeln(d);
