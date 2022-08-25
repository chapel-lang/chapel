class C {
  type t;
  var x : t;
}

class D : C {
  var y : int;
}

var d = (new owned D(real)).borrow();

writeln(d);
