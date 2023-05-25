class C {
  var x : int;
}

class D : C {
  type t;
  var y : t;
}

var d = (new owned D(t=real)).borrow();

writeln(d);
