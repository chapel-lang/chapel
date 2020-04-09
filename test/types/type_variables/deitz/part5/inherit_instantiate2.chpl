class C {
  type t;
  var x : t;
}

class D : C {
  var y : int;
}

var d = new borrowed D(real);

writeln(d);
