class C {
  var x : int;
}

class D : C {
  type t;
  var y : t;
}

var d = new borrowed D(t=real);

writeln(d);
