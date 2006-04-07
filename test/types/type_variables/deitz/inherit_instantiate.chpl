class C {
  var x : int;
}

class D : C {
  type t;
  var y : t;
}

var d = D(float);

writeln(d);
